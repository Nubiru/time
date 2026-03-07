/*
 * star_field.c — Star field GPU data packing
 *
 * Merges star_catalog (195) and star_catalog_ext (400) with deduplication,
 * packs into interleaved vertex arrays for GPU upload. Packs constellation
 * line segments with zodiac-first ordering. Contains GLSL ES 3.00 shader
 * source strings.
 *
 * Deduplication: stars from star_catalog that also appear in star_catalog_ext
 * (RA/Dec proximity < 0.01 degree) are skipped; ext catalog data is preferred
 * as the authoritative source (ext catalog already contains star_catalog data).
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "star_field.h"

#include "constellation.h"
#include "star_catalog.h"
#include "star_catalog_ext.h"
#include "star_colors.h"

#include <math.h>
#include <stddef.h>

#define PI 3.14159265358979323846

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: deduplication threshold (degrees)
 * ────────────────────────────────────────────────────────────────────── */

#define DEDUP_THRESHOLD 0.01f

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: check if a star from star_catalog is duplicated in ext catalog.
 * Returns 1 if the star at (ra_hours, dec_deg) has a match in ext catalog.
 * ────────────────────────────────────────────────────────────────────── */

static int is_duplicate_in_ext(float ra_hours, float dec_deg)
{
    /* Convert RA from hours to degrees for comparison */
    float ra_deg = ra_hours * 15.0f;
    int ext_n = ext_star_count();
    for (int i = 0; i < ext_n; i++) {
        ext_star_t es = ext_star_get(i);
        float ext_ra_deg = es.ra_hours * 15.0f;
        float dra = ra_deg - ext_ra_deg;
        float ddec = dec_deg - es.dec_deg;
        /* Handle RA wrap-around */
        if (dra > 180.0f) dra -= 360.0f;
        if (dra < -180.0f) dra += 360.0f;
        if (dra < 0.0f) dra = -dra;
        if (ddec < 0.0f) ddec = -ddec;
        if (dra < DEDUP_THRESHOLD && ddec < DEDUP_THRESHOLD) {
            return 1;
        }
    }
    return 0;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: count unique stars from star_catalog not in ext catalog.
 * ────────────────────────────────────────────────────────────────────── */

static int count_unique_in_primary(void)
{
    int count = 0;
    int n = star_catalog_count();
    for (int i = 0; i < n; i++) {
        star_entry_t e = star_catalog_entry(i);
        if (!is_duplicate_in_ext(e.ra_hours, e.dec_deg)) {
            count++;
        }
    }
    return count;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: temporary star record for sorting
 * ────────────────────────────────────────────────────────────────────── */

typedef struct {
    float ra_hours;
    float dec_deg;
    float magnitude;
    float bv;
} merged_star_t;

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: simple insertion sort by magnitude (ascending = brightest first)
 * ────────────────────────────────────────────────────────────────────── */

static void sort_by_magnitude(merged_star_t *stars, int n)
{
    for (int i = 1; i < n; i++) {
        merged_star_t key = stars[i];
        int j = i - 1;
        while (j >= 0 && stars[j].magnitude > key.magnitude) {
            stars[j + 1] = stars[j];
            j--;
        }
        stars[j + 1] = key;
    }
}

/* ──────────────────────────────────────────────────────────────────────────
 * star_field_total_count
 * ────────────────────────────────────────────────────────────────────── */

int star_field_total_count(void)
{
    return ext_star_count() + count_unique_in_primary();
}

/* ──────────────────────────────────────────────────────────────────────────
 * star_field_pack
 *
 * Strategy:
 * 1. Collect all ext catalog stars
 * 2. Add star_catalog stars not in ext (deduplicated)
 * 3. Sort by magnitude
 * 4. Write interleaved vertex data: x,y,z, r,g,b, size
 * ────────────────────────────────────────────────────────────────────── */

int star_field_pack(float *out, int max_stars, float base_point_size,
                    float sphere_radius)
{
    if (out == NULL || max_stars <= 0) return 0;

    /* Merge into a static scratch buffer.
     * Max possible: 400 + 195 = 595 (before dedup). */
    static merged_star_t merged[600];
    int merged_count = 0;

    /* Add all ext catalog stars */
    int ext_n = ext_star_count();
    for (int i = 0; i < ext_n && merged_count < 600; i++) {
        ext_star_t es = ext_star_get(i);
        merged[merged_count].ra_hours = es.ra_hours;
        merged[merged_count].dec_deg = es.dec_deg;
        merged[merged_count].magnitude = es.v_mag;
        merged[merged_count].bv = es.bv;
        merged_count++;
    }

    /* Add unique stars from primary catalog */
    int prim_n = star_catalog_count();
    for (int i = 0; i < prim_n && merged_count < 600; i++) {
        star_entry_t e = star_catalog_entry(i);
        if (!is_duplicate_in_ext(e.ra_hours, e.dec_deg)) {
            merged[merged_count].ra_hours = e.ra_hours;
            merged[merged_count].dec_deg = e.dec_deg;
            merged[merged_count].magnitude = e.magnitude;
            merged[merged_count].bv = e.bv;
            merged_count++;
        }
    }

    /* Sort by magnitude (brightest first) */
    sort_by_magnitude(merged, merged_count);

    /* Limit to max_stars */
    int write_count = (merged_count < max_stars) ? merged_count : max_stars;

    /* Pack vertex data */
    for (int i = 0; i < write_count; i++) {
        int off = i * STAR_VERTEX_FLOATS;
        merged_star_t *s = &merged[i];

        /* Position: ecliptic XYZ scaled by sphere_radius */
        star_xyz_t pos = star_to_ecliptic_xyz(s->ra_hours, s->dec_deg,
                                               STAR_FIELD_OBLIQUITY);
        out[off + 0] = pos.x * sphere_radius;
        out[off + 1] = pos.y * sphere_radius;
        out[off + 2] = pos.z * sphere_radius;

        /* Color: from B-V via Ballesteros formula (star_colors.h) */
        star_rgb_t color = star_color_from_bv(s->bv);
        out[off + 3] = color.r;
        out[off + 4] = color.g;
        out[off + 5] = color.b;

        /* Size: from magnitude */
        out[off + 6] = star_mag_to_size(s->magnitude, base_point_size);
    }

    return write_count;
}

/* ──────────────────────────────────────────────────────────────────────────
 * star_field_info
 *
 * Computes tier boundaries based on magnitude thresholds from ext catalog:
 * Tier 0: mag < 1.5, Tier 1: 1.5-3.0, Tier 2: 3.0-4.5, Tier 3: 4.5-6.5
 *
 * Since stars are sorted by magnitude in the packed buffer, tiers are
 * contiguous ranges. We recompute from the total count rather than
 * re-reading the buffer, using the ext catalog tier thresholds.
 * ────────────────────────────────────────────────────────────────────── */

star_field_info_t star_field_info(int star_count)
{
    star_field_info_t info;
    info.vertex_count = star_count;
    info.float_count = star_count * STAR_VERTEX_FLOATS;

    /* We need to recompute tier boundaries by re-merging.
     * Use the same merge + sort logic, then count per tier. */
    static merged_star_t merged[600];
    int merged_count = 0;

    int ext_n = ext_star_count();
    for (int i = 0; i < ext_n && merged_count < 600; i++) {
        ext_star_t es = ext_star_get(i);
        merged[merged_count].ra_hours = es.ra_hours;
        merged[merged_count].dec_deg = es.dec_deg;
        merged[merged_count].magnitude = es.v_mag;
        merged[merged_count].bv = es.bv;
        merged_count++;
    }

    int prim_n = star_catalog_count();
    for (int i = 0; i < prim_n && merged_count < 600; i++) {
        star_entry_t e = star_catalog_entry(i);
        if (!is_duplicate_in_ext(e.ra_hours, e.dec_deg)) {
            merged[merged_count].ra_hours = e.ra_hours;
            merged[merged_count].dec_deg = e.dec_deg;
            merged[merged_count].magnitude = e.magnitude;
            merged[merged_count].bv = e.bv;
            merged_count++;
        }
    }

    sort_by_magnitude(merged, merged_count);

    /* Limit to star_count */
    int n = (merged_count < star_count) ? merged_count : star_count;

    /* Count stars per tier */
    int tier_counts[4] = {0, 0, 0, 0};
    for (int i = 0; i < n; i++) {
        int t = ext_star_tier_for_mag(merged[i].magnitude);
        tier_counts[t]++;
    }

    /* Compute offsets */
    info.tier_offsets[0] = 0;
    info.tier_counts[0] = tier_counts[0];
    for (int t = 1; t < 4; t++) {
        info.tier_offsets[t] = info.tier_offsets[t - 1] + tier_counts[t - 1];
        info.tier_counts[t] = tier_counts[t];
    }

    return info;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Zodiac constellation detection
 *
 * 12 zodiac constellations identified by IAU abbreviation.
 * ────────────────────────────────────────────────────────────────────── */

static const char *ZODIAC_ABBRS[12] = {
    "Ari", "Tau", "Gem", "Cnc", "Leo", "Vir",
    "Lib", "Sco", "Sgr", "Cap", "Aqr", "Psc"
};

static int is_zodiac(const char *abbr)
{
    if (abbr == NULL) return 0;
    for (int i = 0; i < 12; i++) {
        const char *za = ZODIAC_ABBRS[i];
        const char *ab = abbr;
        int match = 1;
        while (*za && *ab) {
            if (*za != *ab) { match = 0; break; }
            za++;
            ab++;
        }
        if (*za != *ab) match = 0;
        if (match) return 1;
    }
    return 0;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: count total zodiac lines
 * ────────────────────────────────────────────────────────────────────── */

static int count_zodiac_lines(void)
{
    int count = 0;
    int n = constellation_count();
    for (int i = 0; i < n; i++) {
        constellation_t c = constellation_get(i);
        if (c.name != NULL && is_zodiac(c.abbr)) {
            count += c.line_count;
        }
    }
    return count;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: write a line segment's two endpoints into the buffer.
 * Returns 1 on success, 0 if star index is invalid.
 * ────────────────────────────────────────────────────────────────────── */

static int write_line_vertices(float *out, int offset, int star_a, int star_b,
                               float r, float g, float b, float alpha,
                               float sphere_radius)
{
    /* Resolve star indices via star_catalog */
    star_entry_t ea = star_catalog_entry(star_a);
    star_entry_t eb = star_catalog_entry(star_b);
    if (ea.magnitude >= 90.0f || eb.magnitude >= 90.0f) return 0;

    star_xyz_t pa = star_to_ecliptic_xyz(ea.ra_hours, ea.dec_deg,
                                          STAR_FIELD_OBLIQUITY);
    star_xyz_t pb = star_to_ecliptic_xyz(eb.ra_hours, eb.dec_deg,
                                          STAR_FIELD_OBLIQUITY);

    /* Endpoint A: x,y,z, r,g,b,a */
    out[offset + 0] = pa.x * sphere_radius;
    out[offset + 1] = pa.y * sphere_radius;
    out[offset + 2] = pa.z * sphere_radius;
    out[offset + 3] = r;
    out[offset + 4] = g;
    out[offset + 5] = b;
    out[offset + 6] = alpha;

    /* Endpoint B: x,y,z, r,g,b,a */
    int off_b = offset + CLINE_VERTEX_FLOATS;
    out[off_b + 0] = pb.x * sphere_radius;
    out[off_b + 1] = pb.y * sphere_radius;
    out[off_b + 2] = pb.z * sphere_radius;
    out[off_b + 3] = r;
    out[off_b + 4] = g;
    out[off_b + 5] = b;
    out[off_b + 6] = alpha;

    return 1;
}

/* ──────────────────────────────────────────────────────────────────────────
 * constellation_lines_pack
 *
 * Packs constellation line segments. Zodiac constellations first with
 * higher alpha, then non-zodiac with base alpha.
 * Line color: cool blue-white (0.6, 0.7, 0.9, alpha).
 * ────────────────────────────────────────────────────────────────────── */

int constellation_lines_pack(float *out, int max_lines, float base_alpha,
                             float zodiac_alpha, float sphere_radius)
{
    if (out == NULL || max_lines <= 0) return 0;

    /* Line color: cool blue-white */
    float lr = 0.6f;
    float lg = 0.7f;
    float lb = 0.9f;

    int lines_written = 0;
    int n_const = constellation_count();

    /* Pass 1: zodiac constellations */
    for (int ci = 0; ci < n_const && lines_written < max_lines; ci++) {
        constellation_t c = constellation_get(ci);
        if (c.name == NULL || !is_zodiac(c.abbr)) continue;
        for (int li = 0; li < c.line_count && lines_written < max_lines; li++) {
            int off = lines_written * 2 * CLINE_VERTEX_FLOATS;
            if (write_line_vertices(out, off, c.lines[li].star_a,
                                    c.lines[li].star_b,
                                    lr, lg, lb, zodiac_alpha,
                                    sphere_radius)) {
                lines_written++;
            }
        }
    }

    /* Pass 2: non-zodiac constellations */
    for (int ci = 0; ci < n_const && lines_written < max_lines; ci++) {
        constellation_t c = constellation_get(ci);
        if (c.name == NULL || is_zodiac(c.abbr)) continue;
        for (int li = 0; li < c.line_count && lines_written < max_lines; li++) {
            int off = lines_written * 2 * CLINE_VERTEX_FLOATS;
            if (write_line_vertices(out, off, c.lines[li].star_a,
                                    c.lines[li].star_b,
                                    lr, lg, lb, base_alpha,
                                    sphere_radius)) {
                lines_written++;
            }
        }
    }

    return lines_written;
}

/* ──────────────────────────────────────────────────────────────────────────
 * constellation_lines_info
 * ────────────────────────────────────────────────────────────────────── */

constellation_line_info_t constellation_lines_info(int line_count)
{
    constellation_line_info_t info;
    info.line_count = line_count;
    info.vertex_count = line_count * 2;
    info.float_count = info.vertex_count * CLINE_VERTEX_FLOATS;
    info.zodiac_line_count = count_zodiac_lines();
    /* Cap zodiac count if fewer lines were actually packed */
    if (info.zodiac_line_count > line_count) {
        info.zodiac_line_count = line_count;
    }
    return info;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Shader source strings — GLSL ES 3.00 (WebGL2)
 *
 * Star vertex shader: MVP transform + perspective-scaled gl_PointSize.
 * Star fragment shader: circular point with smoothstep falloff.
 * Line vertex shader: MVP transform, pass color.
 * Line fragment shader: passthrough color.
 * ────────────────────────────────────────────────────────────────────── */

static const char STAR_VERT_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "uniform float u_scale_factor;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_color;\n"
    "layout(location = 2) in float a_size;\n"
    "\n"
    "out vec3 v_color;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    float ps = a_size * u_scale_factor / gl_Position.w;\n"
    "    gl_PointSize = clamp(ps, 1.0, 32.0);\n"
    "    v_color = a_color;\n"
    "}\n";

static const char STAR_FRAG_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec3 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    vec2 pc = gl_PointCoord - vec2(0.5);\n"
    "    float dist = length(pc) * 2.0;\n"
    "    float alpha = 1.0 - smoothstep(0.5, 1.0, dist);\n"
    "    frag_color = vec4(v_color * alpha, alpha);\n"
    "}\n";

static const char CLINE_VERT_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec4 a_color;\n"
    "\n"
    "out vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    v_color = a_color;\n"
    "}\n";

static const char CLINE_FRAG_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    frag_color = v_color;\n"
    "}\n";

/* ──────────────────────────────────────────────────────────────────────── */

const char *star_field_vert_source(void)
{
    return STAR_VERT_SOURCE;
}

const char *star_field_frag_source(void)
{
    return STAR_FRAG_SOURCE;
}

const char *constellation_line_vert_source(void)
{
    return CLINE_VERT_SOURCE;
}

const char *constellation_line_frag_source(void)
{
    return CLINE_FRAG_SOURCE;
}
