/*
 * hexagram_pass.h — I Ching hexagram render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders current hexagram as line geometry (yang/yin lines).
 */

#ifndef TIME_HEXAGRAM_PASS_H
#define TIME_HEXAGRAM_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int hexagram_pass_init(void);
void hexagram_pass_draw(const render_frame_t *frame);
void hexagram_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_HEXAGRAM_PASS_H */
