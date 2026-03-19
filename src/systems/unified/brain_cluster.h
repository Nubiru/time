/* brain_cluster.h — Automatic system clustering by mathematical kinship (Track 26, L3.4)
 *
 * Groups knowledge systems into families based on their fundamental
 * cycle properties: lunar/solar/lunisolar type, cycle lengths, shared
 * mathematical constants.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_CLUSTER_H
#define TIME_BRAIN_CLUSTER_H

#include "brain_types.h"
#include "convergence_detect.h"

/* Maximum systems per cluster */
#define BR_CLUSTER_MAX_MEMBERS 12

/* Maximum clusters */
#define BR_CLUSTER_MAX 8

/* Calendar type classification */
typedef enum {
    BR_CAL_LUNAR = 0,       /* pure lunar (Islamic) */
    BR_CAL_SOLAR,           /* pure solar (Gregorian, Persian) */
    BR_CAL_LUNISOLAR,       /* lunisolar (Hebrew, Chinese, Hindu) */
    BR_CAL_CYCLICAL,        /* cyclical counting (Tzolkin, I Ching) */
    BR_CAL_OBSERVATIONAL,   /* astronomy-based (Celtic, Astrology) */
    BR_CAL_TYPE_COUNT
} br_cal_type_t;

/* A cluster of related systems */
typedef struct {
    const char     *name;
    br_cal_type_t   type;
    int             members[BR_CLUSTER_MAX_MEMBERS];
    int             member_count;
    const char     *reason;
} br_cluster_t;

/* Cluster result */
typedef struct {
    br_cluster_t    clusters[BR_CLUSTER_MAX];
    int             count;
} br_cluster_result_t;

/* Compute system clusters.
 * Returns number of clusters found. */
int br_cluster_compute(br_cluster_result_t *out);

/* Find which cluster a system belongs to.
 * Returns pointer to the cluster, or NULL if unclustered. */
const br_cluster_t *br_cluster_for_system(const br_cluster_result_t *result,
                                          int system_id);

/* Calendar type name. "?" for invalid. */
const char *br_cal_type_name(br_cal_type_t type);

#endif /* TIME_BRAIN_CLUSTER_H */
