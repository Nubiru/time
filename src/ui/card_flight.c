#include "card_flight.h"

#include <math.h>

static const float LAYER_DEPTHS[CF_LAYER_COUNT] = {
    CF_DEPTH_COSMIC,    /* 0: Cosmic, Geological */
    CF_DEPTH_AGE,       /* 1: Buddhist, Hindu */
    CF_DEPTH_STRUCTURE, /* 2: Kabbalah, Human Design */
    CF_DEPTH_YEAR,      /* 3: Astrology, Chinese, Hebrew, Islamic */
    CF_DEPTH_CYCLE,     /* 4: I Ching, Korean, Japanese */
    CF_DEPTH_TODAY      /* 5: Gregorian, Tzolkin, Haab */
};

static const float SETTLE_EPSILON = 0.01f;

float cf_layer_depth(int layer_index)
{
    if (layer_index < 0 || layer_index >= CF_LAYER_COUNT) {
        return LAYER_DEPTHS[0];
    }
    return LAYER_DEPTHS[layer_index];
}

int cf_nearest_layer(float depth)
{
    int best = 0;
    float best_dist = fabsf(depth - LAYER_DEPTHS[0]);
    for (int i = 1; i < CF_LAYER_COUNT; i++) {
        float dist = fabsf(depth - LAYER_DEPTHS[i]);
        if (dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    return best;
}

int cf_layer_count(void)
{
    return CF_LAYER_COUNT;
}

card_flight_t cf_create(float initial_depth)
{
    int layer = cf_nearest_layer(initial_depth);
    card_flight_t cf;
    cf.zoom_spring = spring_create(initial_depth, initial_depth,
                                   CF_SPRING_STIFFNESS, CF_SPRING_DAMPING);
    cf.current_depth = initial_depth;
    cf.target_depth  = initial_depth;
    cf.start_depth   = initial_depth;
    cf.crossfade     = 0.0f;
    cf.target_layer  = layer;
    cf.active        = 0;
    return cf;
}

static card_flight_t cf_begin_flight(card_flight_t cf, int layer_index)
{
    float target = cf_layer_depth(layer_index);
    cf.start_depth   = cf.current_depth;
    cf.target_depth  = target;
    cf.target_layer  = layer_index;
    cf.crossfade     = 0.0f;
    cf.active        = 1;
    cf.zoom_spring   = spring_set_target(cf.zoom_spring, target);
    return cf;
}

card_flight_t cf_fly_deeper(card_flight_t cf)
{
    int current = cf_nearest_layer(cf.current_depth);
    if (current >= CF_LAYER_COUNT - 1) {
        return cf;
    }
    return cf_begin_flight(cf, current + 1);
}

card_flight_t cf_fly_shallower(card_flight_t cf)
{
    int current = cf_nearest_layer(cf.current_depth);
    if (current <= 0) {
        return cf;
    }
    return cf_begin_flight(cf, current - 1);
}

card_flight_t cf_fly_to_layer(card_flight_t cf, int layer_index)
{
    if (layer_index < 0) {
        layer_index = 0;
    }
    if (layer_index >= CF_LAYER_COUNT) {
        layer_index = CF_LAYER_COUNT - 1;
    }
    return cf_begin_flight(cf, layer_index);
}

card_flight_t cf_fly_to_depth(card_flight_t cf, float depth)
{
    int layer = cf_nearest_layer(depth);
    return cf_fly_to_layer(cf, layer);
}

card_flight_t cf_tick(card_flight_t cf, float dt)
{
    if (!cf.active) {
        return cf;
    }

    cf.zoom_spring  = spring_update(cf.zoom_spring, dt);
    cf.current_depth = cf.zoom_spring.position;

    /* Ramp crossfade toward 1.0 */
    cf.crossfade += CF_CROSSFADE_SPEED * dt;
    if (cf.crossfade > 1.0f) {
        cf.crossfade = 1.0f;
    }

    /* Deactivate when spring settled and crossfade complete */
    if (spring_settled(cf.zoom_spring, SETTLE_EPSILON)
        && cf.crossfade >= 1.0f) {
        cf.active = 0;
    }

    return cf;
}

card_flight_t cf_cancel(card_flight_t cf)
{
    int nearest = cf_nearest_layer(cf.current_depth);
    float target = cf_layer_depth(nearest);
    cf.target_depth = target;
    cf.target_layer = nearest;
    cf.zoom_spring  = spring_set_target(cf.zoom_spring, target);
    /* Stay active until spring settles naturally */
    cf.active = 1;
    return cf;
}

float cf_depth(card_flight_t cf)
{
    return cf.current_depth;
}

float cf_target(card_flight_t cf)
{
    return cf.target_depth;
}

float cf_crossfade(card_flight_t cf)
{
    return cf.crossfade;
}

int cf_active(card_flight_t cf)
{
    return cf.active;
}

int cf_current_layer(card_flight_t cf)
{
    return cf_nearest_layer(cf.current_depth);
}

int cf_target_layer(card_flight_t cf)
{
    return cf.target_layer;
}

float cf_zoom_offset(card_flight_t cf)
{
    if (!cf.active) {
        return 0.0f;
    }
    return cf.zoom_spring.velocity * CF_ZOOM_VELOCITY_SCALE;
}
