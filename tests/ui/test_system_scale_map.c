#include "../unity/unity.h"
#include "../../src/ui/system_scale_map.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Log_zoom centers from camera_scale.c */
#define LOG_PERSONAL  -2.303f
#define LOG_EARTH      0.0f
#define LOG_INNER      1.386f
#define LOG_SOLAR      3.401f
#define LOG_LOCAL      5.298f
#define LOG_GALAXY     7.601f
#define LOG_UNIVERSE   9.903f

/* System names: 1-2 */

void test_all_system_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregorian", system_name(SYS_GREGORIAN));
    TEST_ASSERT_EQUAL_STRING("Astrology", system_name(SYS_ASTROLOGY));
    TEST_ASSERT_EQUAL_STRING("Tzolkin", system_name(SYS_TZOLKIN));
    TEST_ASSERT_EQUAL_STRING("I Ching", system_name(SYS_ICHING));
    TEST_ASSERT_EQUAL_STRING("Chinese", system_name(SYS_CHINESE));
    TEST_ASSERT_EQUAL_STRING("Human Design", system_name(SYS_HD));
    TEST_ASSERT_EQUAL_STRING("Cosmic Time", system_name(SYS_COSMIC));
}

void test_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", system_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", system_name(99));
}

/* Home scales: 3-5 */

void test_home_scale_astrology(void)
{
    TEST_ASSERT_EQUAL_INT(2, system_home_scale(SYS_ASTROLOGY)); /* Inner Solar */
}

void test_home_scale_cosmic(void)
{
    TEST_ASSERT_EQUAL_INT(5, system_home_scale(SYS_COSMIC)); /* Galaxy */
}

void test_home_scale_personal_systems(void)
{
    /* Tzolkin, I Ching, HD all home at Personal */
    TEST_ASSERT_EQUAL_INT(0, system_home_scale(SYS_TZOLKIN));
    TEST_ASSERT_EQUAL_INT(0, system_home_scale(SYS_ICHING));
    TEST_ASSERT_EQUAL_INT(0, system_home_scale(SYS_HD));
}

/* Gregorian always visible: 6-7 */

void test_gregorian_visible_at_personal(void)
{
    float opacity = system_opacity_at_zoom(SYS_GREGORIAN, LOG_PERSONAL);
    TEST_ASSERT_TRUE(opacity > 0.0f);
}

void test_gregorian_visible_at_universe(void)
{
    float opacity = system_opacity_at_zoom(SYS_GREGORIAN, LOG_UNIVERSE);
    TEST_ASSERT_TRUE(opacity > 0.0f);
}

/* Opacity at home scale near 1.0: 8-9 */

void test_astrology_opacity_at_home(void)
{
    float opacity = system_opacity_at_zoom(SYS_ASTROLOGY, LOG_INNER);
    TEST_ASSERT_FLOAT_WITHIN(0.2f, 1.0f, opacity);
}

void test_cosmic_opacity_at_home(void)
{
    float opacity = system_opacity_at_zoom(SYS_COSMIC, LOG_GALAXY);
    TEST_ASSERT_FLOAT_WITHIN(0.2f, 1.0f, opacity);
}

/* Opacity far from home near 0.0: 10-11 */

void test_astrology_invisible_at_universe(void)
{
    float opacity = system_opacity_at_zoom(SYS_ASTROLOGY, LOG_UNIVERSE);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, opacity);
}

void test_cosmic_invisible_at_personal(void)
{
    float opacity = system_opacity_at_zoom(SYS_COSMIC, LOG_PERSONAL);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, opacity);
}

/* Cosmic only at galaxy+: 12 */

void test_cosmic_only_high_zoom(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, system_opacity_at_zoom(SYS_COSMIC, LOG_SOLAR));
    TEST_ASSERT_TRUE(system_opacity_at_zoom(SYS_COSMIC, LOG_GALAXY) > 0.5f);
}

/* systems_visible_at_zoom: 13-15 */

void test_visible_at_personal(void)
{
    knowledge_system_t out[KNOWLEDGE_SYSTEM_COUNT];
    int count = systems_visible_at_zoom(LOG_PERSONAL, out, KNOWLEDGE_SYSTEM_COUNT);
    /* At personal scale: Gregorian, Astrology, Tzolkin, I Ching, Chinese, HD should be visible */
    TEST_ASSERT_TRUE(count >= 5);
    /* Cosmic should NOT be visible */
    int found_cosmic = 0;
    for (int i = 0; i < count; i++) {
        if (out[i] == SYS_COSMIC) found_cosmic = 1;
    }
    TEST_ASSERT_FALSE(found_cosmic);
}

void test_visible_at_galaxy(void)
{
    knowledge_system_t out[KNOWLEDGE_SYSTEM_COUNT];
    int count = systems_visible_at_zoom(LOG_GALAXY, out, KNOWLEDGE_SYSTEM_COUNT);
    /* At galaxy scale: Gregorian + Cosmic should be visible */
    TEST_ASSERT_TRUE(count >= 2);
    int found_cosmic = 0;
    for (int i = 0; i < count; i++) {
        if (out[i] == SYS_COSMIC) found_cosmic = 1;
    }
    TEST_ASSERT_TRUE(found_cosmic);
}

