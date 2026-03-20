/* test_card_style.c — Tests for per-system card visual styling.
 *
 * Verifies that card_style maps each knowledge system to a distinct
 * visual style derived from the theme and color_theory palettes. */

#include "../../tests/unity/unity.h"
#include "../../src/ui/card_style.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Mapping tests --- */

void test_mapping_gregorian(void) {
    TEST_ASSERT_EQUAL(CT_SYSTEM_GREGORIAN,
                      card_style_to_ct_system(TS_SYS_GREGORIAN));
}

void test_mapping_tzolkin(void) {
    TEST_ASSERT_EQUAL(CT_SYSTEM_TZOLKIN,
                      card_style_to_ct_system(TS_SYS_TZOLKIN));
}

void test_mapping_haab_to_tzolkin(void) {
    /* Haab is part of the Mayan calendar system, shares Tzolkin palette */
    TEST_ASSERT_EQUAL(CT_SYSTEM_TZOLKIN,
                      card_style_to_ct_system(TS_SYS_HAAB));
}

void test_mapping_cosmic_to_astronomy(void) {
    TEST_ASSERT_EQUAL(CT_SYSTEM_ASTRONOMY,
                      card_style_to_ct_system(TS_SYS_COSMIC));
}

void test_mapping_all_direct(void) {
    TEST_ASSERT_EQUAL(CT_SYSTEM_CHINESE,      card_style_to_ct_system(TS_SYS_CHINESE));
    TEST_ASSERT_EQUAL(CT_SYSTEM_HEBREW,       card_style_to_ct_system(TS_SYS_HEBREW));
    TEST_ASSERT_EQUAL(CT_SYSTEM_ISLAMIC,      card_style_to_ct_system(TS_SYS_ISLAMIC));
    TEST_ASSERT_EQUAL(CT_SYSTEM_BUDDHIST,     card_style_to_ct_system(TS_SYS_BUDDHIST));
    TEST_ASSERT_EQUAL(CT_SYSTEM_HINDU,        card_style_to_ct_system(TS_SYS_HINDU));
    TEST_ASSERT_EQUAL(CT_SYSTEM_ICHING,       card_style_to_ct_system(TS_SYS_ICHING));
    TEST_ASSERT_EQUAL(CT_SYSTEM_ASTROLOGY,    card_style_to_ct_system(TS_SYS_ASTROLOGY));
    TEST_ASSERT_EQUAL(CT_SYSTEM_HUMAN_DESIGN, card_style_to_ct_system(TS_SYS_HUMAN_DESIGN));
    TEST_ASSERT_EQUAL(CT_SYSTEM_KABBALAH,     card_style_to_ct_system(TS_SYS_KABBALAH));
    TEST_ASSERT_EQUAL(CT_SYSTEM_GEOLOGY,      card_style_to_ct_system(TS_SYS_GEOLOGICAL));
}

void test_mapping_unmapped_returns_negative(void) {
    TEST_ASSERT_EQUAL(-1, card_style_to_ct_system(TS_SYS_COPTIC));
    TEST_ASSERT_EQUAL(-1, card_style_to_ct_system(TS_SYS_ETHIOPIAN));
    TEST_ASSERT_EQUAL(-1, card_style_to_ct_system(TS_SYS_PERSIAN));
    TEST_ASSERT_EQUAL(-1, card_style_to_ct_system(TS_SYS_JAPANESE));
    TEST_ASSERT_EQUAL(-1, card_style_to_ct_system(TS_SYS_KOREAN));
    TEST_ASSERT_EQUAL(-1, card_style_to_ct_system(TS_SYS_THAI));
}

void test_mapping_invalid_ids(void) {
    TEST_ASSERT_EQUAL(-1, card_style_to_ct_system(-1));
    TEST_ASSERT_EQUAL(-1, card_style_to_ct_system(99));
    TEST_ASSERT_EQUAL(-1, card_style_to_ct_system(TS_SYS_COUNT));
}

/* --- Style: theme variation --- */

void test_cosmos_bg_dark(void) {
    card_style_t s = card_style_for_system(TS_SYS_TZOLKIN, 1.0f, THEME_COSMOS);
    float lum = 0.2126f * s.background.r + 0.7152f * s.background.g
              + 0.0722f * s.background.b;
    TEST_ASSERT_LESS_THAN_FLOAT(0.3f, lum);
}

void test_dawn_bg_lighter_than_cosmos(void) {
    card_style_t dawn   = card_style_for_system(TS_SYS_TZOLKIN, 1.0f, THEME_DAWN);
    card_style_t cosmos = card_style_for_system(TS_SYS_TZOLKIN, 1.0f, THEME_COSMOS);
    float lum_d = 0.2126f * dawn.background.r   + 0.7152f * dawn.background.g
                + 0.0722f * dawn.background.b;
    float lum_c = 0.2126f * cosmos.background.r  + 0.7152f * cosmos.background.g
                + 0.0722f * cosmos.background.b;
    TEST_ASSERT_GREATER_THAN_FLOAT(lum_c, lum_d);
}

