/* tree_pack.c -- Kabbalah Tree of Life vertex packing implementation.
 *
 * Packs 10 Sefirot (+ optional Da'at) as colored quads, 22 paths as
 * colored lines (type-coded: mother=gold, double=silver, simple=teal),
 * and 3 pillar structure lines into GPU-ready vertex arrays.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Sources: Golden Dawn Queen Scale colors (from sefirot.h),
 *          Kircher Tree path layout (from tree_geometry.h). */

#include "tree_pack.h"

/* ===== INTERNAL HELPERS ===== */

/* Write a quad (2 triangles, 6 verts) centered at (cx, cy) with half-extents r.
 * Each vertex: position(vec2) + color(vec4) = 6 floats.
 * Returns number of floats written (always 36). */
static int write_quad(float cx, float cy, float r,
                      const float color[4], float *out)
{
    float x0 = cx - r, y0 = cy - r;
    float x1 = cx + r, y1 = cy + r;

    int idx = 0;

    /* Triangle 1: BL */
    out[idx++] = x0; out[idx++] = y0;
    out[idx++] = color[0]; out[idx++] = color[1];
    out[idx++] = color[2]; out[idx++] = color[3];

    /* Triangle 1: BR */
    out[idx++] = x1; out[idx++] = y0;
    out[idx++] = color[0]; out[idx++] = color[1];
    out[idx++] = color[2]; out[idx++] = color[3];

    /* Triangle 1: TR */
    out[idx++] = x1; out[idx++] = y1;
    out[idx++] = color[0]; out[idx++] = color[1];
    out[idx++] = color[2]; out[idx++] = color[3];

    /* Triangle 2: BL */
    out[idx++] = x0; out[idx++] = y0;
    out[idx++] = color[0]; out[idx++] = color[1];
    out[idx++] = color[2]; out[idx++] = color[3];

    /* Triangle 2: TR */
    out[idx++] = x1; out[idx++] = y1;
    out[idx++] = color[0]; out[idx++] = color[1];
    out[idx++] = color[2]; out[idx++] = color[3];

    /* Triangle 2: TL */
    out[idx++] = x0; out[idx++] = y1;
    out[idx++] = color[0]; out[idx++] = color[1];
    out[idx++] = color[2]; out[idx++] = color[3];

    return idx;
}

/* Write a line segment (2 verts) from (x0,y0) to (x1,y1).
 * Returns number of floats written (always 12). */
static int write_line(float x0, float y0, float x1, float y1,
                      const float color[4], float *out)
{
    int idx = 0;

    out[idx++] = x0; out[idx++] = y0;
    out[idx++] = color[0]; out[idx++] = color[1];
    out[idx++] = color[2]; out[idx++] = color[3];

    out[idx++] = x1; out[idx++] = y1;
    out[idx++] = color[0]; out[idx++] = color[1];
    out[idx++] = color[2]; out[idx++] = color[3];

    return idx;
}

/* ===== PATH COLOR CONSTANTS ===== */

/* Mother paths (elements): gold — fire/water/air are primordial forces */
static const float MOTHER_RGB[3] = {1.0f, 0.85f, 0.55f};

/* Double paths (planets): silver — reflective, planetary metal */
static const float DOUBLE_RGB[3] = {0.75f, 0.75f, 0.8f};

/* Simple paths (zodiac): teal — celestial, zodiacal wheel */
static const float SIMPLE_RGB[3] = {0.2f, 0.75f, 0.8f};

/* Inactive path: dim grey */
static const float INACTIVE_RGB[3] = {0.3f, 0.3f, 0.3f};

/* Pillar color: subtle structural lines */
static const float PILLAR_RGB[3] = {0.25f, 0.25f, 0.3f};
static const float PILLAR_ALPHA = 0.35f;

/* ===== PUBLIC API ===== */

trp_config_t trp_default_config(void)
{
    trp_config_t cfg;
    cfg.width = 300.0f;
    cfg.height = 500.0f;
    cfg.offset_x = 0.0f;
    cfg.offset_y = 0.0f;
    cfg.node_radius = 14.0f;
    cfg.active_alpha = 0.9f;
    cfg.inactive_alpha = 0.25f;
    cfg.show_daat = 0;
    return cfg;
}

void trp_node_color(int index, float alpha, float out[4])
{
    sefirah_t s;
    if (index == 10) {
        s = sefirot_get_daat();
    } else if (index >= 0 && index < SEFIROT_COUNT) {
        s = sefirot_get(index);
    } else {
        /* Invalid index: fallback to muted grey */
        out[0] = 0.4f;
        out[1] = 0.4f;
        out[2] = 0.4f;
        out[3] = alpha;
        return;
    }
    out[0] = s.color.r;
    out[1] = s.color.g;
    out[2] = s.color.b;
    out[3] = alpha;
}

void trp_path_color(int letter_type, int active, float alpha, float out[4])
{
    if (!active) {
        out[0] = INACTIVE_RGB[0];
        out[1] = INACTIVE_RGB[1];
        out[2] = INACTIVE_RGB[2];
        out[3] = alpha;
        return;
    }

    const float *rgb;
    switch (letter_type) {
    case TREE_LETTER_MOTHER:
        rgb = MOTHER_RGB;
        break;
    case TREE_LETTER_DOUBLE:
        rgb = DOUBLE_RGB;
        break;
    case TREE_LETTER_SIMPLE:
        rgb = SIMPLE_RGB;
        break;
    default:
        rgb = INACTIVE_RGB;
        break;
    }
    out[0] = rgb[0];
    out[1] = rgb[1];
    out[2] = rgb[2];
    out[3] = alpha;
}

