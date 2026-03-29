#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORK_DIR="$(mktemp -d)"
MATCH_TIMEOUT="${CHAMPIONSHIP_TIMEOUT:-2m}"

declare -a CHAMPION_NAMES=()
declare -a CHAMPION_FILES=()
declare -a CHAMPION_POINTS=()
declare -a CHAMPION_WINS=()
declare -a CHAMPION_LOSSES=()
declare -a CHAMPION_UNRESOLVED=()
declare -a CHAMPION_MATCHES=()

cleanup() {
  rm -rf "$WORK_DIR"
}

usage() {
  cat <<'EOF'
Usage: ./scripts/championship.sh [champion.s|champion.cor ...]

Runs a round-robin Corewar championship.

- If no arguments are provided, every assembleable champion in debug/ is entered.
- .s sources are assembled into a temporary .cor file.
- .cor files are used directly.
- Every pair is played twice with reversed load order to limit slot bias.
- Each match is bounded by $CHAMPIONSHIP_TIMEOUT (default: 2m).
- Timeout or missing-winner matches are reported as unresolved, not as draws.
EOF
}

trap cleanup EXIT

if [[ "${1:-}" == "-h" || "${1:-}" == "--help" ]]; then
  usage
  exit 0
fi

assemble_source() {
  local source_path="$1"
  local output_path="$2"

  if ! "$ROOT_DIR/asm/asm" "$source_path" "$output_path" >/dev/null 2>&1; then
    return 1
  fi
  return 0
}

register_champion() {
  local display_name="$1"
  local file_path="$2"

  CHAMPION_NAMES+=("$display_name")
  CHAMPION_FILES+=("$file_path")
  CHAMPION_POINTS+=(0)
  CHAMPION_WINS+=(0)
  CHAMPION_LOSSES+=(0)
  CHAMPION_UNRESOLVED+=(0)
  CHAMPION_MATCHES+=(0)
}

