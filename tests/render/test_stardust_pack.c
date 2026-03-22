/*
 * test_stardust_pack.c — Tests for procedural star field shader source
 */

#include "../unity/unity.h"
#include "../../src/render/stardust_pack.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* 1. Default config has positive density */
void test_default_config_density(void)
{
    sd_config_t cfg = sd_default_config();
    TEST_ASSERT_TRUE(cfg.density > 0.0f);
    TEST_ASSERT_TRUE(cfg.density <= 1.0f);
}

/* 2. Default config has positive brightness */
void test_default_config_brightness(void)
{
    sd_config_t cfg = sd_default_config();
    TEST_ASSERT_TRUE(cfg.brightness > 0.0f);
}

/* 3. Default config has positive twinkle speed */
void test_default_config_twinkle(void)
{
    sd_config_t cfg = sd_default_config();
    TEST_ASSERT_TRUE(cfg.twinkle_speed > 0.0f);
}

/* 4. Vertex shader starts with #version 300 es */
void test_vert_source_version(void)
{
    const char *src = sd_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 5. Fragment shader starts with #version 300 es */
void test_frag_source_version(void)
{
    const char *src = sd_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_EQUAL_INT(0, strncmp(src, "#version 300 es", 15));
}

/* 6. Fragment shader has hash functions */
void test_frag_has_hash(void)
{
    const char *src = sd_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "hash21"));
    TEST_ASSERT_NOT_NULL(strstr(src, "hash22"));
}

/* 7. Fragment shader has star layer function */
void test_frag_has_star_layer(void)
{
    const char *src = sd_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "star_layer"));
}

/* 8. Fragment shader has spectral color */
void test_frag_has_spectral_color(void)
{
    const char *src = sd_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "star_spectral_color"));
}

/* 9. Fragment shader has density uniform */
void test_frag_has_density_uniform(void)
{
    const char *src = sd_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_density"));
}

/* 10. Fragment shader has zoom_alpha uniform */
void test_frag_has_zoom_alpha(void)
{
    const char *src = sd_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_zoom_alpha"));
}

/* 11. Fragment shader has 3 star layers */
void test_frag_has_three_layers(void)
{
    const char *src = sd_frag_source();
    /* Count occurrences of star_layer( in main */
    int count = 0;
    const char *p = src;
    while ((p = strstr(p, "star_layer(v_uv")) != NULL) {
        count++;
        p++;
    }
    TEST_ASSERT_EQUAL_INT(3, count);
}

/* 12. Fragment shader has discard for performance */
void test_frag_has_discard(void)
{
    const char *src = sd_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "discard"));
}

/* 13. Vertex shader has u_mvp uniform */
void test_vert_has_mvp(void)
{
    const char *src = sd_vert_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "u_mvp"));
}

/* 14. Fragment shader has frag_color output */
void test_frag_has_output(void)
{
    const char *src = sd_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "frag_color"));
}

/* 15. Shader source has reasonable length */
void test_shader_reasonable_length(void)
{
    const char *vert = sd_vert_source();
    const char *frag = sd_frag_source();
    TEST_ASSERT_TRUE(strlen(vert) > 50);
    TEST_ASSERT_TRUE(strlen(vert) < 2000);
    TEST_ASSERT_TRUE(strlen(frag) > 200);
    TEST_ASSERT_TRUE(strlen(frag) < 8000);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_default_config_density);
    RUN_TEST(test_default_config_brightness);
    RUN_TEST(test_default_config_twinkle);
    RUN_TEST(test_vert_source_version);
    RUN_TEST(test_frag_source_version);
    RUN_TEST(test_frag_has_hash);
    RUN_TEST(test_frag_has_star_layer);
    RUN_TEST(test_frag_has_spectral_color);
    RUN_TEST(test_frag_has_density_uniform);
    RUN_TEST(test_frag_has_zoom_alpha);
    RUN_TEST(test_frag_has_three_layers);
    RUN_TEST(test_frag_has_discard);
    RUN_TEST(test_vert_has_mvp);
    RUN_TEST(test_frag_has_output);
    RUN_TEST(test_shader_reasonable_length);
    return UNITY_END();
}
