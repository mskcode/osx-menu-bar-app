#!/usr/bin/env bash
#
# Run clang-tidy on the source code.
#
# Notice that clang-tidy must process the whole buildable solution at once /
# every time. It cannot do partial processing of the changed files, so we must
# process all the files at once.
#

set -euo pipefail

## FUNCTIONS

function die() {
  printf 'ERROR: %s\n' "$@" >&2
  exit 1
}

function lint() {
  # actual code warnings / errors will be printed out to stdout
  clang-tidy ./src/_unity.cpp 2>/dev/null
}

## SCRIPT ENTRYPOINT

readonly verify_mode=${1:-"always"} # 'always', 'when-staged-changes'

if [[ ${verify_mode} == "always" ]]; then
  # run clang-tidy always
  echo "verify-style-cpp.sh: Linting source files..."
  lint
elif [[ ${verify_mode} == "when-staged-changes" ]]; then
  # run clang-tidy only if there are staged source code changes
  readonly changed_source_files=$(git diff --cached --name-only --diff-filter=ACM \
    | grep -c -E '\.h$|\.cpp$')

  if [ "${changed_source_files}" == "0" ]; then
    #echo "verify-style-cpp.sh: No changed source files found."
    true
  else
    echo "verify-style-cpp.sh: Linting source files (due to staged source file changes)..."
    lint
  fi
else
  die "Invalid argument for verify_mode: ${verify_mode}"
fi
