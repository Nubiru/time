/*
 * milkyway_pack.c -- Procedural Milky Way galaxy band vertex packing
 *
 * Generates a lat/lon mesh on the celestial sphere using galactic coordinates.
 * 13 named regions (bright clouds and dark rifts) modulate per-vertex
 * brightness via Gaussian-weighted influence. Galactic (l, b) coordinates
 * are converted to equatorial (RA, Dec) using the standard IAU J2000
 * transformation, then mapped to Cartesian XYZ on the celestial sphere.
 *
 * Fragment shader uses noise_shader.h fbm2 for procedural cloud texture.
 *
 * Near-pure module: no GL calls, no malloc, no globals.
 * Uses mutable static buffers for lazy shader string concatenation (sun_shader.c pattern).
 */

#include "milkyway_pack.h"
#include "noise_shader.h"
#include "shader_builder.h"

#include <math.h>

/* ======================================================================
 * Constants
 * ====================================================================== */

#define DEG2RAD(d) ((d) * PI / 180.0)
#define RAD2DEG(r) ((r) * 180.0 / PI)

/* North Galactic Pole in equatorial coordinates (J2000) */
#define NGP_RA_DEG  192.85
#define NGP_DEC_DEG  27.13

/* Galactic longitude of the north celestial pole (IAU 1958) */
#define L_NCP_DEG  123.0

/* Default band half-width used by mw_brightness_at edge falloff */
#define DEFAULT_BAND_HALF_WIDTH 15.0f

/* Default edge falloff exponent */
#define DEFAULT_EDGE_FALLOFF 2.0f

/* ======================================================================
 * Named Galactic Regions (13 total)
 * ====================================================================== */

#define MW_REGION_COUNT 13

static const mw_region_t s_regions[MW_REGION_COUNT] = {
    /* Bright clouds (brightness > 1.0) */
    { "Galactic Center",     0.0f,   0.0f,  15.0f, 2.0f  },
    { "Cygnus Star Cloud",  80.0f,   0.0f,  12.0f, 1.6f  },
    { "Scutum Star Cloud",  28.0f,  -2.0f,   8.0f, 1.5f  },
    { "Carina Nebula",     287.0f,  -1.0f,  10.0f, 1.4f  },
    { "Centaurus",         310.0f,   0.0f,  12.0f, 1.3f  },
    { "Norma Star Cloud",  330.0f,  -1.0f,   8.0f, 1.3f  },
    { "Vela",              265.0f,  -2.0f,  10.0f, 1.1f  },

    /* Dark rifts (brightness < 1.0) */
    { "Great Rift",          30.0f,  0.0f,  20.0f, 0.3f  },
    { "Coal Sack",          300.0f, -1.0f,   6.0f, 0.15f },
    { "Pipe Nebula",          0.0f,  5.0f,   4.0f, 0.2f  },
    { "Dark Horse",          15.0f,  5.0f,   5.0f, 0.25f },
    { "Northern Coal Sack",  65.0f,  3.0f,   5.0f, 0.3f  },
    { "Aquila Rift",         20.0f,  3.0f,   8.0f, 0.35f },
};

/* ======================================================================
 * mw_default_config
 * ====================================================================== */

mw_config_t mw_default_config(void)
{
    mw_config_t cfg;
    cfg.sphere_radius      = 200.0f;
    cfg.band_width_deg     = 30.0f;
    cfg.core_brightness    = 1.5f;
    cfg.edge_falloff       = 2.0f;
    cfg.longitude_segments = 72;
    cfg.latitude_segments  = 12;
    return cfg;
}

/* ======================================================================
 * mw_region_count / mw_region_get
 * ====================================================================== */

int mw_region_count(void)
{
    return MW_REGION_COUNT;
}

mw_region_t mw_region_get(int index)
{
    if (index < 0 || index >= MW_REGION_COUNT) {
        mw_region_t empty;
        empty.name = 0;
        empty.l_center_deg = 0.0f;
        empty.b_center_deg = 0.0f;
        empty.angular_radius = 0.0f;
        empty.brightness = 0.0f;
        return empty;
    }
    return s_regions[index];
}

