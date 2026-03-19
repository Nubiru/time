/* brain_personal.h — Birth moment integration for brain insights (Track 26, L2.1)
 *
 * Compares today's calendar values with a user's birth profile to find
 * personal milestones: Kin return (Galactic Birthday), zodiac match,
 * Hebrew month match, Chinese element match, I Ching hexagram return.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_PERSONAL_H
#define TIME_BRAIN_PERSONAL_H

#include "brain_types.h"
#include "birth_profile.h"

/* Maximum personal insights per scan */
#define BR_MAX_PERSONAL 8

/* Personal event types */
typedef enum {
    BR_PERSONAL_KIN_RETURN = 0,  /* today's Kin = birth Kin (every 260 days) */
    BR_PERSONAL_TONE_MATCH,      /* today's tone = birth tone (every 13 days) */
    BR_PERSONAL_SEAL_MATCH,      /* today's seal = birth seal (every 20 days) */
    BR_PERSONAL_ZODIAC_MATCH,    /* sun is in birth sign (annual, ~30 days) */
    BR_PERSONAL_HEBREW_MONTH,    /* Hebrew month = birth Hebrew month (annual) */
    BR_PERSONAL_CHINESE_BRANCH,  /* Chinese branch = birth branch (every 12 years) */
    BR_PERSONAL_HEXAGRAM_MATCH,  /* I Ching hexagram = birth hexagram */
    BR_PERSONAL_TYPE_COUNT
} br_personal_type_t;

/* A single personal milestone */
typedef struct {
    br_personal_type_t  type;
    char                headline[BR_HEADLINE_MAX];
    double              score;    /* 0.0 (weak match) to 1.0 (exact return) */
} br_personal_event_t;

/* Result of personal scan */
typedef struct {
    br_personal_event_t events[BR_MAX_PERSONAL];
    int                 count;
    double              relevance;  /* overall personal relevance 0.0-1.0 */
} br_personal_result_t;

/* Scan today's date against a birth profile for personal milestones.
 * Returns number of personal events found.
 * out must point to valid br_personal_result_t (zeroed internally). */
int br_personal_scan(double jd, const birth_profile_t *birth,
                     br_personal_result_t *out);

/* Get the personal event type name. "?" for invalid. */
const char *br_personal_type_name(br_personal_type_t type);

#endif /* TIME_BRAIN_PERSONAL_H */
