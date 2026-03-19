/* brain_audio_bridge.c — Bridge: brain intelligence → audio event pipeline */

#include "brain_audio_bridge.h"
#include <string.h>

/* ===================================================================
 * Mapping tables
 * =================================================================== */

/* Consonance value per correlation type.
 * Structural relationships are perfectly consonant.
 * Coincidental ones are mostly dissonant. */
static const float s_consonance[] = {
    [BR_CORR_STRUCTURAL]   = 1.00f,
    [BR_CORR_HARMONIC]     = 0.75f,
    [BR_CORR_APPROXIMATE]  = 0.50f,
    [BR_CORR_COINCIDENTAL] = 0.25f,
};

/* Audio intensity per insight type.
 * Convergences are dramatic; quiet days are subtle. */
static const float s_intensity[] = {
    [BR_INSIGHT_CONVERGENCE]  = 0.9f,
    [BR_INSIGHT_NUMBER]       = 0.6f,
    [BR_INSIGHT_CYCLE]        = 0.7f,
    [BR_INSIGHT_FESTIVAL]     = 0.5f,
    [BR_INSIGHT_ASTRONOMICAL] = 0.8f,
    [BR_INSIGHT_QUIET]        = 0.1f,
};

/* ===================================================================
 * Individual mapping functions
 * =================================================================== */

float br_audio_correlation_consonance(br_correlation_type_t type) {
    if (type < 0 || type >= BR_CORR_COUNT) return 0.0f;
    return s_consonance[type];
}

float br_audio_insight_intensity(br_insight_type_t type) {
    if (type < 0 || type >= BR_INSIGHT_COUNT) return 0.0f;
    return s_intensity[type];
}

float br_audio_convergence_density(double convergence_strength) {
    if (convergence_strength < 0.0) return 0.0f;
    if (convergence_strength > 1.0) return 1.0f;
    return (float)convergence_strength;
}

/* ===================================================================
 * Full result → audio event conversion
 * =================================================================== */

void br_audio_from_result(const br_result_t *result, br_audio_event_t *out) {
    if (!out) return;
    memset(out, 0, sizeof(*out));

    if (!result) return;

    /* Consonance: weighted average of correlation type consonances */
    if (result->correlation_count > 0) {
        float sum = 0.0f;
        for (int i = 0; i < result->correlation_count; i++) {
            sum += br_audio_correlation_consonance(result->correlations[i].type);
        }
        out->consonance = sum / (float)result->correlation_count;
    }

    /* Convergence density: direct map from convergence_strength */
    out->convergence_density = br_audio_convergence_density(
        result->convergence_strength);

    /* Convergence count: number of insights */
    out->convergence_count = result->insight_count;

    /* Dominant insight type */
    if (result->insight_count > 0 &&
        result->top_insight_index >= 0 &&
        result->top_insight_index < result->insight_count) {
        out->dominant_insight = result->insights[result->top_insight_index].type;
    } else {
        out->dominant_insight = BR_INSIGHT_QUIET;
    }

    /* System count: sum across all insights (may double-count, but
     * gives audio a sense of how "populated" the convergence is) */
    int total_systems = 0;
    for (int i = 0; i < result->insight_count; i++) {
        total_systems += result->insights[i].system_count;
    }
    out->system_count = total_systems;

    /* Event intensity: highest insight intensity × convergence density.
     * Baseline from convergence_strength if no insights present. */
    float max_intensity = 0.0f;
    for (int i = 0; i < result->insight_count; i++) {
        float v = br_audio_insight_intensity(result->insights[i].type);
        if (v > max_intensity) max_intensity = v;
    }
    if (max_intensity > 0.0f) {
        out->event_intensity = max_intensity *
            (0.5f + 0.5f * out->convergence_density);
    } else {
        /* Fallback: derive from convergence density + correlation count */
        out->event_intensity = out->convergence_density * 0.3f;
    }

    /* Surprise factor: from convergence_strength above 0.7 threshold */
    if (result->convergence_strength > 0.7) {
        out->surprise_factor = (float)(
            (result->convergence_strength - 0.7) / 0.3);
        if (out->surprise_factor > 1.0f) out->surprise_factor = 1.0f;
    }
}
