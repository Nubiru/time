/*
 * post_pass.h — Post-processing FBO pipeline render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Wraps the entire scene render with bloom, tone mapping, vignette, and grain.
 *
 * Usage pattern:
 *   post_pass_begin(&frame);   // bind scene FBO
 *   ... draw all passes ...
 *   post_pass_end(&frame);     // apply effects, composite to screen
 */

#ifndef TIME_POST_PASS_H
#define TIME_POST_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

/* Initialize FBO pipeline (4 FBOs, 3 shader programs, fullscreen quad).
 * width/height: initial framebuffer dimensions. */
int post_pass_init(int width, int height);

/* Bind scene FBO. Call BEFORE all other pass draws. */
void post_pass_begin(const render_frame_t *frame);

/* Apply bloom + tonemap + vignette + grain, composite to default framebuffer.
 * Call AFTER all other pass draws. */
void post_pass_end(const render_frame_t *frame);

/* Resize FBOs when canvas size changes. */
void post_pass_resize(int width, int height);

void post_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_POST_PASS_H */
