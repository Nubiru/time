# Time

A visual, interactive, three-dimensional digital artwork that displays time through multiple cultural and astronomical systems.

## Systems

- Astronomy (orbital mechanics, celestial sphere)
- Western Astrology (zodiac, planets, aspects)
- Mayan Tzolkin (260-day sacred calendar)
- I Ching (64 hexagrams)
- Chinese Calendar (stems, branches, lunar phases)
- Human Design (gates, channels)
- Gregorian & Sidereal Time

## Tech Stack

C11 + Emscripten/WebAssembly + WebGL2 + hand-written GLSL shaders.

No frameworks. No engines. Every pixel earned.

## Build

```bash
# Native (local testing)
make native
./build/time

# WebAssembly
make wasm

# Run tests
make test

# Serve locally
make serve
# Open http://localhost:8080
```

## Requirements

- gcc
- Emscripten SDK (emcc)
- make
- Python 3 (for local server)

See `docs/checklists/environment-setup.md` for full setup guide.
