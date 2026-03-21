#include "../unity/unity.h"
#include "../../src/ui/daily_buddhist_layout.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 */
#define TEST_JD 2460676.5

/* --- Buddhist Era --- */

void test_be_year_positive(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.be_year > 0);
}

void test_be_year_gt_ce(void)
{
    /* BE = CE + 543, so for 2025 it should be 2568 */
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(2568, layout.be_year);
}

/* --- Uposatha --- */

void test_uposatha_type_in_range(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.uposatha_type >= 0 && layout.uposatha_type <= 4);
}

void test_uposatha_name_not_null(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.uposatha_name);
}

void test_uposatha_desc_not_null(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.uposatha_desc);
}

void test_moon_age_non_negative(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.moon_age_days >= 0.0);
}

void test_illumination_range(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.illumination >= 0.0 && layout.illumination <= 1.0);
}

/* --- Month --- */

void test_month_number_in_range(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.month_number >= 1 && layout.month_number <= 12);
}

void test_month_name_not_null(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.month_name);
    TEST_ASSERT_TRUE(strlen(layout.month_name) > 0);
}

/* --- Vassa --- */

void test_vassa_is_bool(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.is_vassa == 0 || layout.is_vassa == 1);
}

void test_january_not_vassa(void)
{
    /* Vassa is roughly July-October, so January should not be vassa */
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(0, layout.is_vassa);
}

/* --- Slot positions --- */

void test_title_slot(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_era_slot(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.era_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.era_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.era_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.era_slot.h);
}

void test_month_slot(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.32f, layout.month_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.month_slot.h);
}

void test_uposatha_slot(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.uposatha_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.46f, layout.uposatha_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.uposatha_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.uposatha_slot.h);
}

void test_moon_slot(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.moon_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.60f, layout.moon_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.moon_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.moon_slot.h);
}

void test_vassa_slot(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.vassa_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.74f, layout.vassa_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.vassa_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.vassa_slot.h);
}

void test_all_slots_same_width(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.era_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.month_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.uposatha_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.moon_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.vassa_slot.w);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- Accessor --- */

void test_is_observance_accessor(void)
{
    daily_buddhist_layout_t layout = daily_buddhist_compute(TEST_JD);
    int obs = daily_buddhist_is_observance(&layout);
    TEST_ASSERT_TRUE(obs == 0 || obs == 1);
}

void test_is_observance_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, daily_buddhist_is_observance(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    daily_buddhist_layout_t a = daily_buddhist_compute(TEST_JD);
    daily_buddhist_layout_t b = daily_buddhist_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.be_year, b.be_year);
    TEST_ASSERT_EQUAL_INT(a.uposatha_type, b.uposatha_type);
    TEST_ASSERT_EQUAL_INT(a.month_number, b.month_number);
    TEST_ASSERT_EQUAL_STRING(a.month_name, b.month_name);
    TEST_ASSERT_EQUAL_INT(a.is_vassa, b.is_vassa);
    TEST_ASSERT_FLOAT_WITHIN(0.001, a.moon_age_days, b.moon_age_days);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.card_x, b.card_x);
}

void test_purity_different_dates(void)
{
    daily_buddhist_layout_t a = daily_buddhist_compute(2460676.5);
    daily_buddhist_layout_t b = daily_buddhist_compute(2460706.5);
    /* 30 days apart — moon age or month should differ */
    TEST_ASSERT_TRUE(fabs(a.moon_age_days - b.moon_age_days) > 0.1 ||
                     a.month_number != b.month_number);
}

/* --- Different era test --- */

void test_j2000_era(void)
{
    /* JD 2451545.0 = 2000-01-01, BE should be 2543 */
    daily_buddhist_layout_t layout = daily_buddhist_compute(2451545.0);
    TEST_ASSERT_EQUAL_INT(2543, layout.be_year);
    TEST_ASSERT_NOT_NULL(layout.month_name);
}

int main(void)
{
    UNITY_BEGIN();
    /* Buddhist Era */
    RUN_TEST(test_be_year_positive);
    RUN_TEST(test_be_year_gt_ce);
    /* Uposatha */
    RUN_TEST(test_uposatha_type_in_range);
    RUN_TEST(test_uposatha_name_not_null);
    RUN_TEST(test_uposatha_desc_not_null);
    RUN_TEST(test_moon_age_non_negative);
    RUN_TEST(test_illumination_range);
    /* Month */
    RUN_TEST(test_month_number_in_range);
    RUN_TEST(test_month_name_not_null);
    /* Vassa */
    RUN_TEST(test_vassa_is_bool);
    RUN_TEST(test_january_not_vassa);
    /* Slot positions */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_era_slot);
    RUN_TEST(test_month_slot);
    RUN_TEST(test_uposatha_slot);
    RUN_TEST(test_moon_slot);
    RUN_TEST(test_vassa_slot);
    RUN_TEST(test_all_slots_same_width);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* Accessor */
    RUN_TEST(test_is_observance_accessor);
    RUN_TEST(test_is_observance_null);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_dates);
    /* Different era */
    RUN_TEST(test_j2000_era);
    return UNITY_END();
}