void test_visible_returns_subset(void)
{
    knowledge_system_t out[KNOWLEDGE_SYSTEM_COUNT];
    int count = systems_visible_at_zoom(LOG_SOLAR, out, KNOWLEDGE_SYSTEM_COUNT);
    TEST_ASSERT_TRUE(count > 0);
    TEST_ASSERT_TRUE(count <= KNOWLEDGE_SYSTEM_COUNT);
}

/* Fade ramp monotonic: 16-17 */

void test_fade_in_monotonic(void)
{
    /* Tzolkin: as we zoom from far below minimum toward home, opacity increases */
    float prev = 0.0f;
    system_scale_info_t info = system_scale_info(SYS_TZOLKIN);
    for (float z = info.scale_min; z <= info.scale_min + 1.0f; z += 0.1f) {
        float val = system_opacity_at_zoom(SYS_TZOLKIN, z);
        TEST_ASSERT_TRUE(val >= prev - 0.001f);
        prev = val;
    }
}

void test_fade_out_monotonic(void)
{
    /* Tzolkin: as we zoom past its max, opacity decreases */
    system_scale_info_t info = system_scale_info(SYS_TZOLKIN);
    float prev = 1.0f;
    for (float z = info.scale_max - 1.0f; z <= info.scale_max; z += 0.1f) {
        float val = system_opacity_at_zoom(SYS_TZOLKIN, z);
        TEST_ASSERT_TRUE(val <= prev + 0.001f);
        prev = val;
    }
}

/* Opacity clamped: 18 */

void test_opacity_clamped(void)
{
    for (int s = 0; s < KNOWLEDGE_SYSTEM_COUNT; s++) {
        for (float z = -5.0f; z <= 11.0f; z += 1.0f) {
            float val = system_opacity_at_zoom((knowledge_system_t)s, z);
            TEST_ASSERT_TRUE(val >= 0.0f);
            TEST_ASSERT_TRUE(val <= 1.0f);
        }
    }
}

/* system_count: 19 */

void test_system_count(void)
{
    TEST_ASSERT_EQUAL_INT(7, system_count());
}

/* system_scale_info: 20 */

void test_scale_info_fields(void)
{
    system_scale_info_t info = system_scale_info(SYS_ASTROLOGY);
    TEST_ASSERT_EQUAL_INT(SYS_ASTROLOGY, info.id);
    TEST_ASSERT_EQUAL_STRING("Astrology", info.name);
    TEST_ASSERT_EQUAL_INT(2, info.home_scale);
    TEST_ASSERT_TRUE(info.scale_min < info.scale_max);
    TEST_ASSERT_TRUE(info.home_opacity > 0.0f);
}

/* Invalid system: 21 */

void test_invalid_system_opacity(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, system_opacity_at_zoom(-1, LOG_SOLAR));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, system_opacity_at_zoom(99, LOG_SOLAR));
}

/* Purity: 22 */

void test_purity(void)
{
    float a = system_opacity_at_zoom(SYS_ASTROLOGY, LOG_INNER);
    float b = system_opacity_at_zoom(SYS_ASTROLOGY, LOG_INNER);
    TEST_ASSERT_EQUAL_FLOAT(a, b);

    system_scale_info_t ia = system_scale_info(SYS_COSMIC);
    system_scale_info_t ib = system_scale_info(SYS_COSMIC);
    TEST_ASSERT_EQUAL_INT(ia.home_scale, ib.home_scale);
    TEST_ASSERT_EQUAL_FLOAT(ia.scale_min, ib.scale_min);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_all_system_names);
    RUN_TEST(test_name_invalid);
    RUN_TEST(test_home_scale_astrology);
    RUN_TEST(test_home_scale_cosmic);
    RUN_TEST(test_home_scale_personal_systems);
    RUN_TEST(test_gregorian_visible_at_personal);
    RUN_TEST(test_gregorian_visible_at_universe);
    RUN_TEST(test_astrology_opacity_at_home);
    RUN_TEST(test_cosmic_opacity_at_home);
    RUN_TEST(test_astrology_invisible_at_universe);
    RUN_TEST(test_cosmic_invisible_at_personal);
    RUN_TEST(test_cosmic_only_high_zoom);
    RUN_TEST(test_visible_at_personal);
    RUN_TEST(test_visible_at_galaxy);
    RUN_TEST(test_visible_returns_subset);
    RUN_TEST(test_fade_in_monotonic);
    RUN_TEST(test_fade_out_monotonic);
    RUN_TEST(test_opacity_clamped);
    RUN_TEST(test_system_count);
    RUN_TEST(test_scale_info_fields);
    RUN_TEST(test_invalid_system_opacity);
    RUN_TEST(test_purity);
    return UNITY_END();
}
