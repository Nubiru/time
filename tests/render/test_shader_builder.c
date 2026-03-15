/*
 * test_shader_builder.c — Tests for shader source string builder
 *
 * Tests: init, append, appendf, overflow detection, reset, append_parts,
 *        NULL safety, boundary conditions, GLSL-like content assembly.
 */

#include "../unity/unity.h"
#include "../../src/render/shader_builder.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ═══════════════════════════════════════════════════════════════════════
 * 1. Init
 * ═══════════════════════════════════════════════════════════════════════ */

void test_init_produces_empty_string(void)
{
    shader_src_t s;
    shader_src_init(&s);
    TEST_ASSERT_EQUAL_STRING("", shader_src_get(&s));
}

void test_init_len_is_zero(void)
{
    shader_src_t s;
    shader_src_init(&s);
    TEST_ASSERT_EQUAL_INT(0, shader_src_len(&s));
}

void test_init_is_valid(void)
{
    shader_src_t s;
    shader_src_init(&s);
    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 2. Single append
 * ═══════════════════════════════════════════════════════════════════════ */

void test_single_append(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append(&s, "hello");
    TEST_ASSERT_EQUAL_STRING("hello", shader_src_get(&s));
}

void test_single_append_len(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append(&s, "hello");
    TEST_ASSERT_EQUAL_INT(5, shader_src_len(&s));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 3. Multiple appends concatenate
 * ═══════════════════════════════════════════════════════════════════════ */

void test_multiple_appends_concatenate(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append(&s, "foo");
    shader_src_append(&s, "bar");
    shader_src_append(&s, "baz");
    TEST_ASSERT_EQUAL_STRING("foobarbaz", shader_src_get(&s));
}

void test_multiple_appends_len(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append(&s, "foo");
    shader_src_append(&s, "bar");
    TEST_ASSERT_EQUAL_INT(6, shader_src_len(&s));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 4. Null terminator preserved
 * ═══════════════════════════════════════════════════════════════════════ */

void test_append_preserves_null_terminator(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append(&s, "test");
    const char *result = shader_src_get(&s);
    TEST_ASSERT_EQUAL_INT('\0', result[4]);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 5. Overflow detection
 * ═══════════════════════════════════════════════════════════════════════ */

void test_overflow_detected(void)
{
    shader_src_t s;
    shader_src_init(&s);

    /* Fill with a string that exceeds capacity */
    char big[SHADER_SRC_CAPACITY + 100];
    memset(big, 'A', sizeof(big) - 1);
    big[sizeof(big) - 1] = '\0';

    shader_src_append(&s, big);
    TEST_ASSERT_EQUAL_INT(0, shader_src_valid(&s));
}

void test_overflow_flag_persists(void)
{
    shader_src_t s;
    shader_src_init(&s);

    char big[SHADER_SRC_CAPACITY + 10];
    memset(big, 'X', sizeof(big) - 1);
    big[sizeof(big) - 1] = '\0';

    shader_src_append(&s, big);
    TEST_ASSERT_EQUAL_INT(0, shader_src_valid(&s));

    /* Further appends should not clear overflow */
    shader_src_append(&s, "more");
    TEST_ASSERT_EQUAL_INT(0, shader_src_valid(&s));
}

void test_post_overflow_get_returns_valid_c_string(void)
{
    shader_src_t s;
    shader_src_init(&s);

    char big[SHADER_SRC_CAPACITY + 10];
    memset(big, 'B', sizeof(big) - 1);
    big[sizeof(big) - 1] = '\0';

    shader_src_append(&s, big);

    /* Must still be a valid C string (truncated) */
    const char *result = shader_src_get(&s);
    TEST_ASSERT_NOT_NULL(result);
    int len = (int)strlen(result);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len < SHADER_SRC_CAPACITY);
}

void test_post_overflow_valid_returns_zero(void)
{
    shader_src_t s;
    shader_src_init(&s);

    char big[SHADER_SRC_CAPACITY + 10];
    memset(big, 'C', sizeof(big) - 1);
    big[sizeof(big) - 1] = '\0';

    shader_src_append(&s, big);
    TEST_ASSERT_EQUAL_INT(0, shader_src_valid(&s));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 6. Reset
 * ═══════════════════════════════════════════════════════════════════════ */

void test_reset_clears_content(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append(&s, "some content");
    shader_src_reset(&s);
    TEST_ASSERT_EQUAL_STRING("", shader_src_get(&s));
    TEST_ASSERT_EQUAL_INT(0, shader_src_len(&s));
}

void test_reset_clears_overflow(void)
{
    shader_src_t s;
    shader_src_init(&s);

    char big[SHADER_SRC_CAPACITY + 10];
    memset(big, 'D', sizeof(big) - 1);
    big[sizeof(big) - 1] = '\0';

    shader_src_append(&s, big);
    TEST_ASSERT_EQUAL_INT(0, shader_src_valid(&s));

    shader_src_reset(&s);
    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));
    TEST_ASSERT_EQUAL_INT(0, shader_src_len(&s));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 7. appendf
 * ═══════════════════════════════════════════════════════════════════════ */

void test_appendf_integer(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_appendf(&s, "#define X %d\n", 42);
    TEST_ASSERT_EQUAL_STRING("#define X 42\n", shader_src_get(&s));
}

void test_appendf_float(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_appendf(&s, "#define PI %.5f\n", 3.14159);

    const char *result = shader_src_get(&s);
    TEST_ASSERT_NOT_NULL(strstr(result, "3.14159"));
}

void test_appendf_string(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_appendf(&s, "uniform %s u_%s;\n", "float", "time");
    TEST_ASSERT_EQUAL_STRING("uniform float u_time;\n", shader_src_get(&s));
}

void test_appendf_overflow_detection(void)
{
    shader_src_t s;
    shader_src_init(&s);

    /* Fill nearly to capacity */
    char fill[SHADER_SRC_CAPACITY - 5];
    memset(fill, 'E', sizeof(fill) - 1);
    fill[sizeof(fill) - 1] = '\0';
    shader_src_append(&s, fill);
    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));

    /* This should overflow */
    shader_src_appendf(&s, "this is way too long for remaining space %d", 12345);
    TEST_ASSERT_EQUAL_INT(0, shader_src_valid(&s));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 8. append_parts
 * ═══════════════════════════════════════════════════════════════════════ */

void test_append_parts_multiple(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append_parts(&s, "alpha", " ", "beta", " ", "gamma", NULL);
    TEST_ASSERT_EQUAL_STRING("alpha beta gamma", shader_src_get(&s));
}

void test_append_parts_null_terminates_list(void)
{
    shader_src_t s;
    shader_src_init(&s);
    /* Only "first" should be appended; NULL stops the list */
    shader_src_append_parts(&s, "first", NULL);
    TEST_ASSERT_EQUAL_STRING("first", shader_src_get(&s));
}

void test_append_parts_empty_list(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append_parts(&s, NULL);
    TEST_ASSERT_EQUAL_STRING("", shader_src_get(&s));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 9. Empty string append
 * ═══════════════════════════════════════════════════════════════════════ */

void test_empty_string_append(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append(&s, "");
    TEST_ASSERT_EQUAL_STRING("", shader_src_get(&s));
    TEST_ASSERT_EQUAL_INT(0, shader_src_len(&s));
    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 10. NULL pointer safety
 * ═══════════════════════════════════════════════════════════════════════ */

void test_null_append_is_noop(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append(&s, "before");
    shader_src_append(&s, NULL);
    TEST_ASSERT_EQUAL_STRING("before", shader_src_get(&s));
}

void test_null_src_get_returns_empty(void)
{
    const char *result = shader_src_get(NULL);
    TEST_ASSERT_EQUAL_STRING("", result);
}

void test_null_src_len_returns_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, shader_src_len(NULL));
}

void test_null_src_valid_returns_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, shader_src_valid(NULL));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 11. Boundary conditions
 * ═══════════════════════════════════════════════════════════════════════ */

void test_exact_capacity_fill(void)
{
    shader_src_t s;
    shader_src_init(&s);

    /* Fill exactly SHADER_SRC_CAPACITY - 1 chars (room for null terminator) */
    char exact[SHADER_SRC_CAPACITY];
    memset(exact, 'F', SHADER_SRC_CAPACITY - 1);
    exact[SHADER_SRC_CAPACITY - 1] = '\0';

    shader_src_append(&s, exact);
    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));
    TEST_ASSERT_EQUAL_INT(SHADER_SRC_CAPACITY - 1, shader_src_len(&s));
}

