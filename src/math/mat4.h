#ifndef TIME_MAT4_H
#define TIME_MAT4_H

#include "vec3.h"

/* Column-major 4x4 matrix (OpenGL convention).
 * m[col][row] — m[0] is the first column, etc.
 * Stored as flat array: m[0..3] = col0, m[4..7] = col1, ... */
typedef struct {
    float m[16];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_multiply(mat4_t a, mat4_t b);
mat4_t mat4_translate(float tx, float ty, float tz);
mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_rotate_x(float radians);
mat4_t mat4_rotate_y(float radians);
mat4_t mat4_rotate_z(float radians);
mat4_t mat4_perspective(float fov_radians, float aspect, float near, float far);
mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up);

#endif
