/*
 * test_post_process.c — Tests for post-processing FBO pipeline module
 *
 * Tests: default config, pp_info, VRAM estimation, fullscreen quad packing,
 * shader source strings (quad vert, bright extract, blur, composite).
 *
 * 50 tests covering all public API functions.
 */

#include "../unity/unity.h"
#include "../../src/render/post_process.h"

#include <math.h>
#include <string.h>

#define FTOL 0.001f
#define FCLOSE(exp, act) TEST_ASSERT_FLOAT_WITHIN(FTOL, (exp), (act))

void setUp(void) {}
void tearDown(void) {}

/* ======================================================================
 * 1. pp_default_config — sensible defaults
 * ====================================================================== */

/* 1. Bloom enabled by default */
void test_default_bloom_enabled(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_EQUAL_INT(1, cfg.bloom_enabled);
}

/* 2. Bloom threshold is reasonable (0.5-1.0) */
void test_default_bloom_threshold(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_TRUE(cfg.bloom_threshold >= 0.5f && cfg.bloom_threshold <= 1.0f);
}

/* 3. Bloom intensity is positive and <= 1.0 */
void test_default_bloom_intensity(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_TRUE(cfg.bloom_intensity > 0.0f && cfg.bloom_intensity <= 1.0f);
}

/* 4. Bloom passes is at least 1 */
void test_default_bloom_passes(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_TRUE(cfg.bloom_passes >= 1);
}

/* 5. Tone mapping enabled by default */
void test_default_tonemap_enabled(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_EQUAL_INT(1, cfg.tonemap_enabled);
}

/* 6. Default exposure is 1.0 */
void test_default_exposure(void)
{
    pp_config_t cfg = pp_default_config();
    FCLOSE(1.0f, cfg.exposure);
}

/* 7. Default gamma is 2.2 */
void test_default_gamma(void)
{
    pp_config_t cfg = pp_default_config();
    FCLOSE(2.2f, cfg.gamma);
}

/* 8. Vignette enabled by default */
void test_default_vignette_enabled(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_EQUAL_INT(1, cfg.vignette_enabled);
}

/* 9. Vignette strength in [0, 1] */
void test_default_vignette_strength(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_TRUE(cfg.vignette_strength >= 0.0f && cfg.vignette_strength <= 1.0f);
}

/* 10. Vignette radius in [0, 1] */
void test_default_vignette_radius(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_TRUE(cfg.vignette_radius >= 0.0f && cfg.vignette_radius <= 1.0f);
}

/* 11. Grain disabled by default */
void test_default_grain_disabled(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_EQUAL_INT(0, cfg.grain_enabled);
}

/* 12. Grain intensity is small */
void test_default_grain_intensity(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_TRUE(cfg.grain_intensity >= 0.0f && cfg.grain_intensity <= 0.1f);
}

/* 13. Default screen dimensions are zero (user must set) */
void test_default_screen_dims_zero(void)
{
    pp_config_t cfg = pp_default_config();
    TEST_ASSERT_EQUAL_INT(0, cfg.screen_width);
    TEST_ASSERT_EQUAL_INT(0, cfg.screen_height);
}

/* ======================================================================
 * 2. pp_info — FBO/texture counts
 * ====================================================================== */

/* 14. With all effects enabled: multiple FBOs */
void test_info_all_enabled_fbo_count(void)
{
    pp_config_t cfg = pp_default_config();
    cfg.bloom_enabled = 1;
    pp_info_t info = pp_info(cfg);
    TEST_ASSERT_TRUE(info.fbo_count >= 2);
}

/* 15. With all effects enabled: multiple textures */
void test_info_all_enabled_texture_count(void)
{
    pp_config_t cfg = pp_default_config();
    cfg.bloom_enabled = 1;
    pp_info_t info = pp_info(cfg);
    TEST_ASSERT_TRUE(info.texture_count >= 2);
}

/* 16. With all effects enabled: positive VRAM */
void test_info_all_enabled_vram_positive(void)
{
    pp_config_t cfg = pp_default_config();
    cfg.bloom_enabled = 1;
    cfg.screen_width = 1920;
    cfg.screen_height = 1080;
    pp_info_t info = pp_info(cfg);
    TEST_ASSERT_TRUE(info.total_vram_bytes > 0);
}

