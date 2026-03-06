#!/bin/bash
# Build WASM binary
set -e

mkdir -p build

emcc src/core/main.c -o build/index.html \
    -s USE_WEBGL2=1 \
    -s FULL_ES3=1 \
    -s WASM=1 \
    -Os \
    --shell-file web/index.html

echo "WASM build complete: build/index.html"
