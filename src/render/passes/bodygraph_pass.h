/*
 * bodygraph_pass.h — Human Design bodygraph render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders 9 energy centers, 36 channels, and 64 gates as 2D overlay.
 */

#ifndef TIME_BODYGRAPH_PASS_H
#define TIME_BODYGRAPH_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int bodygraph_pass_init(void);
void bodygraph_pass_draw(const render_frame_t *frame);
void bodygraph_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_BODYGRAPH_PASS_H */
