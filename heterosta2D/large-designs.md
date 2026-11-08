# Five large routed 2D HeteroSTA runs

Executed on archgen-team-vm on 2026-11-08. These are five routed implementations (two Ariane variants, two BlackParrot variants, and MemPool), not five unrelated processor architectures. Functional counts exclude physical-only filler and tap cells.

| Design | Functional instances | SRAM instances | Clock period(s), ns | Worst setup slack, ps | Worst hold slack, ps | Timing run, s | Peak RAM, GiB |
|---|---:|---:|---|---:|---:|---:|---:|
| ariane133 | 171,480 | 132 | 4.0000 | -62.007 | -1803.433 | 17.32 | 2.69 |
| ariane136 | 181,910 | 136 | 6.0000 | 942.562 | -1904.892 | 15.82 | 2.84 |
| bp | 246,570 | 24 | 6.0000 | -3407.523 | 4.001 | 22.00 | 3.35 |
| bp_multi | 124,378 | 26 | 4.8000 | -4043.812 | 0.969 | 10.05 | 1.74 |
| mempool_group | 158,268 | 0 | 3.0000, 3.0000 | -2152.904 | 8.897 | 16.22 | 2.43 |

All five processes returned zero, loaded all functional cell types, preserved the source timing constraints, and wrote 100 setup and 100 hold paths each. No SDC rejection or fatal log messages were detected. Every multi-pin signal net in the source DEF had a routed wire. For each design the extracted SPEF net count equals the routed-net count. This is routed-net and extraction coverage, not a DRC/LVS or electrical-connectivity signoff certification.

## Accuracy qualification

These are HeteroSTA results under the supplied constraints, not confirmed design timing violations or timing closure. The typical Nangate45 library and supplied FakeRAM SRAM timing models are used for both EARLY and LATE. FakeRAM models are research estimates, not characterized foundry SRAMs. Elmore delay calculation and grounded coupling capacitance are used; no crosstalk or PVT sweep was performed. Logs retain unsupported Liberty checks, internal-pin fallbacks, undriven nets, and dead pins.

A targeted independent check of Ariane133 uses OpenSTA inside OpenROAD with the same final netlist, SDC, SPEF, and Liberty files. Compare its results in runs/IP109/opensta-crosscheck.log with the primary HeteroSTA results above and diagnostics/IP109-arnoldi/timing.json. A large latch hold-slack discrepancy persists with both HeteroSTA delay models. Its worst hold endpoint is a DLL_X2 latch. The exact cause is not established; do not treat the latch hold result as a confirmed silicon/design defect. See runs/IP109/opensta-crosscheck.log and diagnostics/IP109-arnoldi/. Other designs were not independently cross-checked.

Four SDCs are unchanged byte-for-byte. For MemPool, two unsupported set_max_transition -clock_path checks were retained as comments, with exact originals in design.original.sdc and sdc-compatibility.json. Clock-only transition-limit checking is not covered; no setup/hold, clock, IO-delay, uncertainty, or case-analysis constraints were removed. Original clock periods differ across designs. Ariane133 has a clock-only SDC without IO delays; MemPool uses real and virtual clocks. Results cannot be compared as equal clock targets or complete IO coverage. Runtime includes input loading and report/SDF writing; compute_seconds in summary.json measures delay and arrival computation only. Extraction is a separate stage. These are single observed runs on a shared VM with other jobs active, not controlled performance comparisons.

## Sources and preparation

Private Archgen-IPs snapshot: 7f35d67f63368a424dab64c236ac4d91c5219e2a, IP109/IP110/IP111/IP114/IP118. Public upstream PDB snapshot: 9e1e3399b1b707f26fee853bce1ff91ab466ce24, https://github.com/SJTU-YONGFU-RESEARCH-GRP/PDB-Physical-Design-Database. Source DEF parts were downloaded from the recorded URLs and matched to their recorded SHA-256 hashes. Metadata and source hashes are retained.

The source OpenDB schema is newer than the installed OpenROAD reader. Rather than modify those databases, the matching public DEF files were imported with the supplied LEFs. Functional netlists were exported from those exact routed databases. OpenRCX extracted RC using the supplied Nangate45 rcx_patterns.rules, with a 0.1 fF coupling threshold. No placement, routing, or clock constraint edits were made. HeteroSTA missed SPEF names for internal one-bit buses and sparse buses with one connected bit. Internal labels were consistently scalarized in Verilog and SPEF, without changing connections or RC values. Every renamed net is recorded in normalization.json, and original files remain on the VM. Final runs have no unmatched SPEF-net or SPEF-pin warnings. Physical-only empty instances were excluded from timing; the first pilot cleanup is recorded in its audit. Original repository thermal/simulation qualification failures are retained as source evidence; these STA runs do not change those source acceptance statuses.

## Repository layout

The five implementations are in `design 2` through `design 6`, respectively: Ariane133, Ariane136, BlackParrot, BlackParrot multi, and MemPool. Each has self-contained `input/` and archived `output/` folders, a C++ driver, build/run scripts, and checksums. Design 1 is the existing Tiny TPU and is preserved.

See each design README for reproduction. The original full campaign, extraction scripts, raw sources and prior attempts remain on the VM at `~/heterosta_large2d/`. Report paths in this qualification correspond to the relevant design's `output/`; the Ariane Arnoldi diagnostic is `design 2/output/arnoldi-crosscheck/`.

Private repository-derived inputs are published here at the owner's explicit request. The HeteroSTA license key and licensed SDK are excluded. Git LFS is required for the large data files.
