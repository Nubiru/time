#ifndef TIME_CARD_FLIGHT_H
#define TIME_CARD_FLIGHT_H

#include "../math/spring.h"

/* Canonical depth stops (6 information layers) */
#define CF_LAYER_COUNT     6
#define CF_DEPTH_COSMIC    0.5f
#define CF_DEPTH_AGE       1.5f
#define CF_DEPTH_STRUCTURE 2.5f
#define CF_DEPTH_YEAR      3.5f
#define CF_DEPTH_CYCLE     4.5f
#define CF_DEPTH_TODAY     5.5f

/* Spring parameters for zoom animation */
#define CF_SPRING_STIFFNESS 250.0f
#define CF_SPRING_DAMPING   22.0f

/* Crossfade timing: how fast old cards fade out / new fade in */
#define CF_CROSSFADE_SPEED  3.0f   /* per second */

typedef struct {
    spring_t zoom_spring;    /* animates zoom level between layers */
    float current_depth;     /* current zoom depth (from spring position) */
    float target_depth;      /* target depth layer */
    float start_depth;       /* depth when flight began */
    float crossfade;         /* 0.0 = at start cards, 1.0 = at target cards */
    int target_layer;        /* index into layer array (0-5) */
    int active;              /* 1 if flight in progress */
} card_flight_t;

/* Create at a given initial zoom depth */
card_flight_t cf_create(float initial_depth);

/* Fly to the next deeper layer (higher depth value).
 * Returns unchanged if already at deepest layer. */
card_flight_t cf_fly_deeper(card_flight_t cf);

/* Fly to the next shallower layer (lower depth value).
 * Returns unchanged if already at shallowest layer. */
card_flight_t cf_fly_shallower(card_flight_t cf);

/* Fly to a specific depth layer by index (0=cosmic, 5=today).
 * Clamped to valid range. */
card_flight_t cf_fly_to_layer(card_flight_t cf, int layer_index);

/* Fly to an exact depth value (finds nearest layer). */
card_flight_t cf_fly_to_depth(card_flight_t cf, float depth);

/* Advance by dt seconds */
card_flight_t cf_tick(card_flight_t cf, float dt);

/* Cancel flight (snap to nearest layer) */
card_flight_t cf_cancel(card_flight_t cf);

/* Current zoom depth (spring-animated) */
float cf_depth(card_flight_t cf);

/* Target depth */
float cf_target(card_flight_t cf);

/* Crossfade blend: 0.0 = showing start-layer cards, 1.0 = showing target-layer cards.
 * Use this to blend card opacity between layers. */
float cf_crossfade(card_flight_t cf);

/* 1 if flight in progress */
int cf_active(card_flight_t cf);

/* Current layer index (nearest to current depth, 0-5) */
int cf_current_layer(card_flight_t cf);

/* Target layer index (0-5) */
int cf_target_layer(card_flight_t cf);

/* Get the canonical depth for a layer index.
 * Returns 0.5 for out-of-range. */
float cf_layer_depth(int layer_index);

/* Find the nearest layer index for an arbitrary depth.
 * Returns 0-5. */
int cf_nearest_layer(float depth);

/* Get layer count (always CF_LAYER_COUNT = 6) */
int cf_layer_count(void);

#endif /* TIME_CARD_FLIGHT_H */
