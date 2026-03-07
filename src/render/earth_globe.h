/* earth_globe.h — Pure geometry for rendering an Earth globe.
 * UV-sphere generation, day/night terminator, simplified coastlines. */
#ifndef TIME_EARTH_GLOBE_H
#define TIME_EARTH_GLOBE_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define EARTH_GLOBE_MAX_LAT 64
#define EARTH_GLOBE_MAX_LON 128
#define EARTH_GLOBE_MAX_VERTICES ((EARTH_GLOBE_MAX_LAT + 1) * (EARTH_GLOBE_MAX_LON + 1))
#define EARTH_GLOBE_MAX_INDICES (EARTH_GLOBE_MAX_LAT * EARTH_GLOBE_MAX_LON * 6)
#define EARTH_TERMINATOR_SEGMENTS 72
#define EARTH_COASTLINE_POINT_COUNT 200

typedef struct {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
} earth_vertex_t;

typedef struct {
    int vertex_count;
    int index_count;
    int lat_segments;
    int lon_segments;
} earth_mesh_info_t;

typedef struct {
    float x, y, z;
    float lat_deg;
    float lon_deg;
} terminator_point_t;

typedef struct {
    float lat1, lon1;
    float lat2, lon2;
} coastline_segment_t;

/* Generate a UV-sphere mesh. Clamps segments to [4, MAX].
 * If out_vertices or out_indices is NULL, returns counts only. */
earth_mesh_info_t earth_globe_generate(int lat_seg, int lon_seg,
                                        earth_vertex_t *out_vertices,
                                        unsigned int *out_indices);

/* Query vertex count for given segment counts. */
int earth_globe_vertex_count(int lat_seg, int lon_seg);

/* Query index count for given segment counts. */
int earth_globe_index_count(int lat_seg, int lon_seg);

/* Compute 72 day/night terminator points on the unit sphere.
 * solar_dec_deg: solar declination in degrees.
 * solar_ra_hours: solar right ascension in hours.
 * Returns EARTH_TERMINATOR_SEGMENTS. */
int earth_terminator(double solar_dec_deg, double solar_ra_hours,
                     terminator_point_t *out_points);

/* Convert latitude/longitude (degrees) to XYZ on unit sphere (Y-up). */
void earth_latlon_to_xyz(double lat_deg, double lon_deg,
                          float *out_x, float *out_y, float *out_z);

/* Get a simplified coastline segment by index.
 * Returns zero segment if index is out of range. */
coastline_segment_t earth_coastline_get(int index);

/* Return the number of coastline segments. */
int earth_coastline_count(void);

/* Test if a point is on the day side of Earth.
 * Returns 1 for day, 0 for night. */
int earth_is_dayside(double lat_deg, double lon_deg,
                     double solar_dec_deg, double solar_lon_deg);

#endif /* TIME_EARTH_GLOBE_H */
