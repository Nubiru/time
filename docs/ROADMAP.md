# Time — Development Roadmap

**Version**: 1.0 **Last Updated**: 2026-03-05

---

## Overview

9 phases from zero to deployed artwork. Each phase builds on the previous. Each has clear "done when" criteria.

---

## Phase 0: Foundation Setup

**Goal**: Working C/WASM toolchain with a compiling project.

**Done when**:
- [x] `make native` compiles and runs, prints "Time" to terminal
- [x] `make wasm` compiles to WebAssembly
- [x] WASM loads in browser without errors
- [x] `make test` runs Unity test runner (even with zero tests)
- [x] Git repo initialized with first commit

**Tasks**:
1. Install gcc, make, git, python3
2. Install Emscripten SDK (emsdk)
3. Verify `emcc --version` works
4. Create main.c with Emscripten main loop stub
5. Create Makefile with native/wasm/test/clean/serve targets
6. Create web/index.html with canvas element
7. Verify native build: `make native && ./build/time`
8. Verify WASM build: `make wasm` then serve and load in browser
9. Vendor Unity test framework into tests/unity/
10. Initialize git repo, create .gitignore, first commit

**C learning introduced**: Compilation, linking, preprocessor directives (`#ifdef`), printf, return codes

**Files created**: `src/core/main.c`, `Makefile`, `web/index.html`, `web/style.css`, `scripts/*.sh`, `.gitignore`

---

## Phase 1: Hello WebGL Triangle

**Goal**: Render a colored triangle in the browser via C/WASM/WebGL2.

**Done when**:
- [x] Colored triangle renders in browser canvas
- [x] 60fps confirmed (requestAnimationFrame loop via Emscripten)
- [x] Canvas is fullscreen, black background
- [x] No WebGL errors in console

**Tasks**:
1. Learn WebGL2 API basics (context creation, shaders, buffers)
2. Write vertex + fragment shaders (first .glsl files)
3. Create WebGL initialization in C (via Emscripten GL bindings)
4. Create vertex buffer with triangle data
5. Compile and link shaders from C
6. Render loop: clear + draw
7. Verify 60fps in browser dev tools

**C learning introduced**: Arrays, structs, pointers, OpenGL ES 3.0 API calls, string handling (shader source)

**Files created**: `src/render/gl_init.c`, `src/render/gl_init.h`, `src/render/shader.c`, `src/render/shader.h`, `shaders/basic.vert`, `shaders/basic.frag`

---

## Phase 2: Math Foundations

**Goal**: Tested astronomical math library — pure functions with no rendering.

**Done when**:
- [x] Julian Day conversion passes tests (known dates)
- [x] Kepler's equation solver converges for all eccentricities < 1
- [x] Sidereal time (GST/LST) matches USNO values
- [x] vec3/mat4 operations pass tests (dot, cross, multiply, rotate)
- [x] All math functions are pure (no side effects, no global state)

**Tasks**:
1. Implement Julian Day Number from Gregorian date
2. Implement Kepler's equation solver (Newton-Raphson)
3. Implement mean/true anomaly calculations
4. Implement Greenwich Sidereal Time (IAU algorithm)
5. Implement Local Sidereal Time
6. Implement vec3 operations (add, subtract, dot, cross, normalize, scale)
7. Implement mat4 operations (identity, multiply, rotate, translate, perspective, lookAt)
8. Write comprehensive tests for each function

**C learning introduced**: Math library (`<math.h>`), floating point precision, iterative algorithms, unit testing with Unity

**Files created**: `src/math/julian.c`, `src/math/kepler.c`, `src/math/sidereal.c`, `src/math/vec3.c`, `src/math/mat4.c`, `src/math/*.h`, `tests/math/test_*.c`

---

## Phase 3: First Visual Layer

**Goal**: Earth orbiting the Sun in 3D, rendered in browser.

**Done when**:
- [x] Sun rendered at origin (glowing sphere or point)
- [x] Earth orbits Sun on correct orbital plane
- [x] Camera can rotate around the scene (mouse/touch)
- [x] Time advances (days pass, Earth moves)
- [x] Orbital speed is visually correct relative to real orbital period

**Tasks**:
1. Implement orbital position from Keplerian elements
2. Create sphere mesh generation (or point rendering)
3. Implement perspective camera with lookAt
4. Implement mouse-drag camera rotation
5. Connect time advancement to orbital calculation
6. Render Sun + Earth with basic shading
7. Add orbital trail/path visualization

**C learning introduced**: 3D transformations, camera math, input handling (Emscripten HTML5 API), mesh generation

**Files created**: `src/systems/astronomy/orbit.c`, `src/render/camera.c`, `src/render/mesh.c`, `src/platform/input.c`

---

## Phase 4: Time Display

**Goal**: Multiple time formats displayed simultaneously on screen.

**Done when**:
- [x] Gregorian date/time formatted (gregorian.c: format_date, format_time, format_full, day_of_week)
- [x] Julian Day number displayed (DOM HUD overlay via EM_ASM)
- [x] Greenwich Sidereal Time displayed (GMST in HH:MM:SS format)
- [x] Local Sidereal Time displayed (LST from observer longitude, Greenwich default)
- [x] All times update in real-time as simulation runs
- [x] Text rendering works in WebGL (DOM overlay approach — canvas HTML div)

**Tasks**:
1. Implement Gregorian date formatting
2. Implement text rendering (bitmap font atlas or HTML overlay)
3. Create time display UI layer
4. Connect all time systems to display
5. Add configurable observer location (latitude/longitude)

**C learning introduced**: String formatting (`snprintf`), bitmap fonts, texture atlases, UI overlay patterns

