#ifdef __EMSCRIPTEN__

#include "mesh.h"
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846f

mesh_t mesh_create_sphere(int stacks, int slices) {
    mesh_t m = {0};

    int vert_count = (stacks + 1) * (slices + 1);
    int idx_count = stacks * slices * 6;
    m.index_count = idx_count;

    /* 6 floats per vertex: pos(3) + normal(3) */
    float *vertices = malloc((size_t)vert_count * 6 * sizeof(float));
    unsigned short *indices = malloc((size_t)idx_count * sizeof(unsigned short));

    /* Generate vertices */
    int v = 0;
    for (int i = 0; i <= stacks; i++) {
        float phi = PI * (float)i / (float)stacks - PI / 2.0f; /* -PI/2 to PI/2 */
        float cp = cosf(phi);
        float sp = sinf(phi);

        for (int j = 0; j <= slices; j++) {
            float theta = 2.0f * PI * (float)j / (float)slices;
            float ct = cosf(theta);
            float st = sinf(theta);

            float x = cp * st;
            float y = sp;
            float z = cp * ct;

            /* position = normal for unit sphere */
            vertices[v++] = x;
            vertices[v++] = y;
            vertices[v++] = z;
            vertices[v++] = x; /* normal */
            vertices[v++] = y;
            vertices[v++] = z;
        }
    }

    /* Generate indices */
    int idx = 0;
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            int row1 = i * (slices + 1) + j;
            int row2 = (i + 1) * (slices + 1) + j;

            indices[idx++] = (unsigned short)row1;
            indices[idx++] = (unsigned short)row2;
            indices[idx++] = (unsigned short)(row1 + 1);

            indices[idx++] = (unsigned short)(row1 + 1);
            indices[idx++] = (unsigned short)row2;
            indices[idx++] = (unsigned short)(row2 + 1);
        }
    }

    /* Upload to GPU */
    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    glGenBuffers(1, &m.ebo);

    glBindVertexArray(m.vao);

    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(vert_count * 6 * (int)sizeof(float)),
                 vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)(idx_count * (int)sizeof(unsigned short)),
                 indices, GL_STATIC_DRAW);

    /* position: location 0 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          6 * (GLsizei)sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    /* normal: location 1 */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          6 * (GLsizei)sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    free(vertices);
    free(indices);

    return m;
}

void mesh_draw(const mesh_t *m) {
    glBindVertexArray(m->vao);
    glDrawElements(GL_TRIANGLES, m->index_count, GL_UNSIGNED_SHORT, 0);
}

void mesh_destroy(mesh_t *m) {
    glDeleteVertexArrays(1, &m->vao);
    glDeleteBuffers(1, &m->vbo);
    glDeleteBuffers(1, &m->ebo);
    m->vao = 0;
    m->vbo = 0;
    m->ebo = 0;
    m->index_count = 0;
}

#endif /* __EMSCRIPTEN__ */
