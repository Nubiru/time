#ifndef TIME_KIN_PULSAR_LAYOUT_H
#define TIME_KIN_PULSAR_LAYOUT_H

/* Kin Pulsar Layout — 4 pulsar groups in the 13-tone wavespell.
 *
 * Each pulsar connects specific tone positions across the wavespell,
 * representing dimensional resonance in the Dreamspell system:
 *
 *   Pulsar 0 "Time"          (1st dimension): tones 1, 5, 9, 13
 *   Pulsar 1 "Sense"         (2nd dimension): tones 2, 6, 10
 *   Pulsar 2 "Mind"          (3rd dimension): tones 3, 7, 11
 *   Pulsar 3 "Self-Existing" (4th dimension): tones 4, 8, 12
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_PULSAR_COUNT 4
#define KIN_PULSAR_MAX_TONES 4

typedef struct {
    const char *name;                      /* "Time", "Sense", "Mind", "Self-Existing" */
    const char *dimension;                 /* "1st", "2nd", "3rd", "4th" */
    int tones[KIN_PULSAR_MAX_TONES];       /* tone numbers in this pulsar */
    int tone_count;                        /* number of tones (3 or 4) */
    int contains_today;                    /* 1 if today's tone is in this pulsar */
    float color[4];                        /* RGBA for the pulsar's connection lines */
} kin_pulsar_t;

typedef struct {
    kin_pulsar_t pulsars[KIN_PULSAR_COUNT];
    int today_tone;                        /* 1-13 */
    int today_pulsar;                      /* 0-3: which pulsar today belongs to */
} kin_pulsar_layout_t;

/* Compute pulsar diagram for a tone (1-13).
 * The 4 pulsars connect these tones:
 *   Pulsar 0 "Time" (1st): tones 1, 5, 9, 13 -> 4 tones (gold color)
 *   Pulsar 1 "Sense" (2nd): tones 2, 6, 10    -> 3 tones (red color)
 *   Pulsar 2 "Mind" (3rd): tones 3, 7, 11     -> 3 tones (blue color)
 *   Pulsar 3 "Self-Existing" (4th): tones 4, 8, 12 -> 3 tones (green color)
 * Returns zeroed struct for invalid tone. */
kin_pulsar_layout_t kin_pulsar_compute(int today_tone);

/* Get which pulsar a tone belongs to (0-3).
 * Tone 1,5,9,13 -> 0; tone 2,6,10 -> 1; tone 3,7,11 -> 2; tone 4,8,12 -> 3.
 * Returns -1 for invalid tone. */
int kin_pulsar_for_tone(int tone);

#endif /* TIME_KIN_PULSAR_LAYOUT_H */
