/*
 * diamond_room_pack.h — Diamond Room wireframe geometry for GPU
 *
 * Packs an icosahedron wireframe (12 vertices, 30 edges, 20 faces)
 * into GL_LINES vertex arrays with per-face glow intensity from the
 * diamond_room data model.
 *
 * Each edge is colored by the average glow of its two adjacent faces.
 * Face-to-system mapping: 20 faces → 16 knowledge systems (4 share).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#ifndef TIME_DIAMOND_ROOM_PACK_H
#define TIME_DIAMOND_ROOM_PACK_H

#include "../systems/earth/diamond_room.h"

/* 12 icosahedron vertices, 30 edges, 20 faces */
#define DRP_VERTEX_COUNT    12
#define DRP_EDGE_COUNT      30
#define DRP_FACE_COUNT      20

/* GL_LINES: 2 vertices per edge, 7 floats per vertex (pos3 + color4) */
#define DRP_VERTEX_FLOATS   7
#define DRP_LINE_VERTS      (DRP_EDGE_COUNT * 2)

/* Pack result */
typedef struct {
    float vertices[DRP_LINE_VERTS * DRP_VERTEX_FLOATS];
    int vertex_count;
    int edge_count;
    float rotation_speed;    /* radians/sec from room ambience */
} drp_data_t;

/* Pack the diamond room wireframe.
 * room: from dr_compute(). scale: scene units radius.
 * Returns packed GL_LINES vertex data. */
drp_data_t drp_pack(const dr_room_t *room, float scale);

/* Get the 3D position of icosahedron vertex i (0-11) at given scale.
 * Returns (0,0,0) for invalid index. */
void drp_vertex_pos(int index, float scale, float *x, float *y, float *z);

/* Get which system_id (0-15) is mapped to face index (0-19).
 * Faces 16-19 map to systems 0-3 (wrap). */
int drp_face_system(int face_index);

/* Vertex shader source (GLSL ES 3.00) — simple MVP transform. */
const char *drp_vert_source(void);

/* Fragment shader source (GLSL ES 3.00) — per-vertex color passthrough. */
const char *drp_frag_source(void);

#endif /* TIME_DIAMOND_ROOM_PACK_H */
