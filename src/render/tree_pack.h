/* tree_pack.h -- Kabbalah Tree of Life vertex packing
 *
 * Packs the 10 Sefirot nodes (+ optional Da'at), 22 paths, and
 * 3 pillar structure lines of the Kabbalah Tree of Life into
 * screen-space 2D vertex arrays for rendering as colored quads
 * (nodes) and line segments (paths, pillars).
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Data sources:
 *   - sefirot.h -- Queen Scale colors, 2D positions, pillar assignments
 *   - tree_geometry.h -- 22 paths with letter type, Sefirot connections
 */

#ifndef TIME_TREE_PACK_H
#define TIME_TREE_PACK_H

#include "../systems/kabbalah/tree_geometry.h"
#include "../systems/kabbalah/sefirot.h"

/* Vertex layout: position(vec2) + color(vec4) = 6 floats per vertex */
#define TRP_VERTEX_FLOATS 6
#define TRP_VERTEX_STRIDE (TRP_VERTEX_FLOATS * (int)sizeof(float))

/* Node quad: 6 vertices (2 triangles) */
#define TRP_NODE_VERTS 6
/* Path line: 2 vertices */
#define TRP_PATH_VERTS 2
/* Pillar line: 2 vertices */
#define TRP_PILLAR_VERTS 2

/* Max: 11 nodes*6 + 22 paths*2 + 3 pillars*2 = 66+44+6 = 116 */
#define TRP_MAX_VERTICES 128

/* Pack result metadata */
typedef struct {
    int node_verts;        /* vertices for Sefirot node shapes */
    int path_verts;        /* vertices for path lines */
    int pillar_verts;      /* vertices for pillar structure lines */
    int total_verts;       /* sum of all */
    int active_nodes;      /* count of activated (lit) Sefirot */
    int active_paths;      /* count of activated paths */
} trp_info_t;

/* Configuration */
typedef struct {
    float width;           /* tree total width in screen units */
    float height;          /* tree total height in screen units */
    float offset_x;        /* screen offset X */
    float offset_y;        /* screen offset Y */
    float node_radius;     /* radius of Sefirot node quads */
    float active_alpha;    /* alpha for activated (lit) nodes/paths */
    float inactive_alpha;  /* alpha for inactive nodes/paths */
    int show_daat;         /* 1 to include Da'at (hidden 11th), 0 to skip */
} trp_config_t;

/* Default configuration. */
trp_config_t trp_default_config(void);

/* Pack Sefirot node shapes (quads) into vertex buffer.
 * active_nodes: array of 11 booleans (indices 0-9 for Sefirot, 10 for Da'at).
 *   Active = planet in domicile/exaltation at that Sefirah.
 * out: vertex buffer, must hold TRP_MAX_VERTICES * TRP_VERTEX_FLOATS floats.
 * Returns number of vertices written. */
int trp_pack_nodes(const int *active_nodes, const trp_config_t *config,
                   float *out);

/* Pack path lines into vertex buffer.
 * active_paths: array of 22 booleans (index = path id).
 *   Active = zodiac/planet associated with path has planets.
 * out: vertex buffer appended after nodes.
 * Returns number of vertices written. */
int trp_pack_paths(const int *active_paths, const trp_config_t *config,
                   float *out);

/* Pack pillar structure lines (3 vertical pillars: Mercy, Severity, Balance).
 * Returns number of vertices written. */
int trp_pack_pillars(const trp_config_t *config, float *out);

/* Pack everything: nodes + paths + pillars in one call.
 * Returns total vertices written. */
int trp_pack(const int *active_nodes, const int *active_paths,
             const trp_config_t *config, float *out);

/* Get metadata from pack parameters. */
trp_info_t trp_info(const int *active_nodes, const int *active_paths,
                     const trp_config_t *config);

/* Color for a Sefirah node. Uses Queen Scale (Golden Dawn).
 * index: 0-9 (Sefirot) or 10 (Da'at). alpha: opacity.
 * out: RGBA float[4]. */
void trp_node_color(int index, float alpha, float out[4]);

/* Color for a path by type.
 * letter_type: TREE_LETTER_MOTHER(0), TREE_LETTER_DOUBLE(1),
 *              TREE_LETTER_SIMPLE(2).
 * active: whether path is currently lit. alpha: opacity.
 * out: RGBA float[4]. */
void trp_path_color(int letter_type, int active, float alpha, float out[4]);

/* Tree vertex shader source (GLSL ES 3.00).
 * Screen-space 2D. Uniforms: u_projection (mat4).
 * Attributes: a_position (vec2, loc 0), a_color (vec4, loc 1). */
const char *trp_vert_source(void);

/* Tree fragment shader source (GLSL ES 3.00).
 * Pass-through color with optional glow for active Sefirot. */
const char *trp_frag_source(void);

#endif /* TIME_TREE_PACK_H */
