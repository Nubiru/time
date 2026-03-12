/*
 * milkyway_pass.h — Milky Way galaxy band render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders procedural Milky Way band on the celestial sphere.
 */

#ifndef TIME_MILKYWAY_PASS_H
#define TIME_MILKYWAY_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int milkyway_pass_init(void);
void milkyway_pass_draw(const render_frame_t *frame);
void milkyway_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_MILKYWAY_PASS_H */
