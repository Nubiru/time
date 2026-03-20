/* brain_language_bridge.c — Bridge: brain intelligence → language stream */

#include "brain_language_bridge.h"
#include <string.h>
#include <stdio.h>

/* ===================================================================
 * Slot name table
 * =================================================================== */

static const char *s_slot_names[] = {
    [BR_LANG_SLOT_HEADLINE]        = "headline",
    [BR_LANG_SLOT_THREAD_TYPE]     = "thread_type",
    [BR_LANG_SLOT_TOP_SYSTEM]      = "top_system",
    [BR_LANG_SLOT_CONVERGENCE_ADJ] = "convergence_adj",
    [BR_LANG_SLOT_SYSTEM_COUNT]    = "system_count",
    [BR_LANG_SLOT_WISDOM_CONTEXT]  = "wisdom_context",
    [BR_LANG_SLOT_TEMPORAL_CONTEXT]= "temporal_context",
    [BR_LANG_SLOT_PERSONAL_NOTE]   = "personal_note",
};

/* ===================================================================
 * Convergence adjective thresholds
 * =================================================================== */

const char *br_lang_convergence_adj(double strength) {
    if (strength >= 0.9) return "extraordinary";
    if (strength >= 0.7) return "rare";
    if (strength >= 0.5) return "significant";
    if (strength >= 0.3) return "notable";
    return "ordinary";
}

const char *br_lang_slot_name(br_lang_slot_type_t type) {
    if (type < 0 || type >= BR_LANG_SLOT_COUNT) return "?";
    return s_slot_names[type];
}

/* ===================================================================
 * Slot helpers
 * =================================================================== */

static void fill_slot(br_lang_context_t *ctx, br_lang_slot_type_t type,
                       const char *value) {
    if (type < 0 || type >= BR_LANG_SLOT_COUNT) return;
    ctx->slots[type].type = type;
    snprintf(ctx->slots[type].value, BR_LANG_SLOT_MAX, "%.*s",
             (int)(BR_LANG_SLOT_MAX - 1), value);
    ctx->slots[type].filled = 1;
    ctx->slot_count++;
}

/* ===================================================================
 * Full conversion
 * =================================================================== */

void br_lang_from_result(const br_result_t *result,
                         const br_narrative_t *narrative,
                         br_lang_context_t *ctx) {
    if (!ctx) return;
    memset(ctx, 0, sizeof(*ctx));

    if (!result) return;

    ctx->convergence_strength = result->convergence_strength;

    /* Convergence adjective slot */
    fill_slot(ctx, BR_LANG_SLOT_CONVERGENCE_ADJ,
              br_lang_convergence_adj(result->convergence_strength));

    /* System count slot */
    {
        int total = 0;
        for (int i = 0; i < result->insight_count; i++) {
            total += result->insights[i].system_count;
        }
        char buf[32];
        snprintf(buf, sizeof(buf), "%d system%s", total, total == 1 ? "" : "s");
        fill_slot(ctx, BR_LANG_SLOT_SYSTEM_COUNT, buf);
    }

    /* Top insight as headline */
    if (result->insight_count > 0 &&
        result->top_insight_index >= 0 &&
        result->top_insight_index < result->insight_count) {
        const br_insight_t *top = &result->insights[result->top_insight_index];
        fill_slot(ctx, BR_LANG_SLOT_HEADLINE, top->headline);
    }

    /* Narrative-derived slots (if narrative provided) */
    if (narrative) {
        if (narrative->thread_type[0]) {
            fill_slot(ctx, BR_LANG_SLOT_THREAD_TYPE, narrative->thread_type);
        }
        if (narrative->headline[0]) {
            /* Override headline with narrative headline if present */
            fill_slot(ctx, BR_LANG_SLOT_HEADLINE, narrative->headline);
            /* Adjust slot_count since we're overwriting */
            ctx->slot_count--;
        }

        /* Wisdom */
        ctx->wisdom_id = narrative->wisdom_id;
        if (narrative->wisdom_id >= 0) {
            fill_slot(ctx, BR_LANG_SLOT_WISDOM_CONTEXT,
                      narrative->summary);
        }
    }

    /* Wisdom display threshold: 0.6 */
    ctx->show_wisdom = (result->convergence_strength >= 0.6) ? 1 : 0;
}

const char *br_lang_slot_value(const br_lang_context_t *ctx,
                               br_lang_slot_type_t type) {
    if (!ctx || type < 0 || type >= BR_LANG_SLOT_COUNT) return "";
    if (!ctx->slots[type].filled) return "";
    return ctx->slots[type].value;
}

int br_lang_should_show_wisdom(const br_lang_context_t *ctx, double threshold) {
    if (!ctx) return 0;
    return ctx->convergence_strength >= threshold ? 1 : 0;
}
