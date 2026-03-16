#!/bin/bash
# run_cppcheck.sh — Static analysis with cppcheck
# Requires: sudo apt install cppcheck
#
# Usage: ./scripts/run_cppcheck.sh

set -e

cd "$(dirname "$0")/.."

if ! command -v cppcheck &>/dev/null; then
    echo "SKIP: cppcheck not installed. Run: sudo apt install cppcheck"
    exit 0
fi

echo "=== Running cppcheck ==="
cppcheck --enable=all --std=c11 --error-exitcode=1 \
    --suppress=missingIncludeSystem \
    --suppress=unusedFunction \
    --suppress=unmatchedSuppression \
    -I src/ \
    --quiet \
    src/ 2>&1

EXIT=$?
if [ $EXIT -eq 0 ]; then
    echo "PASS: cppcheck clean"
else
    echo "FAIL: cppcheck found issues"
fi
exit $EXIT
