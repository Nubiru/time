/* test_buddhist_interpret.c -- Buddhist interpretation data + tiered text
 * TDD RED phase: tests written before implementation.
 * 5 Uposatha types, 4 kalpa phases, 12 Pali months, tiered interpretation.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "../../unity/unity.h"
#include "../../../src/systems/buddhist/buddhist_interpret.h"
#include "../../../src/systems/buddhist/buddhist.h"
#include "../../../src/systems/buddhist/kalpa.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== count functions ===== */

void test_uposatha_count_returns_5(void)
{
    TEST_ASSERT_EQUAL_INT(5, bi_uposatha_count());
}

void test_kalpa_phase_count_returns_4(void)
{
    TEST_ASSERT_EQUAL_INT(4, bi_kalpa_phase_count());
}

void test_month_count_returns_12(void)
{
    TEST_ASSERT_EQUAL_INT(12, bi_month_count());
}

/* ===== bi_uposatha_data -- all 5 types ===== */

void test_uposatha_none(void)
{
    bi_uposatha_t u = bi_uposatha_data(UPOSATHA_NONE);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_NONE, u.type);
    TEST_ASSERT_EQUAL_STRING("No Observance", u.name);
    TEST_ASSERT_TRUE(strlen(u.practice) > 0);
    TEST_ASSERT_TRUE(strlen(u.meaning) > 0);
}

void test_uposatha_new_moon(void)
{
    bi_uposatha_t u = bi_uposatha_data(UPOSATHA_NEW_MOON);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_NEW_MOON, u.type);
    TEST_ASSERT_EQUAL_STRING("New Moon Observance", u.name);
    TEST_ASSERT_NOT_NULL(strstr(u.practice, "Patimokkha"));
    TEST_ASSERT_NOT_NULL(strstr(u.meaning, "seed"));
}

void test_uposatha_full_moon(void)
{
    bi_uposatha_t u = bi_uposatha_data(UPOSATHA_FULL_MOON);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_FULL_MOON, u.type);
    TEST_ASSERT_EQUAL_STRING("Full Moon Observance", u.name);
    TEST_ASSERT_NOT_NULL(strstr(u.practice, "Patimokkha"));
    TEST_ASSERT_NOT_NULL(strstr(u.meaning, "Illumination"));
}

void test_uposatha_first_quarter(void)
{
    bi_uposatha_t u = bi_uposatha_data(UPOSATHA_FIRST_QUARTER);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_FIRST_QUARTER, u.type);
    TEST_ASSERT_EQUAL_STRING("Waxing Half-Moon", u.name);
    TEST_ASSERT_TRUE(strlen(u.practice) > 0);
    TEST_ASSERT_NOT_NULL(strstr(u.meaning, "aspiration"));
}

void test_uposatha_last_quarter(void)
{
    bi_uposatha_t u = bi_uposatha_data(UPOSATHA_LAST_QUARTER);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_LAST_QUARTER, u.type);
    TEST_ASSERT_EQUAL_STRING("Waning Half-Moon", u.name);
    TEST_ASSERT_NOT_NULL(strstr(u.practice, "letting go"));
    TEST_ASSERT_NOT_NULL(strstr(u.meaning, "impermanence"));
}

void test_uposatha_all_strings_nonempty(void)
{
    int i;
    for (i = 0; i <= 4; i++) {
        bi_uposatha_t u = bi_uposatha_data((uposatha_type_t)i);
        TEST_ASSERT_EQUAL_INT(i, (int)u.type);
        TEST_ASSERT_TRUE(strlen(u.name) > 0);
        TEST_ASSERT_TRUE(strlen(u.practice) > 0);
        TEST_ASSERT_TRUE(strlen(u.meaning) > 0);
    }
}

void test_uposatha_invalid_returns_none(void)
{
    bi_uposatha_t u = bi_uposatha_data((uposatha_type_t)99);
    TEST_ASSERT_EQUAL_INT(UPOSATHA_NONE, (int)u.type);
    TEST_ASSERT_EQUAL_STRING("No Observance", u.name);
}

/* ===== bi_kalpa_data -- all 4 phases ===== */

void test_kalpa_formation(void)
{
    bi_kalpa_t k = bi_kalpa_data(KALPA_PHASE_FORMATION);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_FORMATION, (int)k.phase);
    TEST_ASSERT_EQUAL_STRING("Formation", k.name);
    TEST_ASSERT_EQUAL_STRING("Vivatta", k.pali);
    TEST_ASSERT_NOT_NULL(strstr(k.description, "arise"));
    TEST_ASSERT_NOT_NULL(strstr(k.meaning, "creation"));
}

