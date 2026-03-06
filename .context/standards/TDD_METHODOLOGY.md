# TDD Methodology — Time Project

## Core Principle

> **Test-Driven Development in C**: Write tests first (RED), then implement the function to make tests pass (GREEN), then refactor while keeping tests green (REFACTOR). Tests run natively with gcc + Unity framework — no external services needed.

---

## Framework

**Test framework**: Unity (C unit testing framework)
**Location**: `tests/unity/` (vendored)
**Runner**: `tests/test_runner.c`
**Build**: `make test` compiles and runs all tests

---

## TDD Cycle

### 1. RED — Write a failing test

```c
// tests/math/test_julian.c
void test_julian_day_known_epoch(void) {
    // J2000.0 = January 1, 2000, 12:00 TT
    double jd = julian_day_from_date(2000, 1, 1);
    TEST_ASSERT_DOUBLE_WITHIN(0.5, 2451545.0, jd);
}
```

### 2. GREEN — Write minimal code to pass

```c
// src/math/julian.c
double julian_day_from_date(int year, int month, int day) {
    // Meeus algorithm
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}
```

### 3. REFACTOR — Improve while green

- Extract constants
- Add `const` qualifiers
- Improve variable names
- Add edge case tests

---

## Test Organization

```
tests/
  unity/          Unity framework (vendored, do not modify)
    unity.c
    unity.h
    unity_internals.h
  math/           Math layer tests
    test_julian.c
    test_kepler.c
    test_vec3.c
    test_mat4.c
    test_sidereal.c
  systems/        Knowledge system tests
    test_tzolkin.c
    test_iching.c
    test_chinese.c
    test_gregorian.c
  render/         Render tests (limited — GPU not available in native)
  test_runner.c   Main test runner (includes all test files)
```

---

## Test Naming

```
test_[function_name]_[scenario]

Examples:
test_julian_day_known_epoch
test_julian_day_negative_year
test_kepler_circular_orbit
test_kepler_high_eccentricity
test_vec3_cross_product_orthogonal
test_tzolkin_known_date_2012_dec_21
```

---

## What to Test

| Layer | What to test | How |
|-------|-------------|-----|
| Math | Pure function correctness | Known input/output pairs from reference books |
| Systems | Date-to-result accuracy | Known dates verified against external sources |
| Render | Shader compilation (if possible) | Smoke test on native GL context |
| Core | Integration flow | End-to-end with mock time input |

---

## What NOT to Test

- Visual appearance (verify by eye in browser)
- GPU driver behavior (varies per platform)
- Emscripten internals
- Third-party library internals (Unity framework itself)

---

## Running Tests

```bash
make test
# Compiles: gcc -Wall -Wextra -Werror -std=c11 tests/test_runner.c tests/unity/unity.c [src files] -o build/test_runner -lm
# Runs: ./build/test_runner
```

All tests must pass with zero warnings before committing.