/* ======================================================================
 * Angular distance between two galactic coordinates (degrees)
 *
 * Uses the haversine formula for accuracy at small separations.
 * ====================================================================== */

static float angular_distance_deg(float l1, float b1, float l2, float b2)
{
    double l1r = DEG2RAD(l1);
    double b1r = DEG2RAD(b1);
    double l2r = DEG2RAD(l2);
    double b2r = DEG2RAD(b2);

    double dlat = b2r - b1r;
    double dlon = l2r - l1r;

    double a = sin(dlat / 2.0) * sin(dlat / 2.0)
             + cos(b1r) * cos(b2r) * sin(dlon / 2.0) * sin(dlon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return (float)RAD2DEG(c);
}

/* ======================================================================
 * mw_brightness_at
 *
 * For each galactic coordinate (l, b):
 * 1. Start with base brightness = 1.0
 * 2. For each named region, compute angular distance
 * 3. If distance < radius, blend using Gaussian falloff
 * 4. Bright regions (>1) add, dark regions (<1) reduce
 * 5. Multiply by edge falloff: pow(1 - |b|/half_width, edge_falloff)
 * ====================================================================== */

float mw_brightness_at(float l_deg, float b_deg)
{
    float brightness = 1.0f;

    for (int i = 0; i < MW_REGION_COUNT; i++) {
        const mw_region_t *r = &s_regions[i];
        float dist = angular_distance_deg(l_deg, b_deg,
                                           r->l_center_deg, r->b_center_deg);
        if (dist < r->angular_radius) {
            /* Gaussian-like falloff: exp(-3 * (d/r)^2) */
            float norm = dist / r->angular_radius;
            float weight = expf(-3.0f * norm * norm);

            /* Blend toward region brightness */
            if (r->brightness > 1.0f) {
                /* Bright cloud: additive boost */
                brightness += (r->brightness - 1.0f) * weight;
            } else {
                /* Dark rift: multiplicative reduction */
                float reduction = 1.0f - (1.0f - r->brightness) * weight;
                brightness *= reduction;
            }
        }
    }

    /* Edge falloff based on galactic latitude */
    float half_width = DEFAULT_BAND_HALF_WIDTH;
    float b_abs = fabsf(b_deg);
    if (b_abs >= half_width) {
        brightness = 0.0f;
    } else {
        float edge_t = 1.0f - b_abs / half_width;
        float falloff = powf(edge_t, DEFAULT_EDGE_FALLOFF);
        brightness *= falloff;
    }

    if (brightness < 0.0f) brightness = 0.0f;

    return brightness;
}

/* ======================================================================
 * mw_galactic_to_equatorial
 *
 * Standard IAU J2000 galactic-to-equatorial coordinate conversion.
 *
 * sin(dec) = sin(dec_ngp)*sin(b) + cos(dec_ngp)*cos(b)*cos(l - l_NCP)
 * sin(ra - ra_ngp) = cos(b)*sin(l_NCP - l) / cos(dec)
 *
 * Where:
 *   dec_ngp = 27.13 deg, ra_ngp = 192.85 deg, l_NCP = 123.0 deg
 * ====================================================================== */

void mw_galactic_to_equatorial(float l_deg, float b_deg,
                                float *ra_hours, float *dec_deg)
{
    double l_rad = DEG2RAD((double)l_deg);
    double b_rad = DEG2RAD((double)b_deg);
    double dec_ngp_rad = DEG2RAD(NGP_DEC_DEG);
    double ra_ngp_rad  = DEG2RAD(NGP_RA_DEG);
    double l_ncp_rad   = DEG2RAD(L_NCP_DEG);

    double sin_b = sin(b_rad);
    double cos_b = cos(b_rad);
    double sin_dec_ngp = sin(dec_ngp_rad);
    double cos_dec_ngp = cos(dec_ngp_rad);

    /* Standard IAU galactic-to-equatorial conversion:
     * sin(dec) = sin(dec_NGP)*sin(b) + cos(dec_NGP)*cos(b)*cos(l - l_NCP)
     */
    double dl = l_rad - l_ncp_rad;
    double sin_dec = sin_dec_ngp * sin_b + cos_dec_ngp * cos_b * cos(dl);
    if (sin_dec > 1.0) sin_dec = 1.0;
    if (sin_dec < -1.0) sin_dec = -1.0;
    double dec_rad = asin(sin_dec);

    /* Right ascension:
     * cos(dec)*sin(alpha - alpha_NGP) = cos(b)*sin(l_NCP - l)
     * cos(dec)*cos(alpha - alpha_NGP) = sin(b)*cos(dec_NGP)
     *                                   - cos(b)*sin(dec_NGP)*cos(l - l_NCP)
     */
    double cos_dec = cos(dec_rad);
    double sin_ra_diff, cos_ra_diff;

    if (fabs(cos_dec) < 1e-12) {
        /* At poles, RA is arbitrary */
        sin_ra_diff = 0.0;
        cos_ra_diff = 1.0;
    } else {
        sin_ra_diff = cos_b * sin(l_ncp_rad - l_rad) / cos_dec;
        cos_ra_diff = (sin_b * cos_dec_ngp
                       - cos_b * sin_dec_ngp * cos(dl)) / cos_dec;
    }

    double ra_diff = atan2(sin_ra_diff, cos_ra_diff);
    double ra_rad = ra_ngp_rad + ra_diff;

    /* Normalize RA to [0, 2*PI) */
    double two_pi = 2.0 * PI;
    while (ra_rad < 0.0) ra_rad += two_pi;
    while (ra_rad >= two_pi) ra_rad -= two_pi;

    /* Convert to output units */
    *ra_hours = (float)(RAD2DEG(ra_rad) / 15.0);
    *dec_deg  = (float)RAD2DEG(dec_rad);
}

/* ======================================================================
 * mw_pack
 *
 * Generates a lat/lon grid on the celestial sphere in galactic coordinates:
 *   Longitude: 0-360 deg around galactic equator
 *   Latitude: -band_width/2 to +band_width/2
 *
 * Each vertex: position(vec3) + galactic_coord(vec2) + brightness(float) = 6 floats
 *
 * Vertices are indexed: each grid quad becomes 2 triangles.
 * Galactic (l, b) -> equatorial (RA, Dec) -> Cartesian XYZ on sphere.
 * ====================================================================== */

static void emit_vertex(float *out, int idx,
                         float x, float y, float z,
                         float l_norm, float b_norm, float brightness)
{
    int base = idx * MW_VERTEX_FLOATS;
    out[base + 0] = x;
    out[base + 1] = y;
    out[base + 2] = z;
    out[base + 3] = l_norm;
    out[base + 4] = b_norm;
    out[base + 5] = brightness;
}

mw_info_t mw_pack(mw_config_t config, float *out_verts, unsigned short *out_indices)
{
    mw_info_t info;
    info.vertex_count = 0;
    info.index_count = 0;
    info.triangle_count = 0;

    int lon_seg = config.longitude_segments;
    int lat_seg = config.latitude_segments;
    float half_bw = config.band_width_deg / 2.0f;
    float radius = config.sphere_radius;

    int verts_needed = (lon_seg + 1) * (lat_seg + 1);
    if (verts_needed > MW_MAX_VERTICES) {
        /* Clamp to fit */
        return info;
    }

    int vert_idx = 0;

    /* Generate vertices */
    for (int j = 0; j <= lat_seg; j++) {
        float b_deg = -half_bw + (float)j * (2.0f * half_bw) / (float)lat_seg;
        float b_norm = b_deg / half_bw; /* [-1, 1] */

        for (int i = 0; i <= lon_seg; i++) {
            float l_deg = (float)i * 360.0f / (float)lon_seg;
            float l_norm = l_deg / 360.0f; /* [0, 1] */

            /* Brightness from named regions */
            float brightness = mw_brightness_at(l_deg, b_deg);

            /* Convert galactic to equatorial */
            float ra_hours, dec_d;
            mw_galactic_to_equatorial(l_deg, b_deg, &ra_hours, &dec_d);

            /* Equatorial to Cartesian on sphere */
            double ra_rad = DEG2RAD((double)ra_hours * 15.0);
            double dec_rad = DEG2RAD((double)dec_d);

            float x = radius * (float)(cos(dec_rad) * cos(ra_rad));
            float y = radius * (float)(sin(dec_rad));
            float z = -radius * (float)(cos(dec_rad) * sin(ra_rad));

            emit_vertex(out_verts, vert_idx, x, y, z,
                         l_norm, b_norm, brightness);
            vert_idx++;
        }
    }

    info.vertex_count = vert_idx;

    /* Generate indices: each quad is 2 triangles */
    int idx = 0;
    for (int j = 0; j < lat_seg; j++) {
        for (int i = 0; i < lon_seg; i++) {
            int row0 = j * (lon_seg + 1);
            int row1 = (j + 1) * (lon_seg + 1);

            unsigned short bl = (unsigned short)(row0 + i);
            unsigned short br = (unsigned short)(row0 + i + 1);
            unsigned short tl = (unsigned short)(row1 + i);
            unsigned short tr = (unsigned short)(row1 + i + 1);

            /* Triangle 1: bl, br, tr */
            out_indices[idx++] = bl;
            out_indices[idx++] = br;
            out_indices[idx++] = tr;

            /* Triangle 2: bl, tr, tl */
            out_indices[idx++] = bl;
            out_indices[idx++] = tr;
            out_indices[idx++] = tl;
        }
    }

    info.index_count = idx;
    info.triangle_count = idx / 3;

    return info;
}

/* ======================================================================
 * GLSL ES 3.00 shader source strings
 * ====================================================================== */

/* Vertex shader: passthrough with galactic coords and brightness */
static const char *s_vert_source =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec2 a_galcoord;\n"
    "layout(location = 2) in float a_brightness;\n"
    "uniform mat4 u_mvp;\n"
    "out vec2 v_galcoord;\n"
    "out float v_brightness;\n"
    "void main() {\n"
    "    v_galcoord = a_galcoord;\n"
    "    v_brightness = a_brightness;\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "}\n";

