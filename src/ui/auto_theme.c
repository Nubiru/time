/* auto_theme.c — Time-of-day automatic theme selection.
 * Maps sun elevation to theme blend factor for smooth day/night transitions.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "auto_theme.h"
#include "../systems/earth/daylight.h"

auto_theme_state_t at_from_sun_elevation(double sun_elevation_deg)
{
    auto_theme_state_t s;
    s.sun_elevation_deg = sun_elevation_deg;

    if (sun_elevation_deg >= AT_SUN_DAWN_FULL) {
        s.blend_factor = 1.0f;
        s.active_theme = THEME_DAWN;
        s.is_transitioning = 0;
    } else if (sun_elevation_deg <= AT_SUN_COSMOS_FULL) {
        s.blend_factor = 0.0f;
        s.active_theme = THEME_COSMOS;
        s.is_transitioning = 0;
    } else {
        /* Linear interpolation in twilight zone: -6 to +6 degrees */
        double range = AT_SUN_DAWN_FULL - AT_SUN_COSMOS_FULL; /* 12.0 */
        double t = (sun_elevation_deg - AT_SUN_COSMOS_FULL) / range;
        s.blend_factor = (float)t;
        s.active_theme = (t >= 0.5) ? THEME_DAWN : THEME_COSMOS;
        s.is_transitioning = 1;
    }

    return s;
}

auto_theme_state_t at_from_location(double jd, double lat_deg, double lon_deg)
{
    daylight_t dl = daylight_at(jd, lat_deg, lon_deg);
    return at_from_sun_elevation(dl.sun_elevation_deg);
}

theme_t at_blended_theme(auto_theme_state_t state)
{
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    return theme_lerp(cosmos, dawn, state.blend_factor);
}

theme_t at_effective_theme(auto_theme_state_t state, int auto_enabled,
                            theme_id_t manual_theme)
{
    if (!auto_enabled) {
        return theme_get(manual_theme);
    }
    return at_blended_theme(state);
}

auto_theme_state_t at_smooth(auto_theme_state_t prev, auto_theme_state_t target,
                              float dt, float smooth_rate)
{
    auto_theme_state_t result = target;
    float speed = dt * smooth_rate;
    if (speed > 1.0f) {
        speed = 1.0f;
    }

    result.blend_factor = prev.blend_factor
                        + (target.blend_factor - prev.blend_factor) * speed;

    /* Clamp to [0, 1] — prevent overshoot from floating point */
    if (result.blend_factor < 0.0f) {
        result.blend_factor = 0.0f;
    }
    if (result.blend_factor > 1.0f) {
        result.blend_factor = 1.0f;
    }

    return result;
}
