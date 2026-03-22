# Time

A visual, interactive, three-dimensional digital artwork that displays time through multiple cultural and astronomical systems.

**Every pixel earned.** C11 + Emscripten/WebAssembly + raw WebGL2 + hand-written GLSL shaders. No frameworks. No engines.

---

## What Is Time?

Time reveals the hidden connections between humanity's ways of measuring existence. 25+ knowledge systems — from Mayan Tzolkin to I Ching, from Hindu Panchanga to Hebrew Kabbalah — displayed simultaneously as an interactive cosmic experience.

Every calendar system, every astronomical cycle, every cultural tradition of measuring time is a lens on the same underlying truth. Time makes that truth visible.

## What You See

- A dark sky with twinkling stars and a luminous Milky Way with dust lanes
- Planets at computed orbital positions with god rays from the Sun
- Knowledge system cards showing what today means across 25 traditions
- Sacred geometry overlays: natal chart wheel, Tree of Life, bagua, 231 Gates mandala
- A daily narrative woven from convergences across independent systems
- 228 wisdom quotes from 20+ voices (Lao Tzu, Marcus Aurelius, Rumi, Tesla, Gandhi...)
- Your birth moment: enter your date, see your identity in 7 time systems

## Build

```bash
# Native (development + tests)
make build        # compile with gcc
make test         # run all tests

# Browser (WebAssembly)
make serve        # build WASM + serve at localhost:8080
```

### Requirements

- gcc 13+ with C11 support
- cmake 3.28+
- Emscripten SDK (emcc 5.0+) for WASM builds
- Python 3.12+ for local server + E2E tests
- Playwright (optional, for E2E visual tests)

## Interact

| Key | What It Does |
|-----|-------------|
| K | Kin Maya — oracle cross + wavespell |
| I | I Ching — hexagram + judgment |
| A | Astrology — natal chart wheel |
| D | Human Design — Sun/Earth gates |
| T | Kabbalah — Tree of Life + 231 Gates |
| B | Birth sky — travel to your birth moment |
| P | Enter birth date |
| ←→↑↓ | Navigate through time (±1 day, ±1 month) |
| N | Return to now |
| L | Cycle languages (12 available) |
| M | Toggle audio |
| ? | Help |
| , | Settings |
| Space | Pause/resume |

Full controls: [docs/USER_CONTROLS.md](docs/USER_CONTROLS.md)

## Architecture

```
src/
├── core/          main.c, app_state, input handling
├── math/          vec3, mat4, julian day, kepler, easing
├── render/        28 render passes, shaders, camera, post-processing
│   └── passes/    star, planet, milkyway, card, text, natal_chart,
│                  tree_of_life, bagua, gates_mandala, convergence...
├── systems/       25 knowledge system domains
│   ├── astronomy/     orbital mechanics, precession, eclipses
│   ├── astrology/     zodiac, houses, aspects, dignity
│   ├── tzolkin/       Dreamspell, wavespells, oracle, Telektonon
│   ├── iching/        64 hexagrams, trigrams, King Wen sequence
│   ├── kabbalah/      10 Sefirot, 22 paths, 231 Gates, Tree geometry
│   ├── chinese/       stems, branches, animals, elements
│   ├── hebrew/        lunisolar calendar, months, tribes, letters
│   ├── islamic/       Hijri, prayer times, Qibla
│   ├── buddhist/      Uposatha, Kalpas, dharma wheel
│   ├── hindu/         Panchanga, Nakshatras, Yugas, chakras
│   ├── human_design/  bodygraph, gates, channels, types
│   ├── geology/       deep time, tectonics, fossils, climate
│   ├── earth/         tides, seasons, weather, population
│   └── unified/       cross-system convergence, brain, narrative
├── ui/            themes, cards, layouts, i18n, content
└── platform/      storage bridge (native + WASM)
```

## The Numbers

| Metric | Value |
|--------|-------|
| Source files | 510+ .c / .h pairs |
| Tests | 537+ (CTest, all pass) |
| Lines of code | 120K+ |
| Render passes | 28 |
| Knowledge systems | 25 |
| Languages | 12 |
| Wisdom quotes | 228 from 20+ voices |
| Book digests | 161 (from 204-book library) |
| WASM binary | ~780 KB |

## Philosophy

> "This cannot be built fast. I do not mean for us to rush at all. The whole idea is that we enjoy this build. I am willing for this project to take years."

Time honors every tradition equally. No hierarchy of calendars. The Tzolkin is not more valid than the Gregorian, and the I Ching is not less real than orbital mechanics. Each is a human attempt to find pattern in the cosmos.

The code is part of the artwork. Every orbit is solved with Kepler's equation. Every phase is computed from lunar mechanics. Every convergence is detected, not invented.

## Attribution

Every algorithm in this codebase came from a human mind. 230+ contributors are honored in [data/contributors.json](data/contributors.json).

## License

Copyright © 2026 Gabriel. All rights reserved.

---

*Built by Gabriel + Claude. C11. WebGL2. GLSL. Every pixel earned.*
