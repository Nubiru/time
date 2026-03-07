#include "../unity/unity.h"
#include "../../src/render/earth_globe.h"

#include <math.h>

#define TOL 0.001f
#define DTOL 0.001

void setUp(void) {}
void tearDown(void) {}

/* ---- Vertex count helpers (3 tests) ---- */

/* 1. Vertex count for minimum segments */
void test_vertex_count_min(void)
{
    int count = earth_globe_vertex_count(4, 4);
    TEST_ASSERT_EQUAL_INT(25, count); /* (4+1)*(4+1) */
}

/* 2. Vertex count for typical segments */
void test_vertex_count_typical(void)
{
    int count = earth_globe_vertex_count(16, 32);
    TEST_ASSERT_EQUAL_INT(561, count); /* (16+1)*(32+1) */
}

/* 3. Vertex count for max segments */
void test_vertex_count_max(void)
{
    int count = earth_globe_vertex_count(64, 128);
    TEST_ASSERT_EQUAL_INT(EARTH_GLOBE_MAX_VERTICES, count);
}

/* ---- Index count helpers (3 tests) ---- */

/* 4. Index count for minimum segments */
void test_index_count_min(void)
{
    int count = earth_globe_index_count(4, 4);
    TEST_ASSERT_EQUAL_INT(96, count); /* 4*4*6 */
}

/* 5. Index count for typical segments */
void test_index_count_typical(void)
{
    int count = earth_globe_index_count(16, 32);
    TEST_ASSERT_EQUAL_INT(3072, count); /* 16*32*6 */
}

/* 6. Index count for max */
void test_index_count_max(void)
{
    int count = earth_globe_index_count(64, 128);
    TEST_ASSERT_EQUAL_INT(EARTH_GLOBE_MAX_INDICES, count);
}

/* ---- Globe generate: mesh info (2 tests) ---- */

/* 7. Mesh info correct for 8x16 */
void test_generate_mesh_info(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_mesh_info_t info = earth_globe_generate(8, 16, verts, indices);
    TEST_ASSERT_EQUAL_INT(153, info.vertex_count);  /* (8+1)*(16+1) */
    TEST_ASSERT_EQUAL_INT(768, info.index_count);    /* 8*16*6 */
    TEST_ASSERT_EQUAL_INT(8, info.lat_segments);
    TEST_ASSERT_EQUAL_INT(16, info.lon_segments);
}

/* 8. Mesh info with different segments */
void test_generate_mesh_info_4x8(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_mesh_info_t info = earth_globe_generate(4, 8, verts, indices);
    TEST_ASSERT_EQUAL_INT(45, info.vertex_count);  /* (4+1)*(8+1) */
    TEST_ASSERT_EQUAL_INT(192, info.index_count);  /* 4*8*6 */
}

/* ---- Globe generate: NULL buffers return counts only (2 tests) ---- */

