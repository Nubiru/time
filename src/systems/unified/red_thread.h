/* red_thread.h — Convergence narrative intelligence (Track 26, S98)
 *
 * Given a convergence, generates the STORY of invisible connections.
 * "The Spring Equinox aligns with Eid al-Fitr and Bahai Naw-Ruz —
 * three traditions anchored to the same astronomical event, each
 * naming it in their own tongue."
 *
 * Combines brain_explain (WHY pairs relate), brain_narrative (headline),
 * and system names into a multi-sentence narrative.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_RED_THREAD_H
#define TIME_RED_THREAD_H

#include "brain_types.h"
#include "brain_explain.h"

/* Maximum narrative length */
#define RT_NARRATIVE_MAX 512

/* Red thread result */
typedef struct {
    char    narrative[RT_NARRATIVE_MAX];   /* the story */
    int     narrative_len;
    int     pairs_explained;              /* how many pairs had explanations */
    int     pairs_total;                  /* total pairs in convergence */
    double  confidence;                   /* average explanation confidence */
} rt_result_t;

/* Generate a red thread narrative from a brain scan result.
 * Weaves system names + explanations into a story.
 * out is zeroed internally.
 * Returns narrative_len (chars written). */
int rt_compose(const br_result_t *result, rt_result_t *out);

/* Generate a red thread for a specific insight within a result.
 * insight_index selects which insight to narrate.
 * Returns narrative_len. */
int rt_compose_insight(const br_result_t *result, int insight_index,
                       rt_result_t *out);

/* Classify the relationship type of a convergence.
 * Returns static string: "shared observation" / "cultural transmission" /
 * "independent discovery" / "mixed" based on the dominant explanation type. */
const char *rt_classify(const br_result_t *result);

#endif /* TIME_RED_THREAD_H */
