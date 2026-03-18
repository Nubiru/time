/*
 * a11y_score.c — WCAG 2.1 Accessibility Compliance Audit Engine
 *
 * Pure module. No GL, no malloc, no globals, no side effects.
 *
 * Version 1.0
 */

#include "a11y_score.h"

/* ================================================================
 * HELPERS
 * ================================================================ */

static float clamp01(float v) {
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

static float maxf(float a, float b) {
    return a > b ? a : b;
}

/* ================================================================
 * CATEGORY NAMES
 * ================================================================ */

static const char *const CATEGORY_NAMES[] = {
    "Contrast",
    "Text Size",
    "Target Size",
    "Motion",
    "Reflow",
    "Spacing",
    "Focus"
};

static const char *const LEVEL_NAMES[] = {
    "Fail",
    "A",
    "AA",
    "AAA"
};

/* ================================================================
 * SCORING FUNCTIONS (one per category)
 * ================================================================ */

static a11y_result_t score_contrast(a11y_input_t inp) {
    a11y_result_t r;
    r.category = A11Y_CAT_CONTRAST;
    r.guideline = "1.4.3";

    float ratio = inp.text_contrast_ratio;

    if (ratio >= A11Y_AAA_CONTRAST) {
        r.level = A11Y_AAA;
        r.summary = "Excellent contrast (AAA)";
    } else if (ratio >= A11Y_AA_CONTRAST) {
        r.level = A11Y_AA;
        r.summary = "Good contrast (AA)";
    } else if (ratio >= A11Y_CONTRAST_MIN) {
        r.level = A11Y_A;
        r.summary = "Minimum contrast (large text only)";
    } else {
        r.level = A11Y_FAIL;
        r.summary = "Insufficient contrast";
    }

    r.score = clamp01(ratio / A11Y_AAA_CONTRAST);
    return r;
}

static a11y_result_t score_text_size(a11y_input_t inp) {
    a11y_result_t r;
    r.category = A11Y_CAT_TEXT_SIZE;
    r.guideline = "1.4.4";

    float base = inp.base_font_px;
    float min_f = inp.min_font_px;

    if (base >= A11Y_FONT_AAA_BASE && min_f >= A11Y_FONT_AAA_MIN) {
        r.level = A11Y_AAA;
        r.summary = "Excellent text sizing (AAA)";
    } else if (base >= A11Y_FONT_AA_BASE && min_f >= A11Y_FONT_AA_MIN) {
        r.level = A11Y_AA;
        r.summary = "Good text sizing (AA)";
    } else if (base >= A11Y_FONT_MIN) {
        r.level = A11Y_A;
        r.summary = "Minimum text size met";
    } else {
        r.level = A11Y_FAIL;
        r.summary = "Text too small";
    }

    r.score = clamp01(base / A11Y_FONT_AAA_BASE);
    return r;
}

static a11y_result_t score_target_size(a11y_input_t inp) {
    a11y_result_t r;
    r.category = A11Y_CAT_TARGET_SIZE;
    r.guideline = "2.5.5";

    int tap = inp.min_tap_target_px;
    int touch = (inp.pointer_type >= 1);

    if (touch) {
        if (tap >= A11Y_TAP_TOUCH_AAA) {
            r.level = A11Y_AAA;
            r.summary = "Excellent touch target size (AAA)";
        } else if (tap >= A11Y_TAP_TOUCH_AA) {
            r.level = A11Y_AA;
            r.summary = "Good touch target size (AA)";
        } else if (tap >= A11Y_TAP_TOUCH_MIN) {
            r.level = A11Y_A;
            r.summary = "Minimum touch target size";
        } else {
            r.level = A11Y_FAIL;
            r.summary = "Touch targets too small";
        }
        r.score = clamp01((float)tap / (float)A11Y_TAP_TOUCH_AAA);
    } else {
        if (tap >= A11Y_TAP_MOUSE_AAA) {
            r.level = A11Y_AAA;
            r.summary = "Excellent mouse target size (AAA)";
        } else if (tap >= A11Y_TAP_MOUSE_AA) {
            r.level = A11Y_AA;
            r.summary = "Good mouse target size (AA)";
        } else if (tap >= A11Y_TAP_MOUSE_MIN) {
            r.level = A11Y_A;
            r.summary = "Minimum mouse target size";
        } else {
            r.level = A11Y_FAIL;
            r.summary = "Mouse targets too small";
        }
        r.score = clamp01((float)tap / (float)A11Y_TAP_MOUSE_AAA);
    }

    return r;
}

static a11y_result_t score_motion(a11y_input_t inp) {
    a11y_result_t r;
    r.category = A11Y_CAT_MOTION;
    r.guideline = "2.3.3";

    if (!inp.animations_enabled) {
        r.level = A11Y_AAA;
        r.score = 1.0f;
        r.summary = "Animations disabled (AAA)";
        return r;
    }

    int ms = inp.motion_duration_ms;

    if (ms > A11Y_MOTION_FAIL_MS) {
        r.level = A11Y_FAIL;
        r.summary = "Excessive animation duration";
    } else if (ms > A11Y_MOTION_MEDIUM_MS) {
        r.level = A11Y_A;
        r.summary = "Long animations (user can toggle)";
    } else if (ms > A11Y_MOTION_SHORT_MS) {
        r.level = A11Y_AA;
        r.summary = "Moderate animation duration (AA)";
    } else {
        r.level = A11Y_AAA;
        r.summary = "Short animations (AAA)";
    }

    r.score = clamp01(maxf(1.0f - ((float)ms / (float)A11Y_MOTION_FAIL_MS), 0.0f));
    return r;
}

static a11y_result_t score_reflow(a11y_input_t inp) {
    a11y_result_t r;
    r.category = A11Y_CAT_REFLOW;
    r.guideline = "1.4.10";

    if (!inp.content_reflows) {
        r.level = A11Y_FAIL;
        r.score = 0.0f;
        r.summary = "Content does not reflow";
        return r;
    }

    if (inp.screen_width_px <= A11Y_REFLOW_AAA_PX) {
        r.level = A11Y_AAA;
        r.summary = "Content reflows at very narrow widths (AAA)";
    } else {
        r.level = A11Y_AA;
        r.summary = "Content reflows (AA)";
    }

    r.score = 1.0f;
    return r;
}

static a11y_result_t score_spacing(a11y_input_t inp) {
    a11y_result_t r;
    r.category = A11Y_CAT_SPACING;
    r.guideline = "1.4.12";

    float lh = inp.line_height_ratio;
    float ps = inp.paragraph_spacing_em;

    if (lh >= A11Y_SPACING_AAA_LINE && ps >= A11Y_SPACING_AAA_PARA) {
        r.level = A11Y_AAA;
        r.summary = "Excellent spacing (AAA)";
    } else if (lh >= A11Y_SPACING_AA_LINE && ps >= A11Y_SPACING_AA_PARA) {
        r.level = A11Y_AA;
        r.summary = "Good spacing (AA)";
    } else if (lh >= A11Y_SPACING_A) {
        r.level = A11Y_A;
        r.summary = "Minimum spacing met";
    } else if (lh >= A11Y_SPACING_FAIL) {
        r.level = A11Y_A;
        r.summary = "Tight spacing (borderline)";
    } else {
        r.level = A11Y_FAIL;
        r.summary = "Spacing too tight";
    }

    r.score = clamp01(lh / A11Y_SPACING_AAA_LINE);
    return r;
}

static a11y_result_t score_focus(a11y_input_t inp) {
    a11y_result_t r;
    r.category = A11Y_CAT_FOCUS;
    r.guideline = "2.4.7";

    if (!inp.focus_indicator_visible) {
        r.level = A11Y_FAIL;
        r.score = 0.0f;
        r.summary = "No visible focus indicator";
        return r;
    }

    int fc = inp.focus_contrast_ratio;

    if (fc >= A11Y_FOCUS_AAA_RATIO) {
        r.level = A11Y_AAA;
        r.summary = "Excellent focus visibility (AAA)";
    } else if (fc >= A11Y_FOCUS_AA_RATIO) {
        r.level = A11Y_AA;
        r.summary = "Good focus visibility (AA)";
    } else {
        r.level = A11Y_A;
        r.summary = "Focus visible but low contrast";
    }

    r.score = clamp01((float)fc / (float)A11Y_FOCUS_AAA_RATIO);
    return r;
}

/* ================================================================
 * DISPATCH TABLE
 * ================================================================ */

typedef a11y_result_t (*score_fn_t)(a11y_input_t);

static const score_fn_t SCORE_FNS[] = {
    score_contrast,
    score_text_size,
    score_target_size,
    score_motion,
    score_reflow,
    score_spacing,
    score_focus
};

/* ================================================================
 * PUBLIC API
 * ================================================================ */

a11y_result_t a11y_score_category(a11y_input_t input, a11y_category_t cat) {
    if (cat < 0 || cat >= A11Y_CAT_COUNT) {
        a11y_result_t r = {0};
        r.category = cat;
        r.level = A11Y_FAIL;
        r.score = 0.0f;
        r.guideline = "?";
        r.summary = "Invalid category";
        return r;
    }
    return SCORE_FNS[cat](input);
}

a11y_audit_t a11y_audit(a11y_input_t input) {
    a11y_audit_t audit;
    audit.overall = A11Y_AAA;
    audit.overall_score = 0.0f;
    audit.pass_count = 0;
    audit.fail_count = 0;

    float sum = 0.0f;

    for (int i = 0; i < A11Y_CAT_COUNT; i++) {
        audit.results[i] = a11y_score_category(input, (a11y_category_t)i);
        sum += audit.results[i].score;

        if (audit.results[i].level < audit.overall) {
            audit.overall = audit.results[i].level;
        }

        if (audit.results[i].level >= A11Y_AA) {
            audit.pass_count++;
        }
        if (audit.results[i].level == A11Y_FAIL) {
            audit.fail_count++;
        }
    }

    audit.overall_score = sum / (float)A11Y_CAT_COUNT;
    return audit;
}

const char *a11y_category_name(a11y_category_t cat) {
    if (cat < 0 || cat >= A11Y_CAT_COUNT) {
        return "?";
    }
    return CATEGORY_NAMES[cat];
}

const char *a11y_level_name(a11y_level_t level) {
    if (level < A11Y_FAIL || level > A11Y_AAA) {
        return "?";
    }
    return LEVEL_NAMES[level];
}

int a11y_category_count(void) {
    return A11Y_CAT_COUNT;
}

a11y_input_t a11y_input_default(void) {
    a11y_input_t inp = {0};
    inp.text_contrast_ratio = 7.5f;
    inp.large_text_contrast = 10.0f;
    inp.base_font_px = 16.0f;
    inp.min_font_px = 11.0f;
    inp.min_tap_target_px = 34;
    inp.pointer_type = 0;
    inp.animations_enabled = 1;
    inp.motion_duration_ms = 1618;
    inp.screen_width_px = 1920;
    inp.content_reflows = 1;
    inp.line_height_ratio = 1.618f;
    inp.paragraph_spacing_em = 2.618f;
    inp.focus_indicator_visible = 1;
    inp.focus_contrast_ratio = 4;
    return inp;
}

a11y_input_t a11y_input_worst(void) {
    a11y_input_t inp = {0};
    inp.text_contrast_ratio = 1.5f;
    inp.large_text_contrast = 1.5f;
    inp.base_font_px = 8.0f;
    inp.min_font_px = 6.0f;
    inp.min_tap_target_px = 10;
    inp.pointer_type = 1;
    inp.animations_enabled = 1;
    inp.motion_duration_ms = 15000;
    inp.screen_width_px = 1920;
    inp.content_reflows = 0;
    inp.line_height_ratio = 0.8f;
    inp.paragraph_spacing_em = 0.5f;
    inp.focus_indicator_visible = 0;
    inp.focus_contrast_ratio = 0;
    return inp;
}
