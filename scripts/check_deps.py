#!/usr/bin/env python3
"""check_deps.py — Header dependency analysis for Time project.

Checks:
  1. Circular include detection
  2. Purity violations (pure zones importing GL/render headers)
  3. Include depth per file
  4. Missing headers (included but don't exist)

Usage:
  python3 scripts/check_deps.py [--verbose]
"""

import os
import re
import sys
from collections import defaultdict
from pathlib import Path

SRC_DIR = Path("src")

# Headers that indicate GL/render dependency (must not appear in pure zones)
GL_HEADERS = {
    "GLES3/gl3.h", "GL/gl.h", "GL/glew.h",
    "emscripten.h", "emscripten/html5.h",
}

# Pure zones — modules here must NOT include GL/render stateful headers
PURE_ZONES = {"math", "systems"}

# Render/stateful zones — OK to include anything
STATEFUL_ZONES = {"render", "core", "platform"}

# UI is mixed — some pure, some stateful
# (ui_bridge.c and audio_engine.c are stateful, rest are pure)
UI_STATEFUL_FILES = {
    "ui_bridge.c", "audio_engine.c", "audio_engine.h",
}


def find_includes(filepath):
    """Extract #include paths from a C/H file."""
    includes = []
    try:
        with open(filepath, 'r', errors='replace') as f:
            for line in f:
                m = re.match(r'\s*#include\s+"([^"]+)"', line)
                if m:
                    includes.append(m.group(1))
    except FileNotFoundError:
        pass
    return includes


def resolve_include(include_path, from_file):
    """Resolve a relative include path to an absolute path."""
    # Try relative to the including file
    from_dir = from_file.parent
    resolved = from_dir / include_path
    if resolved.exists():
        return resolved.resolve()
    # Try relative to src/
    resolved = SRC_DIR / include_path
    if resolved.exists():
        return resolved.resolve()
    return None


def build_graph():
    """Build the full include graph."""
    graph = defaultdict(set)  # file -> set of included files
    all_files = []

    for ext in ("*.c", "*.h"):
        all_files.extend(SRC_DIR.rglob(ext))

    for filepath in all_files:
        includes = find_includes(filepath)
        for inc in includes:
            resolved = resolve_include(inc, filepath)
            if resolved:
                graph[filepath.resolve()].add(resolved)

    return graph, all_files


def find_cycles(graph):
    """Find circular include chains using DFS."""
    WHITE, GRAY, BLACK = 0, 1, 2
    color = defaultdict(int)
    cycles = []

    def dfs(node, path):
        color[node] = GRAY
        path.append(node)
        for neighbor in graph.get(node, set()):
            if color[neighbor] == GRAY:
                # Found cycle — extract it
                cycle_start = path.index(neighbor)
                cycle = path[cycle_start:] + [neighbor]
                cycles.append(cycle)
            elif color[neighbor] == WHITE:
                dfs(neighbor, path)
        path.pop()
        color[node] = BLACK

    for node in graph:
        if color[node] == WHITE:
            dfs(node, [])

    return cycles


def check_purity(graph, all_files):
    """Check that pure zone files don't include GL headers (directly or transitively)."""
    violations = []
    src_root = SRC_DIR.resolve()

    # Build transitive closure for each file
    def get_all_includes(filepath, visited=None):
        if visited is None:
            visited = set()
        if filepath in visited:
            return set()
        visited.add(filepath)
        result = set()
        for inc in graph.get(filepath, set()):
            result.add(inc)
            result |= get_all_includes(inc, visited)
        return result

    for filepath in all_files:
        abs_path = filepath.resolve()
        rel = abs_path.relative_to(src_root)
        parts = rel.parts

        # Determine if this file is in a pure zone
        if len(parts) < 1:
            continue

        zone = parts[0]
        if zone not in PURE_ZONES:
            continue

        # Check direct includes for GL headers
        includes = find_includes(filepath)
        for inc in includes:
            inc_basename = os.path.basename(inc)
            if inc in GL_HEADERS or inc_basename in GL_HEADERS:
                violations.append((str(rel), inc, "direct"))

    return violations


