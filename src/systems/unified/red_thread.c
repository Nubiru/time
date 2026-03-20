/* red_thread.c — Convergence narrative intelligence implementation.
 *
 * Weaves the "red thread" — the invisible connection between systems
 * that converge on a given day. Uses brain_explain for WHY pairs relate,
 * cd_system_name for human-readable names, and counts explanation types
 * to classify the dominant relationship.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "red_thread.h"
#include "convergence_detect.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* ===================================================================
 * Internal helpers
 * =================================================================== */

/* Count explanation types in an insight's system pairs. */
static void count_explain_types(const br_insight_t *insight,
                                int *shared, int *transmission,
                                int *convergent, int *unknown,
                                int *total_pairs)
{
    *shared = *transmission = *convergent = *unknown = *total_pairs = 0;

    for (int i = 0; i < insight->system_count; i++) {
        for (int j = i + 1; j < insight->system_count; j++) {
            (*total_pairs)++;
            const br_explanation_t *e = br_explain_lookup(
                insight->systems[i], insight->systems[j]);
            if (!e) {
                (*unknown)++;
                continue;
            }
            switch (e->why) {
                case BR_EXPLAIN_SHARED_SOURCE:  (*shared)++;       break;
                case BR_EXPLAIN_TRANSMISSION:   (*transmission)++; break;
                case BR_EXPLAIN_CONVERGENT:     (*convergent)++;   break;
                default:                        (*unknown)++;      break;
            }
        }
    }
}

/* Safe snprintf append: write at pos, update pos and remaining. */
static void append(char *buf, int *pos, int *remaining, const char *fmt, ...)
{
    if (*remaining <= 1) return;

    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf + *pos, (size_t)*remaining, fmt, args);
    va_end(args);

    if (n > 0) {
        int used = n < *remaining ? n : *remaining - 1;
        *pos += used;
        *remaining -= used;
    }
}

/* ===================================================================
 * Public API
 * =================================================================== */

const char *rt_classify(const br_result_t *result)
{
    if (!result || result->insight_count == 0) return "quiet";

    const br_insight_t *top = &result->insights[result->top_insight_index];
    int shared, trans, conv, unk, total;
    count_explain_types(top, &shared, &trans, &conv, &unk, &total);

    if (total == 0) return "quiet";
    if (shared >= trans && shared >= conv) return "shared observation";
    if (trans >= shared && trans >= conv)  return "cultural transmission";
    if (conv >= shared && conv >= trans)   return "independent discovery";
    return "mixed";
}

int rt_compose_insight(const br_result_t *result, int insight_index,
                       rt_result_t *out)
{
    if (!result || !out) return 0;
    memset(out, 0, sizeof(*out));

    if (insight_index < 0 || insight_index >= result->insight_count) return 0;
    const br_insight_t *insight = &result->insights[insight_index];

    if (insight->system_count < 2) {
        int n = snprintf(out->narrative, RT_NARRATIVE_MAX,
                         "A quiet moment — the systems count in silence.");
        out->narrative_len = n > 0 ? n : 0;
        return out->narrative_len;
    }

    int pos = 0;
    int remaining = RT_NARRATIVE_MAX;

    /* Opening: list the systems */
    append(out->narrative, &pos, &remaining, "Today, ");

    for (int i = 0; i < insight->system_count && remaining > 1; i++) {
        const char *name = cd_system_name((cd_system_t)insight->systems[i]);
        if (i == 0) {
            append(out->narrative, &pos, &remaining, "%s", name);
        } else if (i == insight->system_count - 1) {
            if (insight->system_count == 2) {
                append(out->narrative, &pos, &remaining, " and %s", name);
            } else {
                append(out->narrative, &pos, &remaining, ", and %s", name);
            }
        } else {
            append(out->narrative, &pos, &remaining, ", %s", name);
        }
    }

    append(out->narrative, &pos, &remaining, " converge. ");

    /* Find the best explanation to lead with */
    int explained = 0;
    int total_pairs = 0;
    const br_explanation_t *best_explain = NULL;

    for (int i = 0; i < insight->system_count; i++) {
        for (int j = i + 1; j < insight->system_count; j++) {
            total_pairs++;
            const br_explanation_t *e = br_explain_lookup(
                insight->systems[i], insight->systems[j]);
            if (e) {
                explained++;
                if (!best_explain) best_explain = e;
            }
        }
    }

    /* Add the best explanation as the thread */
    if (best_explain && best_explain->brief && remaining > 1) {
        append(out->narrative, &pos, &remaining, "%s.", best_explain->brief);
    }

    /* Classify the relationship */
    const char *classification = rt_classify(result);
    if (remaining > 1) {
        append(out->narrative, &pos, &remaining,
               " This is %s.", classification);
    }

    out->narrative_len = pos;
    out->pairs_explained = explained;
    out->pairs_total = total_pairs;
    out->confidence = total_pairs > 0 ?
        (double)explained / (double)total_pairs : 0.0;

    return pos;
}

int rt_compose(const br_result_t *result, rt_result_t *out)
{
    if (!result || !out) return 0;
    if (result->insight_count == 0) {
        memset(out, 0, sizeof(*out));
        int n = snprintf(out->narrative, RT_NARRATIVE_MAX,
                         "A quiet moment — the systems count in silence.");
        out->narrative_len = n > 0 ? n : 0;
        return out->narrative_len;
    }
    return rt_compose_insight(result, result->top_insight_index, out);
}
