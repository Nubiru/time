/* earth_pack.c — GPU-ready vertex packing for Earth globe, coastlines,
 * terminator, and atmosphere shell.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects. */

#include "earth_pack.h"
#include "earth_globe.h"
#include "earth_atmosphere.h"

#include <math.h>
#include <stddef.h>

/* =========================================================
 *  Default configuration
 * ========================================================= */

erp_config_t erp_default_config(void)
{
    erp_config_t cfg;
    cfg.globe_radius = 1.0f;
    cfg.atmo_radius_ratio = 1.02f;
    cfg.lat_segments = 32;
    cfg.lon_segments = 64;
    cfg.coastline_alpha = 0.7f;
    cfg.terminator_alpha = 0.5f;
    return cfg;
}

/* =========================================================
 *  Info / metadata
 * ========================================================= */

erp_info_t erp_info(const erp_config_t *config)
{
    erp_info_t info;
    info.globe_verts = earth_globe_vertex_count(config->lat_segments,
                                                 config->lon_segments);
    info.globe_indices = earth_globe_index_count(config->lat_segments,
                                                  config->lon_segments);
    info.coastline_verts = earth_coastline_count() * 2;
    info.terminator_verts = ERP_TERMINATOR_POINTS;
    info.atmo_verts = earth_atmo_shell_vertex_count();
    info.atmo_indices = earth_atmo_shell_index_count();
    return info;
}

/* =========================================================
 *  Pack globe mesh
 * ========================================================= */

int erp_pack_globe(const erp_config_t *config,
                   float *out_verts, unsigned int *out_indices)
{
    earth_vertex_t globe_verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int globe_indices[EARTH_GLOBE_MAX_INDICES];

    earth_mesh_info_t mesh = earth_globe_generate(
        config->lat_segments, config->lon_segments,
        globe_verts, globe_indices);

    float r = config->globe_radius;

    /* Repack earth_vertex_t into interleaved float array:
     * position(vec3) + normal(vec3) + uv(vec2) = 8 floats per vertex */
    for (int i = 0; i < mesh.vertex_count; i++) {
        int base = i * ERP_GLOBE_VERTEX_FLOATS;
        out_verts[base + 0] = globe_verts[i].x * r;
        out_verts[base + 1] = globe_verts[i].y * r;
        out_verts[base + 2] = globe_verts[i].z * r;
        out_verts[base + 3] = globe_verts[i].nx;
        out_verts[base + 4] = globe_verts[i].ny;
        out_verts[base + 5] = globe_verts[i].nz;
        out_verts[base + 6] = globe_verts[i].u;
        out_verts[base + 7] = globe_verts[i].v;
    }

    /* Copy indices directly */
    for (int i = 0; i < mesh.index_count; i++) {
        out_indices[i] = globe_indices[i];
    }

    return mesh.vertex_count;
}

/* =========================================================
 *  Pack coastline segments
 * ========================================================= */

int erp_pack_coastlines(const erp_config_t *config, float *out)
{
    int seg_count = earth_coastline_count();
    float r = config->globe_radius;
    float alpha = config->coastline_alpha;

    /* Coastline color: green-cyan for land outlines */
    float cr = 0.3f;
    float cg = 0.8f;
    float cb = 0.4f;

    int vi = 0;
    for (int s = 0; s < seg_count; s++) {
        coastline_segment_t seg = earth_coastline_get(s);

        /* Convert each endpoint from lat/lon to XYZ on unit sphere */
        float x1, y1, z1, x2, y2, z2;
        earth_latlon_to_xyz((double)seg.lat1, (double)seg.lon1, &x1, &y1, &z1);
        earth_latlon_to_xyz((double)seg.lat2, (double)seg.lon2, &x2, &y2, &z2);

        /* Vertex 1 */
        int base = vi * ERP_LINE_VERTEX_FLOATS;
        out[base + 0] = x1 * r;
        out[base + 1] = y1 * r;
        out[base + 2] = z1 * r;
        out[base + 3] = cr;
        out[base + 4] = cg;
        out[base + 5] = cb;
        out[base + 6] = alpha;
        vi++;

        /* Vertex 2 */
        base = vi * ERP_LINE_VERTEX_FLOATS;
        out[base + 0] = x2 * r;
        out[base + 1] = y2 * r;
        out[base + 2] = z2 * r;
        out[base + 3] = cr;
        out[base + 4] = cg;
        out[base + 5] = cb;
        out[base + 6] = alpha;
        vi++;
    }

    return vi;
}

/* =========================================================
 *  Pack terminator great circle
 * ========================================================= */

