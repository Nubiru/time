/*
 * convergence_pass.h — Convergence connection line render pass
 *
 * Stateful module (S1): renders connection lines between concentric
 * knowledge system rings when brain convergence is detected.
 * Uses convergence_visual.h for line geometry.
 */

#ifndef TIME_CONVERGENCE_PASS_H
#define TIME_CONVERGENCE_PASS_H

#ifdef __EMSCRIPTEN__

#include "../render_frame.h"

int  convergence_pass_init(void);
void convergence_pass_draw(const render_frame_t *frame);
void convergence_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_CONVERGENCE_PASS_H */
