/*
 * text_pass.h — Text render pass (font atlas + glyph batches)
 *
 * Stateful module (S1): owns GPU resource handles.
 * Loads font bitmap atlas, renders batched text quads.
 */

#ifndef TIME_TEXT_PASS_H
#define TIME_TEXT_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

/* Compile shaders, create font atlas texture, setup VAO/VBO/EBO.
 * Returns 0 on success, non-zero on failure. */
int text_pass_init(void);

/* Draw text for current frame. */
void text_pass_draw(const render_frame_t *frame);

/* Release GPU resources. */
void text_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_TEXT_PASS_H */
