/*
 * deep_sky_pass.h — Deep sky object billboard render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders 110 Messier catalog DSOs as camera-facing billboards.
 */

#ifndef TIME_DEEP_SKY_PASS_H
#define TIME_DEEP_SKY_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int deep_sky_pass_init(void);
void deep_sky_pass_draw(const render_frame_t *frame);
void deep_sky_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_DEEP_SKY_PASS_H */
