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

void test_mapping_world_calendars_mapped(void) {
    TEST_ASSERT_EQUAL(CT_SYSTEM_COPTIC,    card_style_to_ct_system(TS_SYS_COPTIC));
    TEST_ASSERT_EQUAL(CT_SYSTEM_ETHIOPIAN, card_style_to_ct_system(TS_SYS_ETHIOPIAN));
    TEST_ASSERT_EQUAL(CT_SYSTEM_PERSIAN,   card_style_to_ct_system(TS_SYS_PERSIAN));
    TEST_ASSERT_EQUAL(CT_SYSTEM_JAPANESE,  card_style_to_ct_system(TS_SYS_JAPANESE));
    TEST_ASSERT_EQUAL(CT_SYSTEM_KOREAN,    card_style_to_ct_system(TS_SYS_KOREAN));
    TEST_ASSERT_EQUAL(CT_SYSTEM_THAI,      card_style_to_ct_system(TS_SYS_THAI));
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

void test_dawn_bg_same_system_color_as_cosmos(void) {
    /* Principle 1: card backgrounds use system accent color (cosmos-scope),
     * not theme surface. Both themes produce the same card background
     * for the same system — the system identity IS the data. */
    card_style_t dawn   = card_style_for_system(TS_SYS_TZOLKIN, 1.0f, THEME_DAWN);
    card_style_t cosmos = card_style_for_system(TS_SYS_TZOLKIN, 1.0f, THEME_COSMOS);
    /* Same system = same background color (system accent) */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, cosmos.background.r, dawn.background.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, cosmos.background.g, dawn.background.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, cosmos.background.b, dawn.background.b);
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

void test_coptic_has_unique_accent(void) {
    card_style_t s = card_style_for_system(TS_SYS_COPTIC, 1.0f, THEME_COSMOS);
    /* Coptic accent is liturgical red — distinct from brand_primary solar gold */
    theme_t t = theme_get(THEME_COSMOS);
    float dr = s.title.r - t.brand_primary.r;
    float dg = s.title.g - t.brand_primary.g;
    float db = s.title.b - t.brand_primary.b;
    float dist_sq = dr * dr + dg * dg + db * db;
    TEST_ASSERT_GREATER_THAN_FLOAT(0.01f, dist_sq);
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

/* --- Focus mode mapping --- */

void test_focus_astrology(void) {
    TEST_ASSERT_EQUAL(TS_SYS_ASTROLOGY, card_style_focus_system(1));
}

void test_focus_kin(void) {
    TEST_ASSERT_EQUAL(TS_SYS_TZOLKIN, card_style_focus_system(2));
}

void test_focus_iching(void) {
    TEST_ASSERT_EQUAL(TS_SYS_ICHING, card_style_focus_system(3));
}

void test_focus_chinese(void) {
    TEST_ASSERT_EQUAL(TS_SYS_CHINESE, card_style_focus_system(4));
}

void test_focus_hd(void) {
    TEST_ASSERT_EQUAL(TS_SYS_HUMAN_DESIGN, card_style_focus_system(5));
}

void test_focus_overview_negative(void) {
    TEST_ASSERT_EQUAL(-1, card_style_focus_system(0));
}

void test_focus_invalid_negative(void) {
    TEST_ASSERT_EQUAL(-1, card_style_focus_system(99));
    TEST_ASSERT_EQUAL(-1, card_style_focus_system(-1));
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
    RUN_TEST(test_mapping_world_calendars_mapped);
    RUN_TEST(test_mapping_invalid_ids);

    /* Theme variation */
    RUN_TEST(test_cosmos_bg_dark);
    RUN_TEST(test_dawn_bg_same_system_color_as_cosmos);

    /* Opacity */
    RUN_TEST(test_opacity_scales_background);
    RUN_TEST(test_opacity_scales_border);
    RUN_TEST(test_zero_opacity_transparent);

    /* System distinction */
    RUN_TEST(test_systems_have_distinct_titles);
    RUN_TEST(test_coptic_has_unique_accent);

    /* Text from theme */
    RUN_TEST(test_body_from_theme);
    RUN_TEST(test_muted_from_theme);

    /* Border */
    RUN_TEST(test_border_alpha_less_than_title);

    /* Default */
    RUN_TEST(test_default_style);

    /* Focus mode */
    RUN_TEST(test_focus_astrology);
    RUN_TEST(test_focus_kin);
    RUN_TEST(test_focus_iching);
    RUN_TEST(test_focus_chinese);
    RUN_TEST(test_focus_hd);
    RUN_TEST(test_focus_overview_negative);
    RUN_TEST(test_focus_invalid_negative);

    /* Comprehensive */
    RUN_TEST(test_all_twenty_systems_valid);

    return UNITY_END();
}