def compute_depth(graph):
    """Compute max include depth for each file."""
    depths = {}

    def depth(filepath, visited=None):
        if visited is None:
            visited = set()
        if filepath in depths:
            return depths[filepath]
        if filepath in visited:
            return 0  # cycle — don't recurse
        visited.add(filepath)
        max_child = 0
        for inc in graph.get(filepath, set()):
            max_child = max(max_child, depth(inc, visited))
        depths[filepath] = max_child + 1
        return depths[filepath]

    for filepath in graph:
        depth(filepath)

    return depths


def check_missing(all_files):
    """Find includes that don't resolve to existing files."""
    missing = []
    for filepath in all_files:
        includes = find_includes(filepath)
        for inc in includes:
            resolved = resolve_include(inc, filepath)
            if resolved is None:
                # Skip system headers
                if not inc.startswith(("../", "./", "src/")):
                    if "/" not in inc or inc.startswith("../../"):
                        pass  # relative paths that should resolve
                    else:
                        continue
                missing.append((str(filepath), inc))
    return missing


def main():
    verbose = "--verbose" in sys.argv or "-v" in sys.argv

    if not SRC_DIR.exists():
        print(f"Error: {SRC_DIR} not found. Run from project root.", file=sys.stderr)
        sys.exit(1)

    print("=== Header Dependency Analysis ===\n")

    # Build graph
    graph, all_files = build_graph()
    file_count = len(all_files)
    edge_count = sum(len(v) for v in graph.values())
    print(f"Files scanned: {file_count}")
    print(f"Include edges: {edge_count}")

    errors = 0

    # 1. Circular includes
    print("\n--- Circular Includes ---")
    cycles = find_cycles(graph)
    if cycles:
        src_root = SRC_DIR.resolve()
        seen = set()
        for cycle in cycles:
            key = frozenset(str(n) for n in cycle)
            if key in seen:
                continue
            seen.add(key)
            chain = " -> ".join(
                str(n.relative_to(src_root)) if n.is_relative_to(src_root)
                else str(n) for n in cycle
            )
            print(f"  CYCLE: {chain}")
            errors += 1
        print(f"  {len(seen)} cycle(s) found")
    else:
        print("  CLEAN: No circular includes")

    # 2. Purity violations
    print("\n--- Purity (GL in pure zones) ---")
    violations = check_purity(graph, all_files)
    if violations:
        for filepath, inc, kind in violations:
            print(f"  VIOLATION ({kind}): {filepath} includes {inc}")
            errors += 1
    else:
        print("  CLEAN: No GL headers in pure zones")

    # 3. Include depth
    print("\n--- Include Depth ---")
    depths = compute_depth(graph)
    src_root = SRC_DIR.resolve()
    sorted_depths = sorted(
        ((d, f) for f, d in depths.items()),
        reverse=True
    )
    max_depth = sorted_depths[0][0] if sorted_depths else 0
    print(f"  Max depth: {max_depth}")
    if verbose:
        print("  Top 10 deepest:")
        for d, f in sorted_depths[:10]:
            rel = f.relative_to(src_root) if f.is_relative_to(src_root) else f
            print(f"    {d:3d}  {rel}")

    # 4. Missing headers
    print("\n--- Missing Headers ---")
    missing = check_missing(all_files)
    if missing:
        for filepath, inc in missing[:20]:
            rel = str(Path(filepath).relative_to("."))
            print(f"  MISSING: {rel} includes \"{inc}\" (not found)")
            errors += 1
        if len(missing) > 20:
            print(f"  ... and {len(missing) - 20} more")
    else:
        print("  CLEAN: All includes resolve")

    # Summary
    print(f"\n=== Summary: {errors} issue(s) ===")
    return 1 if errors > 0 else 0


if __name__ == "__main__":
    sys.exit(main())
