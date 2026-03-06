# Environment Setup Checklist

## System Tools

- [x] gcc installed — 13.3.0
- [x] make installed — 4.3
- [x] git installed — 2.43.0
- [x] Python 3 installed — 3.12.3

## Emscripten SDK

- [x] emsdk cloned to ~/emsdk (outside project repo)
- [x] emsdk latest installed — 5.0.2
- [x] emsdk activated
- [x] emsdk in PATH (sourced in ~/.bashrc)
- [x] emcc works — 5.0.2

## Editor

- [x] VS Code C/C++ extension installed

## Git Repository

- [x] Git repo initialized
- [x] .gitignore created
- [x] Remote: `git@github.com-nubiru:Nubiru/time.git` (SSH via nubiru key)
- [x] Branch: main
- [x] First commit made and pushed

## Test Framework

- [x] Unity test framework vendored in tests/unity/ (3 files, ~283KB)

## Build Verification

- [x] Hello world compiles natively: `make native` -> "Time - A Visual Study"
- [x] Hello world compiles to WASM: `make wasm` -> build/index.html + index.wasm + index.js
- [x] WASM loads in browser: `make serve` -> http://localhost:8080 serves 200 OK
- [x] Test runner compiles and runs: `make test` -> "0 Tests 0 Failures 0 Ignored OK"

---

**All checks passing. Phase 0 complete (pending first commit + push). Ready for Phase 1.**
