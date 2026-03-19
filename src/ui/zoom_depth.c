#include "zoom_depth.h"
#include <math.h>

static const float TIER_SETTLE_EPS = 0.05f;

zoom_depth_t zoom_depth_create(float log_zoom, int system_id)
{
    zoom_depth_t zd;

    zd.scale       = scale_from_log_zoom(log_zoom);
    zd.system_id   = system_id;
    zd.target_tier = depth_tier_for_scale((int)zd.scale, system_id, -1);
    zd.tier        = zd.target_tier;

    /* Spring starts at target (no transition on creation) */
    float tier_val = (float)zd.target_tier;
    zd.tier_spring = spring_create(tier_val, tier_val, SPRING_GENTLE);

    if (system_id >= 0 && system_id < (int)TS_SYS_COUNT) {
        zd.ring = depth_ring_for_system(system_id);
    } else {
        zd.ring = DEPTH_RING_TODAY;
    }

    return zd;
}

zoom_depth_t zoom_depth_update(zoom_depth_t zd, float log_zoom, float dt)
{
    if (dt <= 0.0f) {
        return zd;
    }

    scale_id_t new_scale = scale_from_log_zoom(log_zoom);
    depth_tier_t new_tier = depth_tier_for_scale((int)new_scale, zd.system_id, -1);

    zd.scale = new_scale;

    if (new_tier != zd.target_tier) {
        zd.target_tier = new_tier;
        zd.tier_spring = spring_set_target(zd.tier_spring, (float)new_tier);
    }

    zd.tier_spring = spring_update(zd.tier_spring, dt);

    /* Discrete tier from rounded spring position */
    float blend = zd.tier_spring.position;
    if (blend < 0.0f) { blend = 0.0f; }
    if (blend > (float)(DEPTH_TIER_COUNT - 1)) {
        blend = (float)(DEPTH_TIER_COUNT - 1);
    }
    zd.tier = (depth_tier_t)(int)(blend + 0.5f);

    return zd;
}

zoom_depth_t zoom_depth_focus(zoom_depth_t zd, int system_id)
{
    zd.system_id = system_id;

    if (system_id >= 0 && system_id < (int)TS_SYS_COUNT) {
        zd.ring = depth_ring_for_system(system_id);
    } else {
        zd.ring = DEPTH_RING_TODAY;
    }

    /* Recalculate target tier for new system */
    depth_tier_t new_tier = depth_tier_for_scale((int)zd.scale, system_id, -1);
    if (new_tier != zd.target_tier) {
        zd.target_tier = new_tier;
        zd.tier_spring = spring_set_target(zd.tier_spring, (float)new_tier);
    }

    return zd;
}

depth_tier_t zoom_depth_tier(zoom_depth_t zd)
{
    return zd.tier;
}

float zoom_depth_blend(zoom_depth_t zd)
{
    float v = zd.tier_spring.position;
    if (v < 0.0f) { return 0.0f; }
    if (v > (float)(DEPTH_TIER_COUNT - 1)) {
        return (float)(DEPTH_TIER_COUNT - 1);
    }
    return v;
}

depth_ring_t zoom_depth_ring(zoom_depth_t zd)
{
    return zd.ring;
}

int zoom_depth_transitioning(zoom_depth_t zd)
{
    return !spring_settled(zd.tier_spring, TIER_SETTLE_EPS);
}
