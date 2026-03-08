/*
 * planet_pass.h — Sun + planet sprites + orbit trail render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static variables.
 * init/draw/destroy pattern — no vtables.
 */

#ifndef TIME_PLANET_PASS_H
#define TIME_PLANET_PASS_H

#ifdef __EMSCRIPTEN__

#include "../render_frame.h"

/* Compile shaders, create meshes, set up VAOs/VBOs.
 * Returns 0 on success, non-zero on failure. */
int planet_pass_init(void);

/* Draw Sun sphere, planet sprites, and orbit trails. */
void planet_pass_draw(const render_frame_t *frame);

/* Release GPU resources. */
void planet_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_PLANET_PASS_H */
