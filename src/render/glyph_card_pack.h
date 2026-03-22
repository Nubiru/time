/*
 * glyph_card_pack.h — System-to-glyph mapping for card icons (S107)
 *
 * Maps each knowledge system to its representative SDF glyph.
 * Astrology → current zodiac sign, I Ching → current trigram,
 * Human Design → Sun gate planet symbol, etc.
 *
 * Provides GPU-ready vertex data for rendering SDF glyphs as small
 * icons on card headers. Each glyph is a billboard quad with the
 * SDF evaluated in the fragment shader.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#ifndef TIME_GLYPH_CARD_PACK_H
#define TIME_GLYPH_CARD_PACK_H

#include "sdf_glyph.h"

/* Maximum number of simultaneous card glyphs */
#define GCP_MAX_GLYPHS 8

/* A single glyph instance for rendering */
typedef struct {
    sdf_glyph_category_t category;
    int glyph_index;          /* index within category */
    float screen_x, screen_y; /* screen position (pixels) */
    float size;               /* size in pixels */
    float r, g, b, a;         /* glyph color + opacity */
} gcp_instance_t;

/* Pack result — list of glyph instances to render this frame */
typedef struct {
    gcp_instance_t glyphs[GCP_MAX_GLYPHS];
    int count;
} gcp_data_t;

/* Get the representative glyph for a knowledge system.
 * system_id: TS_SYS_* value.
 * glyph_variant: time-dependent variant (e.g., current zodiac sign index).
 * Returns glyph category + index. Returns SDF_CAT_ZODIAC/0 for unknown. */
sdf_glyph_category_t gcp_system_category(int system_id);
int gcp_system_glyph_index(int system_id, int glyph_variant);

/* Get the default glyph variant for a system (no time dependency).
 * Returns 0 for most systems, category-specific default for others. */
int gcp_default_variant(int system_id);

/* Vertex shader source for SDF glyph billboard rendering (GLSL ES 3.00). */
const char *gcp_vert_source(void);

/* Fragment shader source for SDF glyph rendering (GLSL ES 3.00).
 * Evaluates the glyph SDF at each pixel, outputs smoothstep alpha.
 * Uses sdf_glyph_shader_source() for the SDF evaluation functions. */
const char *gcp_frag_source(void);

#endif /* TIME_GLYPH_CARD_PACK_H */
