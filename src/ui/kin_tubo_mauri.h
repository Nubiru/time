#ifndef TIME_KIN_TUBO_MAURI_H
#define TIME_KIN_TUBO_MAURI_H

/* Tubo Mauri — Vertical spine through the Telektonon board.
 *
 * 7 heptad gate positions arranged as a vertical channel with plasma,
 * chakra, mantra, and consciousness labels. Shows the daily gate
 * activation based on the current day of the heptad (week).
 *
 * Gate layout (vertical column, centered at x=0.5):
 *   gate[0] (day 1, Dali/Crown):  y=0.05 (top)
 *   gate[6] (day 7, Silio/Heart): y=0.95 (bottom)
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 *         (Boletin Intergalactico No 7).
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_TUBO_GATES 7

typedef struct {
    int day_of_week;          /* 1-7 */
    const char *plasma;       /* "DALI", "SELI", etc. */
    const char *chakra;       /* "Crown", "Root", etc. */
    const char *mantra;       /* "OM", "HRAM", etc. */
    const char *consciousness;/* "Pre-conscious", etc. */
    const char *mental_sphere;/* "1st Sphere", etc. */
    int hk_number;            /* Hunab Ku 21 number */
    int highlighted;          /* 1 if today's gate */
    float x, y;               /* position (vertical column) */
} kin_tubo_gate_t;

typedef struct {
    kin_tubo_gate_t gates[KIN_TUBO_GATES]; /* 7 gates, top to bottom */
    int today_gate;           /* 0-6: which gate is active today (-1 if invalid) */
    int today_day_of_week;    /* 1-7 */
} kin_tubo_mauri_t;

/* Compute the Tubo Mauri channel for a Julian Day.
 * 7 gates arranged vertically:
 *   gate[0] (day 1, Dali/Crown): top, y=0.05
 *   gate[6] (day 7, Silio/Heart): bottom, y=0.95
 *   All at x=0.5 (centered)
 * Today's gate is highlighted based on thirteen_moon day_of_week.
 * Returns zeroed struct for invalid JD / Day Out of Time. */
kin_tubo_mauri_t kin_tubo_compute(double jd);

#endif /* TIME_KIN_TUBO_MAURI_H */