/* 17. With bloom disabled: fewer FBOs */
void test_info_bloom_disabled_fewer_fbos(void)
{
    pp_config_t cfg = pp_default_config();
    cfg.bloom_enabled = 1;
    cfg.screen_width = 1920;
    cfg.screen_height = 1080;
    pp_info_t info_on = pp_info(cfg);

    cfg.bloom_enabled = 0;
    pp_info_t info_off = pp_info(cfg);
    TEST_ASSERT_TRUE(info_off.fbo_count < info_on.fbo_count);
}

/* 18. With bloom disabled: fewer textures */
void test_info_bloom_disabled_fewer_textures(void)
{
    pp_config_t cfg = pp_default_config();
    cfg.bloom_enabled = 1;
    cfg.screen_width = 1920;
    cfg.screen_height = 1080;
    pp_info_t info_on = pp_info(cfg);

    cfg.bloom_enabled = 0;
    pp_info_t info_off = pp_info(cfg);
    TEST_ASSERT_TRUE(info_off.texture_count < info_on.texture_count);
}

/* 19. FBO count >= 1 even with everything off (still need scene FBO) */
void test_info_minimum_fbo(void)
{
    pp_config_t cfg = pp_default_config();
    cfg.bloom_enabled = 0;
    cfg.tonemap_enabled = 0;
    cfg.vignette_enabled = 0;
    cfg.grain_enabled = 0;
    pp_info_t info = pp_info(cfg);
    TEST_ASSERT_TRUE(info.fbo_count >= 1);
}

/* 20. Texture count >= 1 always */
void test_info_minimum_texture(void)
{
    pp_config_t cfg = pp_default_config();
    cfg.bloom_enabled = 0;
    cfg.tonemap_enabled = 0;
    cfg.vignette_enabled = 0;
    cfg.grain_enabled = 0;
    pp_info_t info = pp_info(cfg);
    TEST_ASSERT_TRUE(info.texture_count >= 1);
}

/* ======================================================================
 * 3. pp_estimate_vram — memory estimation
 * ====================================================================== */

/* 21. VRAM positive for HD resolution */
void test_estimate_vram_hd_positive(void)
{
    pp_config_t cfg = pp_default_config();
    int vram = pp_estimate_vram(1920, 1080, cfg);
    TEST_ASSERT_TRUE(vram > 0);
}

/* 22. Larger resolution = more VRAM */
void test_estimate_vram_larger_more(void)
{
    pp_config_t cfg = pp_default_config();
    int vram_hd = pp_estimate_vram(1920, 1080, cfg);
    int vram_4k = pp_estimate_vram(3840, 2160, cfg);
    TEST_ASSERT_TRUE(vram_4k > vram_hd);
}

/* 23. Bloom enabled = more VRAM than disabled */
void test_estimate_vram_bloom_more(void)
{
    pp_config_t cfg = pp_default_config();
    cfg.bloom_enabled = 1;
    int vram_on = pp_estimate_vram(1920, 1080, cfg);
    cfg.bloom_enabled = 0;
    int vram_off = pp_estimate_vram(1920, 1080, cfg);
    TEST_ASSERT_TRUE(vram_on > vram_off);
}

/* 24. Zero dimensions = zero VRAM */
void test_estimate_vram_zero_dims(void)
{
    pp_config_t cfg = pp_default_config();
    int vram = pp_estimate_vram(0, 0, cfg);
    TEST_ASSERT_EQUAL_INT(0, vram);
}

/* 25. VRAM estimate is in reasonable range for 1080p (1-100 MB) */
void test_estimate_vram_reasonable_range(void)
{
    pp_config_t cfg = pp_default_config();
    int vram = pp_estimate_vram(1920, 1080, cfg);
    /* Should be in range [1MB, 100MB] */
    TEST_ASSERT_TRUE(vram >= 1024 * 1024);
    TEST_ASSERT_TRUE(vram <= 100 * 1024 * 1024);
}

/* ======================================================================
 * 4. pp_pack_quad — fullscreen oversized triangle
 * ====================================================================== */

/* 26. Returns 3 vertices (single triangle replaces quad) */
void test_quad_returns_3(void)
{
    float out[PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS];
    int count = pp_pack_quad(out);
    TEST_ASSERT_EQUAL_INT(3, count);
}

