#include "render_layers.h"
#include "../ui/golden_layout.h"

/* Default layer configurations.
 * scale_min/max are scale_id_t values (0=Personal..6=Universe).
 * base_opacity is set to 1.0f as placeholder; actual values are
 * patched in layer_default_config() using gl_opacity_at().
 *
 * Layer visibility by scale:
 *   Stars:       Local(4) - Universe(6)    — deep sky background
 *   Galaxy:      Galaxy(5) - Universe(6)   — only when zoomed way out
 *   Orbits:      Inner(2) - Solar(3)       — orbital paths
 *   Planets:     Earth(1) - Local(4)       — planet spheres
 *   Zodiac ring: Inner(2) - Solar(3)       — ecliptic wheel
 *   Labels:      Inner(2) - Solar(3)       — text on planets/signs
 *   Cards:       Personal(0) - Earth(1)    — knowledge system panels
 *   HUD:         ALL scales                — always available
 */
static const layer_config_t DEFAULTS[LAYER_COUNT] = {
    { LAYER_STARS,       "Stars",       SCALE_LOCAL,    SCALE_UNIVERSE, 1.0f, 1 },
    { LAYER_GALAXY,      "Galaxy",      SCALE_GALAXY,   SCALE_UNIVERSE, 1.0f, 1 },
    { LAYER_ORBITS,      "Orbits",      SCALE_INNER,    SCALE_SOLAR,    1.0f, 1 },
    { LAYER_PLANETS,     "Planets",     SCALE_EARTH,    SCALE_LOCAL,    1.0f, 1 },
    { LAYER_ZODIAC_RING, "Zodiac Ring", SCALE_INNER,    SCALE_SOLAR,    1.0f, 1 },
    { LAYER_LABELS,      "Labels",      SCALE_INNER,    SCALE_SOLAR,    1.0f, 1 },
    { LAYER_CARDS,       "Cards",       SCALE_PERSONAL, SCALE_EARTH,    1.0f, 1 },
    { LAYER_HUD,         "HUD",         SCALE_PERSONAL, SCALE_UNIVERSE, 1.0f, 1 },
};

/* Opacity level per layer: 0 = full (1.0), 1 = phi^-1 (~0.618).
 * Background/secondary layers use level 1 for phi-cascade depth. */
static const int OPACITY_LEVELS[LAYER_COUNT] = {
    0, /* LAYER_STARS       — foreground: full */
    1, /* LAYER_GALAXY      — background: phi^-1 */
    1, /* LAYER_ORBITS      — supporting: phi^-1 */
    0, /* LAYER_PLANETS     — foreground: full */
    0, /* LAYER_ZODIAC_RING — foreground: full */
    0, /* LAYER_LABELS      — foreground: full */
    0, /* LAYER_CARDS       — foreground: full */
    0, /* LAYER_HUD         — foreground: full */
};

layer_config_t layer_default_config(layer_id_t id) {
    layer_config_t cfg;
    if (id >= 0 && id < LAYER_COUNT) {
        cfg = DEFAULTS[id];
    } else {
        cfg = DEFAULTS[LAYER_HUD];
    }
    /* Patch base_opacity from golden opacity cascade */
    int level = (cfg.id >= 0 && cfg.id < LAYER_COUNT)
                ? OPACITY_LEVELS[cfg.id] : 0;
    cfg.base_opacity = gl_opacity_at(level);
    return cfg;
}

float layer_opacity(layer_config_t config, float log_zoom) {
    if (!config.user_visible) return 0.0f;

    /* Get the scale boundaries for this layer */
    scale_level_t smin = scale_get(config.scale_min);
    scale_level_t smax = scale_get(config.scale_max);

    float range_min = smin.log_zoom_min;
    float range_max = smax.log_zoom_max;

    /* Fully outside range */
    if (log_zoom < range_min || log_zoom > range_max) {
        return 0.0f;
    }

    /* Fade zone: ramp over 20% of the total range at each edge */
    float total = range_max - range_min;
    float fade = total * 0.2f;
    if (fade < 0.5f) fade = 0.5f; /* minimum fade zone */

    float opacity = config.base_opacity;

    /* Fade in at low end */
    if (log_zoom < range_min + fade) {
        float t = (log_zoom - range_min) / fade;
        opacity *= t;
    }

    /* Fade out at high end */
    if (log_zoom > range_max - fade) {
        float t = (range_max - log_zoom) / fade;
        opacity *= t;
    }

    if (opacity < 0.0f) opacity = 0.0f;
    if (opacity > 1.0f) opacity = 1.0f;

    return opacity;
}

layer_state_t layers_compute(const layer_config_t configs[LAYER_COUNT],
                             float log_zoom) {
    layer_state_t state;
    for (int i = 0; i < LAYER_COUNT; i++) {
        state.opacity[i] = layer_opacity(configs[i], log_zoom);
    }
    return state;
}

const char *layer_name(layer_id_t id) {
    if (id >= 0 && id < LAYER_COUNT) {
        return DEFAULTS[id].name;
    }
    return "Unknown";
}

int layer_is_visible(layer_state_t state, layer_id_t id) {
    if (id < 0 || id >= LAYER_COUNT) return 0;
    return state.opacity[id] > 0.01f;
}

layer_state_t layer_override_opacity(layer_state_t state, layer_id_t id,
                                     float override_opacity) {
    if (id >= 0 && id < LAYER_COUNT && override_opacity >= 0.0f) {
        state.opacity[id] = override_opacity > 1.0f ? 1.0f : override_opacity;
    }
    return state;
}

layer_state_t layer_blend_opacity(layer_state_t state, layer_id_t id,
                                  float target_opacity, float mix) {
    if (id < 0 || id >= LAYER_COUNT) return state;
    if (mix < 0.0f) mix = 0.0f;
    if (mix > 1.0f) mix = 1.0f;
    if (target_opacity < 0.0f) target_opacity = 0.0f;
    if (target_opacity > 1.0f) target_opacity = 1.0f;
    state.opacity[id] = state.opacity[id] * (1.0f - mix) + target_opacity * mix;
    return state;
}
