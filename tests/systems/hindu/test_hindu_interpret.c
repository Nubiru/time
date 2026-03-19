/* test_hindu_interpret.c -- Hindu tithi interpretation tests
 * TDD: tests written first, then implementation.
 * Covers: hndi_tithi_data, hndi_interpret, hndi_tithi_count,
 *         invalid inputs, completeness, glyph/glance/detail format. */

#include "../../unity/unity.h"
#include "../../../src/systems/hindu/hindu_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== hndi_tithi_data: specific tithis ===== */

void test_tithi_data_1_shukla_pratipada(void)
{
    hndi_tithi_t t = hndi_tithi_data(1);
    TEST_ASSERT_EQUAL_INT(1, t.number);
    TEST_ASSERT_EQUAL_STRING("Agni", t.deity);
    TEST_ASSERT_EQUAL_STRING("Auspicious", t.quality);
    TEST_ASSERT_EQUAL_STRING("Beginning new ventures", t.activity);
    TEST_ASSERT_NOT_NULL(t.brief);
    TEST_ASSERT_TRUE(strlen(t.brief) > 0);
}

void test_tithi_data_3_shukla_tritiya(void)
{
    hndi_tithi_t t = hndi_tithi_data(3);
    TEST_ASSERT_EQUAL_INT(3, t.number);
    TEST_ASSERT_EQUAL_STRING("Gauri", t.deity);
    TEST_ASSERT_EQUAL_STRING("Very Auspicious", t.quality);
    TEST_ASSERT_EQUAL_STRING("Rituals, celebrations", t.activity);
}

void test_tithi_data_11_shukla_ekadashi(void)
{
    hndi_tithi_t t = hndi_tithi_data(11);
    TEST_ASSERT_EQUAL_INT(11, t.number);
    TEST_ASSERT_EQUAL_STRING("Vishnu", t.deity);
    TEST_ASSERT_EQUAL_STRING("Very Auspicious", t.quality);
    TEST_ASSERT_EQUAL_STRING("Fasting, devotion", t.activity);
}

void test_tithi_data_15_purnima(void)
{
    hndi_tithi_t t = hndi_tithi_data(15);
    TEST_ASSERT_EQUAL_INT(15, t.number);
    TEST_ASSERT_EQUAL_STRING("Soma", t.deity);
    TEST_ASSERT_EQUAL_STRING("Very Auspicious", t.quality);
    TEST_ASSERT_EQUAL_STRING("Worship, celebration", t.activity);
}

void test_tithi_data_23_krishna_ashtami(void)
{
    hndi_tithi_t t = hndi_tithi_data(23);
    TEST_ASSERT_EQUAL_INT(23, t.number);
    TEST_ASSERT_EQUAL_STRING("Shiva", t.deity);
    TEST_ASSERT_EQUAL_STRING("Inauspicious", t.quality);
    TEST_ASSERT_EQUAL_STRING("Fasting, austerity", t.activity);
}

void test_tithi_data_26_krishna_ekadashi(void)
{
    hndi_tithi_t t = hndi_tithi_data(26);
    TEST_ASSERT_EQUAL_INT(26, t.number);
    TEST_ASSERT_EQUAL_STRING("Vishnu", t.deity);
    TEST_ASSERT_EQUAL_STRING("Very Auspicious", t.quality);
    TEST_ASSERT_EQUAL_STRING("Fasting, devotion", t.activity);
}

void test_tithi_data_29_krishna_chaturdashi(void)
{
    hndi_tithi_t t = hndi_tithi_data(29);
    TEST_ASSERT_EQUAL_INT(29, t.number);
    TEST_ASSERT_EQUAL_STRING("Shiva", t.deity);
    TEST_ASSERT_EQUAL_STRING("Inauspicious", t.quality);
}

void test_tithi_data_30_amavasya(void)
{
    hndi_tithi_t t = hndi_tithi_data(30);
    TEST_ASSERT_EQUAL_INT(30, t.number);
    TEST_ASSERT_EQUAL_STRING("Pitris", t.deity);
    TEST_ASSERT_EQUAL_STRING("Neutral", t.quality);
    TEST_ASSERT_EQUAL_STRING("Ancestor worship, new beginnings", t.activity);
}

/* ===== hndi_tithi_data: invalid inputs ===== */

void test_tithi_data_invalid_zero(void)
{
    hndi_tithi_t t = hndi_tithi_data(0);
    TEST_ASSERT_EQUAL_INT(-1, t.number);
    TEST_ASSERT_EQUAL_STRING("?", t.deity);
    TEST_ASSERT_EQUAL_STRING("?", t.quality);
    TEST_ASSERT_EQUAL_STRING("?", t.activity);
    TEST_ASSERT_EQUAL_STRING("?", t.brief);
}

