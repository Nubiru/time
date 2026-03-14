/* weather_overlay.c — Weather overlay geometry: wind arrows, storm basins,
 * ITCZ band, pressure isobars packed as GPU-ready line vertex data.
 * Pure module: no GL calls, no malloc, no globals, no side effects. */

#include "weather_overlay.h"
#include "../systems/earth/wind_patterns.h"
#include "../systems/earth/storm_data.h"

#include <math.h>

/* ============================================================
 * Internal helpers
 * ============================================================ */

static void emit_vertex(float *buf, int idx, float x, float y, float z,
                         float r, float g, float b, float a)
{
    int off = idx * WOV_VERTEX_FLOATS;
    buf[off + 0] = x;
    buf[off + 1] = y;
    buf[off + 2] = z;
    buf[off + 3] = r;
    buf[off + 4] = g;
    buf[off + 5] = b;
    buf[off + 6] = a;
}

/* Normalize a 3-vector in place. Returns length. */
static float normalize3(float v[3])
{
    float len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len > 1e-8f) {
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
    return len;
}

/* Cross product: out = a x b */
static void cross3(const float a[3], const float b[3], float out[3])
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

/* Wind cell color based on latitude band */
static void wind_color_for_lat(double abs_lat, float rgba[4], float alpha)
{
    if (abs_lat < 30.0) {
        /* Trade winds: blue */
        rgba[0] = 0.3f; rgba[1] = 0.5f; rgba[2] = 0.9f; rgba[3] = alpha;
    } else if (abs_lat < 60.0) {
        /* Westerlies: green */
        rgba[0] = 0.3f; rgba[1] = 0.8f; rgba[2] = 0.4f; rgba[3] = alpha;
    } else {
        /* Polar easterlies: white-blue */
        rgba[0] = 0.8f; rgba[1] = 0.85f; rgba[2] = 0.9f; rgba[3] = alpha;
    }
}

/* ============================================================
 * wov_latlon_to_xyz — matches earth_globe.h exactly
 * ============================================================ */

void wov_latlon_to_xyz(double lat_deg, double lon_deg, float radius, float xyz[3])
{
    double lat_rad = lat_deg * PI / 180.0;
    double lon_rad = lon_deg * PI / 180.0;
    xyz[0] = (float)(cos(lat_rad) * cos(lon_rad)) * radius;
    xyz[1] = (float)(sin(lat_rad)) * radius;
    xyz[2] = (float)(cos(lat_rad) * sin(lon_rad)) * radius;
}

/* ============================================================
 * wov_default_config
 * ============================================================ */

wov_config_t wov_default_config(void)
{
    wov_config_t cfg;
    cfg.globe_radius = 1.0f;
    cfg.month = 7;
    cfg.wind_arrows_per_band = 12;
    cfg.arrow_scale = 0.05f;
    cfg.arrow_alpha = 0.6f;
    cfg.basin_alpha = 0.4f;
    cfg.itcz_alpha = 0.5f;
    cfg.isobar_count = 5;
    cfg.isobar_alpha = 0.3f;
    return cfg;
}

/* ============================================================
 * wov_arrow_glyph — wind arrow at a point on the globe
 * ============================================================ */

