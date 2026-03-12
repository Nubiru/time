# Architecture Decision Records — Time

---

## ADR-001: C + Emscripten + WebGL2

**Date**: 2026-03-05
**Status**: Accepted

### Context

Time is a visual artwork that renders multiple time systems in 3D. Needs to run in browsers with high performance (60fps). Gabriel wants to learn a systems language.

### Decision

Use C11 compiled to WebAssembly via Emscripten, with raw WebGL2 for rendering and hand-written GLSL shaders. No frameworks or engines.

### Alternatives Considered

1. **Rust + wasm-pack**: Rejected — steeper learning curve (borrow checker) on top of learning systems programming. C is more fundamental.
2. **JavaScript + Three.js**: Rejected — no new learning, Three.js imposes its model, lower performance ceiling.
3. **C++ + Emscripten**: Rejected — unnecessary complexity (classes, templates, RAII). C11 is sufficient.

### Consequences

**Positive**:
- Maximum performance control
- Deep learning of fundamentals (memory, pointers, GPU)
- Small binary size
- Every line of code is understood

**Negative**:
- Slower initial velocity (learning C)
- No safety net (manual memory management)
- More code for things frameworks give free (scene graphs, text rendering)
- Debugging WASM is harder than native JS

### Rationale

The constraints are the creative medium. Writing C and shaders by hand forces understanding of every byte reaching the GPU. The slower initial velocity is an investment in deep knowledge.

---

## ADR-002: Tzolkin — Arguelles Only

**Date**: 2026-03-05
**Status**: Accepted

### Context

The Mayan Tzolkin/Kin system has two major interpretive traditions: (1) the academic/archaeological view based on epigraphy and classical Maya studies, and (2) Jose Arguelles' Dreamspell interpretation based on the Law of Time, 13:20 frequency, and galactic cosmology.

### Decision

Time follows Jose Arguelles' interpretation exclusively. We implement his Dreamspell Tzolkin, not the academic reconstruction. All books, data sources, and algorithmic references come from Arguelles' work.

### Rationale

This is an artwork, not an academic exercise. Arguelles' vision is coherent, internally consistent, and deeply connected to the project's theme of time as lived experience. The 13:20 frequency concept is central to how Time presents non-Gregorian time awareness.

### Key Sources

- "The Mayan Factor" (foundational)
- "Earth Ascending" (cosmology)
- "Time and the Technosphere" (Law of Time theory)
- "Surfers of the Zuvuya", "The Arcturus Probe", "Manifesto for the Noosphere"

---

## ADR-003: Astrology — Include Jung's Perspective

**Date**: 2026-03-05
**Status**: Accepted

### Context

Western astrology can be approached as (1) predictive/horoscopic tradition, (2) psychological/archetypal framework, or (3) astronomical observation mapped to symbolic meaning.

### Decision

Time incorporates Carl Jung's view of astrology as a framework of synchronicity, archetypes, and the collective unconscious. Astrology in Time is not fortune-telling — it is a lens for seeing patterns of meaning in time.

### Rationale

Jung's concept of synchronicity ("meaningful coincidence") connects astrology to the broader theme of Time: that multiple systems of time measurement reveal different layers of meaning in the same moment. This aligns with the artwork's intent.

### Key Sources

- "Jung on Astrology" — Safron Rossi, Keiron Le Grice (collected writings)
- "Synchronicity: An Acausal Connecting Principle" — C.G. Jung

### Consequences

The astrology layer should visualize archetypal patterns and synchronistic connections, not generate horoscope predictions. Planet positions are astronomical facts; their interpretation follows the Jungian archetypal framework.

---

## ADR-004: Golden Ratio Design System

**Date**: 2026-03-06
**Status**: Accepted

### Decision

phi (1.618...) governs EVERY visual proportion. No magic numbers. Typography, spacing, screen layout, card dimensions, animation timing, opacity cascade, ring proportions, zoom factors — all derived from phi. Brand colors: Solar gold {1.0, 0.85, 0.55} + Celestial teal {0.2, 0.75, 0.8}. Space-black #060709 not #000000. Golden angle 137.508 degrees for color palette spacing.

