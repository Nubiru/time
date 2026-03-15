/*
 * shader_builder.c — Safe fixed-size string builder for GLSL shader source
 *
 * Provides a simple API for concatenating shader source strings with
 * overflow detection. Uses a fixed 16 KB buffer (SHADER_SRC_CAPACITY).
 *
 * Allowed includes: string.h (strlen, memcpy), stdarg.h (variadic),
 * stdio.h (vsnprintf). These are C standard lib, used at init time only.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "shader_builder.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void shader_src_init(shader_src_t *s)
{
    if (!s) return;
    s->buf[0] = '\0';
    s->len = 0;
    s->overflow = 0;
}

void shader_src_append(shader_src_t *s, const char *str)
{
    if (!s || !str) return;
    if (s->overflow) return;

    int slen = (int)strlen(str);
    int remaining = SHADER_SRC_CAPACITY - 1 - s->len;

    if (slen > remaining) {
        /* Partial copy up to capacity, then flag overflow */
        if (remaining > 0) {
            memcpy(s->buf + s->len, str, (size_t)remaining);
            s->len += remaining;
            s->buf[s->len] = '\0';
        }
        s->overflow = 1;
        return;
    }

    memcpy(s->buf + s->len, str, (size_t)slen);
    s->len += slen;
    s->buf[s->len] = '\0';
}

void shader_src_appendf(shader_src_t *s, const char *fmt, ...)
{
    if (!s || !fmt) return;
    if (s->overflow) return;

    int remaining = SHADER_SRC_CAPACITY - 1 - s->len;
    if (remaining <= 0) {
        s->overflow = 1;
        return;
    }

    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(s->buf + s->len, (size_t)(remaining + 1), fmt, args);
    va_end(args);

    if (written < 0) {
        /* Encoding error — flag overflow as safety measure */
        s->overflow = 1;
        return;
    }

    if (written > remaining) {
        /* Output was truncated */
        s->len += remaining;
        s->buf[s->len] = '\0';
        s->overflow = 1;
        return;
    }

    s->len += written;
    /* vsnprintf already null-terminated */
}

int shader_src_valid(const shader_src_t *s)
{
    if (!s) return 0;
    return s->overflow ? 0 : 1;
}

const char *shader_src_get(const shader_src_t *s)
{
    if (!s) return "";
    return s->buf;
}

int shader_src_len(const shader_src_t *s)
{
    if (!s) return 0;
    return s->len;
}

void shader_src_reset(shader_src_t *s)
{
    if (!s) return;
    s->buf[0] = '\0';
    s->len = 0;
    s->overflow = 0;
}

void shader_src_append_parts(shader_src_t *s, ...)
{
    if (!s) return;

    va_list args;
    va_start(args, s);

    const char *part;
    while ((part = va_arg(args, const char *)) != NULL) {
        shader_src_append(s, part);
        if (s->overflow) break;
    }

    va_end(args);
}
