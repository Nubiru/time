/*
 * tree_pass.h — Kabbalah Tree of Life render pass
 *
 * Stateful module (S1): owns GPU resource handles.
 * Renders 10 Sefirot nodes, 22 paths, and 3 pillars as 2D overlay.
 */

#ifndef TIME_TREE_PASS_H
#define TIME_TREE_PASS_H

#ifdef __EMSCRIPTEN__
#include "../render_frame.h"

int tree_pass_init(void);
void tree_pass_draw(const render_frame_t *frame);
void tree_pass_destroy(void);

#endif /* __EMSCRIPTEN__ */
#endif /* TIME_TREE_PASS_H */
