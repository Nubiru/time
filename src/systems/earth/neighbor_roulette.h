/* neighbor_roulette.h — Anonymous neighbor matching for Earth Extrovert Layer.
 * Track 48.3: Profile cards, interest matching, privacy enforcement, cooldown.
 *
 * Privacy-first: Never reveals exact location. Pseudonyms from Dreamspell kin
 * or zodiac sign. Off by default — user must explicitly opt in.
 *
 * Pure functions, no globals, no malloc, no side effects. */

#ifndef TIME_NEIGHBOR_ROULETTE_H
#define TIME_NEIGHBOR_ROULETTE_H

#include "interest_profile.h"
#include "privacy_framework.h"

#define NR_PSEUDONYM_MAX 64
#define NR_MAX_INTERESTS 5
#define NR_MAX_NEIGHBORS 32
#define NR_DEFAULT_MAX_SPINS 1

/* Distance granularity — never reveal exact location */
typedef enum {
    NR_DIST_NEARBY = 0,     /* < 5 km */
    NR_DIST_IN_CITY,        /* 5-25 km */
    NR_DIST_IN_REGION,      /* 25-100 km */
    NR_DIST_FAR,            /* > 100 km */
    NR_DIST_COUNT
} nr_distance_t;

/* Anonymous profile card */
typedef struct {
    char pseudonym[NR_PSEUDONYM_MAX];
    int top_interests[NR_MAX_INTERESTS];
    int top_interest_count;
    int kin_seal;               /* Dreamspell seal (0-19), -1 if not shared */
    int zodiac_sign;            /* Sun sign (0-11), -1 if not shared */
    ip_activity_t activity;
    nr_distance_t distance;
    double match_score;         /* 0.0-1.0 compatibility */
} nr_card_t;

/* Roulette state (functional — pass by value, return new value) */
typedef struct {
    double last_spin_jd;
    int spins_today;
    double day_start_jd;
    int enabled;                /* 0=off (default), 1=opted in */
} nr_state_t;

/* Spin result */
typedef struct {
    nr_card_t card;
    int valid;                  /* 1 = got a match, 0 = no match available */
    int cooldown_active;        /* 1 = can't spin yet */
    double next_spin_jd;        /* when cooldown expires (0 if not active) */
} nr_spin_result_t;

/* Initialize roulette state (disabled, no spins). */
nr_state_t nr_init(void);

/* Enable/disable roulette opt-in. */
nr_state_t nr_set_enabled(nr_state_t state, int enabled);

/* Generate a pseudonym from kin seal + tone.
 * Format: "Color Tone Seal" (Dreamspell).
 * Returns 1 on success, 0 on invalid input or buffer too small. */
int nr_pseudonym_kin(int seal, int tone, char *buf, int buf_size);

/* Generate a pseudonym from zodiac sign.
 * Format: "Sign Suffix" where suffix cycles from a fixed set.
 * Returns 1 on success, 0 on invalid input. */
int nr_pseudonym_zodiac(int sign, char *buf, int buf_size);

/* Create a profile card from a profile + kin/zodiac data.
 * Applies privacy rules based on consent level.
 * At NONE: "Anonymous Visitor", no kin/zodiac, no interests.
 * At BASIC: zodiac pseudonym, top interests, no kin.
 * At FULL: kin pseudonym, all data shown. */
nr_card_t nr_create_card(const ip_profile_t *profile, int kin_seal, int kin_tone,
                          int zodiac_sign, pf_consent_t consent);

/* Compute fuzzy distance category from exact km.
 * <5 -> NEARBY, 5-25 -> IN_CITY, 25-100 -> IN_REGION, >=100 -> FAR.
 * Negative values treated as 0. */
nr_distance_t nr_fuzzy_distance(double distance_km);

/* Compute match score between two profiles (wraps ip_match).
 * Returns 0.0-1.0. Returns 0.0 for NULL inputs. */
double nr_match_score(const ip_profile_t *a, const ip_profile_t *b);

/* Spin the roulette: pick best match from available neighbors.
 * Enforces cooldown (default 1/day). Returns invalid if disabled,
 * on cooldown, or no neighbors available. */
nr_spin_result_t nr_spin(nr_state_t state, double now_jd,
                          const nr_card_t *neighbors, int count,
                          const ip_profile_t *my_profile);

/* Record a spin (update cooldown state).
 * Resets counter if day boundary changed. */
nr_state_t nr_record_spin(nr_state_t state, double now_jd);

/* Check cooldown: can we spin now?
 * Returns 0 if disabled, or spins_today >= max_spins_per_day on same day.
 * Day boundary = floor(JD). */
int nr_can_spin(nr_state_t state, double now_jd, int max_spins_per_day);

/* Time until next spin allowed (days). 0.0 if can spin now.
 * Returns time to next day boundary if on cooldown. */
double nr_cooldown_remaining(nr_state_t state, double now_jd, int max_spins_per_day);

/* Distance label string. Returns "?" for invalid. */
const char *nr_distance_name(nr_distance_t dist);

/* Anonymize a card: strip data if consent insufficient.
 * NONE: "Anonymous Visitor", no kin/zodiac/interests.
 * BASIC: keep zodiac + interests, strip kin.
 * FULL: keep everything. */
nr_card_t nr_anonymize(nr_card_t card, pf_consent_t consent);

#endif /* TIME_NEIGHBOR_ROULETTE_H */
