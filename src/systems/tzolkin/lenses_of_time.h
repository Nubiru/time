#ifndef TIME_LENSES_OF_TIME_H
#define TIME_LENSES_OF_TIME_H

/* Lenses of Time — Synchronotron system.
 *
 * The 13 galactic tones are grouped into 4 Lenses of Time, each a
 * template frequency used to compute the daily Tonal Value.
 * Tonal Value = tone_number x lens_value.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 */

#define LENS_OF_TIME_COUNT 4

typedef struct {
    int number;               /* 1-4 */
    int value;                /* 108, 144, 216, 288 */
    const char *name;         /* "Cosmic Standard", etc. */
    const char *template_name;/* "Stellar Mind", etc. */
    const char *pulsar;       /* "Time Pulsar", etc. */
    int dimension;            /* 1-4 (dimension associated) */
} lens_of_time_t;

/* Get the lens for a galactic tone (1-13).
 * Returns zeroed struct for invalid tone. */
lens_of_time_t lens_for_tone(int tone);

/* Number of lenses (always 4). */
int lens_count(void);

/* Get a lens by its number (1-4).
 * Returns zeroed struct for invalid number. */
lens_of_time_t lens_by_number(int lens_num);

/* Tonal value for a tone: tone * lens_value.
 * Returns 0 for invalid tone. */
int lens_tonal_value(int tone);

/* Lens value for a tone (108/144/216/288).
 * Returns 0 for invalid tone. */
int lens_value_for_tone(int tone);

#endif /* TIME_LENSES_OF_TIME_H */
