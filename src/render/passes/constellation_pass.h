/*
 * constellation_pass.h — IAU constellation boundary render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders 88 IAU constellation boundaries as GL_LINES.
 */

#ifndef TIME_CONSTELLATION_PASS_H
#define TIME_CONSTELLATION_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int constellation_pass_init(void);
void constellation_pass_draw(const render_frame_t *frame);
void constellation_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_CONSTELLATION_PASS_H */
