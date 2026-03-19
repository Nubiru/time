/* brain_personal.c — Birth moment integration implementation.
 *
 * Computes today's values in each calendar system and compares
 * with the user's birth profile to find personal milestones.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_personal.h"
#include <string.h>
#include <stdio.h>

static const char *const PERSONAL_TYPE_NAMES[] = {
    "Kin Return",
    "Tone Match",
    "Seal Match",
    "Zodiac Match",
    "Hebrew Month",
    "Chinese Branch",
    "Hexagram Match"
};

/* ===================================================================
 * Internal: compute today's relevant values from JD
 * Uses the same computation path as birth_profile.
 * =================================================================== */

static void add_event(br_personal_result_t *out, br_personal_type_t type,
                      const char *headline, double score) {
    if (out->count >= BR_MAX_PERSONAL) return;
    br_personal_event_t *e = &out->events[out->count];
    e->type = type;
    e->score = score;
    strncpy(e->headline, headline, BR_HEADLINE_MAX - 1);
    e->headline[BR_HEADLINE_MAX - 1] = '\0';
    out->count++;
}

/* ===================================================================
 * Public API
 * =================================================================== */

int br_personal_scan(double jd, const birth_profile_t *birth,
                     br_personal_result_t *out) {
    if (!birth || !out) return 0;
    memset(out, 0, sizeof(*out));

    /* Compute today's profile using the same JD */
    birth_profile_t today = bp_compute(
        /* We need to extract Gregorian date from JD.
         * bp_compute takes year/month/day. Use a simplified JD→Gregorian. */
        0, 0, 0  /* placeholder — we'll use bp_compute differently */
    );

    /* Actually, bp_compute needs year/month/day, not JD.
     * Convert JD to Gregorian first. */
    /* JD → Gregorian (standard algorithm from julian.h concepts) */
    int z = (int)(jd + 0.5);
    int a = z;
    if (z >= 2299161) {
        int alpha = (int)((z - 1867216.25) / 36524.25);
        a = z + 1 + alpha - alpha / 4;
    }
    int b = a + 1524;
    int c = (int)((b - 122.1) / 365.25);
    int d = (int)(365.25 * c);
    int e = (int)((b - d) / 30.6001);

    int day = b - d - (int)(30.6001 * e);
    int month = (e < 14) ? e - 1 : e - 13;
    int year = (month > 2) ? c - 4716 : c - 4715;

    today = bp_compute(year, month, day);

    /* --- Check each system for matches --- */
    char buf[BR_HEADLINE_MAX];

    /* Kin Return: exact Kin match (every 260 days) */
    if (today.tzolkin.kin == birth->tzolkin.kin && today.tzolkin.kin > 0) {
        snprintf(buf, sizeof(buf), "Galactic Return: Kin %d %s %s",
                 birth->tzolkin.kin,
                 birth->tzolkin.tone_name ? birth->tzolkin.tone_name : "",
                 birth->tzolkin.seal_name ? birth->tzolkin.seal_name : "");
        add_event(out, BR_PERSONAL_KIN_RETURN, buf, 1.0);
    }

    /* Tone Match (every 13 days) */
    if (today.tzolkin.tone == birth->tzolkin.tone && today.tzolkin.tone > 0
        && today.tzolkin.kin != birth->tzolkin.kin) {
        snprintf(buf, sizeof(buf), "Your birth tone %s is active today",
                 birth->tzolkin.tone_name ? birth->tzolkin.tone_name : "");
        add_event(out, BR_PERSONAL_TONE_MATCH, buf, 0.3);
    }

    /* Seal Match (every 20 days) */
    if (today.tzolkin.seal == birth->tzolkin.seal
        && today.tzolkin.kin != birth->tzolkin.kin) {
        snprintf(buf, sizeof(buf), "Your birth seal %s is active today",
                 birth->tzolkin.seal_name ? birth->tzolkin.seal_name : "");
        add_event(out, BR_PERSONAL_SEAL_MATCH, buf, 0.4);
    }

    /* Zodiac Match: sun in birth sign */
    if (today.astrology.sun_sign == birth->astrology.sun_sign
        && today.astrology.sun_sign >= 0) {
        snprintf(buf, sizeof(buf), "Sun is in your birth sign %s",
                 birth->astrology.sun_sign_name ? birth->astrology.sun_sign_name : "");
        add_event(out, BR_PERSONAL_ZODIAC_MATCH, buf, 0.6);
    }

    /* Hebrew Month Match */
    if (today.hebrew.month == birth->hebrew.month
        && today.hebrew.month > 0) {
        snprintf(buf, sizeof(buf), "Hebrew month %s — your birth month",
                 birth->hebrew.month_name ? birth->hebrew.month_name : "");
        add_event(out, BR_PERSONAL_HEBREW_MONTH, buf, 0.4);
    }

    /* Chinese Branch Match (every 12 years) */
    if (today.chinese.branch == birth->chinese.branch
        && today.chinese.branch >= 0) {
        snprintf(buf, sizeof(buf), "Year of your birth animal: %s",
                 birth->chinese.animal_name ? birth->chinese.animal_name : "");
        add_event(out, BR_PERSONAL_CHINESE_BRANCH, buf, 0.7);
    }

    /* I Ching Hexagram Match */
    if (today.iching.king_wen == birth->iching.king_wen
        && today.iching.king_wen > 0) {
        snprintf(buf, sizeof(buf), "Your birth hexagram %s returns",
                 birth->iching.name ? birth->iching.name : "");
        add_event(out, BR_PERSONAL_HEXAGRAM_MATCH, buf, 0.5);
    }

    /* Compute overall relevance: max event score */
    out->relevance = 0.0;
    for (int i = 0; i < out->count; i++) {
        if (out->events[i].score > out->relevance) {
            out->relevance = out->events[i].score;
        }
    }

    return out->count;
}

const char *br_personal_type_name(br_personal_type_t type) {
    if (type < 0 || type >= BR_PERSONAL_TYPE_COUNT) return "?";
    return PERSONAL_TYPE_NAMES[type];
}
