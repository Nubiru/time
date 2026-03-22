/*
 * test_zoom_fade.c — Tests for zoom-level pass opacity mapping
 */

#include "../unity/unity.h"
#include "../../src/render/zoom_fade.h"

void setUp(void) {}
void tearDown(void) {}

/* 1. Milkyway invisible at solar system zoom */
void test_milkyway_invisible_at_solar(void)
{
    float a = zf_opacity(ZF_MILKYWAY, 3.4f); /* SOLAR center */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, a);
}

/* 2. Milkyway fully visible at galaxy zoom */
void test_milkyway_visible_at_galaxy(void)
{
    float a = zf_opacity(ZF_MILKYWAY, 7.6f); /* GALAXY center */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, a);
}

/* 3. Stars visible at solar zoom */
void test_stars_visible_at_solar(void)
{
    float a = zf_opacity(ZF_STARS, 3.4f); /* SOLAR center */
    TEST_ASSERT_TRUE(a > 0.5f);
}

/* 4. Stars invisible at personal zoom */
void test_stars_invisible_at_personal(void)
{
    float a = zf_opacity(ZF_STARS, -2.3f); /* PERSONAL center */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, a);
}

/* 5. Planets visible at inner solar */
void test_planets_visible_at_inner(void)
{
    float a = zf_opacity(ZF_PLANET, 1.4f); /* INNER center */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, a);
}

/* 6. Planets invisible at galaxy zoom */
void test_planets_invisible_at_galaxy(void)
{
    float a = zf_opacity(ZF_PLANET, 7.6f); /* GALAXY center */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, a);
}

/* 7. Cards visible at personal zoom */
void test_cards_visible_at_personal(void)
{
    float a = zf_opacity(ZF_CARD, -2.3f); /* PERSONAL center */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, a);
}

/* 8. Cards nearly invisible at solar zoom (deep in fade-out zone) */
void test_cards_invisible_at_solar(void)
{
    float a = zf_opacity(ZF_CARD, 3.4f); /* SOLAR center */
    TEST_ASSERT_TRUE(a < 0.05f); /* effectively invisible */
}

/* 9. Overlay always visible */
void test_overlay_always_visible(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, zf_opacity(ZF_OVERLAY, -4.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, zf_opacity(ZF_OVERLAY, 0.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, zf_opacity(ZF_OVERLAY, 10.0f));
}

/* 10. Fade transitions are smooth (not binary) */
void test_fade_is_smooth(void)
{
    /* Milkyway fades in between 4.0 and 5.0 (show_min=5, fade_width=1) */
    float a1 = zf_opacity(ZF_MILKYWAY, 3.5f); /* before fade */
    float a2 = zf_opacity(ZF_MILKYWAY, 4.5f); /* mid fade */
    float a3 = zf_opacity(ZF_MILKYWAY, 5.5f); /* after fade */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, a1);
    TEST_ASSERT_TRUE(a2 > 0.1f && a2 < 0.9f); /* mid-transition */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, a3);
}

/* 11. All opacity values are in [0,1] */
void test_opacity_clamped(void)
{
    for (int c = 0; c < ZF_CATEGORY_COUNT; c++) {
        for (float z = -5.0f; z <= 11.0f; z += 0.5f) {
            float a = zf_opacity((zf_category_t)c, z);
            TEST_ASSERT_TRUE(a >= 0.0f);
            TEST_ASSERT_TRUE(a <= 1.0f);
        }
    }
}

/* 12. Zodiac invisible at galaxy */
void test_zodiac_invisible_at_galaxy(void)
{
    float a = zf_opacity(ZF_ZODIAC, 7.6f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, a);
}

/* 13. Orbit trails visible at inner, invisible at local */
void test_orbit_trails_inner_visible(void)
{
    float a = zf_opacity(ZF_ORBIT_TRAIL, 1.4f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, a);
}

/* 14. Orbit trails invisible at galaxy */
void test_orbit_trails_invisible_at_galaxy(void)
{
    float a = zf_opacity(ZF_ORBIT_TRAIL, 7.6f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, a);
}

/* 15. Rings visible at solar zoom */
void test_rings_visible_at_solar(void)
{
    float a = zf_opacity(ZF_RING, 3.4f);
    TEST_ASSERT_TRUE(a > 0.5f);
}

/* 16. Text labels visible at earth zoom */
void test_text_labels_visible_at_earth(void)
{
    float a = zf_opacity(ZF_TEXT_LABEL, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, a);
}

/* 17. Text labels invisible at galaxy */
void test_text_labels_invisible_at_galaxy(void)
{
    float a = zf_opacity(ZF_TEXT_LABEL, 7.6f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, a);
}

/* 18. Constellations visible at local zoom */
void test_constellations_visible_at_local(void)
{
    float a = zf_opacity(ZF_CONSTELLATION, 5.3f);
    TEST_ASSERT_TRUE(a > 0.5f);
}

/* 19. Range returns valid values */
void test_range_valid(void)
{
    for (int c = 0; c < ZF_CATEGORY_COUNT; c++) {
        zf_range_t r = zf_range((zf_category_t)c);
        TEST_ASSERT_TRUE(r.show_max > r.show_min);
    }
}

/* 20. Smoothstep helper works correctly */
void test_smoothstep_boundaries(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, zf_smoothstep(0.0f, 1.0f, -0.5f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, zf_smoothstep(0.0f, 1.0f, 0.5f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, zf_smoothstep(0.0f, 1.0f, 1.5f));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_milkyway_invisible_at_solar);
    RUN_TEST(test_milkyway_visible_at_galaxy);
    RUN_TEST(test_stars_visible_at_solar);
    RUN_TEST(test_stars_invisible_at_personal);
    RUN_TEST(test_planets_visible_at_inner);
    RUN_TEST(test_planets_invisible_at_galaxy);
    RUN_TEST(test_cards_visible_at_personal);
    RUN_TEST(test_cards_invisible_at_solar);
    RUN_TEST(test_overlay_always_visible);
    RUN_TEST(test_fade_is_smooth);
    RUN_TEST(test_opacity_clamped);
    RUN_TEST(test_zodiac_invisible_at_galaxy);
    RUN_TEST(test_orbit_trails_inner_visible);
    RUN_TEST(test_orbit_trails_invisible_at_galaxy);
    RUN_TEST(test_rings_visible_at_solar);
    RUN_TEST(test_text_labels_visible_at_earth);
    RUN_TEST(test_text_labels_invisible_at_galaxy);
    RUN_TEST(test_constellations_visible_at_local);
    RUN_TEST(test_range_valid);
    RUN_TEST(test_smoothstep_boundaries);
    return UNITY_END();
}