/* 27. Vertex 0: position (-1,-1), UV (0,0) */
void test_quad_vertex0(void)
{
    float out[PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS];
    pp_pack_quad(out);
    FCLOSE(-1.0f, out[0]);  /* x */
    FCLOSE(-1.0f, out[1]);  /* y */
    FCLOSE( 0.0f, out[2]);  /* u */
    FCLOSE( 0.0f, out[3]);  /* v */
}

/* 28. Vertex 1: position (3,-1), UV (2,0) — extended right */
void test_quad_vertex1(void)
{
    float out[PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS];
    pp_pack_quad(out);
    FCLOSE( 3.0f, out[4]);  /* x */
    FCLOSE(-1.0f, out[5]);  /* y */
    FCLOSE( 2.0f, out[6]);  /* u */
    FCLOSE( 0.0f, out[7]);  /* v */
}

/* 29. Vertex 2: position (-1,3), UV (0,2) — extended up */
void test_quad_vertex2(void)
{
    float out[PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS];
    pp_pack_quad(out);
    FCLOSE(-1.0f, out[8]);   /* x */
    FCLOSE( 3.0f, out[9]);   /* y */
    FCLOSE( 0.0f, out[10]);  /* u */
    FCLOSE( 2.0f, out[11]);  /* v */
}

/* 30. Triangle covers the full NDC rect: has x < 0 and x > 0, y < 0 and y > 0 */
void test_quad_covers_full_ndc(void)
{
    float out[PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS];
    pp_pack_quad(out);

    int has_neg_x = 0, has_pos_x = 0;
    int has_neg_y = 0, has_pos_y = 0;
    for (int i = 0; i < PP_QUAD_VERTEX_COUNT; i++) {
        float x = out[i * PP_QUAD_VERTEX_FLOATS + 0];
        float y = out[i * PP_QUAD_VERTEX_FLOATS + 1];
        if (x < 0.0f) has_neg_x = 1;
        if (x > 0.0f) has_pos_x = 1;
        if (y < 0.0f) has_neg_y = 1;
        if (y > 0.0f) has_pos_y = 1;
    }
    TEST_ASSERT_TRUE(has_neg_x && has_pos_x);
    TEST_ASSERT_TRUE(has_neg_y && has_pos_y);
}

/* 31. Texcoord (0,0) maps to position (-1,-1) */
void test_quad_texcoord_origin_maps_to_ndc_origin(void)
{
    float out[PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS];
    pp_pack_quad(out);

    /* First vertex should have UV (0,0) at pos (-1,-1) */
    FCLOSE(0.0f, out[2]);
    FCLOSE(0.0f, out[3]);
    FCLOSE(-1.0f, out[0]);
    FCLOSE(-1.0f, out[1]);
}

/* 32. NULL output returns 0 */
void test_quad_null_returns_zero(void)
{
    int count = pp_pack_quad(NULL);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* 33. UV for visible region: at NDC (0,0) UV should be (0.5,0.5) — linear interpolation */
void test_quad_uv_center_correct(void)
{
    /* For the oversized triangle, the UV at any NDC point (x,y) is:
     * u = (x + 1) / 2, v = (y + 1) / 2
     * At NDC center (0,0): u=0.5, v=0.5
     * This is verified by the vertex data: the linear mapping from
     * pos to UV gives UV = (pos + 1) / 2 for the entire triangle. */
    float out[PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS];
    pp_pack_quad(out);

    /* Check vertex 0: pos(-1,-1) -> uv should be (0,0) = (-1+1)/2 */
    FCLOSE((-1.0f + 1.0f) / 2.0f, out[2]);
    FCLOSE((-1.0f + 1.0f) / 2.0f, out[3]);

    /* Check vertex 1: pos(3,-1) -> uv should be (2,0) = (3+1)/2 */
    FCLOSE((3.0f + 1.0f) / 2.0f, out[6]);
    FCLOSE((-1.0f + 1.0f) / 2.0f, out[7]);
}

/* ======================================================================
 * 5. pp_quad_vert_source — passthrough vertex shader
 * ====================================================================== */

/* 32. Non-null and non-empty */
void test_quad_vert_source_exists(void)
{
    const char *src = pp_quad_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 33. Contains GLSL ES 3.00 version */
void test_quad_vert_version_300(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_quad_vert_source(), "#version 300 es"));
}

