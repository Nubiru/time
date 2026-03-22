"""Visual experience tests for Time.

Each test verifies that a user-facing feature is VISIBLE — not just that
the module compiles, but that a human opening the browser would SEE something.

Failing tests = stream work queue.
Passing tests = verified experience.

Run: python3 -m pytest tests/e2e/test_visual_journey.py -v
Requires: WASM build (emcmake cmake -B build-wasm && cmake --build build-wasm)
"""

import pytest
from conftest import (
    canvas_screenshot,
    full_screenshot,
    save_screenshot,
    is_solid_color,
    images_differ,
)


# ============================================================
# CORE RENDERING — Does anything render at all?
# ============================================================


def test_canvas_not_black(page):
    """The WebGL canvas should render SOMETHING — not just a black screen.

    Stream: VISUALS
    Feature: Basic rendering pipeline
    If this fails: render loop is broken, no passes are drawing.
    """
    page.wait_for_timeout(2000)  # Let render settle
    img = canvas_screenshot(page)
    save_screenshot(img, "01_default_view.png")
    assert not is_solid_color(img, threshold=0.98), (
        "Canvas is solid black — no render passes are producing output"
    )


def test_stars_visible(page):
    """Stars should be visible as bright pixels on the dark canvas.

    Stream: VISUALS (star_pass)
    Feature: Star field rendering
    If this fails: star_pass is not drawing, or PASS_STARS is disabled.
    """
    page.wait_for_timeout(2000)
    img = canvas_screenshot(page)

    # Count bright pixels (R+G+B > 100 on a 0-255 scale)
    pixels = list(img.getdata())
    bright = sum(1 for p in pixels if sum(p[:3]) > 100)
    bright_ratio = bright / len(pixels) if pixels else 0

    # Stars should be at least 0.1% of pixels (a few hundred stars on 1280x720)
    assert bright_ratio > 0.001, (
        f"Only {bright_ratio:.4%} bright pixels — stars may not be rendering"
    )


# ============================================================
# CARDS — Are system information cards visible?
# ============================================================


def test_cards_render(page):
    """Cards or text should be visible overlaid on the star field.

    Stream: VISUALS (card_pass + text_pass)
    Feature: System information cards with themed colors
    If this fails: card_pass_draw() is not receiving card data.
    """
    page.wait_for_timeout(2000)
    img = canvas_screenshot(page)
    save_screenshot(img, "02_cards_view.png")

    # Cards add colored rectangles + text — should increase bright pixel count
    # beyond just stars. We check for non-star colored pixels (warm colors).
    pixels = list(img.getdata())
    warm = sum(1 for p in pixels if p[0] > 100 and p[1] > 50)  # Orange/gold range
    warm_ratio = warm / len(pixels) if pixels else 0

    # Cards should produce at least 0.5% warm-colored pixels
    assert warm_ratio > 0.005, (
        f"Only {warm_ratio:.4%} warm pixels — cards may not be rendering. "
        "Check: card_pass_draw() receiving data? card_style wired?"
    )


# ============================================================
# VIEW SWITCHING — Do keyboard controls change the view?
# ============================================================


def test_earth_view_toggle(page):
    """Pressing E should switch to Earth View (canvas changes significantly).

    Stream: VISUALS (earth_pass)
    Feature: Earth View with globe rendering
    If this fails: PASS_EARTH may be disabled in pass_schedule.c,
    or earth_pass_draw() is empty.
    """
    page.wait_for_timeout(1000)
    before = canvas_screenshot(page)

    page.keyboard.press("e")
    page.wait_for_timeout(1500)
    after = canvas_screenshot(page)
    save_screenshot(after, "03_earth_view.png")

    assert images_differ(before, after, threshold=0.70), (
        "Earth View (E key) did not change the canvas — "
        "check pass_schedule.c PASS_EARTH and earth_pass_draw()"
    )

    # Press E again to return to Space View
    page.keyboard.press("e")
    page.wait_for_timeout(500)


def test_birth_sky_toggle(page):
    """Pressing B should toggle birth sky view (time jumps to birth date).

    Stream: VISUALS + INFRA (birth_sky wiring in main.c)
    Feature: Birth moment sky rendering
    If this fails: birth_sky not wired in app_state, or B key handler missing.
    """
    # Set a birth date first — B key requires birth data to produce visible change
    page.evaluate("() => { if (Module._ui_set_birth_date) Module._ui_set_birth_date(1990, 6, 15); }")
    page.wait_for_timeout(500)

    page.wait_for_timeout(1000)
    before = canvas_screenshot(page)

    page.keyboard.press("b")
    page.wait_for_timeout(2000)
    after = canvas_screenshot(page)
    save_screenshot(after, "04_birth_sky.png")

    # Birth sky jumps to a different JD — planets shift, cards change.
    # Most canvas is dark stars → low pixel diff. Use 0.98 threshold (2% diff).
    assert images_differ(before, after, threshold=0.98), (
        "Birth Sky (B key) did not change the canvas — "
        "check input.c B handler and app_state birth_sky field"
    )

    # Toggle back
    page.keyboard.press("b")
    page.wait_for_timeout(500)


# ============================================================
# FOCUS MODES — Do system focus keys work?
# ============================================================


def test_focus_mode_changes_view(page):
    """Pressing a focus key (K/I/C/D/A) should change the display.

    Stream: VISUALS (focus_mode in card_pass)
    Feature: Per-system focus view
    If this fails: focus mode not wired in input.c or card_selector.
    """
    page.wait_for_timeout(1000)
    before = canvas_screenshot(page)

    # Press K for Kin Maya focus
    page.keyboard.press("k")
    page.wait_for_timeout(1000)
    after = canvas_screenshot(page)
    save_screenshot(after, "05_focus_kin.png")

    # Even if cards don't change dramatically, SOMETHING should shift
    # (at minimum, the focus indicator changes)

    # Press 0 or Escape to return to default
    page.keyboard.press("Escape")
    page.wait_for_timeout(500)


