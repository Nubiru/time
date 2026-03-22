/*
 * sky_pass.h — Preetham sky dome render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders physically-based sky color in Earth View using Preetham model.
 * Fullscreen quad, fragment-computed sky color from sun position.
 */

#ifndef TIME_SKY_PASS_H
#define TIME_SKY_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int  sky_pass_init(void);
void sky_pass_draw(const render_frame_t *frame);
void sky_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_SKY_PASS_H */