int wov_arrow_glyph(float pos[3], float dir_deg, float strength, float scale,
                     float color[4], float *out_verts, int max_verts)
{
    if (strength <= 0.0f || scale <= 0.0f) return 0;

    /* We need 6 vertices (3 line segments): shaft(2) + 2 barbs(2+2) */
    int needed = 6;
    if (max_verts < 2) return 0;

    /* Normal at position = normalized position (sphere) */
    float normal[3] = {pos[0], pos[1], pos[2]};
    normalize3(normal);

    /* Tangent east: cross(Y_up, normal), handling poles */
    float y_up[3] = {0.0f, 1.0f, 0.0f};
    float east[3];
    cross3(y_up, normal, east);
    float east_len = normalize3(east);

    if (east_len < 1e-6f) {
        /* At poles, use X as reference */
        float x_right[3] = {1.0f, 0.0f, 0.0f};
        cross3(x_right, normal, east);
        normalize3(east);
    }

    /* Tangent north: cross(normal, east) */
    float north[3];
    cross3(normal, east, north);
    normalize3(north);

    /* Wind is FROM dir_deg, arrow points TO = dir_deg + 180.
     * Compass convention: 0=N, 90=E, 180=S, 270=W.
     * In tangent plane: north=0, east=90.
     * Arrow direction angle from north, clockwise. */
    float to_deg = dir_deg + 180.0f;
    float to_rad = (float)(to_deg * PI / 180.0);

    /* Arrow direction in tangent plane:
     * dir = sin(angle)*east + cos(angle)*north
     * (sin for eastward component, cos for northward) */
    float dir[3];
    float sin_a = sinf(to_rad);
    float cos_a = cosf(to_rad);
    dir[0] = sin_a * east[0] + cos_a * north[0];
    dir[1] = sin_a * east[1] + cos_a * north[1];
    dir[2] = sin_a * east[2] + cos_a * north[2];

    float arrow_len = scale * strength;

    /* Arrow tip = pos + dir * arrow_len */
    float tip[3];
    tip[0] = pos[0] + dir[0] * arrow_len;
    tip[1] = pos[1] + dir[1] * arrow_len;
    tip[2] = pos[2] + dir[2] * arrow_len;

    int vi = 0;

    /* Shaft: pos -> tip */
    emit_vertex(out_verts, vi++, pos[0], pos[1], pos[2],
                color[0], color[1], color[2], color[3]);
    emit_vertex(out_verts, vi++, tip[0], tip[1], tip[2],
                color[0], color[1], color[2], color[3]);

    if (vi >= max_verts) return vi;

    /* Barb length = 30% of shaft */
    float barb_len = arrow_len * 0.3f;
    float barb_angle = 30.0f * (float)PI / 180.0f;

    /* Perpendicular in tangent plane */
    float perp[3];
    cross3(dir, normal, perp);
    normalize3(perp);

    /* Two barbs at +-30 degrees from shaft at the tip */
    float cos_b = cosf(barb_angle);
    float sin_b = sinf(barb_angle);

    if (max_verts - vi >= 2) {
        /* Left barb: tip -> tip - dir*cos_b*barb + perp*sin_b*barb */
        float bl[3];
        bl[0] = tip[0] - (dir[0] * cos_b + perp[0] * sin_b) * barb_len;
        bl[1] = tip[1] - (dir[1] * cos_b + perp[1] * sin_b) * barb_len;
        bl[2] = tip[2] - (dir[2] * cos_b + perp[2] * sin_b) * barb_len;
        emit_vertex(out_verts, vi++, tip[0], tip[1], tip[2],
                    color[0], color[1], color[2], color[3]);
        emit_vertex(out_verts, vi++, bl[0], bl[1], bl[2],
                    color[0], color[1], color[2], color[3]);
    }

    if (max_verts - vi >= 2 && vi + 2 <= needed) {
        /* Right barb: tip -> tip - dir*cos_b*barb - perp*sin_b*barb */
        float br[3];
        br[0] = tip[0] - (dir[0] * cos_b - perp[0] * sin_b) * barb_len;
        br[1] = tip[1] - (dir[1] * cos_b - perp[1] * sin_b) * barb_len;
        br[2] = tip[2] - (dir[2] * cos_b - perp[2] * sin_b) * barb_len;
        emit_vertex(out_verts, vi++, tip[0], tip[1], tip[2],
                    color[0], color[1], color[2], color[3]);
        emit_vertex(out_verts, vi++, br[0], br[1], br[2],
                    color[0], color[1], color[2], color[3]);
    }

    return vi;
}

/* ============================================================
 * wov_pack_wind_arrows
 * ============================================================ */

/* Latitude bands for wind arrows */
static const double WIND_LAT_BANDS[] = {
    75.0, 60.0, 45.0, 30.0, 15.0, 0.0,
    -15.0, -30.0, -45.0, -60.0, -75.0
};
#define WIND_BAND_COUNT 11

int wov_pack_wind_arrows(wov_config_t config, float *out_verts, int max_verts)
{
    int total = 0;
    int arrows_per_band = config.wind_arrows_per_band;
    if (arrows_per_band <= 0) arrows_per_band = 12;
    double lon_step = 360.0 / (double)arrows_per_band;

    for (int b = 0; b < WIND_BAND_COUNT; b++) {
        double lat = WIND_LAT_BANDS[b];
        wind_pattern_t wp = wind_prevailing(lat);
        float rgba[4];
        wind_color_for_lat(fabs(lat), rgba, config.arrow_alpha);

        for (int a = 0; a < arrows_per_band; a++) {
            double lon = -180.0 + lon_step * (double)a;
            float pos[3];
            wov_latlon_to_xyz(lat, lon, config.globe_radius, pos);

            int remaining = max_verts - total;
            if (remaining < 2) return total;

            int n = wov_arrow_glyph(pos, (float)wp.direction_deg,
                                     (float)wp.strength_relative,
                                     config.arrow_scale, rgba,
                                     out_verts + total * WOV_VERTEX_FLOATS,
                                     remaining);
            total += n;
        }
    }

    return total;
}

/* ============================================================
 * wov_pack_basin_outlines
 * ============================================================ */

/* Pack a rectangle on the sphere as GL_LINES segments.
 * Each side is sampled with steps points for curvature. */
