#include "../unity/unity.h"
#include "../../src/ui/kin_card_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- kin_card_compute: identity --- */

void test_kin1_title(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_EQUAL_STRING("Kin 1", c.title);
}

void test_kin1_seal_name_contains_dragon(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_NOT_NULL(strstr(c.seal_name, "Dragon"));
}

void test_kin1_tone_name_magnetic(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_EQUAL_STRING("Magnetic", c.tone_name);
}

void test_kin1_identity(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.kin);
    TEST_ASSERT_EQUAL_INT(0, c.seal);   /* Dragon */
    TEST_ASSERT_EQUAL_INT(1, c.tone);   /* Magnetic */
    TEST_ASSERT_EQUAL_INT(0, c.color);  /* Red */
}

void test_kin131_seal_name_contains_monkey(void)
{
    kin_card_layout_t c = kin_card_compute(131);
    TEST_ASSERT_NOT_NULL(strstr(c.seal_name, "Monkey"));
}

void test_kin131_color_blue(void)
{
    kin_card_layout_t c = kin_card_compute(131);
    TEST_ASSERT_EQUAL_INT(2, c.color);  /* Blue (seal 10, 10%4=2) */
}

void test_kin260_seal_name_contains_sun(void)
{
    kin_card_layout_t c = kin_card_compute(260);
    TEST_ASSERT_NOT_NULL(strstr(c.seal_name, "Sun"));
}

void test_kin260_tone_name_cosmic(void)
{
    kin_card_layout_t c = kin_card_compute(260);
    TEST_ASSERT_EQUAL_STRING("Cosmic", c.tone_name);
}

/* --- Oracle kins --- */

void test_kin1_oracle_guide_valid(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_GREATER_OR_EQUAL(1, c.guide_kin);
    TEST_ASSERT_LESS_OR_EQUAL(260, c.guide_kin);
}

void test_kin1_oracle_analog_valid(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_GREATER_OR_EQUAL(1, c.analog_kin);
    TEST_ASSERT_LESS_OR_EQUAL(260, c.analog_kin);
}

void test_kin1_oracle_antipode_valid(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_GREATER_OR_EQUAL(1, c.antipode_kin);
    TEST_ASSERT_LESS_OR_EQUAL(260, c.antipode_kin);
}

void test_kin1_oracle_occult_valid(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_GREATER_OR_EQUAL(1, c.occult_kin);
    TEST_ASSERT_LESS_OR_EQUAL(260, c.occult_kin);
}

void test_kin1_oracle_occult_is_260(void)
{
    /* occult = 261 - kin = 260 */
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_EQUAL_INT(260, c.occult_kin);
}

/* --- Wavespell and castle --- */

void test_kin1_wavespell_info_not_empty(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_TRUE(strlen(c.wavespell_info) > 0);
}

void test_kin1_wavespell_info_format(void)
{
    /* Kin 1: WS 1 / Pos 1 */
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_EQUAL_STRING("WS 1 / Pos 1", c.wavespell_info);
}

void test_kin131_wavespell_info(void)
{
    /* Kin 131: WS 11 / Pos 1 */
    kin_card_layout_t c = kin_card_compute(131);
    TEST_ASSERT_EQUAL_STRING("WS 11 / Pos 1", c.wavespell_info);
}

void test_kin1_castle_name_not_empty(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_TRUE(strlen(c.castle_name) > 0);
}

void test_kin1_castle_name_red(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_EQUAL_STRING("Red Castle of Turning", c.castle_name);
}

void test_kin131_castle_name_blue(void)
{
    kin_card_layout_t c = kin_card_compute(131);
    TEST_ASSERT_EQUAL_STRING("Blue Castle of Burning", c.castle_name);
}

/* --- Tone dots --- */

void test_tone_dots_equals_tone(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.tone_dots);  /* tone 1 = 1 dot */
}

void test_tone_dots_13_for_cosmic(void)
{
    kin_card_layout_t c = kin_card_compute(260);
    TEST_ASSERT_EQUAL_INT(13, c.tone_dots);  /* tone 13 = 13 dots */
}

/* --- Portal flag --- */

void test_kin1_is_portal(void)
{
    /* Kin 1 is GAP: col=0, row=0, d=0, row==d → true */
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.is_portal);
}

void test_kin2_not_portal(void)
{
    /* Kin 2: col=0, row=1, d=0 → row!=0 && row!=19 → false */
    kin_card_layout_t c = kin_card_compute(2);
    TEST_ASSERT_EQUAL_INT(0, c.is_portal);
}

void test_kin131_not_portal(void)
{
    /* Kin 131: col=6, d=6 → false */
    kin_card_layout_t c = kin_card_compute(131);
    TEST_ASSERT_EQUAL_INT(0, c.is_portal);
}

/* --- Power day flag --- */

void test_kin1_is_power_day(void)
{
    /* Kin 1: wavespell position 1 → power day */
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.is_power_day);
}

void test_kin2_not_power_day(void)
{
    /* Kin 2: wavespell position 2 → not power day */
    kin_card_layout_t c = kin_card_compute(2);
    TEST_ASSERT_EQUAL_INT(0, c.is_power_day);
}

void test_kin5_is_power_day(void)
{
    /* Kin 5: wavespell position 5 → power day */
    kin_card_layout_t c = kin_card_compute(5);
    TEST_ASSERT_EQUAL_INT(1, c.is_power_day);
}

void test_kin13_is_power_day(void)
{
    /* Kin 13: wavespell position 13 → power day */
    kin_card_layout_t c = kin_card_compute(13);
    TEST_ASSERT_EQUAL_INT(1, c.is_power_day);
}

/* --- kin_card_full_name --- */

