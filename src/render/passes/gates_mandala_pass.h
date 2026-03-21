/*
 * gates_mandala_pass.h — Sefer Yetzirah 231 Gates mandala (S125)
 *
 * Renders 22 points (Hebrew letters) on a circle, connected by
 * C(22,2) = 231 lines — every possible pair. The resulting pattern
 * is sacred geometry: the complete graph K₂₂.
 *
 * "With 22 foundation letters He engraved, carved, permuted,
 *  weighed, transformed, and with them depicted all that was
 *  formed and all that would be formed." — Sefer Yetzirah 2:2
 *
 * Visual design:
 *   - 22 points as golden nodes, sized by letter classification
 *   - 3 Mother letters: largest nodes, red/blue/yellow (Fire/Water/Air)
 *   - 7 Double letters: medium nodes, planetary colors
 *   - 12 Simple letters: smallest nodes, zodiac colors
 *   - 231 connecting lines: very faint golden, brighter for active paths
 *   - Active path = today's Hebrew letter → its connections glow
 */

#ifndef TIME_GATES_MANDALA_PASS_H
#define TIME_GATES_MANDALA_PASS_H

#include "../render_frame.h"

void gates_mandala_pass_init(void);
void gates_mandala_pass_draw(const render_frame_t *frame);
void gates_mandala_pass_destroy(void);

#endif /* TIME_GATES_MANDALA_PASS_H */
