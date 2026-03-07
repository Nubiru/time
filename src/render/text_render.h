/* text_render.h -- SDF text rendering data module
 *
 * Pure data types and functions for signed distance field text rendering:
 * - SDF alpha computation (smoothstep-based glyph edges)
 * - SDF effect parameters (outline, shadow, glow)
 * - Text style presets (size, alignment, spacing)
 * - Text animation data (fade, slide, scale, typewriter)
 * - Unicode script range lookup (atlas mapping, RTL detection)
 *
 * No GL, no malloc, no globals, no side effects.
 * All animation functions use value semantics (return new structs). */

#ifndef TIME_TEXT_RENDER_H
#define TIME_TEXT_RENDER_H

/* ===== SDF Rendering Parameters ===== */

typedef struct {
    float edge_threshold;  /* 0.5 typical */
    float smoothing;       /* 0.01-0.2 */
    float outline_width;   /* 0.0 = no outline */
    float outline_r, outline_g, outline_b, outline_a;
    float shadow_offset_x, shadow_offset_y;
    float shadow_softness; /* 0.0-1.0 */
    float shadow_r, shadow_g, shadow_b, shadow_a;
    float glow_radius;     /* 0.0 = no glow */
    float glow_r, glow_g, glow_b, glow_a;
} sdf_params_t;

/* Compute glyph alpha from SDF distance value.
 * distance < edge = inside glyph (opaque).
 * Uses smoothstep for antialiased edges. */
float sdf_alpha(float distance, float edge, float smoothing);

/* Compute outline alpha: opaque between edge and edge+outline_width. */
float sdf_outline_alpha(float distance, float edge, float outline_width,
                        float smoothing);

/* Default SDF params: clean edges, no effects. */
sdf_params_t sdf_params_default(void);

/* SDF params with colored outline. Alpha defaults to 1.0. */
sdf_params_t sdf_params_outlined(float width, float r, float g, float b);

/* SDF params with drop shadow. */
sdf_params_t sdf_params_shadowed(float ox, float oy, float softness);

/* SDF params with colored glow. Alpha defaults to 1.0. */
sdf_params_t sdf_params_glowing(float radius, float r, float g, float b);

/* ===== Text Styles ===== */

typedef enum {
    TEXT_ALIGN_LEFT   = 0,
    TEXT_ALIGN_CENTER = 1,
    TEXT_ALIGN_RIGHT  = 2
} text_align_t;

typedef enum {
    TEXT_SIZE_TINY    = 0,
    TEXT_SIZE_SMALL   = 1,
    TEXT_SIZE_BODY    = 2,
    TEXT_SIZE_LARGE   = 3,
    TEXT_SIZE_TITLE   = 4,
    TEXT_SIZE_DISPLAY = 5,
    TEXT_SIZE_COUNT   = 6
} text_size_t;

typedef struct {
    text_size_t size;
    text_align_t align;
    sdf_params_t sdf;
    float line_spacing;
    float letter_spacing;
    float opacity;
} text_style_t;

/* Get scale multiplier for a text size enum. */
float text_size_scale(text_size_t size);

/* Style presets -- each returns a complete text_style_t. */
text_style_t text_style_label(void);
text_style_t text_style_body(void);
text_style_t text_style_title(void);
text_style_t text_style_subtitle(void);
text_style_t text_style_annotation(void);
text_style_t text_style_data(void);
text_style_t text_style_accent(void);
text_style_t text_style_hud(void);

/* ===== Text Animation ===== */

typedef enum {
    TEXT_ANIM_NONE        = 0,
    TEXT_ANIM_FADE_IN     = 1,
    TEXT_ANIM_FADE_OUT    = 2,
    TEXT_ANIM_SLIDE_UP    = 3,
    TEXT_ANIM_SLIDE_DOWN  = 4,
    TEXT_ANIM_SLIDE_LEFT  = 5,
    TEXT_ANIM_SLIDE_RIGHT = 6,
    TEXT_ANIM_SCALE_UP    = 7,
    TEXT_ANIM_SCALE_DOWN  = 8,
    TEXT_ANIM_TYPEWRITER  = 9,
    TEXT_ANIM_COUNT       = 10
} text_anim_type_t;

typedef struct {
    text_anim_type_t type;
    float duration;
    float elapsed;
    float delay;
    float char_stagger;
    int char_count;
    int active;
    int finished;
} text_anim_t;

/* Create an animation of given type and duration (seconds). */
text_anim_t text_anim_create(text_anim_type_t type, float duration);

/* Create a typewriter animation with per-character delay. */
text_anim_t text_anim_typewriter(float char_delay, int char_count);

/* Return a copy of anim with added delay. */
text_anim_t text_anim_delayed(text_anim_t anim, float delay);

/* Advance animation by dt seconds. Returns new anim (value semantics). */
text_anim_t text_anim_update(text_anim_t anim, float dt);

/* Normalized progress [0, 1], accounting for delay. */
float text_anim_progress(text_anim_t anim);

/* Current opacity for the animation type. */
float text_anim_opacity(text_anim_t anim);

/* Current position offset. Writes to caller-provided pointers. */
void text_anim_offset(text_anim_t anim, float *dx, float *dy);

/* Current scale factor. */
float text_anim_scale(text_anim_t anim);

/* Per-character opacity (for typewriter; uniform for others). */
float text_anim_char_opacity(text_anim_t anim, int char_index);

/* ===== Unicode Script Ranges ===== */

typedef struct {
    const char *name;
    int start_codepoint;
    int end_codepoint;
    int rtl;
    int atlas_index;
} script_range_t;

/* Number of pre-defined script ranges. */
int script_range_count(void);

/* Get script range by index. */
script_range_t script_range_get(int index);

/* Find which range a codepoint belongs to. Returns -1 if none. */
int script_range_find(int codepoint);

/* Check if a codepoint is RTL. Returns 0 for unknown. */
int script_is_rtl(int codepoint);

/* Get atlas index for a codepoint. Returns -1 if unknown. */
int script_atlas_for(int codepoint);

#endif /* TIME_TEXT_RENDER_H */