void test_tithi_data_invalid_31(void)
{
    hndi_tithi_t t = hndi_tithi_data(31);
    TEST_ASSERT_EQUAL_INT(-1, t.number);
    TEST_ASSERT_EQUAL_STRING("?", t.deity);
}

void test_tithi_data_invalid_negative(void)
{
    hndi_tithi_t t = hndi_tithi_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, t.number);
    TEST_ASSERT_EQUAL_STRING("?", t.quality);
}

/* ===== Completeness: all 30 tithis have non-empty fields ===== */

void test_all_30_tithis_have_nonempty_fields(void)
{
    for (int i = 1; i <= 30; i++) {
        hndi_tithi_t t = hndi_tithi_data(i);
        TEST_ASSERT_EQUAL_INT(i, t.number);
        TEST_ASSERT_NOT_NULL(t.deity);
        TEST_ASSERT_TRUE(strlen(t.deity) > 0);
        TEST_ASSERT_NOT_NULL(t.quality);
        TEST_ASSERT_TRUE(strlen(t.quality) > 0);
        TEST_ASSERT_NOT_NULL(t.activity);
        TEST_ASSERT_TRUE(strlen(t.activity) > 0);
        TEST_ASSERT_NOT_NULL(t.brief);
        TEST_ASSERT_TRUE(strlen(t.brief) > 0);
    }
}

/* ===== Ekadashi tithis (11, 26): both Very Auspicious ===== */

void test_both_ekadashis_very_auspicious(void)
{
    hndi_tithi_t shukla = hndi_tithi_data(11);
    hndi_tithi_t krishna = hndi_tithi_data(26);
    TEST_ASSERT_EQUAL_STRING("Very Auspicious", shukla.quality);
    TEST_ASSERT_EQUAL_STRING("Very Auspicious", krishna.quality);
}

/* ===== hndi_tithi_count ===== */

void test_tithi_count_returns_30(void)
{
    TEST_ASSERT_EQUAL_INT(30, hndi_tithi_count());
}

/* ===== hndi_interpret: glyph format ===== */

void test_interpret_glyph_shukla_1(void)
{
    hindu_interp_t r = hndi_interpret(1, NULL, NULL);
    TEST_ASSERT_EQUAL_STRING("S1", r.glyph);
}

void test_interpret_glyph_shukla_15(void)
{
    hindu_interp_t r = hndi_interpret(15, NULL, NULL);
    TEST_ASSERT_EQUAL_STRING("S15", r.glyph);
}

void test_interpret_glyph_krishna_1(void)
{
    /* Tithi 16 = Krishna Pratipada = K1 */
    hindu_interp_t r = hndi_interpret(16, NULL, NULL);
    TEST_ASSERT_EQUAL_STRING("K1", r.glyph);
}

void test_interpret_glyph_krishna_15(void)
{
    /* Tithi 30 = Amavasya = K15 */
    hindu_interp_t r = hndi_interpret(30, NULL, NULL);
    TEST_ASSERT_EQUAL_STRING("K15", r.glyph);
}

void test_interpret_glyph_shukla_5(void)
{
    hindu_interp_t r = hndi_interpret(5, NULL, NULL);
    TEST_ASSERT_EQUAL_STRING("S5", r.glyph);
}

void test_interpret_glyph_krishna_12(void)
{
    /* Tithi 27 = Krishna Dvadashi = K12 */
    hindu_interp_t r = hndi_interpret(27, NULL, NULL);
    TEST_ASSERT_EQUAL_STRING("K12", r.glyph);
}

/* ===== hndi_interpret: glance contains deity and quality ===== */

