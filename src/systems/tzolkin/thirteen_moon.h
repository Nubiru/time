#ifndef TIME_THIRTEEN_MOON_H
#define TIME_THIRTEEN_MOON_H

/* Arguelles 13-Moon/28-Day Calendar (Dreamspell "New Time").
 *
 * Divides the solar year into 13 months of exactly 28 days (364 days)
 * plus 1 "Day Out of Time" (July 25 Gregorian). Year starts July 26.
 *
 * Each month (moon) is named after a galactic tone.
 * Each day is associated with one of 7 radial plasmas.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define TM_MONTH_COUNT 13
#define TM_MONTH_DAYS 28
#define TM_YEAR_DAYS 365   /* 364 + Day Out of Time */
#define TM_HEPTAD_DAYS 7

typedef struct {
    int moon;           /* 1-13 (month number) */
    int day;            /* 1-28 (day within month) */
    int heptad;         /* 1-4 (which week of the month) */
    int day_of_week;    /* 1-7 (day within heptad) */
    int plasma;         /* 0-6 (Dali=0, Seli=1, Gamma=2, Kali=3, Alpha=4, Limi=5, Silio=6) */
    int day_of_year;    /* 0-364 (0-363 = regular days, 364 = Day Out of Time) */
    int is_day_out;     /* 1 if Day Out of Time (July 25), 0 otherwise */
    int year;           /* 13-Moon year number (starts July 26 Gregorian) */
} thirteen_moon_t;

/* Compute 13-Moon date from Julian Day.
 * The 13-Moon year starts on July 26 (Gregorian).
 * Day Out of Time = July 25 = day before new year.
 *
 * Returns zeroed struct for invalid JD (< 0). */
thirteen_moon_t thirteen_moon_from_jd(double jd);

/* Moon name (1-13). Each moon is named after a galactic tone.
 * "Magnetic", "Lunar", "Electric", "Self-Existing", "Overtone",
 * "Rhythmic", "Resonant", "Galactic", "Solar", "Planetary",
 * "Spectral", "Crystal", "Cosmic".
 * Returns "?" for invalid moon. */
const char *thirteen_moon_name(int moon);

/* Plasma name (0-6).
 * "Dali", "Seli", "Gamma", "Kali", "Alpha", "Limi", "Silio".
 * Returns "?" for invalid plasma. */
const char *thirteen_moon_plasma_name(int plasma);

/* Plasma chakra association (0-6).
 * "Crown", "Root", "Third Eye", "Sacral", "Throat", "Solar Plexus", "Heart".
 * Returns "?" for invalid plasma. */
const char *thirteen_moon_plasma_chakra(int plasma);

/* Plasma action word (0-6).
 * "Target", "Flow", "Pacify", "Establish", "Release", "Purify", "Discharge".
 * Returns "?" for invalid plasma. */
const char *thirteen_moon_plasma_action(int plasma);

#endif /* TIME_THIRTEEN_MOON_H */
