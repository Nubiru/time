/*
 * aspect_lines_pass.h — Birth chart aspect lines render pass (S106)
 *
 * Renders colored lines between planets at birth moment positions.
 * Trine=blue (harmony), Square=red (friction), Opposition=red (tension),
 * Sextile=green (excitation), Conjunction=gold (fusion).
 * Only visible when birth sky mode is active (B key).
 */

#ifndef TIME_ASPECT_LINES_PASS_H
#define TIME_ASPECT_LINES_PASS_H

#include "../render_frame.h"

void aspect_lines_pass_init(void);
void aspect_lines_pass_draw(const render_frame_t *frame);
void aspect_lines_pass_destroy(void);

#endif
