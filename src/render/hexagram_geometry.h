/* hexagram_geometry.h -- I Ching Hexagram Visual Geometry
 *
 * Pure vertex packing module that generates I Ching hexagram line
 * geometry as GPU-ready vertex arrays. Each hexagram has 6 horizontal
 * lines (solid yang, broken yin) with optional changing line highlights,
 * trigram decomposition gap, and nuclear hexagram derivation.
 *
 * Vertex format: position(vec2) + color(vec4) = 6 floats per vertex.
 * Geometry is GL_LINES pairs (2 vertices per segment).
 * Yang line: 1 segment (2 vertices). Yin line: 2 segments (4 vertices).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_HEXAGRAM_GEOMETRY_H
#define TIME_HEXAGRAM_GEOMETRY_H

/* Line vertex: position(vec2) + color(vec4) = 6 floats */
#define HG_VERTEX_FLOATS 6
#define HG_MAX_LINES 18      /* 6 main + 6 nuclear upper + 6 nuclear lower */
#define HG_VERTS_PER_LINE 4  /* 2 endpoints per segment, yang=1 segment, yin=2 segments */
#define HG_MAX_VERTICES (HG_MAX_LINES * HG_VERTS_PER_LINE)

/* Packed hexagram vertex data */
typedef struct {
    float vertices[HG_MAX_VERTICES * HG_VERTEX_FLOATS];
    int vertex_count;        /* actual vertices written */
    int line_count;          /* number of line segments (GL_LINES pairs) */
    int main_vertex_count;   /* vertices for main hexagram only */
    int nuclear_vertex_count;/* vertices for nuclear hexagrams */
} hg_data_t;

/* Changing line flags */
typedef struct {
    int changing[6];  /* 1 = this line is changing, 0 = stable */
} hg_changing_t;

/* Pack hexagram as line vertex data.
 * king_wen: hexagram number (1-64).
 * width, height: bounding box dimensions.
 * changing: which lines are changing (NULL for none).
 * Returns vertex data with GL_LINES pairs. */
hg_data_t hg_pack(int king_wen, float width, float height,
                    const hg_changing_t *changing);

/* Pack hexagram with nuclear hexagrams rendered alongside.
 * Nuclear hexagram = lines 2-3-4 (lower) and 3-4-5 (upper).
 * nuclear_offset_x: horizontal offset for nuclear display.
 * nuclear_scale: size multiplier for nuclear (suggested: 0.5). */
hg_data_t hg_pack_with_nuclear(int king_wen, float width, float height,
                                const hg_changing_t *changing,
                                float nuclear_offset_x, float nuclear_scale);

/* Pack just the trigram decomposition (upper 3 lines + lower 3 lines with gap).
 * gap_ratio: fraction of height used for trigram gap (suggested: 0.15). */
hg_data_t hg_pack_trigrams(int king_wen, float width, float height,
                            float gap_ratio);

/* Get the nuclear hexagram King Wen numbers.
 * Returns 0 for invalid input.
 * Nuclear lower: lower trigram from lines[1,2,3], upper from lines[2,3,4]. */
int hg_nuclear_lower(int king_wen);

/* Nuclear upper: lower trigram from lines[2,3,4], upper from lines[3,4,5].
 * Returns 0 for invalid input. */
int hg_nuclear_upper(int king_wen);

/* Get line count for a hexagram (always 6 for valid, 0 for invalid). */
int hg_line_count(int king_wen);

/* Get vertex byte size for glBufferData. */
int hg_vertex_bytes(const hg_data_t *data);

/* Hexagram line vertex shader (GLSL ES 3.00). */
const char *hg_line_vert_source(void);

/* Hexagram line fragment shader (GLSL ES 3.00). */
const char *hg_line_frag_source(void);

#endif /* TIME_HEXAGRAM_GEOMETRY_H */
