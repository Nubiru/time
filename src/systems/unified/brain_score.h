/* brain_score.h — Confidence and rarity scoring for brain correlations (Track 26, L0.3)
 *
 * Refines raw correlations from brain_scan with mathematical analysis:
 *   - Classifies system pairs as structural/harmonic/approximate/coincidental
 *   - Assigns confidence scores based on known relationships
 *   - Computes rarity of convergence patterns
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_SCORE_H
#define TIME_BRAIN_SCORE_H

#include "brain_types.h"

/* Maximum known system-pair relationships in the static database */
#define BR_MAX_KNOWN_PAIRS 96

/* Known relationship between two systems */
typedef struct {
    int                    system_a;
    int                    system_b;
    br_correlation_type_t  type;
    double                 base_confidence;
    const char            *reason;
} br_known_pair_t;

/* Score a single correlation by looking up the system pair
 * in the known-relationship database.
 * Modifies c->type, c->confidence in place. */
void br_score_correlation(br_correlation_t *c);

/* Score all correlations in a brain result.
 * Calls br_score_correlation on each, then recalculates
 * convergence_strength from the refined scores. */
void br_score_result(br_result_t *result);

/* Compute rarity score for a given convergence event count.
 * Returns 0.0 (very common) to 1.0 (extremely rare).
 * Based on the statistical distribution of convergence counts:
 *   0 events → 0.0 (most days)
 *   1-2 events → 0.2
 *   3-4 events → 0.5
 *   5-7 events → 0.8
 *   8+ events → 1.0 */
double br_score_rarity(int event_count);

/* Look up a known relationship between two systems.
 * Returns pointer to static data, or NULL if pair is unknown.
 * Order-independent: (a,b) and (b,a) match the same entry. */
const br_known_pair_t *br_lookup_pair(int system_a, int system_b);

/* Total number of known system-pair relationships. */
int br_known_pair_count(void);

#endif /* TIME_BRAIN_SCORE_H */