void test_interpret_glance_contains_deity(void)
{
    hindu_interp_t r = hndi_interpret(3, NULL, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Gauri"));
}

void test_interpret_glance_contains_quality(void)
{
    hindu_interp_t r = hndi_interpret(3, NULL, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Very Auspicious"));
}

void test_interpret_glance_contains_activity(void)
{
    hindu_interp_t r = hndi_interpret(11, NULL, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Fasting, devotion"));
}

/* ===== hndi_interpret: detail contains brief and activity ===== */

void test_interpret_detail_contains_brief(void)
{
    hindu_interp_t r = hndi_interpret(15, NULL, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Full Moon"));
}

void test_interpret_detail_contains_activity(void)
{
    hindu_interp_t r = hndi_interpret(15, NULL, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Worship, celebration"));
}

void test_interpret_detail_contains_deity(void)
{
    hindu_interp_t r = hndi_interpret(30, NULL, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Pitris"));
}

/* ===== hndi_interpret: NULL nakshatra/yoga ===== */

void test_interpret_null_nakshatra_omits(void)
{
    hindu_interp_t r = hndi_interpret(1, NULL, "Priti");
    TEST_ASSERT_NULL(strstr(r.detail, "Nakshatra"));
}

void test_interpret_null_yoga_omits(void)
{
    hindu_interp_t r = hndi_interpret(1, "Ashwini", NULL);
    TEST_ASSERT_NULL(strstr(r.detail, "Yoga"));
}

void test_interpret_both_null_no_nakshatra_yoga(void)
{
    hindu_interp_t r = hndi_interpret(5, NULL, NULL);
    TEST_ASSERT_NULL(strstr(r.detail, "Nakshatra"));
    TEST_ASSERT_NULL(strstr(r.detail, "Yoga"));
}

/* ===== hndi_interpret: with nakshatra and yoga ===== */

void test_interpret_with_nakshatra_includes(void)
{
    hindu_interp_t r = hndi_interpret(1, "Ashwini", NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Nakshatra: Ashwini"));
}

void test_interpret_with_yoga_includes(void)
{
    hindu_interp_t r = hndi_interpret(1, NULL, "Priti");
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Yoga: Priti"));
}

void test_interpret_with_both_nakshatra_and_yoga(void)
{
    hindu_interp_t r = hndi_interpret(7, "Rohini", "Saubhagya");
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Nakshatra: Rohini"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Yoga: Saubhagya"));
}

/* ===== hndi_interpret: invalid tithi ===== */

void test_interpret_invalid_tithi_glyph(void)
{
    hindu_interp_t r = hndi_interpret(0, NULL, NULL);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

void test_interpret_invalid_tithi_glance(void)
{
    hindu_interp_t r = hndi_interpret(31, NULL, NULL);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    /* Should contain "?" for unknown */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* hndi_tithi_data: specific tithis */
    RUN_TEST(test_tithi_data_1_shukla_pratipada);
    RUN_TEST(test_tithi_data_3_shukla_tritiya);
    RUN_TEST(test_tithi_data_11_shukla_ekadashi);
    RUN_TEST(test_tithi_data_15_purnima);
    RUN_TEST(test_tithi_data_23_krishna_ashtami);
    RUN_TEST(test_tithi_data_26_krishna_ekadashi);
    RUN_TEST(test_tithi_data_29_krishna_chaturdashi);
    RUN_TEST(test_tithi_data_30_amavasya);

    /* hndi_tithi_data: invalid inputs */
    RUN_TEST(test_tithi_data_invalid_zero);
    RUN_TEST(test_tithi_data_invalid_31);
    RUN_TEST(test_tithi_data_invalid_negative);

    /* Completeness */
    RUN_TEST(test_all_30_tithis_have_nonempty_fields);

    /* Ekadashi */
    RUN_TEST(test_both_ekadashis_very_auspicious);

    /* hndi_tithi_count */
    RUN_TEST(test_tithi_count_returns_30);

    /* hndi_interpret: glyph format */
    RUN_TEST(test_interpret_glyph_shukla_1);
    RUN_TEST(test_interpret_glyph_shukla_15);
    RUN_TEST(test_interpret_glyph_krishna_1);
    RUN_TEST(test_interpret_glyph_krishna_15);
    RUN_TEST(test_interpret_glyph_shukla_5);
    RUN_TEST(test_interpret_glyph_krishna_12);

    /* hndi_interpret: glance */
    RUN_TEST(test_interpret_glance_contains_deity);
    RUN_TEST(test_interpret_glance_contains_quality);
    RUN_TEST(test_interpret_glance_contains_activity);

    /* hndi_interpret: detail */
    RUN_TEST(test_interpret_detail_contains_brief);
    RUN_TEST(test_interpret_detail_contains_activity);
    RUN_TEST(test_interpret_detail_contains_deity);

    /* hndi_interpret: NULL handling */
    RUN_TEST(test_interpret_null_nakshatra_omits);
    RUN_TEST(test_interpret_null_yoga_omits);
    RUN_TEST(test_interpret_both_null_no_nakshatra_yoga);

    /* hndi_interpret: with nakshatra/yoga */
    RUN_TEST(test_interpret_with_nakshatra_includes);
    RUN_TEST(test_interpret_with_yoga_includes);
    RUN_TEST(test_interpret_with_both_nakshatra_and_yoga);

    /* hndi_interpret: invalid tithi */
    RUN_TEST(test_interpret_invalid_tithi_glyph);
    RUN_TEST(test_interpret_invalid_tithi_glance);

    return UNITY_END();
}
