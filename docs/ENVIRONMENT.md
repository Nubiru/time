# Environment — Time Project

**Version**: 1.0 **Last Updated**: 2026-03-06

This document is the **complete reproducible setup checklist**. Any developer or Claude agent should be able to set up the project from scratch on a new Linux host by following these steps in order.

---

## 1. Host Requirements

| Property | Current | Minimum |
|----------|---------|---------|
| OS | Pop!_OS (Ubuntu 24.04 based) | Any Debian/Ubuntu-based Linux |
| Kernel | Linux x86_64 | x86_64 |
| RAM | 4GB | 2GB |
| CPU | Intel i5 | Any x86_64 |
| Shell | Bash | Bash |

---

## 2. System Dependencies

Install with `apt`:

```bash
sudo apt update && sudo apt install -y \
    gcc \
    make \
    git \
    python3 \
    inotify-tools \
    lsof \
    calibre
```

### Verification

```bash
gcc --version          # >= 13.x
make --version         # >= 4.3
git --version          # >= 2.43
python3 --version      # >= 3.12
inotifywait --help     # inotify-tools (for watch.sh)
lsof -v                # for make dev (port cleanup)
ebook-convert --version  # calibre (for EPUB/DOC -> PDF conversion)
```

### What each tool does

| Tool | Purpose |
|------|---------|
| `gcc` | C11 compiler for native builds and tests |
| `make` | Build system (Makefile targets) |
| `git` | Version control |
| `python3` | Local HTTP server (`python3 -m http.server`) |
| `inotify-tools` | File watcher for `scripts/watch.sh` auto-rebuild |
| `lsof` | Port detection for `make dev` stale server cleanup |
| `calibre` | `ebook-convert` CLI for EPUB/DOC to PDF conversion (research pipeline) |

---

## 3. Emscripten SDK

```bash
# Clone emsdk (outside project repo)
cd ~
git clone https://github.com/emscripten-core/emsdk.git

# Install and activate latest
cd ~/emsdk
./emsdk install latest
./emsdk activate latest

# Add to shell profile for persistence
echo 'source "$HOME/emsdk/emsdk_env.sh"' >> ~/.bashrc
source ~/.bashrc
```

### Verification

```bash
emcc --version         # >= 5.0.x
node --version         # bundled with emsdk, used internally by emcc
```

**Note**: `node` is bundled with emsdk at `~/emsdk/node/`. It is used internally by `emcc` only — we do NOT use Node.js, npm, or any JS build tools in this project.

---

## 4. Git Configuration

```bash
# Generate SSH key for GitHub
ssh-keygen -t ed25519 -f ~/.ssh/id_ed25519_nubiru -C "nubiru@github"

# Add to SSH config
cat >> ~/.ssh/config << 'EOF'
Host github.com-nubiru
    HostName github.com
    User git
    IdentityFile ~/.ssh/id_ed25519_nubiru
EOF

# Configure git
git config --global user.name "Nubiru"
git config --global user.email "your-email@example.com"
```

| Property | Value |
|----------|-------|
| GitHub Account | Nubiru |
| Repo | github.com/Nubiru/time |
| SSH Host Alias | github.com-nubiru |
| SSH Key | ~/.ssh/id_ed25519_nubiru |
| Remote URL | git@github.com-nubiru:Nubiru/time.git |
| Default Branch | main |

---

## 5. Project Setup

```bash
# Clone the repo
git clone git@github.com-nubiru:Nubiru/time.git
cd time

# Create build directory
mkdir -p build

# Verify native build
make native && ./build/time
# Should print: "Time - A Visual Study"

# Verify tests
make test
# Should show all tests passing (53+ tests, 0 failures)

# Verify WASM build
source ~/emsdk/emsdk_env.sh   # if not in .bashrc yet
make wasm
# Should compile without errors

# Verify server
make serve
# Open http://localhost:8080 in browser
# Should see colored triangle on black background
```

---

## 6. Unity Test Framework

Already vendored in `tests/unity/` (3 files). No install needed.

Source: https://github.com/ThrowTheSwitch/Unity

To update (if ever needed):
```bash
# Download latest unity.c, unity.h, unity_internals.h
# Place in tests/unity/
```

---

## 7. Editor

VS Code with C/C++ extension (optional — any editor works).

---

## 8. Compiler Flags

| Build | Flags |
|-------|-------|
| Native | `-Wall -Wextra -Werror -std=c11 -pedantic -lm` |
| Debug | add `-g -O0 -DDEBUG` |
| Release | add `-O2 -DNDEBUG` |
| WASM | `-Os -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1` |

---

## 9. Build Targets

| Target | What it does |
|--------|-------------|
| `make native` | Compile core with gcc (no GL — for math/tests only) |
| `make wasm` | Compile all sources with emcc to WebAssembly |
| `make test` | Compile and run all Unity tests with gcc |
| `make test-vec3` | Run vec3 tests only |
| `make test-mat4` | Run mat4 tests only |
| `make test-julian` | Run Julian Day tests only |
| `make test-kepler` | Run Kepler equation tests only |
| `make test-sidereal` | Run sidereal time tests only |
| `make dev` | Kill stale server + build WASM + serve |
| `make serve` | Start HTTP server on :8080 |
| `make clean` | Remove build artifacts |

---

## 10. Development Workflow

**Two-terminal mode** (preferred):
- **Terminal 1**: `make serve` — persistent HTTP server on :8080
- **Terminal 2**: `./scripts/watch.sh` — auto-rebuilds WASM on file save

On file change (.c, .h, .glsl, .html, .css), watch.sh runs `make wasm` automatically. Refresh browser to see changes.

**One-shot mode**: `make dev` — kills stale server + rebuilds WASM + serves.

---

## 11. Research Pipeline

| Tool | Purpose |
|------|---------|
| NotebookLM (Google) | Extract knowledge from books via Q&A cards |
| calibre (`ebook-convert`) | Convert EPUB/DOC to PDF for NotebookLM |
| libreoffice (`--headless`) | Alternative DOC to PDF converter |

**Book files**: `~/Desktop/temp/time/`
**Q&A cards**: `.context/research/prompts/NNN-topic.md`
**Method**: Chat mode in NotebookLM. One focused question at a time.

### Converting books for NotebookLM

```bash
# EPUB to PDF
ebook-convert input.epub output.pdf

# DOC to PDF (calibre)
ebook-convert input.doc output.pdf

# DOC to PDF (libreoffice alternative)
libreoffice --headless --convert-to pdf input.doc
```

---

## 12. Known Issues

- `emcc` produces clang warnings about libstdc++ include directories (gcc 13 vs 14). Harmless informational warnings from the Emscripten toolchain, not from our code.
- First `emcc` run after emsdk install triggers cache regeneration (normal, one-time).
- `M_PI` is not available under strict `-std=c11 -pedantic`. Define `PI` manually in source files that need it.

---

## 13. Build Status (verified 2026-03-06)

| Target | Status | Notes |
|--------|--------|-------|
| `make native` | WORKS | `./build/time` prints "Time - A Visual Study" |
| `make wasm` | WORKS | WebGL2 triangle renders at 60fps |
| `make test` | WORKS | 53 tests, 0 failures |
| `make serve` | WORKS | http://localhost:8080 |
| `make dev` | WORKS | One-shot build+serve |
| `scripts/watch.sh` | WORKS | Auto-rebuild on file save |
