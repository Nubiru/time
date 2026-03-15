/*
 * shared_uniforms.c — UBO layout constants, GLSL block declaration, data packing
 *
 * Provides pure data functions for building a std140-layout UBO buffer
 * containing view, projection, view*proj matrices and animation time.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 * Uses only string.h (memcpy, memset).
 */

#include "shared_uniforms.h"
#include <string.h>

/* -----------------------------------------------------------------------
 * su_block_declaration — GLSL uniform block string
 * ----------------------------------------------------------------------- */

static const char s_block_decl[] =
    "layout(std140) uniform SharedData {"
    " mat4 u_view;"
    " mat4 u_proj;"
    " mat4 u_view_proj;"
    " float u_time;"
    " };\n";

const char *su_block_declaration(void)
{
    return s_block_decl;
}

/* -----------------------------------------------------------------------
 * su_pack — pack matrices + time into std140 byte buffer
 * ----------------------------------------------------------------------- */

int su_pack(const float view[16], const float proj[16],
            const float view_proj[16], float time_sec,
            unsigned char *out)
{
    if (!out) return 0;

    /* Zero the entire buffer first (handles std140 padding) */
    memset(out, 0, SU_UBO_SIZE);

    /* Copy matrices: each mat4 is 16 floats = 64 bytes */
    if (view) {
        memcpy(out + SU_VIEW_OFFSET, view, 16 * sizeof(float));
    }
    if (proj) {
        memcpy(out + SU_PROJ_OFFSET, proj, 16 * sizeof(float));
    }
    if (view_proj) {
        memcpy(out + SU_VIEW_PROJ_OFFSET, view_proj, 16 * sizeof(float));
    }

    /* Copy time float at offset 192 */
    memcpy(out + SU_TIME_OFFSET, &time_sec, sizeof(float));

    return SU_UBO_SIZE;
}

/* -----------------------------------------------------------------------
 * su_read_mat4 — read a mat4 back from packed buffer
 * ----------------------------------------------------------------------- */

int su_read_mat4(const unsigned char *buf, int offset, float out[16])
{
    if (!buf || !out) return 0;

    /* Only valid mat4 offsets */
    if (offset != SU_VIEW_OFFSET &&
        offset != SU_PROJ_OFFSET &&
        offset != SU_VIEW_PROJ_OFFSET) {
        return 0;
    }

    memcpy(out, buf + offset, 16 * sizeof(float));
    return 1;
}

/* -----------------------------------------------------------------------
 * su_read_time — read time float back from packed buffer
 * ----------------------------------------------------------------------- */

float su_read_time(const unsigned char *buf)
{
    if (!buf) return 0.0f;

    float t;
    memcpy(&t, buf + SU_TIME_OFFSET, sizeof(float));
    return t;
}
