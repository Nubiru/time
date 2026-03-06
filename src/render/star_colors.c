/*
 * star_colors.c — B-V color index to RGB via Planck blackbody physics
 *
 * Temperature formula: Ballesteros (2012)
 *   "New insights into the HR diagram"
 *   T = 4600 * (1/(0.92*BV + 1.7) + 1/(0.92*BV + 0.62))
 *
 * Color temperature to RGB: Tanner Helland approximation
 *   (based on Mitchell Charity's blackbody color data)
 *
 * Pure module: no malloc, no globals, no side effects.
 */

#include "star_colors.h"

#include <math.h>
#include <string.h>

/* ----- Static spectral data tables ----- */

static const float SPECTRAL_TEMPS[SPECTRAL_COUNT] = {
    33000.0f, /* O */
    20000.0f, /* B */
     9500.0f, /* A */
     7200.0f, /* F */
     5900.0f, /* G */
     4400.0f, /* K */
     3200.0f  /* M */
};

static const float SPECTRAL_BV[SPECTRAL_COUNT] = {
    -0.33f, /* O */
    -0.17f, /* B */
     0.00f, /* A */
     0.35f, /* F */
     0.65f, /* G */
     1.00f, /* K */
     1.50f  /* M */
};

static const char *SPECTRAL_NAMES[SPECTRAL_COUNT] = {
    "O", "B", "A", "F", "G", "K", "M"
};

/* Reference star B-V values */
typedef struct {
    const char *name;
    float bv;
} reference_star_t;

static const reference_star_t REFERENCE_STARS[] = {
    {"Rigel",      -0.03f},
    {"Sirius",      0.00f},
    {"Procyon",     0.42f},
    {"Sun",         0.65f},
    {"Arcturus",    1.23f},
    {"Betelgeuse",  1.85f}
};

static const int REFERENCE_STAR_COUNT =
    (int)(sizeof(REFERENCE_STARS) / sizeof(REFERENCE_STARS[0]));

/* ----- Helper: clamp float to [0, 1] ----- */

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* ----- Public functions ----- */

float star_bv_to_temperature(float bv)
{
    /* Ballesteros (2012) formula */
    float term1 = 1.0f / (0.92f * bv + 1.7f);
    float term2 = 1.0f / (0.92f * bv + 0.62f);
    return 4600.0f * (term1 + term2);
}

star_rgb_t star_color_from_temperature(float temp_kelvin)
{
    /*
     * Tanner Helland / Mitchell Charity approximation.
     * Input: temperature in Kelvin (1000-40000).
     * Output: linear RGB in [0,1].
     */
    star_rgb_t c;
    float t = temp_kelvin / 100.0f;
    float r, g, b;

    /* Red */
    if (temp_kelvin < 6600.0f) {
        r = 255.0f;
    } else {
        r = 329.698727446f * powf(t - 60.0f, -0.1332047592f);
    }

    /* Green */
    if (temp_kelvin < 6600.0f) {
        g = 99.4708025861f * logf(t) - 161.1195681661f;
    } else {
        g = 288.1221695283f * powf(t - 60.0f, -0.0755148492f);
    }

    /* Blue */
    if (temp_kelvin >= 6600.0f) {
        b = 255.0f;
    } else if (temp_kelvin <= 1900.0f) {
        b = 0.0f;
    } else {
        b = 138.5177312231f * logf(t - 10.0f) - 305.0447927307f;
    }

    /* Clamp to 0-255 then normalize to 0-1 */
    c.r = clampf(r, 0.0f, 255.0f) / 255.0f;
    c.g = clampf(g, 0.0f, 255.0f) / 255.0f;
    c.b = clampf(b, 0.0f, 255.0f) / 255.0f;

    return c;
}

star_rgb_t star_color_from_bv(float bv)
{
    float temp = star_bv_to_temperature(bv);
    return star_color_from_temperature(temp);
}

