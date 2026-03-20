/* today_for_you.h — Daily briefing card composer.
 *
 * Integrates pre-computed personal data (cycle_tracker, personal_resonance,
 * brain_personal) into a single card-ready daily briefing struct. The caller
 * pre-computes the raw data; this module FORMATS it for display.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_TODAY_FOR_YOU_H
#define TIME_TODAY_FOR_YOU_H

#define TFY_GREETING_MAX    128
#define TFY_HEADLINE_MAX    256
#define TFY_BODY_MAX        512
#define TFY_MAX_HIGHLIGHTS  6
#define TFY_MAX_CYCLES      5
#define TFY_LABEL_MAX       48
#define TFY_VALUE_MAX       96

/* A cycle status line for the briefing */
typedef struct {
    char label[TFY_LABEL_MAX];     /* "Wavespell", "Zodiac Transit", etc. */
    char value[TFY_VALUE_MAX];     /* "Red Dragon — 3 days left" */
    double progress;                /* 0.0-1.0 through cycle */
    int days_remaining;
    int transition_soon;            /* 1 if within 3 days of change */
} tfy_cycle_t;

/* A highlight card (resonance, milestone, or event) */
typedef struct {
    char text[TFY_HEADLINE_MAX];   /* "Today's Kin shares your seal" */
    int strength;                   /* 1-5 */
    int type;                       /* 0=resonance, 1=milestone, 2=cycle_event */
} tfy_highlight_t;

/* Energy level for the day */
typedef enum {
    TFY_ENERGY_QUIET = 0,          /* No personal resonances today */
    TFY_ENERGY_GENTLE,              /* 1 weak resonance */
    TFY_ENERGY_ACTIVE,              /* Multiple resonances or moderate strength */
    TFY_ENERGY_INTENSE,             /* Strong resonance or milestone */
    TFY_ENERGY_EXTRAORDINARY,       /* Kin return or rare convergence */
    TFY_ENERGY_COUNT
} tfy_energy_t;

/* Input: pre-computed personal data */
typedef struct {
    /* Date info */
    int year, month, day;
    int hour;                       /* 0-23, for greeting (morning/afternoon/evening) */

    /* Cycle data (from cycle_tracker) */
    int days_alive;
    int age_years;
    int days_until_birthday;

    /* Cycle summaries (simplified from ct_tracker_t) */
    struct {
        char name[TFY_LABEL_MAX];
        char value[TFY_VALUE_MAX];
        double progress;
        int days_remaining;
        int cycle_length;
    } cycles[TFY_MAX_CYCLES];
    int cycle_count;

    /* Resonances (from personal_resonance) */
    struct {
        char description[256];
        int strength;               /* 1-5 */
    } resonances[TFY_MAX_HIGHLIGHTS];
    int resonance_count;
    int strongest_resonance;        /* max strength, 0 if none */

    /* Milestones (from brain_personal) */
    struct {
        char headline[128];
        double score;               /* 0.0-1.0 */
    } milestones[4];
    int milestone_count;
} tfy_input_t;

/* Output: the daily briefing card */
typedef struct {
    char greeting[TFY_GREETING_MAX];                /* "Good morning" */
    char headline[TFY_HEADLINE_MAX];                /* Primary daily message */
    tfy_highlight_t highlights[TFY_MAX_HIGHLIGHTS]; /* Ordered by strength desc */
    int highlight_count;
    tfy_cycle_t cycles[TFY_MAX_CYCLES];             /* Cycle status lines */
    int cycle_count;
    tfy_energy_t energy;                             /* Overall energy level */
    int days_until_birthday;
    int show_birthday_countdown;                     /* 1 if <= 30 days */
} tfy_briefing_t;

/* Create default (zeroed) input. */
tfy_input_t tfy_default_input(void);

/* Compose daily briefing from pre-computed input. NULL input returns empty briefing. */
tfy_briefing_t tfy_compose(const tfy_input_t *input);

/* Get greeting text for a time of day (0-23 hours).
 * "Good morning" (5-11), "Good afternoon" (12-17), "Good evening" (18-4).
 * Out-of-range hours return "Good evening". */
const char *tfy_greeting_for_hour(int hour);

/* Get energy level name. Returns "?" for invalid. */
const char *tfy_energy_name(tfy_energy_t energy);

/* Get highlight count from briefing. */
int tfy_highlight_count(const tfy_briefing_t *b);

/* Get cycle count from briefing. */
int tfy_cycle_count(const tfy_briefing_t *b);

/* Format briefing as multi-line text. Returns bytes written (excluding null),
 * or 0 on error/zero buffer. */
int tfy_format(const tfy_briefing_t *b, char *buf, int buf_size);

#endif /* TIME_TODAY_FOR_YOU_H */
