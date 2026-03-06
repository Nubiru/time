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
