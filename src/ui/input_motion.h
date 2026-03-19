#ifndef TIME_INPUT_MOTION_H
#define TIME_INPUT_MOTION_H

/* Spring-animated responses to discrete input events.
 * Provides smooth interpolation for values that change instantly
 * (speed changes, layer toggles, etc.)
 * Pure: no GL, no globals, no I/O. */

#include "../math/spring.h"

#define INPUT_MOTION_MAX_LAYERS 16

typedef struct {
    /* Speed spring: smooths time_speed transitions */
    spring_t speed_spring;

    /* Layer opacity springs: smooth toggle transitions */
    spring_t layer_springs[INPUT_MOTION_MAX_LAYERS];
    int layer_count;

    /* Pulse effect: brief visual emphasis on value change */
    float pulse;       /* current pulse intensity (0.0 to 1.0) */
    float pulse_decay; /* decay rate per second */
} input_motion_t;

/* Create with initial speed value and layer count */
input_motion_t input_motion_create(float initial_speed, int layer_count);

/* Set new target speed (spring animates to it) */
input_motion_t input_motion_set_speed(input_motion_t im, float new_speed);

/* Toggle layer visibility (spring to 0 or 1) */
input_motion_t input_motion_toggle_layer(input_motion_t im, int layer_id);

/* Set layer to specific opacity target */
input_motion_t input_motion_set_layer(input_motion_t im, int layer_id,
                                      float target);

/* Trigger a pulse effect (e.g., on date step or speed change) */
input_motion_t input_motion_pulse(input_motion_t im);

/* Advance all springs by dt */
input_motion_t input_motion_update(input_motion_t im, float dt);

/* Get current smooth speed value */
float input_motion_speed(input_motion_t im);

/* Get current smooth layer opacity (0.0 to 1.0) */
float input_motion_layer_opacity(input_motion_t im, int layer_id);

/* Get current pulse intensity (0.0 to 1.0) */
float input_motion_pulse_value(input_motion_t im);

/* Check if any animation is still running */
int input_motion_active(input_motion_t im);

#endif