int trp_pack_nodes(const int *active_nodes, const trp_config_t *config,
                   float *out)
{
    tree_layout_t layout = tree_layout_generate(config->width, config->height);
    int offset = 0;
    int count = config->show_daat ? SEFIROT_COUNT_WITH_DAAT : SEFIROT_COUNT;

    for (int i = 0; i < count; i++) {
        float sx = layout.node_x[i] + config->offset_x;
        float sy = layout.node_y[i] + config->offset_y;

        float alpha = active_nodes[i] ? config->active_alpha
                                      : config->inactive_alpha;
        float color[4];
        trp_node_color(i, alpha, color);

        offset += write_quad(sx, sy, config->node_radius, color,
                             out + offset);
    }

    return offset / TRP_VERTEX_FLOATS;
}

int trp_pack_paths(const int *active_paths, const trp_config_t *config,
                   float *out)
{
    tree_layout_t layout = tree_layout_generate(config->width, config->height);
    int offset = 0;

    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        tree_path_t path = tree_path_get(i);

        float x0 = layout.node_x[path.from_sefirah] + config->offset_x;
        float y0 = layout.node_y[path.from_sefirah] + config->offset_y;
        float x1 = layout.node_x[path.to_sefirah] + config->offset_x;
        float y1 = layout.node_y[path.to_sefirah] + config->offset_y;

        int active = active_paths[i];
        float alpha = active ? config->active_alpha : config->inactive_alpha;
        float color[4];
        trp_path_color(path.letter_type, active, alpha, color);

        offset += write_line(x0, y0, x1, y1, color, out + offset);
    }

    return offset / TRP_VERTEX_FLOATS;
}

int trp_pack_pillars(const trp_config_t *config, float *out)
{
    /* Three pillars: Balance (middle, x=0.0), Mercy (right, x=0.5),
     * Severity (left, x=-0.5).
     * Pillar lines run from top (y=0) to bottom (y=height).
     * Sefirot x positions: -0.5, 0.0, 0.5 in normalized coords.
     * Scaled: (pos_x + 0.5) * width */
    static const float PILLAR_X[3] = {0.0f, 0.5f, -0.5f};
    int offset = 0;

    float color[4];
    color[0] = PILLAR_RGB[0];
    color[1] = PILLAR_RGB[1];
    color[2] = PILLAR_RGB[2];
    color[3] = PILLAR_ALPHA;

    for (int p = 0; p < 3; p++) {
        float sx = (PILLAR_X[p] + 0.5f) * config->width + config->offset_x;
        float y_top = config->offset_y;
        float y_bot = config->height + config->offset_y;

        offset += write_line(sx, y_top, sx, y_bot, color, out + offset);
    }

    return offset / TRP_VERTEX_FLOATS;
}

int trp_pack(const int *active_nodes, const int *active_paths,
             const trp_config_t *config, float *out)
{
    int total = 0;

    int node_verts = trp_pack_nodes(active_nodes, config, out);
    total += node_verts;

    int path_verts = trp_pack_paths(active_paths, config,
                                    out + total * TRP_VERTEX_FLOATS);
    total += path_verts;

    int pillar_verts = trp_pack_pillars(config,
                                        out + total * TRP_VERTEX_FLOATS);
    total += pillar_verts;

    return total;
}

trp_info_t trp_info(const int *active_nodes, const int *active_paths,
                     const trp_config_t *config)
{
    trp_info_t info;
    int node_count = config->show_daat ? SEFIROT_COUNT_WITH_DAAT
                                       : SEFIROT_COUNT;

    /* Count active nodes */
    info.active_nodes = 0;
    for (int i = 0; i < node_count; i++) {
        if (active_nodes[i]) {
            info.active_nodes++;
        }
    }

    /* Count active paths */
    info.active_paths = 0;
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        if (active_paths[i]) {
            info.active_paths++;
        }
    }

    /* Vertex counts */
    info.node_verts = node_count * TRP_NODE_VERTS;
    info.path_verts = TREE_PATH_COUNT * TRP_PATH_VERTS;
    info.pillar_verts = 3 * TRP_PILLAR_VERTS;
    info.total_verts = info.node_verts + info.path_verts + info.pillar_verts;

    return info;
}

const char *trp_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "layout(location = 0) in vec2 a_position;\n"
        "layout(location = 1) in vec4 a_color;\n"
        "\n"
        "uniform mat4 u_projection;\n"
        "\n"
        "out vec4 v_color;\n"
        "\n"
        "void main() {\n"
        "    gl_Position = u_projection * vec4(a_position, 0.0, 1.0);\n"
        "    v_color = a_color;\n"
        "}\n";
}

const char *trp_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "in vec4 v_color;\n"
        "out vec4 frag_color;\n"
        "\n"
        "void main() {\n"
        "    frag_color = v_color;\n"
        "}\n";
}
