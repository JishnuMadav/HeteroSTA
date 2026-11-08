Tiny TPU timing inputs: top module tpu

All files are in this one folder.
design.v: final physical gate-level netlist
design.sdc: matching final timing constraints
design.spef: matching final extracted parasitics
early_*.lib: five ASAP7 RVT FF NLDM library components
late_*.lib: five ASAP7 RVT SS NLDM library components

Load all five files for each corner; each component covers different cells.
The libraries are decompressed unchanged, not concatenated or renamed to imply a single library.
load_inputs.tcl loads FF for min and SS for max in OpenSTA.
All instantiated netlist cell types were found in both library sets.

The original delivered run used TT. This package provides FF/SS cell corners;
the single SPEF is the existing extracted RC dataset, not separate min/max RC corners.
Packaging and coverage checks do not establish timing closure or signoff.
Source paths and SHA-256 hashes are in manifest.json.
