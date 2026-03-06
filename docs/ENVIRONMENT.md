# Environment — Time Project

**Version**: 1.0 **Last Updated**: 2026-03-05

---

## Host System

| Property | Value |
|----------|-------|
| OS | Pop!_OS (Ubuntu 24.04 based) |
| Kernel | Linux x86_64 |
| RAM | 4GB |
| CPU | Intel i5 |
| Shell | Bash |

---

## Toolchain

| Tool | Version | Location | Notes |
|------|---------|----------|-------|
| gcc | 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04) | system | C11 compiler for native builds |
| make | 4.3 | system | Build system |
| git | 2.43.0 | system | Version control |
| python3 | 3.12.3 | system | Local HTTP server (`python3 -m http.server`) |
| emcc | 5.0.2 | ~/emsdk/upstream/emscripten | Emscripten C-to-WASM compiler |
| emsdk | latest (5.0.2) | ~/emsdk | Emscripten SDK manager |
| node | 22.16.0 | ~/emsdk/node/ | Bundled with emsdk (used internally by emcc) |

---

## Compiler Flags

| Build | Flags |
|-------|-------|
| Native | `-Wall -Wextra -Werror -std=c11 -pedantic -lm` |
| Debug | add `-g -O0 -DDEBUG` |
| Release | add `-O2 -DNDEBUG` |
| WASM | `-Os -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1` |

---

## Emscripten Setup

```bash
# emsdk installed at ~/emsdk (NOT inside the project repo)
# Activate in shell:
source ~/emsdk/emsdk_env.sh

# Or add to ~/.bashrc for persistence:
echo 'source "$HOME/emsdk/emsdk_env.sh"' >> ~/.bashrc
```

---

## Git Configuration

| Property | Value |
|----------|-------|
| GitHub Account | Nubiru |
| Repo | github.com/Nubiru/time |
| SSH Host Alias | github.com-nubiru |
| SSH Key | ~/.ssh/id_ed25519_nubiru |
| Remote URL | git@github.com-nubiru:Nubiru/time.git |
| Default Branch | main |

---

## Test Framework

| Framework | Version | Location |
|-----------|---------|----------|
| Unity (C test) | latest | tests/unity/ (vendored, 3 files) |

Source: https://github.com/ThrowTheSwitch/Unity

---

## Editor

VS Code with C/C++ extension.

---

## Known Warnings

- `emcc` produces clang warnings about libstdc++ include directories (gcc 13 vs 14). These are harmless informational warnings from the Emscripten toolchain, not from our code.
