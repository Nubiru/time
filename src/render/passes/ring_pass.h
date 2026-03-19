/*
 * ring_pass.h — Concentric knowledge system ring render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders concentric rings for multiple knowledge systems using
 * system_rings (data) + concentric_ring (geometry).
 */

#ifndef TIME_RING_PASS_H
#define TIME_RING_PASS_H

#ifdef __EMSCRIPTEN__

#include "../render_frame.h"

/* Compile shaders, create VAO/VBO/EBO for concentric rings.
 * Returns 0 on success, non-zero on failure. */
int ring_pass_init(void);

/* Draw concentric knowledge system rings in the ecliptic plane.
 * Uses system_rings for data, concentric_ring for geometry. */
void ring_pass_draw(const render_frame_t *frame);

/* Release GPU resources. */
void ring_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_RING_PASS_H */
