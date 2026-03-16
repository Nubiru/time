/* neighbor_roulette.c — Anonymous neighbor matching for Earth Extrovert Layer.
 * Track 48.3: Profile cards, interest matching, privacy enforcement, cooldown.
 *
 * Pure functions. No malloc, no globals, no side effects. */

#include "neighbor_roulette.h"
#include "../tzolkin/tzolkin.h"
#include "../tzolkin/dreamspell.h"
#include "../astrology/zodiac.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Zodiac suffix pool for pseudonym generation */
static const char *const s_zodiac_suffixes[] = {
    "Seeker", "Explorer", "Wanderer", "Observer", "Dreamer"
};
#define NR_ZODIAC_SUFFIX_COUNT 5

/* ---- Internal helpers ---- */

/* Day boundary from JD: floor(JD) */
static double nr_day_floor(double jd) {
    return floor(jd);
}

/* Check if two JDs are on the same day */
static int nr_same_day(double jd_a, double jd_b) {
    return nr_day_floor(jd_a) == nr_day_floor(jd_b);
}

/* Fill top_interests from profile weights, up to NR_MAX_INTERESTS.
 * Picks the highest-weight interests above 0. */
static int nr_fill_top_interests(const ip_profile_t *profile, int *out, int max) {
    int used[IP_MAX_INTERESTS];
    memset(used, 0, sizeof(used));
    int count = 0;

    for (int i = 0; i < max && i < NR_MAX_INTERESTS; i++) {
        int best = -1;
        double best_w = 0.0;
        for (int j = 0; j < IP_MAX_INTERESTS; j++) {
            if (!used[j] && profile->weights[j] > best_w) {
                best_w = profile->weights[j];
                best = j;
            }
        }
        if (best < 0 || best_w <= 0.0) break;
        out[count++] = best;
        used[best] = 1;
    }
    return count;
}

/* ---- Public API ---- */

nr_state_t nr_init(void) {
    nr_state_t s;
    memset(&s, 0, sizeof(s));
    return s;
}

nr_state_t nr_set_enabled(nr_state_t state, int enabled) {
    state.enabled = enabled ? 1 : 0;
    return state;
}

/* Extract bare seal name from tzolkin_seal_name() output.
 * tzolkin_seal_name() returns "Color Seal" (e.g., "Blue Hand").
 * We skip past the first space to get just "Hand". */
static const char *nr_bare_seal_name(const char *full_name) {
    if (!full_name) return NULL;
    const char *space = full_name;
    while (*space && *space != ' ') space++;
    if (*space == ' ') return space + 1;
    return full_name;  /* no space found, return as-is */
}

int nr_pseudonym_kin(int seal, int tone, char *buf, int buf_size) {
    if (!buf || buf_size < 1) return 0;
    if (seal < 0 || seal > 19) return 0;
    if (tone < 1 || tone > 13) return 0;

    dreamspell_color_t color = dreamspell_color(seal);
    const char *tone_name = tzolkin_tone_name(tone);
    const char *full_seal = tzolkin_seal_name(seal);

    if (!color.name || !tone_name || !full_seal) return 0;
    if (color.name[0] == '?' || tone_name[0] == '?' || full_seal[0] == '?') return 0;

    const char *bare_seal = nr_bare_seal_name(full_seal);
    if (!bare_seal) return 0;

    int written = snprintf(buf, (size_t)buf_size, "%s %s %s",
                           color.name, tone_name, bare_seal);
    if (written < 0 || written >= buf_size) return 0;
    return 1;
}

int nr_pseudonym_zodiac(int sign, char *buf, int buf_size) {
    if (!buf || buf_size < 1) return 0;
    if (sign < 0 || sign > 11) return 0;

    const char *name = zodiac_sign_name(sign);
    if (!name || name[0] == '?') return 0;

    /* Pick suffix deterministically from sign index */
    const char *suffix = s_zodiac_suffixes[sign % NR_ZODIAC_SUFFIX_COUNT];

    int written = snprintf(buf, (size_t)buf_size, "%s %s", name, suffix);
    if (written < 0 || written >= buf_size) return 0;
    return 1;
}

