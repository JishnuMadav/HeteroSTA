# Design 1: Tiny TPU

## Files

- `input/`: physical gate-level Verilog, extracted SPEF, original SDC, HeteroSTA-compatible SDC, all five EARLY (FF) and five LATE (SS) ASAP7 RVT Liberty files, and original input documentation/checksums.
- `run_my_design.cpp`: CPU driver; reads `input/` and writes `output/`.
- `output/`: saved `output.rpt`, `output.sdf`, and `run.log` copied from the successful VM run.
- `build.sh`, `run.sh`: build and run from any working directory.
- `checksums.sha256`: hashes of packaged inputs, saved outputs, driver, and scripts.

`input/load_inputs.tcl` is an OpenSTA helper, not the HeteroSTA entry point. `input/manifest.json` preserves the original delivery provenance. The compatible SDC and saved outputs are covered by `checksums.sha256`.

## Build and run

Requirements: Linux x86-64, C++17 compiler, Git LFS, the HeteroSTA SDK, and a valid HeteroSTA license. The SDK must contain `include/heterosta.h`, `include/netlistdb.h`, and `lib/libheterosta.cpu.so`.

```bash
cd 'heterosta2D/design 1'
export HETEROSTA_ROOT="$HOME/heterosta"
bash build.sh
read -rsp 'HeteroSTA license: ' HeteroSTA_Lic; echo
export HeteroSTA_Lic
bash run.sh
```

`HETEROSTA_ROOT` defaults to `$HOME/heterosta`. The top module is selected automatically; optionally export `HeteroSTA_Top=tpu` to select it explicitly. The license is read from the environment, never stored in the driver. Running again overwrites the saved files under `output/`. The compiled executable is generated under ignored `build/`.

## Saved results and assumptions

The saved report header is dated 2026-11-07. These are archived results from the VM's original `mydesign` working directory; log path messages retain that original location. They were not regenerated merely to package this repository.

- Top module: `tpu`.
- Library corners: ASAP7 RVT FF for EARLY and SS for LATE.
- One extracted SPEF is shared by both corners, not separate min/max RC datasets.
- CPU execution, Elmore delay model, propagated `core_clk`.
- Clock period: 1000 ps (1 GHz); setup uncertainty: 50 ps.
- Setup WNS: approximately -1250.59 ps.
- Setup TNS: approximately -1.61033e+06 ps.
- Report contains up to 100 setup paths. This driver does not generate a hold report.

The compatibility SDC joins the reset false-path command onto one line and comments out the unsupported ideal-clock `set_clock_transition` command. It retains propagated-clock analysis and does not substitute a 20 ps input-port slew. Input slews are initialized to zero before the SDC is read.

The recorded run warns about unsupported Liberty checks (`min_pulse_width`, `non_seq_hold_rising`) and internal-pin handling. Coupling capacitances are converted to grounded capacitances without signal-integrity analysis. These results do not establish signoff or timing closure.

## Provenance

Inputs and saved outputs were copied unchanged from `/home/jishnu/heterosta/mydesign` and its `results/heterosta` directory. The original input manifest records upstream source paths and SHA-256 hashes. License/copyright notices embedded in the ASAP7 Liberty files are retained. Obtain HeteroSTA separately from https://heterosta.pkueda.org.cn/.

Verify packaged files before rerunning (new results will change their hashes):

```bash
sha256sum -c checksums.sha256
```
