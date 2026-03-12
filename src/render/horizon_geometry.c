/* horizon_geometry.c — Horizon line geometry implementation.
 *
 * Pure functions for computing horizon great circle, altitude/azimuth grid,
 * cardinal points, and alt/az to XYZ conversion on a Y-up unit sphere.
 *
 * No GL, no malloc, no globals, no side effects. */

#include "horizon_geometry.h"
#include <math.h>

/* ---- Internal helpers ---- */

static void deg_to_rad_pair(double lat_deg, double lon_deg,
                             double *lat_rad, double *lon_rad)
{
    *lat_rad = lat_deg * PI / 180.0;
    *lon_rad = lon_deg * PI / 180.0;
}

/* Compute zenith unit vector (Y-up, matching earth_globe.h) */
static void compute_zenith(double lat_deg, double lon_deg, double z[3])
{
    double lat_r, lon_r;
    deg_to_rad_pair(lat_deg, lon_deg, &lat_r, &lon_r);
    z[0] = cos(lat_r) * cos(lon_r);
    z[1] = sin(lat_r);
    z[2] = cos(lat_r) * sin(lon_r);
}

/* Normalize a 3-vector in place. Returns length. */
static double normalize3(double v[3])
{
    double len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len > 1e-12) {
        v[0] /= len;
        v[1] /= len;
        v[2] /= len;
    }
    return len;
}

