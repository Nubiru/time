# Quality Gates — Time Project

**Version**: 1.0 **Last Updated**: 2026-03-06

Machine-checkable quality gates for the autonomous agent pipeline. The Maintainer subagent runs these after every Writer+Checker cycle.

---

## Gate Definitions

| # | Gate | Check | Threshold | Tool | Severity |
|---|------|-------|-----------|------|----------|
| G1 | Compilation | `gcc -Wall -Wextra -Werror -std=c11 -pedantic` | 0 warnings | Bash | CRITICAL |
| G2 | Tests | `make test` | 0 failures | Bash | CRITICAL |
| G3 | Regression | `make clean && make test` | 0 failures | Bash | CRITICAL |
| G4 | Purity P1 | No GL calls in pure zones (`src/math/`, `src/systems/`) | 0 matches | Grep | CRITICAL |
| G5 | Purity P2 | No heap allocation in pure zones | 0 matches | Grep | CRITICAL |
| G6 | Purity P3 | No printf/fprintf/puts in production code | 0 matches | Grep | CRITICAL |
| G7 | Purity P4 | No globals (app_state, g_state) in pure zones | 0 matches | Grep | CRITICAL |
| G8 | Include guards | `#ifndef TIME_{MODULE}_H` in every `.h` file | Every .h | Grep | CRITICAL |
| G9 | Naming | snake_case functions, UPPER_CASE constants, TypeName_t typedefs | Manual | Grep | WARNING |
| G10 | Dead code | Every `.h` declaration used in `.c` or test | 0 orphans | Grep | WARNING |
| G11 | TODOs | No naked TODO/FIXME without reference | 0 naked | Grep | WARNING |
| G12 | Duplication | No duplicate functionality vs docs/STATE.md | 0 duplicates | Grep | WARNING |
| G13 | Attribution | External algorithms attributed in data/contributors.json | All attributed | Read | WARNING |
| G14 | WASM build | `make wasm` succeeds | 0 errors | Bash | CRITICAL |

---

## Grep Patterns

### G4: Purity P1 — No GL in pure zones
```bash
grep -rn "GLES3\|glCreate\|glBind\|glGen\|glDelete\|glUniform\|glDraw\|GL_" src/math/ src/systems/
```
Expected: empty (0 matches)

### G5: Purity P2 — No heap allocation in pure zones
```bash
grep -rn "malloc\|calloc\|realloc\|free" src/math/ src/systems/
```
Expected: empty (0 matches)

### G6: Purity P3 — No I/O in production code
```bash
grep -rn "printf\|fprintf\|puts" src/math/ src/systems/ --include="*.c"
```
Expected: empty (0 matches). Test files exempt.

### G7: Purity P4 — No globals in pure zones
```bash
grep -rn "app_state\|g_state" src/math/ src/systems/
```
Expected: empty (0 matches)

### G8: Include guards
```bash
for f in $(find src/ -name "*.h"); do
  guard=$(head -5 "$f" | grep "#ifndef TIME_")
  [ -z "$guard" ] && echo "MISSING GUARD: $f"
done
```
Expected: no output

### G10: Dead code
```bash
# For each function in new .h, verify it appears in a .c or test file
grep -rn "FUNCTION_NAME" src/ tests/ --include="*.c"
```
Expected: at least 1 match per declaration

### G11: Naked TODOs
```bash
grep -rn "TODO\|FIXME\|HACK\|XXX" src/ --include="*.c" --include="*.h" | grep -v "TODO("
```
Expected: empty (all TODOs should have a reference like `TODO(014)`)

---

## Severity Levels

| Level | Meaning | Pipeline Action |
|-------|---------|----------------|
| CRITICAL | Blocks delivery | Maintainer returns CRITICAL, Writer must fix, re-run Maintainer |
| WARNING | Non-blocking | Noted in maintenance.md, pipeline continues |

After 3 CRITICAL cycles on the same issue: write `escalation.md` and move to next task.

---

## Coverage Targets

| Layer | Target | Rationale |
|-------|--------|-----------|
| Math (pure functions) | 95%+ | Critical calculations, easy to test |
| Systems (domain logic) | 90%+ | Knowledge system accuracy is core |
| Render (GPU interface) | 50%+ | Hard to unit test GPU, integration test |
| Visual/UI | 40%+ | Visual output verified by eye |
| Core/Platform | 60%+ | Integration-level testing |

---

## Deployment Gates (Manual — Gabriel)

| Gate | Check | Method |
|------|-------|--------|
| Browser test | All HUD data displays correctly | Visual inspection |
| Performance | 60fps maintained | Browser dev tools |
| Cross-browser | Chrome + Firefox + Safari | Manual test |
| Console clean | No console errors | Browser dev tools |
| WASM size | < 1MB release build | `ls -la build/*.wasm` |
