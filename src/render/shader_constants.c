/*
 * shader_constants.c — GLSL #define preamble with project constants
 *
 * Centralizes all magic numbers used in GLSL shaders into named constants.
 * The preamble string is suitable for insertion into any shader source
 * after the #version and precision lines.
 *
 * Values sourced from:
 *   - Mathematics: PI, TAU, PHI (golden ratio)
 *   - Brand: Solar gold, Celestial teal, Space-black (CLAUDE.md / theme.c)
 *   - Rendering: Bloom threshold (post_process.c), ambient level
 *   - Luminance: ITU-R BT.709 (Rec. 709) coefficients
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "shader_constants.h"

#include <string.h>

#define PI  3.14159265358979323846
#define TAU 6.28318530717958647692
#define PHI 1.6180339887498948482
#define PHI_INV 0.6180339887498948482

/* Constant table entry */
typedef struct {
    const char *name;
    float value;
} shader_const_entry_t;

static const shader_const_entry_t s_constants[] = {
    /* Mathematical */
    { "PI",              3.14159265f },
    { "TAU",             6.28318530f },
    { "PHI",             1.61803398f },
    { "PHI_INV",         0.61803398f },

    /* Rendering */
    { "BLOOM_THRESHOLD", 0.80f },
    { "AMBIENT_LEVEL",   0.02f },

    /* Brand: Solar gold {1.0, 0.85, 0.55} */
    { "SOLAR_GOLD_R",    1.00f },
    { "SOLAR_GOLD_G",    0.85f },
    { "SOLAR_GOLD_B",    0.55f },

    /* Brand: Celestial teal {0.2, 0.75, 0.8} */
    { "CELESTIAL_TEAL_R", 0.20f },
    { "CELESTIAL_TEAL_G", 0.75f },
    { "CELESTIAL_TEAL_B", 0.80f },

    /* Space-black: #060709 -> normalized sRGB */
    { "SPACE_BLACK_R",   0.024f },
    { "SPACE_BLACK_G",   0.027f },
    { "SPACE_BLACK_B",   0.036f },

    /* ITU-R BT.709 luminance coefficients */
    { "LUMINANCE_R",     0.2126f },
    { "LUMINANCE_G",     0.7152f },
    { "LUMINANCE_B",     0.0722f },
};

#define CONST_COUNT ((int)(sizeof(s_constants) / sizeof(s_constants[0])))

/* GLSL preamble: #define NAME VALUE for each constant */
static const char *s_preamble =
    "/* --- Shader constants (auto-generated preamble) --- */\n"
    "#define PI              3.14159265\n"
    "#define TAU             6.28318530\n"
    "#define PHI             1.61803398\n"
    "#define PHI_INV         0.61803398\n"
    "#define BLOOM_THRESHOLD 0.80\n"
    "#define AMBIENT_LEVEL   0.02\n"
    "#define SOLAR_GOLD_R    1.00\n"
    "#define SOLAR_GOLD_G    0.85\n"
    "#define SOLAR_GOLD_B    0.55\n"
    "#define CELESTIAL_TEAL_R 0.20\n"
    "#define CELESTIAL_TEAL_G 0.75\n"
    "#define CELESTIAL_TEAL_B 0.80\n"
    "#define SPACE_BLACK_R   0.024\n"
    "#define SPACE_BLACK_G   0.027\n"
    "#define SPACE_BLACK_B   0.036\n"
    "#define LUMINANCE_R     0.2126\n"
    "#define LUMINANCE_G     0.7152\n"
    "#define LUMINANCE_B     0.0722\n"
    "\n";

const char *shader_constants_preamble(void)
{
    return s_preamble;
}

float shader_constant_value(const char *name)
{
    if (!name) return 0.0f;

    for (int i = 0; i < CONST_COUNT; i++) {
        if (strcmp(name, s_constants[i].name) == 0) {
            return s_constants[i].value;
        }
    }

    return 0.0f;
}

int shader_constants_count(void)
{
    return CONST_COUNT;
}
