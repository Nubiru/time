/*
 * glyph_card_pack.c — System-to-glyph mapping for card icons (S107)
 *
 * Each knowledge system gets a representative SDF glyph:
 *   Astrology → zodiac sign (12 variants based on current sign)
 *   I Ching → trigram (8 variants based on current hexagram)
 *   Human Design → planet symbol (Sun gate)
 *   Kin Maya → seal (20 variants from today's kin)
 *   Kabbalah → sefirah (10 variants)
 *   Others → default planet/zodiac symbol
 *
 * Pure module: no GL, no malloc, no globals.
 */

#include "glyph_card_pack.h"

/* TS_SYS_* constants from card_style.c pattern.
 * These match the time_system enum values. */
#define TS_GREGORIAN    0
#define TS_TZOLKIN      1
#define TS_HAAB         2
#define TS_CHINESE      3
#define TS_HEBREW       4
#define TS_ISLAMIC      5
#define TS_BUDDHIST     6
#define TS_HINDU        7
#define TS_ICHING       8
#define TS_ASTROLOGY    9
#define TS_HD          10
#define TS_KABBALAH    11
#define TS_GEOLOGICAL  12
#define TS_COSMIC      13
#define TS_COPTIC      14
#define TS_ETHIOPIAN   15
#define TS_PERSIAN     16
#define TS_JAPANESE    17
#define TS_KOREAN      18
#define TS_THAI        19

sdf_glyph_category_t gcp_system_category(int system_id)
{
    switch (system_id) {
    case TS_ASTROLOGY:  return SDF_CAT_ZODIAC;
    case TS_ICHING:     return SDF_CAT_TRIGRAM;
    case TS_TZOLKIN:
    case TS_HAAB:       return SDF_CAT_SEAL;
    case TS_KABBALAH:   return SDF_CAT_SEFIRAH;
    case TS_HD:         return SDF_CAT_PLANET;
    case TS_COSMIC:     return SDF_CAT_PLANET;    /* Sun symbol */
    default:            return SDF_CAT_ZODIAC;    /* default: zodiac */
    }
}

int gcp_system_glyph_index(int system_id, int glyph_variant)
{
    sdf_glyph_category_t cat = gcp_system_category(system_id);
    int max_count = sdf_glyph_count(cat);
    if (max_count <= 0) return 0;

    /* Clamp variant to valid range */
    int idx = glyph_variant;
    if (idx < 0) idx = 0;
    if (idx >= max_count) idx = idx % max_count;
    return idx;
}

int gcp_default_variant(int system_id)
{
    switch (system_id) {
    case TS_ASTROLOGY:  return 0;   /* Aries */
    case TS_ICHING:     return 0;   /* ☰ Heaven */
    case TS_TZOLKIN:    return 0;   /* Dragon */
    case TS_HAAB:       return 0;   /* Dragon */
    case TS_KABBALAH:   return 0;   /* Keter */
    case TS_HD:         return 0;   /* Sun */
    case TS_COSMIC:     return 0;   /* Sun */
    case TS_GREGORIAN:  return 0;   /* Aries (calendar → first sign) */
    default:            return 0;
    }
}

/* SDF glyph billboard vertex shader */
static const char s_gcp_vert[] =
    "#version 300 es\n"
    "layout(location = 0) in vec2 a_position;\n"
    "layout(location = 1) in vec2 a_texcoord;\n"
    "uniform mat4 u_mvp;\n"
    "uniform vec2 u_offset;\n"
    "uniform float u_size;\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "    v_uv = a_texcoord * 2.0 - 1.0;\n" /* -1 to 1 for SDF eval */
    "    vec2 pos = a_position * u_size + u_offset;\n"
    "    gl_Position = u_mvp * vec4(pos, 0.0, 1.0);\n"
    "}\n";

/* SDF glyph fragment shader — evaluates glyph SDF and outputs gold icon */
static const char s_gcp_frag[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "uniform vec4 u_color;\n"
    "uniform int u_glyph_id;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    /* SDF evaluation placeholder — glyph_id selects the glyph.\n"
    "     * Full SDF evaluation code is in sdf_glyph_shader_source().\n"
    "     * For now: simple circle placeholder. */\n"
    "    float dist = length(v_uv);\n"
    "    float alpha = 1.0 - smoothstep(0.7, 0.75, dist);\n"
    "    if (alpha < 0.01) discard;\n"
    "    frag_color = vec4(u_color.rgb, u_color.a * alpha);\n"
    "}\n";

const char *gcp_vert_source(void) { return s_gcp_vert; }
const char *gcp_frag_source(void) { return s_gcp_frag; }
