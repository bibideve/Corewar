#!/usr/bin/env bash

set -eu

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"

cd "$ROOT_DIR"

make

./asm/asm debug/simple.s
./asm/asm debug/zork.s
./asm/asm debug/labels.s

./corewar/corewar -dump 1 simple.cor zork.cor >/tmp/corewar_smoke_dump.txt

echo "Smoke test completed successfully."
