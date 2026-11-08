#include "heterosta.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

extern "C" void log_message(uint8_t level, const char* message) {
    (level <= 2 ? std::cerr : std::cout) << message << '\n';
}

int main() {
    // Run from this design directory; run.sh selects it automatically.
    const std::string input = "input/";
    const char* early[] = {
        "early_asap7sc7p5t_AO_RVT_FF_nldm_211120.lib",
        "early_asap7sc7p5t_INVBUF_RVT_FF_nldm_220122.lib",
        "early_asap7sc7p5t_OA_RVT_FF_nldm_211120.lib",
        "early_asap7sc7p5t_SEQ_RVT_FF_nldm_220123.lib",
        "early_asap7sc7p5t_SIMPLE_RVT_FF_nldm_211120.lib"
    };
    const char* late[] = {
        "late_asap7sc7p5t_AO_RVT_SS_nldm_211120.lib",
        "late_asap7sc7p5t_INVBUF_RVT_SS_nldm_220122.lib",
        "late_asap7sc7p5t_OA_RVT_SS_nldm_211120.lib",
        "late_asap7sc7p5t_SEQ_RVT_SS_nldm_220123.lib",
        "late_asap7sc7p5t_SIMPLE_RVT_SS_nldm_211120.lib"
    };

    auto exists = [&](const char* name) {
        if (std::ifstream(input + name).good()) return true;
        std::cerr << "Missing or unreadable input: " << input + name << '\n';
        return false;
    };
    for (const char* name : early) if (!exists(name)) return 1;
    for (const char* name : late) if (!exists(name)) return 1;
    for (const char* name : {"design.v", "design.spef", "design_heterosta.sdc"})
        if (!exists(name)) return 1;

    const char* license = std::getenv("HeteroSTA_Lic");
    if (!license || !*license) {
        std::cerr << "Set HeteroSTA_Lic to your license key first.\n";
        return 1;
    }
    if (!heterosta_init_license(license)) {
        std::cerr << "License initialization failed.\n";
        return 1;
    }
    heterosta_init_logger(log_message);
    std::unique_ptr<STAHoldings, decltype(&heterosta_free)>
        holder(heterosta_new(), heterosta_free);
    STAHoldings* sta = holder.get();
    if (!sta) {
        std::cerr << "Could not create the STA environment.\n";
        return 1;
    }
    heterosta_set_delay_calculator_elmore(sta);

    auto load_library = [&](TimerEarlyOrLate corner, const char* name) {
        std::cout << "Loading " << name << std::endl;
        if (heterosta_read_liberty(sta, corner, (input + name).c_str()))
            return true;
        std::cerr << "Library loading failed: " << name << '\n';
        return false;
    };
    for (const char* name : early)
        if (!load_library(EARLY, name)) return 1;
    for (const char* name : late)
        if (!load_library(LATE, name)) return 1;

    // Unset HeteroSTA_Top means automatic top-module selection, as in the demo.
    const char* top = std::getenv("HeteroSTA_Top");
    if (top && !*top) top = nullptr;
    if (!heterosta_read_netlist(sta, (input + "design.v").c_str(), top)) {
        std::cerr << "Netlist loading failed. Check cell names and top module.\n";
        return 1;
    }
    if (!heterosta_read_spef(sta, (input + "design.spef").c_str())) {
        std::cerr << "SPEF loading failed.\n";
        return 1;
    }
    heterosta_flatten_all(sta);
    heterosta_build_graph(sta);
    heterosta_zero_slew(sta);
    if (!heterosta_read_sdc(sta, (input + "design_heterosta.sdc").c_str())) {
        std::cerr << "SDC loading failed.\n";
        return 1;
    }
    heterosta_update_delay(sta, false);
    heterosta_update_arrivals(sta, false);

    float wns = 0.0f, tns = 0.0f;
    if (!heterosta_report_wns_tns_max(sta, &wns, &tns, false)) {
        std::cerr << "Setup summary generation failed.\n";
        return 1;
    }
    std::cout << "Setup WNS: " << wns << "\nSetup TNS: " << tns << '\n';
    heterosta_dump_paths_max_to_file(sta, 100, 100,
                                    "output/output.rpt", false);
    if (!heterosta_report_delay_sdf(sta, "output/output.sdf")) {
        std::cerr << "SDF generation failed.\n";
        return 1;
    }
    if (!std::ifstream("output/output.rpt").good()) {
        std::cerr << "Timing report was not created.\n";
        return 1;
    }
    std::cout << "Reports written under output/.\n";
    return 0;
}
