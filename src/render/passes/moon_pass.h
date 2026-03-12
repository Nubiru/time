/*
 * moon_pass.h — Major moons render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders 9 major moons (Earth:1, Mars:2, Jupiter:4, Saturn:2)
 * as point sprites with orbit trails.
 */

#ifndef TIME_MOON_PASS_H
#define TIME_MOON_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int moon_pass_init(void);
void moon_pass_draw(const render_frame_t *frame);
void moon_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_MOON_PASS_H */
