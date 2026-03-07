#include "../unity/unity.h"
#include "../../src/ui/rtl_layout.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- rtl_is_rtl_codepoint: tests 1-12 ---- */

void test_is_rtl_arabic_basic(void)
{
    /* U+0627 ARABIC LETTER ALEF */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x0627));
}

void test_is_rtl_arabic_start(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x0600));
}

void test_is_rtl_arabic_end(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x06FF));
}

void test_is_rtl_arabic_supplement(void)
{
    /* U+0750 Arabic Supplement */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x0750));
}

void test_is_rtl_arabic_extended_a(void)
{
    /* U+08A0 Arabic Extended-A */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x08A0));
}

void test_is_rtl_arabic_pres_forms_a(void)
{
    /* U+FB50 Arabic Presentation Forms-A */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0xFB50));
}

void test_is_rtl_arabic_pres_forms_b(void)
{
    /* U+FE70 Arabic Presentation Forms-B */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0xFE70));
}

void test_is_rtl_hebrew_basic(void)
{
    /* U+05D0 HEBREW LETTER ALEF */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x05D0));
}

void test_is_rtl_hebrew_start(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x0590));
}

void test_is_rtl_hebrew_end(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x05FF));
}

void test_is_rtl_hebrew_pres_forms(void)
{
    /* U+FB1D Hebrew presentation forms */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0xFB1D));
}

void test_is_rtl_hebrew_pres_forms_end(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0xFB4F));
}

/* ---- rtl_is_rtl_codepoint: Syriac, Thaana, RTL marks ---- */

void test_is_rtl_syriac(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x0710));
}

void test_is_rtl_syriac_start(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x0700));
}

void test_is_rtl_syriac_end(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x074F));
}

void test_is_rtl_thaana(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x0780));
}

void test_is_rtl_thaana_end(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x07BF));
}

void test_is_rtl_rlm_mark(void)
{
    /* U+200F RIGHT-TO-LEFT MARK */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x200F));
}

void test_is_rtl_rle_mark(void)
{
    /* U+202B RIGHT-TO-LEFT EMBEDDING */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x202B));
}

void test_is_rtl_rlo_mark(void)
{
    /* U+202E RIGHT-TO-LEFT OVERRIDE */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x202E));
}

/* ---- rtl_is_rtl_codepoint: LTR / false cases ---- */

void test_is_not_rtl_latin(void)
{
    TEST_ASSERT_FALSE(rtl_is_rtl_codepoint('A'));
}

void test_is_not_rtl_digit(void)
{
    TEST_ASSERT_FALSE(rtl_is_rtl_codepoint('0'));
}

void test_is_not_rtl_devanagari(void)
{
    /* U+0905 Devanagari A */
    TEST_ASSERT_FALSE(rtl_is_rtl_codepoint(0x0905));
}

void test_is_not_rtl_cjk(void)
{
    /* U+4E00 CJK basic */
    TEST_ASSERT_FALSE(rtl_is_rtl_codepoint(0x4E00));
}

void test_is_not_rtl_null(void)
{
    TEST_ASSERT_FALSE(rtl_is_rtl_codepoint(0));
}

/* ---- rtl_codepoint_direction: tests 26-32 ---- */

void test_direction_arabic_is_rtl(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_RTL, rtl_codepoint_direction(0x0627));
}

void test_direction_hebrew_is_rtl(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_RTL, rtl_codepoint_direction(0x05D0));
}

void test_direction_latin_is_ltr(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_LTR, rtl_codepoint_direction('A'));
}

void test_direction_devanagari_is_ltr(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_LTR, rtl_codepoint_direction(0x0905));
}

void test_direction_digit_is_neutral(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_NEUTRAL, rtl_codepoint_direction('5'));
}

void test_direction_space_is_neutral(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_NEUTRAL, rtl_codepoint_direction(' '));
}

void test_direction_punctuation_is_neutral(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_NEUTRAL, rtl_codepoint_direction('.'));
}

/* ---- rtl_identify_script: tests 33-44 ---- */

void test_script_latin(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_LATIN, rtl_identify_script('A'));
}

void test_script_latin_lowercase(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_LATIN, rtl_identify_script('z'));
}

void test_script_arabic(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_ARABIC, rtl_identify_script(0x0627));
}

void test_script_arabic_supplement(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_ARABIC, rtl_identify_script(0x0760));
}

