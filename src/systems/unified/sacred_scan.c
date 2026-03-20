/* sacred_scan.c — Sacred number detection across calendar systems (S89).
 *
 * Extracts numeric values from each calendar system for a given JD
 * and checks which sacred numbers appear. Groups detections by number,
 * recording which systems produced each match.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "sacred_scan.h"
#include "sacred_numbers.h"
#include "../../math/julian.h"
#include "../tzolkin/tzolkin.h"
#include "../iching/iching.h"
#include "../hebrew/hebrew.h"
#include "../islamic/hijri.h"
#include "../chinese/chinese.h"
#include <string.h>
#include <math.h>

/* ===================================================================
 * Static sacred number list (subset of sacred_numbers.h registry
 * that can appear as actual calendar system values)
 * =================================================================== */

static const ss_number_t TRACKED[] = {
    {  3, "3 (Trinity)",        "Triadic division across traditions" },
    {  4, "4 (Quaternary)",     "Four elements, seasons, directions" },
    {  5, "5 (Pentad)",         "Five elements, Panchanga limbs" },
    {  7, "7 (Heptad)",         "Week cycle, planets, chakras" },
    {  8, "8 (Octad)",          "Trigrams, Eightfold Path" },
    { 10, "10 (Decad)",         "Sefirot, Heavenly Stems" },
    { 12, "12 (Dodecad)",       "Zodiac, months, tribes" },
    { 13, "13 (Trecena)",       "Tzolkin tones, lunar months" },
    { 20, "20 (Uinal)",         "Tzolkin seals, vigesimal base" },
    { 22, "22 (Paths)",         "Hebrew letters, Kabbalistic paths" },
    { 27, "27 (Nakshatras)",    "Lunar mansions" },
    { 30, "30 (Lunar Month)",   "Tithis, synodic month" },
    { 64, "64 (Hexagrams)",     "I Ching hexagrams, HD gates, DNA codons" },
};

#define TRACKED_COUNT ((int)(sizeof(TRACKED) / sizeof(TRACKED[0])))

/* ===================================================================
 * Internal: value extraction from each system
 * =================================================================== */

/* Maximum extracted values per day across all systems */
#define MAX_VALUES 24

typedef struct {
    int         value;
    const char *system_name;
} extracted_value_t;

/* Extract numeric values from calendar systems for a given JD.
 * Returns count of values extracted. */
static int extract_values(double jd, extracted_value_t *out, int max) {
    int n = 0;

    /* Gregorian: day of month and month number */
    {
        int year, month;
        double day_frac = jd_to_gregorian(jd, &year, &month);
        int day = (int)(day_frac + 0.5);
        if (day < 1) day = 1;
        if (n < max) { out[n].value = day;   out[n].system_name = "Gregorian"; n++; }
        if (n < max) { out[n].value = month; out[n].system_name = "Gregorian"; n++; }
    }

    /* Tzolkin: tone (1-13) and kin (1-260) */
    {
        tzolkin_day_t tz = tzolkin_from_jd(jd);
        if (n < max) { out[n].value = tz.tone; out[n].system_name = "Tzolkin"; n++; }
        /* Only check kin if it's a small sacred number (kin goes up to 260) */
        if (tz.kin <= 30 && n < max) {
            out[n].value = tz.kin; out[n].system_name = "Tzolkin (Kin)"; n++;
        }
    }

    /* I Ching: hexagram number (1-64) */
    {
        hexagram_t hex = iching_from_jd(jd);
        if (n < max) { out[n].value = hex.king_wen; out[n].system_name = "I Ching"; n++; }
    }

    /* Hebrew: day (1-30) and month (1-13) */
    {
        hebrew_date_t heb = hebrew_from_jd(jd);
        if (n < max) { out[n].value = heb.day;   out[n].system_name = "Hebrew"; n++; }
        if (n < max) { out[n].value = heb.month; out[n].system_name = "Hebrew"; n++; }
    }

    /* Islamic/Hijri: day (1-30) and month (1-12) */
    {
        hijri_date_t hij = hijri_from_jd(jd);
        if (n < max) { out[n].value = hij.day;   out[n].system_name = "Islamic"; n++; }
        if (n < max) { out[n].value = hij.month; out[n].system_name = "Islamic"; n++; }
    }

    /* Chinese: cycle_year (1-60), stem+1 (1-10), branch+1 (1-12) */
    {
        chinese_year_t cy = chinese_year_from_jd(jd);
        if (n < max) { out[n].value = cy.cycle_year;  out[n].system_name = "Chinese"; n++; }
        if (n < max) { out[n].value = cy.stem + 1;    out[n].system_name = "Chinese"; n++; }
        if (n < max) { out[n].value = cy.branch + 1;  out[n].system_name = "Chinese"; n++; }
    }

    return n;
}

/* ===================================================================
 * Public API
 * =================================================================== */

int ss_get_numbers(ss_number_t *out, int max) {
    if (!out || max <= 0) return 0;
    int count = TRACKED_COUNT;
    if (count > max) count = max;
    for (int i = 0; i < count; i++) {
        out[i] = TRACKED[i];
    }
    return count;
}

const ss_number_t *ss_is_sacred(int value) {
    if (value <= 0) return NULL;
    for (int i = 0; i < TRACKED_COUNT; i++) {
        if (TRACKED[i].value == value) return &TRACKED[i];
    }
    return NULL;
}

int ss_scan(double jd, ss_result_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->jd = jd;

    /* Extract all numeric values from calendar systems */
    extracted_value_t values[MAX_VALUES];
    int val_count = extract_values(jd, values, MAX_VALUES);

    /* For each extracted value, check if it's a sacred number.
     * Group by sacred number, collecting system names. */
    for (int v = 0; v < val_count; v++) {
        const ss_number_t *sacred = ss_is_sacred(values[v].value);
        if (!sacred) continue;

        /* Find or create detection for this sacred number */
        int det_idx = -1;
        for (int d = 0; d < out->count; d++) {
            if (out->detections[d].number == sacred->value) {
                det_idx = d;
                break;
            }
        }

        if (det_idx < 0) {
            if (out->count >= SS_MAX_DETECTIONS) continue;
            det_idx = out->count;
            out->detections[det_idx].number = sacred->value;
            out->detections[det_idx].system_count = 0;
            out->count++;
        }

        /* Add this system to the detection (avoid duplicates) */
        ss_detection_t *det = &out->detections[det_idx];
        int already = 0;
        for (int s = 0; s < det->system_count; s++) {
            if (det->systems[s] == values[v].system_name) {
                already = 1;
                break;
            }
        }
        if (!already && det->system_count < SS_MAX_SYSTEMS) {
            det->systems[det->system_count] = values[v].system_name;
            det->system_count++;
        }
    }

    return out->count;
}

int ss_year_count(int year, int min_detections) {
    if (min_detections <= 0) return 365;

    double jan1 = gregorian_to_jd(year, 1, 1.0);
    int days_with = 0;

    for (int d = 0; d < 365; d++) {
        ss_result_t r;
        int count = ss_scan(jan1 + (double)d, &r);
        if (count >= min_detections) {
            days_with++;
        }
    }

    return days_with;
}
