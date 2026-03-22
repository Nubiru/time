/*
 * stardust_pass.h — Procedural background star field render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders thousands of faint procedural stars on a fullscreen quad
 * using stardust_pack shader source. Additive blending.
 */

#ifndef TIME_STARDUST_PASS_H
#define TIME_STARDUST_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int  stardust_pass_init(void);
void stardust_pass_draw(const render_frame_t *frame);
void stardust_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_STARDUST_PASS_H */
