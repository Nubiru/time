/*
 * orbit_trail_pass.h — Orbit trail ellipse render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders Keplerian orbit ellipses for 8 planets as gradient-faded line strips.
 */

#ifndef TIME_ORBIT_TRAIL_PASS_H
#define TIME_ORBIT_TRAIL_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int orbit_trail_pass_init(void);
void orbit_trail_pass_draw(const render_frame_t *frame);
void orbit_trail_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_ORBIT_TRAIL_PASS_H */
