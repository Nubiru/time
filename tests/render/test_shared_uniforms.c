/*
 * test_shared_uniforms.c — Tests for shared UBO data module
 *
 * Tests: block declaration, su_pack, su_read_mat4, su_read_time,
 * layout constants, round-trip integrity, NULL safety.
 *
 * 26 tests covering all public API functions.
 */

#include "../unity/unity.h"
#include "../../src/render/shared_uniforms.h"

#include <math.h>
#include <string.h>

#define FTOL 0.0001f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* Helper: build an identity matrix (16 floats, column-major) */
static void identity_mat4(float m[16])
{
    memset(m, 0, 16 * sizeof(float));
    m[0] = 1.0f; m[5] = 1.0f; m[10] = 1.0f; m[15] = 1.0f;
}

/* Helper: build a test matrix with distinct values per element */
static void test_mat4(float m[16], float base)
{
    for (int i = 0; i < 16; i++) {
        m[i] = base + (float)i * 0.1f;
    }
}

/* ======================================================================
 * 1. su_block_declaration — GLSL uniform block string
 * ====================================================================== */

/* 1. Block declaration is non-NULL */
void test_block_decl_non_null(void)
{
    const char *s = su_block_declaration();
    TEST_ASSERT_NOT_NULL(s);
}

/* 2. Block declaration is non-empty */
void test_block_decl_non_empty(void)
{
    const char *s = su_block_declaration();
    TEST_ASSERT_TRUE(strlen(s) > 0);
}

/* 3. Contains "SharedData" */
void test_block_decl_contains_shared_data(void)
{
    TEST_ASSERT_NOT_NULL(strstr(su_block_declaration(), "SharedData"));
}

/* 4. Contains "u_view" */
void test_block_decl_contains_u_view(void)
{
    TEST_ASSERT_NOT_NULL(strstr(su_block_declaration(), "u_view"));
}

/* 5. Contains "u_proj" */
void test_block_decl_contains_u_proj(void)
{
    TEST_ASSERT_NOT_NULL(strstr(su_block_declaration(), "u_proj"));
}

/* 6. Contains "u_view_proj" */
void test_block_decl_contains_u_view_proj(void)
{
    TEST_ASSERT_NOT_NULL(strstr(su_block_declaration(), "u_view_proj"));
}

/* 7. Contains "u_time" */
void test_block_decl_contains_u_time(void)
{
    TEST_ASSERT_NOT_NULL(strstr(su_block_declaration(), "u_time"));
}

/* 8. Contains "std140" */
void test_block_decl_contains_std140(void)
{
    TEST_ASSERT_NOT_NULL(strstr(su_block_declaration(), "std140"));
}

/* ======================================================================
 * 2. su_pack — pack data into std140 buffer
 * ====================================================================== */

/* 9. Pack with identity matrices returns SU_UBO_SIZE */
void test_pack_returns_ubo_size(void)
{
    float view[16], proj[16], vp[16];
    identity_mat4(view);
    identity_mat4(proj);
    identity_mat4(vp);
    unsigned char buf[SU_UBO_SIZE];
    int result = su_pack(view, proj, vp, 1.0f, buf);
    TEST_ASSERT_EQUAL_INT(SU_UBO_SIZE, result);
}

/* 10. Pack identity view: element [0] = 1.0 at offset 0 */
void test_pack_identity_view_offset(void)
{
    float view[16], proj[16], vp[16];
    identity_mat4(view);
    identity_mat4(proj);
    identity_mat4(vp);
    unsigned char buf[SU_UBO_SIZE];
    su_pack(view, proj, vp, 0.0f, buf);

    float val;
    memcpy(&val, buf + SU_VIEW_OFFSET, sizeof(float));
    FCLOSE(1.0f, val);
}

/* 11. Pack identity proj: element [0] = 1.0 at offset 64 */
void test_pack_identity_proj_offset(void)
{
    float view[16], proj[16], vp[16];
    identity_mat4(view);
    identity_mat4(proj);
    identity_mat4(vp);
    unsigned char buf[SU_UBO_SIZE];
    su_pack(view, proj, vp, 0.0f, buf);

    float val;
    memcpy(&val, buf + SU_PROJ_OFFSET, sizeof(float));
    FCLOSE(1.0f, val);
}

/* 12. Pack identity view_proj: element [0] = 1.0 at offset 128 */
void test_pack_identity_vp_offset(void)
{
    float view[16], proj[16], vp[16];
    identity_mat4(view);
    identity_mat4(proj);
    identity_mat4(vp);
    unsigned char buf[SU_UBO_SIZE];
    su_pack(view, proj, vp, 0.0f, buf);

    float val;
    memcpy(&val, buf + SU_VIEW_PROJ_OFFSET, sizeof(float));
    FCLOSE(1.0f, val);
}