### Files

- `src/ui/golden_layout.h` — spatial proportions
- `src/render/color_theory.h` — color science + palettes
- `src/ui/theme.h` — unified design token system (Cosmos + Dawn themes)

---

## ADR-005: Render Pass Architecture (Level 2)

**Date**: 2026-03-08
**Status**: Accepted

### Context

main.c was 960 lines of mixed GL code. app_state_t carried 75 lines of GL handle fields. Every new visual feature required modifying main.c in 2+ places, creating coupling and merge conflicts between agents.

### Decision

Extract GL into self-contained pass modules (`src/render/passes/`). Each pass exports `xxx_pass_init()`, `xxx_pass_draw(const render_frame_t *)`, `xxx_pass_destroy()`. GL handles live as module-static `s_` prefix variables. No vtables, no registration — direct function calls in explicit draw order. `render_frame_t` built once per frame, passed by const pointer.

### Consequences

- main.c: 960 -> 108 lines. app_state.h: GL-free.
- Wiring a new visual is 3 lines in main.c + 1 CMake entry.
- Module-static GL handles follow the input.c precedent (resource IDs, not application state).
- Three initial passes: star_pass, planet_pass, zodiac_pass.

---

## ADR-006: Art-First Rendering — Cinematic Visual Quality Pivot

**Date**: 2026-03-08
**Status**: Accepted

### Context

After 147+ agent deliveries, 172 test suites, and 16+ calendar systems, the visual output remains a prototype: flat yellow Sun sphere, colored planet dots, basic zodiac ring. The data engine is massive but the art is not visible. Gabriel's vision: "game quality rendering but as real as we can make it." The Sun should be living plasma, planets should have atmosphere and surface detail, the Milky Way should feel cinematic.

### Decision

**Pivot ALL rendering work toward cinematic visual quality.** Every pixel must be earned art. Specifically:

1. **Procedural Sun shader**: Animated Perlin noise plasma, corona rays, solar prominences. Not a yellow sphere — a living star.
2. **Planetary atmosphere shaders**: Rayleigh scattering, limb glow, latitude-dependent gas giant banding, day/night terminators.
3. **Earth View mode**: AR-style star map from observer's perspective. Sun = daylight, Moon = realistic phase and position, constellations as seen from the ground.
4. **Milky Way**: Volumetric band across the sky, not a placeholder spiral.
5. **Post-processing**: Bloom/glow pass, HDR-like tone mapping, anti-aliased lines.
6. **Everything uses phi**: Golden ratio design system (ADR-004) governs all proportions, spacing, timing, opacity. Brand colors throughout.

### Constraints

- WebGL2 / GLSL ES 3.00 (no compute shaders, no geometry shaders)
- Must maintain 60fps
- Procedural over textures (smaller binary, infinite resolution)
- All techniques researched from real references (GPU Gems, Shadertoy, NASA imagery)

### Agent Role Changes

- **MEGA**: Architecture, shader authorship, the art itself. Highest-impact visual work.
- **OMEGA**: Expanded to include wiring delivered packs, git commits, maintenance. The finisher.
- **DELTA**: Expanded to include shader technique research (Shadertoy, papers, books) alongside book mining.
- **ALPHA**: Shader data packs — procedural noise functions, atmosphere models, as pure C modules.
- **BETA/GAMMA**: Continue calendar/data work, UI data formatting.

### Books / Resources Needed

- "The Book of Shaders" (Gonzalez Vivo) — procedural noise, fire, atmosphere
- "GPU Gems 2, Ch.16" — accurate atmospheric scattering (Preetham/O'Neil)
- "Texturing & Modeling: A Procedural Approach" (Ebert et al) — procedural planets
- Shadertoy implementations: solar fire, planetary atmospheres, Milky Way
- NASA/ESA texture references for visual accuracy (public domain)

### Consequences

- Agent work shifts from "build more invisible data modules" to "make what exists beautiful"
- Wiring work delegated to OMEGA, freeing MEGA for art
- DELTA's scope grows to include shader/technique research
- Visual progress becomes the primary success metric, not test count
