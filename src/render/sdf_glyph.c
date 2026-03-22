/* sdf_glyph.c — Procedural SDF glyph generation (S107)
 *
 * Each zodiac sign, planet symbol, and sacred glyph defined as
 * combinations of SDF primitives. Resolution-independent.
 * "Every pixel earned" — no textures, no DALL-E, pure math.
 */

#include "sdf_glyph.h"
#include <math.h>
#include <string.h>

/* --- Zodiac sign definitions (12) --- */

static const sdf_glyph_t ZODIAC_GLYPHS[12] = {
    /* Aries ♈ — two curved horns from a center point */
    { 0, SDF_CAT_ZODIAC, "Aries", 3, {
        { SDF_ARC, 0, 0.2f, 0.4f, 180, 360, 0.06f, SDF_OP_UNION },
        { SDF_ARC, 0, 0.2f, 0.4f, 0, 180, 0.06f, SDF_OP_UNION },
        { SDF_LINE, 0, 0.6f, 0, -0.6f, 0.06f, 0, SDF_OP_UNION },
    }},
    /* Taurus ♉ — circle with horns on top */
    { 1, SDF_CAT_ZODIAC, "Taurus", 3, {
        { SDF_CIRCLE, 0, 0.1f, 0.3f, 0, 0, 0, SDF_OP_UNION },
        { SDF_ARC, -0.2f, -0.3f, 0.25f, 200, 340, 0.06f, SDF_OP_UNION },
        { SDF_ARC, 0.2f, -0.3f, 0.25f, 200, 340, 0.06f, SDF_OP_UNION },
    }},
    /* Gemini ♊ — two vertical lines with horizontal caps */
    { 2, SDF_CAT_ZODIAC, "Gemini", 4, {
        { SDF_LINE, -0.2f, -0.5f, -0.2f, 0.5f, 0.06f, 0, SDF_OP_UNION },
        { SDF_LINE, 0.2f, -0.5f, 0.2f, 0.5f, 0.06f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.35f, -0.5f, 0.35f, -0.5f, 0.06f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.35f, 0.5f, 0.35f, 0.5f, 0.06f, 0, SDF_OP_UNION },
    }},
    /* Cancer ♋ — two interlocking circles (69 shape) */
    { 3, SDF_CAT_ZODIAC, "Cancer", 2, {
        { SDF_ARC, -0.2f, 0, 0.25f, 0, 180, 0.06f, SDF_OP_UNION },
        { SDF_ARC, 0.2f, 0, 0.25f, 180, 360, 0.06f, SDF_OP_UNION },
    }},
    /* Leo ♌ — curved loop */
    { 4, SDF_CAT_ZODIAC, "Leo", 2, {
        { SDF_ARC, -0.1f, 0.1f, 0.3f, 30, 330, 0.06f, SDF_OP_UNION },
        { SDF_ARC, 0.3f, -0.2f, 0.15f, 0, 360, 0.06f, SDF_OP_UNION },
    }},
    /* Virgo ♍ — M with tail */
    { 5, SDF_CAT_ZODIAC, "Virgo", 5, {
        { SDF_LINE, -0.4f, 0.4f, -0.4f, -0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.4f, -0.2f, -0.1f, 0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.1f, 0.2f, -0.1f, -0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.1f, -0.2f, 0.2f, 0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, 0.2f, 0.2f, 0.4f, 0.5f, 0.05f, 0, SDF_OP_UNION },
    }},
    /* Libra ♎ — scales (horizontal line + arch) */
    { 6, SDF_CAT_ZODIAC, "Libra", 3, {
        { SDF_LINE, -0.5f, 0.2f, 0.5f, 0.2f, 0.06f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.5f, 0, 0.5f, 0, 0.06f, 0, SDF_OP_UNION },
        { SDF_ARC, 0, -0.1f, 0.3f, 180, 360, 0.06f, SDF_OP_UNION },
    }},
    /* Scorpio ♏ — M with arrow tail */
    { 7, SDF_CAT_ZODIAC, "Scorpio", 5, {
        { SDF_LINE, -0.4f, 0.3f, -0.4f, -0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.4f, -0.2f, -0.1f, 0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.1f, 0.2f, -0.1f, -0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.1f, -0.2f, 0.2f, 0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, 0.2f, 0.2f, 0.45f, 0.05f, 0.05f, 0, SDF_OP_UNION },
    }},
    /* Sagittarius ♐ — arrow */
    { 8, SDF_CAT_ZODIAC, "Sagittarius", 3, {
        { SDF_LINE, -0.4f, 0.4f, 0.4f, -0.4f, 0.06f, 0, SDF_OP_UNION },
        { SDF_LINE, 0.4f, -0.4f, 0.1f, -0.4f, 0.06f, 0, SDF_OP_UNION },
        { SDF_LINE, 0.4f, -0.4f, 0.4f, -0.1f, 0.06f, 0, SDF_OP_UNION },
    }},
    /* Capricorn ♑ — V with loop */
    { 9, SDF_CAT_ZODIAC, "Capricorn", 3, {
        { SDF_LINE, -0.3f, -0.4f, 0, 0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, 0, 0.2f, 0.2f, -0.1f, 0.05f, 0, SDF_OP_UNION },
        { SDF_ARC, 0.3f, 0.1f, 0.2f, 0, 360, 0.05f, SDF_OP_UNION },
    }},
    /* Aquarius ♒ — two wavy lines */
    { 10, SDF_CAT_ZODIAC, "Aquarius", 4, {
        { SDF_ARC, -0.2f, -0.15f, 0.15f, 180, 360, 0.05f, SDF_OP_UNION },
        { SDF_ARC, 0.2f, -0.15f, 0.15f, 0, 180, 0.05f, SDF_OP_UNION },
        { SDF_ARC, -0.2f, 0.15f, 0.15f, 180, 360, 0.05f, SDF_OP_UNION },
        { SDF_ARC, 0.2f, 0.15f, 0.15f, 0, 180, 0.05f, SDF_OP_UNION },
    }},
    /* Pisces ♓ — two arcs with horizontal bar */
    { 11, SDF_CAT_ZODIAC, "Pisces", 3, {
        { SDF_ARC, -0.25f, 0, 0.35f, 270, 90, 0.06f, SDF_OP_UNION },
        { SDF_ARC, 0.25f, 0, 0.35f, 90, 270, 0.06f, SDF_OP_UNION },
        { SDF_LINE, -0.5f, 0, 0.5f, 0, 0.06f, 0, SDF_OP_UNION },
    }},
};

