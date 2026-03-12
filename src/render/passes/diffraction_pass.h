/*
 * diffraction_pass.h — Diffraction spike render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static variables.
 * Wires diffraction.h spike data into the render pipeline.
 * Adds 4-point diffraction spikes to the brightest stars.
 * init/draw/destroy pattern — no vtables.
 */

#ifndef TIME_DIFFRACTION_PASS_H
#define TIME_DIFFRACTION_PASS_H

#ifdef __EMSCRIPTEN__

#include "../render_frame.h"

/* Compile shaders, pack diffraction spikes for bright stars, upload to GPU.
 * Returns 0 on success, non-zero on failure. */
int diffraction_pass_init(void);

/* Draw diffraction spikes if LAYER_STARS is visible. */
void diffraction_pass_draw(const render_frame_t *frame);

/* Release GPU resources. */
void diffraction_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_DIFFRACTION_PASS_H */