/* 9. NULL vertices returns counts */
void test_generate_null_vertices(void)
{
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_mesh_info_t info = earth_globe_generate(8, 16, NULL, indices);
    TEST_ASSERT_EQUAL_INT(153, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(768, info.index_count);
}

/* 10. NULL both returns counts */
void test_generate_null_both(void)
{
    earth_mesh_info_t info = earth_globe_generate(8, 16, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(153, info.vertex_count);
    TEST_ASSERT_EQUAL_INT(768, info.index_count);
}

/* ---- Globe generate: clamping (2 tests) ---- */

/* 11. Lat segments clamped to min 4 */
void test_generate_clamp_lat_min(void)
{
    earth_mesh_info_t info = earth_globe_generate(2, 8, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(4, info.lat_segments);
    TEST_ASSERT_EQUAL_INT(45, info.vertex_count); /* (4+1)*(8+1) */
}

/* 12. Lon segments clamped to min 4 */
void test_generate_clamp_lon_min(void)
{
    earth_mesh_info_t info = earth_globe_generate(8, 1, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(4, info.lon_segments);
    TEST_ASSERT_EQUAL_INT(45, info.vertex_count); /* (8+1)*(4+1) */
}

/* ---- Globe generate: pole vertices (2 tests) ---- */

/* 13. North pole vertex (first ring, i=0) */
void test_generate_north_pole(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_globe_generate(8, 16, verts, indices);
    /* i=0 (north pole): theta=0, y=cos(0)=1, x=sin(0)*cos(phi)=0, z=0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[0].x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, verts[0].y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[0].z);
}

/* 14. South pole vertex (last ring, i=lat_seg) */
void test_generate_south_pole(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_mesh_info_t info = earth_globe_generate(8, 16, verts, indices);
    /* Last ring: i=8, theta=PI, y=cos(PI)=-1, x=sin(PI)*cos(phi)~0, z~0 */
    int last_ring_start = 8 * (16 + 1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[last_ring_start].x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, verts[last_ring_start].y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[last_ring_start].z);
    (void)info;
}

/* ---- Globe generate: equator vertex (1 test) ---- */

/* 15. Equator vertex at j=0 */
void test_generate_equator(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_globe_generate(8, 16, verts, indices);
    /* Equator: i=4 (halfway), theta=PI/2, j=0, phi=0 */
    /* x=sin(PI/2)*cos(0)=1, y=cos(PI/2)=0, z=sin(PI/2)*sin(0)=0 */
    int eq_idx = 4 * (16 + 1); /* i=4, j=0 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, verts[eq_idx].x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[eq_idx].y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[eq_idx].z);
}

/* ---- Globe generate: normals are unit length (2 tests) ---- */

/* 16. Normal at north pole is unit length */
void test_generate_normal_unit_pole(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_globe_generate(8, 16, verts, indices);
    float nx = verts[0].nx, ny = verts[0].ny, nz = verts[0].nz;
    float len = sqrtf(nx * nx + ny * ny + nz * nz);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, len);
}

/* 17. Normal at equator is unit length */
void test_generate_normal_unit_equator(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_globe_generate(8, 16, verts, indices);
    int eq_idx = 4 * (16 + 1) + 4; /* somewhere on equator */
    float nx = verts[eq_idx].nx, ny = verts[eq_idx].ny, nz = verts[eq_idx].nz;
    float len = sqrtf(nx * nx + ny * ny + nz * nz);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, len);
}

/* ---- Globe generate: normals equal position (1 test) ---- */

/* 18. Normal equals position for unit sphere */
void test_generate_normal_equals_position(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_globe_generate(8, 16, verts, indices);
    int idx = 3 * (16 + 1) + 5; /* arbitrary vertex */
    TEST_ASSERT_FLOAT_WITHIN(TOL, verts[idx].x, verts[idx].nx);
    TEST_ASSERT_FLOAT_WITHIN(TOL, verts[idx].y, verts[idx].ny);
    TEST_ASSERT_FLOAT_WITHIN(TOL, verts[idx].z, verts[idx].nz);
}

/* ---- Globe generate: UV coordinates (2 tests) ---- */

/* 19. First vertex UV is (0, 0) */
void test_generate_uv_first(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_globe_generate(8, 16, verts, indices);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[0].u);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, verts[0].v);
}

/* 20. Last vertex UV is (1, 1) */
void test_generate_uv_last(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_mesh_info_t info = earth_globe_generate(8, 16, verts, indices);
    int last = info.vertex_count - 1;
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, verts[last].u);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, verts[last].v);
}

/* ---- Lat/Lon to XYZ (4 tests) ---- */

/* 21. Equator + prime meridian -> (1, 0, 0) */
void test_latlon_to_xyz_equator_prime(void)
{
    float x, y, z;
    earth_latlon_to_xyz(0.0, 0.0, &x, &y, &z);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, z);
}

/* 22. North pole -> (0, 1, 0) */
void test_latlon_to_xyz_north_pole(void)
{
    float x, y, z;
    earth_latlon_to_xyz(90.0, 0.0, &x, &y, &z);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, z);
}

/* 23. South pole -> (0, -1, 0) */
void test_latlon_to_xyz_south_pole(void)
{
    float x, y, z;
    earth_latlon_to_xyz(-90.0, 0.0, &x, &y, &z);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, -1.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, z);
}

/* 24. Equator + 90E -> (0, 0, 1) */
void test_latlon_to_xyz_90east(void)
{
    float x, y, z;
    earth_latlon_to_xyz(0.0, 90.0, &x, &y, &z);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, z);
}

/* ---- Terminator (3 tests) ---- */

/* 25. Terminator returns 72 points */
void test_terminator_count(void)
{
    terminator_point_t pts[EARTH_TERMINATOR_SEGMENTS];
    int count = earth_terminator(0.0, 0.0, pts);
    TEST_ASSERT_EQUAL_INT(EARTH_TERMINATOR_SEGMENTS, count);
}

/* 26. All terminator points are on the unit sphere */
void test_terminator_on_unit_sphere(void)
{
    terminator_point_t pts[EARTH_TERMINATOR_SEGMENTS];
    earth_terminator(23.4, 6.0, pts);
    for (int i = 0; i < EARTH_TERMINATOR_SEGMENTS; i++) {
        float len = sqrtf(pts[i].x * pts[i].x +
                          pts[i].y * pts[i].y +
                          pts[i].z * pts[i].z);
        TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, len);
    }
}

