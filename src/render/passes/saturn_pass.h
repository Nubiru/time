/*
 * saturn_pass.h — Saturn ring system render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders Saturn's ring system as a lit, textured annular disc.
 */

#ifndef TIME_SATURN_PASS_H
#define TIME_SATURN_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int saturn_pass_init(void);
void saturn_pass_draw(const render_frame_t *frame);
void saturn_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_SATURN_PASS_H */
