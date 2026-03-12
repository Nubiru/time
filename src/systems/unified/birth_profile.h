/* birth_profile.h -- Birth Profile Computer
 *
 * Computes a multi-system birth profile for a given Gregorian date.
 * Aggregates Tzolkin (Dreamspell), Chinese, Hebrew, Islamic, Buddhist,
 * I Ching, Western astrology, and Human Design data into a single struct.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BIRTH_PROFILE_H
#define TIME_BIRTH_PROFILE_H

#include <stdbool.h>

typedef struct {
    int kin;                    /* 1-260 */
    int tone;                   /* 1-13 */
    int seal;                   /* 0-19 */
    const char *seal_name;
    const char *tone_name;
    int guide_kin;
    int analog_kin;
    int antipode_kin;
    int occult_kin;
    int wavespell;
    int castle;
} bp_tzolkin_t;

typedef struct {
    int animal;
    int element;
    int stem;
    int branch;
    int polarity;
    const char *animal_name;
    const char *element_name;
} bp_chinese_t;

typedef struct {
    int year;
    int month;
    int day;
    const char *month_name;
} bp_hebrew_t;

typedef struct {
    int year;
    int month;
    int day;
    const char *month_name;
} bp_islamic_t;

typedef struct {
    int era_year;
} bp_buddhist_t;

typedef struct {
    int king_wen;
    const char *name;
} bp_iching_t;

typedef struct {
    int sun_sign;
    const char *sun_sign_name;
    double sun_longitude;
    int hd_sun_gate;
    int hd_earth_gate;
    const char *hd_sun_gate_name;
} bp_astrology_t;

typedef struct {
    int year;
    int month;
    int day;
    double jd;
    bool has_location;
    double latitude;
    double longitude;
    bp_tzolkin_t tzolkin;
    bp_chinese_t chinese;
    bp_hebrew_t hebrew;
    bp_islamic_t islamic;
    bp_buddhist_t buddhist;
    bp_iching_t iching;
    bp_astrology_t astrology;
    double earth_age_ratio;
    int day_of_year;
    const char *western_zodiac;
} birth_profile_t;

/* Compute a birth profile for a Gregorian date (no location). */
birth_profile_t bp_compute(int year, int month, int day);

/* Compute a birth profile with geographic location. */
birth_profile_t bp_compute_with_location(int year, int month, int day,
                                          double latitude, double longitude);

/* Format a one-line summary for a specific system.
 * System names (case-insensitive): "tzolkin", "chinese", "hebrew",
 * "islamic", "buddhist", "iching", "astrology".
 * Returns NULL for unknown system or NULL arguments.
 * Uses a static buffer -- not thread-safe. */
const char *bp_summary(const birth_profile_t *profile, const char *system);

/* Number of supported systems. */
int bp_system_count(void);

/* Name of system by index (0-based). Returns NULL for invalid index. */
const char *bp_system_name(int index);

#endif /* TIME_BIRTH_PROFILE_H */
