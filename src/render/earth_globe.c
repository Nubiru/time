/* earth_globe.c — Pure geometry for rendering an Earth globe. */
#include "earth_globe.h"

#include <math.h>
#include <stddef.h>

/* ---- Internal helpers ---- */

static int clamp_int(int val, int lo, int hi)
{
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

/* ---- Vertex / index count ---- */

int earth_globe_vertex_count(int lat_seg, int lon_seg)
{
    return (lat_seg + 1) * (lon_seg + 1);
}

int earth_globe_index_count(int lat_seg, int lon_seg)
{
    return lat_seg * lon_seg * 6;
}

/* ---- UV-sphere generation ---- */

earth_mesh_info_t earth_globe_generate(int lat_seg, int lon_seg,
                                        earth_vertex_t *out_vertices,
                                        unsigned int *out_indices)
{
    lat_seg = clamp_int(lat_seg, 4, EARTH_GLOBE_MAX_LAT);
    lon_seg = clamp_int(lon_seg, 4, EARTH_GLOBE_MAX_LON);

    earth_mesh_info_t info;
    info.lat_segments = lat_seg;
    info.lon_segments = lon_seg;
    info.vertex_count = earth_globe_vertex_count(lat_seg, lon_seg);
    info.index_count = earth_globe_index_count(lat_seg, lon_seg);

    if (out_vertices != NULL) {
        int idx = 0;
        for (int i = 0; i <= lat_seg; i++) {
            double theta = PI * (double)i / (double)lat_seg;
            double sin_theta = sin(theta);
            double cos_theta = cos(theta);
            float v_coord = (float)i / (float)lat_seg;

            for (int j = 0; j <= lon_seg; j++) {
                double phi = 2.0 * PI * (double)j / (double)lon_seg;
                float x = (float)(sin_theta * cos(phi));
                float y = (float)(cos_theta);
                float z = (float)(sin_theta * sin(phi));

                out_vertices[idx].x = x;
                out_vertices[idx].y = y;
                out_vertices[idx].z = z;
                out_vertices[idx].nx = x;
                out_vertices[idx].ny = y;
                out_vertices[idx].nz = z;
                out_vertices[idx].u = (float)j / (float)lon_seg;
                out_vertices[idx].v = v_coord;
                idx++;
            }
        }
    }

    if (out_indices != NULL && out_vertices != NULL) {
        int idx = 0;
        int cols = lon_seg + 1;
        for (int i = 0; i < lat_seg; i++) {
            for (int j = 0; j < lon_seg; j++) {
                unsigned int tl = (unsigned int)(i * cols + j);
                unsigned int bl = (unsigned int)((i + 1) * cols + j);
                unsigned int br = (unsigned int)((i + 1) * cols + j + 1);
                unsigned int tr = (unsigned int)(i * cols + j + 1);

                /* Triangle 1 */
                out_indices[idx++] = tl;
                out_indices[idx++] = bl;
                out_indices[idx++] = br;

                /* Triangle 2 */
                out_indices[idx++] = tl;
                out_indices[idx++] = br;
                out_indices[idx++] = tr;
            }
        }
    }

    return info;
}

/* ---- Lat/Lon to XYZ ---- */

void earth_latlon_to_xyz(double lat_deg, double lon_deg,
                          float *out_x, float *out_y, float *out_z)
{
    double lat_rad = lat_deg * PI / 180.0;
    double lon_rad = lon_deg * PI / 180.0;
    *out_x = (float)(cos(lat_rad) * cos(lon_rad));
    *out_y = (float)(sin(lat_rad));
    *out_z = (float)(cos(lat_rad) * sin(lon_rad));
}

/* ---- Day/Night terminator ---- */

int earth_terminator(double solar_dec_deg, double solar_ra_hours,
                     terminator_point_t *out_points)
{
    /* Subsolar point: lat = declination, lon = RA in degrees */
    double sub_lat = solar_dec_deg;
    double sub_lon = solar_ra_hours * 15.0;

    /* Sun direction vector on unit sphere */
    float sx, sy, sz;
    earth_latlon_to_xyz(sub_lat, sub_lon, &sx, &sy, &sz);
    double sdx = (double)sx;
    double sdy = (double)sy;
    double sdz = (double)sz;

    /* Build orthonormal basis perpendicular to sun direction.
     * Choose a vector not parallel to sun direction to cross with. */
    double ax, ay, az;
    if (fabs(sdy) < 0.9) {
        /* Cross with Y-up (0, 1, 0) */
        ax = sdz;  /* sy*0 - sz*1 => -sz... let me do cross properly */
        /* sun x (0,1,0) = (sz*1 - sy*0, sx*0 - sz*0... no.
         * cross(S, Y) = (Sy*0 - Sz*1, Sz*0 - Sx*0, Sx*1 - Sy*0)
         * Nope. cross(a,b) = (ay*bz - az*by, az*bx - ax*bz, ax*by - ay*bx)
         * cross(S, (0,1,0)) = (Sy*0 - Sz*1, Sz*0 - Sx*0, Sx*1 - Sy*0)
         *                    = (-Sz, 0, Sx) */
        ax = -sdz;
        ay = 0.0;
        az = sdx;
    } else {
        /* Cross with X-right (1, 0, 0) */
        /* cross(S, (1,0,0)) = (Sy*0 - Sz*0, Sz*1 - Sx*0, Sx*0 - Sy*1)
         *                    = (0, Sz, -Sy) */
        ax = 0.0;
        ay = sdz;
        az = -sdy;
    }

    /* Normalize a */
    double a_len = sqrt(ax * ax + ay * ay + az * az);
    ax /= a_len;
    ay /= a_len;
    az /= a_len;

    /* Second basis vector: b = sun x a */
    double bx = sdy * az - sdz * ay;
    double by = sdz * ax - sdx * az;
    double bz = sdx * ay - sdy * ax;

    /* Normalize b (should already be unit, but be safe) */
    double b_len = sqrt(bx * bx + by * by + bz * bz);
    bx /= b_len;
    by /= b_len;
    bz /= b_len;

    for (int i = 0; i < EARTH_TERMINATOR_SEGMENTS; i++) {
        double angle = 2.0 * PI * (double)i / (double)EARTH_TERMINATOR_SEGMENTS;
        double ca = cos(angle);
        double sa = sin(angle);

        /* Point on great circle perpendicular to sun direction */
        double px = ax * ca + bx * sa;
        double py = ay * ca + by * sa;
        double pz = az * ca + bz * sa;

        out_points[i].x = (float)px;
        out_points[i].y = (float)py;
        out_points[i].z = (float)pz;

        /* Convert back to lat/lon */
        double lat_rad = asin(py);
        double lon_rad = atan2(pz, px);
        out_points[i].lat_deg = (float)(lat_rad * 180.0 / PI);
        out_points[i].lon_deg = (float)(lon_rad * 180.0 / PI);
    }

    return EARTH_TERMINATOR_SEGMENTS;
}

/* ---- Simplified coastline data ---- */

/* Approximate outlines for major landmasses in (lat, lon) degrees.
 * Each consecutive pair of points forms a segment. */

static const float coastline_raw[][4] = {
    /* North America (~16 segments) */
    { 48.0f, -67.0f,  42.0f, -70.0f},
    { 42.0f, -70.0f,  35.0f, -75.0f},
    { 35.0f, -75.0f,  30.0f, -81.0f},
    { 30.0f, -81.0f,  25.0f, -80.0f},
    { 25.0f, -80.0f,  25.0f, -97.0f},
    { 25.0f, -97.0f,  30.0f, -97.0f},
    { 30.0f, -97.0f,  32.0f,-117.0f},
    { 32.0f,-117.0f,  38.0f,-122.0f},
    { 38.0f,-122.0f,  48.0f,-124.0f},
    { 48.0f,-124.0f,  60.0f,-140.0f},
    { 60.0f,-140.0f,  70.0f,-160.0f},
    { 70.0f,-160.0f,  70.0f,-140.0f},
    { 70.0f,-140.0f,  60.0f, -95.0f},
    { 60.0f, -95.0f,  55.0f, -80.0f},
    { 55.0f, -80.0f,  48.0f, -67.0f},

    /* South America (~15 segments) */
    { 12.0f, -72.0f,   8.0f, -77.0f},
    {  8.0f, -77.0f,   0.0f, -80.0f},
    {  0.0f, -80.0f,  -5.0f, -81.0f},
    { -5.0f, -81.0f,  -6.0f, -35.0f},
    { -6.0f, -35.0f, -15.0f, -39.0f},
    {-15.0f, -39.0f, -23.0f, -43.0f},
    {-23.0f, -43.0f, -33.0f, -52.0f},
    {-33.0f, -52.0f, -42.0f, -64.0f},
    {-42.0f, -64.0f, -55.0f, -68.0f},
    {-55.0f, -68.0f, -55.0f, -64.0f},
    {-55.0f, -64.0f, -42.0f, -64.0f},
    {-42.0f, -64.0f, -33.0f, -71.0f},
    {-33.0f, -71.0f, -18.0f, -70.0f},
    {-18.0f, -70.0f,  -5.0f, -76.0f},
    { -5.0f, -76.0f,   0.0f, -80.0f},

    /* Europe (~17 segments) */
    { 36.0f,  -6.0f,  43.0f,  -9.0f},
    { 43.0f,  -9.0f,  47.0f,  -2.0f},
    { 47.0f,  -2.0f,  51.0f,   2.0f},
    { 51.0f,   2.0f,  54.0f,  10.0f},
    { 54.0f,  10.0f,  56.0f,  10.0f},
    { 56.0f,  10.0f,  60.0f,   5.0f},
    { 60.0f,   5.0f,  62.0f,   6.0f},
    { 62.0f,   6.0f,  65.0f,  14.0f},
    { 65.0f,  14.0f,  70.0f,  25.0f},
    { 70.0f,  25.0f,  70.0f,  30.0f},
    { 70.0f,  30.0f,  60.0f,  30.0f},
    { 60.0f,  30.0f,  55.0f,  20.0f},
    { 55.0f,  20.0f,  50.0f,  15.0f},
    { 50.0f,  15.0f,  45.0f,  12.0f},
    { 45.0f,  12.0f,  40.0f,  15.0f},
    { 40.0f,  15.0f,  36.0f,  10.0f},
    { 36.0f,  10.0f,  36.0f,  -6.0f},

    /* Africa (~13 segments) */
    { 35.0f,  -6.0f,  32.0f,  10.0f},
    { 32.0f,  10.0f,  30.0f,  32.0f},
    { 30.0f,  32.0f,  12.0f,  44.0f},
    { 12.0f,  44.0f,   0.0f,  42.0f},
    {  0.0f,  42.0f, -12.0f,  40.0f},
    {-12.0f,  40.0f, -25.0f,  33.0f},
    {-25.0f,  33.0f, -35.0f,  18.0f},
    {-35.0f,  18.0f, -35.0f,  20.0f},
    {-35.0f,  20.0f, -25.0f,  15.0f},
    {-25.0f,  15.0f,  -5.0f,  10.0f},
    { -5.0f,  10.0f,   5.0f,   0.0f},
    {  5.0f,   0.0f,  15.0f, -17.0f},
    { 15.0f, -17.0f,  35.0f,  -6.0f},

    /* Asia (~15 segments) */
    { 70.0f,  30.0f,  70.0f, 180.0f},
    { 70.0f, 180.0f,  55.0f, 165.0f},
    { 55.0f, 165.0f,  50.0f, 140.0f},
    { 50.0f, 140.0f,  40.0f, 130.0f},
    { 40.0f, 130.0f,  35.0f, 129.0f},
    { 35.0f, 129.0f,  30.0f, 120.0f},
    { 30.0f, 120.0f,  22.0f, 114.0f},
    { 22.0f, 114.0f,  10.0f, 107.0f},
    { 10.0f, 107.0f,   1.0f, 104.0f},
    {  1.0f, 104.0f,   7.0f,  80.0f},
    {  7.0f,  80.0f,  25.0f,  68.0f},
    { 25.0f,  68.0f,  30.0f,  48.0f},
    { 30.0f,  48.0f,  40.0f,  44.0f},
    { 40.0f,  44.0f,  45.0f,  40.0f},
    { 45.0f,  40.0f,  55.0f,  40.0f},
    { 55.0f,  40.0f,  70.0f,  30.0f},

    /* Australia (~8 segments) */
    {-12.0f, 130.0f, -12.0f, 142.0f},
    {-12.0f, 142.0f, -18.0f, 148.0f},
    {-18.0f, 148.0f, -28.0f, 153.0f},
    {-28.0f, 153.0f, -38.0f, 145.0f},
    {-38.0f, 145.0f, -35.0f, 137.0f},
    {-35.0f, 137.0f, -32.0f, 115.0f},
    {-32.0f, 115.0f, -22.0f, 114.0f},
    {-22.0f, 114.0f, -12.0f, 130.0f},

    /* Antarctica (~9 segments) */
    {-65.0f, -60.0f, -70.0f, -10.0f},
    {-70.0f, -10.0f, -70.0f,  30.0f},
    {-70.0f,  30.0f, -68.0f,  75.0f},
    {-68.0f,  75.0f, -66.0f, 110.0f},
    {-66.0f, 110.0f, -68.0f, 140.0f},
    {-68.0f, 140.0f, -72.0f, 170.0f},
    {-72.0f, 170.0f, -78.0f,-170.0f},
    {-78.0f,-170.0f, -75.0f,-120.0f},
    {-75.0f,-120.0f, -65.0f, -60.0f},
};

static const int COASTLINE_COUNT =
    (int)(sizeof(coastline_raw) / sizeof(coastline_raw[0]));

coastline_segment_t earth_coastline_get(int index)
{
    coastline_segment_t seg = {0.0f, 0.0f, 0.0f, 0.0f};
    if (index < 0 || index >= COASTLINE_COUNT) {
        return seg;
    }
    seg.lat1 = coastline_raw[index][0];
    seg.lon1 = coastline_raw[index][1];
    seg.lat2 = coastline_raw[index][2];
    seg.lon2 = coastline_raw[index][3];
    return seg;
}

int earth_coastline_count(void)
{
    return COASTLINE_COUNT;
}

/* ---- Day/Night test ---- */

int earth_is_dayside(double lat_deg, double lon_deg,
                     double solar_dec_deg, double solar_lon_deg)
{
    float px, py, pz;
    float sx, sy, sz;
    earth_latlon_to_xyz(lat_deg, lon_deg, &px, &py, &pz);
    earth_latlon_to_xyz(solar_dec_deg, solar_lon_deg, &sx, &sy, &sz);
    double dot = (double)px * (double)sx +
                 (double)py * (double)sy +
                 (double)pz * (double)sz;
    return dot > 0.0 ? 1 : 0;
}