/* Cross product: out = a x b */
static void cross3(const double a[3], const double b[3], double out[3])
{
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

/* Build orthonormal basis (east, north_horizon) for the tangent plane at zenith.
 * east: points geographic east
 * north_hz: points geographic north along the horizon
 * zenith: the up direction at observer location */
static void build_horizon_basis(const double zenith[3],
                                 double east[3], double north_hz[3])
{
    /* East = normalize(cross(zenith, Y_up))
     * This gives geographic east (increasing longitude = +Z at equator).
     * At poles, Y_up is parallel to zenith, so use X_right instead. */
    double up[3] = {0.0, 1.0, 0.0};
    cross3(zenith, up, east);
    double elen = normalize3(east);
    if (elen < 1e-9) {
        /* At a pole: use (1, 0, 0) instead */
        double right[3] = {1.0, 0.0, 0.0};
        cross3(zenith, right, east);
        normalize3(east);
    }
    /* North along horizon = cross(east, zenith) — points towards geographic north */
    cross3(east, zenith, north_hz);
    normalize3(north_hz);
}

/* Compute a point at given altitude and azimuth from observer.
 * Compass convention: azimuth 0 = north, 90 = east.
 * altitude 0 = horizon, 90 = zenith.
 * Returns unit vector direction. */
static void altaz_direction(const double zenith[3],
                             const double north_hz[3],
                             const double east[3],
                             double alt_rad, double az_rad,
                             double out[3])
{
    /* The point at (alt, az) on the celestial sphere:
     * direction = sin(alt) * zenith + cos(alt) * (cos(az) * north + sin(az) * east) */
    double ca = cos(alt_rad);
    double sa = sin(alt_rad);
    double caz = cos(az_rad);
    double saz = sin(az_rad);

    out[0] = sa * zenith[0] + ca * (caz * north_hz[0] + saz * east[0]);
    out[1] = sa * zenith[1] + ca * (caz * north_hz[1] + saz * east[1]);
    out[2] = sa * zenith[2] + ca * (caz * north_hz[2] + saz * east[2]);
}

/* Write a vertex (position + color) into buffer */
static void write_vertex(float *buf, int idx,
                          float x, float y, float z,
                          float r, float g, float b, float a)
{
    int base = idx * HZG_VERTEX_FLOATS;
    buf[base + 0] = x;
    buf[base + 1] = y;
    buf[base + 2] = z;
    buf[base + 3] = r;
    buf[base + 4] = g;
    buf[base + 5] = b;
    buf[base + 6] = a;
}

/* ---- Public API ---- */

hzg_config_t hzg_default_config(void)
{
    hzg_config_t cfg;
    cfg.observer_lat_deg = 0.0;
    cfg.observer_lon_deg = 0.0;
    cfg.globe_radius = 1.0f;
    cfg.horizon_segments = 72;
    cfg.altitude_steps = 5;
    cfg.azimuth_lines = 12;
    return cfg;
}

void hzg_zenith(double lat_deg, double lon_deg, float xyz[3])
{
    double z[3];
    compute_zenith(lat_deg, lon_deg, z);
    xyz[0] = (float)z[0];
    xyz[1] = (float)z[1];
    xyz[2] = (float)z[2];
}

float hzg_angular_distance(double lat1, double lon1, double lat2, double lon2)
{
    double lat1_r = lat1 * PI / 180.0;
    double lon1_r = lon1 * PI / 180.0;
    double lat2_r = lat2 * PI / 180.0;
    double lon2_r = lon2 * PI / 180.0;

    double dlat = lat2_r - lat1_r;
    double dlon = lon2_r - lon1_r;

    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(lat1_r) * cos(lat2_r) *
               sin(dlon / 2.0) * sin(dlon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return (float)(c * 180.0 / PI);
}

int hzg_is_visible(double obs_lat, double obs_lon, double pt_lat, double pt_lon)
{
    float d = hzg_angular_distance(obs_lat, obs_lon, pt_lat, pt_lon);
    return d < 90.0f ? 1 : 0;
}

void hzg_altaz_to_xyz(double lat_deg, double lon_deg, float globe_radius,
                       double altitude_deg, double azimuth_deg, float xyz[3])
{
    double zenith[3], east[3], north_hz[3];
    compute_zenith(lat_deg, lon_deg, zenith);
    build_horizon_basis(zenith, east, north_hz);

    double alt_rad = altitude_deg * PI / 180.0;
    double az_rad = azimuth_deg * PI / 180.0;

    double dir[3];
    altaz_direction(zenith, north_hz, east, alt_rad, az_rad, dir);

    xyz[0] = (float)(dir[0] * (double)globe_radius);
    xyz[1] = (float)(dir[1] * (double)globe_radius);
    xyz[2] = (float)(dir[2] * (double)globe_radius);
}

int hzg_cardinals(hzg_config_t config, hzg_cardinal_t out[8])
{
    static const float azimuths[8] = {
        0.0f, 45.0f, 90.0f, 135.0f, 180.0f, 225.0f, 270.0f, 315.0f
    };
    static const char *labels[8] = {
        "N", "NE", "E", "SE", "S", "SW", "W", "NW"
    };

    for (int i = 0; i < 8; i++) {
        float xyz[3];
        hzg_altaz_to_xyz(config.observer_lat_deg, config.observer_lon_deg,
                          config.globe_radius,
                          0.0, (double)azimuths[i], xyz);
        out[i].x = xyz[0];
        out[i].y = xyz[1];
        out[i].z = xyz[2];
        out[i].azimuth_deg = azimuths[i];
        out[i].label = labels[i];
    }
    return 8;
}

hzg_info_t hzg_pack(hzg_config_t config, float *out_verts)
{
    hzg_info_t info;
    info.cardinal_count = 8;

    double zenith[3], east[3], north_hz[3];
    compute_zenith(config.observer_lat_deg, config.observer_lon_deg, zenith);
    build_horizon_basis(zenith, east, north_hz);

    float radius = config.globe_radius;
    int v = 0; /* vertex counter */

    /* ---- Horizon great circle (altitude = 0) ---- */
    int hsegs = config.horizon_segments;
    if (hsegs < 4) hsegs = 4;
    for (int i = 0; i <= hsegs; i++) {
        double az_rad = (double)i / (double)hsegs * 2.0 * PI;
        double dir[3];
        altaz_direction(zenith, north_hz, east, 0.0, az_rad, dir);

        /* Horizon line color: bright cyan-white */
        float r = 0.8f, g = 0.9f, b = 1.0f, a = 0.8f;

        /* Brighten at cardinal directions (every 90 deg => every hsegs/4) */
        if (hsegs >= 4) {
            int quarter = hsegs / 4;
            if (quarter > 0 && (i % quarter) == 0) {
                r = 1.0f; g = 1.0f; b = 1.0f; a = 1.0f;
            }
        }

        write_vertex(out_verts, v,
                      (float)(dir[0] * (double)radius),
                      (float)(dir[1] * (double)radius),
                      (float)(dir[2] * (double)radius),
                      r, g, b, a);
        v++;
        if (v >= HZG_MAX_VERTICES) break;
    }
    info.horizon_verts = v;

    /* ---- Altitude grid circles ---- */
    int alt_start = v;
    int nalt = config.altitude_steps;
    if (nalt < 0) nalt = 0;
    int alt_segs = hsegs; /* same resolution as horizon */
    for (int step = 1; step <= nalt; step++) {
        double alt_deg = (double)step * (90.0 / (double)(nalt + 1));
        double alt_rad = alt_deg * PI / 180.0;
        for (int i = 0; i <= alt_segs; i++) {
            double az_rad = (double)i / (double)alt_segs * 2.0 * PI;
            double dir[3];
            altaz_direction(zenith, north_hz, east, alt_rad, az_rad, dir);

            write_vertex(out_verts, v,
                          (float)(dir[0] * (double)radius),
                          (float)(dir[1] * (double)radius),
                          (float)(dir[2] * (double)radius),
                          0.5f, 0.6f, 0.7f, 0.4f);
            v++;
            if (v >= HZG_MAX_VERTICES) goto done;
        }
    }
    info.altitude_verts = v - alt_start;

    /* ---- Azimuth grid lines ---- */
    {
        int az_start = v;
        int naz = config.azimuth_lines;
        if (naz < 1) naz = 1;
        /* Each azimuth line goes from altitude ~75 deg down to 0 (horizon) */
        int line_steps = 16; /* segments per azimuth line */
        for (int ai = 0; ai < naz; ai++) {
            double az_deg = (double)ai * (360.0 / (double)naz);
            double az_rad = az_deg * PI / 180.0;
            for (int si = 0; si <= line_steps; si++) {
                /* Altitude from 75 degrees down to 0 */
                double alt_deg = 75.0 * (1.0 - (double)si / (double)line_steps);
                double alt_rad = alt_deg * PI / 180.0;
                double dir[3];
                altaz_direction(zenith, north_hz, east, alt_rad, az_rad, dir);

                write_vertex(out_verts, v,
                              (float)(dir[0] * (double)radius),
                              (float)(dir[1] * (double)radius),
                              (float)(dir[2] * (double)radius),
                              0.5f, 0.6f, 0.7f, 0.3f);
                v++;
                if (v >= HZG_MAX_VERTICES) goto done;
            }
        }
        info.azimuth_verts = v - az_start;
    }

done:
    info.total_verts = v;
    return info;
}

const char *hzg_vert_source(void)
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

const char *hzg_frag_source(void)
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
