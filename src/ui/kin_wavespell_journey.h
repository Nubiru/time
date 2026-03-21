#ifndef TIME_KIN_WAVESPELL_JOURNEY_H
#define TIME_KIN_WAVESPELL_JOURNEY_H

/* Wavespell Journey Diagram — 13-tone arc from Magnetic to Cosmic.
 *
 * Shows the 13 tones as a journey arc with tone name, action, and power
 * keywords at each step. Today's position is highlighted on the journey.
 *
 * Gate tones: 1 (Magnetic), 5 (Overtone), 9 (Solar), 13 (Cosmic).
 * Phases: Initiate (1-4), Extend (5-8), Convert (9-12), Complete (13).
 *
 * Source: Arguelles, "Dreamspell" (1990).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_JOURNEY_STEPS 13

typedef struct {
    int tone;               /* 1-13 */
    const char *name;       /* "Magnetic", "Lunar", etc. */
    const char *action;     /* "Unify", "Polarize", etc. */
    const char *power;      /* "Purpose", "Challenge", etc. */
    float x, y;             /* position on journey arc */
    int highlighted;        /* 1 = today's tone */
    int is_gate;            /* 1 = gate tone (1, 5, 9, 13) */
} kin_journey_step_t;

typedef struct {
    kin_journey_step_t steps[KIN_JOURNEY_STEPS];
    int today_tone;         /* 1-13 */
    int phase;              /* 0=Initiate(1-4), 1=Extend(5-8), 2=Convert(9-12), 3=Complete(13) */
} kin_wavespell_journey_t;

/* Compute the wavespell journey diagram for a tone (1-13).
 * Steps arranged in an arc:
 *   x = 0.05 + (step / 12.0) * 0.90    (left to right, 0.05 to 0.95)
 *   y = 0.5 - 0.3 * sin(step/12.0 * PI) (arc peaking at center)
 * Gate tones: 1 (Magnetic), 5 (Overtone), 9 (Solar), 13 (Cosmic).
 * Phase: tones 1-4 = Initiate(0), 5-8 = Extend(1), 9-12 = Convert(2), 13 = Complete(3).
 * Returns zeroed struct for invalid tone. */
kin_wavespell_journey_t kin_journey_compute(int today_tone);

/* Get the journey phase name.
 * 0="Initiate", 1="Extend", 2="Convert", 3="Complete". "?" for invalid. */
const char *kin_journey_phase_name(int phase);

#endif /* TIME_KIN_WAVESPELL_JOURNEY_H */