/* 27. Terminator points vary with declination */
void test_terminator_varies_with_dec(void)
{
    terminator_point_t pts_a[EARTH_TERMINATOR_SEGMENTS];
    terminator_point_t pts_b[EARTH_TERMINATOR_SEGMENTS];
    earth_terminator(0.0, 0.0, pts_a);
    earth_terminator(23.4, 0.0, pts_b);
    /* At least one point should differ significantly */
    int differ = 0;
    for (int i = 0; i < EARTH_TERMINATOR_SEGMENTS; i++) {
        float dx = pts_a[i].x - pts_b[i].x;
        float dy = pts_a[i].y - pts_b[i].y;
        float dz = pts_a[i].z - pts_b[i].z;
        if (sqrtf(dx * dx + dy * dy + dz * dz) > 0.01f) {
            differ = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(differ);
}

/* ---- Terminator lat/lon fields (1 test) ---- */

/* 28. Terminator points have valid lat/lon */
void test_terminator_latlon_valid(void)
{
    terminator_point_t pts[EARTH_TERMINATOR_SEGMENTS];
    earth_terminator(10.0, 3.0, pts);
    for (int i = 0; i < EARTH_TERMINATOR_SEGMENTS; i++) {
        TEST_ASSERT_TRUE(pts[i].lat_deg >= -90.0f && pts[i].lat_deg <= 90.0f);
        TEST_ASSERT_TRUE(pts[i].lon_deg >= -180.0f && pts[i].lon_deg <= 180.0f);
    }
}

/* ---- Coastline (3 tests) ---- */

/* 29. Coastline count > 0 */
void test_coastline_count_positive(void)
{
    int count = earth_coastline_count();
    TEST_ASSERT_TRUE(count > 0);
    TEST_ASSERT_TRUE(count <= EARTH_COASTLINE_POINT_COUNT);
}

/* 30. First coastline segment has valid coordinates */
void test_coastline_first_segment_valid(void)
{
    coastline_segment_t seg = earth_coastline_get(0);
    TEST_ASSERT_TRUE(seg.lat1 >= -90.0f && seg.lat1 <= 90.0f);
    TEST_ASSERT_TRUE(seg.lon1 >= -180.0f && seg.lon1 <= 180.0f);
    TEST_ASSERT_TRUE(seg.lat2 >= -90.0f && seg.lat2 <= 90.0f);
    TEST_ASSERT_TRUE(seg.lon2 >= -180.0f && seg.lon2 <= 180.0f);
}

/* 31. All coastline segments in range */
void test_coastline_all_in_range(void)
{
    int count = earth_coastline_count();
    for (int i = 0; i < count; i++) {
        coastline_segment_t seg = earth_coastline_get(i);
        TEST_ASSERT_TRUE(seg.lat1 >= -90.0f && seg.lat1 <= 90.0f);
        TEST_ASSERT_TRUE(seg.lon1 >= -180.0f && seg.lon1 <= 180.0f);
        TEST_ASSERT_TRUE(seg.lat2 >= -90.0f && seg.lat2 <= 90.0f);
        TEST_ASSERT_TRUE(seg.lon2 >= -180.0f && seg.lon2 <= 180.0f);
    }
}

/* ---- Is dayside (3 tests) ---- */

/* 32. Subsolar point is dayside */
void test_is_dayside_subsolar(void)
{
    /* Sun at (23.4, 45.0) — that point should be day */
    int result = earth_is_dayside(23.4, 45.0, 23.4, 45.0);
    TEST_ASSERT_EQUAL_INT(1, result);
}

/* 33. Antipodal point is night */
void test_is_dayside_antipodal(void)
{
    /* Sun at (23.4, 45.0) — antipodal is (-23.4, -135.0) */
    int result = earth_is_dayside(-23.4, -135.0, 23.4, 45.0);
    TEST_ASSERT_EQUAL_INT(0, result);
}

/* 34. Edge case: point 90 degrees away from subsolar point is night */
void test_is_dayside_opposite_hemisphere(void)
{
    /* Sun at (0, 0), point at (0, 180) is on the opposite side -> night */
    int result = earth_is_dayside(0.0, 180.0, 0.0, 0.0);
    TEST_ASSERT_EQUAL_INT(0, result);
}

/* ---- Globe generate: index validity (1 test) ---- */

/* 35. All indices are within vertex range */
void test_generate_indices_in_range(void)
{
    earth_vertex_t verts[EARTH_GLOBE_MAX_VERTICES];
    unsigned int indices[EARTH_GLOBE_MAX_INDICES];
    earth_mesh_info_t info = earth_globe_generate(8, 16, verts, indices);
    for (int i = 0; i < info.index_count; i++) {
        TEST_ASSERT_TRUE(indices[i] < (unsigned int)info.vertex_count);
    }
}

/* ---- Coastline out of bounds returns zeros (1 test) ---- */

/* 36. Out-of-bounds coastline returns zeros */
void test_coastline_out_of_bounds(void)
{
    coastline_segment_t seg = earth_coastline_get(-1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, seg.lat1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, seg.lon1);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, seg.lat2);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, seg.lon2);
}

