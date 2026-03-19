#ifndef TIME_SCALE_SPRING_H
#define TIME_SCALE_SPRING_H

/* Spring-based scale transitions with anticipation and follow-through.
 * Alternative to scale_transition_t that uses physics instead of easing curves.
 * Pure: no GL, no globals, no I/O. */

#include "../math/spring.h"
#include "camera_scale.h"

typedef enum {
    SS_IDLE,      /* no transition active */
    SS_ANTICIPATE, /* brief counter-move */
    SS_SPRING,    /* main spring transition */
    SS_SETTLED    /* arrived (spring settled) */
} scale_spring_phase_e;

typedef struct {
    spring_t spring;              /* drives log_zoom */
    scale_spring_phase_e phase;
    float anticipation_timer;     /* countdown for anticipation phase */
    float anticipation_amount;    /* how far to counter-move (log_zoom units) */
    float anticipation_duration;  /* seconds for anticipation phase */
    float final_target;           /* the real destination */
    scale_id_t target_scale;      /* which scale we're heading to */
} scale_spring_t;

/* Create an idle (no transition) state at current position */
scale_spring_t scale_spring_create(float current_log_zoom);

/* Start transition to target scale level.
 * anticipation_frac: fraction of distance to counter-move (0.05 = 5%)
 * anticipation_dur: seconds for anticipation phase (0.1 typical) */
scale_spring_t scale_spring_start(scale_spring_t ss,
                                  float current_log_zoom,
                                  scale_id_t target,
                                  float anticipation_frac,
                                  float anticipation_dur);

/* Start with default anticipation (5%, 0.1s) */
scale_spring_t scale_spring_start_default(scale_spring_t ss,
                                          float current_log_zoom,
                                          scale_id_t target);

/* Advance by dt. Returns updated state. */
scale_spring_t scale_spring_tick(scale_spring_t ss, float dt);

/* Get current log_zoom value */
float scale_spring_value(scale_spring_t ss);

/* Is transition complete (settled)? */
int scale_spring_done(scale_spring_t ss);

/* Is transition active (not idle, not settled)? */
int scale_spring_active(scale_spring_t ss);

/* Get current phase */
scale_spring_phase_e scale_spring_phase(scale_spring_t ss);

/* Cancel transition -- snap to wherever spring currently is */
scale_spring_t scale_spring_cancel(scale_spring_t ss);

#endif
