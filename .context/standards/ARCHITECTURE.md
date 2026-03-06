# Time — Architecture

**Version**: 1.0 **Last Updated**: 2026-03-05

---

## System Overview

```
                    Browser (WebGL2)
                         |
                    [WASM Module]
                         |
              +----------+----------+
              |          |          |
           Render     Systems     UI
           Layer      Layer      Layer
              |          |          |
              +-----+----+         |
                    |              |
                Math Layer    Platform
                (pure)        Layer
```

---

## Layer Architecture

### Math Layer (pure, no dependencies)
**Location**: `src/math/`
**Purpose**: Mathematical primitives — vector/matrix ops, astronomical algorithms
**Rules**: Pure functions only. No side effects. No global state. No I/O.
**Key files**: `vec3.c`, `mat4.c`, `julian.c`, `kepler.c`, `sidereal.c`

### Systems Layer (domain logic)
**Location**: `src/systems/`
**Purpose**: Knowledge system calculations — each system is an independent domain
**Rules**: Depends only on Math layer. Each system in its own subdirectory. No rendering.
**Domains**: `astronomy/`, `astrology/`, `tzolkin/`, `iching/`, `chinese/`, `human_design/`, `gregorian/`

### Render Layer (GPU interface)
**Location**: `src/render/`
**Purpose**: WebGL2 context management, shader compilation, draw calls
**Rules**: Handles GPU state. Does not compute positions (Systems layer does that).
**Key files**: `gl_init.c`, `shader.c`, `camera.c`, `mesh.c`

### Visual Layer (art direction)
**Location**: `src/visual/`
**Purpose**: Compositing, transitions, color palette, visual effects
**Rules**: Sits above Render. Orchestrates what Render draws.
**Key files**: `compositor.c`, `transitions.c`, `palette.c`

### UI Layer (information display)
**Location**: `src/ui/`
**Purpose**: Text rendering, time displays, overlays
**Rules**: Reads from Systems, draws via Render. No domain logic.
**Key files**: `text.c`, `time_display.c`

### Platform Layer (host integration)
**Location**: `src/platform/`
**Purpose**: Input handling, window management, Emscripten-specific code
**Rules**: Isolates platform-specific APIs behind clean interfaces.
**Key files**: `input.c`, `window.c`

### Core (entry point)
**Location**: `src/core/`
**Purpose**: Main loop, initialization, orchestration
**Key files**: `main.c`

---

## Data Flow

```
Time Input (real-time or user-set)
     |
     v
Julian Day Number (universal time representation)
     |
     +---> Astronomy: orbital positions (ecliptic coordinates)
     +---> Astrology: zodiac signs, houses, aspects
     +---> Gregorian: human-readable date/time
     +---> Tzolkin: day sign + tone
     +---> I Ching: hexagram
     +---> Chinese: stem + branch + animal
     +---> Human Design: gates + channels
     |
     v
Visual Layer: composites all system outputs into layered rendering
     |
     v
Render Layer: issues WebGL2 draw calls
     |
     v
Browser Canvas (60fps)
```

---

## Build Pipeline

### Native (testing)
```
src/**/*.c --[gcc]--> build/time (native executable)
```

### WASM (deployment)
```
src/**/*.c --[emcc]--> build/index.js + build/index.wasm
web/index.html (shell file) loads the WASM module
```

### Tests
```
tests/**/*.c + src/**/*.c + tests/unity/unity.c --[gcc]--> build/test_runner
```

---

## Shader Pipeline

```
shaders/*.vert + shaders/*.frag
     |
     v
Loaded as C string constants (embedded or file-read)
     |
     v
glCreateShader + glCompileShader (at runtime, on GPU)
     |
     v
glCreateProgram + glLinkProgram
     |
     v
Ready for draw calls
```

---

## Dependency Rules

| Layer | Can depend on | Cannot depend on |
|-------|--------------|-----------------|
| Math | Nothing (pure) | Everything else |
| Systems | Math | Render, Visual, UI, Platform |
| Render | Math | Systems, Visual, UI |
| Visual | Render, Math | Systems (receives data via Core) |
| UI | Render, Math | Systems (receives data via Core) |
| Platform | Nothing project-specific | All layers |
| Core | Everything | (it's the orchestrator) |

---

## Key Design Decisions

1. **Julian Day as universal time**: All systems receive JD, convert internally
2. **No global state in math/systems**: Pure functions enable testing and reuse
3. **Emscripten main loop**: `emscripten_set_main_loop()` drives the frame cycle
4. **Stack allocation preferred**: No malloc in hot path (render loop)
5. **Each knowledge system is independent**: Can be developed/tested in isolation
