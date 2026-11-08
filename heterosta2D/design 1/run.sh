#!/usr/bin/env bash
set -euo pipefail
case_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$case_dir"
: "${HeteroSTA_Lic:?Export your HeteroSTA_Lic license key before running}"
if [[ ! -x build/run_my_design ]]; then
    printf 'Run bash build.sh first.\n' >&2
    exit 1
fi
mkdir -p output
./build/run_my_design 2>&1 | tee output/run.log
