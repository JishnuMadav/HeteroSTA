# HeteroSTA design runs

This repository stores design inputs, C++ drivers, and saved timing outputs.

- [heterosta2D / design 1](heterosta2D/design%201/README.md): tiny TPU (`tpu`), ASAP7 RVT FF/SS, CPU Elmore timing analysis.

- [heterosta2D / design 2](heterosta2D/design%202/README.md): ariane133, 171,480 functional instances, routed Nangate45, CPU Elmore.
- [heterosta2D / design 3](heterosta2D/design%203/README.md): ariane136, 181,910 functional instances, routed Nangate45, CPU Elmore.
- [heterosta2D / design 4](heterosta2D/design%204/README.md): bp, 246,570 functional instances, routed Nangate45, CPU Elmore.
- [heterosta2D / design 5](heterosta2D/design%205/README.md): bp_multi, 124,378 functional instances, routed Nangate45, CPU Elmore.
- [heterosta2D / design 6](heterosta2D/design%206/README.md): mempool_group, 158,268 functional instances, routed Nangate45, CPU Elmore.

The five added implementations include two Ariane variants, two BlackParrot variants, and MemPool. See the [comparison and accuracy qualifications](heterosta2D/large-designs.md) and [CSV results](heterosta2D/large-designs-summary.csv). Ariane latch hold results disagree with OpenSTA; two unsupported MemPool clock-slew checks are documented. These are qualified research timing results.

## Download the data

Large Liberty, SPEF, and SDF files use Git LFS. Install Git LFS before cloning:

```bash
git lfs install
git clone https://github.com/JishnuMadav/HeteroSTA.git
cd HeteroSTA
git lfs pull
```

The licensed HeteroSTA SDK and private license key are external dependencies and are not included.