int erp_pack_terminator(double solar_dec_deg, double solar_ra_hours,
                         const erp_config_t *config, float *out)
{
    terminator_point_t points[EARTH_TERMINATOR_SEGMENTS];
    earth_terminator(solar_dec_deg, solar_ra_hours, points);

    float r = config->globe_radius;
    float alpha = config->terminator_alpha;

    /* Terminator color: warm amber/gold for day-night boundary */
    float cr = 0.9f;
    float cg = 0.7f;
    float cb = 0.3f;

    for (int i = 0; i < ERP_TERMINATOR_POINTS; i++) {
        int base = i * ERP_LINE_VERTEX_FLOATS;
        out[base + 0] = points[i].x * r;
        out[base + 1] = points[i].y * r;
        out[base + 2] = points[i].z * r;
        out[base + 3] = cr;
        out[base + 4] = cg;
        out[base + 5] = cb;
        out[base + 6] = alpha;
    }

    return ERP_TERMINATOR_POINTS;
}

/* =========================================================
 *  Pack atmosphere shell
 * ========================================================= */

int erp_pack_atmosphere(const erp_config_t *config,
                         float *out_verts, unsigned int *out_indices)
{
    atmo_vertex_t atmo_verts[EARTH_ATMO_MAX_VERTICES];
    unsigned int atmo_indices[EARTH_ATMO_MAX_INDICES];

    atmo_mesh_info_t mesh = earth_atmo_shell_generate(
        (double)config->atmo_radius_ratio, atmo_verts, atmo_indices);

    float r = config->globe_radius;

    /* Repack atmo_vertex_t into interleaved float array:
     * position(vec3) + normal(vec3) + opacity(float) = 7 floats per vertex.
     * The shell is generated at atmo_radius_ratio scale on a unit sphere,
     * so we multiply positions by globe_radius to place in scene. */
    for (int i = 0; i < mesh.vertex_count; i++) {
        int base = i * 7;
        out_verts[base + 0] = atmo_verts[i].x * r;
        out_verts[base + 1] = atmo_verts[i].y * r;
        out_verts[base + 2] = atmo_verts[i].z * r;
        out_verts[base + 3] = atmo_verts[i].nx;
        out_verts[base + 4] = atmo_verts[i].ny;
        out_verts[base + 5] = atmo_verts[i].nz;
        out_verts[base + 6] = atmo_verts[i].opacity;
    }

    /* Copy indices */
    for (int i = 0; i < mesh.index_count; i++) {
        out_indices[i] = atmo_indices[i];
    }

    return mesh.vertex_count;
}

/* =========================================================
 *  GLSL ES 3.00 Shader Sources
 * ========================================================= */

/* ---- Globe vertex shader ---- */

const char *erp_globe_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "layout(location = 0) in vec3 a_position;\n"
        "layout(location = 1) in vec3 a_normal;\n"
        "layout(location = 2) in vec2 a_uv;\n"
        "\n"
        "uniform mat4 u_mvp;\n"
        "uniform mat4 u_model;\n"
        "uniform vec3 u_sun_dir;\n"
        "\n"
        "out vec3 v_world_normal;\n"
        "out vec3 v_world_pos;\n"
        "out vec2 v_uv;\n"
        "out float v_sun_dot;\n"
        "\n"
        "void main() {\n"
        "    vec4 world_pos = u_model * vec4(a_position, 1.0);\n"
        "    v_world_pos = world_pos.xyz;\n"
        "    v_world_normal = normalize(mat3(u_model) * a_normal);\n"
        "    v_uv = a_uv;\n"
        "    v_sun_dot = dot(v_world_normal, normalize(u_sun_dir));\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "}\n";
}

/* ---- Globe fragment shader ---- */

const char *erp_globe_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "uniform vec3 u_sun_dir;\n"
        "\n"
        "in vec3 v_world_normal;\n"
        "in vec3 v_world_pos;\n"
        "in vec2 v_uv;\n"
        "in float v_sun_dot;\n"
        "\n"
        "out vec4 frag_color;\n"
        "\n"
        "void main() {\n"
        "    // Day color: warm blue-green ocean\n"
        "    vec3 day_color = vec3(0.1, 0.35, 0.55);\n"
        "    // Night color: dark blue\n"
        "    vec3 night_color = vec3(0.02, 0.03, 0.08);\n"
        "\n"
        "    // Smooth terminator transition over ~10 degrees\n"
        "    float terminator = smoothstep(-0.15, 0.15, v_sun_dot);\n"
        "\n"
        "    // Simple latitude-based land hint (continental green)\n"
        "    float lat = v_uv.y * 3.14159 - 1.5708;\n"
        "    float land_hint = smoothstep(0.2, 0.8, \n"
        "        abs(sin(v_uv.x * 6.28318 * 3.0 + 1.0)) *\n"
        "        smoothstep(0.15, 0.75, 1.0 - abs(sin(lat))));\n"
        "    vec3 land_color = vec3(0.15, 0.35, 0.12);\n"
        "    vec3 surface = mix(day_color, land_color, land_hint * 0.3);\n"
        "\n"
        "    // Diffuse lighting\n"
        "    float diffuse = max(v_sun_dot, 0.0) * 0.6 + 0.4;\n"
        "    vec3 lit_day = surface * diffuse;\n"
        "\n"
        "    // City lights on night side (subtle)\n"
        "    float cities = step(0.7, fract(v_uv.x * 50.0 + v_uv.y * 30.0))\n"
        "                 * step(0.85, fract(v_uv.y * 40.0 + v_uv.x * 20.0))\n"
        "                 * 0.4;\n"
        "    vec3 night_lit = night_color + vec3(1.0, 0.9, 0.6) * cities;\n"
        "\n"
        "    vec3 final_color = mix(night_lit, lit_day, terminator);\n"
        "    frag_color = vec4(final_color, 1.0);\n"
        "}\n";
}

