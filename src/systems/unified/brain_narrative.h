/* brain_narrative.h — Daily narrative composer (Track 26, L1.2)
 *
 * Composes a human-readable daily story from brain scan results.
 * Selects narrative template based on insight type, fills headline
 * and summary, picks a wisdom quote for context.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_NARRATIVE_H
#define TIME_BRAIN_NARRATIVE_H

#include "brain_types.h"

/* Thread types — what kind of story today tells */
#define BR_THREAD_CONVERGENCE  "convergence"
#define BR_THREAD_FESTIVAL     "festival"
#define BR_THREAD_NUMBER       "number"
#define BR_THREAD_CYCLE        "cycle"
#define BR_THREAD_ASTRONOMICAL "astronomical"
#define BR_THREAD_QUIET        "quiet"

/* Compose a daily narrative from brain scan results.
 * Fills out->headline, out->summary, out->thread_type, out->systems_featured.
 * out must point to valid br_narrative_t (will be zeroed internally).
 *
 * Returns 1 on success, 0 if result is NULL or empty. */
int br_narrative_compose(const br_result_t *result, br_narrative_t *out);

/* Format a single insight as a headline string.
 * Returns characters written (excluding null). */
int br_narrative_headline(const br_insight_t *insight, char *buf, int buf_size);

/* Format a narrative summary from multiple insights.
 * Returns characters written (excluding null). */
int br_narrative_summary(const br_result_t *result, char *buf, int buf_size);

#endif /* TIME_BRAIN_NARRATIVE_H */
