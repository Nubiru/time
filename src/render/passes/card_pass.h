/*
 * card_pass.h — Knowledge system info card render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders card backgrounds and borders as 2D screen-space overlay.
 */

#ifndef TIME_CARD_PASS_H
#define TIME_CARD_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int card_pass_init(void);
void card_pass_draw(const render_frame_t *frame);
void card_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_CARD_PASS_H */