void test_kalpa_existence(void)
{
    bi_kalpa_t k = bi_kalpa_data(KALPA_PHASE_EXISTENCE);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_EXISTENCE, (int)k.phase);
    TEST_ASSERT_EQUAL_STRING("Existence", k.name);
    TEST_ASSERT_EQUAL_STRING("Vivattathaayi", k.pali);
    TEST_ASSERT_NOT_NULL(strstr(k.description, "wheel"));
    TEST_ASSERT_NOT_NULL(strstr(k.meaning, "sentient"));
}

void test_kalpa_destruction(void)
{
    bi_kalpa_t k = bi_kalpa_data(KALPA_PHASE_DESTRUCTION);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_DESTRUCTION, (int)k.phase);
    TEST_ASSERT_EQUAL_STRING("Destruction", k.name);
    TEST_ASSERT_EQUAL_STRING("Samvatta", k.pali);
    TEST_ASSERT_NOT_NULL(strstr(k.description, "dissolve"));
    TEST_ASSERT_NOT_NULL(strstr(k.meaning, "impermanent"));
}

void test_kalpa_emptiness(void)
{
    bi_kalpa_t k = bi_kalpa_data(KALPA_PHASE_EMPTINESS);
    TEST_ASSERT_EQUAL_INT(KALPA_PHASE_EMPTINESS, (int)k.phase);
    TEST_ASSERT_EQUAL_STRING("Emptiness", k.name);
    TEST_ASSERT_EQUAL_STRING("Samvattathaayi", k.pali);
    TEST_ASSERT_NOT_NULL(strstr(k.description, "darkness"));
    TEST_ASSERT_NOT_NULL(strstr(k.meaning, "rest"));
}

void test_kalpa_all_strings_nonempty(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        bi_kalpa_t k = bi_kalpa_data((kalpa_phase_t)i);
        TEST_ASSERT_EQUAL_INT(i, (int)k.phase);
        TEST_ASSERT_TRUE(strlen(k.name) > 0);
        TEST_ASSERT_TRUE(strlen(k.pali) > 0);
        TEST_ASSERT_TRUE(strlen(k.description) > 0);
        TEST_ASSERT_TRUE(strlen(k.meaning) > 0);
    }
}

void test_kalpa_invalid_returns_unknown(void)
{
    bi_kalpa_t k = bi_kalpa_data((kalpa_phase_t)99);
    TEST_ASSERT_EQUAL_STRING("?", k.name);
}

/* ===== bi_month_data -- all 12 months ===== */

void test_month_citta(void)
{
    bi_month_t m = bi_month_data(1);
    TEST_ASSERT_EQUAL_INT(1, m.month);
    TEST_ASSERT_EQUAL_STRING("Citta", m.pali_name);
    TEST_ASSERT_EQUAL_STRING("Hot", m.season);
    TEST_ASSERT_TRUE(strlen(m.meaning) > 0);
}

void test_month_vesakha(void)
{
    bi_month_t m = bi_month_data(2);
    TEST_ASSERT_EQUAL_INT(2, m.month);
    TEST_ASSERT_EQUAL_STRING("Vesakha", m.pali_name);
    TEST_ASSERT_NOT_NULL(strstr(m.meaning, "sacred"));
}

void test_month_asalha(void)
{
    bi_month_t m = bi_month_data(4);
    TEST_ASSERT_EQUAL_INT(4, m.month);
    TEST_ASSERT_EQUAL_STRING("Asalha", m.pali_name);
    TEST_ASSERT_EQUAL_STRING("Rainy", m.season);
    TEST_ASSERT_TRUE(strlen(m.observance) > 0);
}

void test_month_all_strings_nonempty(void)
{
    int i;
    for (i = 1; i <= 12; i++) {
        bi_month_t m = bi_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_TRUE(strlen(m.pali_name) > 0);
        TEST_ASSERT_TRUE(strlen(m.season) > 0);
        TEST_ASSERT_NOT_NULL(m.observance);
        TEST_ASSERT_TRUE(strlen(m.meaning) > 0);
    }
}

void test_month_seasons_hot_rainy_cool(void)
{
    /* Months 1-3: Hot, 4-7: Rainy, 8-11: Cool, 12: Hot */
    TEST_ASSERT_EQUAL_STRING("Hot", bi_month_data(1).season);
    TEST_ASSERT_EQUAL_STRING("Hot", bi_month_data(2).season);
    TEST_ASSERT_EQUAL_STRING("Hot", bi_month_data(3).season);
    TEST_ASSERT_EQUAL_STRING("Rainy", bi_month_data(4).season);
    TEST_ASSERT_EQUAL_STRING("Rainy", bi_month_data(5).season);
    TEST_ASSERT_EQUAL_STRING("Rainy", bi_month_data(6).season);
    TEST_ASSERT_EQUAL_STRING("Rainy", bi_month_data(7).season);
    TEST_ASSERT_EQUAL_STRING("Cool", bi_month_data(8).season);
    TEST_ASSERT_EQUAL_STRING("Cool", bi_month_data(9).season);
    TEST_ASSERT_EQUAL_STRING("Cool", bi_month_data(10).season);
    TEST_ASSERT_EQUAL_STRING("Cool", bi_month_data(11).season);
    TEST_ASSERT_EQUAL_STRING("Hot", bi_month_data(12).season);
}

