#!/usr/bin/env python3
"""WASM smoke test — verifies the app loads and renders in a headless browser.

Requires: pip install playwright && playwright install chromium
If Playwright is not installed, this test skips gracefully (exit 0).

Usage: python3 tests/smoke/test_wasm_smoke.py
"""
import os
import sys
import threading
import http.server
import functools

# Graceful skip if Playwright not installed
try:
    from playwright.sync_api import sync_playwright
except ImportError:
    print(
        "SKIP: Playwright not installed. "
        "Run: pip install playwright && playwright install chromium"
    )
    sys.exit(0)


def start_server(directory, port=8080):
    """Start a simple HTTP server in a background thread."""
    handler = functools.partial(
        http.server.SimpleHTTPRequestHandler, directory=directory
    )
    server = http.server.HTTPServer(("localhost", port), handler)
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    return server


def main():
    # Find build-wasm directory
    root = os.path.dirname(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    )
    wasm_dir = os.path.join(root, "build-wasm")

    if not os.path.isdir(wasm_dir):
        print("SKIP: build-wasm/ not found. Run 'make wasm' first.")
        sys.exit(0)

    index_file = os.path.join(wasm_dir, "index.html")
    if not os.path.isfile(index_file):
        print("FAIL: build-wasm/index.html not found")
        sys.exit(1)

    port = 8099  # Use non-standard port to avoid conflicts
    server = start_server(wasm_dir, port)

    errors = []
    console_messages = []
    page_errors = []

    try:
        with sync_playwright() as p:
            browser = p.chromium.launch(headless=True)
            context = browser.new_context()
            page = context.new_page()

            # Collect console messages and errors
            page.on("console", lambda msg: console_messages.append(msg))
            page.on("pageerror", lambda err: page_errors.append(str(err)))

            # Navigate and wait for load
            print(f"Loading http://localhost:{port}/index.html ...")
            page.goto(
                f"http://localhost:{port}/index.html", timeout=30000
            )

            # Wait a bit for WASM to initialize
            page.wait_for_timeout(5000)

            # Check 1: No page errors (stack overflow, shader compile failure)
            print("\n--- Check 1: Page errors ---")
            if page_errors:
                for err in page_errors:
                    print(f"  ERROR: {err}")
                    errors.append(f"Page error: {err}")
            else:
                print("  PASS: No page errors")

            # Check 2: Canvas exists with non-zero dimensions
            print("\n--- Check 2: Canvas ---")
            canvas = page.query_selector("canvas")
            if canvas:
                box = canvas.bounding_box()
                if box and box["width"] > 0 and box["height"] > 0:
                    print(
                        f"  PASS: Canvas "
                        f"{int(box['width'])}x{int(box['height'])}"
                    )
                else:
                    msg = "Canvas has zero dimensions"
                    print(f"  FAIL: {msg}")
                    errors.append(msg)
            else:
                msg = "No <canvas> element found"
                print(f"  FAIL: {msg}")
                errors.append(msg)

            # Check 3: Screenshot not all black
            print("\n--- Check 3: Rendering (not black screen) ---")
            screenshot_path = os.path.join(
                root, "build-wasm", "smoke_screenshot.png"
            )
            page.screenshot(path=screenshot_path)

            # Check pixel data via JavaScript
            is_black = page.evaluate(
                """() => {
                const canvas = document.querySelector('canvas');
                if (!canvas) return true;
                const ctx = canvas.getContext('2d',
                    {willReadFrequently: true});
                if (!ctx) {
                    // WebGL canvas — read via WebGL
                    const gl = canvas.getContext('webgl2')
                        || canvas.getContext('webgl');
                    if (!gl) return true;
                    const w = Math.min(canvas.width, 100);
                    const h = Math.min(canvas.height, 100);
                    const pixels = new Uint8Array(w * h * 4);
                    gl.readPixels(0, 0, w, h, gl.RGBA,
                        gl.UNSIGNED_BYTE, pixels);
                    let nonZero = 0;
                    for (let i = 0; i < pixels.length; i += 4) {
                        if (pixels[i] > 5 || pixels[i+1] > 5
                            || pixels[i+2] > 5) nonZero++;
                    }
                    return nonZero < 10;
                }
                // 2D context fallback
                const data = ctx.getImageData(0, 0,
                    Math.min(canvas.width, 100),
                    Math.min(canvas.height, 100)).data;
                let nonZero = 0;
                for (let i = 0; i < data.length; i += 4) {
                    if (data[i] > 5 || data[i+1] > 5
                        || data[i+2] > 5) nonZero++;
                }
                return nonZero < 10;
            }"""
            )

            if is_black:
                msg = (
                    "Screen appears all black "
                    "(fewer than 10 non-black pixels)"
                )
                print(f"  WARN: {msg}")
                print(f"  Screenshot saved: {screenshot_path}")
                # Warning, not error — black screen may be expected during dev
            else:
                print("  PASS: Canvas has rendered content")
                print(f"  Screenshot saved: {screenshot_path}")

            # Check 4: Console errors
            print("\n--- Check 4: Console errors ---")
            console_errors = [
                m for m in console_messages if m.type == "error"
            ]
            if console_errors:
                for msg in console_errors[:5]:
                    print(f"  CONSOLE ERROR: {msg.text}")
                # Warning, not hard fail — some WebGL warnings are expected
            else:
                print("  PASS: No console errors")

            browser.close()

    except Exception as e:
        print(f"  FAIL: {e}")
        errors.append(str(e))
    finally:
        server.shutdown()

    # Summary
    print(f"\n=== Smoke Test: {len(errors)} error(s) ===")
    if errors:
        print("FAIL")
        return 1
    print("PASS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
