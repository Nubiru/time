/*
 * diamond_room_pack.c — Diamond Room wireframe geometry for GPU
 *
 * Icosahedron vertices from the classic construction:
 *   12 vertices at (0, ±1, ±φ), (±1, ±φ, 0), (±φ, 0, ±1)
 * where φ = (1+√5)/2 ≈ 1.618 (golden ratio).
 *
 * Pure module: no GL, no malloc, no globals.
 */

#include "diamond_room_pack.h"
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define PHI 1.6180339887498948482

/* Icosahedron 12 vertices (normalized, then scaled) */
static const float ICO_VERTS[12][3] = {
    { 0.0f,  1.0f,  (float)PHI},  /*  0 */
    { 0.0f, -1.0f,  (float)PHI},  /*  1 */
    { 0.0f,  1.0f, -(float)PHI},  /*  2 */
    { 0.0f, -1.0f, -(float)PHI},  /*  3 */
    { 1.0f,  (float)PHI, 0.0f},   /*  4 */
    {-1.0f,  (float)PHI, 0.0f},   /*  5 */
    { 1.0f, -(float)PHI, 0.0f},   /*  6 */
    {-1.0f, -(float)PHI, 0.0f},   /*  7 */
    { (float)PHI, 0.0f,  1.0f},   /*  8 */
    {-(float)PHI, 0.0f,  1.0f},   /*  9 */
    { (float)PHI, 0.0f, -1.0f},   /* 10 */
    {-(float)PHI, 0.0f, -1.0f},   /* 11 */
};

/* 30 edges of the icosahedron (vertex index pairs) */
static const int ICO_EDGES[30][2] = {
    {0,1}, {0,4}, {0,5}, {0,8}, {0,9},
    {1,6}, {1,7}, {1,8}, {1,9},
    {2,3}, {2,4}, {2,5}, {2,10}, {2,11},
    {3,6}, {3,7}, {3,10}, {3,11},
    {4,5}, {4,8}, {4,10},
    {5,9}, {5,11},
    {6,7}, {6,8}, {6,10},
    {7,9}, {7,11},
    {8,10}, {9,11},
};

/* 20 faces of the icosahedron (vertex index triples) */
static const int ICO_FACES[20][3] = {
    {0,1,8}, {0,1,9}, {0,4,5}, {0,4,8}, {0,5,9},
    {2,3,10}, {2,3,11}, {2,4,5}, {2,4,10}, {2,5,11},
    {1,6,8}, {1,7,9}, {1,6,7},
    {3,6,10}, {3,7,11}, {3,6,7},
    {4,8,10}, {5,9,11},
    {6,8,10}, {7,9,11},
};

/* Map face index (0-19) to system_id (0-15). Faces 16-19 wrap to 0-3. */
int drp_face_system(int face_index)
{
    if (face_index < 0 || face_index >= DRP_FACE_COUNT) return 0;
    return face_index % DR_MAX_FACETS;
}

/* Get normalized vertex position, scaled */
void drp_vertex_pos(int index, float scale, float *x, float *y, float *z)
{
    if (index < 0 || index >= DRP_VERTEX_COUNT || !x || !y || !z) {
        if (x) *x = 0.0f;
        if (y) *y = 0.0f;
        if (z) *z = 0.0f;
        return;
    }

    /* Normalize to unit sphere, then scale */
    float vx = ICO_VERTS[index][0];
    float vy = ICO_VERTS[index][1];
    float vz = ICO_VERTS[index][2];
    float mag = sqrtf(vx*vx + vy*vy + vz*vz);
    if (mag > 0.001f) {
        vx /= mag; vy /= mag; vz /= mag;
    }
    *x = vx * scale;
    *y = vy * scale;
    *z = vz * scale;
}

/* Find which face an edge belongs to (returns face with max glow).
 * Each edge touches exactly 2 faces. Returns average glow of both. */
static float edge_glow(int edge_idx, const dr_room_t *room)
{
    int va = ICO_EDGES[edge_idx][0];
    int vb = ICO_EDGES[edge_idx][1];

    float max_glow = 0.0f;
    for (int f = 0; f < DRP_FACE_COUNT; f++) {
        /* Check if this face shares both vertices of the edge */
        int has_a = 0, has_b = 0;
        for (int v = 0; v < 3; v++) {
            if (ICO_FACES[f][v] == va) has_a = 1;
            if (ICO_FACES[f][v] == vb) has_b = 1;
        }
        if (has_a && has_b) {
            int sys = drp_face_system(f);
            if (sys < room->facet_count) {
                float g = (float)room->facets[sys].brightness;
                if (g > max_glow) max_glow = g;
            }
        }
    }
    return max_glow;
}

drp_data_t drp_pack(const dr_room_t *room, float scale)
{
    drp_data_t data;
    data.vertex_count = 0;
    data.edge_count = 0;
    data.rotation_speed = (float)room->ambience.rotation_speed;

    for (int e = 0; e < DRP_EDGE_COUNT; e++) {
        int va = ICO_EDGES[e][0];
        int vb = ICO_EDGES[e][1];

        float ax, ay, az, bx, by, bz;
        drp_vertex_pos(va, scale, &ax, &ay, &az);
        drp_vertex_pos(vb, scale, &bx, &by, &bz);

        float glow = edge_glow(e, room);
        /* Base color: dim crystal (0.15) when unexplored, bright when explored */
        float base = 0.15f + glow * 0.85f;

        /* Tint toward system hue via golden angle */
        float r = base * 0.8f + 0.2f;
        float g = base * 0.85f + 0.1f;
        float b = base * 0.9f + 0.05f;
        float a = 0.3f + glow * 0.7f;

        /* Vertex A */
        int i = data.vertex_count * DRP_VERTEX_FLOATS;
        data.vertices[i+0] = ax; data.vertices[i+1] = ay; data.vertices[i+2] = az;
        data.vertices[i+3] = r;  data.vertices[i+4] = g;
        data.vertices[i+5] = b;  data.vertices[i+6] = a;
        data.vertex_count++;

        /* Vertex B */
        i = data.vertex_count * DRP_VERTEX_FLOATS;
        data.vertices[i+0] = bx; data.vertices[i+1] = by; data.vertices[i+2] = bz;
        data.vertices[i+3] = r;  data.vertices[i+4] = g;
        data.vertices[i+5] = b;  data.vertices[i+6] = a;
        data.vertex_count++;

        data.edge_count++;
    }

    return data;
}

/* Simple MVP vertex shader */
static const char s_drp_vert[] =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec4 a_color;\n"
    "uniform mat4 u_mvp;\n"
    "out vec4 v_color;\n"
    "void main() {\n"
    "    v_color = a_color;\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "}\n";

/* Color passthrough fragment shader */
static const char s_drp_frag[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    frag_color = v_color;\n"
    "}\n";

const char *drp_vert_source(void) { return s_drp_vert; }
const char *drp_frag_source(void) { return s_drp_frag; }
