#include "mat4.h"
#include <math.h>
#include <string.h>

/* Helper: zero out a matrix */
static mat4_t mat4_zero(void) {
    mat4_t r;
    memset(r.m, 0, sizeof(r.m));
    return r;
}

mat4_t mat4_identity(void) {
    mat4_t r = mat4_zero();
    r.m[0]  = 1.0f;
    r.m[5]  = 1.0f;
    r.m[10] = 1.0f;
    r.m[15] = 1.0f;
    return r;
}

/* Column-major: element at (row, col) = m[col * 4 + row] */
mat4_t mat4_multiply(mat4_t a, mat4_t b) {
    mat4_t r = mat4_zero();
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            r.m[col * 4 + row] = sum;
        }
    }
    return r;
}

mat4_t mat4_translate(float tx, float ty, float tz) {
    mat4_t r = mat4_identity();
    r.m[12] = tx;
    r.m[13] = ty;
    r.m[14] = tz;
    return r;
}

mat4_t mat4_scale(float sx, float sy, float sz) {
    mat4_t r = mat4_zero();
    r.m[0]  = sx;
    r.m[5]  = sy;
    r.m[10] = sz;
    r.m[15] = 1.0f;
    return r;
}

mat4_t mat4_rotate_x(float radians) {
    float c = cosf(radians);
    float s = sinf(radians);
    mat4_t r = mat4_identity();
    r.m[5]  =  c;
    r.m[6]  =  s;
    r.m[9]  = -s;
    r.m[10] =  c;
    return r;
}

mat4_t mat4_rotate_y(float radians) {
    float c = cosf(radians);
    float s = sinf(radians);
    mat4_t r = mat4_identity();
    r.m[0]  =  c;
    r.m[2]  = -s;
    r.m[8]  =  s;
    r.m[10] =  c;
    return r;
}

mat4_t mat4_rotate_z(float radians) {
    float c = cosf(radians);
    float s = sinf(radians);
    mat4_t r = mat4_identity();
    r.m[0] =  c;
    r.m[1] =  s;
    r.m[4] = -s;
    r.m[5] =  c;
    return r;
}

mat4_t mat4_perspective(float fov_radians, float aspect, float near, float far) {
    float f = 1.0f / tanf(fov_radians / 2.0f);
    float range_inv = 1.0f / (near - far);
    mat4_t r = mat4_zero();
    r.m[0]  = f / aspect;
    r.m[5]  = f;
    r.m[10] = (near + far) * range_inv;
    r.m[11] = -1.0f;
    r.m[14] = 2.0f * near * far * range_inv;
    return r;
}

mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up) {
    vec3_t f = vec3_normalize(vec3_sub(center, eye));
    vec3_t s = vec3_normalize(vec3_cross(f, up));
    vec3_t u = vec3_cross(s, f);

    mat4_t r = mat4_identity();
    r.m[0]  =  s.x;
    r.m[4]  =  s.y;
    r.m[8]  =  s.z;
    r.m[1]  =  u.x;
    r.m[5]  =  u.y;
    r.m[9]  =  u.z;
    r.m[2]  = -f.x;
    r.m[6]  = -f.y;
    r.m[10] = -f.z;
    r.m[12] = -vec3_dot(s, eye);
    r.m[13] = -vec3_dot(u, eye);
    r.m[14] =  vec3_dot(f, eye);
    return r;
}
