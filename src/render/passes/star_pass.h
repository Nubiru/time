/*
 * star_pass.h — Star field + constellation line render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static variables.
 * init/draw/destroy pattern — no vtables.
 */

#ifndef TIME_STAR_PASS_H
#define TIME_STAR_PASS_H

#ifdef __EMSCRIPTEN__

#include "../render_frame.h"

/* Compile shaders, upload star + constellation data to GPU.
 * Returns 0 on success, non-zero on failure. */
int star_pass_init(void);

/* Draw stars and constellation lines if LAYER_STARS is visible. */
void star_pass_draw(const render_frame_t *frame);

/* Release GPU resources. */
void star_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_STAR_PASS_H */
