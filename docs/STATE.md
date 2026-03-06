# Time — State Inventory

**Last Updated**: 2026-03-06

This document tracks what is **pure** (stateless) and what is **stateful** in the codebase. The goal: maximize pure code, minimize and isolate mutable state.

---

## Architecture Pattern: Functional Core, Imperative Shell

```
 PURE CORE (stateless)          IMPERATIVE SHELL (stateful)
 ─────────────────────          ───────────────────────────
 Input → Output                 Mutates state, talks to GPU
 No side effects                GL calls, input handling
 Fully testable                 Tested visually / integration
 ─────────────────────          ───────────────────────────
 src/math/*                     src/core/main.c (orchestrator)
 src/systems/*                  src/core/app_state.c (init)
 src/render/camera.c            src/render/gl_init.c
                                src/render/mesh.c
                                src/render/shader.c
```

---

## State Container

**One struct holds ALL mutable state**: `app_state_t` in `src/core/app_state.h`

| Field | Type | Mutated by |
|-------|------|-----------|
| `simulation_jd` | double | main loop (time advancement) |
| `time_speed` | double | user input (speed controls) |
| `camera` | camera_t | mouse drag, scroll wheel |
| `mouse_down` | int | mousedown/mouseup callbacks |
| `mouse_x/y` | double | mousemove callback |
| `program` | GLuint | init only (created once) |
| `loc_*` | GLint | init only (cached once) |
| `sun_mesh` | mesh_t | init only (created once) |
| `planet_mesh` | mesh_t | init only (created once) |
| `show_trails` | int | user input (T key toggle) |
| `trail_program` | GLuint | init only (created once) |
| `trail_loc_*` | GLint | init only (cached once) |
| `trail_vao` | GLuint | init only (created once) |
| `prev_time_ms` | double | main loop (frame timing) |

**Only one global variable in the entire program**: `g_state` in main.c

---

## Pure Modules (stateless)

These take input, return output, never touch globals or GPU.

| Module | Functions | Testable natively | Notes |
|--------|-----------|------------------|-------|
| vec3.c | 8 | YES | All return by value |
| mat4.c | 9 | YES | All return by value |
| julian.c | 3 | YES | Calendar ↔ JD |
| kepler.c | 2 | YES | Newton-Raphson iteration |
| sidereal.c | 3 | YES | GMST, GAST, LST |
| ecliptic.c | 2 | YES | Ecliptic ↔ equatorial, obliquity |
| camera.c | 6 | YES | Spherical ↔ Cartesian, matrices |
| orbit.c | 2 (+8 consts) | YES | Keplerian orbital mechanics |
| planets.c | 3 | YES | All-planet snapshot, names, symbols |
| gregorian.c | 5 | YES | Date/time formatting, day of week |
| zodiac.c | 5 | YES | Sign, degree, decan, symbols |
| tzolkin.c | 3 (+names) | YES | Dreamspell kin from JD |

**Total pure functions**: 56
**Rule**: These files MUST NEVER include `<GLES3/gl3.h>` or reference `app_state_t`.

---

## Stateful Modules (imperative shell)

These interact with GPU state, global state, or the browser.

| Module | Side effects | Guarded by |
|--------|-------------|-----------|
| main.c | Owns g_state, runs loop, registers callbacks | `#ifdef __EMSCRIPTEN__` |
| app_state.c | Creates initial state (no GL) | pure initializer |
| gl_init.c | Creates WebGL2 context, sets viewport | `#ifdef __EMSCRIPTEN__` |
| shader.c | Creates/deletes GL shader objects | `#ifdef __EMSCRIPTEN__` |
| mesh.c | Allocates VAO/VBO/EBO, uploads to GPU | `#ifdef __EMSCRIPTEN__` |

**Rule**: All GL code behind `#ifdef __EMSCRIPTEN__` guards. Native build never sees it.

---

## Future Systems (all will be pure)

| System | Input | Output | State? |
|--------|-------|--------|--------|
| iching/iching.c | JD | hexagram | PURE |
| chinese/chinese.c | JD | stem + branch + animal | PURE |
| human_design/hd.c | planetary positions | gates + channels | PURE |

**Pattern**: Every system takes a Julian Day, returns a result struct. No mutation.

---

## Dependency Rules (enforced)

```
PURE code must NEVER:
  - Include GL headers
  - Reference app_state_t
  - Use printf (except debug builds)
  - Call malloc (stack only)
  - Modify any global

STATEFUL code:
  - Must be behind #ifdef __EMSCRIPTEN__ if it uses GL
  - Must only mutate g_state (not random globals)
  - Must use pure functions for computation
```

---

## How to Verify Purity

```bash
# No GL headers in pure code:
grep -r "GLES3\|glCreate\|glBind" src/math/ src/systems/
# Should return nothing

# No globals in pure code:
grep -r "g_state\|app_state" src/math/ src/systems/
# Should return nothing

# Full audit:
/purity
```
