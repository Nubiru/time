# Makefile — Convenience wrapper around CMake
# Usage: make build, make test, make wasm, make serve, make clean

.PHONY: build test wasm serve clean rebuild check smoke lint coverage sanitize

# Native build (gcc)
build:
	@cmake -B build-native -DCMAKE_BUILD_TYPE=Debug 2>/dev/null | tail -1
	@cmake --build build-native -j$$(nproc)

# Build + run all tests
test: build
	@ctest --test-dir build-native -j$$(nproc) --output-on-failure

# WASM build (emscripten)
wasm:
	@emcmake cmake -B build-wasm 2>/dev/null | tail -1
	@cmake --build build-wasm -j$$(nproc)

# Build WASM + serve locally
serve: wasm
	@echo "Serving at http://localhost:8080"
	@python3 -m http.server 8080 -d build-wasm

# Remove all build artifacts
clean:
	@rm -rf build-native build-wasm build-coverage build-sanitize coverage.info coverage-report

# Clean + rebuild
rebuild: clean build

# Build safety check (phantom files, orphaned sources)
check: build
	@python3 scripts/check_build_safety.py
	@echo "Build safety: PASS"

# WASM smoke test (requires Playwright — skips gracefully if not installed)
smoke: wasm
	@python3 tests/smoke/test_wasm_smoke.py
	@echo "Smoke test: PASS"

# Static analysis with cppcheck (skips gracefully if not installed)
lint:
	@bash scripts/run_cppcheck.sh
	@echo "Lint: PASS"

# Code coverage report (requires lcov: sudo apt install lcov)
coverage:
	@cmake -B build-coverage -DCMAKE_BUILD_TYPE=Coverage 2>/dev/null | tail -1
	@cmake --build build-coverage -j$$(nproc)
	@ctest --test-dir build-coverage -j$$(nproc) --output-on-failure
	@lcov --capture --directory build-coverage --output-file coverage.info --ignore-errors mismatch
	@lcov --remove coverage.info '*/tests/*' '*/unity/*' --output-file coverage.info --ignore-errors unused
	@genhtml coverage.info --output-directory coverage-report
	@echo "Coverage report: coverage-report/index.html"

# Address + undefined behavior sanitizers
sanitize:
	@cmake -B build-sanitize -DCMAKE_BUILD_TYPE=Sanitize 2>/dev/null | tail -1
	@cmake --build build-sanitize -j$$(nproc)
	@ctest --test-dir build-sanitize -j$$(nproc) --output-on-failure