/* --- Planet symbol definitions (10) --- */

static const sdf_glyph_t PLANET_GLYPHS[10] = {
    /* Sun ☉ — circle with dot */
    { 0, SDF_CAT_PLANET, "Sun", 2, {
        { SDF_CIRCLE, 0, 0, 0.45f, 0, 0, 0, SDF_OP_UNION },
        { SDF_CIRCLE, 0, 0, 0.08f, 0, 0, 0, SDF_OP_UNION },
    }},
    /* Moon ☽ — crescent */
    { 1, SDF_CAT_PLANET, "Moon", 2, {
        { SDF_CIRCLE, -0.1f, 0, 0.4f, 0, 0, 0, SDF_OP_UNION },
        { SDF_CIRCLE, 0.15f, 0, 0.35f, 0, 0, 0, SDF_OP_SUBTRACT },
    }},
    /* Mercury ☿ — circle + cross + horns */
    { 2, SDF_CAT_PLANET, "Mercury", 4, {
        { SDF_CIRCLE, 0, 0, 0.25f, 0, 0, 0, SDF_OP_UNION },
        { SDF_LINE, 0, 0.25f, 0, 0.6f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.15f, 0.5f, 0.15f, 0.5f, 0.05f, 0, SDF_OP_UNION },
        { SDF_ARC, 0, -0.25f, 0.15f, 0, 180, 0.05f, SDF_OP_UNION },
    }},
    /* Venus ♀ — circle + cross below */
    { 3, SDF_CAT_PLANET, "Venus", 3, {
        { SDF_CIRCLE, 0, -0.15f, 0.25f, 0, 0, 0, SDF_OP_UNION },
        { SDF_LINE, 0, 0.1f, 0, 0.55f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.15f, 0.35f, 0.15f, 0.35f, 0.05f, 0, SDF_OP_UNION },
    }},
    /* Mars ♂ — circle + arrow upper-right */
    { 4, SDF_CAT_PLANET, "Mars", 4, {
        { SDF_CIRCLE, -0.1f, 0.1f, 0.25f, 0, 0, 0, SDF_OP_UNION },
        { SDF_LINE, 0.1f, -0.1f, 0.45f, -0.45f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, 0.45f, -0.45f, 0.2f, -0.45f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, 0.45f, -0.45f, 0.45f, -0.2f, 0.05f, 0, SDF_OP_UNION },
    }},
    /* Jupiter ♃ — crescent LEFT + cross */
    { 5, SDF_CAT_PLANET, "Jupiter", 3, {
        { SDF_ARC, 0.1f, -0.2f, 0.3f, 90, 270, 0.06f, SDF_OP_UNION },
        { SDF_LINE, -0.4f, 0, 0.4f, 0, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, 0.1f, -0.5f, 0.1f, 0.5f, 0.05f, 0, SDF_OP_UNION },
    }},
    /* Saturn ♄ — cross + crescent right */
    { 6, SDF_CAT_PLANET, "Saturn", 3, {
        { SDF_LINE, -0.15f, -0.5f, -0.15f, 0.2f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.35f, -0.3f, 0.05f, -0.3f, 0.05f, 0, SDF_OP_UNION },
        { SDF_ARC, 0, 0.2f, 0.2f, 270, 90, 0.05f, SDF_OP_UNION },
    }},
    /* Uranus ♅ — circle + H on top */
    { 7, SDF_CAT_PLANET, "Uranus", 4, {
        { SDF_CIRCLE, 0, 0.25f, 0.15f, 0, 0, 0, SDF_OP_UNION },
        { SDF_LINE, -0.2f, -0.5f, -0.2f, 0, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, 0.2f, -0.5f, 0.2f, 0, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.2f, -0.25f, 0.2f, -0.25f, 0.05f, 0, SDF_OP_UNION },
    }},
    /* Neptune ♆ — trident + cross */
    { 8, SDF_CAT_PLANET, "Neptune", 4, {
        { SDF_LINE, 0, -0.5f, 0, 0.3f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.15f, 0.15f, 0.15f, 0.15f, 0.05f, 0, SDF_OP_UNION },
        { SDF_ARC, 0, -0.3f, 0.25f, 180, 360, 0.05f, SDF_OP_UNION },
        { SDF_LINE, -0.25f, -0.5f, 0.25f, -0.5f, 0.05f, 0, SDF_OP_UNION },
    }},
    /* Pluto ♇ — arc right + cross */
    { 9, SDF_CAT_PLANET, "Pluto", 3, {
        { SDF_ARC, -0.1f, -0.15f, 0.25f, 270, 90, 0.06f, SDF_OP_UNION },
        { SDF_LINE, -0.1f, -0.5f, -0.1f, 0.3f, 0.05f, 0, SDF_OP_UNION },
        { SDF_LINE, -0.3f, 0.1f, 0.1f, 0.1f, 0.05f, 0, SDF_OP_UNION },
    }},
};

