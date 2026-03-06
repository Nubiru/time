#include "../unity/unity.h"
#include "../../src/ui/scale_hud.h"

void setUp(void) {}
void tearDown(void) {}

/* Always-visible items: 1-3 */

void test_gregorian_always_visible(void)
{
    for (int s = 0; s <= 6; s++) {
        hud_visibility_t v = hud_visibility_at_scale(s);
        TEST_ASSERT_TRUE(v.show_gregorian);
    }
}

void test_speed_always_visible(void)
{
    for (int s = 0; s <= 6; s++) {
        hud_visibility_t v = hud_visibility_at_scale(s);
        TEST_ASSERT_TRUE(v.show_speed);
    }
}

void test_scale_name_always_visible(void)
{
    for (int s = 0; s <= 6; s++) {
        hud_visibility_t v = hud_visibility_at_scale(s);
        TEST_ASSERT_TRUE(v.show_scale_name);
    }
}

/* Personal scale (0): 4-6 */

void test_personal_shows_astrology(void)
{
    hud_visibility_t v = hud_visibility_at_scale(0);
    TEST_ASSERT_TRUE(v.show_astrology);
    TEST_ASSERT_TRUE(v.show_hd);
    TEST_ASSERT_TRUE(v.show_tzolkin);
    TEST_ASSERT_TRUE(v.show_iching);
}

void test_personal_shows_houses(void)
{
    hud_visibility_t v = hud_visibility_at_scale(0);
    TEST_ASSERT_TRUE(v.show_houses);
    TEST_ASSERT_TRUE(v.show_aspects);
}

void test_personal_hides_cosmic(void)
{
    hud_visibility_t v = hud_visibility_at_scale(0);
    TEST_ASSERT_FALSE(v.show_cosmic);
}

/* Earth scale (1): 7 */

void test_earth_shows_chinese(void)
{
    hud_visibility_t v = hud_visibility_at_scale(1);
    TEST_ASSERT_TRUE(v.show_chinese);
    TEST_ASSERT_TRUE(v.show_sun_sign);
}

/* Inner Solar scale (2): 8 */

void test_inner_shows_sun_sign(void)
{
    hud_visibility_t v = hud_visibility_at_scale(2);
    TEST_ASSERT_TRUE(v.show_sun_sign);
    TEST_ASSERT_TRUE(v.show_aspects);
}

/* Solar scale (3): 9-10 */

void test_solar_shows_core_info(void)
{
    hud_visibility_t v = hud_visibility_at_scale(3);
    TEST_ASSERT_TRUE(v.show_jd);
    TEST_ASSERT_TRUE(v.show_sidereal);
    TEST_ASSERT_TRUE(v.show_sun_sign);
}

void test_solar_hides_cosmic(void)
{
    hud_visibility_t v = hud_visibility_at_scale(3);
    TEST_ASSERT_FALSE(v.show_cosmic);
}

/* Galaxy scale (5): 11-12 */

void test_galaxy_shows_cosmic(void)
{
    hud_visibility_t v = hud_visibility_at_scale(5);
    TEST_ASSERT_TRUE(v.show_cosmic);
}

void test_galaxy_hides_personal(void)
{
    hud_visibility_t v = hud_visibility_at_scale(5);
    TEST_ASSERT_FALSE(v.show_hd);
    TEST_ASSERT_FALSE(v.show_iching);
    TEST_ASSERT_FALSE(v.show_houses);
}

/* Universe scale (6): 13 */

void test_universe_shows_cosmic(void)
{
    hud_visibility_t v = hud_visibility_at_scale(6);
    TEST_ASSERT_TRUE(v.show_cosmic);
    TEST_ASSERT_FALSE(v.show_tzolkin);
}

/* Visible count: 14-15 */

void test_visible_count_personal(void)
{
    hud_visibility_t v = hud_visibility_at_scale(0);
    int count = hud_visible_count(v);
    /* Personal shows many items */
    TEST_ASSERT_TRUE(count >= 10);
}

void test_visible_count_universe(void)
{
    hud_visibility_t v = hud_visibility_at_scale(6);
    int count = hud_visible_count(v);
    /* Universe shows fewer items */
    TEST_ASSERT_TRUE(count >= 4);
    TEST_ASSERT_TRUE(count < 14);
}

