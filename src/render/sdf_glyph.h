/* sdf_glyph.h — Procedural SDF glyph generation (S107)
 *
 * Code-generated symbols using Signed Distance Fields. "Every pixel earned."
 * Each glyph is a pure function: glyph_id → SDF → fragment color.
 * Resolution-independent. Infinitely scalable. No textures needed.
 *
 * Supported glyph sets:
 *   - 12 zodiac signs (Aries ♈ through Pisces ♓)
 *   - 8 planet symbols (Sun ☉ through Neptune ♆)
 *   - 20 Kin Maya seal outlines (simplified geometric forms)
 *   - 10 Sefirot nodes (circles with Hebrew letter shapes)
 *   - 8 I Ching trigrams (3 lines: solid/broken)
 *
 * Each glyph is defined as a combination of SDF primitives:
 *   circle, line, arc, box, ring — composed with union/subtract/intersect.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * The shader (sdf_glyph_pass) evaluates these SDFs in fragment shader.
 */

#ifndef TIME_SDF_GLYPH_H
#define TIME_SDF_GLYPH_H

/* Glyph categories */
typedef enum {
    SDF_CAT_ZODIAC = 0,    /* 12 signs */
    SDF_CAT_PLANET,        /* 8 planets + Sun + Moon */
    SDF_CAT_SEAL,          /* 20 Kin Maya seals (simplified) */
    SDF_CAT_SEFIRAH,       /* 10 Sefirot */
    SDF_CAT_TRIGRAM,       /* 8 I Ching trigrams */
    SDF_CAT_COUNT
} sdf_glyph_category_t;

/* SDF primitive types */
typedef enum {
    SDF_CIRCLE = 0,        /* circle(center, radius) */
    SDF_LINE,              /* line segment(a, b, thickness) */
    SDF_ARC,               /* arc(center, radius, start_angle, end_angle, thickness) */
    SDF_BOX,               /* rounded box(center, half_size, corner_radius) */
    SDF_RING,              /* ring(center, inner_r, outer_r) */
} sdf_prim_type_t;

/* SDF boolean operations */
typedef enum {
    SDF_OP_UNION = 0,      /* min(a, b) — combine shapes */
    SDF_OP_SUBTRACT,       /* max(a, -b) — cut shape b from a */
    SDF_OP_INTERSECT,      /* max(a, b) — keep only overlap */
} sdf_op_type_t;

/* A single SDF primitive */
typedef struct {
    sdf_prim_type_t type;
    float cx, cy;          /* center position (normalized -1 to 1) */
    float p1, p2, p3, p4;  /* type-dependent parameters */
    sdf_op_type_t op;      /* how to combine with previous */
} sdf_prim_t;

#define SDF_MAX_PRIMS 16   /* max primitives per glyph */

/* A complete glyph definition */
typedef struct {
    int id;                          /* glyph index within category */
    sdf_glyph_category_t category;
    const char *name;                /* "Aries", "Sun", "Dragon", etc. */
    int prim_count;
    sdf_prim_t prims[SDF_MAX_PRIMS];
} sdf_glyph_t;

/* Get a glyph definition.
 * Returns zeroed struct for invalid category/index. */
sdf_glyph_t sdf_glyph_get(sdf_glyph_category_t category, int index);

/* Get glyph count for a category. */
int sdf_glyph_count(sdf_glyph_category_t category);

/* Evaluate the SDF at a point (for CPU-side testing).
 * Returns signed distance: negative = inside, positive = outside.
 * px, py in normalized -1 to 1 range. */
float sdf_glyph_eval(const sdf_glyph_t *glyph, float px, float py);

/* Generate GLSL ES 3.00 fragment shader source for a glyph.
 * The shader evaluates the SDF and outputs alpha-blended color.
 * Returns static string — do NOT free.
 * NULL for invalid glyph. */
const char *sdf_glyph_shader_source(void);

#endif /* TIME_SDF_GLYPH_H */
