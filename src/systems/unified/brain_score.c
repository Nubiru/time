/* brain_score.c — Confidence and rarity scoring implementation.
 *
 * Static database of known system-pair relationships, derived from
 * structural_map concordance data and calendar mathematics.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_score.h"
#include "convergence_detect.h"
#include <stddef.h>

/* ===================================================================
 * Known system-pair relationship database
 *
 * Sources: structural_map.h concordance, Calendrical Calculations,
 * Sefer Yetzirah, Crowley 777, Ra Uru Hu (Human Design).
 * =================================================================== */

static const br_known_pair_t KNOWN_PAIRS[] = {
    /* STRUCTURAL — exact mathematical equivalence */
    { CD_SYS_ICHING,    CD_SYS_CHINESE,   BR_CORR_STRUCTURAL, 1.0,
      "I Ching trigrams map to 8 directions in Chinese compass" },
    { CD_SYS_ASTRONOMY, CD_SYS_ASTROLOGY, BR_CORR_STRUCTURAL, 1.0,
      "Same sky, different interpretation framework" },

    /* HARMONIC — shared cycle lengths or simple ratios */
    { CD_SYS_HEBREW,    CD_SYS_ISLAMIC,   BR_CORR_HARMONIC,   0.95,
      "Both lunar calendars tracking the same synodic month" },
    { CD_SYS_HEBREW,    CD_SYS_BUDDHIST,  BR_CORR_HARMONIC,   0.90,
      "Both use lunar month boundaries for sacred observance" },
    { CD_SYS_ISLAMIC,   CD_SYS_BUDDHIST,  BR_CORR_HARMONIC,   0.90,
      "Both pure lunar calendars with month-start at new moon" },
    { CD_SYS_HEBREW,    CD_SYS_CHINESE,   BR_CORR_HARMONIC,   0.85,
      "Both lunisolar: lunar months corrected to solar year" },
    { CD_SYS_HEBREW,    CD_SYS_HINDU,     BR_CORR_HARMONIC,   0.85,
      "Both lunisolar with intercalary month correction" },
    { CD_SYS_CHINESE,   CD_SYS_HINDU,     BR_CORR_HARMONIC,   0.85,
      "Both lunisolar calendars with similar intercalation logic" },
    { CD_SYS_CHINESE,   CD_SYS_KOREAN,    BR_CORR_HARMONIC,   0.95,
      "Korean calendar derived from Chinese astronomical methods" },
    { CD_SYS_CHINESE,   CD_SYS_JAPANESE,  BR_CORR_HARMONIC,   0.90,
      "Japanese calendar historically adopted from Chinese system" },
    { CD_SYS_CHINESE,   CD_SYS_THAI,      BR_CORR_HARMONIC,   0.85,
      "Thai calendar shares lunisolar structure with Chinese" },
    { CD_SYS_HINDU,     CD_SYS_BUDDHIST,  BR_CORR_HARMONIC,   0.90,
      "Buddhist calendar derived from Hindu Surya Siddhanta" },
    { CD_SYS_HINDU,     CD_SYS_THAI,      BR_CORR_HARMONIC,   0.85,
      "Thai calendar uses Hindu astronomical constants" },
    { CD_SYS_HINDU,     CD_SYS_MYANMAR,   BR_CORR_HARMONIC,   0.85,
      "Myanmar calendar based on Surya Siddhanta parameters" },
    { CD_SYS_COPTIC,    CD_SYS_PERSIAN,   BR_CORR_HARMONIC,   0.80,
      "Both solar calendars with 12x30+5 day structure" },
    { CD_SYS_ASTRONOMY, CD_SYS_CELTIC,    BR_CORR_HARMONIC,   0.85,
      "Celtic Sabbats align with solstices, equinoxes, cross-quarters" },
    { CD_SYS_ASTRONOMY, CD_SYS_HEBREW,    BR_CORR_HARMONIC,   0.80,
      "Hebrew months start at new moon — direct lunar observation" },
    { CD_SYS_ASTRONOMY, CD_SYS_ISLAMIC,   BR_CORR_HARMONIC,   0.80,
      "Islamic months start at crescent moon sighting" },

    /* APPROXIMATE — cultural parallels, independent development */
    { CD_SYS_TZOLKIN,   CD_SYS_HEBREW,    BR_CORR_APPROXIMATE, 0.50,
      "Independent calendar traditions; 260 and Metonic cycle unrelated" },
    { CD_SYS_TZOLKIN,   CD_SYS_ISLAMIC,   BR_CORR_APPROXIMATE, 0.40,
      "Mesoamerican and Arabian calendar traditions developed independently" },
    { CD_SYS_TZOLKIN,   CD_SYS_HINDU,     BR_CORR_APPROXIMATE, 0.50,
      "Both encode long cycles; possible precession awareness in both" },
    { CD_SYS_TZOLKIN,   CD_SYS_CHINESE,   BR_CORR_APPROXIMATE, 0.45,
      "Both use cyclic counting (260-day vs 60-year sexagenary)" },
    { CD_SYS_ICHING,    CD_SYS_HEBREW,    BR_CORR_APPROXIMATE, 0.40,
      "Chinese divination and Hebrew calendar — unrelated traditions" },
    { CD_SYS_CELTIC,    CD_SYS_TZOLKIN,   BR_CORR_APPROXIMATE, 0.35,
      "Pre-Christian European and Mesoamerican — no known contact" },
    { CD_SYS_ZOROASTRIAN, CD_SYS_PERSIAN, BR_CORR_HARMONIC,   0.90,
      "Zoroastrian calendar is the pre-Islamic Persian calendar" },
    { CD_SYS_BAHAI,     CD_SYS_PERSIAN,   BR_CORR_HARMONIC,   0.80,
      "Bahai calendar starts at Naw-Ruz (Persian New Year)" },
};