void test_full_name_seal_0_red_dragon(void)
{
    char buf[KIN_CARD_MAX_TEXT];
    const char *result = kin_card_full_name(0, buf, KIN_CARD_MAX_TEXT);
    TEST_ASSERT_EQUAL_STRING("Red Dragon", result);
}

void test_full_name_seal_10_blue_monkey(void)
{
    char buf[KIN_CARD_MAX_TEXT];
    const char *result = kin_card_full_name(10, buf, KIN_CARD_MAX_TEXT);
    TEST_ASSERT_NOT_NULL(strstr(result, "Monkey"));
}

void test_full_name_invalid_negative(void)
{
    char buf[KIN_CARD_MAX_TEXT];
    const char *result = kin_card_full_name(-1, buf, KIN_CARD_MAX_TEXT);
    TEST_ASSERT_EQUAL_STRING("", result);
}

void test_full_name_invalid_20(void)
{
    char buf[KIN_CARD_MAX_TEXT];
    const char *result = kin_card_full_name(20, buf, KIN_CARD_MAX_TEXT);
    TEST_ASSERT_EQUAL_STRING("", result);
}

/* --- Invalid kin → zeroed struct --- */

void test_kin0_returns_zeroed(void)
{
    kin_card_layout_t c = kin_card_compute(0);
    TEST_ASSERT_EQUAL_INT(0, c.kin);
    TEST_ASSERT_EQUAL_INT(0, c.seal);
    TEST_ASSERT_EQUAL_INT(0, c.tone);
    TEST_ASSERT_EQUAL_INT(0, c.guide_kin);
    TEST_ASSERT_EQUAL_STRING("", c.title);
}

void test_kin261_returns_zeroed(void)
{
    kin_card_layout_t c = kin_card_compute(261);
    TEST_ASSERT_EQUAL_INT(0, c.kin);
    TEST_ASSERT_EQUAL_INT(0, c.seal);
    TEST_ASSERT_EQUAL_INT(0, c.tone);
    TEST_ASSERT_EQUAL_INT(0, c.guide_kin);
}

/* --- Color matches seal % 4 --- */

void test_color_matches_seal_mod_4(void)
{
    /* Spot-check several kins */
    for (int k = 1; k <= 260; k += 37) {
        kin_card_layout_t c = kin_card_compute(k);
        TEST_ASSERT_EQUAL_INT(c.seal % 4, c.color);
    }
}

/* --- Oracle summary contains "Guide:" --- */

void test_oracle_summary_format(void)
{
    kin_card_layout_t c = kin_card_compute(1);
    TEST_ASSERT_NOT_NULL(strstr(c.oracle_summary, "Guide:"));
}

/* --- Purity: same input → same output --- */

void test_purity_deterministic(void)
{
    kin_card_layout_t a = kin_card_compute(42);
    kin_card_layout_t b = kin_card_compute(42);
    TEST_ASSERT_EQUAL_INT(a.kin, b.kin);
    TEST_ASSERT_EQUAL_INT(a.guide_kin, b.guide_kin);
    TEST_ASSERT_EQUAL_INT(a.is_portal, b.is_portal);
    TEST_ASSERT_EQUAL_STRING(a.seal_name, b.seal_name);
}

int main(void)
{
    UNITY_BEGIN();
    /* identity */
    RUN_TEST(test_kin1_title);
    RUN_TEST(test_kin1_seal_name_contains_dragon);
    RUN_TEST(test_kin1_tone_name_magnetic);
    RUN_TEST(test_kin1_identity);
    RUN_TEST(test_kin131_seal_name_contains_monkey);
    RUN_TEST(test_kin131_color_blue);
    RUN_TEST(test_kin260_seal_name_contains_sun);
    RUN_TEST(test_kin260_tone_name_cosmic);
    /* oracle */
    RUN_TEST(test_kin1_oracle_guide_valid);
    RUN_TEST(test_kin1_oracle_analog_valid);
    RUN_TEST(test_kin1_oracle_antipode_valid);
    RUN_TEST(test_kin1_oracle_occult_valid);
    RUN_TEST(test_kin1_oracle_occult_is_260);
    /* wavespell / castle */
    RUN_TEST(test_kin1_wavespell_info_not_empty);
    RUN_TEST(test_kin1_wavespell_info_format);
    RUN_TEST(test_kin131_wavespell_info);
    RUN_TEST(test_kin1_castle_name_not_empty);
    RUN_TEST(test_kin1_castle_name_red);
    RUN_TEST(test_kin131_castle_name_blue);
    /* tone dots */
    RUN_TEST(test_tone_dots_equals_tone);
    RUN_TEST(test_tone_dots_13_for_cosmic);
    /* portal */
    RUN_TEST(test_kin1_is_portal);
    RUN_TEST(test_kin2_not_portal);
    RUN_TEST(test_kin131_not_portal);
    /* power day */
    RUN_TEST(test_kin1_is_power_day);
    RUN_TEST(test_kin2_not_power_day);
    RUN_TEST(test_kin5_is_power_day);
    RUN_TEST(test_kin13_is_power_day);
    /* full name */
    RUN_TEST(test_full_name_seal_0_red_dragon);
    RUN_TEST(test_full_name_seal_10_blue_monkey);
    RUN_TEST(test_full_name_invalid_negative);
    RUN_TEST(test_full_name_invalid_20);
    /* invalid kin */
    RUN_TEST(test_kin0_returns_zeroed);
    RUN_TEST(test_kin261_returns_zeroed);
    /* color */
    RUN_TEST(test_color_matches_seal_mod_4);
    /* oracle summary format */
    RUN_TEST(test_oracle_summary_format);
    /* purity */
    RUN_TEST(test_purity_deterministic);
    return UNITY_END();
}
