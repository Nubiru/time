#!/usr/bin/env python3
"""Build safety checks — validates build system integrity.

Catches recurring agent bugs:
- Phantom files: .c referenced in CMakeLists.txt but missing on disk
- Orphaned files: .c on disk but not registered in any CMakeLists.txt
- Missing test files: test references in cmake/testing.cmake pointing to non-existent files
- Duplicate OBJECT library names

Run: python3 scripts/check_build_safety.py
Exit 0 if clean, exit 1 if any issues found.
"""
import os
import re
import sys
import glob


def main():
    # Navigate to project root
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    os.chdir(root)

    errors = 0
    warnings = 0

    # Find all CMakeLists.txt under src/
    cmake_files = glob.glob("src/**/CMakeLists.txt", recursive=True)
    cmake_files.append("CMakeLists.txt")  # root

    # Check 1: Phantom files — .c referenced but missing
    print("--- Check 1: Phantom files ---")
    phantom_count = 0
    for cmake_file in cmake_files:
        cmake_dir = os.path.dirname(cmake_file) or "."
        with open(cmake_file) as f:
            content = f.read()
        # Find ${CMAKE_CURRENT_SOURCE_DIR}/xxx.c patterns
        for m in re.finditer(
            r'\$\{CMAKE_CURRENT_SOURCE_DIR\}/([a-zA-Z0-9_/]+\.c)', content
        ):
            cfile = os.path.join(cmake_dir, m.group(1))
            if not os.path.isfile(cfile):
                print(f"  PHANTOM: {cfile} (from {cmake_file})")
                errors += 1
                phantom_count += 1
        # Find bare .c in OBJECT lines
        for m in re.finditer(
            r'add_library\(\w+\s+OBJECT\s+([a-zA-Z0-9_/]+\.c)', content
        ):
            cfile = os.path.join(cmake_dir, m.group(1))
            if not os.path.isfile(cfile):
                print(f"  PHANTOM: {cfile} (from {cmake_file} OBJECT)")
                errors += 1
                phantom_count += 1
    if phantom_count == 0:
        print("  PASS: No phantom files")

    # Check 2: Orphaned source files
    print("\n--- Check 2: Orphaned source files ---")
    # Build registered set from all cmake files
    registered = set()
    for cmake_file in cmake_files:
        with open(cmake_file) as f:
            content = f.read()
        for m in re.finditer(
            r'\$\{CMAKE_CURRENT_SOURCE_DIR\}/([a-zA-Z0-9_/]+\.c)', content
        ):
            registered.add(m.group(1))
        for m in re.finditer(
            r'add_library\(\w+\s+OBJECT\s+([a-zA-Z0-9_/]+\.c)', content
        ):
            registered.add(m.group(1))

    orphan_count = 0
    for cfile in sorted(glob.glob("src/**/*.c", recursive=True)):
        # Skip stateful shell (managed separately)
        if "/core/" in cfile or "/passes/" in cfile:
            continue
        basename = os.path.basename(cfile)
        if basename not in registered:
            print(f"  ORPHAN: {cfile}")
            warnings += 1
            orphan_count += 1
    if orphan_count == 0:
        print("  PASS: No orphaned source files")

    # Check 3: Test file references in cmake/testing.cmake
    print("\n--- Check 3: Test file references ---")
    testing_cmake = "cmake/testing.cmake"
    test_errors = 0
    if os.path.isfile(testing_cmake):
        with open(testing_cmake) as f:
            content = f.read()
        for m in re.finditer(r'tests/[a-zA-Z0-9_/]+\.c', content):
            if not os.path.isfile(m.group(0)):
                print(f"  MISSING TEST: {m.group(0)} (from {testing_cmake})")
                errors += 1
                test_errors += 1
        if test_errors == 0:
            print("  PASS: All test files exist")
    else:
        print("  SKIP: cmake/testing.cmake not found")

    # Check 4: Duplicate OBJECT library names
    print("\n--- Check 4: Duplicate OBJECT library names ---")
    lib_names = []
    for cmake_file in cmake_files:
        with open(cmake_file) as f:
            content = f.read()
        for m in re.finditer(r'add_library\((\w+)\s+OBJECT', content):
            lib_names.append(m.group(1))

    seen = set()
    dupes = set()
    for name in lib_names:
        if name in seen:
            dupes.add(name)
        seen.add(name)

    if dupes:
        for d in sorted(dupes):
            print(f"  DUPLICATE: {d}")
            errors += 1
    else:
        print("  PASS: No duplicate library names")

    # Summary
    print(f"\n=== Summary: {errors} error(s), {warnings} warning(s) ===")
    if errors > 0:
        print("FAIL — fix errors before committing")
        return 1
    if warnings > 0:
        print("PASS with warnings")
    else:
        print("PASS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
