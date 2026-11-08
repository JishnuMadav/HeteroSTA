# Design 5: bp_multi (IP114)

Completed CPU HeteroSTA 2D analysis of a routed implementation on 2026-11-08. Functional instances: **124,378**, including 26 SRAM instances; physical filler/tap cells are excluded.

## Files

- `input/`: the exact analyzed Verilog, SDC and extracted SPEF; every required Liberty file; relative library list; provenance, normalization and routing audits; license notices.
- `output/`: saved setup/hold reports (100 paths each), delay SDF, run/extraction logs, resource usage, timing JSON and validation records.
- `run_my_design.cpp`, `build.sh`, `run.sh`: portable CPU runner. The library list resolves relative to `input/`, with no dependency on the original VM folder.
- `checksums.sha256`: hashes of all packaged files except the checksum file itself.

## Build and run

Linux x86-64, C++17, Git LFS, HeteroSTA v1.3 CPU SDK and a private license are required. Download LFS objects before running.

```bash
git lfs pull
cd 'heterosta2D/design 5'
export HETEROSTA_ROOT="$HOME/heterosta"
bash build.sh
read -rsp 'HeteroSTA license: ' HeteroSTA_Lic; echo
export HeteroSTA_Lic
bash run.sh
```

`run.sh` works from any working directory. The top module is `bp_multi_top`. Default output is `output/`; rerunning overwrites its generated reports and run log. Set `HETEROSTA_OUTPUT_DIR=/absolute/new/output` to retain archived outputs. The license and SDK are external and are not committed.

## Archived results

| Measure | Value |
|---|---:|
| Clock period(s), ns | 4.8000 |
| Worst reported setup slack, ps | -4043.812000 |
| Setup WNS / TNS, ps | -4043.812012 / -4043.812012 |
| Worst reported hold slack, ps | 0.968933 |
| Hold WNS / TNS, ps | 0 / 0 |
| Wall time, s | 10.050 |
| Peak RAM, GiB | 1.741 |

These are archived executed results, not rerun measurements made for this publication. Logs and execution records retain their original VM paths and original driver hashes. The packaged driver differs only in resolving portable relative Liberty paths. Wall time includes input loading and report writing, excludes extraction, and was measured once on a shared VM.

## Qualification

The supplied Nangate45 typical and FakeRAM research SRAM libraries are used for both EARLY and LATE. Elmore delay calculation grounds coupling capacitance; there is no crosstalk analysis or fast/slow PVT sweep. Unsupported Liberty checks and any internal-pin or dead-pin warnings remain in the logs. These are research timing results, not signoff or confirmed timing closure.

All multi-pin signal nets had routed wires. OpenRCX produced the SPEF with the supplied Nangate45 rules. Final SPEF coverage equals the routed-net count, with no unmatched SPEF-net/pin warnings. Internal one-bit and sparse bus names were consistently normalized in the Verilog and SPEF; validation proves the recorded netlist changes and identical SPEF bytes outside the name map. This does not constitute DRC/LVS certification.

The SDC is byte-for-byte identical to the source constraints.

Ariane133's latch hold result disagrees substantially with OpenSTA, including after trying HeteroSTA Arnoldi. This remains unresolved; see [the campaign qualification](../large-designs.md). Do not interpret the reported latch hold violations as confirmed design defects. Other designs were not independently cross-checked. Clock periods and IO-constraint coverage differ across designs.

## Provenance and verification

Archgen-IPs `IP114`, commit `7f35d67f63368a424dab64c236ac4d91c5219e2a`, derived from [PDB Physical Design Database](https://github.com/SJTU-YONGFU-RESEARCH-GRP/PDB-Physical-Design-Database) commit `9e1e3399b1b707f26fee853bce1ff91ab466ce24`. Source URLs and hashes are in `input/provenance/`. Upstream PDB attribution and platform/library notices are preserved. These STA runs do not change the source repository's thermal/simulation acceptance status.

```bash
sha256sum -c checksums.sha256
```

Verify before rerunning, because new output files change their hashes.
