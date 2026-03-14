/* weather_overlay.h — Weather overlay geometry: wind arrows, storm basins,
 * ITCZ band, pressure isobars as GPU-ready line vertex data.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 *
 * Coordinate system: Y-up right-handed, matching earth_globe.h.
 *   X = cos(lat) * cos(lon)
 *   Y = sin(lat)
 *   Z = cos(lat) * sin(lon)
 *
 * Vertex layout: position(vec3) + color(vec4) = 7 floats (GL_LINES).
 *
 * Data sources:
 *   - wind_patterns.h — prevailing winds, ITCZ latitude
 *   - storm_data.h — cyclone basin boundaries and seasons
 */

#ifndef TIME_WEATHER_OVERLAY_H
#define TIME_WEATHER_OVERLAY_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define WOV_VERTEX_FLOATS 7      /* position(3) + color(4) */
#define WOV_MAX_VERTICES 8000
#define WOV_ARROW_SEGMENTS 8     /* line segments per wind arrow glyph */

typedef struct {
    float globe_radius;          /* Earth sphere radius in scene units (default: 1.0) */
    int   month;                 /* current month 1-12 (for ITCZ position, storm season) */
    int   wind_arrows_per_band;  /* arrows per latitude band (default: 12, every 30 deg lon) */
    float arrow_scale;           /* arrow size in scene units (default: 0.05) */
    float arrow_alpha;           /* wind arrow opacity (default: 0.6) */
    float basin_alpha;           /* storm basin outline opacity (default: 0.4) */
    float itcz_alpha;            /* ITCZ band opacity (default: 0.5) */
    int   isobar_count;          /* pressure isobar circles (default: 5) */
    float isobar_alpha;          /* isobar opacity (default: 0.3) */
} wov_config_t;

/* Default configuration with sensible values. */
wov_config_t wov_default_config(void);

typedef struct {
    int wind_arrow_verts;        /* wind direction arrows */
    int basin_outline_verts;     /* storm basin boundary lines */
    int itcz_verts;              /* ITCZ band line strip */
    int isobar_verts;            /* pressure isobar circles */
    int total_verts;             /* sum of all */
} wov_info_t;

/* Pack all weather overlay geometry.
 * out_verts: WOV_MAX_VERTICES * WOV_VERTEX_FLOATS floats.
 * Returns info with vertex counts per layer. */
wov_info_t wov_pack(wov_config_t config, float *out_verts);

/* Pack only wind arrows at specified latitude bands.
 * out_verts: caller-provided buffer.
 * max_verts: maximum vertices to write.
 * Returns vertex count. */
int wov_pack_wind_arrows(wov_config_t config, float *out_verts, int max_verts);

/* Pack storm basin outlines for basins active in the given month.
 * Returns vertex count (GL_LINES pairs). */
int wov_pack_basin_outlines(wov_config_t config, float *out_verts, int max_verts);

/* Pack ITCZ band as line segments at seasonal latitude.
 * Returns vertex count. */
int wov_pack_itcz(wov_config_t config, float *out_verts, int max_verts);

/* Pack pressure isobar circles at regular latitude intervals.
 * Returns vertex count. */
int wov_pack_isobars(wov_config_t config, float *out_verts, int max_verts);

/* Convert lat/lon to XYZ on globe (Y-up, matches earth_globe.h). */
void wov_latlon_to_xyz(double lat_deg, double lon_deg, float radius, float xyz[3]);

/* Generate a wind arrow glyph at position pointing in wind direction.
 * dir_deg: wind FROM direction in degrees (0=N, 90=E). Arrow points opposite.
 * strength: 0.0-1.0 relative strength (scales arrow length).
 * scale: arrow size in scene units.
 * Returns vertex count (GL_LINES pairs). */
int wov_arrow_glyph(float pos[3], float dir_deg, float strength, float scale,
                     float color[4], float *out_verts, int max_verts);

/* Shader sources for weather overlay rendering (GLSL ES 3.00). */
const char *wov_vert_source(void);
const char *wov_frag_source(void);

#endif /* TIME_WEATHER_OVERLAY_H */