void test_script_hebrew(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_HEBREW, rtl_identify_script(0x05D0));
}

void test_script_hebrew_presentation(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_HEBREW, rtl_identify_script(0xFB1D));
}

void test_script_syriac(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_SYRIAC, rtl_identify_script(0x0710));
}

void test_script_thaana(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_THAANA, rtl_identify_script(0x0780));
}

void test_script_devanagari(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_DEVANAGARI, rtl_identify_script(0x0905));
}

void test_script_cjk(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_CJK, rtl_identify_script(0x4E00));
}

void test_script_thai(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_THAI, rtl_identify_script(0x0E01));
}

void test_script_geez(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_GEEZ, rtl_identify_script(0x1200));
}

void test_script_unknown_digit(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_UNKNOWN, rtl_identify_script('5'));
}

/* ---- rtl_is_rtl_script: tests 46-52 ---- */

void test_is_rtl_script_arabic(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_script(RTL_SCRIPT_ARABIC));
}

void test_is_rtl_script_hebrew(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_script(RTL_SCRIPT_HEBREW));
}

void test_is_rtl_script_persian(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_script(RTL_SCRIPT_PERSIAN));
}

void test_is_rtl_script_urdu(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_script(RTL_SCRIPT_URDU));
}

void test_is_rtl_script_syriac(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_script(RTL_SCRIPT_SYRIAC));
}

void test_is_rtl_script_thaana(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_script(RTL_SCRIPT_THAANA));
}

void test_is_not_rtl_script_latin(void)
{
    TEST_ASSERT_FALSE(rtl_is_rtl_script(RTL_SCRIPT_LATIN));
}

void test_is_not_rtl_script_devanagari(void)
{
    TEST_ASSERT_FALSE(rtl_is_rtl_script(RTL_SCRIPT_DEVANAGARI));
}

void test_is_not_rtl_script_cjk(void)
{
    TEST_ASSERT_FALSE(rtl_is_rtl_script(RTL_SCRIPT_CJK));
}

void test_is_not_rtl_script_thai(void)
{
    TEST_ASSERT_FALSE(rtl_is_rtl_script(RTL_SCRIPT_THAI));
}

void test_is_not_rtl_script_geez(void)
{
    TEST_ASSERT_FALSE(rtl_is_rtl_script(RTL_SCRIPT_GEEZ));
}

void test_is_not_rtl_script_unknown(void)
{
    TEST_ASSERT_FALSE(rtl_is_rtl_script(RTL_SCRIPT_UNKNOWN));
}

/* ---- rtl_should_mirror: tests 58-65 ---- */

void test_mirror_text(void)
{
    TEST_ASSERT_TRUE(rtl_should_mirror(RTL_ELEM_TEXT));
}

void test_mirror_menu(void)
{
    TEST_ASSERT_TRUE(rtl_should_mirror(RTL_ELEM_MENU));
}

void test_mirror_card(void)
{
    TEST_ASSERT_TRUE(rtl_should_mirror(RTL_ELEM_CARD));
}

void test_mirror_hud(void)
{
    TEST_ASSERT_TRUE(rtl_should_mirror(RTL_ELEM_HUD));
}

void test_no_mirror_sky(void)
{
    TEST_ASSERT_FALSE(rtl_should_mirror(RTL_ELEM_SKY));
}

void test_no_mirror_zodiac(void)
{
    TEST_ASSERT_FALSE(rtl_should_mirror(RTL_ELEM_ZODIAC));
}

void test_no_mirror_chart(void)
{
    TEST_ASSERT_FALSE(rtl_should_mirror(RTL_ELEM_CHART));
}

void test_no_mirror_clock(void)
{
    TEST_ASSERT_FALSE(rtl_should_mirror(RTL_ELEM_CLOCK));
}

/* ---- rtl_script_name: tests 66-69 ---- */

void test_script_name_arabic(void)
{
    TEST_ASSERT_EQUAL_STRING("Arabic", rtl_script_name(RTL_SCRIPT_ARABIC));
}

void test_script_name_hebrew(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew", rtl_script_name(RTL_SCRIPT_HEBREW));
}

void test_script_name_latin(void)
{
    TEST_ASSERT_EQUAL_STRING("Latin", rtl_script_name(RTL_SCRIPT_LATIN));
}

