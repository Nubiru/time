/* tzolkin_interpret.h — Dreamspell interpretation engine (Arguelles system)
 *
 * Deep archetypal meaning for any kin: seal archetype, tone purpose,
 * color meaning, wavespell journey, castle narrative, oracle relationships,
 * and daily energy. Composes data from dreamspell.h into narrative text.
 *
 * Pure functions: no globals, no malloc, no side effects.
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 */

#ifndef TIME_TZOLKIN_INTERPRET_H
#define TIME_TZOLKIN_INTERPRET_H

/* Per-seal archetype (20 seals) */
typedef struct {
    int seal;                   /* 0-19 */
    const char *archetype;      /* The seal's archetypal meaning */
    const char *question;       /* The question this seal asks */
    const char *power;          /* The seal's power/gift */
    const char *action;         /* The seal's action/verb */
    const char *essence;        /* One-word essence */
} ti_seal_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: "KIN 207" */
    char glance[128];           /* T1: "Kin 207 Blue Crystal Hand — Dedicate Accomplishment" */
    char detail[1024];          /* T3: Full narrative — seal + tone + oracle + wavespell + castle */
} tzolkin_interp_t;

/* Return archetype data for a seal (0-19).
 * Invalid: returns struct with seal=-1, all strings "?". */
ti_seal_t ti_seal_data(int seal);

/* Compose tiered interpretation for a kin.
 * kin: 1-260, seal: 0-19, tone: 1-13.
 * Invalid input: fills with "?" strings. */
tzolkin_interp_t ti_interpret(int kin, int seal, int tone);

/* Returns 20 (total number of solar seals). */
int ti_seal_count(void);

/* Color meaning string for color index (0=Red, 1=White, 2=Blue, 3=Yellow).
 * Invalid: returns "?". */
const char *ti_color_meaning(int color);

/* Tone meaning string for tone number (1-13).
 * Invalid: returns "?". */
const char *ti_tone_meaning(int tone);

#endif /* TIME_TZOLKIN_INTERPRET_H */
