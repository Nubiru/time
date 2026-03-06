#ifndef TIME_MESH_H
#define TIME_MESH_H

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    int index_count;
} mesh_t;

/* Create a UV sphere mesh with given subdivisions.
 * Vertex data: position (3f) + normal (3f) = 6 floats per vertex.
 * stacks = latitude divisions, slices = longitude divisions. */
mesh_t mesh_create_sphere(int stacks, int slices);

void mesh_draw(const mesh_t *m);
void mesh_destroy(mesh_t *m);

#endif /* __EMSCRIPTEN__ */

#endif
