#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUTPUT_PATH="${COREWAR_REPLAY_OUT:-$ROOT_DIR/web/latest_replay.json}"
TMP_DIR="$(mktemp -d)"
ARGS=()

cleanup() {
  rm -rf "$TMP_DIR"
}

trap cleanup EXIT

if [[ "$#" -gt 0 && "$1" == *.json ]]; then
  OUTPUT_PATH="$1"
  shift
fi

if [[ "$#" -eq 0 ]]; then
  set -- debug/zork.s debug/simple.s
fi

mkdir -p "$(dirname "$OUTPUT_PATH")"

cd "$ROOT_DIR"
make >/dev/null

for input_path in "$@"; do
  case "$input_path" in
    *.s)
      name="$(basename "${input_path%.s}")"
      output_cor="$TMP_DIR/${name}.cor"
      ./asm/asm "$input_path" "$output_cor" >/dev/null
      ARGS+=("$output_cor")
      ;;
    *.cor)
      ARGS+=("$input_path")
      ;;
    *)
      printf 'Unsupported replay input: %s\n' "$input_path" >&2
      exit 1
      ;;
  esac
done

if [[ "${#ARGS[@]}" -lt 2 ]]; then
  printf 'Need at least two champions to generate a replay.\n' >&2
  exit 1
fi

COREWAR_TRACE_FILE="$OUTPUT_PATH" COREWAR_TRACE_EVERY="${COREWAR_REPLAY_EVERY:-1}" \
  ./corewar/corewar "${ARGS[@]}"

printf 'Replay written to %s\n' "$OUTPUT_PATH"