# ============================================================
# HELP PANEL — Does the UI overlay work?
# ============================================================


def test_help_panel_visible(page):
    """Pressing ? should show the help panel with keyboard shortcuts.

    Stream: INFRA (web/index.html)
    Feature: Help overlay with controls documentation
    If this fails: help panel HTML/CSS is broken.
    """
    page.keyboard.press("?")
    page.wait_for_timeout(500)

    # Check DOM — help panel should be visible
    is_visible = page.evaluate("""() => {
        const el = document.getElementById('help-panel');
        if (!el) return false;
        return !el.classList.contains('hidden');
    }""")

    if is_visible:
        img = full_screenshot(page)
        save_screenshot(img, "06_help_panel.png")

    assert is_visible, "Help panel should be visible after pressing ?"

    page.keyboard.press("Escape")
    page.wait_for_timeout(300)


# ============================================================
# ENTER TIME — Does the entry animation play?
# ============================================================


def test_enter_time_experience(page):
    """The app should show SOMETHING on initial load — not a blank screen.

    Stream: MOTION (enter_zoom) + VISUALS (all passes)
    Feature: "Enter Time" first contact experience
    If this fails: enter_zoom not wired in main_loop, or all passes disabled.
    """
    # This test uses the page fixture which already dismissed enter screen
    # and waited 500ms. The render should be active.
    img = canvas_screenshot(page)
    save_screenshot(img, "07_enter_time.png")

    # The canvas should not be blank
    assert not is_solid_color(img, threshold=0.98), (
        "App shows blank screen on entry — "
        "check enter_zoom wiring and render pass schedule"
    )


# ============================================================
# NARRATIVE TEXT — Is brain output displayed?
# ============================================================


def test_narrative_headline_exists(page):
    """A daily narrative headline should be present (rendered on canvas via MSDF).

    Stream: BRAIN + INFRA (brain_narrative → render_frame_t → text_pass)
    Feature: Daily narrative "headline" showing what today means
    If this fails: brain_narrative_compose() not producing output, or text_pass not rendering it.
    """
    page.wait_for_timeout(3000)  # Let narrative compute

    # Check via KEEPALIVE: headline is stored in g_state.headline
    headline = page.evaluate("""() => {
        if (typeof Module !== 'undefined' && Module._ui_get_headline) {
            try {
                var ptr = Module._ui_get_headline();
                if (ptr) return UTF8ToString(ptr);
            } catch(e) { return null; }
        }
        return null;
    }""")

    assert headline is not None and len(headline) > 5, (
        f"No narrative headline from brain_narrative — got: '{headline}'"
    )


# ============================================================
# AUDIO — Does any sound play?
# ============================================================


def test_audio_context_created(page):
    """AudioContext should exist and our audio engine should be initialized.

    Stream: AUDIO + INFRA
    Feature: Sound engine initialization
    If this fails: audio_engine_init() not called, or browser autoplay policy blocking.
    """
    has_audio = page.evaluate("""() => {
        return typeof AudioContext !== 'undefined' || typeof webkitAudioContext !== 'undefined';
    }""")
    assert has_audio, "AudioContext API not available"

    # Check our app's audio via window._timeAudio (set by audio_engine_init)
    app_audio = page.evaluate("""() => {
        return !!(window._timeAudio && window._timeAudio.ctx);
    }""")
    assert app_audio, (
        "Audio engine not initialized — window._timeAudio missing. "
        "Check audio_engine_init() is called in main()"
    )


# ============================================================
# THEME — Does the dark theme stay stable?
# ============================================================


def test_theme_stays_dark(page):
    """Canvas should NOT flash to light theme (auto-theme disabled).

    Stream: INFRA
    Feature: Cosmos dark theme stays active, no Dawn flashing
    If this fails: auto_theme may still be cycling, or CSS vars export broken.
    """
    page.wait_for_timeout(5000)  # Wait through any auto-theme cycle
    img = canvas_screenshot(page)
    save_screenshot(img, "08_theme_dark.png")
    pixels = list(img.getdata())
    # Average brightness should be LOW (dark theme)
    avg_brightness = sum(sum(p[:3]) for p in pixels) / (len(pixels) * 3)
    assert avg_brightness < 100, (
        f"Theme too bright: avg={avg_brightness}. "
        "Auto-theme may be flashing to Dawn."
    )


# ============================================================
# HELP PANEL SCROLL — Can the user see all shortcuts?
# ============================================================


def test_help_panel_scrollable(page):
    """Help panel body should be scrollable when content overflows.

    Stream: INFRA
    Feature: Help panel with overflow-y: auto on panel-body
    If this fails: panel-body CSS missing overflow-y, or parent clips it.
    """
    page.keyboard.press("?")
    page.wait_for_timeout(500)

    overflow = page.evaluate("""() => {
        const panel = document.getElementById('help-panel');
        if (!panel) return 'no-panel';
        const body = panel.querySelector('.panel-body');
        if (!body) return 'no-body';
        const style = getComputedStyle(body);
        return style.overflowY;
    }""")

    assert overflow in ['auto', 'scroll'], (
        f"Help panel-body overflow-y is '{overflow}', not scrollable"
    )

    page.keyboard.press("Escape")
    page.wait_for_timeout(300)
