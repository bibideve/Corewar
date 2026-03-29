#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

cd "$ROOT_DIR"

make >/dev/null

cat >"$TMP_DIR/invalid_register.s" <<'EOF'
.name "invalid"
.comment "invalid register"
ld %1,r17
EOF

if ./asm/asm "$TMP_DIR/invalid_register.s" >/dev/null 2>&1; then
  echo "Expected invalid register assembly to fail" >&2
  exit 1
fi

./asm/asm debug/simple.s >/dev/null 2>&1

if ./corewar/corewar -n 1 debug/simple.cor -n 1 debug/simple.cor >/dev/null 2>&1; then
  echo "Expected duplicate player id invocation to fail" >&2
  exit 1
fi

printf 'not-a-corewar-binary' >"$TMP_DIR/not_corewar.cor"
if ./corewar/corewar "$TMP_DIR/not_corewar.cor" debug/simple.cor >/dev/null 2>&1; then
  echo "Expected invalid .cor file invocation to fail" >&2
  exit 1
fi

echo "Spec checks completed successfully."
