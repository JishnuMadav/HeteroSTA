#!/usr/bin/env bash
set -euo pipefail
case_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
sdk="${HETEROSTA_ROOT:-$HOME/heterosta}"
if [[ ! -f "$sdk/include/heterosta.h" || ! -f "$sdk/lib/libheterosta.cpu.so" ]]; then
    printf 'Set HETEROSTA_ROOT to the extracted HeteroSTA SDK directory.\n' >&2
    exit 1
fi
mkdir -p "$case_dir/build"
"${CXX:-g++}" -std=c++17 -O2 -Wall -Wextra \
    -I"$sdk/include" "$case_dir/run_my_design.cpp" \
    -L"$sdk/lib" -Wl,-rpath,"$sdk/lib" \
    -lheterosta.cpu -lgomp -lrt -pthread -ldl \
    -o "$case_dir/build/run_my_design"
printf 'Built %s\n' "$case_dir/build/run_my_design"
