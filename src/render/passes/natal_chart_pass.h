#ifndef TIME_NATAL_CHART_PASS_H
#define TIME_NATAL_CHART_PASS_H

/* natal_chart_pass.h — Natal chart wheel render pass (S121).
 *
 * Stateful module (S1): renders a traditional astrology natal chart
 * wheel with concentric rings, house cusps, planet positions, and
 * aspect lines. Activated by Astrology focus mode (A key).
 *
 * Visual design principles (from Jung on Astrology extraction):
 *   - "The horoscope is itself a mandala (a clock) with a dark centre"
 *   - Chart is a sacred circle of wholeness, not a data dashboard
 *   - Golden ratio proportions for ring sizing
 *   - Zodiac sign colors from color_theory.h element mapping
 *
 * Geometry:
 *   - Outer ring: 12 zodiac sign arcs (colored by element)
 *   - Inner ring: house boundary
 *   - Planet ring: 8 planet positions between inner and outer
 *   - Cusps: 12 radial lines from inner to outer ring
 *   - Aspects: colored lines between planets inside inner ring
 *     (Trine=blue, Square=red, Opposition=red, Sextile=green, Conjunction=gold)
 *   - Ascendant marker: emphasized cusp line at 9 o'clock
 *
 * Data flow:
 *   render_frame_t contains sun_lon, moon_lon, planet positions.
 *   natal_chart_compute() produces layout with screen coordinates.
 *   This pass renders the layout as GL geometry.
 */

#include "../render_frame.h"

/* Initialize GL resources (shaders, VAO, VBO). Call once. */
void natal_chart_pass_init(void);

/* Render the natal chart wheel.
 * frame: current render frame with planet data.
 * Only draws when Astrology focus mode is active AND overlay is requested. */
void natal_chart_pass_draw(const render_frame_t *frame);

/* Release GL resources. Call once at shutdown. */
void natal_chart_pass_destroy(void);

#endif /* TIME_NATAL_CHART_PASS_H */