/* --- API --- */

int sdf_glyph_count(sdf_glyph_category_t category)
{
    switch (category) {
        case SDF_CAT_ZODIAC:  return 12;
        case SDF_CAT_PLANET:  return 10;
        case SDF_CAT_SEAL:    return 0;  /* TODO: simplified seal outlines */
        case SDF_CAT_SEFIRAH: return 0;  /* TODO */
        case SDF_CAT_TRIGRAM: return 0;  /* TODO */
        default: return 0;
    }
}

sdf_glyph_t sdf_glyph_get(sdf_glyph_category_t category, int index)
{
    switch (category) {
        case SDF_CAT_ZODIAC:
            if (index >= 0 && index < 12) return ZODIAC_GLYPHS[index];
            break;
        case SDF_CAT_PLANET:
            if (index >= 0 && index < 10) return PLANET_GLYPHS[index];
            break;
        default:
            break;
    }
    sdf_glyph_t empty = {0};
    return empty;
}

/* --- SDF evaluation (CPU-side for testing) --- */

static float sdf_circle_eval(float px, float py, float cx, float cy, float r)
{
    float dx = px - cx, dy = py - cy;
    return sqrtf(dx*dx + dy*dy) - r;
}

static float sdf_line_eval(float px, float py,
                            float ax, float ay, float bx, float by, float thick)
{
    float abx = bx - ax, aby = by - ay;
    float apx = px - ax, apy = py - ay;
    float t = (apx*abx + apy*aby) / (abx*abx + aby*aby);
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    float cx = ax + t*abx - px, cy = ay + t*aby - py;
    return sqrtf(cx*cx + cy*cy) - thick;
}

float sdf_glyph_eval(const sdf_glyph_t *glyph, float px, float py)
{
    if (!glyph || glyph->prim_count == 0) return 1.0f;

    float d = 1e9f;
    for (int i = 0; i < glyph->prim_count; i++) {
        const sdf_prim_t *p = &glyph->prims[i];
        float pd = 1e9f;

        switch (p->type) {
            case SDF_CIRCLE:
                pd = sdf_circle_eval(px, py, p->cx, p->cy, p->p1);
                break;
            case SDF_LINE:
                pd = sdf_line_eval(px, py, p->cx, p->cy, p->p1, p->p2, p->p3);
                break;
            case SDF_ARC: {
                /* Approximate arc as circle ring segment */
                float dx = px - p->cx, dy = py - p->cy;
                float dist = sqrtf(dx*dx + dy*dy);
                pd = fabsf(dist - p->p1) - p->p4; /* ring SDF */
                break;
            }
            default:
                pd = 1e9f;
        }

        switch (p->op) {
            case SDF_OP_UNION:     d = (pd < d) ? pd : d; break;
            case SDF_OP_SUBTRACT:  d = (d > -pd) ? d : -pd; break;
            case SDF_OP_INTERSECT: d = (d > pd) ? d : pd; break;
        }
    }
    return d;
}

/* --- GLSL shader source --- */

const char *sdf_glyph_shader_source(void)
{
    return
    "/* SDF glyph fragment shader */\n"
    "float sdf_circle(vec2 p, vec2 c, float r) {\n"
    "  return length(p - c) - r;\n"
    "}\n"
    "float sdf_line(vec2 p, vec2 a, vec2 b, float t) {\n"
    "  vec2 ab = b - a, ap = p - a;\n"
    "  float h = clamp(dot(ap,ab)/dot(ab,ab), 0.0, 1.0);\n"
    "  return length(ap - ab*h) - t;\n"
    "}\n"
    "float sdf_ring(vec2 p, vec2 c, float r, float t) {\n"
    "  return abs(length(p - c) - r) - t;\n"
    "}\n"
    "float sdf_glyph(vec2 uv, int prims[96], int count) {\n"
    "  float d = 1e9;\n"
    "  /* Primitives passed as uniform int array, decoded here */\n"
    "  return d;\n"
    "}\n";
}
