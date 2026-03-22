/*
 * diamond_room_pass.h — Diamond Room wireframe render pass (E7)
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders the 20-face icosahedron wireframe with per-edge glow
 * from knowledge system activity. Visible at overview zoom.
 */

#ifndef TIME_DIAMOND_ROOM_PASS_H
#define TIME_DIAMOND_ROOM_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int  diamond_room_pass_init(void);
void diamond_room_pass_draw(const render_frame_t *frame);
void diamond_room_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_DIAMOND_ROOM_PASS_H */