void test_script_name_unknown(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", rtl_script_name(RTL_SCRIPT_UNKNOWN));
}

void test_script_name_out_of_range(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", rtl_script_name(RTL_SCRIPT_COUNT));
}

/* ---- rtl_direction_name: tests 71-74 ---- */

void test_direction_name_ltr(void)
{
    TEST_ASSERT_EQUAL_STRING("LTR", rtl_direction_name(RTL_DIR_LTR));
}

void test_direction_name_rtl(void)
{
    TEST_ASSERT_EQUAL_STRING("RTL", rtl_direction_name(RTL_DIR_RTL));
}

void test_direction_name_neutral(void)
{
    TEST_ASSERT_EQUAL_STRING("Neutral", rtl_direction_name(RTL_DIR_NEUTRAL));
}

void test_direction_name_out_of_range(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", rtl_direction_name(RTL_DIR_COUNT));
}

/* ---- rtl_element_name: tests 75-78 ---- */

void test_element_name_text(void)
{
    TEST_ASSERT_EQUAL_STRING("Text", rtl_element_name(RTL_ELEM_TEXT));
}

void test_element_name_sky(void)
{
    TEST_ASSERT_EQUAL_STRING("Sky", rtl_element_name(RTL_ELEM_SKY));
}

void test_element_name_zodiac(void)
{
    TEST_ASSERT_EQUAL_STRING("Zodiac", rtl_element_name(RTL_ELEM_ZODIAC));
}

void test_element_name_out_of_range(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", rtl_element_name(RTL_ELEM_COUNT));
}

/* ---- rtl_mirror_x: tests 79-84 ---- */

void test_mirror_x_basic(void)
{
    /* element at x=10, width=100, container=800 -> 800 - 10 - 100 = 690 */
    double result = rtl_mirror_x(10.0, 100.0, 800.0);
    TEST_ASSERT_TRUE(result > 689.5 && result < 690.5);
}

void test_mirror_x_at_zero(void)
{
    /* element at x=0, width=50, container=500 -> 500 - 0 - 50 = 450 */
    double result = rtl_mirror_x(0.0, 50.0, 500.0);
    TEST_ASSERT_TRUE(result > 449.5 && result < 450.5);
}

void test_mirror_x_at_end(void)
{
    /* element at x=700, width=100, container=800 -> 800 - 700 - 100 = 0 */
    double result = rtl_mirror_x(700.0, 100.0, 800.0);
    TEST_ASSERT_TRUE(result > -0.5 && result < 0.5);
}

void test_mirror_x_centered(void)
{
    /* element at x=350, width=100, container=800 -> 800 - 350 - 100 = 350 */
    double result = rtl_mirror_x(350.0, 100.0, 800.0);
    TEST_ASSERT_TRUE(result > 349.5 && result < 350.5);
}

void test_mirror_x_small_container(void)
{
    double result = rtl_mirror_x(5.0, 10.0, 20.0);
    /* 20 - 5 - 10 = 5 */
    TEST_ASSERT_TRUE(result > 4.5 && result < 5.5);
}

void test_mirror_x_double_mirror_returns_original(void)
{
    double original = 120.0;
    double elem_w = 60.0;
    double cont_w = 800.0;
    double mirrored = rtl_mirror_x(original, elem_w, cont_w);
    double back = rtl_mirror_x(mirrored, elem_w, cont_w);
    TEST_ASSERT_TRUE(back > 119.5 && back < 120.5);
}

/* ---- rtl_starts_rtl (UTF-8 string): tests 85-92 ---- */

void test_starts_rtl_arabic_string(void)
{
    /* Arabic "bsm" = ba sin mim */
    TEST_ASSERT_TRUE(rtl_starts_rtl("\xd8\xa8\xd8\xb3\xd9\x85"));
}

void test_starts_rtl_hebrew_string(void)
{
    /* Hebrew "shalom" */
    TEST_ASSERT_TRUE(rtl_starts_rtl("\xd7\xa9\xd7\x9c\xd7\x95\xd7\x9d"));
}

void test_starts_rtl_latin_string(void)
{
    TEST_ASSERT_FALSE(rtl_starts_rtl("Hello World"));
}

void test_starts_rtl_digit_then_arabic(void)
{
    /* Digits are neutral; first alphabetic should be Arabic */
    TEST_ASSERT_TRUE(rtl_starts_rtl("123\xd8\xa8"));
}

