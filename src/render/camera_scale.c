#include "camera_scale.h"
#include "../math/easing.h"
#include <math.h>

/* Scale levels define log_zoom boundaries.
 * distance = exp(log_zoom), so log_zoom = ln(distance).
 *
 * Distance ranges (in scene units, where 1 unit ≈ 1 AU after sqrt scaling):
 *   Personal:  0.01 - 0.5    (chart view)
 *   Earth:     0.5  - 2.0    (Earth-Moon scale)
 *   Inner:     2.0  - 8.0    (inner planets + zodiac ring)
 *   Solar:     8.0  - 80.0   (full solar system — current default)
 *   Local:     80   - 500    (nearby stars)
 *   Galaxy:    500  - 5000   (galactic arm)
 *   Universe:  5000 - 50000  (cosmic scale)
 */

static const scale_level_t LEVELS[SCALE_COUNT] = {
    { SCALE_PERSONAL, "Personal Chart",
      -4.605f, -0.693f, -2.303f },      /* exp: 0.01 - 0.5, center 0.1 */
    { SCALE_EARTH, "Earth-Moon System",
      -0.693f, 0.693f, 0.0f },          /* exp: 0.5 - 2.0, center 1.0 */
    { SCALE_INNER, "Inner Solar System",
      0.693f, 2.079f, 1.386f },         /* exp: 2.0 - 8.0, center 4.0 */
    { SCALE_SOLAR, "Solar System",
      2.079f, 4.382f, 3.401f },         /* exp: 8.0 - 80.0, center 30.0 */
    { SCALE_LOCAL, "Solar Neighborhood",
      4.382f, 6.215f, 5.298f },         /* exp: 80 - 500, center 200 */
    { SCALE_GALAXY, "Milky Way",
      6.215f, 8.517f, 7.601f },         /* exp: 500 - 5000, center 2000 */
    { SCALE_UNIVERSE, "Observable Universe",
      8.517f, 10.820f, 9.903f },        /* exp: 5000 - 50000, center 20000 */
};

scale_level_t scale_get(scale_id_t id) {
    if (id >= 0 && id < SCALE_COUNT) {
        return LEVELS[id];
    }
    return LEVELS[SCALE_SOLAR]; /* default */
}

float scale_log_to_distance(float log_zoom) {
    return expf(log_zoom);
}

float scale_distance_to_log(float distance) {
    if (distance <= 0.0f) return LEVELS[0].log_zoom_min;
    return logf(distance);
}

scale_id_t scale_from_log_zoom(float log_zoom) {
    for (int i = SCALE_COUNT - 1; i >= 0; i--) {
        if (log_zoom >= LEVELS[i].log_zoom_min) {
            return LEVELS[i].id;
        }
    }
    return SCALE_PERSONAL;
}

const char *scale_name(scale_id_t id) {
    if (id >= 0 && id < SCALE_COUNT) {
        return LEVELS[id].name;
    }
    return "Unknown";
}

float scale_center_log_zoom(scale_id_t id) {
    if (id >= 0 && id < SCALE_COUNT) {
        return LEVELS[id].log_zoom_center;
    }
    return LEVELS[SCALE_SOLAR].log_zoom_center;
}

float scale_log_zoom_min(void) {
    return LEVELS[0].log_zoom_min;
}

float scale_log_zoom_max(void) {
    return LEVELS[SCALE_COUNT - 1].log_zoom_max;
}

/* --- Scale transitions --- */

scale_transition_t scale_transition_create(float current_log_zoom,
                                           scale_id_t target,
                                           float duration) {
    scale_transition_t t;
    t.from_log_zoom = current_log_zoom;
    t.to_log_zoom = scale_center_log_zoom(target);
    t.duration = duration > 0.0f ? duration : 0.001f;
    t.elapsed = 0.0f;
    t.active = 1;
    return t;
}

scale_transition_t scale_transition_tick(scale_transition_t t, float dt) {
    if (!t.active) return t;
    t.elapsed += dt;
    if (t.elapsed >= t.duration) {
        t.elapsed = t.duration;
        t.active = 0;
    }
    return t;
}

float scale_transition_value(scale_transition_t t) {
    if (!t.active && t.elapsed >= t.duration) {
        return t.to_log_zoom;
    }
    float progress = t.elapsed / t.duration;
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;
    double eased = ease_in_out_cubic((double)progress);
    return t.from_log_zoom + (float)eased * (t.to_log_zoom - t.from_log_zoom);
}

int scale_transition_done(scale_transition_t t) {
    return !t.active;
}