/* Fragment shader preamble: version, precision, uniforms, inputs */
static const char *s_frag_preamble =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_galcoord;\n"
    "in float v_brightness;\n"
    "uniform float u_time;\n"
    "uniform vec2 u_resolution;\n"
    "out vec4 frag_color;\n"
    "\n";

/* Fragment shader body: domain-warped Milky Way with dust lanes and core glow
 *
 * Techniques from Inigo Quilez (domain warping, cosine palettes),
 * GPU Gems 3, and Stellarium project. See shader-milkyway-galaxy.md digest.
 *
 * ~32 noise evaluations per pixel (high quality path).
 */
static const char *s_frag_body =
    "\n"
    /* Color constants: physically motivated stellar populations */
    "const vec3 MW_CORE_WARM  = vec3(1.0, 0.92, 0.75);\n"
    "const vec3 MW_EDGE_COOL  = vec3(0.65, 0.70, 0.85);\n"
    "const vec3 MW_CENTER_GOLD = vec3(1.0, 0.85, 0.55);\n"
    "\n"
    /* Cosine palette for subtle hue variation (Quilez method) */
    "vec3 mw_palette(float t) {\n"
    "    vec3 a = vec3(0.50, 0.48, 0.45);\n"
    "    vec3 b = vec3(0.35, 0.30, 0.25);\n"
    "    vec3 c = vec3(0.50, 0.50, 0.50);\n"
    "    vec3 d = vec3(0.00, 0.05, 0.15);\n"
    "    return a + b * cos(6.283185 * (c * t + d));\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    vec2 uv = v_galcoord;\n"
    "    float lat_abs = abs(uv.y);\n"
    "\n"
    "    /* --- Edge boundary with noise modulation --- */\n"
    "    float edge_noise = snoise2(uv * vec2(8.0, 1.5)) * 0.12;\n"
    "    float edge = smoothstep(1.0, 0.15, lat_abs + edge_noise);\n"
    "    if (edge < 0.005) discard;\n"
    "\n"
    "    /* --- Slow animation offset --- */\n"
    "    float t = u_time * 0.008;\n"
    "\n"
    "    /* --- Domain-warped base cloud structure --- */\n"
    "    vec2 base_uv = uv * vec2(5.0, 2.5);\n"
    "\n"
    "    /* First warp layer: large-scale flow */\n"
    "    vec2 q;\n"
    "    q.x = fbm2(base_uv + vec2(t * 0.3, 0.0));\n"
    "    q.y = fbm2(base_uv + vec2(5.2, 1.3 + t * 0.1));\n"
    "\n"
    "    /* Second warp layer: fine turbulence */\n"
    "    vec2 r;\n"
    "    r.x = fbm2(base_uv + 3.0 * q + vec2(1.7, 9.2));\n"
    "    r.y = fbm2(base_uv + 3.0 * q + vec2(8.3, 2.8));\n"
    "\n"
    "    /* Warped density field */\n"
    "    float density = fbm2(base_uv + 3.0 * r);\n"
    "    density = density * 0.5 + 0.5;\n"
    "    density = pow(density, 0.8);\n"
    "\n"
    "    /* --- Dust lane absorption --- */\n"
    "    vec2 dust_uv = uv * vec2(7.0, 3.5) + vec2(t * 0.2, 0.0);\n"
    "    float dust_warp = snoise2(dust_uv * 0.6) * 0.4;\n"
    "    float dust_raw = vfbm(dust_uv + vec2(dust_warp, dust_warp * 0.7));\n"
    "\n"
    "    /* Threshold: sharp boundary between dust and clear */\n"
    "    float dust_mask = smoothstep(-0.05, 0.45, dust_raw);\n"
    "\n"
    "    /* Dust absorption concentrated near galactic plane */\n"
    "    float plane_conc = exp(-uv.y * uv.y * 6.0);\n"
    "    float transmission = mix(1.0, mix(0.12, 1.0, dust_mask), plane_conc);\n"
    "\n"
    "    /* --- Galactic center glow --- */\n"
    "    float lx = uv.x;\n"
    "    float dl = min(lx, 1.0 - lx) * 2.0;\n"
    "    float center_dist = length(vec2(dl, uv.y * 2.5));\n"
    "    float core_glow = exp(-center_dist * center_dist * 8.0) * 1.8;\n"
    "\n"
    "    /* --- Fine star-dust detail (high frequency, cheap noise) --- */\n"
    "    float fine = vnoise(uv * vec2(40.0, 15.0) + vec2(t * 0.5, 0.0));\n"
    "    fine = fine * 0.3 + 0.7;\n"
    "\n"
    "    /* --- Combine all layers --- */\n"
    "    float total = (density * transmission + core_glow) * edge * v_brightness * fine;\n"
    "    total = clamp(total, 0.0, 1.0);\n"
    "\n"
    "    /* --- Color --- */\n"
    "    /* Base: warm core to cool edge based on latitude */\n"
    "    vec3 color = mix(MW_CORE_WARM, MW_EDGE_COOL, lat_abs);\n"
    "\n"
    "    /* Galactic center: extra golden warmth */\n"
    "    float center_warmth = exp(-center_dist * center_dist * 5.0);\n"
    "    color = mix(color, MW_CENTER_GOLD, center_warmth * 0.4);\n"
    "\n"
    "    /* Subtle hue variation from domain warp intermediates */\n"
    "    float hue_shift = (q.x + r.y) * 0.1;\n"
    "    color *= 1.0 + hue_shift * vec3(-0.05, 0.0, 0.08);\n"
    "\n"
    "    /* Apply brightness */\n"
    "    color *= total;\n"
    "\n"
    "    /* Final alpha: soft edges, dust transparency */\n"
    "    float alpha = total * 0.55;\n"
    "\n"
    "    frag_color = vec4(color, alpha);\n"
    "}\n";

/* Concatenated fragment source: preamble + noise lib + body */
static shader_src_t s_frag_src;
static int s_frag_built = 0;

static const char *build_frag_source(void)
{
    if (!s_frag_built) {
        shader_src_init(&s_frag_src);
        shader_src_append(&s_frag_src, s_frag_preamble);
        shader_src_append(&s_frag_src, noise_shader_source());
        shader_src_append(&s_frag_src, s_frag_body);
        s_frag_built = 1;
    }
    return shader_src_get(&s_frag_src);
}

const char *mw_vert_source(void) { return s_vert_source; }
const char *mw_frag_source(void) { return build_frag_source(); }