/* 34. Contains void main() */
void test_quad_vert_has_main(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_quad_vert_source(), "void main()"));
}

/* 35. Contains gl_Position */
void test_quad_vert_has_gl_position(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_quad_vert_source(), "gl_Position"));
}

/* 36. Contains v_uv varying output */
void test_quad_vert_has_v_uv(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_quad_vert_source(), "v_uv"));
}

/* ======================================================================
 * 6. pp_bright_extract_frag_source — brightness extraction
 * ====================================================================== */

/* 37. Non-null and non-empty */
void test_bright_extract_source_exists(void)
{
    const char *src = pp_bright_extract_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 38. Contains u_threshold uniform */
void test_bright_extract_has_threshold(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_bright_extract_frag_source(), "u_threshold"));
}

/* 39. Contains luminance coefficients (Rec. 709) */
void test_bright_extract_has_luminance(void)
{
    const char *src = pp_bright_extract_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "0.2126"));
    TEST_ASSERT_NOT_NULL(strstr(src, "0.7152"));
    TEST_ASSERT_NOT_NULL(strstr(src, "0.0722"));
}

/* 40. Contains version 300 es */
void test_bright_extract_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_bright_extract_frag_source(), "#version 300 es"));
}

/* ======================================================================
 * 7. pp_blur_frag_source — Gaussian blur
 * ====================================================================== */

/* 41. Non-null and non-empty */
void test_blur_source_exists(void)
{
    const char *src = pp_blur_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 42. Contains u_horizontal uniform */
void test_blur_has_horizontal(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_blur_frag_source(), "u_horizontal"));
}

/* 43. Contains Gaussian weight 0.227027 (center tap) */
void test_blur_has_gaussian_weights(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_blur_frag_source(), "0.227027"));
}

/* 44. Contains version 300 es */
void test_blur_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_blur_frag_source(), "#version 300 es"));
}

/* 45. Contains textureSize for offset calculation */
void test_blur_has_texture_size(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_blur_frag_source(), "textureSize"));
}

/* ======================================================================
 * 8. pp_composite_frag_source — final composite
 * ====================================================================== */

/* 46. Non-null and non-empty */
void test_composite_source_exists(void)
{
    const char *src = pp_composite_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_TRUE(strlen(src) > 10);
}

/* 47. Contains version 300 es */
void test_composite_version(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_composite_frag_source(), "#version 300 es"));
}

/* 48. Contains u_bloom_intensity uniform */
void test_composite_has_bloom_intensity(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_composite_frag_source(), "u_bloom_intensity"));
}

/* 49. Contains u_exposure uniform */
void test_composite_has_exposure(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_composite_frag_source(), "u_exposure"));
}

/* 50. Contains u_gamma uniform */
void test_composite_has_gamma(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_composite_frag_source(), "u_gamma"));
}

/* 51. Contains u_vignette_strength uniform */
void test_composite_has_vignette(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_composite_frag_source(), "u_vignette_strength"));
}

/* 52. Contains u_grain_intensity uniform */
void test_composite_has_grain(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_composite_frag_source(), "u_grain_intensity"));
}

/* 53. Contains Reinhard tone mapping: color / (color + 1) pattern */
void test_composite_has_reinhard(void)
{
    const char *src = pp_composite_frag_source();
    /* Look for the characteristic Reinhard pattern */
    TEST_ASSERT_NOT_NULL(strstr(src, "color + vec3(1.0)"));
}

/* 54. Contains grain function for film grain effect */
void test_composite_has_noise(void)
{
    const char *src = pp_composite_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "grain"));
}

/* 55. Contains smoothstep for vignette */
void test_composite_has_smoothstep(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_composite_frag_source(), "smoothstep"));
}

/* 56. Contains void main() */
void test_composite_has_main(void)
{
    TEST_ASSERT_NOT_NULL(strstr(pp_composite_frag_source(), "void main()"));
}

