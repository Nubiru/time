#version 300 es

/* Fullscreen quad vertex shader — no buffers needed.
 * Uses gl_VertexID to generate a triangle strip covering the screen.
 * Draw with: glDrawArrays(GL_TRIANGLE_STRIP, 0, 4) */

void main() {
    vec2 pos = vec2(
        float((gl_VertexID & 1) * 2 - 1),
        float((gl_VertexID >> 1 & 1) * 2 - 1)
    );
    gl_Position = vec4(pos, 0.0, 1.0);
}
