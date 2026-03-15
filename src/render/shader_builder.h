/*
 * shader_builder.h — Safe fixed-size string builder for GLSL shader source
 *
 * Replaces fragile manual char-by-char concatenation with a clean API.
 * Fixed 16 KB buffer with overflow detection. No malloc, no GL.
 *
 * Used at init time only (shader source assembly), never in render loop.
 *
 * Pure module: no GL calls, no malloc, no globals.
 */

#ifndef TIME_SHADER_BUILDER_H
#define TIME_SHADER_BUILDER_H

#define SHADER_SRC_CAPACITY 16384

typedef struct {
    char buf[SHADER_SRC_CAPACITY];
    int len;
    int overflow;  /* set to 1 if capacity exceeded */
} shader_src_t;

/* Initialize to empty string */
void shader_src_init(shader_src_t *s);

/* Append a string. Sets overflow flag if capacity exceeded. */
void shader_src_append(shader_src_t *s, const char *str);

/* Append with printf-style formatting. Sets overflow flag if needed. */
void shader_src_appendf(shader_src_t *s, const char *fmt, ...);

/* Returns 1 if no overflow occurred, 0 if overflow. */
int shader_src_valid(const shader_src_t *s);

/* Get the composed string (null-terminated). */
const char *shader_src_get(const shader_src_t *s);

/* Get current length in bytes. */
int shader_src_len(const shader_src_t *s);

/* Reset to empty (re-use without re-declaring). */
void shader_src_reset(shader_src_t *s);

/* Append multiple strings in order (NULL-terminated variadic list). */
void shader_src_append_parts(shader_src_t *s, ...);

#endif /* TIME_SHADER_BUILDER_H */
