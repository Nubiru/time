/* golden_layout.h — Golden Ratio Design System
 *
 * phi governs EVERY visual proportion in Time:
 * typography, spacing, layout, animation timing, opacity cascades.
 *
 * This module is the single source of truth for visual proportions.
 * No magic numbers. Every dimension derives from phi.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_GOLDEN_LAYOUT_H
#define TIME_GOLDEN_LAYOUT_H

/* phi and its powers — the proportion vocabulary */
#define PHI_RATIO       1.6180339887498948482
#define PHI_INV   0.6180339887498948482  /* 1/phi = phi - 1 */
#define PHI_SQ    2.6180339887498948482  /* phi^2 = phi + 1 */
#define PHI_CU    4.2360679774997896964  /* phi^3 */
#define PHI_INV2  0.3819660112501051518  /* 1/phi^2 */
#define PHI_INV3  0.2360679774997896964  /* 1/phi^3 */
#define PHI_INV4  0.1458980337503154554  /* 1/phi^4 */
#define PHI_INV5  0.0901699437494742410  /* 1/phi^5 */

/* --- Typography Scale ---
 * Each level = previous * phi^-1.
 * Base size is level 0 (body text). */
#define PHI_TYPE_LEVELS 8

typedef struct {
    float sizes[PHI_TYPE_LEVELS];  /* font sizes: [0]=display .. [7]=micro */
    float line_height;            /* phi = 1.618 */
    float paragraph_spacing;      /* line_height * phi */
} gl_type_scale_t;

/* Compute typography scale from a base font size (pixels).
 * Level 0 = largest (display), level 3 = body, level 7 = micro.
 * Each step down multiplies by phi^-1 (0.618). */
gl_type_scale_t gl_type_scale(float base_size);

/* Get font size for a specific level (0-7). */
float gl_font_size(gl_type_scale_t scale, int level);

/* --- Spacing Scale ---
 * A series of spacing values, each phi times the previous.
 * Used for padding, margins, gaps, offsets. */
#define PHI_SPACE_LEVELS 10

typedef struct {
    float values[PHI_SPACE_LEVELS]; /* [0]=smallest .. [9]=largest */
} gl_spacing_t;

/* Compute spacing scale from a base unit (pixels).
 * values[4] = base. Each step up *= phi, each step down *= phi^-1. */
gl_spacing_t gl_spacing_scale(float base_unit);

/* Get spacing value by level (0-9). */
float gl_space(gl_spacing_t spacing, int level);

/* --- Screen Layout ---
 * Golden rectangle partitioning for screen regions. */

typedef struct {
    float x, y, w, h;  /* normalized 0.0-1.0 screen coordinates */
} gl_rect_t;

typedef struct {
    gl_rect_t main_area;      /* primary content: golden major portion */
    gl_rect_t sidebar;        /* secondary: golden minor portion */
    gl_rect_t hud_top_left;   /* HUD anchor: phi-positioned */
    gl_rect_t hud_top_right;  /* HUD anchor: phi-positioned */
    gl_rect_t hud_bottom;     /* HUD anchor: phi-positioned */
    float margin;             /* screen edge margin (phi-derived) */
} gl_screen_t;

/* Compute golden screen layout for given aspect ratio.
 * Divides screen at golden section points. */
gl_screen_t gl_screen_layout(float aspect_ratio);

/* Subdivide a rect into golden major (left/top) and minor (right/bottom).
 * horizontal=1: split width. horizontal=0: split height. */
gl_rect_t gl_rect_major(gl_rect_t r, int horizontal);
gl_rect_t gl_rect_minor(gl_rect_t r, int horizontal);

/* Center a golden rectangle of given width inside a container. */
gl_rect_t gl_rect_golden(float center_x, float center_y, float width);

/* Position N items along a line with golden-ratio spacing.
 * out: array of N floats receiving positions along [start, end].
 * Items cluster toward the golden section point. */
void gl_distribute(float start, float end, int n, float *out);

/* --- Card Dimensions ---
 * Cards are golden rectangles with phi-derived internal layout. */

typedef struct {
    float width, height;       /* outer dimensions (golden ratio) */
    float padding;             /* internal padding */
    float header_height;       /* top section (phi^-2 of height) */
    float body_height;         /* main section (phi^-1 of height) */
    float footer_height;       /* bottom section (phi^-3 of height) */
    float corner_radius;       /* rounded corners */
} gl_card_t;

/* Compute card dimensions from a width. Height = width / phi. */
gl_card_t gl_card_dimensions(float width, float base_spacing);

/* --- Animation Timing ---
 * Durations and delays as phi-derived series. */

typedef struct {
    float micro;      /* ~90ms:  phi^-5 * base */
    float fast;       /* ~146ms: phi^-4 * base */
    float normal;     /* ~236ms: phi^-3 * base (snappy interactions) */
    float medium;     /* ~382ms: phi^-2 * base */
    float slow;       /* ~618ms: phi^-1 * base */
    float base;       /* 1000ms: base unit */
    float dramatic;   /* ~1618ms: phi * base */
    float epic;       /* ~2618ms: phi^2 * base */
} gl_timing_t;

/* Compute animation timing scale from a base duration (seconds). */
gl_timing_t gl_timing_scale(float base_seconds);

/* Stagger delay for item at index i in a sequence of n items.
 * Returns delay in seconds. Uses fibonacci-timed stagger. */
float gl_stagger_delay(gl_timing_t timing, int index, int total);

/* --- Opacity Cascade ---
 * Layer opacities descending by phi powers. */

#define PHI_OPACITY_LEVELS 8

typedef struct {
    float values[PHI_OPACITY_LEVELS]; /* [0]=1.0, [1]=phi^-1, [2]=phi^-2... */
} gl_opacity_t;

/* Compute opacity cascade. Level 0 = full, each step *= phi^-1. */
gl_opacity_t gl_opacity_cascade(void);

/* Get opacity for a depth level (0=front, 7=back). */
float gl_opacity_at(int level);

/* --- Proportional Sizes ---
 * For rings, orbits, UI elements that need phi-relative sizing. */

/* Given an inner radius, compute outer radius as inner * phi. */
float gl_ring_outer(float inner_radius);

/* Given an outer radius, compute inner radius as outer * phi^-1. */
float gl_ring_inner(float outer_radius);

/* Compute N concentric ring radii starting from innermost.
 * Each successive ring radius = previous * phi.
 * out: array of N floats. */
void gl_concentric_radii(float innermost, int n, float *out);

/* Width-to-height ratio for a golden rectangle. Always phi. */
float gl_aspect_ratio(void);

/* --- Scale System ---
 * Camera zoom levels as phi-spaced logarithmic steps. */

/* Compute zoom factor for scale level n (0-based).
 * Each level = previous * phi. Returns phi^n. */
float gl_zoom_factor(int level);

/* Compute fade fraction for smooth crossfade between scale levels.
 * value: current zoom. level: target level.
 * Returns 0.0 (fully faded) to 1.0 (fully visible). */
float gl_scale_fade(float value, int level, int total_levels);

#endif /* TIME_GOLDEN_LAYOUT_H */
