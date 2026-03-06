# Time — Project Genesis

**Date**: 2026-03-05
**Purpose**: Record of the founding decisions, vision, and technology choices made during the initial project conversation.

---

## Vision

Time is a visual, interactive, three-dimensional digital artwork that displays time through multiple cultural and astronomical systems. It is not a clock app, not a utility — it is a piece of art that happens to tell time in ways most people have never seen.

The artwork renders seven time systems simultaneously:
1. **Astronomy** — Planetary positions, orbital mechanics, celestial sphere
2. **Western Astrology** — Zodiac wheel, planetary dignities, aspects, houses
3. **Mayan Tzolkin** — 20 day signs x 13 tones (260-day sacred calendar)
4. **I Ching** — 64 hexagrams mapped to temporal cycles
5. **Chinese Calendar** — Heavenly Stems, Earthly Branches, lunar phases
6. **Human Design** — Gates, channels, types derived from planetary positions
7. **Gregorian/Sidereal** — Standard calendar plus astronomical time (GST, LST, Julian Day)

---

## Inspiration Analysis

### dark.netflix.io
- Dark, atmospheric visual aesthetic
- Particle systems and ambient motion
- Time as a central theme
- Layered visual depth

### igloo.inc
- Technical excellence in WebGL
- Smooth interactions
- Performance-first approach
- Art-direction-driven development

### Key takeaways for Time
- Dark background (near-black, #080808 range)
- Subtle motion even when "idle"
- Performance is non-negotiable (60fps)
- Every visual element earns its pixel

---

## Technology Path Chosen

**Path A: C + Emscripten + WebGL2** (CHOSEN)

### Why C
- Maximum control over memory and performance
- Direct access to OpenGL ES 3.0 via Emscripten
- No runtime overhead (no GC, no JIT)
- Forces understanding of fundamentals
- Gabriel wants to learn C — this project is the vehicle
- The constraint IS the creative medium

### Why Emscripten/WebAssembly
- Deploys to any modern browser
- Near-native performance in the browser
- Well-documented C-to-WASM pipeline
- `emscripten_set_main_loop()` integrates with browser frame cycle

### Why raw WebGL2 (no Three.js, no engine)
- Every shader is hand-written — the art is in the code
- No framework opinions to fight
- Direct GPU control for visual effects
- Learning WebGL deeply, not learning an API wrapper
- Smaller binary size

### Alternatives Considered and Rejected

**Path B: Rust + wasm-pack + WebGL2**
- Rejected: Steeper learning curve than C for a first systems language
- Rust's borrow checker adds cognitive load on top of learning low-level programming
- C is more fundamental — understanding C makes Rust easier later

**Path C: JavaScript + Three.js**
- Rejected: Gabriel already knows JS/TS
- No new learning, no creative constraint
- Three.js imposes its scene graph model
- Performance ceiling lower than WASM for compute-heavy astronomy

**Path D: C++ + Emscripten**
- Rejected: C++ complexity unnecessary for this project
- Classes, templates, RAII — overhead without proportional benefit
- C11 is sufficient for everything we need

---

## Architecture Decisions

### Source Structure
- `src/` divided by architectural layer (core, math, systems, render, visual, ui, platform)
- Each knowledge system in its own subdirectory under `src/systems/`
- Math layer is purely functional — no state, no I/O
- Render layer handles GPU, doesn't compute positions

### Data Flow
- Julian Day Number is the universal time representation
- All systems receive JD, compute their outputs independently
- Visual layer composites all system outputs
- Render layer issues draw calls

### Testing
- Unity framework (C unit testing, not the game engine)
- Tests run natively with gcc — no browser needed
- Math and systems layers are highly testable (pure functions)
- Render layer tested by visual inspection in browser

---

## Data Sources

| Source | System | Reference |
|--------|--------|-----------|
| JPL Orbital Elements | Astronomy | Keplerian approximation for solar system bodies |
| Meeus "Astronomical Algorithms" | Astronomy, Sidereal Time | Standard algorithms for JD, GST, planetary positions |
| GMT Correlation Constant 584283 | Tzolkin | Thompson correlation, most widely accepted |
| King Wen Sequence | I Ching | Traditional 64-hexagram arrangement |
| IAU GST Formula | Sidereal Time | International Astronomical Union standard |
| Reingold/Dershowitz "Calendrical Calculations" | Chinese Calendar, Gregorian | Authoritative calendar algorithms |

---

## Development Environment

- **OS**: Linux, 4GB RAM, i5 processor
- **Compiler**: gcc (native), emcc (WASM)
- **Editor**: VS Code with C/C++ extension
- **Background**: Gabriel has experience in JavaScript/TypeScript. This is his first C project.

---

## Philosophy

The code is part of the artwork. Writing C by hand, writing shaders by hand, understanding every byte that reaches the GPU — this discipline IS the creative process. The constraints (no frameworks, no engines, limited RAM) are not obstacles. They are the medium.