/* ---- Terminator perpendicular to sun direction (1 test) ---- */

/* 37. All terminator points are perpendicular to sun vector (dot ~ 0) */
void test_terminator_perpendicular_to_sun(void)
{
    terminator_point_t pts[EARTH_TERMINATOR_SEGMENTS];
    earth_terminator(0.0, 0.0, pts);
    /* Sun at dec=0, ra=0h => subsolar at lat=0, lon=0 => sun_dir = (1,0,0) */
    float sx, sy, sz;
    earth_latlon_to_xyz(0.0, 0.0, &sx, &sy, &sz);
    for (int i = 0; i < EARTH_TERMINATOR_SEGMENTS; i++) {
        float dot = pts[i].x * sx + pts[i].y * sy + pts[i].z * sz;
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, dot);
    }
}

/* ---- Clamp lat/lon to max (1 test) ---- */

/* 38. Segments clamped to max */
void test_generate_clamp_max(void)
{
    earth_mesh_info_t info = earth_globe_generate(200, 300, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(EARTH_GLOBE_MAX_LAT, info.lat_segments);
    TEST_ASSERT_EQUAL_INT(EARTH_GLOBE_MAX_LON, info.lon_segments);
}

int main(void)
{
    UNITY_BEGIN();

    /* Vertex count helpers */
    RUN_TEST(test_vertex_count_min);
    RUN_TEST(test_vertex_count_typical);
    RUN_TEST(test_vertex_count_max);

    /* Index count helpers */
    RUN_TEST(test_index_count_min);
    RUN_TEST(test_index_count_typical);
    RUN_TEST(test_index_count_max);

    /* Globe generate: mesh info */
    RUN_TEST(test_generate_mesh_info);
    RUN_TEST(test_generate_mesh_info_4x8);

    /* Globe generate: NULL buffers */
    RUN_TEST(test_generate_null_vertices);
    RUN_TEST(test_generate_null_both);

    /* Globe generate: clamping */
    RUN_TEST(test_generate_clamp_lat_min);
    RUN_TEST(test_generate_clamp_lon_min);

    /* Globe generate: pole vertices */
    RUN_TEST(test_generate_north_pole);
    RUN_TEST(test_generate_south_pole);

    /* Globe generate: equator */
    RUN_TEST(test_generate_equator);

    /* Globe generate: normals */
    RUN_TEST(test_generate_normal_unit_pole);
    RUN_TEST(test_generate_normal_unit_equator);
    RUN_TEST(test_generate_normal_equals_position);

    /* Globe generate: UV */
    RUN_TEST(test_generate_uv_first);
    RUN_TEST(test_generate_uv_last);

    /* Lat/Lon to XYZ */
    RUN_TEST(test_latlon_to_xyz_equator_prime);
    RUN_TEST(test_latlon_to_xyz_north_pole);
    RUN_TEST(test_latlon_to_xyz_south_pole);
    RUN_TEST(test_latlon_to_xyz_90east);

    /* Terminator */
    RUN_TEST(test_terminator_count);
    RUN_TEST(test_terminator_on_unit_sphere);
    RUN_TEST(test_terminator_varies_with_dec);
    RUN_TEST(test_terminator_latlon_valid);
    RUN_TEST(test_terminator_perpendicular_to_sun);

    /* Coastline */
    RUN_TEST(test_coastline_count_positive);
    RUN_TEST(test_coastline_first_segment_valid);
    RUN_TEST(test_coastline_all_in_range);
    RUN_TEST(test_coastline_out_of_bounds);

    /* Is dayside */
    RUN_TEST(test_is_dayside_subsolar);
    RUN_TEST(test_is_dayside_antipodal);
    RUN_TEST(test_is_dayside_opposite_hemisphere);

    /* Index validity */
    RUN_TEST(test_generate_indices_in_range);

    /* Clamp max */
    RUN_TEST(test_generate_clamp_max);

    return UNITY_END();
}
