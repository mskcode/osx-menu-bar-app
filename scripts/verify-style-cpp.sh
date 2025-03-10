#!/usr/bin/env bash
#
# Run clang-tidy on the source code.
#

set -euo pipefail

echo "verify-style-cpp.sh: Linting project..."

# actual code warnings / errors will be printed out to stdout
clang-tidy ./src/_unity.cpp 2>/dev/null
