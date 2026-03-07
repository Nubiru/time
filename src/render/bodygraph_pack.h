/* bodygraph_pack.h -- Human Design bodygraph vertex packing
 *
 * Packs the 9 energy centers, 36 channels, and 64 gates of the
 * Human Design bodygraph into screen-space 2D vertex arrays for
 * rendering as colored shapes (center quads), line segments
 * (channels), and small marker quads (gates).
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Data sources:
 *   - bodygraph.h -- center positions, channel connections, gate data
 */

#ifndef TIME_BODYGRAPH_PACK_H
#define TIME_BODYGRAPH_PACK_H

#include "../systems/human_design/bodygraph.h"

/* Vertex layout: position(vec2) + color(vec4) = 6 floats per vertex */
#define BGP_VERTEX_FLOATS 6
#define BGP_VERTEX_STRIDE (BGP_VERTEX_FLOATS * (int)sizeof(float))

/* Center quad: 6 vertices (2 triangles) */
#define BGP_CENTER_VERTS 6
/* Channel line: 2 vertices */
#define BGP_CHANNEL_VERTS 2
/* Gate marker: 6 vertices (small quad) */
#define BGP_GATE_VERTS 6

/* Max vertices: 9 centers * 6 + 36 channels * 2 + 64 gates * 6 = 54 + 72 + 384 = 510 */
#define BGP_MAX_VERTICES 512

/* Pack result metadata */
typedef struct {
    int center_verts;      /* vertices for center shapes */
    int channel_verts;     /* vertices for channel lines */
    int gate_verts;        /* vertices for gate markers */
    int total_verts;       /* sum of all */
    int defined_centers;   /* count of defined (colored) centers */
    int active_channels;   /* count of activated channels */
    int active_gates;      /* count of activated gates */
} bgp_info_t;

/* Configuration */
typedef struct {
    float width;           /* bodygraph total width in screen units */
    float height;          /* bodygraph total height in screen units */
    float offset_x;        /* screen offset X */
    float offset_y;        /* screen offset Y */
    float center_radius;   /* radius of center shapes */
    float gate_radius;     /* radius of gate marker dots */
    float defined_alpha;   /* alpha for defined centers/channels */
    float undefined_alpha; /* alpha for undefined centers/channels */
} bgp_config_t;

/* Default configuration. */
bgp_config_t bgp_default_config(void);

/* Pack center shapes (quads) into vertex buffer.
 * defined_centers: array of 9 booleans (from bodygraph_defined_centers).
 * out: vertex buffer, must hold BGP_MAX_VERTICES * BGP_VERTEX_FLOATS floats.
 * Returns number of vertices written. */
int bgp_pack_centers(const int *defined_centers, const bgp_config_t *config,
                     float *out);

/* Pack channel lines into vertex buffer.
 * active_gates: array of 65 booleans (indices 1-64).
 * out: vertex buffer appended after centers.
 * Returns number of vertices written. */
int bgp_pack_channels(const int *active_gates, const bgp_config_t *config,
                      float *out);

/* Pack gate markers into vertex buffer.
 * active_gates: array of 65 booleans (indices 1-64).
 * out: vertex buffer appended after channels.
 * Returns number of vertices written. */
int bgp_pack_gates(const int *active_gates, const bgp_config_t *config,
                   float *out);

/* Pack everything: centers + channels + gates in one call.
 * defined_centers: array of 9 booleans.
 * active_gates: array of 65 booleans (indices 1-64).
 * Returns total vertices written. */
int bgp_pack(const int *defined_centers, const int *active_gates,
             const bgp_config_t *config, float *out);

/* Get metadata from a pack result. */
bgp_info_t bgp_info(const int *defined_centers, const int *active_gates);

/* Color for a defined center (warm gold glow). out: RGBA */
void bgp_defined_color(bg_center_id_t center, float alpha, float out[4]);

/* Color for an undefined center (muted grey). out: RGBA */
void bgp_undefined_color(float alpha, float out[4]);

/* Color for an active channel. out: RGBA */
void bgp_channel_color(int active, float defined_alpha,
                       float undefined_alpha, float out[4]);

/* Bodygraph vertex shader source (GLSL ES 3.00).
 * Screen-space 2D. Uniforms: u_projection (mat4).
 * Attributes: a_position (vec2, loc 0), a_color (vec4, loc 1). */
const char *bgp_vert_source(void);

/* Bodygraph fragment shader source (GLSL ES 3.00).
 * Pass-through color with optional glow for defined centers. */
const char *bgp_frag_source(void);

#endif /* TIME_BODYGRAPH_PACK_H */
