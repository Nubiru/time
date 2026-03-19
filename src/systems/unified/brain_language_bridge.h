/* brain_language_bridge.h — Bridge: brain intelligence → language stream
 *
 * Provides structured template slots and convergence signals that the
 * language stream's narrative composer can consume. Unblocks LANGUAGE L1.1+L1.4.
 *
 * Template slots: named pieces of data for narrative templates.
 * Convergence signals: when to show wisdom quotes vs ordinary text.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_LANGUAGE_BRIDGE_H
#define TIME_BRAIN_LANGUAGE_BRIDGE_H

#include "brain_types.h"

#define BR_LANG_SLOT_MAX 256

/* Template slot types — named pieces for narrative composition */
typedef enum {
    BR_LANG_SLOT_HEADLINE = 0,      /* today's main headline */
    BR_LANG_SLOT_THREAD_TYPE,       /* "convergence"/"festival"/"quiet" etc */
    BR_LANG_SLOT_TOP_SYSTEM,        /* name of most active system */
    BR_LANG_SLOT_CONVERGENCE_ADJ,   /* "ordinary"/"notable"/"rare"/etc */
    BR_LANG_SLOT_SYSTEM_COUNT,      /* "3 systems" etc */
    BR_LANG_SLOT_WISDOM_CONTEXT,    /* what the wisdom quote relates to */
    BR_LANG_SLOT_TEMPORAL_CONTEXT,  /* "last seen 47 days ago" */
    BR_LANG_SLOT_PERSONAL_NOTE,     /* personal milestone if applicable */
    BR_LANG_SLOT_COUNT
} br_lang_slot_type_t;

/* A single template slot */
typedef struct {
    br_lang_slot_type_t  type;
    char                 value[BR_LANG_SLOT_MAX];
    int                  filled;    /* 1 if value is populated */
} br_lang_slot_t;

/* Complete template context for the language stream */
typedef struct {
    br_lang_slot_t  slots[BR_LANG_SLOT_COUNT];
    int             slot_count;
    double          convergence_strength; /* 0.0-1.0 */
    int             wisdom_id;            /* suggested wisdom quote index */
    int             show_wisdom;          /* 1 if convergence warrants it */
} br_lang_context_t;

/* Fill template context from brain scan result + narrative.
 * ctx is zeroed internally.
 * narrative may be NULL (only result-derived slots filled). */
void br_lang_from_result(const br_result_t *result,
                         const br_narrative_t *narrative,
                         br_lang_context_t *ctx);

/* Get slot value by type. Returns "" if slot not filled or type invalid. */
const char *br_lang_slot_value(const br_lang_context_t *ctx,
                               br_lang_slot_type_t type);

/* Check if convergence warrants wisdom display.
 * Returns 1 if convergence_strength >= threshold. */
int br_lang_should_show_wisdom(const br_lang_context_t *ctx, double threshold);

/* Convergence adjective for a given strength.
 * Returns static string: "ordinary"/"notable"/"significant"/"rare"/"extraordinary" */
const char *br_lang_convergence_adj(double strength);

/* Name of a slot type. "?" for invalid. */
const char *br_lang_slot_name(br_lang_slot_type_t type);

#endif /* TIME_BRAIN_LANGUAGE_BRIDGE_H */
