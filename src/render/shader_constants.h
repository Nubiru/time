/*
 * shader_constants.h — GLSL #define preamble with project constants
 *
 * Provides a GLSL preamble string containing named constants derived
 * from project conventions: mathematical constants, brand colors,
 * rendering parameters, and luminance coefficients.
 *
 * Insert the preamble AFTER #version and precision lines, BEFORE
 * function definitions in any shader that needs these values.
 *
 * Replaces magic numbers scattered across shader source strings.
 *
 * Pure module: no GL calls, no malloc, no globals.
 */

#ifndef TIME_SHADER_CONSTANTS_H
#define TIME_SHADER_CONSTANTS_H

/* Get a GLSL #define preamble with all shader constants.
 * Returns a null-terminated string of #define lines.
 * Insert this AFTER #version and precision, BEFORE function definitions.
 * Constants include:
 *   PI, TAU, PHI, PHI_INV
 *   BLOOM_THRESHOLD, AMBIENT_LEVEL
 *   SOLAR_GOLD_R/G/B, CELESTIAL_TEAL_R/G/B
 *   SPACE_BLACK_R/G/B
 *   LUMINANCE_R/G/B (BT.709 coefficients)
 */
const char *shader_constants_preamble(void);

/* Get the value of a named constant as a float.
 * Returns 0.0 if name not found. */
float shader_constant_value(const char *name);

/* Get count of defined constants. */
int shader_constants_count(void);

#endif /* TIME_SHADER_CONSTANTS_H */