/* 13. Time value packed at offset 192 */
void test_pack_time_at_offset(void)
{
    float view[16], proj[16], vp[16];
    identity_mat4(view);
    identity_mat4(proj);
    identity_mat4(vp);
    unsigned char buf[SU_UBO_SIZE];
    su_pack(view, proj, vp, 42.5f, buf);

    float val;
    memcpy(&val, buf + SU_TIME_OFFSET, sizeof(float));
    FCLOSE(42.5f, val);
}

/* 14. NULL out returns 0, no crash */
void test_pack_null_out_returns_zero(void)
{
    float view[16], proj[16], vp[16];
    identity_mat4(view);
    identity_mat4(proj);
    identity_mat4(vp);
    int result = su_pack(view, proj, vp, 1.0f, NULL);
    TEST_ASSERT_EQUAL_INT(0, result);
}

/* 15. NULL matrices are safe (zero-filled) */
void test_pack_null_matrices_safe(void)
{
    unsigned char buf[SU_UBO_SIZE];
    int result = su_pack(NULL, NULL, NULL, 5.0f, buf);
    TEST_ASSERT_EQUAL_INT(SU_UBO_SIZE, result);

    /* View matrix should be zero */
    float val;
    memcpy(&val, buf + SU_VIEW_OFFSET, sizeof(float));
    FCLOSE(0.0f, val);

    /* Time should still be set */
    memcpy(&val, buf + SU_TIME_OFFSET, sizeof(float));
    FCLOSE(5.0f, val);
}

/* ======================================================================
 * 3. su_read_mat4 — read matrices back from packed buffer
 * ====================================================================== */