/* --- Style: opacity --- */

void test_opacity_scales_background(void) {
    card_style_t full = card_style_for_system(TS_SYS_ASTROLOGY, 1.0f, THEME_COSMOS);
    card_style_t half = card_style_for_system(TS_SYS_ASTROLOGY, 0.5f, THEME_COSMOS);
    TEST_ASSERT_LESS_THAN_FLOAT(full.background.a, half.background.a);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, half.background.a);
}

void test_opacity_scales_border(void) {
    card_style_t full = card_style_for_system(TS_SYS_ICHING, 1.0f, THEME_COSMOS);
    card_style_t half = card_style_for_system(TS_SYS_ICHING, 0.5f, THEME_COSMOS);
    TEST_ASSERT_LESS_THAN_FLOAT(full.border.a, half.border.a);
}

void test_zero_opacity_transparent(void) {
    card_style_t s = card_style_for_system(TS_SYS_GREGORIAN, 0.0f, THEME_COSMOS);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.background.a);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, s.border.a);
}

/* --- Style: system distinction --- */

void test_systems_have_distinct_titles(void) {
    card_style_t tz = card_style_for_system(TS_SYS_TZOLKIN, 1.0f, THEME_COSMOS);
    card_style_t as = card_style_for_system(TS_SYS_ASTROLOGY, 1.0f, THEME_COSMOS);
    float dr = tz.title.r - as.title.r;
    float dg = tz.title.g - as.title.g;
    float db = tz.title.b - as.title.b;
    float dist_sq = dr * dr + dg * dg + db * db;
    TEST_ASSERT_GREATER_THAN_FLOAT(0.01f, dist_sq);
}

void test_unmapped_uses_brand_primary(void) {
    card_style_t s = card_style_for_system(TS_SYS_COPTIC, 1.0f, THEME_COSMOS);
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, t.brand_primary.r, s.title.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, t.brand_primary.g, s.title.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, t.brand_primary.b, s.title.b);
}

/* --- Style: text colors from theme --- */

void test_body_from_theme(void) {
    card_style_t s = card_style_for_system(TS_SYS_CHINESE, 1.0f, THEME_COSMOS);
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, t.text_primary.r, s.body.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, t.text_primary.g, s.body.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, t.text_primary.b, s.body.b);
}

void test_muted_from_theme(void) {
    card_style_t s = card_style_for_system(TS_SYS_HINDU, 1.0f, THEME_DAWN);
    theme_t t = theme_get(THEME_DAWN);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, t.text_secondary.r, s.muted.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, t.text_secondary.g, s.muted.g);
}

/* --- Style: border subtlety --- */

void test_border_alpha_less_than_title(void) {
    card_style_t s = card_style_for_system(TS_SYS_ASTROLOGY, 1.0f, THEME_COSMOS);
    TEST_ASSERT_LESS_THAN_FLOAT(s.title.a, s.border.a);
}

/* --- Style: default (no system) --- */

void test_default_style(void) {
    card_style_t s = card_style_default(1.0f, THEME_COSMOS);
    theme_t t = theme_get(THEME_COSMOS);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, t.brand_primary.r, s.title.r);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, s.background.a);
    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, s.border.a);
}

/* --- Comprehensive: all 20 systems produce valid output --- */

void test_all_twenty_systems_valid(void) {
    for (int i = 0; i < TS_SYS_COUNT; i++) {
        card_style_t s = card_style_for_system(i, 0.8f, THEME_COSMOS);
        TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, s.background.a);
        TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, s.title.a);
        TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, s.body.a);
        TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, s.muted.a);
    }
}

int main(void) {
    UNITY_BEGIN();

    /* Mapping */
    RUN_TEST(test_mapping_gregorian);
    RUN_TEST(test_mapping_tzolkin);
    RUN_TEST(test_mapping_haab_to_tzolkin);
    RUN_TEST(test_mapping_cosmic_to_astronomy);
    RUN_TEST(test_mapping_all_direct);
    RUN_TEST(test_mapping_unmapped_returns_negative);
    RUN_TEST(test_mapping_invalid_ids);

    /* Theme variation */
    RUN_TEST(test_cosmos_bg_dark);
    RUN_TEST(test_dawn_bg_lighter_than_cosmos);

    /* Opacity */
    RUN_TEST(test_opacity_scales_background);
    RUN_TEST(test_opacity_scales_border);
    RUN_TEST(test_zero_opacity_transparent);

    /* System distinction */
    RUN_TEST(test_systems_have_distinct_titles);
    RUN_TEST(test_unmapped_uses_brand_primary);

    /* Text from theme */
    RUN_TEST(test_body_from_theme);
    RUN_TEST(test_muted_from_theme);

    /* Border */
    RUN_TEST(test_border_alpha_less_than_title);

    /* Default */
    RUN_TEST(test_default_style);

    /* Comprehensive */
    RUN_TEST(test_all_twenty_systems_valid);

    return UNITY_END();
}
