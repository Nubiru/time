/*
 * shared_uniforms.h — UBO layout constants, GLSL block declaration, data packing
 *
 * Pure data module providing std140 layout constants, a GLSL uniform block
 * declaration string, and pack/read functions for shared uniform data.
 * The GL integration (buffer creation, upload, binding) lives elsewhere.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#ifndef TIME_SHARED_UNIFORMS_H
#define TIME_SHARED_UNIFORMS_H

/* std140 layout:
 *   mat4 u_view      (64 bytes, offset 0)
 *   mat4 u_proj      (64 bytes, offset 64)
 *   mat4 u_view_proj (64 bytes, offset 128)
 *   float u_time     (4 bytes, offset 192)
 *   Total: 196 bytes, padded to 208 (std140 rounds to 16-byte boundary)
 */

#define SU_BINDING_POINT 0
#define SU_VIEW_OFFSET 0
#define SU_PROJ_OFFSET 64
#define SU_VIEW_PROJ_OFFSET 128
#define SU_TIME_OFFSET 192
#define SU_UBO_SIZE 208  /* std140 pads to 16-byte boundary: ceil(196/16)*16 = 208 */

/* Get the GLSL uniform block declaration string.
 * Insert this near the top of any shader that uses shared uniforms.
 * Returns: "layout(std140) uniform SharedData { mat4 u_view; mat4 u_proj; mat4 u_view_proj; float u_time; };\n"
 */
const char *su_block_declaration(void);

/* Pack shared uniform data into a byte buffer (std140 layout).
 * view: 4x4 view matrix (16 floats, column-major)
 * proj: 4x4 projection matrix (16 floats, column-major)
 * view_proj: 4x4 combined view*proj matrix (16 floats, column-major)
 * time_sec: animation time in seconds
 * out: output buffer, must be at least SU_UBO_SIZE bytes
 * Returns number of bytes written (SU_UBO_SIZE), or 0 if out is NULL.
 */
int su_pack(const float view[16], const float proj[16],
            const float view_proj[16], float time_sec,
            unsigned char *out);

/* Read back a mat4 from a packed UBO buffer.
 * buf: packed UBO data (from su_pack)
 * offset: byte offset (SU_VIEW_OFFSET, SU_PROJ_OFFSET, or SU_VIEW_PROJ_OFFSET)
 * out: receives 16 floats. NULL-safe.
 * Returns 1 on success, 0 on invalid offset or NULL. */
int su_read_mat4(const unsigned char *buf, int offset, float out[16]);

/* Read back the time value from a packed UBO buffer.
 * buf: packed UBO data (from su_pack)
 * Returns the float time value, or 0.0f if buf is NULL. */
float su_read_time(const unsigned char *buf);

#endif /* TIME_SHARED_UNIFORMS_H */