static int pack_basin_rect(double lat_min, double lat_max,
                            double lon_min, double lon_max,
                            float radius, float color[4],
                            float *out_verts, int max_verts)
{
    int steps = 10;
    int vi = 0;

    /* Bottom edge: lat_min, lon_min -> lon_max */
    for (int i = 0; i < steps && vi + 2 <= max_verts; i++) {
        double t0 = (double)i / (double)steps;
        double t1 = (double)(i + 1) / (double)steps;
        double lon0 = lon_min + t0 * (lon_max - lon_min);
        double lon1 = lon_min + t1 * (lon_max - lon_min);
        float p0[3], p1[3];
        wov_latlon_to_xyz(lat_min, lon0, radius, p0);
        wov_latlon_to_xyz(lat_min, lon1, radius, p1);
        emit_vertex(out_verts, vi++, p0[0], p0[1], p0[2],
                    color[0], color[1], color[2], color[3]);
        emit_vertex(out_verts, vi++, p1[0], p1[1], p1[2],
                    color[0], color[1], color[2], color[3]);
    }

    /* Top edge: lat_max, lon_min -> lon_max */
    for (int i = 0; i < steps && vi + 2 <= max_verts; i++) {
        double t0 = (double)i / (double)steps;
        double t1 = (double)(i + 1) / (double)steps;
        double lon0 = lon_min + t0 * (lon_max - lon_min);
        double lon1 = lon_min + t1 * (lon_max - lon_min);
        float p0[3], p1[3];
        wov_latlon_to_xyz(lat_max, lon0, radius, p0);
        wov_latlon_to_xyz(lat_max, lon1, radius, p1);
        emit_vertex(out_verts, vi++, p0[0], p0[1], p0[2],
                    color[0], color[1], color[2], color[3]);
        emit_vertex(out_verts, vi++, p1[0], p1[1], p1[2],
                    color[0], color[1], color[2], color[3]);
    }

    /* Left edge: lon_min, lat_min -> lat_max */
    for (int i = 0; i < steps && vi + 2 <= max_verts; i++) {
        double t0 = (double)i / (double)steps;
        double t1 = (double)(i + 1) / (double)steps;
        double lat0 = lat_min + t0 * (lat_max - lat_min);
        double lat1 = lat_min + t1 * (lat_max - lat_min);
        float p0[3], p1[3];
        wov_latlon_to_xyz(lat0, lon_min, radius, p0);
        wov_latlon_to_xyz(lat1, lon_min, radius, p1);
        emit_vertex(out_verts, vi++, p0[0], p0[1], p0[2],
                    color[0], color[1], color[2], color[3]);
        emit_vertex(out_verts, vi++, p1[0], p1[1], p1[2],
                    color[0], color[1], color[2], color[3]);
    }

    /* Right edge: lon_max, lat_min -> lat_max */
    for (int i = 0; i < steps && vi + 2 <= max_verts; i++) {
        double t0 = (double)i / (double)steps;
        double t1 = (double)(i + 1) / (double)steps;
        double lat0 = lat_min + t0 * (lat_max - lat_min);
        double lat1 = lat_min + t1 * (lat_max - lat_min);
        float p0[3], p1[3];
        wov_latlon_to_xyz(lat0, lon_max, radius, p0);
        wov_latlon_to_xyz(lat1, lon_max, radius, p1);
        emit_vertex(out_verts, vi++, p0[0], p0[1], p0[2],
                    color[0], color[1], color[2], color[3]);
        emit_vertex(out_verts, vi++, p1[0], p1[1], p1[2],
                    color[0], color[1], color[2], color[3]);
    }

    return vi;
}

int wov_pack_basin_outlines(wov_config_t config, float *out_verts, int max_verts)
{
    int total = 0;
    float basin_color[4] = {0.9f, 0.4f, 0.2f, config.basin_alpha};

    for (int i = 0; i < STORM_BASIN_COUNT; i++) {
        cyclone_basin_t b = storm_basin_get(i);
        if (!storm_basin_active(b.id, config.month)) continue;

        int remaining = max_verts - total;
        if (remaining < 2) break;

        int n = pack_basin_rect(b.lat_min, b.lat_max,
                                 b.lon_min, b.lon_max,
                                 config.globe_radius, basin_color,
                                 out_verts + total * WOV_VERTEX_FLOATS,
                                 remaining);
        total += n;
    }

    return total;
}

/* ============================================================
 * wov_pack_itcz
 * ============================================================ */