collect_default_champions() {
  local source_path
  local output_path
  local name

  shopt -s nullglob
  for source_path in "$ROOT_DIR"/debug/*.s; do
    name="$(basename "${source_path%.s}")"
    output_path="$WORK_DIR/${name}.cor"
    if assemble_source "$source_path" "$output_path"; then
      register_champion "$name" "$output_path"
    else
      printf 'Skipping %s: assembly failed.\n' "$name" >&2
    fi
  done
  shopt -u nullglob
}

collect_from_args() {
  local input_path
  local absolute_input
  local base_name
  local output_path

  for input_path in "$@"; do
    if [[ ! -f "$input_path" ]]; then
      printf 'Missing champion file: %s\n' "$input_path" >&2
      exit 1
    fi
    absolute_input="$(cd "$(dirname "$input_path")" && pwd)/$(basename "$input_path")"
    case "$absolute_input" in
      *.s)
        base_name="$(basename "${absolute_input%.s}")"
        output_path="$WORK_DIR/${base_name}.cor"
        if ! assemble_source "$absolute_input" "$output_path"; then
          printf 'Failed to assemble %s\n' "$input_path" >&2
          exit 1
        fi
        register_champion "$base_name" "$output_path"
        ;;
      *.cor)
        base_name="$(basename "${absolute_input%.cor}")"
        register_champion "$base_name" "$absolute_input"
        ;;
      *)
        printf 'Unsupported champion file: %s\n' "$input_path" >&2
        exit 1
        ;;
    esac
  done
}

run_match() {
  local first_index="$1"
  local second_index="$2"
  local log_path="$WORK_DIR/match_${first_index}_${second_index}.log"
  local winner_id
  local status
  local first_name="${CHAMPION_NAMES[$first_index]}"
  local second_name="${CHAMPION_NAMES[$second_index]}"

  set +e
  timeout "$MATCH_TIMEOUT" "$ROOT_DIR/corewar/corewar" \
    -n 1 "${CHAMPION_FILES[$first_index]}" \
    -n 2 "${CHAMPION_FILES[$second_index]}" \
    >"$log_path" 2>&1
  status=$?
  set -e

  if [[ "$status" -ne 0 && "$status" -ne 124 ]]; then
    printf 'Match failed: %s vs %s\n' "$first_name" "$second_name" >&2
    sed -n '1,40p' "$log_path" >&2
    exit 1
  fi

  winner_id="$(sed -n 's/^The player \([0-9][0-9]*\)(.*) has won\.$/\1/p' "$log_path" | sed -n '$p')"
  CHAMPION_MATCHES[$first_index]=$((CHAMPION_MATCHES[$first_index] + 1))
  CHAMPION_MATCHES[$second_index]=$((CHAMPION_MATCHES[$second_index] + 1))

  if [[ "$winner_id" == "1" ]]; then
    CHAMPION_WINS[$first_index]=$((CHAMPION_WINS[$first_index] + 1))
    CHAMPION_LOSSES[$second_index]=$((CHAMPION_LOSSES[$second_index] + 1))
    CHAMPION_POINTS[$first_index]=$((CHAMPION_POINTS[$first_index] + 3))
    printf '%s vs %s -> %s\n' "$first_name" "$second_name" "$first_name"
  elif [[ "$winner_id" == "2" ]]; then
    CHAMPION_WINS[$second_index]=$((CHAMPION_WINS[$second_index] + 1))
    CHAMPION_LOSSES[$first_index]=$((CHAMPION_LOSSES[$first_index] + 1))
    CHAMPION_POINTS[$second_index]=$((CHAMPION_POINTS[$second_index] + 3))
    printf '%s vs %s -> %s\n' "$first_name" "$second_name" "$second_name"
  elif [[ "$status" -eq 124 || -z "$winner_id" ]]; then
    CHAMPION_UNRESOLVED[$first_index]=$((CHAMPION_UNRESOLVED[$first_index] + 1))
    CHAMPION_UNRESOLVED[$second_index]=$((CHAMPION_UNRESOLVED[$second_index] + 1))
    printf '%s vs %s -> unresolved (%s)\n' \
      "$first_name" "$second_name" "$MATCH_TIMEOUT"
  else
    printf 'Unexpected winner id "%s" for %s vs %s\n' \
      "$winner_id" "$first_name" "$second_name" >&2
    sed -n '1,40p' "$log_path" >&2
    exit 1
  fi
}

print_standings() {
  local lines=()
  local index

  printf '\nStandings\n'
  printf '%-4s %-18s %-7s %-5s %-10s %-5s %-6s\n' \
    "Rank" "Champion" "Points" "Wins" "Unresolved" "Loss" "Games"

  for index in "${!CHAMPION_NAMES[@]}"; do
    lines+=("$(printf '%010d|%010d|%010d|%010d|%s|%d|%d|%d' \
      "${CHAMPION_POINTS[$index]}" \
      "${CHAMPION_WINS[$index]}" \
      "$((999999999 - CHAMPION_UNRESOLVED[$index]))" \
      "${CHAMPION_UNRESOLVED[$index]}" \
      "${CHAMPION_NAMES[$index]}" \
      "${CHAMPION_LOSSES[$index]}" \
      "${CHAMPION_MATCHES[$index]}" \
      "$index")")
  done

  printf '%s\n' "${lines[@]}" | sort -r | awk -F'|' '
    {
      rank += 1;
      printf "%-4d %-18s %-7d %-5d %-10d %-5d %-6d\n",
        rank, $5, $1 + 0, $2 + 0, $4 + 0, $6 + 0, $7 + 0;
    }
  '
}

main() {
  local i
  local j

  cd "$ROOT_DIR"
  make >/dev/null

  if [[ "$#" -eq 0 ]]; then
    collect_default_champions
  else
    collect_from_args "$@"
  fi

  if [[ "${#CHAMPION_NAMES[@]}" -lt 2 ]]; then
    printf 'Need at least two valid champions for a championship.\n' >&2
    exit 1
  fi

  printf 'Championship entrants:\n'
  for i in "${!CHAMPION_NAMES[@]}"; do
    printf ' - %s\n' "${CHAMPION_NAMES[$i]}"
  done
  printf '\nResults\n'

  for ((i = 0; i < ${#CHAMPION_NAMES[@]}; i++)); do
    for ((j = i + 1; j < ${#CHAMPION_NAMES[@]}; j++)); do
      run_match "$i" "$j"
      run_match "$j" "$i"
    done
  done

  print_standings
}

main "$@"
