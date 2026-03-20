"""Shared fixtures for E2E tests.

Provides: server (WASM HTTP server), browser (headless Chromium with WebGL),
page (navigated + enter screen dismissed), screenshot utilities.

Requires: playwright, Pillow
Run: python3 -m pytest tests/e2e/ -v
"""

import subprocess
import time
import signal
import os
import io
import pytest
from playwright.sync_api import sync_playwright
from PIL import Image

WASM_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "build-wasm")
PORT = 8089
SCREENSHOT_DIR = os.path.join(os.path.dirname(__file__), "screenshots")
BASELINE_DIR = os.path.join(os.path.dirname(__file__), "baselines")


@pytest.fixture(scope="session")
def server():
    """Start a local HTTP server serving the WASM build."""
    abs_dir = os.path.abspath(WASM_DIR)
    if not os.path.isfile(os.path.join(abs_dir, "index.html")):
        pytest.skip("WASM build not found — run: emcmake cmake -B build-wasm && cmake --build build-wasm")

    proc = subprocess.Popen(
        ["python3", "-m", "http.server", str(PORT), "-d", abs_dir],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        preexec_fn=os.setsid,
    )
    time.sleep(0.5)
    yield f"http://localhost:{PORT}"
    os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
    proc.wait(timeout=5)


@pytest.fixture(scope="session")
def browser_context(server):
    """Launch headless Chromium with WebGL support (SwiftShader)."""
    with sync_playwright() as pw:
        browser = pw.chromium.launch(
            headless=True,
            args=[
                "--use-gl=angle",
                "--use-angle=swiftshader",
                "--enable-unsafe-swiftshader",
            ],
        )
        context = browser.new_context(
            viewport={"width": 1280, "height": 720},
            record_video_dir=os.path.join(SCREENSHOT_DIR, "videos"),
        )
        yield context, server
        context.close()
        browser.close()


@pytest.fixture
def page(browser_context):
    """Fresh page with WASM loaded and enter screen dismissed."""
    context, server_url = browser_context
    page = context.new_page()

    errors = []
    page.on("pageerror", lambda err: errors.append(str(err)))

    page.goto(server_url, wait_until="networkidle")

    # Wait for WASM
    try:
        page.wait_for_function("typeof Module !== 'undefined'", timeout=15000)
    except Exception:
        pytest.skip("WASM module did not load within 15s")

    # Dismiss enter screen
    page.evaluate("""() => {
        const el = document.getElementById('enter-screen');
        if (el) { el.classList.add('hidden'); }
        localStorage.setItem('time_entered', '1');
    }""")
    page.wait_for_timeout(500)

    yield page

    page.close()


# --- Screenshot Utilities ---

def canvas_screenshot(page):
    """Take a screenshot of just the WebGL canvas, return as PIL Image."""
    canvas = page.locator("#canvas")
    raw = canvas.screenshot()
    return Image.open(io.BytesIO(raw))


def full_screenshot(page):
    """Take a full page screenshot, return as PIL Image."""
    raw = page.screenshot()
    return Image.open(io.BytesIO(raw))


def save_screenshot(img, name):
    """Save a screenshot to the screenshots directory."""
    os.makedirs(SCREENSHOT_DIR, exist_ok=True)
    img.save(os.path.join(SCREENSHOT_DIR, name))


def is_solid_color(img, threshold=0.99):
    """Check if image is essentially one solid color (e.g., all black)."""
    pixels = list(img.getdata())
    if not pixels:
        return True
    first = pixels[0]
    same_count = sum(1 for p in pixels if p == first)
    return (same_count / len(pixels)) > threshold


def images_differ(img1, img2, threshold=0.80):
    """Check if two images are significantly different using pixel comparison.
    Returns True if images differ more than (1 - threshold).
    """
    if img1.size != img2.size:
        img2 = img2.resize(img1.size)

    pixels1 = list(img1.getdata())
    pixels2 = list(img2.getdata())

    if len(pixels1) != len(pixels2):
        return True

    diff_count = 0
    for p1, p2 in zip(pixels1, pixels2):
        # Compare RGB channels (ignore alpha if present)
        r1, g1, b1 = p1[:3]
        r2, g2, b2 = p2[:3]
        if abs(r1 - r2) > 30 or abs(g1 - g2) > 30 or abs(b1 - b2) > 30:
            diff_count += 1

    diff_ratio = diff_count / len(pixels1)
    return diff_ratio > (1 - threshold)