int wov_pack_itcz(wov_config_t config, float *out_verts, int max_verts)
{
    double itcz_lat = wind_itcz_latitude(config.month);
    float color[4] = {0.9f, 0.7f, 0.2f, config.itcz_alpha};

    int segments = 72;
    int vi = 0;

    for (int i = 0; i < segments && vi + 2 <= max_verts; i++) {
        double lon0 = -180.0 + 360.0 * (double)i / (double)segments;
        double lon1 = -180.0 + 360.0 * (double)(i + 1) / (double)segments;
        float p0[3], p1[3];
        wov_latlon_to_xyz(itcz_lat, lon0, config.globe_radius, p0);
        wov_latlon_to_xyz(itcz_lat, lon1, config.globe_radius, p1);
        emit_vertex(out_verts, vi++, p0[0], p0[1], p0[2],
                    color[0], color[1], color[2], color[3]);
        emit_vertex(out_verts, vi++, p1[0], p1[1], p1[2],
                    color[0], color[1], color[2], color[3]);
    }

    return vi;
}

/* ============================================================
 * wov_pack_isobars
 * ============================================================ */

/* Isobars at subtropical highs (~30 deg) and subpolar lows (~60 deg).
 * We distribute isobar_count circles across both hemispheres. */

int wov_pack_isobars(wov_config_t config, float *out_verts, int max_verts)
{
    if (config.isobar_count <= 0) return 0;

    float color[4] = {0.5f, 0.5f, 0.5f, config.isobar_alpha};
    int segments_per_circle = 36;
    int vi = 0;

    /* Place isobars at latitude intervals from equator to poles,
     * both hemispheres. Step = 90 / (count + 1). */
    double lat_step = 90.0 / (double)(config.isobar_count + 1);

    for (int c = 0; c < config.isobar_count; c++) {
        double lat = lat_step * (double)(c + 1);

        /* Northern hemisphere */
        for (int i = 0; i < segments_per_circle && vi + 2 <= max_verts; i++) {
            double lon0 = -180.0 + 360.0 * (double)i / (double)segments_per_circle;
            double lon1 = -180.0 + 360.0 * (double)(i + 1) / (double)segments_per_circle;
            float p0[3], p1[3];
            wov_latlon_to_xyz(lat, lon0, config.globe_radius, p0);
            wov_latlon_to_xyz(lat, lon1, config.globe_radius, p1);
            emit_vertex(out_verts, vi++, p0[0], p0[1], p0[2],
                        color[0], color[1], color[2], color[3]);
            emit_vertex(out_verts, vi++, p1[0], p1[1], p1[2],
                        color[0], color[1], color[2], color[3]);
        }

        /* Southern hemisphere */
        for (int i = 0; i < segments_per_circle && vi + 2 <= max_verts; i++) {
            double lon0 = -180.0 + 360.0 * (double)i / (double)segments_per_circle;
            double lon1 = -180.0 + 360.0 * (double)(i + 1) / (double)segments_per_circle;
            float p0[3], p1[3];
            wov_latlon_to_xyz(-lat, lon0, config.globe_radius, p0);
            wov_latlon_to_xyz(-lat, lon1, config.globe_radius, p1);
            emit_vertex(out_verts, vi++, p0[0], p0[1], p0[2],
                        color[0], color[1], color[2], color[3]);
            emit_vertex(out_verts, vi++, p1[0], p1[1], p1[2],
                        color[0], color[1], color[2], color[3]);
        }
    }

    return vi;
}

/* ============================================================
 * wov_pack — full pack
 * ============================================================ */

wov_info_t wov_pack(wov_config_t config, float *out_verts)
{
    wov_info_t info;
    int offset = 0;
    int remaining = WOV_MAX_VERTICES;

    info.wind_arrow_verts = wov_pack_wind_arrows(config,
        out_verts + offset * WOV_VERTEX_FLOATS, remaining);
    offset += info.wind_arrow_verts;
    remaining = WOV_MAX_VERTICES - offset;
    if (remaining < 0) remaining = 0;

    info.basin_outline_verts = wov_pack_basin_outlines(config,
        out_verts + offset * WOV_VERTEX_FLOATS, remaining);
    offset += info.basin_outline_verts;
    remaining = WOV_MAX_VERTICES - offset;
    if (remaining < 0) remaining = 0;

    info.itcz_verts = wov_pack_itcz(config,
        out_verts + offset * WOV_VERTEX_FLOATS, remaining);
    offset += info.itcz_verts;
    remaining = WOV_MAX_VERTICES - offset;
    if (remaining < 0) remaining = 0;

    info.isobar_verts = wov_pack_isobars(config,
        out_verts + offset * WOV_VERTEX_FLOATS, remaining);
    offset += info.isobar_verts;

    info.total_verts = offset;
    return info;
}

/* ============================================================
 * Shaders (GLSL ES 3.00)
 * ============================================================ */

const char *wov_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "uniform mat4 u_mvp;\n"
        "layout(location = 0) in vec3 a_position;\n"
        "layout(location = 1) in vec4 a_color;\n"
        "out vec4 v_color;\n"
        "void main() {\n"
        "    v_color = a_color;\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "}\n";
}

const char *wov_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec4 v_color;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "    frag_color = v_color;\n"
        "}\n";
}