/* 16. Read back view correctly after pack */
void test_read_mat4_view(void)
{
    float view[16], proj[16], vp[16];
    test_mat4(view, 1.0f);
    identity_mat4(proj);
    identity_mat4(vp);
    unsigned char buf[SU_UBO_SIZE];
    su_pack(view, proj, vp, 0.0f, buf);

    float out[16];
    int ok = su_read_mat4(buf, SU_VIEW_OFFSET, out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    for (int i = 0; i < 16; i++) {
        FCLOSE(view[i], out[i]);
    }
}

/* 17. Read back proj correctly */
void test_read_mat4_proj(void)
{
    float view[16], proj[16], vp[16];
    identity_mat4(view);
    test_mat4(proj, 2.0f);
    identity_mat4(vp);
    unsigned char buf[SU_UBO_SIZE];
    su_pack(view, proj, vp, 0.0f, buf);

    float out[16];
    int ok = su_read_mat4(buf, SU_PROJ_OFFSET, out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    for (int i = 0; i < 16; i++) {
        FCLOSE(proj[i], out[i]);
    }
}

/* 18. Read back view_proj correctly */
void test_read_mat4_view_proj(void)
{
    float view[16], proj[16], vp[16];
    identity_mat4(view);
    identity_mat4(proj);
    test_mat4(vp, 3.0f);
    unsigned char buf[SU_UBO_SIZE];
    su_pack(view, proj, vp, 0.0f, buf);

    float out[16];
    int ok = su_read_mat4(buf, SU_VIEW_PROJ_OFFSET, out);
    TEST_ASSERT_EQUAL_INT(1, ok);
    for (int i = 0; i < 16; i++) {
        FCLOSE(vp[i], out[i]);
    }
}

/* 19. Invalid offset returns 0 */
void test_read_mat4_invalid_offset(void)
{
    unsigned char buf[SU_UBO_SIZE];
    memset(buf, 0, SU_UBO_SIZE);
    float out[16];
    int ok = su_read_mat4(buf, 32, out);  /* 32 is not a valid mat4 offset */
    TEST_ASSERT_EQUAL_INT(0, ok);
}

/* 20. NULL buf returns 0 */
void test_read_mat4_null_buf(void)
{
    float out[16];
    int ok = su_read_mat4(NULL, SU_VIEW_OFFSET, out);
    TEST_ASSERT_EQUAL_INT(0, ok);
}

/* 21. NULL out returns 0 */
void test_read_mat4_null_out(void)
{
    unsigned char buf[SU_UBO_SIZE];
    memset(buf, 0, SU_UBO_SIZE);
    int ok = su_read_mat4(buf, SU_VIEW_OFFSET, NULL);
    TEST_ASSERT_EQUAL_INT(0, ok);
}

/* ======================================================================
 * 4. su_read_time — read time back from packed buffer
 * ====================================================================== */

/* 22. Read back time correctly */
void test_read_time_correct(void)
{
    float view[16], proj[16], vp[16];
    identity_mat4(view);
    identity_mat4(proj);
    identity_mat4(vp);
    unsigned char buf[SU_UBO_SIZE];
    su_pack(view, proj, vp, 99.75f, buf);

    float t = su_read_time(buf);
    FCLOSE(99.75f, t);
}

/* 23. NULL buf returns 0.0f */
void test_read_time_null_buf(void)
{
    float t = su_read_time(NULL);
    FCLOSE(0.0f, t);
}

/* ======================================================================
 * 5. Layout constants
 * ====================================================================== */

/* 24. SU_VIEW_OFFSET == 0 */
void test_layout_view_offset(void)
{
    TEST_ASSERT_EQUAL_INT(0, SU_VIEW_OFFSET);
}

/* 25. SU_PROJ_OFFSET == 64 */
void test_layout_proj_offset(void)
{
    TEST_ASSERT_EQUAL_INT(64, SU_PROJ_OFFSET);
}

/* 26. SU_TIME_OFFSET == 192 */
void test_layout_time_offset(void)
{
    TEST_ASSERT_EQUAL_INT(192, SU_TIME_OFFSET);
}

/* 27. SU_UBO_SIZE == 208 (std140 padded) */
void test_layout_ubo_size(void)
{
    TEST_ASSERT_EQUAL_INT(208, SU_UBO_SIZE);
}

/* 28. SU_VIEW_PROJ_OFFSET == 128 */
void test_layout_view_proj_offset(void)
{
    TEST_ASSERT_EQUAL_INT(128, SU_VIEW_PROJ_OFFSET);
}

/* 29. SU_BINDING_POINT == 0 */
void test_layout_binding_point(void)
{
    TEST_ASSERT_EQUAL_INT(0, SU_BINDING_POINT);
}

/* ======================================================================
 * 6. Round-trip — pack then read all values back
 * ====================================================================== */

/* 30. Full round-trip: pack 3 distinct matrices + time, read all back */
void test_round_trip_all(void)
{
    float view[16], proj[16], vp[16];
    test_mat4(view, 10.0f);
    test_mat4(proj, 20.0f);
    test_mat4(vp, 30.0f);
    float time_val = 123.456f;

    unsigned char buf[SU_UBO_SIZE];
    int wrote = su_pack(view, proj, vp, time_val, buf);
    TEST_ASSERT_EQUAL_INT(SU_UBO_SIZE, wrote);

    /* Read view */
    float rv[16];
    TEST_ASSERT_EQUAL_INT(1, su_read_mat4(buf, SU_VIEW_OFFSET, rv));
    for (int i = 0; i < 16; i++) {
        FCLOSE(view[i], rv[i]);
    }

    /* Read proj */
    float rp[16];
    TEST_ASSERT_EQUAL_INT(1, su_read_mat4(buf, SU_PROJ_OFFSET, rp));
    for (int i = 0; i < 16; i++) {
        FCLOSE(proj[i], rp[i]);
    }

    /* Read view_proj */
    float rvp[16];
    TEST_ASSERT_EQUAL_INT(1, su_read_mat4(buf, SU_VIEW_PROJ_OFFSET, rvp));
    for (int i = 0; i < 16; i++) {
        FCLOSE(vp[i], rvp[i]);
    }

    /* Read time */
    FCLOSE(time_val, su_read_time(buf));
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Block declaration (8 tests) */
    RUN_TEST(test_block_decl_non_null);
    RUN_TEST(test_block_decl_non_empty);
    RUN_TEST(test_block_decl_contains_shared_data);
    RUN_TEST(test_block_decl_contains_u_view);
    RUN_TEST(test_block_decl_contains_u_proj);
    RUN_TEST(test_block_decl_contains_u_view_proj);
    RUN_TEST(test_block_decl_contains_u_time);
    RUN_TEST(test_block_decl_contains_std140);

    /* su_pack (7 tests) */
    RUN_TEST(test_pack_returns_ubo_size);
    RUN_TEST(test_pack_identity_view_offset);
    RUN_TEST(test_pack_identity_proj_offset);
    RUN_TEST(test_pack_identity_vp_offset);
    RUN_TEST(test_pack_time_at_offset);
    RUN_TEST(test_pack_null_out_returns_zero);
    RUN_TEST(test_pack_null_matrices_safe);

    /* su_read_mat4 (6 tests) */
    RUN_TEST(test_read_mat4_view);
    RUN_TEST(test_read_mat4_proj);
    RUN_TEST(test_read_mat4_view_proj);
    RUN_TEST(test_read_mat4_invalid_offset);
    RUN_TEST(test_read_mat4_null_buf);
    RUN_TEST(test_read_mat4_null_out);

    /* su_read_time (2 tests) */
    RUN_TEST(test_read_time_correct);
    RUN_TEST(test_read_time_null_buf);

    /* Layout constants (6 tests) */
    RUN_TEST(test_layout_view_offset);
    RUN_TEST(test_layout_proj_offset);
    RUN_TEST(test_layout_time_offset);
    RUN_TEST(test_layout_ubo_size);
    RUN_TEST(test_layout_view_proj_offset);
    RUN_TEST(test_layout_binding_point);

    /* Round-trip (1 test) */
    RUN_TEST(test_round_trip_all);

    return UNITY_END();
}
