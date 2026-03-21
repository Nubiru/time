/* daily_narrative.h — Personalized daily digest composer.
 *
 * Composes a "what today means" narrative from pre-computed calendar data,
 * convergence information, birth profile resonances, and wisdom quotes.
 * Turns raw system data into a layered story across knowledge systems.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * All inputs are pre-computed by the caller (main loop or UI layer). */

#ifndef TIME_DAILY_NARRATIVE_H
#define TIME_DAILY_NARRATIVE_H

#include "../../ui/i18n.h"
#include <stdio.h>

#define DN_MAX_SYSTEMS  8
#define DN_MAX_INSIGHTS 6

/* A single system insight for the daily digest */
typedef struct {
    int system_id;              /* ts_system_t value */
    const char *system_name;    /* "Hebrew", "Tzolkin", etc. */
    char summary[128];          /* "5786 Tevet 15 — Month of transformation" */
    int significance;           /* 0-3 */
} dn_insight_t;

/* Birth resonance — personal connection to today */
typedef struct {
    char description[256];      /* "Your birth kin shares the Hand seal..." */
    int strength;               /* 1-5 (5 = strongest resonance) */
} dn_resonance_t;

/* Input: pre-computed daily data */
typedef struct {
    /* Today's date info */
    int gregorian_year;
    int gregorian_month;
    int gregorian_day;
    const char *weekday;        /* "Wednesday" etc., NULL if unknown */

    /* System insights (pre-computed, top N most significant) */
    dn_insight_t insights[DN_MAX_SYSTEMS];
    int insight_count;          /* 0 to DN_MAX_SYSTEMS */

    /* Convergence */
    int convergence_score;      /* 0-10, from today_summary */
    const char *convergence_desc; /* "3 lunar traditions align" or NULL */

    /* Birth profile (optional) */
    int has_birth;              /* 1 if birth data provided */
    int birth_kin;              /* Tzolkin kin 1-260, 0 if unknown */
    const char *birth_seal;     /* "Hand", "Eagle", etc., NULL if unknown */
    int birth_tone;             /* 1-13, 0 if unknown */
    int birth_hexagram;         /* 1-64, 0 if unknown */
    int birth_hebrew_month;     /* 1-13, 0 if unknown */

    /* Today's Tzolkin (for resonance detection) */
    int today_kin;              /* 1-260 */
    int today_seal;             /* 0-19 */
    int today_tone;             /* 1-13 */
    int today_hexagram;         /* 1-64 */
    int today_hebrew_month;     /* 1-13 */

    /* Wisdom quote (optional, pre-selected) */
    const char *wisdom_quote;   /* NULL if none */
    const char *wisdom_author;  /* NULL if none */

    /* Locale for translated output (0 = EN default via memset) */
    i18n_locale_t locale;
} dn_input_t;

/* Output: the daily narrative */
typedef struct {
    char headline[256];         /* Main headline */
    char personal[512];         /* Personal section: birth resonances */
    char systems[1024];         /* Top system insights formatted */
    char wisdom[256];           /* Wisdom quote section */
    int insight_count;          /* How many system insights included */
    int resonance_count;        /* How many personal resonances found */
    int convergence_score;      /* Echoed from input */
} daily_narrative_t;

/* Compose a daily narrative from pre-computed input.
 * If input is NULL, returns empty narrative. */
daily_narrative_t dn_compose(const dn_input_t *input);

/* Return a zeroed default input struct (all 0/NULL). */
dn_input_t dn_default_input(void);

/* Determine headline type from a composed narrative.
 * Returns: 0 = date-based, 1 = significance-based, 2 = convergence-based.
 * Returns 0 for NULL. */
int dn_headline_type(const daily_narrative_t *narrative);

#endif /* TIME_DAILY_NARRATIVE_H */
