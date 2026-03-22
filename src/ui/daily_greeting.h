/* daily_greeting.h — First text a user sees when entering Time.
 *
 * Composes a locale-aware greeting from time-of-day, date context,
 * Kin/Tzolkin identity, and system convergence information.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Content keys used (create separately in content data):
 *   greeting.dawn        — "The light returns"
 *   greeting.morning     — "Good morning"
 *   greeting.midday      — "Good afternoon"
 *   greeting.afternoon   — "Good afternoon"
 *   greeting.evening     — "Good evening"
 *   greeting.night       — "Good night"
 *   greeting.teaser      — "%d systems converge today"
 *   greeting.teaser.none — "The cycles continue in quiet rhythm"
 */

#ifndef TIME_DAILY_GREETING_H
#define TIME_DAILY_GREETING_H

#include "i18n.h"

/* Time-of-day periods */
typedef enum {
    DG_PERIOD_DAWN = 0,      /* 5:00-7:59  */
    DG_PERIOD_MORNING,        /* 8:00-11:59 */
    DG_PERIOD_MIDDAY,         /* 12:00-13:59 */
    DG_PERIOD_AFTERNOON,      /* 14:00-17:59 */
    DG_PERIOD_EVENING,        /* 18:00-20:59 */
    DG_PERIOD_NIGHT,          /* 21:00-4:59  */
    DG_PERIOD_COUNT
} dg_period_t;

/* Input context for greeting composition */
typedef struct {
    int hour;                 /* 0-23 local time */
    int minute;               /* 0-59 */
    const char *weekday;      /* "Monday" etc, NULL ok */
    const char *date_str;     /* "March 22, 2026" etc, NULL ok */
    const char *kin_str;      /* "Kin 187 Blue Cosmic Hand", NULL ok */
    const char *convergence;  /* "3 systems converge", NULL ok */
    int convergence_count;    /* 0 = none, 1+ = how many systems */
    i18n_locale_t locale;     /* 0 = EN */
} dg_input_t;

/* Output greeting fields */
typedef struct {
    char greeting[64];        /* "Good morning" / "Buenos dias" */
    char summary[256];        /* "Monday, March 22. Kin 187 Blue Cosmic Hand." */
    char teaser[128];         /* "3 systems converge today." or "" if none */
    dg_period_t period;       /* which time period */
} dg_output_t;

/* Determine time period from hour (0-23).
 * Out-of-range hours map to DG_PERIOD_NIGHT. */
dg_period_t dg_period(int hour);

/* Compose a daily greeting from input context.
 * Fills all fields of out. Returns 1 on success, 0 on error
 * (NULL input or NULL out). */
int dg_compose(const dg_input_t *input, dg_output_t *out);

#endif /* TIME_DAILY_GREETING_H */