static const int KNOWN_PAIR_COUNT =
    (int)(sizeof(KNOWN_PAIRS) / sizeof(KNOWN_PAIRS[0]));

/* ===================================================================
 * Public API
 * =================================================================== */

const br_known_pair_t *br_lookup_pair(int system_a, int system_b) {
    for (int i = 0; i < KNOWN_PAIR_COUNT; i++) {
        if ((KNOWN_PAIRS[i].system_a == system_a &&
             KNOWN_PAIRS[i].system_b == system_b) ||
            (KNOWN_PAIRS[i].system_a == system_b &&
             KNOWN_PAIRS[i].system_b == system_a)) {
            return &KNOWN_PAIRS[i];
        }
    }
    return NULL;
}

int br_known_pair_count(void) {
    return KNOWN_PAIR_COUNT;
}

void br_score_correlation(br_correlation_t *c) {
    if (!c) return;

    const br_known_pair_t *pair = br_lookup_pair(c->system_a, c->system_b);
    if (pair) {
        c->type = pair->type;
        c->confidence = pair->base_confidence;
    } else {
        /* Unknown pair — default to coincidental with low confidence */
        c->type = BR_CORR_COINCIDENTAL;
        c->confidence = 0.1;
    }
}

void br_score_result(br_result_t *result) {
    if (!result) return;

    double max_confidence = 0.0;
    for (int i = 0; i < result->correlation_count; i++) {
        br_score_correlation(&result->correlations[i]);
        if (result->correlations[i].confidence > max_confidence) {
            max_confidence = result->correlations[i].confidence;
        }
    }

    /* Recalculate convergence_strength as max of:
     * existing strength (from tier) and max correlation confidence */
    if (max_confidence > result->convergence_strength) {
        result->convergence_strength = max_confidence;
    }
}

double br_score_rarity(int event_count) {
    if (event_count <= 0) return 0.0;
    if (event_count <= 2) return 0.2;
    if (event_count <= 4) return 0.5;
    if (event_count <= 7) return 0.8;
    return 1.0;
}
