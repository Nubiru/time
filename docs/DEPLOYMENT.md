# Deployment Guide

**Version**: 1.0
**Last Updated**: 2026-03-17

---

## Build Environments

| Environment | Flag | Optimization | Assertions | Use |
|-------------|------|-------------|------------|-----|
| Development | (default) | `-Os` | Level 2 | Local testing |
| Staging | `-DSTAGING=ON` | `-O2 -g2` | Level 1 | Pre-release validation |
| Production | `-DPRODUCTION=ON` | `-O3 --closure 1 -flto` | Off | Public deployment |

---

## Build Commands

```bash
# Development (default)
emcmake cmake -B build-wasm
cmake --build build-wasm

# Staging
emcmake cmake -B build-wasm -DSTAGING=ON
cmake --build build-wasm

# Production
emcmake cmake -B build-wasm -DPRODUCTION=ON
cmake --build build-wasm
```

---

## Build Output

After building, `build-wasm/` contains:

| File | Purpose | Typical Size |
|------|---------|-------------|
| `index.html` | Shell page (processed from `web/index.html`) | ~4 KB |
| `index.js` | Emscripten runtime + glue code | ~90 KB |
| `index.wasm` | Compiled C binary | ~220 KB (dev) |
| `style.css` | Copied from `web/style.css` | ~16 KB |

**Total bundle**: ~330 KB uncompressed, ~100 KB gzipped.

---

## Required Files for Deployment

Upload these 4 files to your static host:

1. `build-wasm/index.html`
2. `build-wasm/index.js`
3. `build-wasm/index.wasm`
4. `build-wasm/style.css`

All files must be served from the same directory. No subdirectories needed.

---

## Static Hosting Setup (Cloudflare Pages)

1. Connect GitHub repo (`Nubiru/time`) to Cloudflare Pages
2. Build settings:
   - **Build command**: `emcmake cmake -B build-wasm -DPRODUCTION=ON && cmake --build build-wasm`
   - **Build output directory**: `build-wasm`
   - **Environment variable**: `EMSDK_VERSION=3.1.51`
3. Deploy triggers: push to `main`

### MIME Types

Ensure your host serves correct MIME types:

| Extension | MIME Type |
|-----------|-----------|
| `.wasm` | `application/wasm` |
| `.js` | `application/javascript` |
| `.html` | `text/html` |
| `.css` | `text/css` |

Most static hosts handle this automatically. Cloudflare Pages does.

### Headers

Recommended response headers for performance:

```
Cache-Control: public, max-age=31536000, immutable  # .wasm, .js, .css
Cache-Control: no-cache                              # index.html
Cross-Origin-Opener-Policy: same-origin              # Required for SharedArrayBuffer (future)
Cross-Origin-Embedder-Policy: require-corp           # Required for SharedArrayBuffer (future)
```

---

## Binary Size Budget

| Component | Budget | Current |
|-----------|--------|---------|
| `index.wasm` | < 2 MB | ~220 KB |
| `index.js` | < 500 KB | ~90 KB |
| Total (gzip) | < 500 KB | ~100 KB |

Monitor with CI: the WASM job reports raw and gzipped sizes on every push.

---

## Pre-Deploy Checklist

- [ ] All tests pass: `cmake --build build-native && ctest --test-dir build-native -j12`
- [ ] WASM builds clean: `emcmake cmake -B build-wasm -DPRODUCTION=ON && cmake --build build-wasm`
- [ ] No compiler warnings (enforced by `-Werror`)
- [ ] Binary size within budget (check CI output)
- [ ] Local browser test: `python3 -m http.server 8080 -d build-wasm` then open `localhost:8080`
- [ ] WebGL2 renders correctly (canvas visible, no console errors)
- [ ] HUD displays time data
- [ ] Keyboard controls respond (arrow keys, number keys, ?, L, E)
- [ ] CI pipeline green (native + sanitizer + WASM)

---

## CI Pipeline

GitHub Actions runs on every push to `main`:

1. **Native Build + Tests**: gcc compile + CTest + address/undefined sanitizer
2. **WASM Build + Size Check**: emcc compile + binary size report

Both must pass before deployment.

---

## Rollback

Static hosting with immutable assets. To rollback:
- Redeploy previous commit's build artifacts
- Or revert commit on `main` and let CI rebuild

No database migrations. No backend state. Rollback is always safe.
