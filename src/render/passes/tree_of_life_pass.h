/*
 * tree_of_life_pass.h — Kabbalah Tree of Life render pass (S123)
 *
 * Stateful module (S1): renders the 10 Sefirot as glowing nodes with
 * 22 connecting paths colored by letter type (Mother/Double/Simple).
 * Activated when Kabbalah system is focused.
 *
 * Visual design (from Sefer Yetzirah extraction):
 *   - 3 Mother paths (horizontal): red/blue/yellow (Fire/Water/Air)
 *   - 7 Double paths: planetary colors (Saturn=lead, Jupiter=violet, etc.)
 *   - 12 Simple paths: zodiac sign colors
 *   - Defined Sefirot glow brighter
 *   - Da'at (hidden 11th) rendered as dotted outline
 *   - 231 Gates mode: all C(22,2) connections as faint golden lines
 */

#ifndef TIME_TREE_OF_LIFE_PASS_H
#define TIME_TREE_OF_LIFE_PASS_H

#include "../render_frame.h"

void tree_of_life_pass_init(void);
void tree_of_life_pass_draw(const render_frame_t *frame);
void tree_of_life_pass_destroy(void);

#endif /* TIME_TREE_OF_LIFE_PASS_H */
