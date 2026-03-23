/*
 * wheel_pass.h — The Concentric Wheel of Time (THE central visual)
 *
 * This is not a feature. This is the ARCHITECTURE of Time's visual experience.
 * Every knowledge system is a ring. Every ring shows today.
 * The wheel is home. Everything else is a destination.
 *
 * PHI governs every proportion:
 *   Center: (W/2, H×φ⁻¹) — golden point, slightly above center
 *   Outer radius: min(W,H) × φ⁻¹ × 0.5
 *   Inner radius: outer × φ⁻¹
 *   Ring gap: ring_width × φ⁻¹
 *   Line weights: 3px → 2px → 1px (φ⁻¹ cascade)
 *   Animation: φ⁻¹ seconds per transition
 *   Center pulse: φ⁻¹ Hz
 *
 * "Every proportion: φ. Every line: golden. Every pixel: earned."
 */

#ifndef TIME_WHEEL_PASS_H
#define TIME_WHEEL_PASS_H

#include "../render_frame.h"

void wheel_pass_init(void);
void wheel_pass_draw(const render_frame_t *frame);
void wheel_pass_destroy(void);

#endif /* TIME_WHEEL_PASS_H */
