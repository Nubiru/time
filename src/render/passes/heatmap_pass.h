/*
 * heatmap_pass.h — Convergence heatmap render pass (S88)
 *
 * Renders a 365×N year grid showing convergence intensity per day.
 * X-axis = day of year (1-365). Y-axis = year (configurable range).
 * Color = convergence strength (dark → warm gold → white hot).
 * Reveals: Islamic diagonal drift, Hebrew Metonic wobble, seasonal
 * patterns, and the rare mega-convergences.
 *
 * Uses decade_heatmap.h data (12yr × 366day grid, 20 tests).
 */

#ifndef TIME_HEATMAP_PASS_H
#define TIME_HEATMAP_PASS_H

#include "../render_frame.h"

void heatmap_pass_init(void);
void heatmap_pass_draw(const render_frame_t *frame);
void heatmap_pass_destroy(void);

#endif