void test_one_byte_over_overflow(void)
{
    shader_src_t s;
    shader_src_init(&s);

    /* Fill exactly to capacity */
    char exact[SHADER_SRC_CAPACITY];
    memset(exact, 'G', SHADER_SRC_CAPACITY - 1);
    exact[SHADER_SRC_CAPACITY - 1] = '\0';
    shader_src_append(&s, exact);
    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));

    /* One more byte should overflow */
    shader_src_append(&s, "X");
    TEST_ASSERT_EQUAL_INT(0, shader_src_valid(&s));
}

void test_large_single_string_near_boundary(void)
{
    shader_src_t s;
    shader_src_init(&s);

    /* Fill to within 10 bytes of capacity */
    char near[SHADER_SRC_CAPACITY - 10];
    memset(near, 'H', sizeof(near) - 1);
    near[sizeof(near) - 1] = '\0';

    shader_src_append(&s, near);
    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));

    /* 10 more chars should fit */
    shader_src_append(&s, "1234567890");
    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));
    TEST_ASSERT_EQUAL_INT(SHADER_SRC_CAPACITY - 1, shader_src_len(&s));
}

void test_multiple_small_appends_near_boundary(void)
{
    shader_src_t s;
    shader_src_init(&s);

    /* Fill most of the buffer with small appends */
    int count = 0;
    while (shader_src_len(&s) < SHADER_SRC_CAPACITY - 20) {
        shader_src_append(&s, "ABCD");
        count++;
    }
    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));
    TEST_ASSERT_TRUE(count > 10);
}

