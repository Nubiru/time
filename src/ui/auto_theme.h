/* auto_theme.h — Time-of-day automatic theme selection.
 *
 * Maps sun elevation at the observer's location to a theme choice:
 *   Sun well above horizon (> +6 deg): Dawn theme (light)
 *   Sun well below horizon (< -6 deg): Cosmos theme (dark)
 *   Transition zone (-6 deg to +6 deg): smooth blend between themes
 *
 * The transition zone corresponds roughly to civil twilight,
 * creating a natural visual shift as day transitions to night.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_AUTO_THEME_H
#define TIME_AUTO_THEME_H

#include "theme.h"

/* Sun elevation thresholds for theme transitions (degrees) */
#define AT_SUN_DAWN_FULL     6.0    /* above this: fully Dawn */
#define AT_SUN_COSMOS_FULL  -6.0    /* below this: fully Cosmos */

/* Auto-theme state */
typedef struct {
    theme_id_t active_theme;    /* current primary theme */
    float blend_factor;         /* 0.0 = Cosmos, 1.0 = Dawn */
    int is_transitioning;       /* 1 if in twilight zone */
    double sun_elevation_deg;   /* last computed sun elevation */
} auto_theme_state_t;

/* Compute theme state from sun elevation.
 * sun_elevation_deg: degrees above horizon (from daylight_at().sun_elevation_deg)
 * Returns auto_theme_state_t with blend_factor for theme_lerp(). */
auto_theme_state_t at_from_sun_elevation(double sun_elevation_deg);

/* Convenience: compute from JD + observer location.
 * Uses daylight_at() internally to get sun elevation. */
auto_theme_state_t at_from_location(double jd, double lat_deg, double lon_deg);

/* Get the blended theme for current state.
 * Applies theme_lerp(cosmos, dawn, blend_factor). */
theme_t at_blended_theme(auto_theme_state_t state);

/* Apply user override. If user manually selected a theme, return that.
 * auto_enabled: 1 = use auto-theme, 0 = use manual_theme.
 * Returns the effective theme. */
theme_t at_effective_theme(auto_theme_state_t state, int auto_enabled,
                            theme_id_t manual_theme);

/* Smooth the blend factor over time to avoid jarring transitions.
 * prev: previous frame's state.
 * target: new computed state.
 * dt: frame delta time in seconds.
 * smooth_rate: how fast to chase target (e.g., 2.0 = 2 seconds to full blend).
 * Returns smoothed state. */
auto_theme_state_t at_smooth(auto_theme_state_t prev, auto_theme_state_t target,
                              float dt, float smooth_rate);

#endif /* TIME_AUTO_THEME_H */
