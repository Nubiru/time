#include "input_motion.h"

#include <math.h>

static const float SETTLE_EPSILON     = 0.001f;
static const float DEFAULT_PULSE_DECAY = 8.0f;

input_motion_t input_motion_create(float initial_speed, int layer_count)
{
    input_motion_t im = {0};

    im.speed_spring = spring_create(initial_speed, initial_speed,
                                    SPRING_GENTLE);
    im.layer_count = layer_count;
    if (im.layer_count > INPUT_MOTION_MAX_LAYERS) {
        im.layer_count = INPUT_MOTION_MAX_LAYERS;
    }
    if (im.layer_count < 0) {
        im.layer_count = 0;
    }

    for (int i = 0; i < im.layer_count; i++) {
        im.layer_springs[i] = spring_create(1.0f, 1.0f, SPRING_SNAPPY);
    }

    im.pulse       = 0.0f;
    im.pulse_decay = DEFAULT_PULSE_DECAY;
    return im;
}

input_motion_t input_motion_set_speed(input_motion_t im, float new_speed)
{
    im.speed_spring = spring_set_target(im.speed_spring, new_speed);
    return im;
}

input_motion_t input_motion_toggle_layer(input_motion_t im, int layer_id)
{
    if (layer_id < 0 || layer_id >= im.layer_count) {
        return im;
    }
    float current_target = im.layer_springs[layer_id].target;
    float new_target     = (current_target > 0.5f) ? 0.0f : 1.0f;
    im.layer_springs[layer_id] =
        spring_set_target(im.layer_springs[layer_id], new_target);
    return im;
}

input_motion_t input_motion_set_layer(input_motion_t im, int layer_id,
                                      float target)
{
    if (layer_id < 0 || layer_id >= im.layer_count) {
        return im;
    }
    im.layer_springs[layer_id] =
        spring_set_target(im.layer_springs[layer_id], target);
    return im;
}

input_motion_t input_motion_pulse(input_motion_t im)
{
    im.pulse = 1.0f;
    return im;
}

input_motion_t input_motion_update(input_motion_t im, float dt)
{
    if (dt <= 0.0f) {
        return im;
    }

    im.speed_spring = spring_update(im.speed_spring, dt);

    for (int i = 0; i < im.layer_count; i++) {
        im.layer_springs[i] = spring_update(im.layer_springs[i], dt);
    }

    /* Exponential pulse decay */
    if (im.pulse > 0.0f) {
        im.pulse *= expf(-im.pulse_decay * dt);
        if (im.pulse < 0.001f) {
            im.pulse = 0.0f;
        }
    }

    return im;
}

float input_motion_speed(input_motion_t im)
{
    return im.speed_spring.position;
}

float input_motion_layer_opacity(input_motion_t im, int layer_id)
{
    if (layer_id < 0 || layer_id >= im.layer_count) {
        return 0.0f;
    }
    float v = im.layer_springs[layer_id].position;
    if (v < 0.0f) {
        return 0.0f;
    }
    if (v > 1.0f) {
        return 1.0f;
    }
    return v;
}

float input_motion_pulse_value(input_motion_t im)
{
    return im.pulse;
}

int input_motion_active(input_motion_t im)
{
    if (!spring_settled(im.speed_spring, SETTLE_EPSILON)) {
        return 1;
    }
    for (int i = 0; i < im.layer_count; i++) {
        if (!spring_settled(im.layer_springs[i], SETTLE_EPSILON)) {
            return 1;
        }
    }
    if (im.pulse > 0.001f) {
        return 1;
    }
    return 0;
}
