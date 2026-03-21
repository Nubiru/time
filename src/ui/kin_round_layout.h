#ifndef TIME_KIN_ROUND_LAYOUT_H
#define TIME_KIN_ROUND_LAYOUT_H

/* Calendar Round wheel layout — 52-petal flower.
 *
 * Visualizes the 52-year Calendar Round (18,980 days).
 * Each petal represents one Haab year with its year bearer.
 * Current year's petal is highlighted.
 *
 * Year bearers rotate through 4 Dreamspell seals:
 *   Seed(3), Moon(8), Wizard(13), Storm(18).
 *
 * Angles: 0 = top, clockwise.
 * Positions: normalized unit square, center at (0.5, 0.5).
 * Petal radius: 0.4.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_ROUND_PETALS 52

typedef struct {
    float angle;         /* degrees, 0=top, CW */
    float x, y;          /* normalized position (center = 0.5, 0.5) */
    int year_in_round;   /* 0-51 */
    int bearer_seal;     /* year bearer seal (0-19) */
    int bearer_tone;     /* year bearer tone (1-13) */
    int color;           /* directional color of bearer seal (0-3) */
    int highlighted;     /* 1 = current year */
} kin_round_petal_t;

typedef struct {
    kin_round_petal_t petals[KIN_ROUND_PETALS]; /* 52 petals */
    int current_year;          /* 0-51: which petal is today's year */
    int days_remaining;        /* days until next Calendar Round */
    float progress;            /* 0.0-1.0 progress through current round */
} kin_round_layout_t;

/* Compute the Calendar Round wheel for a Julian Day.
 * 52 petals at radius 0.4 from center (0.5, 0.5).
 * Angle: petal * (360/52) degrees.
 * Each petal has the year bearer seal and tone.
 * Current year highlighted.
 * Returns zeroed struct for invalid JD (< 0). */
kin_round_layout_t kin_round_compute(double jd);

/* Get the year bearer seal for a specific year in the round.
 * year_in_round: 0-51.
 * Dreamspell year bearers: Seed(3), Moon(8), Wizard(13), Storm(18).
 * Pattern: bearers[year % 4].
 * Returns -1 for invalid year. */
int kin_round_year_bearer(int year_in_round);

#endif /* TIME_KIN_ROUND_LAYOUT_H */
