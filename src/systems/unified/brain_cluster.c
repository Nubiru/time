/* brain_cluster.c — System clustering implementation.
 *
 * Clusters knowledge systems by calendar type and mathematical kinship.
 * Uses static classification based on each system's fundamental properties.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_cluster.h"
#include <string.h>
#include <stddef.h>

static const char *const CAL_TYPE_NAMES[] = {
    "Lunar",
    "Solar",
    "Lunisolar",
    "Cyclical",
    "Observational"
};

/* ===================================================================
 * Static cluster definitions
 *
 * Each cluster groups systems that share fundamental cycle properties.
 * This is computed from known calendar mathematics, not hardcoded
 * cultural groupings.
 * =================================================================== */

static const br_cluster_t CLUSTERS[] = {
    {
        "Lunar Calendars", BR_CAL_LUNAR,
        { CD_SYS_ISLAMIC, CD_SYS_BUDDHIST, -1 },
        2,
        "Pure lunar: month = synodic month (29.53 days), no solar correction"
    },
    {
        "Solar Calendars", BR_CAL_SOLAR,
        { CD_SYS_PERSIAN, CD_SYS_COPTIC, CD_SYS_EGYPTIAN, CD_SYS_FRENCH, -1 },
        4,
        "Pure solar: year = tropical year, 12x30+5 or similar structure"
    },
    {
        "Lunisolar Calendars", BR_CAL_LUNISOLAR,
        { CD_SYS_HEBREW, CD_SYS_CHINESE, CD_SYS_HINDU, CD_SYS_KOREAN,
          CD_SYS_JAPANESE, CD_SYS_THAI, CD_SYS_MYANMAR, CD_SYS_TAMIL, -1 },
        8,
        "Lunisolar: lunar months + intercalary months to track solar year"
    },
    {
        "Cyclical Systems", BR_CAL_CYCLICAL,
        { CD_SYS_TZOLKIN, CD_SYS_ICHING, CD_SYS_BAHAI, -1 },
        3,
        "Cyclical counting: fixed-length repeating cycles (260, 64, 19-day)"
    },
    {
        "Observational Systems", BR_CAL_OBSERVATIONAL,
        { CD_SYS_ASTRONOMY, CD_SYS_ASTROLOGY, CD_SYS_CELTIC, CD_SYS_ZOROASTRIAN, -1 },
        4,
        "Tied to direct astronomical observation: solstices, equinoxes, aspects"
    },
};

static const int CLUSTER_COUNT = (int)(sizeof(CLUSTERS) / sizeof(CLUSTERS[0]));

/* ===================================================================
 * Public API
 * =================================================================== */

int br_cluster_compute(br_cluster_result_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));

    int count = CLUSTER_COUNT;
    if (count > BR_CLUSTER_MAX) count = BR_CLUSTER_MAX;

    for (int i = 0; i < count; i++) {
        out->clusters[i] = CLUSTERS[i];
    }
    out->count = count;

    return count;
}

const br_cluster_t *br_cluster_for_system(const br_cluster_result_t *result,
                                          int system_id) {
    if (!result) return NULL;

    for (int c = 0; c < result->count; c++) {
        for (int m = 0; m < result->clusters[c].member_count; m++) {
            if (result->clusters[c].members[m] == system_id) {
                return &result->clusters[c];
            }
        }
    }
    return NULL;
}

const char *br_cal_type_name(br_cal_type_t type) {
    if (type < 0 || type >= BR_CAL_TYPE_COUNT) return "?";
    return CAL_TYPE_NAMES[type];
}
