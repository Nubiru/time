/*
 * zodiac_pass.h — Zodiac ring + markers + aspects + cusps + glyphs render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static variables.
 * init/draw/destroy pattern — no vtables.
 */

#ifndef TIME_ZODIAC_PASS_H
#define TIME_ZODIAC_PASS_H

#ifdef __EMSCRIPTEN__

#include "../render_frame.h"

/* Compile shaders, generate ring mesh, set up VAOs/VBOs/textures.
 * Returns 0 on success, non-zero on failure. */
int zodiac_pass_init(void);

/* Draw zodiac ring, planet markers, aspect lines, cusp lines, sign glyphs. */
void zodiac_pass_draw(const render_frame_t *frame);

/* Release GPU resources. */
void zodiac_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_ZODIAC_PASS_H */