void test_vassa_months_have_observance(void)
{
    /* Months 5-7 are Vassa-related, observance should mention Vassa */
    bi_month_t m5 = bi_month_data(5);
    bi_month_t m6 = bi_month_data(6);
    bi_month_t m7 = bi_month_data(7);
    TEST_ASSERT_NOT_NULL(strstr(m5.observance, "Vassa"));
    TEST_ASSERT_NOT_NULL(strstr(m6.observance, "Vassa"));
    TEST_ASSERT_NOT_NULL(strstr(m7.observance, "Vassa"));
}

/* ===== bi_month_data -- invalid ===== */

void test_month_invalid_zero(void)
{
    bi_month_t m = bi_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
}

void test_month_invalid_13(void)
{
    bi_month_t m = bi_month_data(13);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
}

void test_month_invalid_negative(void)
{
    bi_month_t m = bi_month_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
}

/* ===== bi_interpret -- glyph ===== */

void test_interpret_glyph_nonempty(void)
{
    buddhist_interp_t r = bi_interpret(2569, 2, UPOSATHA_FULL_MOON,
                                       KALPA_PHASE_EXISTENCE);
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
}

void test_interpret_glyph_is_be(void)
{
    buddhist_interp_t r = bi_interpret(2569, 4, UPOSATHA_NONE,
                                       KALPA_PHASE_FORMATION);
    TEST_ASSERT_EQUAL_STRING("BE", r.glyph);
}

/* ===== bi_interpret -- glance ===== */

void test_interpret_glance_contains_be_year(void)
{
    buddhist_interp_t r = bi_interpret(2569, 2, UPOSATHA_NONE,
                                       KALPA_PHASE_EXISTENCE);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "BE"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "2569"));
}

void test_interpret_glance_contains_month_name(void)
{
    buddhist_interp_t r = bi_interpret(2569, 2, UPOSATHA_NONE,
                                       KALPA_PHASE_EXISTENCE);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Vesakha"));
}

void test_interpret_glance_uposatha_shows_name(void)
{
    buddhist_interp_t r = bi_interpret(2569, 2, UPOSATHA_FULL_MOON,
                                       KALPA_PHASE_EXISTENCE);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Full Moon Observance"));
}

void test_interpret_glance_no_uposatha_shows_season(void)
{
    buddhist_interp_t r = bi_interpret(2569, 8, UPOSATHA_NONE,
                                       KALPA_PHASE_EXISTENCE);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Cool"));
}

/* ===== bi_interpret -- detail ===== */

void test_interpret_detail_contains_month_meaning(void)
{
    buddhist_interp_t r = bi_interpret(2569, 1, UPOSATHA_NONE,
                                       KALPA_PHASE_EXISTENCE);
    /* Citta meaning: "awakening" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "awakening"));
}

void test_interpret_detail_contains_uposatha_practice(void)
{
    buddhist_interp_t r = bi_interpret(2569, 1, UPOSATHA_NEW_MOON,
                                       KALPA_PHASE_EXISTENCE);
    /* New Moon practice mentions Patimokkha */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Patimokkha"));
}

void test_interpret_detail_contains_kalpa_context(void)
{
    buddhist_interp_t r = bi_interpret(2569, 1, UPOSATHA_NONE,
                                       KALPA_PHASE_EXISTENCE);
    /* Should mention the kalpa phase name */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Existence"));
}

void test_interpret_vesak_detail_mentions_sacred(void)
{
    buddhist_interp_t r = bi_interpret(2569, 2, UPOSATHA_FULL_MOON,
                                       KALPA_PHASE_EXISTENCE);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "sacred"));
}

void test_interpret_full_moon_detail_mentions_illumination(void)
{
    buddhist_interp_t r = bi_interpret(2569, 4, UPOSATHA_FULL_MOON,
                                       KALPA_PHASE_EXISTENCE);
    /* Full moon meaning mentions illumination */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "llumination"));
}

/* ===== purity ===== */

void test_purity_uposatha_data(void)
{
    bi_uposatha_t a = bi_uposatha_data(UPOSATHA_FULL_MOON);
    bi_uposatha_t b = bi_uposatha_data(UPOSATHA_FULL_MOON);
    TEST_ASSERT_EQUAL_INT((int)a.type, (int)b.type);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_STRING(a.practice, b.practice);
    TEST_ASSERT_EQUAL_STRING(a.meaning, b.meaning);
}

