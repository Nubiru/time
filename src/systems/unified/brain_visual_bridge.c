/* brain_visual_bridge.c — Bridge: brain intelligence → visual encoding */

#include "brain_visual_bridge.h"
#include <string.h>

/* ===================================================================
 * Mapping: correlation → visual link
 * =================================================================== */

br_visual_link_t br_visual_link_from_correlation(const br_correlation_t *c) {
    br_visual_link_t link;
    memset(&link, 0, sizeof(link));

    if (!c) return link;

    link.system_a = c->system_a;
    link.system_b = c->system_b;

    /* Opacity scales with alignment score */
    link.line_opacity = (float)c->alignment_score;

    /* Thickness: 1.0 base + up to 3.0 extra based on confidence */
    link.line_thickness = 1.0f + 3.0f * (float)c->confidence;

    /* Glow: structural=bright, coincidental=dim */
    switch (c->type) {
    case BR_CORR_STRUCTURAL:   link.glow_strength = 1.0f;  break;
    case BR_CORR_HARMONIC:     link.glow_strength = 0.7f;  break;
    case BR_CORR_APPROXIMATE:  link.glow_strength = 0.4f;  break;
    case BR_CORR_COINCIDENTAL: link.glow_strength = 0.15f; break;
    default:                   link.glow_strength = 0.0f;  break;
    }

    /* Color warmth: high-confidence = warm (gold), low = cool (blue) */
    link.color_warmth = (float)c->confidence;

    return link;
}

/* ===================================================================
 * Mapping: insight → visual highlight
 * =================================================================== */

br_visual_insight_t br_visual_insight_from_brain(const br_insight_t *insight) {
    br_visual_insight_t vis;
    memset(&vis, 0, sizeof(vis));

    if (!insight) return vis;

    /* Highlight the first system in the insight's system list */
    vis.highlight_system = (insight->system_count > 0)
        ? insight->systems[0] : -1;

    /* Pulse intensity from insight score */
    vis.pulse_intensity = (float)insight->score;

    /* Ring glow: convergences glow more, quiet days barely glow */
    switch (insight->type) {
    case BR_INSIGHT_CONVERGENCE:  vis.ring_glow = 0.9f;  break;
    case BR_INSIGHT_ASTRONOMICAL: vis.ring_glow = 0.7f;  break;
    case BR_INSIGHT_CYCLE:        vis.ring_glow = 0.6f;  break;
    case BR_INSIGHT_NUMBER:       vis.ring_glow = 0.5f;  break;
    case BR_INSIGHT_FESTIVAL:     vis.ring_glow = 0.4f;  break;
    case BR_INSIGHT_QUIET:        vis.ring_glow = 0.05f; break;
    default:                      vis.ring_glow = 0.0f;  break;
    }

    /* Background shift: only for high-score convergences */
    if (insight->score > 0.7f) {
        vis.background_shift = (float)(insight->score - 0.7) / 0.3f;
    }

    return vis;
}

/* ===================================================================
 * Scene-level intensity
 * =================================================================== */

float br_visual_scene_intensity(double convergence_strength) {
    if (convergence_strength < 0.0) return 0.0f;
    if (convergence_strength > 1.0) return 1.0f;
    return (float)convergence_strength;
}

/* ===================================================================
 * Full result → visual frame conversion
 * =================================================================== */

void br_visual_from_result(const br_result_t *result,
                           br_visual_frame_t *frame) {
    if (!frame) return;
    memset(frame, 0, sizeof(*frame));

    if (!result) return;

    /* Convert correlations → visual links */
    int link_count = result->correlation_count;
    if (link_count > BR_VISUAL_MAX_LINKS) link_count = BR_VISUAL_MAX_LINKS;
    for (int i = 0; i < link_count; i++) {
        frame->links[i] = br_visual_link_from_correlation(
            &result->correlations[i]);
    }
    frame->link_count = link_count;

    /* Convert insights → visual highlights */
    int ins_count = result->insight_count;
    if (ins_count > BR_VISUAL_MAX_INSIGHTS) ins_count = BR_VISUAL_MAX_INSIGHTS;
    for (int i = 0; i < ins_count; i++) {
        frame->insights[i] = br_visual_insight_from_brain(
            &result->insights[i]);
    }
    frame->insight_count = ins_count;

    /* Scene-level parameters */
    frame->overall_intensity = br_visual_scene_intensity(
        result->convergence_strength);

    /* Atmosphere warmth: quiet days are cool, convergences are warm */
    frame->atmosphere_warmth = frame->overall_intensity * 0.8f;
}