void test_starts_rtl_space_then_arabic(void)
{
    /* Whitespace then Arabic */
    TEST_ASSERT_TRUE(rtl_starts_rtl("  \xd8\xa8"));
}

void test_starts_rtl_empty_string(void)
{
    TEST_ASSERT_FALSE(rtl_starts_rtl(""));
}

void test_starts_rtl_null_string(void)
{
    TEST_ASSERT_FALSE(rtl_starts_rtl(NULL));
}

void test_starts_rtl_only_digits(void)
{
    /* All neutral, no strong direction -> false */
    TEST_ASSERT_FALSE(rtl_starts_rtl("12345"));
}

/* ---- rtl_dominant_direction (UTF-8 string): tests 93-100 ---- */

void test_dominant_arabic_string(void)
{
    /* Pure Arabic text */
    TEST_ASSERT_EQUAL_INT(RTL_DIR_RTL,
        rtl_dominant_direction("\xd8\xa8\xd8\xb3\xd9\x85"));
}

void test_dominant_latin_string(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_LTR,
        rtl_dominant_direction("Hello World"));
}

void test_dominant_mixed_more_rtl(void)
{
    /* 3 Arabic chars + 1 Latin char -> RTL dominant */
    TEST_ASSERT_EQUAL_INT(RTL_DIR_RTL,
        rtl_dominant_direction("\xd8\xa8\xd8\xb3\xd9\x85" "a"));
}

void test_dominant_mixed_more_ltr(void)
{
    /* 1 Arabic char + 5 Latin -> LTR dominant */
    TEST_ASSERT_EQUAL_INT(RTL_DIR_LTR,
        rtl_dominant_direction("\xd8\xa8" "Hello"));
}

void test_dominant_only_digits(void)
{
    /* All neutral -> neutral */
    TEST_ASSERT_EQUAL_INT(RTL_DIR_NEUTRAL,
        rtl_dominant_direction("12345"));
}

void test_dominant_empty(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_NEUTRAL,
        rtl_dominant_direction(""));
}

void test_dominant_null(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_DIR_NEUTRAL,
        rtl_dominant_direction(NULL));
}

void test_dominant_hebrew_with_digits(void)
{
    /* Hebrew + digits -> RTL (digits are neutral) */
    TEST_ASSERT_EQUAL_INT(RTL_DIR_RTL,
        rtl_dominant_direction("\xd7\xa9\xd7\x9c" "123"));
}

/* ---- rtl_script_count / rtl_element_count: tests 101-102 ---- */

void test_script_count(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_COUNT, rtl_script_count());
}

void test_element_count(void)
{
    TEST_ASSERT_EQUAL_INT(RTL_ELEM_COUNT, rtl_element_count());
}

/* ---- Edge cases: tests 103-110 ---- */

void test_is_rtl_arabic_extended_a_end(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x08FF));
}

void test_is_rtl_arabic_pres_forms_a_end(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0xFDFF));
}

void test_is_rtl_arabic_pres_forms_b_end(void)
{
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0xFEFF));
}

void test_boundary_below_arabic(void)
{
    /* U+05FF is Hebrew end, U+0600 is Arabic start */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x05FF));  /* Hebrew */
    TEST_ASSERT_TRUE(rtl_is_rtl_codepoint(0x0600));  /* Arabic */
}

void test_boundary_between_blocks(void)
{
    /* U+07C0 is just after Thaana (U+0780-U+07BF) */
    TEST_ASSERT_FALSE(rtl_is_rtl_codepoint(0x07C0));
}

void test_script_arabic_pres_b_is_arabic(void)
{
    /* U+FE70 should identify as Arabic */
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_ARABIC, rtl_identify_script(0xFE70));
}

void test_cjk_unified(void)
{
    /* U+9FFF end of CJK Unified Ideographs */
    TEST_ASSERT_EQUAL_INT(RTL_SCRIPT_CJK, rtl_identify_script(0x9FFF));
}

void test_element_name_menu(void)
{
    TEST_ASSERT_EQUAL_STRING("Menu", rtl_element_name(RTL_ELEM_MENU));
}

/* ---- Additional script name tests: 111-114 ---- */

void test_script_name_persian(void)
{
    TEST_ASSERT_EQUAL_STRING("Persian", rtl_script_name(RTL_SCRIPT_PERSIAN));
}

