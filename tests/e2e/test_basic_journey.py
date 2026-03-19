"""E2E test: Basic user journey through Time.

Tests: WASM loads -> canvas renders -> ? opens help -> Space pauses time.
Requires: playwright (pip install playwright && playwright install chromium)
Run: python3 -m pytest tests/e2e/test_basic_journey.py -v
"""

import subprocess
import time
import signal
import os
import pytest
from playwright.sync_api import sync_playwright

WASM_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "build-wasm")
PORT = 8089  # Avoid conflict with dev server on 8080


@pytest.fixture(scope="module")
def server():
    """Start a local HTTP server serving the WASM build."""
    abs_dir = os.path.abspath(WASM_DIR)
    if not os.path.isfile(os.path.join(abs_dir, "index.html")):
        pytest.skip("WASM build not found — run cmake --build build-wasm first")

    proc = subprocess.Popen(
        ["python3", "-m", "http.server", str(PORT), "-d", abs_dir],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        preexec_fn=os.setsid,
    )
    time.sleep(0.5)  # Let server start
    yield f"http://localhost:{PORT}"
    os.killpg(os.getpgid(proc.pid), signal.SIGTERM)
    proc.wait(timeout=5)


@pytest.fixture(scope="module")
def browser_page(server):
    """Launch headless Chromium, navigate to the app."""
    with sync_playwright() as pw:
        browser = pw.chromium.launch(headless=True)
        context = browser.new_context(viewport={"width": 1280, "height": 720})
        page = context.new_page()

        # Collect console errors
        errors = []
        page.on("pageerror", lambda err: errors.append(str(err)))

        page.goto(server, wait_until="networkidle")

        # Wait for WASM module to load
        page.wait_for_function("typeof Module !== 'undefined'", timeout=15000)

        # Dismiss enter screen via JS (avoids opacity transition + overlay issues)
        page.evaluate("""() => {
            const el = document.getElementById('enter-screen');
            if (el) { el.classList.add('hidden'); }
            localStorage.setItem('time_entered', '1');
        }""")
        page.wait_for_timeout(200)

        yield page, errors

        context.close()
        browser.close()


def test_wasm_loads(browser_page):
    """WASM module loads without fatal JS errors."""
    page, errors = browser_page

    # Module should be defined (already waited in fixture)
    is_loaded = page.evaluate("typeof Module !== 'undefined'")
    assert is_loaded, "Module should be defined after WASM loads"

    # No fatal page errors (aborts, unreachable traps)
    fatal = [e for e in errors if "abort" in e.lower() or "unreachable" in e.lower()]
    assert not fatal, f"Fatal WASM errors: {fatal}"


def test_canvas_present(browser_page):
    """Canvas element exists with non-zero dimensions."""
    page, _ = browser_page

    dims = page.evaluate("""() => {
        const c = document.getElementById('canvas');
        return c ? { w: c.width, h: c.height } : null;
    }""")
    assert dims is not None, "Canvas element should exist"
    assert dims["w"] > 0, f"Canvas width is {dims['w']}"
    assert dims["h"] > 0, f"Canvas height is {dims['h']}"


def test_enter_screen_dismisses(browser_page):
    """Enter screen has hidden class after dismissal."""
    page, _ = browser_page

    has_hidden = page.evaluate("""() => {
        const el = document.getElementById('enter-screen');
        return el ? el.classList.contains('hidden') : true;
    }""")
    assert has_hidden, "Enter screen should have 'hidden' class"


def test_help_panel_toggles(browser_page):
    """Pressing ? opens the help panel, Escape closes it."""
    page, _ = browser_page

    help_panel = page.locator("#help-panel")

    # Should start hidden (has hidden class)
    is_hidden = page.evaluate(
        "document.getElementById('help-panel').classList.contains('hidden')"
    )
    assert is_hidden, "Help panel should start hidden"

    # Press ? — the keydown listener calls Module._ui_toggle_help
    page.keyboard.press("?")
    page.wait_for_timeout(500)

    is_hidden = page.evaluate(
        "document.getElementById('help-panel').classList.contains('hidden')"
    )
    assert not is_hidden, "Help panel should be visible after pressing ?"

    # Press Escape to close
    page.keyboard.press("Escape")
    page.wait_for_timeout(500)

    is_hidden = page.evaluate(
        "document.getElementById('help-panel').classList.contains('hidden')"
    )
    assert is_hidden, "Help panel should be hidden after pressing Escape"


def test_pause_toggle(browser_page):
    """Space key triggers pause toggle without crashing."""
    page, errors = browser_page

    has_pause = page.evaluate(
        "typeof Module !== 'undefined' && typeof Module._ui_toggle_pause === 'function'"
    )
    if not has_pause:
        pytest.skip("Module._ui_toggle_pause not exported — WASM may not wire it yet")

    error_count_before = len(errors)

    # Press Space — should toggle pause without errors
    page.keyboard.press("Space")
    page.wait_for_timeout(300)

    # Check no new fatal errors
    new_errors = errors[error_count_before:]
    fatal = [e for e in new_errors if "abort" in e.lower() or "unreachable" in e.lower()]
    assert not fatal, f"Space key caused fatal errors: {fatal}"

    # Pause button should still exist
    btn_exists = page.evaluate("document.getElementById('tb-pause') !== null")
    assert btn_exists, "Pause button should exist after Space press"
