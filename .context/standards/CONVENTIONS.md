# C Coding Conventions — Time Project

**Version**: 1.0 **Last Updated**: 2026-03-05

---

## Naming

| Element | Convention | Example |
|---------|-----------|---------|
| Functions | `snake_case` | `julian_day_from_date()` |
| Variables | `snake_case` | `mean_anomaly` |
| Constants | `UPPER_CASE` | `DAYS_PER_YEAR` |
| Macros | `UPPER_CASE` | `#define PI 3.14159265358979323846` |
| Typedefs | `PascalCase_t` | `Vec3_t`, `Planet_t` |
| Enums | `UPPER_CASE` values | `SIGN_ARIES`, `SIGN_TAURUS` |
| File names | `snake_case.c/.h` | `julian_day.c`, `kepler.h` |

---

## File Organization

- `.h` files declare (prototypes, typedefs, constants)
- `.c` files implement
- Every `.h` uses include guards:

```c
#ifndef TIME_JULIAN_H
#define TIME_JULIAN_H

// declarations here

#endif // TIME_JULIAN_H
```

Guard format: `TIME_[MODULE]_H`

---

## Compiler Flags

**Required for all builds**:
```
-Wall -Wextra -Werror -std=c11 -pedantic
```

**Debug builds** add: `-g -O0 -DDEBUG`
**Release builds** add: `-O2 -DNDEBUG`
**WASM builds** add: `-Os -s USE_WEBGL2=1 -s FULL_ES3=1`

---

## Memory Rules

- Prefer stack allocation over heap
- **No malloc in the render loop** — allocate at init, reuse
- If malloc is needed, pair every `malloc` with a `free`
- Clear pointers after free: `ptr = NULL;`
- Use arena allocator pattern for dynamic needs (when we get there)
- Arrays: always track size alongside the pointer

---

## Math Functions

- All pure functions (no side effects, no global state)
- Use `const` parameters where the function doesn't modify input
- Return values, don't modify output pointers (unless arrays/structs)
- Use `double` for astronomical calculations (not `float`)
- Define constants as `static const double`, not `#define` (type safety)

```c
// Good
static const double JULIAN_EPOCH = 2451545.0; // J2000.0

double julian_day_from_date(int year, int month, int day);

// Bad
#define JULIAN_EPOCH 2451545.0
```

---

## Structs

- Use for data grouping
- No methods (C has no methods) — free functions take struct pointers
- Initialize all fields at declaration
- Use designated initializers:

```c
typedef struct {
    double x;
    double y;
    double z;
} Vec3_t;

Vec3_t position = { .x = 0.0, .y = 0.0, .z = 1.0 };
```

---

## Error Handling

- Return codes: 0 = success, non-zero = error
- No exceptions (C has none)
- Check return values of all system calls
- Use `enum` for error codes when there are multiple failure modes:

```c
typedef enum {
    TIME_OK = 0,
    TIME_ERR_INVALID_DATE,
    TIME_ERR_CONVERGENCE,
    TIME_ERR_GL_INIT
} TimeError_t;
```

---

## Comments

- Use `//` for single-line comments
- Use `/* */` for multi-line block comments
- Comment WHY, not WHAT (code should be self-documenting)
- Every `.h` file starts with a brief description of the module
- No comment noise (don't comment obvious code)

---

## GLSL Conventions

| Element | Prefix | Example |
|---------|--------|---------|
| Uniforms | `u_` | `u_model_matrix` |
| Attributes | `a_` | `a_position` |
| Varyings/Out | `v_` | `v_color` |
| Constants | none | `PI` |

Shader files: `.vert` for vertex, `.frag` for fragment. Stored in `shaders/`.

---

## Includes

- System includes first, then project includes
- Sort alphabetically within each group
- Use angle brackets for system, quotes for project:

```c
#include <math.h>
#include <stdio.h>

#include "julian.h"
#include "vec3.h"
```

---

## Testing (Unity Framework)

- Test files: `tests/[module]/test_[name].c`
- Test function naming: `test_[function]_[scenario]`
- One assertion per test (prefer, not strict)
- AAA pattern: Arrange, Act, Assert

```c
void test_julian_day_j2000(void) {
    // Arrange
    int year = 2000, month = 1, day = 1;
    double expected = 2451544.5;

    // Act
    double jd = julian_day_from_date(year, month, day);

    // Assert
    TEST_ASSERT_DOUBLE_WITHIN(0.001, expected, jd);
}
```

---

## Git Commit Format

```
type(scope): subject line (imperative, <72 chars)

- What changed and why
- What changed and why

Co-Authored-By: Claude Opus 4.6 <noreply@anthropic.com>
```

**Types**: `feat`, `fix`, `refactor`, `test`, `docs`, `chore`, `perf`
**Scopes**: `math`, `render`, `astronomy`, `astrology`, `tzolkin`, `iching`, `chinese`, `human-design`, `build`, `docs`