/* Mode names: 16-17 */

void test_mode_names(void)
{
    TEST_ASSERT_NOT_NULL(hud_mode_name(0));
    TEST_ASSERT_NOT_NULL(hud_mode_name(3));
    TEST_ASSERT_NOT_NULL(hud_mode_name(6));
}

void test_mode_name_invalid(void)
{
    /* Invalid scale returns some reasonable default */
    const char *name = hud_mode_name(-1);
    TEST_ASSERT_NOT_NULL(name);
    const char *name2 = hud_mode_name(99);
    TEST_ASSERT_NOT_NULL(name2);
}

/* Merge: 18-19 */

void test_merge_combines(void)
{
    hud_visibility_t a = hud_visibility_at_scale(0); /* personal: has astrology */
    hud_visibility_t b = hud_visibility_at_scale(5); /* galaxy: has cosmic */
    hud_visibility_t m = hud_visibility_merge(a, b);
    TEST_ASSERT_TRUE(m.show_astrology);
    TEST_ASSERT_TRUE(m.show_cosmic);
}

void test_merge_count_gte_either(void)
{
    hud_visibility_t a = hud_visibility_at_scale(0);
    hud_visibility_t b = hud_visibility_at_scale(5);
    hud_visibility_t m = hud_visibility_merge(a, b);
    TEST_ASSERT_TRUE(hud_visible_count(m) >= hud_visible_count(a));
    TEST_ASSERT_TRUE(hud_visible_count(m) >= hud_visible_count(b));
}

/* Default: 20 */

void test_default_is_solar(void)
{
    hud_visibility_t def = hud_visibility_default();
    hud_visibility_t solar = hud_visibility_at_scale(3);
    TEST_ASSERT_EQUAL_INT(def.show_gregorian, solar.show_gregorian);
    TEST_ASSERT_EQUAL_INT(def.show_cosmic, solar.show_cosmic);
    TEST_ASSERT_EQUAL_INT(def.show_jd, solar.show_jd);
    TEST_ASSERT_EQUAL_INT(def.show_sidereal, solar.show_sidereal);
}

/* Invalid scale: 21 */

void test_invalid_scale_returns_default(void)
{
    hud_visibility_t v = hud_visibility_at_scale(-1);
    hud_visibility_t def = hud_visibility_default();
    TEST_ASSERT_EQUAL_INT(def.show_gregorian, v.show_gregorian);
    TEST_ASSERT_EQUAL_INT(def.show_cosmic, v.show_cosmic);
}

/* Purity: 22 */

void test_purity(void)
{
    hud_visibility_t a = hud_visibility_at_scale(2);
    hud_visibility_t b = hud_visibility_at_scale(2);
    TEST_ASSERT_EQUAL_INT(a.show_gregorian, b.show_gregorian);
    TEST_ASSERT_EQUAL_INT(a.show_astrology, b.show_astrology);
    TEST_ASSERT_EQUAL_INT(a.show_cosmic, b.show_cosmic);
    TEST_ASSERT_EQUAL_INT(hud_visible_count(a), hud_visible_count(b));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_gregorian_always_visible);
    RUN_TEST(test_speed_always_visible);
    RUN_TEST(test_scale_name_always_visible);
    RUN_TEST(test_personal_shows_astrology);
    RUN_TEST(test_personal_shows_houses);
    RUN_TEST(test_personal_hides_cosmic);
    RUN_TEST(test_earth_shows_chinese);
    RUN_TEST(test_inner_shows_sun_sign);
    RUN_TEST(test_solar_shows_core_info);
    RUN_TEST(test_solar_hides_cosmic);
    RUN_TEST(test_galaxy_shows_cosmic);
    RUN_TEST(test_galaxy_hides_personal);
    RUN_TEST(test_universe_shows_cosmic);
    RUN_TEST(test_visible_count_personal);
    RUN_TEST(test_visible_count_universe);
    RUN_TEST(test_mode_names);
    RUN_TEST(test_mode_name_invalid);
    RUN_TEST(test_merge_combines);
    RUN_TEST(test_merge_count_gte_either);
    RUN_TEST(test_default_is_solar);
    RUN_TEST(test_invalid_scale_returns_default);
    RUN_TEST(test_purity);
    return UNITY_END();
}
