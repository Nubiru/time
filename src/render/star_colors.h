#ifndef TIME_STAR_COLORS_H
#define TIME_STAR_COLORS_H

/* Spectral type classification */
typedef enum {
    SPECTRAL_O = 0,
    SPECTRAL_B,
    SPECTRAL_A,
    SPECTRAL_F,
    SPECTRAL_G,
    SPECTRAL_K,
    SPECTRAL_M,
    SPECTRAL_COUNT
} spectral_type_t;

/* Star color in linear RGB (0-1) */
typedef struct {
    float r, g, b;
} star_rgb_t;

/* Color temperature to linear RGB using Tanner Helland / Mitchell Charity
 * approximation of Planck blackbody radiation.
 * temp_kelvin: effective temperature (1000-40000 K range). */
star_rgb_t star_color_from_temperature(float temp_kelvin);

/* B-V color index to effective temperature (Kelvin).
 * Uses Ballesteros (2012) formula:
 *   T = 4600 * (1/(0.92*BV + 1.7) + 1/(0.92*BV + 0.62))
 * bv: B-V index (typically -0.4 to +2.0). */
float star_bv_to_temperature(float bv);

/* B-V color index to linear RGB (convenience: bv -> temp -> rgb).
 * This is the primary function for star rendering. */
star_rgb_t star_color_from_bv(float bv);

/* Spectral type to typical effective temperature.
 * Returns midpoint of spectral class range.
 * Returns 0 for invalid type. */
float spectral_type_temperature(spectral_type_t type);

/* Spectral type to typical B-V index.
 * Returns 0 for invalid type. */
float spectral_type_bv(spectral_type_t type);

/* Spectral type name string ("O", "B", "A", ..., "M").
 * Returns "?" for invalid type. */
const char *spectral_type_name(spectral_type_t type);

/* Spectral type from B-V index (approximate classification). */
spectral_type_t spectral_type_from_bv(float bv);

/* Spectral type from temperature (approximate classification). */
spectral_type_t spectral_type_from_temperature(float temp_kelvin);

/* Verify known star colors against expected appearance.
 * Returns the RGB for a named reference star.
 * Known stars: "Rigel", "Sirius", "Procyon", "Sun", "Arcturus", "Betelgeuse".
 * Unknown name returns white (1,1,1). */
star_rgb_t star_reference_color(const char *name);

#endif /* TIME_STAR_COLORS_H */
