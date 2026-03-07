/* bodygraph_pack.c -- Human Design bodygraph vertex packing implementation.
 * Pure module: no GL calls, no malloc, no globals, no side effects. */

#include "bodygraph_pack.h"

/* ===== INTERNAL HELPERS ===== */

/* Write a quad (2 triangles, 6 verts) centered at (cx, cy) with half-extents r.
 * Each vertex: position(vec2) + color(vec4) = 6 floats.
 * Returns number of floats written (always 36). */
static int write_quad(float cx, float cy, float r,
                      const float color[4], float *out)
{
    /* BL, BR, TR, BL, TR, TL */
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

/* Transform normalized (0-1) position to screen coordinates. */
static void to_screen(bg_point_t p, const bgp_config_t *cfg,
                      float *sx, float *sy)
{
    *sx = p.x * cfg->width + cfg->offset_x;
    *sy = p.y * cfg->height + cfg->offset_y;
}

/* ===== CENTER COLORS ===== */

/* Each center gets a distinct warm-gold hue variant.
 * HD system personality: amber/crimson/teal/violet.
 * Defined centers glow warm; each has a slightly different hue
 * to distinguish the 9 energy centers visually. */
static const float CENTER_COLORS[9][3] = {
    /* HEAD          */ {1.00f, 0.85f, 0.55f},  /* solar gold */
    /* AJNA          */ {0.85f, 0.70f, 0.95f},  /* soft violet */
    /* THROAT        */ {0.55f, 0.85f, 0.95f},  /* sky blue */
    /* G CENTER      */ {0.95f, 0.80f, 0.40f},  /* deep gold */
    /* HEART         */ {0.95f, 0.50f, 0.40f},  /* warm crimson */
    /* SPLEEN        */ {0.60f, 0.85f, 0.60f},  /* earth green */
    /* SACRAL        */ {0.95f, 0.65f, 0.35f},  /* amber */
    /* SOLAR PLEXUS  */ {0.90f, 0.55f, 0.70f},  /* warm rose */
    /* ROOT          */ {0.80f, 0.50f, 0.40f},  /* russet */
};

/* Muted grey for undefined centers */
static const float UNDEFINED_GREY[3] = {0.35f, 0.35f, 0.35f};

/* Active channel color: bright teal-gold */
static const float CHANNEL_ACTIVE_RGB[3] = {0.90f, 0.80f, 0.50f};

/* Inactive channel color: dim grey */
static const float CHANNEL_INACTIVE_RGB[3] = {0.30f, 0.30f, 0.30f};

/* Gate marker color: bright highlight */
static const float GATE_COLOR_RGB[3] = {1.00f, 0.90f, 0.60f};

/* ===== PUBLIC API ===== */

bgp_config_t bgp_default_config(void)
{
    bgp_config_t cfg;
    cfg.width = 300.0f;
    cfg.height = 500.0f;
    cfg.offset_x = 0.0f;
    cfg.offset_y = 0.0f;
    cfg.center_radius = 18.0f;
    cfg.gate_radius = 4.0f;
    cfg.defined_alpha = 0.9f;
    cfg.undefined_alpha = 0.25f;
    return cfg;
}

void bgp_defined_color(bg_center_id_t center, float alpha, float out[4])
{
    int idx = (int)center;
    if (idx < 0 || idx >= BG_CENTER_COUNT) {
        idx = 0;
    }
    out[0] = CENTER_COLORS[idx][0];
    out[1] = CENTER_COLORS[idx][1];
    out[2] = CENTER_COLORS[idx][2];
    out[3] = alpha;
}

void bgp_undefined_color(float alpha, float out[4])
{
    out[0] = UNDEFINED_GREY[0];
    out[1] = UNDEFINED_GREY[1];
    out[2] = UNDEFINED_GREY[2];
    out[3] = alpha;
}

void bgp_channel_color(int active, float defined_alpha,
                       float undefined_alpha, float out[4])
{
    if (active) {
        out[0] = CHANNEL_ACTIVE_RGB[0];
        out[1] = CHANNEL_ACTIVE_RGB[1];
        out[2] = CHANNEL_ACTIVE_RGB[2];
        out[3] = defined_alpha;
    } else {
        out[0] = CHANNEL_INACTIVE_RGB[0];
        out[1] = CHANNEL_INACTIVE_RGB[1];
        out[2] = CHANNEL_INACTIVE_RGB[2];
        out[3] = undefined_alpha;
    }
}

int bgp_pack_centers(const int *defined_centers, const bgp_config_t *config,
                     float *out)
{
    int offset = 0;

    for (int i = 0; i < BG_CENTER_COUNT; i++) {
        bg_point_t pos = bodygraph_center_position((bg_center_id_t)i);
        float sx, sy;
        to_screen(pos, config, &sx, &sy);

        float color[4];
        if (defined_centers[i]) {
            bgp_defined_color((bg_center_id_t)i, config->defined_alpha,
                              color);
        } else {
            bgp_undefined_color(config->undefined_alpha, color);
        }

        offset += write_quad(sx, sy, config->center_radius, color,
                             out + offset);
    }

    return offset / BGP_VERTEX_FLOATS;
}

int bgp_pack_channels(const int *active_gates, const bgp_config_t *config,
                      float *out)
{
    int offset = 0;
    int count = bodygraph_channel_count();

    for (int i = 0; i < count; i++) {
        bg_channel_t ch = bodygraph_channel(i);

        bg_point_t pos_a = bodygraph_center_position(ch.center_a);
        bg_point_t pos_b = bodygraph_center_position(ch.center_b);

        float ax, ay, bx, by;
        to_screen(pos_a, config, &ax, &ay);
        to_screen(pos_b, config, &bx, &by);

        int active = active_gates[ch.gate_a] && active_gates[ch.gate_b];
        float color[4];
        bgp_channel_color(active, config->defined_alpha,
                          config->undefined_alpha, color);

        offset += write_line(ax, ay, bx, by, color, out + offset);
    }

    return offset / BGP_VERTEX_FLOATS;
}

int bgp_pack_gates(const int *active_gates, const bgp_config_t *config,
                   float *out)
{
    int offset = 0;

    for (int g = 1; g <= BODYGRAPH_GATE_COUNT; g++) {
        if (!active_gates[g]) {
            continue;
        }

        bg_gate_pos_t gp = bodygraph_gate_position(g);
        float sx, sy;
        to_screen(gp.position, config, &sx, &sy);

        float color[4];
        color[0] = GATE_COLOR_RGB[0];
        color[1] = GATE_COLOR_RGB[1];
        color[2] = GATE_COLOR_RGB[2];
        color[3] = config->defined_alpha;

        offset += write_quad(sx, sy, config->gate_radius, color,
                             out + offset);
    }

    return offset / BGP_VERTEX_FLOATS;
}

int bgp_pack(const int *defined_centers, const int *active_gates,
             const bgp_config_t *config, float *out)
{
    int total = 0;

    int center_verts = bgp_pack_centers(defined_centers, config, out);
    total += center_verts;

    int channel_verts = bgp_pack_channels(active_gates, config,
                                          out + total * BGP_VERTEX_FLOATS);
    total += channel_verts;

    int gate_verts = bgp_pack_gates(active_gates, config,
                                    out + total * BGP_VERTEX_FLOATS);
    total += gate_verts;

    return total;
}

bgp_info_t bgp_info(const int *defined_centers, const int *active_gates)
{
    bgp_info_t info;

    /* Count defined centers */
    info.defined_centers = 0;
    for (int i = 0; i < BG_CENTER_COUNT; i++) {
        if (defined_centers[i]) {
            info.defined_centers++;
        }
    }

    /* Count active channels */
    info.active_channels = bodygraph_active_channel_count(active_gates);

    /* Count active gates */
    info.active_gates = 0;
    for (int g = 1; g <= BODYGRAPH_GATE_COUNT; g++) {
        if (active_gates[g]) {
            info.active_gates++;
        }
    }

    /* Vertex counts */
    info.center_verts = BG_CENTER_COUNT * BGP_CENTER_VERTS;
    info.channel_verts = BODYGRAPH_CHANNEL_COUNT * BGP_CHANNEL_VERTS;
    info.gate_verts = info.active_gates * BGP_GATE_VERTS;
    info.total_verts = info.center_verts + info.channel_verts
                     + info.gate_verts;

    return info;
}

const char *bgp_vert_source(void)
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

const char *bgp_frag_source(void)
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
