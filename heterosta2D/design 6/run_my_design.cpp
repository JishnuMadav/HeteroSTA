#include "heterosta.h"
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

extern "C" void logger(uint8_t, const char* message) {
    std::cerr << message << '\n';
}

int main(int argc, char** argv) {
    if (argc != 5) {
        std::cerr << "Usage: run_2d INPUT_DIRECTORY LIBRARY_LIST TOP OUTPUT_DIRECTORY\n";
        return 2;
    }
    namespace fs = std::filesystem;
    const fs::path input(argv[1]), output(argv[4]);
    fs::create_directories(output);
    auto begin = std::chrono::steady_clock::now();
    const char* license = std::getenv("HeteroSTA_Lic");
    if (!license || !heterosta_init_license(license)) {
        std::cerr << "Missing or rejected HeteroSTA license\n";
        return 3;
    }
    heterosta_init_logger(logger);
    std::unique_ptr<STAHoldings, decltype(&heterosta_free)> owner(heterosta_new(), heterosta_free);
    auto* sta = owner.get();
    if (!sta) return 4;
    heterosta_set_delay_calculator_elmore(sta);
    // Load standard cells and every used SRAM model for both corners.
    std::ifstream libraries(argv[2]);
    std::string lib;
    size_t loaded_libraries = 0;
    while (std::getline(libraries, lib)) {
        if (lib.empty()) continue;
        fs::path library_path(lib);
        if (library_path.is_relative()) library_path = fs::path(argv[2]).parent_path() / library_path;
        if (!heterosta_read_liberty(sta, EARLY, library_path.c_str()) ||
            !heterosta_read_liberty(sta, LATE, library_path.c_str())) return 5;
        ++loaded_libraries;
    }
    if (!loaded_libraries) return 5;
    if (!heterosta_read_netlist(sta, (input / "design.v").c_str(), argv[3])) return 6;
    if (!heterosta_read_spef(sta, (input / "design.spef").c_str())) return 7;
    heterosta_flatten_all(sta);
    heterosta_build_graph(sta);
    heterosta_zero_slew(sta);
    if (!heterosta_read_sdc(sta, (input / "design.sdc").c_str())) return 8;
    auto loaded = std::chrono::steady_clock::now();
    heterosta_update_delay(sta, false);
    heterosta_update_arrivals(sta, false);
    auto computed = std::chrono::steady_clock::now();
    float sw, st, hw, ht;
    if (!heterosta_report_wns_tns_max(sta, &sw, &st, false) ||
        !heterosta_report_wns_tns_min(sta, &hw, &ht, false)) return 9;
    if (!std::isfinite(sw) || !std::isfinite(st) || !std::isfinite(hw) || !std::isfinite(ht)) return 10;
    heterosta_dump_paths_max_to_file(sta, 100, 1, (output / "setup.rpt").c_str(), false);
    heterosta_dump_paths_min_to_file(sta, 100, 1, (output / "hold.rpt").c_str(), false);
    if (!heterosta_report_delay_sdf(sta, (output / "delays.sdf").c_str())) return 11;
    auto seconds = [](auto a, auto b) { return std::chrono::duration<double>(b-a).count(); };
    std::ofstream summary(output / "timing.json");
    summary.precision(10);
    summary << "{\n  \"setup_wns\": " << sw << ",\n  \"setup_tns\": " << st
            << ",\n  \"hold_wns\": " << hw << ",\n  \"hold_tns\": " << ht
            << ",\n  \"time_unit\": \"ps\",\n  \"library_corner\": \"typical for EARLY and LATE\","
            << "\n  \"load_seconds\": " << seconds(begin, loaded)
            << ",\n  \"compute_seconds\": " << seconds(loaded, computed) << "\n}\n";
    std::cout << "Setup WNS=" << sw << " TNS=" << st << "\nHold WNS=" << hw << " TNS=" << ht << '\n';
    return summary ? 0 : 12;
}
