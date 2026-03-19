/* personal_resonance.h — Cross-system echo detector for birth profiles.
 *
 * Detects resonances between a user's birth profile and the current moment
 * across Tzolkin, I Ching, Hebrew, Chinese, and Astrology systems.
 * Finds connections like "your birth kin shares today's seal" or
 * "your Hebrew birth month is this month."
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * All inputs are pre-computed by the caller. */

#ifndef TIME_PERSONAL_RESONANCE_H
#define TIME_PERSONAL_RESONANCE_H

#define PR_MAX_ECHOES 12

/* Resonance type categories */
typedef enum {
    PR_TYPE_SEAL_MATCH = 0,     /* Same Tzolkin seal (1 in 20) */
    PR_TYPE_TONE_MATCH,         /* Same Tzolkin tone (1 in 13) */
    PR_TYPE_KIN_MATCH,          /* Exact kin match (1 in 260!) */
    PR_TYPE_ORACLE_MATCH,       /* Today's kin is in birth oracle */
    PR_TYPE_WAVESPELL_MATCH,    /* Same wavespell */
    PR_TYPE_HEXAGRAM_MATCH,     /* Same I Ching hexagram (1 in 64) */
    PR_TYPE_HEBREW_MONTH_MATCH, /* Same Hebrew month */
    PR_TYPE_CHINESE_MATCH,      /* Same Chinese animal or element */
    PR_TYPE_ZODIAC_MATCH,       /* Same zodiac sign */
    PR_TYPE_COUNT
} pr_type_t;

/* A single detected resonance */
typedef struct {
    pr_type_t type;
    const char *system;         /* "Tzolkin", "I Ching", "Hebrew", etc. */
    char description[256];      /* Human-readable description */
    int strength;               /* 1-5 (5 = rarest/strongest) */
} pr_echo_t;

/* Input: birth + current moment (pre-computed) */
typedef struct {
    /* Birth data */
    int birth_kin;              /* 1-260, 0 = unknown */
    int birth_seal;             /* 0-19, -1 = unknown */
    int birth_tone;             /* 1-13, 0 = unknown */
    int birth_guide_kin;        /* oracle partner, 0 = unknown */
    int birth_analog_kin;
    int birth_antipode_kin;
    int birth_occult_kin;
    int birth_wavespell;        /* 1-20, 0 = unknown */
    int birth_hexagram;         /* 1-64, 0 = unknown */
    int birth_hebrew_month;     /* 1-13, 0 = unknown */
    int birth_chinese_animal;   /* 0-11, -1 = unknown */
    int birth_chinese_element;  /* 0-4, -1 = unknown */
    int birth_sun_sign;         /* 0-11, -1 = unknown */

    /* Current moment */
    int today_kin;              /* 1-260 */
    int today_seal;             /* 0-19 */
    int today_tone;             /* 1-13 */
    int today_wavespell;        /* 1-20 */
    int today_hexagram;         /* 1-64 */
    int today_hebrew_month;     /* 1-13 */
    int today_chinese_animal;   /* 0-11 */
    int today_chinese_element;  /* 0-4 */
    int today_sun_sign;         /* 0-11 */
} pr_input_t;

/* Output: all detected resonances */
typedef struct {
    pr_echo_t echoes[PR_MAX_ECHOES];
    int count;
    int strongest;              /* strength of strongest echo (0 if none) */
} pr_result_t;

/* Detect all resonances between birth profile and current moment.
 * Scans all 9 resonance types, skips types where birth data is unknown,
 * sorts results by strength descending, sets strongest to max.
 * NULL input returns empty result. */
pr_result_t pr_detect(const pr_input_t *input);

/* Return a default input with all birth fields set to unknown (0 or -1). */
pr_input_t pr_default_input(void);

/* Return human-readable name for a resonance type.
 * Returns "?" for invalid/out-of-range values. */
const char *pr_type_name(pr_type_t type);

/* Return the number of resonance types (PR_TYPE_COUNT). */
int pr_type_count(void);

#endif /* TIME_PERSONAL_RESONANCE_H */
