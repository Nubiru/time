#ifndef TIME_VEC_INTERP_H
#define TIME_VEC_INTERP_H

/* Vector interpolation and multi-component spring animation.
 * Pure: no GL, no globals, no I/O. */

#include "vec3.h"
#include "spring.h"

/* Linear interpolation of vec3 (t unclamped) */
vec3_t lerp_vec3(vec3_t a, vec3_t b, float t);

/* Linear interpolation of vec3 (t clamped to [0,1]) */
vec3_t lerp_vec3_clamped(vec3_t a, vec3_t b, float t);

/* Spherical linear interpolation for unit vectors */
vec3_t slerp_vec3(vec3_t a, vec3_t b, float t);

/* Float lerp (unclamped) */
float lerp_f(float a, float b, float t);

/* Float lerp (clamped to [0,1]) */
float lerp_f_clamped(float a, float b, float t);

/* Angle lerp in radians (shortest path through wrap) */
float lerp_angle(float a_rad, float b_rad, float t);

/* 3-component spring for animating 3D positions */
typedef struct {
    spring_t x, y, z;
} spring_vec3_t;

spring_vec3_t spring_vec3_create(vec3_t initial, vec3_t target,
                                 float stiffness, float damping);
spring_vec3_t spring_vec3_update(spring_vec3_t s, float dt);
spring_vec3_t spring_vec3_set_target(spring_vec3_t s, vec3_t target);
int           spring_vec3_settled(spring_vec3_t s, float epsilon);
vec3_t        spring_vec3_position(spring_vec3_t s);
vec3_t        spring_vec3_velocity(spring_vec3_t s);

#endif