/* ═══════════════════════════════════════════════════════════════════════
 * 12. Real GLSL-like content
 * ═══════════════════════════════════════════════════════════════════════ */

void test_glsl_like_content(void)
{
    shader_src_t s;
    shader_src_init(&s);

    shader_src_append(&s, "#version 300 es\n");
    shader_src_append(&s, "precision highp float;\n");
    shader_src_append(&s, "in vec2 v_uv;\n");
    shader_src_append(&s, "uniform float u_time;\n");
    shader_src_append(&s, "out vec4 frag_color;\n");
    shader_src_append(&s, "\n");
    shader_src_append(&s, "void main() {\n");
    shader_src_append(&s, "    frag_color = vec4(v_uv, 0.0, 1.0);\n");
    shader_src_append(&s, "}\n");

    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));

    const char *result = shader_src_get(&s);
    TEST_ASSERT_NOT_NULL(strstr(result, "#version 300 es"));
    TEST_ASSERT_NOT_NULL(strstr(result, "precision highp float"));
    TEST_ASSERT_NOT_NULL(strstr(result, "void main()"));
    TEST_ASSERT_NOT_NULL(strstr(result, "frag_color"));
}

void test_glsl_version_precision_body_assembly(void)
{
    shader_src_t s;
    shader_src_init(&s);

    const char *version = "#version 300 es\n";
    const char *precision = "precision highp float;\n";
    const char *body = "void main() { gl_FragColor = vec4(1.0); }\n";

    shader_src_append_parts(&s, version, precision, body, NULL);

    TEST_ASSERT_EQUAL_INT(1, shader_src_valid(&s));
    int expected_len = (int)(strlen(version) + strlen(precision) + strlen(body));
    TEST_ASSERT_EQUAL_INT(expected_len, shader_src_len(&s));
}

/* ═══════════════════════════════════════════════════════════════════════
 * 13. appendf combined with append
 * ═══════════════════════════════════════════════════════════════════════ */

void test_appendf_mixed_with_append(void)
{
    shader_src_t s;
    shader_src_init(&s);
    shader_src_append(&s, "#version 300 es\n");
    shader_src_appendf(&s, "#define COUNT %d\n", 16);
    shader_src_append(&s, "void main() {}\n");

    const char *result = shader_src_get(&s);
    TEST_ASSERT_NOT_NULL(strstr(result, "#define COUNT 16"));
}

/* ═══════════════════════════════════════════════════════════════════════
 * Runner
 * ═══════════════════════════════════════════════════════════════════════ */

int main(void)
{
    UNITY_BEGIN();

    /* Init */
    RUN_TEST(test_init_produces_empty_string);
    RUN_TEST(test_init_len_is_zero);
    RUN_TEST(test_init_is_valid);

    /* Single append */
    RUN_TEST(test_single_append);
    RUN_TEST(test_single_append_len);

    /* Multiple appends */
    RUN_TEST(test_multiple_appends_concatenate);
    RUN_TEST(test_multiple_appends_len);

    /* Null terminator */
    RUN_TEST(test_append_preserves_null_terminator);

    /* Overflow */
    RUN_TEST(test_overflow_detected);
    RUN_TEST(test_overflow_flag_persists);
    RUN_TEST(test_post_overflow_get_returns_valid_c_string);
    RUN_TEST(test_post_overflow_valid_returns_zero);

    /* Reset */
    RUN_TEST(test_reset_clears_content);
    RUN_TEST(test_reset_clears_overflow);

    /* appendf */
    RUN_TEST(test_appendf_integer);
    RUN_TEST(test_appendf_float);
    RUN_TEST(test_appendf_string);
    RUN_TEST(test_appendf_overflow_detection);

    /* append_parts */
    RUN_TEST(test_append_parts_multiple);
    RUN_TEST(test_append_parts_null_terminates_list);
    RUN_TEST(test_append_parts_empty_list);

    /* Empty string */
    RUN_TEST(test_empty_string_append);

    /* NULL safety */
    RUN_TEST(test_null_append_is_noop);
    RUN_TEST(test_null_src_get_returns_empty);
    RUN_TEST(test_null_src_len_returns_zero);
    RUN_TEST(test_null_src_valid_returns_zero);

    /* Boundary conditions */
    RUN_TEST(test_exact_capacity_fill);
    RUN_TEST(test_one_byte_over_overflow);
    RUN_TEST(test_large_single_string_near_boundary);
    RUN_TEST(test_multiple_small_appends_near_boundary);

    /* GLSL-like content */
    RUN_TEST(test_glsl_like_content);
    RUN_TEST(test_glsl_version_precision_body_assembly);

    /* Mixed append + appendf */
    RUN_TEST(test_appendf_mixed_with_append);

    return UNITY_END();
}
