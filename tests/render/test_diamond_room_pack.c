/*
 * test_diamond_room_pack.c — Tests for Diamond Room wireframe geometry
 */

#include "../unity/unity.h"
#include "../../src/render/diamond_room_pack.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* 1. Pack with default room produces correct counts */
void test_pack_default_counts(void)
{
    dr_input_t input = dr_default_input();
    dr_room_t room = dr_compute(&input);
    drp_data_t data = drp_pack(&room, 1.0f);
    TEST_ASSERT_EQUAL_INT(DRP_EDGE_COUNT, data.edge_count);
    TEST_ASSERT_EQUAL_INT(DRP_LINE_VERTS, data.vertex_count);
}

/* 2. All vertex positions are finite */
void test_vertices_finite(void)
{
    dr_input_t input = dr_default_input();
    dr_room_t room = dr_compute(&input);
    drp_data_t data = drp_pack(&room, 5.0f);
    for (int i = 0; i < data.vertex_count; i++) {
        int base = i * DRP_VERTEX_FLOATS;
        for (int j = 0; j < 3; j++) {
            TEST_ASSERT_TRUE(isfinite(data.vertices[base + j]));
        }
    }
}

/* 3. Color values in [0,1] range */
void test_colors_valid(void)
{
    dr_input_t input = dr_default_input();
    dr_room_t room = dr_compute(&input);
    drp_data_t data = drp_pack(&room, 1.0f);
    for (int i = 0; i < data.vertex_count; i++) {
        int base = i * DRP_VERTEX_FLOATS;
        for (int j = 3; j < 7; j++) {
            TEST_ASSERT_TRUE(data.vertices[base + j] >= 0.0f);
            TEST_ASSERT_TRUE(data.vertices[base + j] <= 1.01f);
        }
    }
}

/* 4. Vertex positions on sphere at given scale */
void test_vertex_pos_on_sphere(void)
{
    float x, y, z;
    for (int i = 0; i < DRP_VERTEX_COUNT; i++) {
        drp_vertex_pos(i, 10.0f, &x, &y, &z);
        float mag = sqrtf(x*x + y*y + z*z);
        TEST_ASSERT_FLOAT_WITHIN(0.1f, 10.0f, mag);
    }
}

/* 5. Invalid vertex index returns zeros */
void test_vertex_pos_invalid(void)
{
    float x = 99.0f, y = 99.0f, z = 99.0f;
    drp_vertex_pos(-1, 10.0f, &x, &y, &z);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, z);
}

/* 6. Face system mapping wraps correctly */
void test_face_system_mapping(void)
{
    for (int f = 0; f < DRP_FACE_COUNT; f++) {
        int sys = drp_face_system(f);
        TEST_ASSERT_TRUE(sys >= 0);
        TEST_ASSERT_TRUE(sys < DR_MAX_FACETS);
    }
}

/* 7. Face system wraps at 16 */
void test_face_system_wraps(void)
{
    TEST_ASSERT_EQUAL_INT(0, drp_face_system(16));
    TEST_ASSERT_EQUAL_INT(1, drp_face_system(17));
}

/* 8. Scale affects vertex magnitude */
void test_scale_affects_position(void)
{
    float x1, y1, z1, x2, y2, z2;
    drp_vertex_pos(0, 1.0f, &x1, &y1, &z1);
    drp_vertex_pos(0, 5.0f, &x2, &y2, &z2);
    float m1 = sqrtf(x1*x1 + y1*y1 + z1*z1);
    float m2 = sqrtf(x2*x2 + y2*y2 + z2*z2);
    TEST_ASSERT_TRUE(m2 > m1 * 4.0f);
}

/* 9. Vertex shader source is valid GLSL ES 3.00 */
void test_vert_source(void)
{
    const char *src = drp_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

/* 10. Fragment shader source is valid */
void test_frag_source(void)
{
    const char *src = drp_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "frag_color"));
}

/* 11. Rotation speed is positive */
void test_rotation_speed(void)
{
    dr_input_t input = dr_default_input();
    dr_room_t room = dr_compute(&input);
    drp_data_t data = drp_pack(&room, 1.0f);
    TEST_ASSERT_TRUE(data.rotation_speed >= 0.0f);
}

/* 12. DRP constants are correct */
void test_constants(void)
{
    TEST_ASSERT_EQUAL_INT(12, DRP_VERTEX_COUNT);
    TEST_ASSERT_EQUAL_INT(30, DRP_EDGE_COUNT);
    TEST_ASSERT_EQUAL_INT(20, DRP_FACE_COUNT);
    TEST_ASSERT_EQUAL_INT(7, DRP_VERTEX_FLOATS);
    TEST_ASSERT_EQUAL_INT(60, DRP_LINE_VERTS);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_pack_default_counts);
    RUN_TEST(test_vertices_finite);
    RUN_TEST(test_colors_valid);
    RUN_TEST(test_vertex_pos_on_sphere);
    RUN_TEST(test_vertex_pos_invalid);
    RUN_TEST(test_face_system_mapping);
    RUN_TEST(test_face_system_wraps);
    RUN_TEST(test_scale_affects_position);
    RUN_TEST(test_vert_source);
    RUN_TEST(test_frag_source);
    RUN_TEST(test_rotation_speed);
    RUN_TEST(test_constants);
    return UNITY_END();
}