**Files created**: `src/systems/gregorian/gregorian.c`, `src/ui/text.c`, `src/ui/time_display.c`

---

## Phase 5: Astrology Layer

**Goal**: Zodiac wheel with planets positioned in signs.

**Done when**:
- [x] 12 zodiac signs rendered as wheel/circle (colored ring + glyph labels + planet markers + aspect lines + house cusps)
- [x] Planetary positions calculated (planets.c: all 8 planets via orbit.c)
- [x] Zodiac sign from longitude computed (zodiac.c: sign, degree, decan, symbols)
- [ ] Positions match external astrology software (within 1 degree)
- [x] Aspects between planets indicated (aspects.c: 5 Ptolemaic aspects, angular separation, orb detection)
- [x] Houses displayed (houses.c: Whole Sign system, observer.c: Ascendant calculation)

**Tasks**:
1. Implement ecliptic longitude calculation for each planet
2. Map ecliptic longitude to zodiac sign
3. Render zodiac wheel (12 segments)
4. Place planet glyphs on wheel at correct positions
5. Calculate and display aspects
6. Implement house system

**C learning introduced**: Circular data structures, angle arithmetic, glyph rendering

**Files created**: `src/systems/astrology/zodiac.c`, `src/systems/astrology/aspects.c`, `src/systems/astrology/houses.c`

---

## Phase 6: Knowledge Systems

**Goal**: Tzolkin + I Ching + Chinese Calendar + Human Design all compute correctly.

**Done when**:
- [x] Tzolkin day sign + tone correct for any date (verified: 2012-12-21=Kin207, Kin1 anchor, Feb29 skip, 260-cycle)
- [x] I Ching hexagram derivation works for any date
- [x] Chinese calendar year animal + element correct (chinese.c: sexagenary cycle, stems, branches, animals, elements)
- [x] Human Design gate + channel derivation works
- [x] Ecliptic-to-equatorial coordinate transform (ecliptic.c: verified at 4 cardinal points + Antares)
- [ ] All systems have comprehensive tests
- [ ] Visual representation for each system exists (even if minimal)

**Tasks**:
1. Implement Tzolkin calculation (day sign from Julian Day + GMT correlation)
2. Implement Tzolkin tone calculation (13-day cycle)
3. Implement I Ching hexagram mapping
4. Implement Chinese calendar calculations (stems, branches, animals)
5. Implement Human Design gate calculation from planetary positions
6. Write tests with known date-to-result pairs
7. Create minimal visual representations for each system

**C learning introduced**: Modular arithmetic, lookup tables, enum types, data-driven design

**Files created**: `src/systems/tzolkin/tzolkin.c`, `src/systems/iching/iching.c`, `src/systems/chinese/chinese.c`, `src/systems/human_design/human_design.c`, `data/*/`

---

## Phase 7: Visual Integration

**Goal**: All layers composited into a unified artwork.

**Done when**:
- [ ] All 7 time systems visible simultaneously (layered)
- [ ] Smooth transitions between focused/overview modes
- [ ] Consistent visual language across all layers
- [ ] 60fps maintained with all layers active
- [ ] Color palette and typography finalized
- [ ] Responsive to window resize

**Tasks**:
1. Design visual hierarchy (which layers foreground/background)
2. Implement layer compositing (blend modes, opacity)
3. Create transition animations between views
4. Optimize render pipeline for all layers
5. Finalize color palette and typography
6. Add responsive layout handling
7. Performance profiling and optimization

**C learning introduced**: Render pipeline optimization, framebuffer objects, blend modes, profiling

**Files created**: `src/visual/compositor.c`, `src/visual/transitions.c`, `src/visual/palette.c`

---

## Phase 8: Deployment

**Goal**: Live on the web, accessible via URL.

**Done when**:
- [ ] Hosted on a public URL
- [ ] Works in Chrome, Firefox, Safari (WebGL2 support)
- [ ] Loads in < 3 seconds on broadband
- [ ] WASM binary size optimized (< 1MB ideal)
- [ ] Fallback message for browsers without WebGL2
- [ ] Open Graph / social media preview configured

**Tasks**:
1. Optimize WASM binary size (-Os, --closure 1, strip debug)
2. Set up static hosting (GitHub Pages, Netlify, or Cloudflare Pages)
3. Add loading screen / progress indicator
4. Browser compatibility testing
5. Add WebGL2 detection with fallback message
6. Configure meta tags for social sharing
7. Final performance audit

**C learning introduced**: Release optimization flags, binary size analysis, deployment pipelines

**Files created**: `scripts/deploy.sh`, `web/fallback.html`

---

## Current Status

**Active Phases**: 5 + 6 advancing in parallel
**Last Updated**: 2026-03-06
**Phases complete**: 5/9

**Phase 4 COMPLETE** — all 6/6 criteria met. HUD displays: Gregorian date/time, JD, GMST, LST, Ascendant + MC with signs, Tzolkin Kin, Sun sign, Chinese year, planetary aspects (top 3), speed indicator. All updating in real-time via DOM overlay.

Phase 5: 5/6 criteria met — zodiac ring renders with colored segments, sign glyph labels, planet markers at geocentric positions, aspect chord lines, and house cusp lines. Missing only external validation against astrology software. Phase 6: 5/7 criteria met (Tzolkin, ecliptic, Chinese calendar, I Ching, Human Design all done — missing comprehensive tests checkoff + visual representations). 983 tests across 49 suites, 0 failures. Agent pipeline autonomous; Agent A idle (all geometry delivered), Agent B has 5 discoverable items.
