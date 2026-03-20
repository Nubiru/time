/* sacred_scan.h — Sacred number detection across calendar systems (Track 27, S89)
 *
 * Detects when significant numbers appear as day positions or cycle
 * counts across multiple calendar systems simultaneously.
 *
 * Sacred numbers: 7 (days/planets/chakras), 12 (zodiac/months/tribes),
 * 13 (Tzolkin tones/lunar months), 22 (Hebrew letters/Tarot),
 * 40 (Biblical significance), 64 (hexagrams/DNA codons).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_SACRED_SCAN_H
#define TIME_SACRED_SCAN_H

/* Maximum sacred numbers tracked */
#define SS_MAX_NUMBERS 16

/* Maximum systems that can share a number */
#define SS_MAX_SYSTEMS 8

/* Maximum detections per scan */
#define SS_MAX_DETECTIONS 16

/* A sacred number definition */
typedef struct {
    int         value;
    const char *name;           /* "7 (completion)" */
    const char *significance;   /* why this number matters */
} ss_number_t;

/* A detection: a sacred number appearing in multiple systems */
typedef struct {
    int         number;         /* the sacred number value */
    int         system_count;   /* how many systems show this number */
    const char *systems[SS_MAX_SYSTEMS]; /* system names */
} ss_detection_t;

/* Result of a sacred number scan */
typedef struct {
    ss_detection_t  detections[SS_MAX_DETECTIONS];
    int             count;
    double          jd;
} ss_result_t;

/* Get the list of tracked sacred numbers.
 * Returns count. out must hold SS_MAX_NUMBERS entries. */
int ss_get_numbers(ss_number_t *out, int max);

/* Check if a value is a sacred number.
 * Returns pointer to static ss_number_t, or NULL. */
const ss_number_t *ss_is_sacred(int value);

/* Scan a Julian Day for sacred number appearances across systems.
 * Checks day numbers in Gregorian, Hebrew, Islamic, Chinese cycle position,
 * Tzolkin tone (1-13), I Ching hexagram number (1-64).
 * Returns number of detections. */
int ss_scan(double jd, ss_result_t *out);

/* Scan a year and count days with N+ sacred number detections.
 * Returns total detection-days found. */
int ss_year_count(int year, int min_detections);

#endif /* TIME_SACRED_SCAN_H */
