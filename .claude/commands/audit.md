# /audit Command

## Purpose

Run quality gate validation against the project's quality standards.

## Usage

```
/audit [scope]
```

## Scopes

```
/audit all          # All quality gates
/audit build        # Compilation and build gates only
/audit tests        # Test gates only
/audit phase N      # Check Phase N roadmap criteria
```

## What Happens

### Step 1: Read Quality Standards

Read `.context/standards/QUALITY_GATES.md` for gate definitions.

### Step 2: Run Applicable Gates

**Compilation Gate**:
- [ ] Does `make native` succeed with zero warnings?
- [ ] Does `make wasm` succeed?

**Test Gate**:
- [ ] Does `make test` pass all tests?
- [ ] Are math functions tested against reference values?

**Performance Gate** (when applicable):
- [ ] 60fps in browser?
- [ ] No malloc in render loop?
- [ ] WASM binary < 1MB?

**Code Quality Gate**:
- [ ] Follows CONVENTIONS.md?
- [ ] Layer dependencies respected?
- [ ] Pure functions in math/systems?

### Step 3: Report

```
# Quality Audit

Score: X/Y gates passed

## Compilation
- [x] Native build: PASS
- [ ] WASM build: FAIL (emcc not installed)

## Tests
- [x] All tests pass: PASS (5 tests, 0 failures)

## Recommendations
1. Install Emscripten SDK to unblock WASM gate
```

## When to Use

- Before committing significant changes
- After completing a roadmap phase
- Periodic quality check

---

**Last Updated**: 2026-03-05
