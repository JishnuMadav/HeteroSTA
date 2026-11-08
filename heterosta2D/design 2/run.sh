#!/usr/bin/env bash
set -euo pipefail
case_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$case_dir"
: "${HeteroSTA_Lic:?Export your HeteroSTA_Lic license key before running}"
if [[ ! -x build/run_my_design ]]; then
    printf 'Run bash build.sh first.\n' >&2
    exit 1
fi
output_dir="${HETEROSTA_OUTPUT_DIR:-$case_dir/output}"
mkdir -p "$output_dir"
export OMP_NUM_THREADS="${OMP_NUM_THREADS:-8}"
export RAYON_NUM_THREADS="${RAYON_NUM_THREADS:-8}"
./build/run_my_design "$case_dir/input" "$case_dir/input/libraries.txt" "ariane" "$output_dir" 2>&1 | tee "$output_dir/run.log"
