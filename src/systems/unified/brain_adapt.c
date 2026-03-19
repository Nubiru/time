/* brain_adapt.c — Context-adaptive insight ranking implementation.
 *
 * Reorders insights using a composite score:
 *   affinity_boost + freshness_penalty + base_score
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_adapt.h"
#include <string.h>

/* ===================================================================
 * Insight hashing (for seen-cache)
 * =================================================================== */

int br_insight_hash(const br_insight_t *insight) {
    if (!insight) return 0;
    /* Combine type + first two systems for a quick identity hash */
    int hash = (int)insight->type * 1000;
    if (insight->system_count > 0) hash += insight->systems[0] * 31;
    if (insight->system_count > 1) hash += insight->systems[1];
    return hash;
}

/* ===================================================================
 * Seen cache
 * =================================================================== */

br_seen_cache_t br_seen_init(void) {
    br_seen_cache_t cache;
    memset(&cache, 0, sizeof(cache));
    return cache;
}

void br_seen_record(br_seen_cache_t *cache, const br_insight_t *insight) {
    if (!cache || !insight) return;
    int hash = br_insight_hash(insight);
    cache->seen_types[cache->head] = hash;
    cache->head = (cache->head + 1) % BR_SEEN_CACHE_SIZE;
    if (cache->count < BR_SEEN_CACHE_SIZE) cache->count++;
}

int br_seen_check(const br_seen_cache_t *cache, const br_insight_t *insight) {
    if (!cache || !insight) return 0;
    int hash = br_insight_hash(insight);
    for (int i = 0; i < cache->count; i++) {
        if (cache->seen_types[i] == hash) return 1;
    }
    return 0;
}

/* ===================================================================
 * Adaptation scoring
 * =================================================================== */

/* Compute affinity boost for an insight based on user's system preferences.
 * Returns 0.0-0.5 bonus. */
static double affinity_boost(const br_insight_t *insight,
                             const uc_context_t *ctx) {
    if (!insight || !ctx) return 0.0;
    double max_aff = 0.0;
    for (int i = 0; i < insight->system_count; i++) {
        int sys = insight->systems[i];
        if (sys >= 0 && sys < UC_MAX_SYSTEMS) {
            if (ctx->system_affinity[sys] > max_aff) {
                max_aff = ctx->system_affinity[sys];
            }
        }
    }
    /* Scale affinity to a 0.0-0.5 boost */
    return max_aff * 0.5;
}

/* ===================================================================
 * Public API
 * =================================================================== */

void br_adapt(br_result_t *result, const uc_context_t *ctx,
              br_seen_cache_t *cache) {
    if (!result || result->insight_count <= 1) return;

    /* Compute composite scores for each insight */
    double scores[BR_MAX_INSIGHTS];
    for (int i = 0; i < result->insight_count; i++) {
        br_insight_t *ins = &result->insights[i];

        /* Base score (0.0-1.0) */
        double s = ins->score;

        /* Affinity boost (0.0-0.5) */
        if (ctx) {
            s += affinity_boost(ins, ctx);
        }

        /* Freshness penalty: -0.3 if recently seen */
        if (cache && br_seen_check(cache, ins)) {
            s -= 0.3;
        }

        scores[i] = s;
    }

    /* Simple insertion sort by composite score (descending).
     * N is small (max 16), so O(n²) is fine. */
    for (int i = 1; i < result->insight_count; i++) {
        double key_score = scores[i];
        br_insight_t key_insight = result->insights[i];
        int j = i - 1;
        while (j >= 0 && scores[j] < key_score) {
            scores[j + 1] = scores[j];
            result->insights[j + 1] = result->insights[j];
            j--;
        }
        scores[j + 1] = key_score;
        result->insights[j + 1] = key_insight;
    }

    /* Update top_insight_index (now always 0 after sort) */
    result->top_insight_index = 0;
}
