#include "../unity/unity.h"
#include "../../src/ui/daily_japanese_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 Gregorian = Reiwa 7 */
#define TEST_JD 2460676.5
/* Solar longitude near Jan 1 ~ 280 degrees (Capricorn) */
#define TEST_SUN_LON 280.0

/* --- Date conversion --- */

void test_era_reiwa(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_EQUAL_INT(4, layout.date.era_index); /* JP_ERA_REIWA = 4 */
}

void test_era_year(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_EQUAL_INT(7, layout.date.era_year); /* Reiwa 7 = 2025 */
}

void test_greg_year(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_EQUAL_INT(2025, layout.date.greg_year);
}

void test_era_name(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_EQUAL_STRING("Reiwa", layout.era_name);
}

void test_era_kanji(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_NOT_NULL(layout.era_kanji);
    TEST_ASSERT_TRUE(strlen(layout.era_kanji) > 0);
}

/* --- Rokuyo --- */

void test_rokuyo_valid(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_TRUE(layout.rokuyo >= 0 && layout.rokuyo <= 5);
}

void test_rokuyo_name_not_null(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_NOT_NULL(layout.rokuyo_name);
    TEST_ASSERT_TRUE(strlen(layout.rokuyo_name) > 0);
}

void test_rokuyo_meaning_not_null(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_NOT_NULL(layout.rokuyo_meaning);
    TEST_ASSERT_TRUE(strlen(layout.rokuyo_meaning) > 0);
}

/* --- Sekki --- */

void test_sekki_valid(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_TRUE(layout.sekki >= 0 && layout.sekki <= 23);
}

void test_sekki_name_not_null(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_NOT_NULL(layout.sekki_name);
    TEST_ASSERT_TRUE(strlen(layout.sekki_name) > 0);
}

void test_sekki_kanji_not_null(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_NOT_NULL(layout.sekki_kanji);
    TEST_ASSERT_TRUE(strlen(layout.sekki_kanji) > 0);
}

/* --- Zodiac animal --- */

void test_animal_valid(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_TRUE(layout.animal >= 0 && layout.animal <= 11);
}

void test_animal_name_not_null(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_NOT_NULL(layout.animal_name);
    TEST_ASSERT_TRUE(strlen(layout.animal_name) > 0);
}

void test_animal_kanji_not_null(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_NOT_NULL(layout.animal_kanji);
    TEST_ASSERT_TRUE(strlen(layout.animal_kanji) > 0);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
}

void test_date_slot(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.date_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.date_slot.y);
}

void test_rokuyo_slot(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.rokuyo_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.34f, layout.rokuyo_slot.y);
}

void test_sekki_slot(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.sekki_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.sekki_slot.y);
}

void test_animal_slot(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.animal_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.66f, layout.animal_slot.y);
}

void test_all_slots_same_width(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.date_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.rokuyo_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.sekki_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.animal_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessors --- */

void test_rokuyo_accessor(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_NOT_NULL(daily_japanese_rokuyo(&layout));
    TEST_ASSERT_TRUE(strlen(daily_japanese_rokuyo(&layout)) > 0);
}

void test_rokuyo_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_japanese_rokuyo(NULL));
}

void test_sekki_accessor(void)
{
    daily_japanese_layout_t layout = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_NOT_NULL(daily_japanese_sekki(&layout));
    TEST_ASSERT_TRUE(strlen(daily_japanese_sekki(&layout)) > 0);
}

void test_sekki_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_japanese_sekki(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_japanese_layout_t a = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    daily_japanese_layout_t b = daily_japanese_compute(TEST_JD, TEST_SUN_LON);
    TEST_ASSERT_EQUAL_INT(a.date.era_index, b.date.era_index);
    TEST_ASSERT_EQUAL_INT(a.date.era_year, b.date.era_year);
    TEST_ASSERT_EQUAL_INT(a.rokuyo, b.rokuyo);
    TEST_ASSERT_EQUAL_INT(a.sekki, b.sekki);
    TEST_ASSERT_EQUAL_INT(a.animal, b.animal);
    TEST_ASSERT_EQUAL_STRING(a.era_name, b.era_name);
}

void test_purity_different_dates(void)
{
    daily_japanese_layout_t a = daily_japanese_compute(2460676.5, 280.0);
    daily_japanese_layout_t b = daily_japanese_compute(2460706.5, 310.0);
    TEST_ASSERT_TRUE(a.date.day != b.date.day ||
                     a.date.month != b.date.month ||
                     a.sekki != b.sekki);
}

int main(void)
{
    UNITY_BEGIN();
    /* Date conversion */
    RUN_TEST(test_era_reiwa);
    RUN_TEST(test_era_year);
    RUN_TEST(test_greg_year);
    RUN_TEST(test_era_name);
    RUN_TEST(test_era_kanji);
    /* Rokuyo */
    RUN_TEST(test_rokuyo_valid);
    RUN_TEST(test_rokuyo_name_not_null);
    RUN_TEST(test_rokuyo_meaning_not_null);
    /* Sekki */
    RUN_TEST(test_sekki_valid);
    RUN_TEST(test_sekki_name_not_null);
    RUN_TEST(test_sekki_kanji_not_null);
    /* Zodiac animal */
    RUN_TEST(test_animal_valid);
    RUN_TEST(test_animal_name_not_null);
    RUN_TEST(test_animal_kanji_not_null);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_date_slot);
    RUN_TEST(test_rokuyo_slot);
    RUN_TEST(test_sekki_slot);
    RUN_TEST(test_animal_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessors */
    RUN_TEST(test_rokuyo_accessor);
    RUN_TEST(test_rokuyo_accessor_null);
    RUN_TEST(test_sekki_accessor);
    RUN_TEST(test_sekki_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    return UNITY_END();
}