float spectral_type_temperature(spectral_type_t type)
{
    if (type < 0 || type >= SPECTRAL_COUNT) {
        return 0.0f;
    }
    return SPECTRAL_TEMPS[type];
}

float spectral_type_bv(spectral_type_t type)
{
    if (type < 0 || type >= SPECTRAL_COUNT) {
        return 0.0f;
    }
    return SPECTRAL_BV[type];
}

const char *spectral_type_name(spectral_type_t type)
{
    if (type < 0 || type >= SPECTRAL_COUNT) {
        return "?";
    }
    return SPECTRAL_NAMES[type];
}

spectral_type_t spectral_type_from_bv(float bv)
{
    /*
     * Classify by finding the closest B-V boundary midpoint.
     * Boundaries between types are midpoints of adjacent B-V values.
     */
    if (bv < (SPECTRAL_BV[SPECTRAL_O] + SPECTRAL_BV[SPECTRAL_B]) / 2.0f) {
        return SPECTRAL_O;
    }
    if (bv < (SPECTRAL_BV[SPECTRAL_B] + SPECTRAL_BV[SPECTRAL_A]) / 2.0f) {
        return SPECTRAL_B;
    }
    if (bv < (SPECTRAL_BV[SPECTRAL_A] + SPECTRAL_BV[SPECTRAL_F]) / 2.0f) {
        return SPECTRAL_A;
    }
    if (bv < (SPECTRAL_BV[SPECTRAL_F] + SPECTRAL_BV[SPECTRAL_G]) / 2.0f) {
        return SPECTRAL_F;
    }
    if (bv < (SPECTRAL_BV[SPECTRAL_G] + SPECTRAL_BV[SPECTRAL_K]) / 2.0f) {
        return SPECTRAL_G;
    }
    if (bv < (SPECTRAL_BV[SPECTRAL_K] + SPECTRAL_BV[SPECTRAL_M]) / 2.0f) {
        return SPECTRAL_K;
    }
    return SPECTRAL_M;
}

spectral_type_t spectral_type_from_temperature(float temp_kelvin)
{
    /*
     * Classify by finding the closest temperature boundary midpoint.
     * Temperatures decrease O -> M, so check from hottest down.
     */
    if (temp_kelvin > (SPECTRAL_TEMPS[SPECTRAL_O] + SPECTRAL_TEMPS[SPECTRAL_B]) / 2.0f) {
        return SPECTRAL_O;
    }
    if (temp_kelvin > (SPECTRAL_TEMPS[SPECTRAL_B] + SPECTRAL_TEMPS[SPECTRAL_A]) / 2.0f) {
        return SPECTRAL_B;
    }
    if (temp_kelvin > (SPECTRAL_TEMPS[SPECTRAL_A] + SPECTRAL_TEMPS[SPECTRAL_F]) / 2.0f) {
        return SPECTRAL_A;
    }
    if (temp_kelvin > (SPECTRAL_TEMPS[SPECTRAL_F] + SPECTRAL_TEMPS[SPECTRAL_G]) / 2.0f) {
        return SPECTRAL_F;
    }
    if (temp_kelvin > (SPECTRAL_TEMPS[SPECTRAL_G] + SPECTRAL_TEMPS[SPECTRAL_K]) / 2.0f) {
        return SPECTRAL_G;
    }
    if (temp_kelvin > (SPECTRAL_TEMPS[SPECTRAL_K] + SPECTRAL_TEMPS[SPECTRAL_M]) / 2.0f) {
        return SPECTRAL_K;
    }
    return SPECTRAL_M;
}

star_rgb_t star_reference_color(const char *name)
{
    star_rgb_t white = {1.0f, 1.0f, 1.0f};

    if (!name) {
        return white;
    }

    for (int i = 0; i < REFERENCE_STAR_COUNT; i++) {
        if (strcmp(name, REFERENCE_STARS[i].name) == 0) {
            return star_color_from_bv(REFERENCE_STARS[i].bv);
        }
    }

    return white;
}
