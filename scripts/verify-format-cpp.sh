#!/usr/bin/env bash
#
# Run clang-format on the source code and fail if it finds any problems.
# This is useful for CI to ensure that the source code is properly formatted.
# See https://clang.llvm.org/docs/ClangFormat.html for more information.
#

set -euo pipefail

## FUNCTIONS

function die() {
  printf 'ERROR: %s\n' "$@" >&2
  exit 1
}

## SCRIPT ENTRYPOINT

readonly source_directories=(src)

readonly verify_mode=${1:-"all"} # 'all', 'staged'

source_files=()
if [[ ${verify_mode} == "all" ]]; then
  # enumerate all source files
  echo "verify-format-cpp.sh: Verifying format of ALL source files..."
  for directory in "${source_directories[@]}"; do
    for file in $(find "$directory" -name '*.h' -o -name '*.cpp'); do
      source_files+=("$file")
    done
  done
elif [[ ${verify_mode} == "staged" ]]; then
  # enumerate source files that have been staged in git
  echo "verify-format-cpp.sh: Verifying format of staged source files..."
  for file in $(git diff --cached --name-only --diff-filter=ACM | grep -E '\.h$|\.cpp$'); do
    source_files+=("$file")
  done
else
  die "Invalid argument for verify_mode: ${verify_mode}"
fi

# verify format for source_files
for file in "${source_files[@]}"; do
  echo "verify-format-cpp.sh: $file"
  clang-format --Werror --dry-run --fail-on-incomplete-format "${file}"
done