/* 57. Composite shader is longer than other shaders (it includes noise) */
void test_composite_longer_than_blur(void)
{
    size_t comp_len = strlen(pp_composite_frag_source());
    size_t blur_len = strlen(pp_blur_frag_source());
    TEST_ASSERT_TRUE(comp_len > blur_len);
}

/* 58. Calling composite source twice returns same pointer */
void test_composite_source_stable(void)
{
    const char *a = pp_composite_frag_source();
    const char *b = pp_composite_frag_source();
    TEST_ASSERT_EQUAL_PTR(a, b);
}

/* ======================================================================
 * Runner
 * ====================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Default config (13 tests) */
    RUN_TEST(test_default_bloom_enabled);
    RUN_TEST(test_default_bloom_threshold);
    RUN_TEST(test_default_bloom_intensity);
    RUN_TEST(test_default_bloom_passes);
    RUN_TEST(test_default_tonemap_enabled);
    RUN_TEST(test_default_exposure);
    RUN_TEST(test_default_gamma);
    RUN_TEST(test_default_vignette_enabled);
    RUN_TEST(test_default_vignette_strength);
    RUN_TEST(test_default_vignette_radius);
    RUN_TEST(test_default_grain_disabled);
    RUN_TEST(test_default_grain_intensity);
    RUN_TEST(test_default_screen_dims_zero);

    /* pp_info (7 tests) */
    RUN_TEST(test_info_all_enabled_fbo_count);
    RUN_TEST(test_info_all_enabled_texture_count);
    RUN_TEST(test_info_all_enabled_vram_positive);
    RUN_TEST(test_info_bloom_disabled_fewer_fbos);
    RUN_TEST(test_info_bloom_disabled_fewer_textures);
    RUN_TEST(test_info_minimum_fbo);
    RUN_TEST(test_info_minimum_texture);

    /* pp_estimate_vram (5 tests) */
    RUN_TEST(test_estimate_vram_hd_positive);
    RUN_TEST(test_estimate_vram_larger_more);
    RUN_TEST(test_estimate_vram_bloom_more);
    RUN_TEST(test_estimate_vram_zero_dims);
    RUN_TEST(test_estimate_vram_reasonable_range);

    /* pp_pack_quad (8 tests) */
    RUN_TEST(test_quad_returns_3);
    RUN_TEST(test_quad_vertex0);
    RUN_TEST(test_quad_vertex1);
    RUN_TEST(test_quad_vertex2);
    RUN_TEST(test_quad_covers_full_ndc);
    RUN_TEST(test_quad_texcoord_origin_maps_to_ndc_origin);
    RUN_TEST(test_quad_null_returns_zero);
    RUN_TEST(test_quad_uv_center_correct);

    /* Quad vertex shader (5 tests) */
    RUN_TEST(test_quad_vert_source_exists);
    RUN_TEST(test_quad_vert_version_300);
    RUN_TEST(test_quad_vert_has_main);
    RUN_TEST(test_quad_vert_has_gl_position);
    RUN_TEST(test_quad_vert_has_v_uv);

    /* Brightness extraction shader (4 tests) */
    RUN_TEST(test_bright_extract_source_exists);
    RUN_TEST(test_bright_extract_has_threshold);
    RUN_TEST(test_bright_extract_has_luminance);
    RUN_TEST(test_bright_extract_version);

    /* Blur shader (5 tests) */
    RUN_TEST(test_blur_source_exists);
    RUN_TEST(test_blur_has_horizontal);
    RUN_TEST(test_blur_has_gaussian_weights);
    RUN_TEST(test_blur_version);
    RUN_TEST(test_blur_has_texture_size);

    /* Composite shader (11 tests) */
    RUN_TEST(test_composite_source_exists);
    RUN_TEST(test_composite_version);
    RUN_TEST(test_composite_has_bloom_intensity);
    RUN_TEST(test_composite_has_exposure);
    RUN_TEST(test_composite_has_gamma);
    RUN_TEST(test_composite_has_vignette);
    RUN_TEST(test_composite_has_grain);
    RUN_TEST(test_composite_has_reinhard);
    RUN_TEST(test_composite_has_noise);
    RUN_TEST(test_composite_has_smoothstep);
    RUN_TEST(test_composite_has_main);
    RUN_TEST(test_composite_longer_than_blur);
    RUN_TEST(test_composite_source_stable);

    return UNITY_END();
}
