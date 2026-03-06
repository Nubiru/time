#ifndef TIME_TZOLKIN_H
#define TIME_TZOLKIN_H

/* Tzolkin day result (Dreamspell/Arguelles system). */
typedef struct {
    int kin;   /* 1-260 */
    int tone;  /* 1-13 */
    int seal;  /* 0-19 (solar seal index) */
} tzolkin_day_t;

/* Compute Tzolkin day from Julian Day Number.
 * Uses GMT correlation 584283 (Dreamspell count). */
tzolkin_day_t tzolkin_from_jd(double jd);

/* Solar seal name from index (0-19).
 * Returns "?" for invalid index. */
const char *tzolkin_seal_name(int seal);

/* Galactic tone name from tone number (1-13).
 * Returns "?" for invalid tone. */
const char *tzolkin_tone_name(int tone);

#endif
