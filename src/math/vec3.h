#ifndef TIME_VEC3_H
#define TIME_VEC3_H

typedef struct {
    float x, y, z;
} vec3_t;

vec3_t vec3_create(float x, float y, float z);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_scale(vec3_t v, float s);
float  vec3_dot(vec3_t a, vec3_t b);
vec3_t vec3_cross(vec3_t a, vec3_t b);
float  vec3_length(vec3_t v);
vec3_t vec3_normalize(vec3_t v);

#endif
