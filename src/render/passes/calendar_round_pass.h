/*
 * calendar_round_pass.h — Calendar Round mandala render pass (S104)
 *
 * Renders the 52-year Tzolkin×Haab cycle as a sacred wheel:
 * 52 sections showing year names, color-coded by seal family.
 * Current position highlighted. Gabriel's reference: IMG_5382.
 *
 * The Calendar Round = 18,980 days = 52 Haab years = 73 Tzolkin cycles.
 * After 52 years, the same Haab day falls on the same Tzolkin kin.
 */

#ifndef TIME_CALENDAR_ROUND_PASS_H
#define TIME_CALENDAR_ROUND_PASS_H

#include "../render_frame.h"

void calendar_round_pass_init(void);
void calendar_round_pass_draw(const render_frame_t *frame);
void calendar_round_pass_destroy(void);

#endif
