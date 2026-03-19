/* brain_scan.c — Core brain scanner implementation.
 *
 * Maps convergence detector output into brain insights and correlations.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_scan.h"
#include <string.h>

/* ===================================================================
 * Internal helpers
 * =================================================================== */

/* Map cd_strength_t to normalized strength (0.0-1.0). */
double br_strength_from_tier(cd_strength_t tier) {
    switch (tier) {
        case CD_STRENGTH_MINOR:   return 0.25;
        case CD_STRENGTH_NOTABLE: return 0.50;
        case CD_STRENGTH_MAJOR:   return 0.75;
        case CD_STRENGTH_RARE:    return 1.00;
        default:                  return 0.0;
    }
}

/* Extract system-pair correlations from a convergence event.
 * For N systems, produces N*(N-1)/2 pairs. Returns pairs added. */
static int extract_pairs(const cd_event_t *event,
                         br_correlation_t *corrs, int max_corrs,
                         int current_count) {
    int added = 0;
    double strength = br_strength_from_tier(event->strength);

    for (int i = 0; i < event->system_count && current_count + added < max_corrs; i++) {
        for (int j = i + 1; j < event->system_count && current_count + added < max_corrs; j++) {
            br_correlation_t *c = &corrs[current_count + added];
            c->system_a = (int)event->systems[i];
            c->system_b = (int)event->systems[j];
            c->alignment_score = strength;
            c->type = BR_CORR_APPROXIMATE; /* default; brain_score refines later */
            c->confidence = event->significance;
            added++;
        }
    }
    return added;
}

/* ===================================================================
 * Public API
 * =================================================================== */

void br_process(const cd_result_t *cd, double jd, br_result_t *out) {
    memset(out, 0, sizeof(*out));
    out->jd = jd;

    if (!cd) {
        /* No convergence data: quiet day */
        out->insights[0].type = BR_INSIGHT_QUIET;
        strncpy(out->insights[0].headline, "A quiet day across all systems",
                BR_HEADLINE_MAX - 1);
        out->insights[0].score = 0.0;
        out->insight_count = 1;
        out->top_insight_index = 0;
        return;
    }

    double max_strength = 0.0;
    int best_insight = 0;

    /* Map each convergence event to an insight */
    for (int i = 0; i < cd->count && out->insight_count < BR_MAX_INSIGHTS; i++) {
        const cd_event_t *event = &cd->events[i];
        br_insight_t *insight = &out->insights[out->insight_count];

        insight->type = BR_INSIGHT_CONVERGENCE;
        if (event->description) {
            strncpy(insight->headline, event->description, BR_HEADLINE_MAX - 1);
            insight->headline[BR_HEADLINE_MAX - 1] = '\0';
        }
        insight->score = event->significance;

        /* Copy system list */
        int count = event->system_count;
        if (count > BR_MAX_SYSTEMS) count = BR_MAX_SYSTEMS;
        for (int s = 0; s < count; s++) {
            insight->systems[s] = (int)event->systems[s];
        }
        insight->system_count = count;

        /* Track best insight */
        double strength = br_strength_from_tier(event->strength);
        if (strength > max_strength) {
            max_strength = strength;
            best_insight = out->insight_count;
        }

        out->insight_count++;

        /* Extract pairwise correlations from this event */
        int pairs_added = extract_pairs(event, out->correlations,
                                        BR_MAX_CORRELATIONS,
                                        out->correlation_count);
        out->correlation_count += pairs_added;
    }

    /* If no events detected, add quiet insight */
    if (out->insight_count == 0) {
        out->insights[0].type = BR_INSIGHT_QUIET;
        strncpy(out->insights[0].headline, "A quiet day across all systems",
                BR_HEADLINE_MAX - 1);
        out->insights[0].score = 0.0;
        out->insight_count = 1;
        best_insight = 0;
    }

    out->top_insight_index = best_insight;
    out->convergence_strength = max_strength;
}

void br_scan(double jd, br_result_t *out) {
    cd_result_t cd = cd_scan(jd);
    br_process(&cd, jd, out);
}

int br_active_count(const br_result_t *result, double min_score) {
    if (!result) return 0;
    int count = 0;
    for (int i = 0; i < result->insight_count; i++) {
        if (result->insights[i].score >= min_score) {
            count++;
        }
    }
    return count;
}

const br_insight_t *br_top_insight(const br_result_t *result) {
    if (!result || result->insight_count == 0) return NULL;
    if (result->top_insight_index < 0 ||
        result->top_insight_index >= result->insight_count) return NULL;
    return &result->insights[result->top_insight_index];
}
