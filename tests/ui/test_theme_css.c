#include "../../tests/unity/unity.h"
#include "../../src/ui/theme_css.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * BASIC OUTPUT
 * ================================================================ */

void test_cosmos_produces_nonempty_output(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    int n = theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_GREATER_THAN(0, (int)strlen(buf));
}

/* ================================================================
 * INDIVIDUAL CSS VARIABLE PRESENCE
 * ================================================================ */

void test_output_contains_bg_space(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    /* bg_space = (0.024, 0.027, 0.036) -> (6, 7, 9) */
    TEST_ASSERT_NOT_NULL(strstr(buf, "--bg-space: 6, 7, 9;"));
}

void test_output_contains_bg_surface(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--bg-surface:"));
}

void test_output_contains_bg_elevated(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--bg-elevated:"));
}

void test_output_contains_text_primary(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--text-primary:"));
}

void test_output_contains_text_secondary(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--text-secondary:"));
}

void test_output_contains_text_muted(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--text-muted:"));
}

void test_output_contains_brand_primary(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    /* brand_primary = (1.0, 0.85, 0.55) -> (255, 217, 140) */
    TEST_ASSERT_NOT_NULL(strstr(buf, "--brand-primary: 255, 217, 140;"));
}

void test_output_contains_brand_secondary(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    /* brand_secondary = (0.2, 0.75, 0.8) -> (51, 191, 204) */
    TEST_ASSERT_NOT_NULL(strstr(buf, "--brand-secondary: 51, 191, 204;"));
}

void test_output_contains_accent(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--accent:"));
}

void test_output_contains_danger(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--danger:"));
}

void test_output_contains_success(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--success:"));
}

void test_output_contains_warning(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--warning:"));
}

void test_output_contains_border(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--border:"));
}

void test_output_contains_star_glow(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--star-glow:"));
}

/* ================================================================
 * DAWN vs COSMOS
 * ================================================================ */

void test_dawn_differs_from_cosmos(void) {
    theme_t cosmos = theme_get(THEME_COSMOS);
    theme_t dawn = theme_get(THEME_DAWN);
    char buf_c[2048], buf_d[2048];
    theme_css_vars(&cosmos, buf_c, (int)sizeof(buf_c));
    theme_css_vars(&dawn, buf_d, (int)sizeof(buf_d));
    /* Dawn and Cosmos must produce different output */
    TEST_ASSERT_TRUE(strcmp(buf_c, buf_d) != 0);
}

/* ================================================================
 * EDGE CASES
 * ================================================================ */

void test_buffer_too_small_truncates(void) {
    theme_t t = theme_get(THEME_COSMOS);
    /* Give very small buffer — should truncate but not crash */
    char buf[32];
    int n = theme_css_vars(&t, buf, (int)sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, n);
    TEST_ASSERT_TRUE(n < (int)sizeof(buf));
    /* Must be null-terminated */
    TEST_ASSERT_EQUAL_CHAR('\0', buf[n]);
}

void test_null_buffer_returns_zero(void) {
    theme_t t = theme_get(THEME_COSMOS);
    int n = theme_css_vars(&t, NULL, 1024);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_zero_bufsize_returns_zero(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[64];
    int n = theme_css_vars(&t, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ================================================================
 * INTEGER FORMAT VERIFICATION
 * ================================================================ */

void test_values_are_integers_no_decimals(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));
    /* No decimal points should appear in the output.
     * The old theme_to_css used %.3f floats; this module uses %d integers. */
    TEST_ASSERT_NULL(strchr(buf, '.'));
}

/* ================================================================
 * ROUND-TRIP: COSMOS CSS MATCHES style.css VALUES
 * ================================================================ */

void test_cosmos_css_roundtrip_values(void) {
    theme_t t = theme_get(THEME_COSMOS);
    char buf[2048];
    theme_css_vars(&t, buf, (int)sizeof(buf));

    /* Verify integer conversion of known Cosmos theme_t values:
     * bg_space    = (0.024, 0.027, 0.036) -> round(x*255) = (6, 7, 9)
     * brand_primary = (1.0, 0.85, 0.55)   -> (255, 217, 140)
     * brand_secondary = (0.2, 0.75, 0.8)  -> (51, 191, 204)
     * danger      = (0.85, 0.25, 0.25)    -> (217, 64, 64)
     * star_glow   = (1.0, 0.95, 0.85)     -> (255, 242, 217) */
    TEST_ASSERT_NOT_NULL(strstr(buf, "--bg-space: 6, 7, 9;"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--brand-primary: 255, 217, 140;"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--brand-secondary: 51, 191, 204;"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--danger: 217, 64, 64;"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "--star-glow: 255, 242, 217;"));
}

/* ================================================================
 * RUNNER
 * ================================================================ */

int main(void) {
    UNITY_BEGIN();

    /* Basic output */
    RUN_TEST(test_cosmos_produces_nonempty_output);

    /* Individual CSS variables */
    RUN_TEST(test_output_contains_bg_space);
    RUN_TEST(test_output_contains_bg_surface);
    RUN_TEST(test_output_contains_bg_elevated);
    RUN_TEST(test_output_contains_text_primary);
    RUN_TEST(test_output_contains_text_secondary);
    RUN_TEST(test_output_contains_text_muted);
    RUN_TEST(test_output_contains_brand_primary);
    RUN_TEST(test_output_contains_brand_secondary);
    RUN_TEST(test_output_contains_accent);
    RUN_TEST(test_output_contains_danger);
    RUN_TEST(test_output_contains_success);
    RUN_TEST(test_output_contains_warning);
    RUN_TEST(test_output_contains_border);
    RUN_TEST(test_output_contains_star_glow);

    /* Dawn vs Cosmos */
    RUN_TEST(test_dawn_differs_from_cosmos);

    /* Edge cases */
    RUN_TEST(test_buffer_too_small_truncates);
    RUN_TEST(test_null_buffer_returns_zero);
    RUN_TEST(test_zero_bufsize_returns_zero);

    /* Integer format */
    RUN_TEST(test_values_are_integers_no_decimals);

    /* Round-trip */
    RUN_TEST(test_cosmos_css_roundtrip_values);

    return UNITY_END();
}
