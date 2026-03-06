#!/bin/bash
# Auto-rebuild WASM on file changes (like nodemon for C)
# Requires: sudo apt install inotify-tools
#
# Usage: Run in one terminal, keep `make serve` in another.
#        Just refresh the browser after rebuild.

SOURCE_EMSDK="source $HOME/emsdk/emsdk_env.sh 2>/dev/null"

echo "Watching src/ shaders/ web/ for changes..."
echo "Press Ctrl+C to stop"
echo ""

while true; do
    inotifywait -r -e modify,create,delete \
        --include '\.(c|h|glsl|vert|frag|html|css)$' \
        src/ shaders/ web/ 2>/dev/null

    echo ""
    echo "--- Change detected, rebuilding WASM... ---"
    bash -c "$SOURCE_EMSDK && make wasm" 2>&1
    if [ $? -eq 0 ]; then
        echo "--- Build OK. Refresh browser. ---"
    else
        echo "--- BUILD FAILED ---"
    fi
    echo ""
done
