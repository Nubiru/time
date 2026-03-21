/*
 * bagua_pass.h — I Ching Bagua (8 trigrams) circular render pass (S124)
 *
 * Renders the Later Heaven (King Wen) arrangement of 8 trigrams
 * in a circle, with the current hexagram's two trigrams highlighted.
 *
 * Each trigram rendered as 3 horizontal lines (solid or broken)
 * arranged radially around the circle. Ink-wash aesthetic per
 * art direction (NOT neon — warm ivory on dark, contemplative).
 */

#ifndef TIME_BAGUA_PASS_H
#define TIME_BAGUA_PASS_H

#include "../render_frame.h"

void bagua_pass_init(void);
void bagua_pass_draw(const render_frame_t *frame);
void bagua_pass_destroy(void);

#endif /* TIME_BAGUA_PASS_H */