void test_script_name_urdu(void)
{
    TEST_ASSERT_EQUAL_STRING("Urdu", rtl_script_name(RTL_SCRIPT_URDU));
}

void test_script_name_syriac(void)
{
    TEST_ASSERT_EQUAL_STRING("Syriac", rtl_script_name(RTL_SCRIPT_SYRIAC));
}

void test_script_name_thaana(void)
{
    TEST_ASSERT_EQUAL_STRING("Thaana", rtl_script_name(RTL_SCRIPT_THAANA));
}

/* ---- Additional element name tests: 115-118 ---- */

void test_element_name_card(void)
{
    TEST_ASSERT_EQUAL_STRING("Card", rtl_element_name(RTL_ELEM_CARD));
}

void test_element_name_hud(void)
{
    TEST_ASSERT_EQUAL_STRING("HUD", rtl_element_name(RTL_ELEM_HUD));
}

void test_element_name_chart(void)
{
    TEST_ASSERT_EQUAL_STRING("Chart", rtl_element_name(RTL_ELEM_CHART));
}

void test_element_name_clock(void)
{
    TEST_ASSERT_EQUAL_STRING("Clock", rtl_element_name(RTL_ELEM_CLOCK));
}

/* ---- main ---- */

int main(void)
{
    UNITY_BEGIN();

    /* rtl_is_rtl_codepoint */
    RUN_TEST(test_is_rtl_arabic_basic);
    RUN_TEST(test_is_rtl_arabic_start);
    RUN_TEST(test_is_rtl_arabic_end);
    RUN_TEST(test_is_rtl_arabic_supplement);
    RUN_TEST(test_is_rtl_arabic_extended_a);
    RUN_TEST(test_is_rtl_arabic_pres_forms_a);
    RUN_TEST(test_is_rtl_arabic_pres_forms_b);
    RUN_TEST(test_is_rtl_hebrew_basic);
    RUN_TEST(test_is_rtl_hebrew_start);
    RUN_TEST(test_is_rtl_hebrew_end);
    RUN_TEST(test_is_rtl_hebrew_pres_forms);
    RUN_TEST(test_is_rtl_hebrew_pres_forms_end);
    RUN_TEST(test_is_rtl_syriac);
    RUN_TEST(test_is_rtl_syriac_start);
    RUN_TEST(test_is_rtl_syriac_end);
    RUN_TEST(test_is_rtl_thaana);
    RUN_TEST(test_is_rtl_thaana_end);
    RUN_TEST(test_is_rtl_rlm_mark);
    RUN_TEST(test_is_rtl_rle_mark);
    RUN_TEST(test_is_rtl_rlo_mark);
    RUN_TEST(test_is_not_rtl_latin);
    RUN_TEST(test_is_not_rtl_digit);
    RUN_TEST(test_is_not_rtl_devanagari);
    RUN_TEST(test_is_not_rtl_cjk);
    RUN_TEST(test_is_not_rtl_null);

    /* rtl_codepoint_direction */
    RUN_TEST(test_direction_arabic_is_rtl);
    RUN_TEST(test_direction_hebrew_is_rtl);
    RUN_TEST(test_direction_latin_is_ltr);
    RUN_TEST(test_direction_devanagari_is_ltr);
    RUN_TEST(test_direction_digit_is_neutral);
    RUN_TEST(test_direction_space_is_neutral);
    RUN_TEST(test_direction_punctuation_is_neutral);

    /* rtl_identify_script */
    RUN_TEST(test_script_latin);
    RUN_TEST(test_script_latin_lowercase);
    RUN_TEST(test_script_arabic);
    RUN_TEST(test_script_arabic_supplement);
    RUN_TEST(test_script_hebrew);
    RUN_TEST(test_script_hebrew_presentation);
    RUN_TEST(test_script_syriac);
    RUN_TEST(test_script_thaana);
    RUN_TEST(test_script_devanagari);
    RUN_TEST(test_script_cjk);
    RUN_TEST(test_script_thai);
    RUN_TEST(test_script_geez);
    RUN_TEST(test_script_unknown_digit);

    /* rtl_is_rtl_script */
    RUN_TEST(test_is_rtl_script_arabic);
    RUN_TEST(test_is_rtl_script_hebrew);
    RUN_TEST(test_is_rtl_script_persian);
    RUN_TEST(test_is_rtl_script_urdu);
    RUN_TEST(test_is_rtl_script_syriac);
    RUN_TEST(test_is_rtl_script_thaana);
    RUN_TEST(test_is_not_rtl_script_latin);
    RUN_TEST(test_is_not_rtl_script_devanagari);
    RUN_TEST(test_is_not_rtl_script_cjk);
    RUN_TEST(test_is_not_rtl_script_thai);
    RUN_TEST(test_is_not_rtl_script_geez);
    RUN_TEST(test_is_not_rtl_script_unknown);

    /* rtl_should_mirror */
    RUN_TEST(test_mirror_text);
    RUN_TEST(test_mirror_menu);
    RUN_TEST(test_mirror_card);
    RUN_TEST(test_mirror_hud);
    RUN_TEST(test_no_mirror_sky);
    RUN_TEST(test_no_mirror_zodiac);
    RUN_TEST(test_no_mirror_chart);
    RUN_TEST(test_no_mirror_clock);

    /* rtl_script_name */
    RUN_TEST(test_script_name_arabic);
    RUN_TEST(test_script_name_hebrew);
    RUN_TEST(test_script_name_latin);
    RUN_TEST(test_script_name_unknown);
    RUN_TEST(test_script_name_out_of_range);

    /* rtl_direction_name */
    RUN_TEST(test_direction_name_ltr);
    RUN_TEST(test_direction_name_rtl);
    RUN_TEST(test_direction_name_neutral);
    RUN_TEST(test_direction_name_out_of_range);

    /* rtl_element_name */
    RUN_TEST(test_element_name_text);
    RUN_TEST(test_element_name_sky);
    RUN_TEST(test_element_name_zodiac);
    RUN_TEST(test_element_name_out_of_range);

    /* rtl_mirror_x */
    RUN_TEST(test_mirror_x_basic);
    RUN_TEST(test_mirror_x_at_zero);
    RUN_TEST(test_mirror_x_at_end);
    RUN_TEST(test_mirror_x_centered);
    RUN_TEST(test_mirror_x_small_container);
    RUN_TEST(test_mirror_x_double_mirror_returns_original);

    /* rtl_starts_rtl */
    RUN_TEST(test_starts_rtl_arabic_string);
    RUN_TEST(test_starts_rtl_hebrew_string);
    RUN_TEST(test_starts_rtl_latin_string);
    RUN_TEST(test_starts_rtl_digit_then_arabic);
    RUN_TEST(test_starts_rtl_space_then_arabic);
    RUN_TEST(test_starts_rtl_empty_string);
    RUN_TEST(test_starts_rtl_null_string);
    RUN_TEST(test_starts_rtl_only_digits);

    /* rtl_dominant_direction */
    RUN_TEST(test_dominant_arabic_string);
    RUN_TEST(test_dominant_latin_string);
    RUN_TEST(test_dominant_mixed_more_rtl);
    RUN_TEST(test_dominant_mixed_more_ltr);
    RUN_TEST(test_dominant_only_digits);
    RUN_TEST(test_dominant_empty);
    RUN_TEST(test_dominant_null);
    RUN_TEST(test_dominant_hebrew_with_digits);

    /* count functions */
    RUN_TEST(test_script_count);
    RUN_TEST(test_element_count);

    /* edge cases */
    RUN_TEST(test_is_rtl_arabic_extended_a_end);
    RUN_TEST(test_is_rtl_arabic_pres_forms_a_end);
    RUN_TEST(test_is_rtl_arabic_pres_forms_b_end);
    RUN_TEST(test_boundary_below_arabic);
    RUN_TEST(test_boundary_between_blocks);
    RUN_TEST(test_script_arabic_pres_b_is_arabic);
    RUN_TEST(test_cjk_unified);
    RUN_TEST(test_element_name_menu);

    /* additional script names */
    RUN_TEST(test_script_name_persian);
    RUN_TEST(test_script_name_urdu);
    RUN_TEST(test_script_name_syriac);
    RUN_TEST(test_script_name_thaana);

    /* additional element names */
    RUN_TEST(test_element_name_card);
    RUN_TEST(test_element_name_hud);
    RUN_TEST(test_element_name_chart);
    RUN_TEST(test_element_name_clock);

    return UNITY_END();
}
