/* brain_adapt.h — Context-adaptive insight ranking (Track 26, L2.2)
 *
 * Reorders brain insights based on user navigation context:
 *   - User who explores Tzolkin → Tzolkin insights ranked higher
 *   - Insights already shown → ranked lower (freshness)
 *   - New users → surface-level insights; returning users → deeper
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * Uses stack-allocated arrays for reordering. */

#ifndef TIME_BRAIN_ADAPT_H
#define TIME_BRAIN_ADAPT_H

#include "brain_types.h"
#include "user_context.h"

/* Ring buffer size for recently shown insight tracking */
#define BR_SEEN_CACHE_SIZE 16

/* Recently-shown insight tracker (passed by caller, stack-allocated) */
typedef struct {
    int  seen_types[BR_SEEN_CACHE_SIZE];  /* insight type + system hash */
    int  count;
    int  head;
} br_seen_cache_t;

/* Adapt a brain result based on user context.
 * Reorders insights by: user affinity > freshness > score.
 * Modifies result->insights[] ordering and result->top_insight_index.
 * cache may be NULL (no freshness filtering). */
void br_adapt(br_result_t *result, const uc_context_t *ctx,
              br_seen_cache_t *cache);

/* Record an insight as "seen" in the cache. */
void br_seen_record(br_seen_cache_t *cache, const br_insight_t *insight);

/* Check if an insight was recently seen. Returns 1 if seen. */
int br_seen_check(const br_seen_cache_t *cache, const br_insight_t *insight);

/* Create an empty seen cache. */
br_seen_cache_t br_seen_init(void);

/* Compute a hash for an insight (for seen-cache comparison). */
int br_insight_hash(const br_insight_t *insight);

#endif /* TIME_BRAIN_ADAPT_H */
