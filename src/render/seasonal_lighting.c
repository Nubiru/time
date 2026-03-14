/* seasonal_lighting.c — Seasonal sky gradient color ramps for Earth View.
 *
 * Computes sky gradients from first principles using simplified Rayleigh
 * scattering approximation. No external dependencies beyond math.h.
 *
 * Key concepts:
 *   - Sun elevation determines time-of-day regime (day/golden/blue/night)
 *   - Latitude + day-of-year determine seasonal modulation
 *   - Solar declination = 23.44 * sin(2*PI*(doy-81)/365)
 *   - Seasonal factor modulates saturation and warmth
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 */

#include "seasonal_lighting.h"
#include <math.h>

/* --------------------------------------------------------------------
 * Internal helpers
 * -------------------------------------------------------------------- */

static float slp_clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static float slp_lerpf(float a, float b, float t)
{
    return a + (b - a) * t;
}

/* Smoothstep for smooth transitions */
static float slp_smoothstep(float edge0, float edge1, float x)
{
    float t = slp_clampf((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

/* Solar declination (degrees) at day of year.
 * Simplified sinusoidal: dec = 23.44 * sin(2*PI*(doy-81)/365) */
static double slp_solar_declination(int day_of_year)
{
    return 23.44 * sin(2.0 * PI * (day_of_year - 81) / 365.0);
}

/* Seasonal factor: how "summery" is it at this latitude and day?
 * Returns 0.0 (deep winter) to 1.0 (peak summer).
 * Considers hemisphere: southern gets flipped. */
static float slp_seasonal_factor(double latitude_deg, int day_of_year)
{
    double decl = slp_solar_declination(day_of_year);

    /* In the northern hemisphere, positive declination = summer.
     * In the southern hemisphere, negative declination = summer.
     * Multiply by sign of latitude to align. */
    double alignment;
    if (latitude_deg >= 0.0) {
        alignment = decl;
    } else {
        alignment = -decl;
    }

    /* Normalize: -23.44 to +23.44 -> 0.0 to 1.0 */
    float factor = (float)((alignment + 23.44) / (2.0 * 23.44));
    return slp_clampf(factor, 0.0f, 1.0f);
}

/* Latitude intensity factor: how much seasonal effect does latitude have?
 * Tropics (0-23.5) = minimal. Polar (66.5-90) = extreme.
 * Returns 0.0 (equator) to 1.0 (pole). */
static float slp_latitude_seasonal_strength(double latitude_deg)
{
    double abs_lat = fabs(latitude_deg);
    if (abs_lat < 10.0) return 0.05f;
    if (abs_lat > 66.5) return 1.0f;
    /* Linear ramp from 10 to 66.5 */
    return (float)((abs_lat - 10.0) / (66.5 - 10.0));
}

/* Helper to set a color stop */
static slp_color_stop_t slp_make_stop(float altitude, float r, float g, float b)
{
    slp_color_stop_t s;
    s.altitude = altitude;
    s.r = slp_clampf(r, 0.0f, 1.0f);
    s.g = slp_clampf(g, 0.0f, 1.0f);
    s.b = slp_clampf(b, 0.0f, 1.0f);
    return s;
}

/* Apply seasonal modulation to a base color.
 * summer_shift > 0 makes colors warmer and more saturated.
 * winter_shift > 0 makes colors cooler and more desaturated. */
static void slp_apply_seasonal(float *r, float *g, float *b,
                                float seasonal_factor,
                                float seasonal_strength)
{
    /* Modulation amount: strength * distance from neutral (0.5) */
    float mod = seasonal_strength * (seasonal_factor - 0.5f) * 2.0f;

    /* Summer: slightly warmer, more saturated */
    if (mod > 0.0f) {
        *r += mod * 0.04f;
        *g += mod * 0.02f;
        *b -= mod * 0.02f;
    }
    /* Winter: slightly cooler, more desaturated */
    else {
        float abs_mod = -mod;
        float avg = (*r + *g + *b) / 3.0f;
        *r = slp_lerpf(*r, avg, abs_mod * 0.15f);
        *g = slp_lerpf(*g, avg, abs_mod * 0.15f);
        *b = slp_lerpf(*b, avg + 0.02f, abs_mod * 0.15f);
    }

    *r = slp_clampf(*r, 0.0f, 1.0f);
    *g = slp_clampf(*g, 0.0f, 1.0f);
    *b = slp_clampf(*b, 0.0f, 1.0f);
}

/* --------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------- */

slp_config_t slp_default_config(void)
{
    slp_config_t cfg;
    cfg.latitude_deg = 0.0;
    cfg.day_of_year = 80;    /* spring equinox */
    cfg.sun_elevation_deg = 70.0f;  /* near noon */
    return cfg;
}

slp_sky_t slp_sky_gradient(slp_config_t config)
{
    slp_sky_t sky;
    float elev = config.sun_elevation_deg;
    float sfactor = slp_seasonal_factor(config.latitude_deg, config.day_of_year);
    float sstrength = slp_latitude_seasonal_strength(config.latitude_deg);

    /* Determine which regime we're in based on sun elevation:
     *   Day:         elev > 6
     *   Golden hour: elev 0 to 6
     *   Blue hour:   elev -6 to 0
     *   Night:       elev < -6
     * We use smoothstep blending at boundaries. */

    float day_t = slp_smoothstep(6.0f, 12.0f, elev);    /* 0 at 6, 1 at 12 */
    float golden_t = slp_smoothstep(0.0f, 6.0f, elev);  /* 0 at 0, 1 at 6 */
    float blue_t = slp_smoothstep(-6.0f, 0.0f, elev);   /* 0 at -6, 1 at 0 */
    float night_t = 1.0f - slp_smoothstep(-12.0f, -6.0f, elev); /* 1 below -12, 0 at -6 */

    /* Base colors for each regime */
    /* Day: horizon=light blue, mid=medium blue, zenith=deep blue */
    float day_hz_r = 0.65f, day_hz_g = 0.78f, day_hz_b = 0.92f;
    float day_md_r = 0.40f, day_md_g = 0.60f, day_md_b = 0.88f;
    float day_zn_r = 0.25f, day_zn_g = 0.45f, day_zn_b = 0.85f;

    /* Golden hour: horizon=orange-gold, mid=purple-pink, zenith=deep blue-purple */
    float gld_hz_r = 0.95f, gld_hz_g = 0.65f, gld_hz_b = 0.30f;
    float gld_md_r = 0.55f, gld_md_g = 0.35f, gld_md_b = 0.55f;
    float gld_zn_r = 0.15f, gld_zn_g = 0.20f, gld_zn_b = 0.55f;

    /* Blue hour: horizon=blue+orange hint, mid=navy, zenith=deep blue */
    float blu_hz_r = 0.20f, blu_hz_g = 0.25f, blu_hz_b = 0.50f;
    float blu_md_r = 0.12f, blu_md_g = 0.15f, blu_md_b = 0.40f;
    float blu_zn_r = 0.10f, blu_zn_g = 0.12f, blu_zn_b = 0.35f;

    /* Night: horizon=slightly lighter dark, zenith=very dark blue-black */
    float nit_hz_r = 0.04f, nit_hz_g = 0.04f, nit_hz_b = 0.10f;
    float nit_md_r = 0.03f, nit_md_g = 0.03f, nit_md_b = 0.08f;
    float nit_zn_r = 0.02f, nit_zn_g = 0.02f, nit_zn_b = 0.06f;

    /* Blend between regimes using smoothstep weights.
     * Strategy: from night base, blend up through blue, golden, day. */

    /* Interpolation weights — each regime blends into the next */
    float hz_r, hz_g, hz_b;
    float md_r, md_g, md_b;
    float zn_r, zn_g, zn_b;

    if (elev >= 6.0f) {
        /* Fully day or transitioning from golden to day */
        hz_r = slp_lerpf(gld_hz_r, day_hz_r, day_t);
        hz_g = slp_lerpf(gld_hz_g, day_hz_g, day_t);
        hz_b = slp_lerpf(gld_hz_b, day_hz_b, day_t);
        md_r = slp_lerpf(gld_md_r, day_md_r, day_t);
        md_g = slp_lerpf(gld_md_g, day_md_g, day_t);
        md_b = slp_lerpf(gld_md_b, day_md_b, day_t);
        zn_r = slp_lerpf(gld_zn_r, day_zn_r, day_t);
        zn_g = slp_lerpf(gld_zn_g, day_zn_g, day_t);
        zn_b = slp_lerpf(gld_zn_b, day_zn_b, day_t);
    } else if (elev >= 0.0f) {
        /* Golden hour: blend from blue hour to golden */
        hz_r = slp_lerpf(blu_hz_r, gld_hz_r, golden_t);
        hz_g = slp_lerpf(blu_hz_g, gld_hz_g, golden_t);
        hz_b = slp_lerpf(blu_hz_b, gld_hz_b, golden_t);
        md_r = slp_lerpf(blu_md_r, gld_md_r, golden_t);
        md_g = slp_lerpf(blu_md_g, gld_md_g, golden_t);
        md_b = slp_lerpf(blu_md_b, gld_md_b, golden_t);
        zn_r = slp_lerpf(blu_zn_r, gld_zn_r, golden_t);
        zn_g = slp_lerpf(blu_zn_g, gld_zn_g, golden_t);
        zn_b = slp_lerpf(blu_zn_b, gld_zn_b, golden_t);
    } else if (elev >= -6.0f) {
        /* Blue hour: blend from night to blue */
        hz_r = slp_lerpf(nit_hz_r, blu_hz_r, blue_t);
        hz_g = slp_lerpf(nit_hz_g, blu_hz_g, blue_t);
        hz_b = slp_lerpf(nit_hz_b, blu_hz_b, blue_t);
        md_r = slp_lerpf(nit_md_r, blu_md_r, blue_t);
        md_g = slp_lerpf(nit_md_g, blu_md_g, blue_t);
        md_b = slp_lerpf(nit_md_b, blu_md_b, blue_t);
        zn_r = slp_lerpf(nit_zn_r, blu_zn_r, blue_t);
        zn_g = slp_lerpf(nit_zn_g, blu_zn_g, blue_t);
        zn_b = slp_lerpf(nit_zn_b, blu_zn_b, blue_t);
    } else {
        /* Deep night: blend to full night */
        float deep_t = night_t; /* 1.0 at -12 and below */
        hz_r = slp_lerpf(blu_hz_r, nit_hz_r, deep_t);
        hz_g = slp_lerpf(blu_hz_g, nit_hz_g, deep_t);
        hz_b = slp_lerpf(blu_hz_b, nit_hz_b, deep_t);
        md_r = slp_lerpf(blu_md_r, nit_md_r, deep_t);
        md_g = slp_lerpf(blu_md_g, nit_md_g, deep_t);
        md_b = slp_lerpf(blu_md_b, nit_md_b, deep_t);
        zn_r = slp_lerpf(blu_zn_r, nit_zn_r, deep_t);
        zn_g = slp_lerpf(blu_zn_g, nit_zn_g, deep_t);
        zn_b = slp_lerpf(blu_zn_b, nit_zn_b, deep_t);
    }

    /* Apply seasonal modulation */
    slp_apply_seasonal(&hz_r, &hz_g, &hz_b, sfactor, sstrength);
    slp_apply_seasonal(&md_r, &md_g, &md_b, sfactor, sstrength);
    slp_apply_seasonal(&zn_r, &zn_g, &zn_b, sfactor, sstrength);

    /* Build 4-stop gradient: horizon, lower-mid, upper-mid, zenith */
    sky.stop_count = 4;
    sky.stops[0] = slp_make_stop(0.00f, hz_r, hz_g, hz_b);
    sky.stops[1] = slp_make_stop(0.30f, slp_lerpf(hz_r, md_r, 0.6f),
                                         slp_lerpf(hz_g, md_g, 0.6f),
                                         slp_lerpf(hz_b, md_b, 0.6f));
    sky.stops[2] = slp_make_stop(0.65f, slp_lerpf(md_r, zn_r, 0.5f),
                                         slp_lerpf(md_g, zn_g, 0.5f),
                                         slp_lerpf(md_b, zn_b, 0.5f));
    sky.stops[3] = slp_make_stop(1.00f, zn_r, zn_g, zn_b);

    /* Ambient light: weighted average of gradient + sun-dependent intensity */
    float avg_r = (hz_r + md_r + zn_r) / 3.0f;
    float avg_g = (hz_g + md_g + zn_g) / 3.0f;
    float avg_b = (hz_b + md_b + zn_b) / 3.0f;

    /* Ambient intensity: smooth ramp based on sun elevation
     * -12 and below: 0.02
     * -6: 0.10
     *  0: 0.35
     *  6: 0.60
     * 12: 0.80
     * 30+: 0.90-1.0 */
    float intensity;
    if (elev <= -12.0f) {
        intensity = 0.02f;
    } else if (elev <= -6.0f) {
        float t = slp_smoothstep(-12.0f, -6.0f, elev);
        intensity = slp_lerpf(0.02f, 0.10f, t);
    } else if (elev <= 0.0f) {
        float t = slp_smoothstep(-6.0f, 0.0f, elev);
        intensity = slp_lerpf(0.10f, 0.35f, t);
    } else if (elev <= 6.0f) {
        float t = slp_smoothstep(0.0f, 6.0f, elev);
        intensity = slp_lerpf(0.35f, 0.60f, t);
    } else if (elev <= 30.0f) {
        float t = slp_smoothstep(6.0f, 30.0f, elev);
        intensity = slp_lerpf(0.60f, 0.90f, t);
    } else {
        float t = slp_smoothstep(30.0f, 90.0f, elev);
        intensity = slp_lerpf(0.90f, 1.00f, t);
    }

    /* Seasonal adjustment to intensity: summer slightly brighter */
    float seasonal_intensity_mod = sstrength * (sfactor - 0.5f) * 0.08f;
    intensity = slp_clampf(intensity + seasonal_intensity_mod, 0.0f, 1.0f);

    sky.ambient_r = slp_clampf(avg_r, 0.0f, 1.0f);
    sky.ambient_g = slp_clampf(avg_g, 0.0f, 1.0f);
    sky.ambient_b = slp_clampf(avg_b, 0.0f, 1.0f);
    sky.ambient_intensity = intensity;

    return sky;
}

void slp_sky_color_at(const slp_sky_t *sky, float altitude, float rgb[3])
{
    if (sky->stop_count <= 0) {
        rgb[0] = rgb[1] = rgb[2] = 0.0f;
        return;
    }

    /* Clamp altitude */
    float alt = slp_clampf(altitude, 0.0f, 1.0f);

    /* Find the two stops bracketing this altitude */
    if (alt <= sky->stops[0].altitude) {
        rgb[0] = sky->stops[0].r;
        rgb[1] = sky->stops[0].g;
        rgb[2] = sky->stops[0].b;
        return;
    }

    if (alt >= sky->stops[sky->stop_count - 1].altitude) {
        rgb[0] = sky->stops[sky->stop_count - 1].r;
        rgb[1] = sky->stops[sky->stop_count - 1].g;
        rgb[2] = sky->stops[sky->stop_count - 1].b;
        return;
    }

    for (int i = 1; i < sky->stop_count; i++) {
        if (alt <= sky->stops[i].altitude) {
            float range = sky->stops[i].altitude - sky->stops[i - 1].altitude;
            float t = (range > 0.0001f) ? (alt - sky->stops[i - 1].altitude) / range : 0.0f;
            rgb[0] = slp_lerpf(sky->stops[i - 1].r, sky->stops[i].r, t);
            rgb[1] = slp_lerpf(sky->stops[i - 1].g, sky->stops[i].g, t);
            rgb[2] = slp_lerpf(sky->stops[i - 1].b, sky->stops[i].b, t);
            return;
        }
    }

    /* Fallback: last stop */
    rgb[0] = sky->stops[sky->stop_count - 1].r;
    rgb[1] = sky->stops[sky->stop_count - 1].g;
    rgb[2] = sky->stops[sky->stop_count - 1].b;
}

void slp_ambient_light(slp_config_t config, float rgb[3], float *intensity)
{
    slp_sky_t sky = slp_sky_gradient(config);
    rgb[0] = sky.ambient_r;
    rgb[1] = sky.ambient_g;
    rgb[2] = sky.ambient_b;
    *intensity = sky.ambient_intensity;
}

void slp_sun_color(float sun_elevation_deg, float rgb[3])
{
    float elev = sun_elevation_deg;

    /* Sun color transitions:
     * > 30:   White        (1.00, 0.98, 0.92)
     * 10-30:  Warm white   (1.00, 0.92, 0.80)
     * 0-10:   Golden       (1.00, 0.75, 0.40)
     * -2 to 0: Orange      (1.00, 0.50, 0.15)
     * < -2:   Deep orange  (0.95, 0.35, 0.10) */

    if (elev >= 30.0f) {
        rgb[0] = 1.00f; rgb[1] = 0.98f; rgb[2] = 0.92f;
    } else if (elev >= 10.0f) {
        float t = slp_smoothstep(10.0f, 30.0f, elev);
        rgb[0] = 1.00f;
        rgb[1] = slp_lerpf(0.92f, 0.98f, t);
        rgb[2] = slp_lerpf(0.80f, 0.92f, t);
    } else if (elev >= 0.0f) {
        float t = slp_smoothstep(0.0f, 10.0f, elev);
        rgb[0] = 1.00f;
        rgb[1] = slp_lerpf(0.75f, 0.92f, t);
        rgb[2] = slp_lerpf(0.40f, 0.80f, t);
    } else if (elev >= -2.0f) {
        float t = slp_smoothstep(-2.0f, 0.0f, elev);
        rgb[0] = slp_lerpf(1.00f, 1.00f, t);
        rgb[1] = slp_lerpf(0.50f, 0.75f, t);
        rgb[2] = slp_lerpf(0.15f, 0.40f, t);
    } else {
        /* Below -2: clamp to deep orange, fading slightly */
        float fade = slp_smoothstep(-10.0f, -2.0f, elev);
        rgb[0] = slp_lerpf(0.80f, 1.00f, fade);
        rgb[1] = slp_lerpf(0.20f, 0.50f, fade);
        rgb[2] = slp_lerpf(0.05f, 0.15f, fade);
    }

    rgb[0] = slp_clampf(rgb[0], 0.0f, 1.0f);
    rgb[1] = slp_clampf(rgb[1], 0.0f, 1.0f);
    rgb[2] = slp_clampf(rgb[2], 0.0f, 1.0f);
}

void slp_night_sky_color(double latitude_deg, int day_of_year, float rgb[3])
{
    /* Base: deep blue-black, not pure black (space-black philosophy) */
    float base_r = 0.02f;
    float base_g = 0.02f;
    float base_b = 0.06f;

    /* Slight latitude variation: higher latitudes slightly lighter
     * (scattered light from below horizon) */
    double abs_lat = fabs(latitude_deg);
    float lat_factor = (float)(abs_lat / 90.0);
    base_r += lat_factor * 0.01f;
    base_g += lat_factor * 0.01f;
    base_b += lat_factor * 0.02f;

    /* Slight seasonal variation: summer nights slightly lighter */
    float sfactor = slp_seasonal_factor(latitude_deg, day_of_year);
    float sstrength = slp_latitude_seasonal_strength(latitude_deg);
    float seasonal_mod = sstrength * (sfactor - 0.5f) * 0.02f;
    base_r += seasonal_mod * 0.5f;
    base_g += seasonal_mod * 0.3f;
    base_b += seasonal_mod;

    rgb[0] = slp_clampf(base_r, 0.0f, 1.0f);
    rgb[1] = slp_clampf(base_g, 0.0f, 1.0f);
    rgb[2] = slp_clampf(base_b, 0.0f, 1.0f);
}

void slp_fog_color(slp_config_t config, float rgb[3])
{
    /* Fog/haze: blended version of sky horizon color, slightly desaturated.
     * At night: very dark. At day: horizon color with reduced saturation. */
    slp_sky_t sky = slp_sky_gradient(config);

    /* Use horizon color as base */
    float hr = sky.stops[0].r;
    float hg = sky.stops[0].g;
    float hb = sky.stops[0].b;

    /* Desaturate slightly for haze effect */
    float avg = (hr + hg + hb) / 3.0f;
    float desat = 0.3f; /* 30% toward gray */
    rgb[0] = slp_clampf(slp_lerpf(hr, avg, desat), 0.0f, 1.0f);
    rgb[1] = slp_clampf(slp_lerpf(hg, avg, desat), 0.0f, 1.0f);
    rgb[2] = slp_clampf(slp_lerpf(hb, avg, desat), 0.0f, 1.0f);
}

int slp_pack_gradient_strip(const slp_sky_t *sky, float *out_verts)
{
    if (sky->stop_count <= 0) return 0;

    int count = sky->stop_count * 2;
    if (count > SLP_MAX_STRIP_VERTICES) {
        count = SLP_MAX_STRIP_VERTICES;
    }

    int vert_idx = 0;
    int stops_to_pack = count / 2;
    for (int i = 0; i < stops_to_pack; i++) {
        float alt = sky->stops[i].altitude;
        float r = sky->stops[i].r;
        float g = sky->stops[i].g;
        float b = sky->stops[i].b;

        /* Left vertex: x=-1, y=altitude, color */
        int base = vert_idx * SLP_STRIP_VERTEX_FLOATS;
        out_verts[base + 0] = -1.0f;
        out_verts[base + 1] = alt;
        out_verts[base + 2] = r;
        out_verts[base + 3] = g;
        out_verts[base + 4] = b;
        vert_idx++;

        /* Right vertex: x=+1, y=altitude, color */
        base = vert_idx * SLP_STRIP_VERTEX_FLOATS;
        out_verts[base + 0] = 1.0f;
        out_verts[base + 1] = alt;
        out_verts[base + 2] = r;
        out_verts[base + 3] = g;
        out_verts[base + 4] = b;
        vert_idx++;
    }

    return vert_idx;
}

const char *slp_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision mediump float;\n"
        "\n"
        "layout(location = 0) in vec2 a_position;\n"
        "layout(location = 1) in vec3 a_color;\n"
        "\n"
        "out vec3 v_color;\n"
        "\n"
        "void main() {\n"
        "    v_color = a_color;\n"
        "    gl_Position = vec4(a_position, 0.0, 1.0);\n"
        "}\n";
}

const char *slp_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision mediump float;\n"
        "\n"
        "in vec3 v_color;\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main() {\n"
        "    fragColor = vec4(v_color, 1.0);\n"
        "}\n";
}
