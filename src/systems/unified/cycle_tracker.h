/* cycle_tracker.h — Personal cycle status from birth data + current date.
 *
 * Given a birth_profile_t and current Julian Day, computes the status of
 * personal cycles: Tzolkin wavespell, castle, 260-day round, zodiac
 * transit, and personal year. Each cycle reports position, days elapsed,
 * days remaining, and progress.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CYCLE_TRACKER_H
#define TIME_CYCLE_TRACKER_H

#include "birth_profile.h"

#define CT_MAX_CYCLES  8
#define CT_NAME_MAX    32
#define CT_VALUE_MAX   64

/* Cycle identifiers. */
typedef enum {
    CT_WAVESPELL = 0,   /* 13-day Tzolkin wavespell */
    CT_CASTLE,          /* 52-day Tzolkin castle */
    CT_TZOLKIN_ROUND,   /* 260-day Tzolkin cycle */
    CT_ZODIAC_TRANSIT,  /* ~30-day zodiac sign transit */
    CT_PERSONAL_YEAR,   /* 365/366-day birthday cycle */
    CT_CYCLE_COUNT
} ct_cycle_id_t;

/* Status of a single cycle. */
typedef struct {
    ct_cycle_id_t id;
    char name[CT_NAME_MAX];     /* "Wavespell", "Castle", etc. */
    char value[CT_VALUE_MAX];   /* Current value: "Red Dragon", "Gemini", etc. */
    int days_elapsed;           /* Days since cycle started */
    int days_remaining;         /* Days until next transition */
    int cycle_length;           /* Total cycle length in days */
    double progress;            /* 0.0-1.0 through current cycle */
} ct_cycle_t;

/* Full tracker result. */
typedef struct {
    ct_cycle_t cycles[CT_CYCLE_COUNT];
    int count;
    int days_alive;             /* Total days since birth */
    int age_years;              /* Approximate age in years */
    int days_until_birthday;    /* Days until next birthday (1-366) */
} ct_tracker_t;

/* Compute cycle tracker for a birth profile at a given JD. */
ct_tracker_t ct_compute(const birth_profile_t *profile, double current_jd);

/* Get a specific cycle by ID. Returns NULL if out of range. */
const ct_cycle_t *ct_cycle(const ct_tracker_t *tracker, ct_cycle_id_t id);

/* Get cycle name for an ID. */
const char *ct_cycle_name(ct_cycle_id_t id);

/* Check if any cycle is within N days of transition. */
int ct_any_transition_soon(const ct_tracker_t *tracker, int within_days);

/* Count cycles that are within N days of transition. */
int ct_transitions_soon(const ct_tracker_t *tracker, int within_days);

#endif /* TIME_CYCLE_TRACKER_H */
