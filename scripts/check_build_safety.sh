#!/bin/bash
# check_build_safety.sh — Validates build system integrity
# Run before committing to catch phantom files and broken deps
#
# Usage: ./scripts/check_build_safety.sh
#
# Checks:
#   1. Every .c file referenced in CMakeLists.txt exists on disk
#   2. Every .c file in src/ is registered in some CMakeLists.txt
#   3. Every .h file in src/ has a matching .c (or is header-only)
#   4. CMake configure succeeds with zero warnings
#
# Exits 0 on success, 1 on errors.

set -e

# Navigate to project root (parent of scripts/)
cd "$(dirname "$0")/.."

ERRORS=0
WARNINGS=0

echo "=== Build Safety Check ==="
echo "Project root: $(pwd)"

# --------------------------------------------------------------------------
# Check 1: Phantom files — .c referenced in CMakeLists.txt but missing on disk
# --------------------------------------------------------------------------
echo ""
echo "--- Check 1: Phantom files (registered but missing) ---"
PHANTOM_COUNT=0

# Find all CMakeLists.txt under src/ and root
while IFS= read -r cmake_file; do
    cmake_dir=$(dirname "$cmake_file")
    # Extract .c filenames from ${CMAKE_CURRENT_SOURCE_DIR}/xxx.c patterns
    while IFS= read -r cfile; do
        resolved="${cmake_dir}/${cfile}"
        if [ ! -f "$resolved" ]; then
            echo "  PHANTOM: ${resolved} (from ${cmake_file})"
            ERRORS=$((ERRORS + 1))
            PHANTOM_COUNT=$((PHANTOM_COUNT + 1))
        fi
    done < <(grep -oP '\$\{CMAKE_CURRENT_SOURCE_DIR\}/\K[a-zA-Z0-9_/]+\.c' "$cmake_file" 2>/dev/null)

    # Also catch bare .c references in add_library OBJECT lines: add_library(name OBJECT file.c)
    while IFS= read -r cfile; do
        resolved="${cmake_dir}/${cfile}"
        if [ ! -f "$resolved" ]; then
            echo "  PHANTOM: ${resolved} (from ${cmake_file} OBJECT lib)"
            # Don't double-count — OBJECT lines mirror the SOURCES list
            # Only count if not already caught above
            if ! grep -q "\${CMAKE_CURRENT_SOURCE_DIR}/${cfile}" "$cmake_file" 2>/dev/null; then
                ERRORS=$((ERRORS + 1))
                PHANTOM_COUNT=$((PHANTOM_COUNT + 1))
            fi
        fi
    done < <(grep -P 'OBJECT\s+' "$cmake_file" 2>/dev/null | grep -oP 'OBJECT\s+\K[a-zA-Z0-9_/]+\.c')
done < <(find . -name "CMakeLists.txt" -path "*/src/*" -o -name "CMakeLists.txt" -maxdepth 1 | sort)

if [ "$PHANTOM_COUNT" -eq 0 ]; then
    echo "  PASS: No phantom files"
fi

# --------------------------------------------------------------------------
# Check 2: Orphaned source files — .c in src/ but not in any CMakeLists.txt
# --------------------------------------------------------------------------
echo ""
echo "--- Check 2: Orphaned source files (on disk but unregistered) ---"
ORPHAN_COUNT=0

# Build a list of all registered .c filenames (basenames from SOURCES lists)
REGISTERED=$(find . -name "CMakeLists.txt" -path "*/src/*" -exec \
    grep -ohP '\$\{CMAKE_CURRENT_SOURCE_DIR\}/\K[a-zA-Z0-9_/]+\.c' {} \; 2>/dev/null | sort -u)

while IFS= read -r src_file; do
    # Skip excluded directories: core/ (stateful, managed separately) and passes/ (stateful)
    case "$src_file" in
        ./src/core/*) continue ;;
        ./src/render/passes/*) continue ;;
    esac

    # Get relative path from src dir for matching
    rel_path="${src_file#./src/}"
    fname=$(basename "$src_file")

    # Check if this file's basename appears in registered list
    # Also check with subdirectory path (e.g., passes/star_pass.c)
    if ! echo "$REGISTERED" | grep -qF "$fname"; then
        echo "  ORPHAN: ${src_file}"
        WARNINGS=$((WARNINGS + 1))
        ORPHAN_COUNT=$((ORPHAN_COUNT + 1))
    fi
done < <(find ./src -name "*.c" | sort)

if [ "$ORPHAN_COUNT" -eq 0 ]; then
    echo "  PASS: No orphaned source files"
fi

# --------------------------------------------------------------------------
# Check 3: Header/source pairing — .h without matching .c (info only)
# --------------------------------------------------------------------------
echo ""
echo "--- Check 3: Header-only modules (info) ---"
HEADER_ONLY=0

while IFS= read -r hfile; do
    cfile="${hfile%.h}.c"
    if [ ! -f "$cfile" ]; then
        HEADER_ONLY=$((HEADER_ONLY + 1))
    fi
done < <(find ./src -name "*.h" | sort)

echo "  INFO: ${HEADER_ONLY} header-only files (no matching .c)"

# --------------------------------------------------------------------------
# Check 4: CMake configure — must succeed cleanly
# --------------------------------------------------------------------------
echo ""
echo "--- Check 4: CMake configure ---"

# Use a temporary build directory to avoid polluting the workspace
BUILD_DIR="build-safety-check"
rm -rf "$BUILD_DIR"

CMAKE_OUTPUT=$(cmake -B "$BUILD_DIR" 2>&1)
CMAKE_EXIT=$?

if [ $CMAKE_EXIT -eq 0 ]; then
    echo "  PASS: CMake configure succeeded"
else
    echo "  FAIL: CMake configure failed (exit code $CMAKE_EXIT)"
    echo "$CMAKE_OUTPUT" | tail -20
    ERRORS=$((ERRORS + 1))
fi

# Check for CMake warnings (even if configure succeeded)
CMAKE_WARNS=$(echo "$CMAKE_OUTPUT" | grep -ci "warning" || true)
if [ "$CMAKE_WARNS" -gt 0 ]; then
    echo "  WARN: CMake produced $CMAKE_WARNS warning(s)"
    echo "$CMAKE_OUTPUT" | grep -i "warning"
    WARNINGS=$((WARNINGS + 1))
fi

# Clean up temporary build dir
rm -rf "$BUILD_DIR"

# --------------------------------------------------------------------------
# Check 5: Duplicate OBJECT library names
# --------------------------------------------------------------------------
echo ""
echo "--- Check 5: Duplicate OBJECT library names ---"
DUPES=$(find . -name "CMakeLists.txt" -path "*/src/*" -exec \
    grep -ohP 'add_library\(\K[a-zA-Z0-9_]+' {} \; 2>/dev/null | sort | uniq -d)

if [ -n "$DUPES" ]; then
    echo "  DUPLICATE library names found:"
    echo "$DUPES" | while read -r d; do
        echo "    $d"
    done
    ERRORS=$((ERRORS + 1))
else
    echo "  PASS: No duplicate library names"
fi

# --------------------------------------------------------------------------
# Summary
# --------------------------------------------------------------------------
echo ""
echo "==========================================="
echo "  Summary: $ERRORS error(s), $WARNINGS warning(s)"
echo "==========================================="

if [ $ERRORS -gt 0 ]; then
    echo "  RESULT: FAIL — fix errors before committing"
    exit 1
else
    if [ $WARNINGS -gt 0 ]; then
        echo "  RESULT: PASS with warnings"
    else
        echo "  RESULT: PASS"
    fi
    exit 0
fi
