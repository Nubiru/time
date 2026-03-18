/*
 * a11y_score.h — WCAG 2.1 Accessibility Compliance Audit Engine
 *
 * Pure module. No GL, no malloc, no globals, no side effects.
 * Evaluates UI configuration against WCAG 2.1 criteria and
 * produces per-category scores + overall compliance level.
 *
 * Version 1.0
 */

#ifndef TIME_A11Y_SCORE_H
#define TIME_A11Y_SCORE_H

/* ================================================================
 * WCAG THRESHOLD CONSTANTS
 * ================================================================ */

/* Contrast (1.4.3 / 1.4.6) */
#define A11Y_CONTRAST_MIN       3.0f    /* Level A large text */
#define A11Y_AA_CONTRAST        4.5f    /* Level AA */
#define A11Y_AAA_CONTRAST       7.0f    /* Level AAA */

/* Text Size (1.4.4) */
#define A11Y_FONT_MIN           10.0f   /* Level A */
#define A11Y_FONT_AA_BASE       14.0f   /* Level AA base */
#define A11Y_FONT_AA_MIN        10.0f   /* Level AA minimum */
#define A11Y_FONT_AAA_BASE      16.0f   /* Level AAA base */
#define A11Y_FONT_AAA_MIN       12.0f   /* Level AAA minimum */

/* Target Size (2.5.5) — Touch */
#define A11Y_TAP_TOUCH_MIN      24      /* Level A touch */
#define A11Y_TAP_TOUCH_AA       44      /* Level AA touch */
#define A11Y_TAP_TOUCH_AAA      48      /* Level AAA touch */

/* Target Size (2.5.5) — Mouse */
#define A11Y_TAP_MOUSE_MIN      16      /* Level A mouse */
#define A11Y_TAP_MOUSE_AA       24      /* Level AA mouse */
#define A11Y_TAP_MOUSE_AAA      44      /* Level AAA mouse */

/* Motion (2.3.3) */
#define A11Y_MOTION_SHORT_MS    300     /* AAA threshold */
#define A11Y_MOTION_MEDIUM_MS   5000    /* AA threshold */
#define A11Y_MOTION_FAIL_MS     10000   /* FAIL threshold */

/* Reflow (1.4.10) */
#define A11Y_REFLOW_AAA_PX      256     /* AAA screen width */

/* Spacing (1.4.12) */
#define A11Y_SPACING_FAIL       1.0f    /* below this = FAIL */
#define A11Y_SPACING_A          1.2f    /* Level A */
#define A11Y_SPACING_AA_LINE    1.5f    /* Level AA line-height */
#define A11Y_SPACING_AA_PARA    1.5f    /* Level AA paragraph */
#define A11Y_SPACING_AAA_LINE   1.618f  /* Level AAA line-height (phi) */
#define A11Y_SPACING_AAA_PARA   2.0f    /* Level AAA paragraph */

/* Focus (2.4.7) */
#define A11Y_FOCUS_AA_RATIO     3       /* Level AA contrast */
#define A11Y_FOCUS_AAA_RATIO    5       /* Level AAA contrast */

/* ================================================================
 * TYPES
 * ================================================================ */

/* WCAG 2.1 compliance levels */
typedef enum {
    A11Y_FAIL = 0,   /* does not meet Level A */
    A11Y_A,          /* meets Level A */
    A11Y_AA,         /* meets Level AA */
    A11Y_AAA         /* meets Level AAA */
} a11y_level_t;

/* WCAG criteria categories */
typedef enum {
    A11Y_CAT_CONTRAST = 0,   /* 1.4.3/1.4.6 text contrast */
    A11Y_CAT_TEXT_SIZE,       /* 1.4.4 text resize up to 200% */
    A11Y_CAT_TARGET_SIZE,    /* 2.5.5 target size (44px min) */
    A11Y_CAT_MOTION,         /* 2.3.3 animation control */
    A11Y_CAT_REFLOW,         /* 1.4.10 content reflow at 320px */
    A11Y_CAT_SPACING,        /* 1.4.12 text spacing adjustable */
    A11Y_CAT_FOCUS,          /* 2.4.7 focus indicator visible */
    A11Y_CAT_COUNT
} a11y_category_t;

/* Input: current UI state for scoring */
typedef struct {
    /* Contrast */
    float text_contrast_ratio;   /* computed fg/bg contrast (1-21) */
    float large_text_contrast;   /* for headings/large text */

    /* Text */
    float base_font_px;          /* current base font size in px */
    float min_font_px;           /* smallest text on screen in px */

    /* Touch */
    int min_tap_target_px;       /* smallest interactive element in px */
    int pointer_type;            /* 0=mouse(fine), 1=touch(coarse) */

    /* Motion */
    int animations_enabled;      /* 1=yes, 0=reduced motion */
    int motion_duration_ms;      /* longest animation duration */

    /* Layout */
    int screen_width_px;         /* viewport width */
    int content_reflows;         /* 1=content reflows at narrow widths */

    /* Spacing */
    float line_height_ratio;     /* e.g. 1.618 */
    float paragraph_spacing_em;  /* spacing between paragraphs in em */

    /* Focus */
    int focus_indicator_visible; /* 1=visible focus ring, 0=none */
    int focus_contrast_ratio;    /* contrast of focus indicator */
} a11y_input_t;

/* Score result per category */
typedef struct {
    a11y_category_t category;
    a11y_level_t level;          /* A, AA, AAA, or FAIL */
    float score;                 /* 0.0-1.0 normalized score */
    const char *guideline;       /* "1.4.3" WCAG reference */
    const char *summary;         /* brief pass/fail description */
} a11y_result_t;

/* Overall audit result */
typedef struct {
    a11y_result_t results[A11Y_CAT_COUNT];
    a11y_level_t overall;        /* lowest level across all categories */
    float overall_score;         /* average of all scores */
    int pass_count;              /* categories at AA or above */
    int fail_count;              /* categories below A */
} a11y_audit_t;

/* ================================================================
 * API
 * ================================================================ */

/* Run full accessibility audit */
a11y_audit_t a11y_audit(a11y_input_t input);

/* Score a single category */
a11y_result_t a11y_score_category(a11y_input_t input, a11y_category_t cat);

/* Get category name string */
const char *a11y_category_name(a11y_category_t cat);

/* Get level name string ("Fail", "A", "AA", "AAA") */
const char *a11y_level_name(a11y_level_t level);

/* Get number of categories */
int a11y_category_count(void);

/* Create a default "good" input (desktop, good contrast, etc.) */
a11y_input_t a11y_input_default(void);

/* Create a worst-case input (everything fails) */
a11y_input_t a11y_input_worst(void);

#endif /* TIME_A11Y_SCORE_H */