void test_purity_kalpa_data(void)
{
    bi_kalpa_t a = bi_kalpa_data(KALPA_PHASE_DESTRUCTION);
    bi_kalpa_t b = bi_kalpa_data(KALPA_PHASE_DESTRUCTION);
    TEST_ASSERT_EQUAL_INT((int)a.phase, (int)b.phase);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_STRING(a.pali, b.pali);
    TEST_ASSERT_EQUAL_STRING(a.description, b.description);
    TEST_ASSERT_EQUAL_STRING(a.meaning, b.meaning);
}

void test_purity_month_data(void)
{
    bi_month_t a = bi_month_data(7);
    bi_month_t b = bi_month_data(7);
    TEST_ASSERT_EQUAL_INT(a.month, b.month);
    TEST_ASSERT_EQUAL_STRING(a.pali_name, b.pali_name);
    TEST_ASSERT_EQUAL_STRING(a.season, b.season);
    TEST_ASSERT_EQUAL_STRING(a.observance, b.observance);
    TEST_ASSERT_EQUAL_STRING(a.meaning, b.meaning);
}

void test_purity_interpret(void)
{
    buddhist_interp_t a = bi_interpret(2569, 9, UPOSATHA_FULL_MOON,
                                       KALPA_PHASE_EMPTINESS);
    buddhist_interp_t b = bi_interpret(2569, 9, UPOSATHA_FULL_MOON,
                                       KALPA_PHASE_EMPTINESS);
    TEST_ASSERT_EQUAL_STRING(a.glyph, b.glyph);
    TEST_ASSERT_EQUAL_STRING(a.glance, b.glance);
    TEST_ASSERT_EQUAL_STRING(a.detail, b.detail);
}

int main(void)
{
    UNITY_BEGIN();

    /* counts */
    RUN_TEST(test_uposatha_count_returns_5);
    RUN_TEST(test_kalpa_phase_count_returns_4);
    RUN_TEST(test_month_count_returns_12);

    /* bi_uposatha_data -- all 5 types */
    RUN_TEST(test_uposatha_none);
    RUN_TEST(test_uposatha_new_moon);
    RUN_TEST(test_uposatha_full_moon);
    RUN_TEST(test_uposatha_first_quarter);
    RUN_TEST(test_uposatha_last_quarter);
    RUN_TEST(test_uposatha_all_strings_nonempty);
    RUN_TEST(test_uposatha_invalid_returns_none);

    /* bi_kalpa_data -- all 4 phases */
    RUN_TEST(test_kalpa_formation);
    RUN_TEST(test_kalpa_existence);
    RUN_TEST(test_kalpa_destruction);
    RUN_TEST(test_kalpa_emptiness);
    RUN_TEST(test_kalpa_all_strings_nonempty);
    RUN_TEST(test_kalpa_invalid_returns_unknown);

    /* bi_month_data -- all 12 months */
    RUN_TEST(test_month_citta);
    RUN_TEST(test_month_vesakha);
    RUN_TEST(test_month_asalha);
    RUN_TEST(test_month_all_strings_nonempty);
    RUN_TEST(test_month_seasons_hot_rainy_cool);
    RUN_TEST(test_vassa_months_have_observance);

    /* bi_month_data -- invalid */
    RUN_TEST(test_month_invalid_zero);
    RUN_TEST(test_month_invalid_13);
    RUN_TEST(test_month_invalid_negative);

    /* bi_interpret -- glyph */
    RUN_TEST(test_interpret_glyph_nonempty);
    RUN_TEST(test_interpret_glyph_is_be);

    /* bi_interpret -- glance */
    RUN_TEST(test_interpret_glance_contains_be_year);
    RUN_TEST(test_interpret_glance_contains_month_name);
    RUN_TEST(test_interpret_glance_uposatha_shows_name);
    RUN_TEST(test_interpret_glance_no_uposatha_shows_season);

    /* bi_interpret -- detail */
    RUN_TEST(test_interpret_detail_contains_month_meaning);
    RUN_TEST(test_interpret_detail_contains_uposatha_practice);
    RUN_TEST(test_interpret_detail_contains_kalpa_context);
    RUN_TEST(test_interpret_vesak_detail_mentions_sacred);
    RUN_TEST(test_interpret_full_moon_detail_mentions_illumination);

    /* purity */
    RUN_TEST(test_purity_uposatha_data);
    RUN_TEST(test_purity_kalpa_data);
    RUN_TEST(test_purity_month_data);
    RUN_TEST(test_purity_interpret);

    return UNITY_END();
}
