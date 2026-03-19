/* fun_facts.h -- Pure module providing curated cross-system fun facts.
 *
 * Contains 85 fun facts spanning astronomy, calendars, geology, culture,
 * and mathematics. Each fact has a category, trigger condition, system
 * bitmask, and priority level. An evaluation function matches facts to
 * a navigation context and returns ranked results. Supports seen-fact
 * exclusion for rotation.
 *
 * System bitmask bits:
 *   0=astronomy, 1=astrology, 2=tzolkin, 3=iching, 4=chinese,
 *   5=human_design, 6=hebrew, 7=islamic, 8=buddhist, 9=hindu,
 *   10=kabbalah, 11=gregorian, 12=geology, 13=earth
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_FUN_FACTS_H
#define TIME_FUN_FACTS_H

#include "../../ui/sensitivity.h"

#define FF_MAX_RESULTS  16
#define FF_TEXT_MAX     256

typedef enum {
    FF_CAT_CROSS_SYSTEM = 0,
    FF_CAT_HISTORICAL,
    FF_CAT_ASTRONOMICAL,
    FF_CAT_MATHEMATICAL,
    FF_CAT_GEOLOGICAL,
    FF_CAT_CULTURAL,
    FF_CAT_COUNT
} ff_category_t;

typedef enum {
    FF_TRIG_ALWAYS = 0,
    FF_TRIG_VIEW_SPACE,
    FF_TRIG_VIEW_EARTH,
    FF_TRIG_VIEW_TZOLKIN,
    FF_TRIG_VIEW_ICHING,
    FF_TRIG_VIEW_CALENDAR,
    FF_TRIG_VIEW_DEEP_TIME,
    FF_TRIG_CONVERGENCE,
    FF_TRIG_SYSTEM_ACTIVE,
    FF_TRIG_COUNT
} ff_trigger_t;

typedef struct {
    int id;
    const char *text;
    ff_category_t category;
    ff_trigger_t trigger;
    unsigned int system_mask;
    int priority;
    sensitivity_t sensitivity;
} ff_fact_t;

typedef struct {
    int view;
    int scale;
    unsigned int active_systems;
    int convergence_active;
    int user_seal;
} ff_context_t;

typedef struct {
    int fact_index;
    int score;
} ff_result_t;

typedef struct {
    ff_result_t results[FF_MAX_RESULTS];
    int count;
} ff_selection_t;

/* Return total number of facts in the database. */
int ff_fact_count(void);

/* Return fact at index. Returns fact with id=-1 for invalid index. */
ff_fact_t ff_fact_get(int index);

/* Collect indices of facts in a given category. Returns count written. */
int ff_by_category(ff_category_t cat, int *out, int max_out);

/* Evaluate all facts against a context, excluding seen facts. */
ff_selection_t ff_evaluate(const ff_context_t *ctx,
                           const int *seen, int seen_count);

/* Convenience: return the index of the single best fact, or -1. */
int ff_best_fact(const ff_context_t *ctx, const int *seen, int seen_count);

/* Return human-readable category name. */
const char *ff_category_name(ff_category_t cat);

/* Return human-readable trigger name. */
const char *ff_trigger_name(ff_trigger_t trig);

/* Return a default context (space view, human scale, no systems). */
ff_context_t ff_context_default(void);

/* Find facts by sensitivity level. Writes matching indices to out[].
 * Returns count found (up to max_out). */
int ff_by_sensitivity(sensitivity_t level, int *out, int max_out);

#endif /* TIME_FUN_FACTS_H */
