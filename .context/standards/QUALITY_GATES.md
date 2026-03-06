# Quality Gates — Time Project

**Version**: 1.0 **Last Updated**: 2026-03-05

---

## Compilation Gate

- [ ] `gcc -Wall -Wextra -Werror -std=c11 -pedantic` produces zero warnings
- [ ] `emcc` WASM build succeeds without errors
- [ ] No undefined behavior flagged by `-fsanitize=undefined` (when available)

---

## Test Gate

- [ ] All Unity tests pass (`make test` returns 0)
- [ ] Math functions tested against known reference values
- [ ] Each knowledge system tested with known date-to-result pairs
- [ ] No flaky tests (deterministic, no timing dependencies)

---

## Performance Gate

- [ ] 60fps maintained in browser with all active layers
- [ ] No malloc/free calls in render loop
- [ ] WASM binary size < 1MB (release build)
- [ ] Page load < 3 seconds on broadband

---

## Code Quality Gate

- [ ] Follows CONVENTIONS.md (naming, file organization, includes)
- [ ] No compiler warnings with full flags
- [ ] Pure functions in math/systems layers (no side effects)
- [ ] Layer dependency rules respected (ARCHITECTURE.md)

---

## Coverage Targets

| Layer | Target | Rationale |
|-------|--------|-----------|
| Math (pure functions) | 95%+ | Critical calculations, easy to test |
| Systems (domain logic) | 90%+ | Knowledge system accuracy is core to the project |
| Render (GPU interface) | 50%+ | Hard to unit test GPU calls, integration test instead |
| Visual/UI | 40%+ | Visual output verified by eye, basic smoke tests |
| Core/Platform | 60%+ | Integration-level testing |

---

## Review Gate

- [ ] Code reviewed (by agent or self-review)
- [ ] No memory leaks detected (valgrind on native build, when applicable)
- [ ] GLSL shaders compile without errors on target browsers
- [ ] Responsive to window resize

---

## Deployment Gate

- [ ] WASM loads in Chrome, Firefox, Safari
- [ ] WebGL2 detection with fallback message
- [ ] No console errors in browser
- [ ] Static hosting configured and serving correctly
