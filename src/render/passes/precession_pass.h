/*
 * precession_pass.h — Precession of the Equinoxes clock (S90)
 *
 * Renders a circular clock showing position in the 25,772-year
 * Great Year (axial precession cycle). Current Age highlighted.
 * 12 astrological ages around the wheel.
 */

#ifndef TIME_PRECESSION_PASS_H
#define TIME_PRECESSION_PASS_H

#include "../render_frame.h"

void precession_pass_init(void);
void precession_pass_draw(const render_frame_t *frame);
void precession_pass_destroy(void);

#endif
