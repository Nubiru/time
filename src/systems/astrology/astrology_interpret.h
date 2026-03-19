/* astrology_interpret.h — Zodiac sign interpretation data
 *
 * Deep interpretation for 12 zodiac signs: archetype, keywords, shadow,
 * ruler, element, modality, brief meaning. Plus element and modality
 * lookups, and a tiered composition function for Sun/Moon/Ascendant.
 *
 * Jungian archetypal framework — NOT predictive horoscopes.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_ASTROLOGY_INTERPRET_H
#define TIME_ASTROLOGY_INTERPRET_H

/* Per-sign archetype */
typedef struct {
    int sign;                   /* 0-11 (Aries-Pisces) */
    const char *name;           /* "Aries", "Taurus", etc. */
    const char *element;        /* "Fire", "Earth", "Air", "Water" */
    const char *modality;       /* "Cardinal", "Fixed", "Mutable" */
    const char *ruler;          /* Ruling planet name */
    const char *archetype;      /* Jungian archetype */
    const char *keywords;       /* 3 key traits */
    const char *shadow;         /* The sign's shadow/challenge */
    const char *brief;          /* One-sentence meaning */
} ai_sign_t;

/* Element interpretation */
typedef struct {
    const char *name;           /* "Fire", "Earth", "Air", "Water" */
    const char *quality;        /* Essential quality */
    const char *expression;     /* How it expresses */
} ai_element_t;

/* Modality interpretation */
typedef struct {
    const char *name;
    const char *quality;
    const char *role;           /* Role in the cycle */
} ai_modality_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: zodiac abbreviation */
    char glance[128];           /* T1: Sun/Moon/Ascendant one-liner */
    char detail[1024];          /* T3: Full narrative */
} astrology_interp_t;

/* Return archetype data for a zodiac sign (0-11).
 * Invalid: sign=-1, all strings "?". */
ai_sign_t ai_sign_data(int sign);

/* Return element interpretation (0=Fire, 1=Earth, 2=Air, 3=Water).
 * Invalid: name="?". */
ai_element_t ai_element_data(int element);

/* Return modality interpretation (0=Cardinal, 1=Fixed, 2=Mutable).
 * Invalid: name="?". */
ai_modality_t ai_modality_data(int modality);

/* Compose tiered interpretation from Sun/Moon/Ascendant signs (0-11).
 * asc_sign: 0-11 or -1 if unknown.
 * Invalid sun/moon: "?" strings. */
astrology_interp_t ai_interpret(int sun_sign, int moon_sign, int asc_sign);

/* Returns 12 (total zodiac signs). */
int ai_sign_count(void);

/* Returns 4 (total elements). */
int ai_element_count(void);

/* Returns 3 (total modalities). */
int ai_modality_count(void);

#endif /* TIME_ASTROLOGY_INTERPRET_H */
