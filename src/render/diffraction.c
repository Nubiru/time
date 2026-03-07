/*
 * diffraction.c — Diffraction spike vertex packing for bright stars
 *
 * Generates 4 or 6 radial spike lines per bright star. Each spike is a
 * GL_LINES segment from the star center to a tip point in the star's
 * local tangent plane on the celestial sphere. Alpha fades linearly
 * from center to tip.
 *
 * Spike directions are computed by deriving two orthogonal tangent vectors
 * at the star's position on the unit sphere, then rotating by evenly-spaced
 * angles plus an optional rotation offset.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "diffraction.h"

#include <math.h>
#include <stddef.h>

#define PI 3.14159265358979323846

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: clamp spike_count to valid range [4, 6]
 * ────────────────────────────────────────────────────────────────────── */

static int clamp_spike_count(int n)
{
    if (n < 4) return 4;
    if (n > 6) return 6;
    return n;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: compute two orthogonal tangent vectors at a point on the sphere.
 * pos must be a non-zero vector. t1 and t2 will be unit-length tangent vectors
 * perpendicular to pos and to each other.
 * ────────────────────────────────────────────────────────────────────── */

static void tangent_basis(const float *pos, float *t1, float *t2)
{
    /* Normalize position */
    float len = sqrtf(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
    float nx, ny, nz;
    if (len < 1e-8f) {
        /* Degenerate: use default basis */
        t1[0] = 1.0f; t1[1] = 0.0f; t1[2] = 0.0f;
        t2[0] = 0.0f; t2[1] = 1.0f; t2[2] = 0.0f;
        return;
    }
    nx = pos[0] / len;
    ny = pos[1] / len;
    nz = pos[2] / len;

    /* Choose an up vector not parallel to n */
    float ux, uy, uz;
    if (fabsf(ny) < 0.9f) {
        ux = 0.0f; uy = 1.0f; uz = 0.0f;
    } else {
        ux = 0.0f; uy = 0.0f; uz = 1.0f;
    }

    /* t1 = normalize(cross(up, n)) */
    float cx = uy * nz - uz * ny;
    float cy = uz * nx - ux * nz;
    float cz = ux * ny - uy * nx;
    float clen = sqrtf(cx * cx + cy * cy + cz * cz);
    if (clen < 1e-8f) {
        t1[0] = 1.0f; t1[1] = 0.0f; t1[2] = 0.0f;
        t2[0] = 0.0f; t2[1] = 1.0f; t2[2] = 0.0f;
        return;
    }
    t1[0] = cx / clen;
    t1[1] = cy / clen;
    t1[2] = cz / clen;

    /* t2 = cross(n, t1) — already unit length since n and t1 are unit & perp */
    t2[0] = ny * t1[2] - nz * t1[1];
    t2[1] = nz * t1[0] - nx * t1[2];
    t2[2] = nx * t1[1] - ny * t1[0];
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: write one vertex (position vec3 + color vec4) into buffer.
 * Returns number of floats written (always DIFFRACTION_VERTEX_FLOATS).
 * ────────────────────────────────────────────────────────────────────── */

static int write_vertex(float *out, float px, float py, float pz,
                        float r, float g, float b, float a)
{
    out[0] = px;
    out[1] = py;
    out[2] = pz;
    out[3] = r;
    out[4] = g;
    out[5] = b;
    out[6] = a;
    return DIFFRACTION_VERTEX_FLOATS;
}

/* ──────────────────────────────────────────────────────────────────────────
 * GLSL shader source strings
 * ────────────────────────────────────────────────────────────────────── */

static const char DIFF_VERT_SOURCE[] =
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

static const char DIFF_FRAG_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    frag_color = v_color;\n"
    "}\n";

/* ──────────────────────────────────────────────────────────────────────────
 * Public API
 * ────────────────────────────────────────────────────────────────────── */

diffraction_config_t diffraction_default_config(void)
{
    diffraction_config_t cfg;
    cfg.base_length = 3.0f;
    cfg.min_magnitude = 1.5f;
    cfg.center_alpha = 0.8f;
    cfg.tip_alpha = 0.0f;
    cfg.rotation_deg = 45.0f;
    cfg.spike_count = 4;
    return cfg;
}

float diffraction_spike_length(float magnitude, float base_length,
                               float min_magnitude)
{
    if (magnitude >= min_magnitude) {
        return 0.0f;
    }
    /* Linear: length = base_length * (min_magnitude - magnitude) / min_magnitude
     * At mag=0: length = base_length
     * At mag=min_magnitude: length = 0
     * At mag<0 (very bright): length > base_length */
    return base_length * (min_magnitude - magnitude) / min_magnitude;
}

float diffraction_spike_alpha(float t, float center_alpha, float tip_alpha)
{
    /* Clamp t to [0, 1] */
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return center_alpha + t * (tip_alpha - center_alpha);
}

diffraction_info_t diffraction_info(int star_count, int spike_count)
{
    diffraction_info_t info;
    int sc = clamp_spike_count(spike_count);
    info.star_count = star_count;
    info.line_count = star_count * sc;
    info.vertex_count = info.line_count * 2;
    info.float_count = info.vertex_count * DIFFRACTION_VERTEX_FLOATS;
    return info;
}

int diffraction_pack(const float *star_positions, const float *star_colors,
                     const float *star_magnitudes, int count,
                     const diffraction_config_t *config, float *out)
{
    if (out == NULL || count <= 0) {
        return 0;
    }

    int spikes = clamp_spike_count(config->spike_count);
    float rot_rad = (float)(config->rotation_deg * PI / 180.0);
    float angle_step = (float)(2.0 * PI / spikes);
    int packed_stars = 0;
    int out_idx = 0;

    for (int i = 0; i < count; i++) {
        float mag = star_magnitudes[i];
        float spike_len = diffraction_spike_length(mag, config->base_length,
                                                    config->min_magnitude);
        if (spike_len <= 0.0f) {
            continue;
        }

        float px = star_positions[i * 3 + 0];
        float py = star_positions[i * 3 + 1];
        float pz = star_positions[i * 3 + 2];

        float cr = star_colors[i * 3 + 0];
        float cg = star_colors[i * 3 + 1];
        float cb = star_colors[i * 3 + 2];

        /* Compute tangent basis at star position */
        float pos[3] = { px, py, pz };
        float t1[3], t2[3];
        tangent_basis(pos, t1, t2);

        /* Generate spikes */
        for (int s = 0; s < spikes; s++) {
            float angle = rot_rad + (float)s * angle_step;
            float ca = cosf(angle);
            float sa = sinf(angle);

            /* Spike direction in tangent plane */
            float dx = t1[0] * ca + t2[0] * sa;
            float dy = t1[1] * ca + t2[1] * sa;
            float dz = t1[2] * ca + t2[2] * sa;

            /* Center vertex */
            out_idx += write_vertex(out + out_idx, px, py, pz,
                                     cr, cg, cb, config->center_alpha);

            /* Tip vertex */
            float tx = px + dx * spike_len;
            float ty = py + dy * spike_len;
            float tz = pz + dz * spike_len;
            out_idx += write_vertex(out + out_idx, tx, ty, tz,
                                     cr, cg, cb, config->tip_alpha);
        }
        packed_stars++;
    }

    return packed_stars;
}

const char *diffraction_vert_source(void)
{
    return DIFF_VERT_SOURCE;
}

const char *diffraction_frag_source(void)
{
    return DIFF_FRAG_SOURCE;
}
