/*
 * earth_pass.h — Earth globe render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders Earth globe with day/night shading, coastlines,
 * terminator, and atmosphere glow.
 */

#ifndef TIME_EARTH_PASS_H
#define TIME_EARTH_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int earth_pass_init(void);
void earth_pass_draw(const render_frame_t *frame);
void earth_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_EARTH_PASS_H */