nr_card_t nr_create_card(const ip_profile_t *profile, int kin_seal, int kin_tone,
                          int zodiac_sign, pf_consent_t consent) {
    nr_card_t card;
    memset(&card, 0, sizeof(card));
    card.kin_seal = -1;
    card.zodiac_sign = -1;

    /* Terminate interests array */
    for (int i = 0; i < NR_MAX_INTERESTS; i++) {
        card.top_interests[i] = -1;
    }

    if (!profile) {
        snprintf(card.pseudonym, NR_PSEUDONYM_MAX, "Anonymous Visitor");
        return card;
    }

    card.activity = profile->activity_level;

    if (consent == PF_CONSENT_NONE) {
        snprintf(card.pseudonym, NR_PSEUDONYM_MAX, "Anonymous Visitor");
        return card;
    }

    if (consent == PF_CONSENT_BASIC) {
        /* Zodiac pseudonym, top interests, no kin */
        card.zodiac_sign = zodiac_sign;
        if (!nr_pseudonym_zodiac(zodiac_sign, card.pseudonym, NR_PSEUDONYM_MAX)) {
            snprintf(card.pseudonym, NR_PSEUDONYM_MAX, "Anonymous Visitor");
        }
        card.top_interest_count = nr_fill_top_interests(profile,
                                                         card.top_interests,
                                                         NR_MAX_INTERESTS);
        return card;
    }

    /* PF_CONSENT_FULL */
    card.kin_seal = kin_seal;
    card.zodiac_sign = zodiac_sign;
    if (!nr_pseudonym_kin(kin_seal, kin_tone, card.pseudonym, NR_PSEUDONYM_MAX)) {
        /* Fallback to zodiac if kin fails */
        if (!nr_pseudonym_zodiac(zodiac_sign, card.pseudonym, NR_PSEUDONYM_MAX)) {
            snprintf(card.pseudonym, NR_PSEUDONYM_MAX, "Anonymous Visitor");
        }
    }
    card.top_interest_count = nr_fill_top_interests(profile,
                                                     card.top_interests,
                                                     NR_MAX_INTERESTS);
    return card;
}

nr_distance_t nr_fuzzy_distance(double distance_km) {
    if (distance_km < 5.0)   return NR_DIST_NEARBY;
    if (distance_km < 25.0)  return NR_DIST_IN_CITY;
    if (distance_km < 100.0) return NR_DIST_IN_REGION;
    return NR_DIST_FAR;
}

double nr_match_score(const ip_profile_t *a, const ip_profile_t *b) {
    if (!a || !b) return 0.0;
    ip_match_t m = ip_match(*a, *b);
    return m.score;
}

int nr_can_spin(nr_state_t state, double now_jd, int max_spins_per_day) {
    if (!state.enabled) return 0;
    /* If different day, counter resets */
    if (state.spins_today > 0 && !nr_same_day(state.day_start_jd, now_jd)) {
        return 1;
    }
    return state.spins_today < max_spins_per_day ? 1 : 0;
}

nr_state_t nr_record_spin(nr_state_t state, double now_jd) {
    /* Reset counter if new day */
    if (state.spins_today == 0 || !nr_same_day(state.day_start_jd, now_jd)) {
        state.spins_today = 0;
        state.day_start_jd = nr_day_floor(now_jd);
    }
    state.spins_today++;
    state.last_spin_jd = now_jd;
    return state;
}

double nr_cooldown_remaining(nr_state_t state, double now_jd, int max_spins_per_day) {
    if (!state.enabled) return 0.0;
    if (nr_can_spin(state, now_jd, max_spins_per_day)) return 0.0;
    /* Time until next day boundary */
    double next_day = nr_day_floor(now_jd) + 1.0;
    double remaining = next_day - now_jd;
    return remaining > 0.0 ? remaining : 0.0;
}

nr_spin_result_t nr_spin(nr_state_t state, double now_jd,
                          const nr_card_t *neighbors, int count,
                          const ip_profile_t *my_profile) {
    nr_spin_result_t result;
    memset(&result, 0, sizeof(result));

    (void)my_profile; /* reserved for future use beyond pre-computed match_score */

    if (!state.enabled) return result;

    if (!nr_can_spin(state, now_jd, NR_DEFAULT_MAX_SPINS)) {
        result.cooldown_active = 1;
        double next_day = nr_day_floor(now_jd) + 1.0;
        result.next_spin_jd = next_day;
        return result;
    }

    if (!neighbors || count <= 0) return result;

    /* Find neighbor with highest match_score */
    int best = 0;
    for (int i = 1; i < count; i++) {
        if (neighbors[i].match_score > neighbors[best].match_score) {
            best = i;
        }
    }

    result.card = neighbors[best];
    result.valid = 1;
    return result;
}

const char *nr_distance_name(nr_distance_t dist) {
    switch (dist) {
        case NR_DIST_NEARBY:    return "Nearby";
        case NR_DIST_IN_CITY:   return "In City";
        case NR_DIST_IN_REGION: return "In Region";
        case NR_DIST_FAR:       return "Far Away";
        default:                return "?";
    }
}

nr_card_t nr_anonymize(nr_card_t card, pf_consent_t consent) {
    if (consent == PF_CONSENT_NONE) {
        snprintf(card.pseudonym, NR_PSEUDONYM_MAX, "Anonymous Visitor");
        card.kin_seal = -1;
        card.zodiac_sign = -1;
        card.top_interest_count = 0;
        for (int i = 0; i < NR_MAX_INTERESTS; i++) {
            card.top_interests[i] = -1;
        }
        return card;
    }

    if (consent == PF_CONSENT_BASIC) {
        /* Strip kin, keep zodiac and interests */
        card.kin_seal = -1;
        return card;
    }

    /* PF_CONSENT_FULL: keep everything */
    return card;
}