/* ---- Atmosphere vertex shader ---- */

const char *erp_atmo_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "layout(location = 0) in vec3 a_position;\n"
        "layout(location = 1) in vec3 a_normal;\n"
        "layout(location = 2) in float a_opacity;\n"
        "\n"
        "uniform mat4 u_mvp;\n"
        "uniform mat4 u_model;\n"
        "uniform vec3 u_camera_pos;\n"
        "\n"
        "out vec3 v_normal;\n"
        "out vec3 v_view_dir;\n"
        "out float v_opacity;\n"
        "\n"
        "void main() {\n"
        "    vec4 world_pos = u_model * vec4(a_position, 1.0);\n"
        "    v_normal = normalize(mat3(u_model) * a_normal);\n"
        "    v_view_dir = normalize(u_camera_pos - world_pos.xyz);\n"
        "    v_opacity = a_opacity;\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "}\n";
}

/* ---- Atmosphere fragment shader ---- */

const char *erp_atmo_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "uniform vec3 u_sun_dir;\n"
        "\n"
        "in vec3 v_normal;\n"
        "in vec3 v_view_dir;\n"
        "in float v_opacity;\n"
        "\n"
        "out vec4 frag_color;\n"
        "\n"
        "void main() {\n"
        "    /* Rayleigh scattering coefficients (relative, normalized) */\n"
        "    vec3 rayleigh_color = vec3(0.15, 0.4, 1.0);\n"
        "    vec3 sunset_color = vec3(1.0, 0.4, 0.1);\n"
        "\n"
        "    /* Sun angle at this point on atmosphere shell */\n"
        "    vec3 n = normalize(v_normal);\n"
        "    vec3 sun = normalize(u_sun_dir);\n"
        "    float sun_dot = dot(n, sun);\n"
        "\n"
        "    /* Color blend: blue on sun side, warm at terminator, dim on night */\n"
        "    float terminator = smoothstep(-0.3, 0.3, sun_dot);\n"
        "    float sunset_zone = 1.0 - abs(sun_dot);\n"
        "    sunset_zone = sunset_zone * sunset_zone;\n"
        "\n"
        "    vec3 day_atmo = rayleigh_color;\n"
        "    vec3 atmo_color = mix(day_atmo, sunset_color, sunset_zone * 0.6);\n"
        "\n"
        "    /* Limb glow: bright at edges (grazing angle), transparent at center */\n"
        "    float vdn = abs(dot(normalize(v_view_dir), n));\n"
        "    float limb = 1.0 - vdn;\n"
        "    float glow = limb * limb * limb;\n"
        "\n"
        "    /* Directional brightness: sun-lit side brighter */\n"
        "    float sun_brightness = 0.4 + 0.6 * max(terminator, 0.0);\n"
        "\n"
        "    float alpha = glow * v_opacity * sun_brightness * 0.8;\n"
        "    frag_color = vec4(atmo_color, alpha);\n"
        "}\n";
}

/* ---- Line vertex shader (coastlines + terminator) ---- */

const char *erp_line_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "layout(location = 0) in vec3 a_position;\n"
        "layout(location = 1) in vec4 a_color;\n"
        "\n"
        "uniform mat4 u_mvp;\n"
        "\n"
        "out vec4 v_color;\n"
        "\n"
        "void main() {\n"
        "    v_color = a_color;\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "}\n";
}

/* ---- Line fragment shader (coastlines + terminator) ---- */

const char *erp_line_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "in vec4 v_color;\n"
        "\n"
        "out vec4 frag_color;\n"
        "\n"
        "void main() {\n"
        "    frag_color = v_color;\n"
        "}\n";
}
