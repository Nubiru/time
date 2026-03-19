/*
 * convergence_pass.c — Convergence connection line render pass
 *
 * Stateful module (S1): renders lines connecting concentric knowledge
 * system rings when brain convergence data is available.
 * Uses convergence_visual.h for line geometry computation.
 *
 * For now, this is a placeholder pass that draws nothing.
 * When brain_visual_bridge is wired, it will draw connection lines
 * and ring glows based on correlation data.
 */

#ifdef __EMSCRIPTEN__

#include "convergence_pass.h"
#include <GLES3/gl3.h>

static int s_initialized;

int convergence_pass_init(void) {
    /* Placeholder: no GPU resources needed until brain wiring */
    s_initialized = 1;
    return 0;
}

void convergence_pass_draw(const render_frame_t *frame) {
    (void)frame;
    if (!s_initialized) return;
    /* Placeholder: convergence visualization drawn when brain data is wired.
     * Will use cv_line_between_rings() for each br_visual_link_t. */
}

void convergence_pass_destroy(void) {
    s_initialized = 0;
}

#endif /* __EMSCRIPTEN__ */
