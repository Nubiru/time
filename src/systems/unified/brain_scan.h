/* brain_scan.h — Core brain scanner (Track 26, L0.2)
 *
 * The brain's primary entry point. Scans a Julian Day across all
 * knowledge systems and produces a structured result of correlations
 * and insights.
 *
 * Two modes:
 *   br_scan()    — full scan: calls cd_scan internally, fills result
 *   br_process() — pure transform: maps existing cd_result_t to brain result
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_SCAN_H
#define TIME_BRAIN_SCAN_H

#include "brain_types.h"
#include "convergence_detect.h"

/* Process convergence detector output into brain results.
 * Pure transform: no external calls, deterministic.
 * Maps cd_events → br_insights, extracts system-pair correlations.
 * out must point to valid br_result_t (will be zeroed internally). */
void br_process(const cd_result_t *cd, double jd, br_result_t *out);

/* Full scan: calls cd_scan(jd), then br_process.
 * Convenience wrapper for the complete pipeline entry point. */
void br_scan(double jd, br_result_t *out);

/* Count insights with score >= threshold (0.0-1.0). */
int br_active_count(const br_result_t *result, double min_score);

/* Get pointer to the top-scoring insight, or NULL if no insights. */
const br_insight_t *br_top_insight(const br_result_t *result);

/* Map a cd_strength_t tier to a 0.0-1.0 convergence strength. */
double br_strength_from_tier(cd_strength_t tier);

#endif /* TIME_BRAIN_SCAN_H */
