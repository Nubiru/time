#include "../unity/unity.h"
#include "../../src/ui/daily_hindu_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2451545.0 = 2000-01-01 (J2000.0)
 * sun_lon=280.0, moon_lon=60.0 */
#define TEST_JD       2451545.0
#define TEST_SUN_LON  280.0
#define TEST_MOON_LON 60.0

/* --- Tithi --- */

void test_tithi_number_in_range(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_TRUE(layout.tithi_number >= 1 && layout.tithi_number <= 30);
}

void test_tithi_name_not_null(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_NOT_NULL(layout.tithi_name);
    TEST_ASSERT_TRUE(strlen(layout.tithi_name) > 0);
}

void test_paksha_valid(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_NOT_NULL(layout.paksha);
    /* Must be either Shukla or Krishna */
    TEST_ASSERT_TRUE(strcmp(layout.paksha, "Shukla") == 0 ||
                     strcmp(layout.paksha, "Krishna") == 0);
}

/* --- Vara --- */

void test_vara_number_in_range(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_TRUE(layout.vara_number >= 0 && layout.vara_number <= 6);
}

void test_vara_name_not_null(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_NOT_NULL(layout.vara_name);
    TEST_ASSERT_TRUE(strlen(layout.vara_name) > 0);
}

void test_vara_deity_not_null(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_NOT_NULL(layout.vara_deity);
    TEST_ASSERT_TRUE(strlen(layout.vara_deity) > 0);
}

/* --- Nakshatra --- */

void test_nakshatra_index_in_range(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_TRUE(layout.nakshatra_index >= 0 && layout.nakshatra_index <= 26);
}

/* --- Yoga --- */

void test_yoga_number_in_range(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_TRUE(layout.yoga_number >= 0 && layout.yoga_number <= 26);
}

void test_yoga_name_not_null(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_NOT_NULL(layout.yoga_name);
    TEST_ASSERT_TRUE(strlen(layout.yoga_name) > 0);
}

void test_yoga_quality_valid(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_NOT_NULL(layout.yoga_quality);
    TEST_ASSERT_TRUE(strlen(layout.yoga_quality) > 0);
}

/* --- Karana --- */

void test_karana_number_in_range(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_TRUE(layout.karana_number >= 0 && layout.karana_number <= 10);
}

void test_karana_name_not_null(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_NOT_NULL(layout.karana_name);
    TEST_ASSERT_TRUE(strlen(layout.karana_name) > 0);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_tithi_slot(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.tithi_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.tithi_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.tithi_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.tithi_slot.h);
}

void test_vara_slot(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.vara_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.32f, layout.vara_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.vara_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.vara_slot.h);
}

void test_nakshatra_slot(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.nakshatra_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.46f, layout.nakshatra_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.nakshatra_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.nakshatra_slot.h);
}

void test_yoga_slot(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.yoga_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.60f, layout.yoga_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.yoga_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.yoga_slot.h);
}

void test_karana_slot(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.karana_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.74f, layout.karana_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.karana_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.karana_slot.h);
}

void test_all_slots_same_width(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.tithi_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.vara_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.nakshatra_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.yoga_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.karana_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_tithi_name_accessor(void)
{
    daily_hindu_layout_t layout = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    const char *name = daily_hindu_tithi_name(&layout);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_tithi_name_accessor_null(void)
{
    TEST_ASSERT_EQUAL_STRING("?", daily_hindu_tithi_name(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_hindu_layout_t a = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    daily_hindu_layout_t b = daily_hindu_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_EQUAL_INT(a.tithi_number, b.tithi_number);
    TEST_ASSERT_EQUAL_INT(a.vara_number, b.vara_number);
    TEST_ASSERT_EQUAL_INT(a.nakshatra_index, b.nakshatra_index);
    TEST_ASSERT_EQUAL_INT(a.yoga_number, b.yoga_number);
    TEST_ASSERT_EQUAL_INT(a.karana_number, b.karana_number);
    TEST_ASSERT_EQUAL_STRING(a.tithi_name, b.tithi_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_longitudes(void)
{
    daily_hindu_layout_t a = daily_hindu_compute(TEST_JD, 280.0, 60.0);
    daily_hindu_layout_t b = daily_hindu_compute(TEST_JD, 280.0, 180.0);
    /* Different moon longitudes should yield different nakshatra or tithi */
    TEST_ASSERT_TRUE(a.nakshatra_index != b.nakshatra_index ||
                     a.tithi_number != b.tithi_number);
}

/* --- Different date test --- */

void test_different_date(void)
{
    /* JD 2460676.5 = 2025-01-01, different sun/moon positions */
    daily_hindu_layout_t layout = daily_hindu_compute(2460676.5, 280.0, 120.0);
    TEST_ASSERT_NOT_NULL(layout.tithi_name);
    TEST_ASSERT_NOT_NULL(layout.vara_name);
    TEST_ASSERT_TRUE(layout.tithi_number >= 1 && layout.tithi_number <= 30);
}

int main(void)
{
    UNITY_BEGIN();
    /* Tithi */
    RUN_TEST(test_tithi_number_in_range);
    RUN_TEST(test_tithi_name_not_null);
    RUN_TEST(test_paksha_valid);
    /* Vara */
    RUN_TEST(test_vara_number_in_range);
    RUN_TEST(test_vara_name_not_null);
    RUN_TEST(test_vara_deity_not_null);
    /* Nakshatra */
    RUN_TEST(test_nakshatra_index_in_range);
    /* Yoga */
    RUN_TEST(test_yoga_number_in_range);
    RUN_TEST(test_yoga_name_not_null);
    RUN_TEST(test_yoga_quality_valid);
    /* Karana */
    RUN_TEST(test_karana_number_in_range);
    RUN_TEST(test_karana_name_not_null);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_tithi_slot);
    RUN_TEST(test_vara_slot);
    RUN_TEST(test_nakshatra_slot);
    RUN_TEST(test_yoga_slot);
    RUN_TEST(test_karana_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_tithi_name_accessor);
    RUN_TEST(test_tithi_name_accessor_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_longitudes);
    /* Different date */
    RUN_TEST(test_different_date);
    return UNITY_END();
}
