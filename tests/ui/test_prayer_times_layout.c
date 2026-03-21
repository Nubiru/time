#include "../unity/unity.h"
#include "../../src/ui/prayer_times_layout.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Test location: Merida, Mexico (Gabriel's area) */
#define TEST_LAT   21.4225
#define TEST_LON  -89.1667
#define TEST_JD_NOON 2460676.5   /* 2025-01-01 noon UTC */
#define TEST_METHOD  0           /* MWL */

/* Helper: compute layout once for this test file */
static prayer_times_layout_t test_layout(void)
{
    /* current_jd slightly after noon, so Dhuhr should be around or before */
    return prayer_times_layout_compute(TEST_JD_NOON, TEST_LAT, TEST_LON,
                                       TEST_METHOD, TEST_JD_NOON + 0.1);
}

/* --- Structure --- */

void test_prayer_count(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_EQUAL_INT(6, layout.prayer_count);
}

void test_prayer_indices(void)
{
    prayer_times_layout_t layout = test_layout();
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.prayers[i].index);
    }
}

/* --- Prayer names --- */

void test_prayer_name_fajr(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_EQUAL_STRING("Fajr", layout.prayers[0].name);
}

void test_prayer_name_sunrise(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_EQUAL_STRING("Sunrise", layout.prayers[1].name);
}

void test_prayer_name_dhuhr(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_EQUAL_STRING("Dhuhr", layout.prayers[2].name);
}

void test_prayer_name_asr(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_EQUAL_STRING("Asr", layout.prayers[3].name);
}

void test_prayer_name_maghrib(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_EQUAL_STRING("Maghrib", layout.prayers[4].name);
}

void test_prayer_name_isha(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_EQUAL_STRING("Isha", layout.prayers[5].name);
}

/* --- All names populated --- */

void test_all_names_not_null(void)
{
    prayer_times_layout_t layout = test_layout();
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(layout.prayers[i].name);
        TEST_ASSERT_TRUE(strlen(layout.prayers[i].name) > 0);
    }
}

/* --- Slot positions --- */

void test_slots_stacked_vertically(void)
{
    prayer_times_layout_t layout = test_layout();
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        float expected_y = 0.18f + (float)i * 0.12f;
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected_y,
                                 layout.prayers[i].slot.y);
    }
}

void test_slots_same_x(void)
{
    prayer_times_layout_t layout = test_layout();
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.prayers[i].slot.x);
    }
}

void test_slots_same_width(void)
{
    prayer_times_layout_t layout = test_layout();
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.prayers[i].slot.w);
    }
}

void test_slots_same_height(void)
{
    prayer_times_layout_t layout = test_layout();
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.prayers[i].slot.h);
    }
}

/* --- Title and method slots --- */

void test_title_slot(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.03f, layout.title_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.title_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.title_slot.h);
}

void test_method_slot(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.method_slot.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.method_slot.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.method_slot.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.method_slot.h);
}

/* --- Method name --- */

void test_method_name_not_null(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_NOT_NULL(layout.method_name);
    TEST_ASSERT_TRUE(strlen(layout.method_name) > 0);
}

/* --- Card frame --- */

void test_card_bounds(void)
{
    prayer_times_layout_t layout = test_layout();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.card_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.02f, layout.card_y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, layout.card_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.96f, layout.card_h);
}

/* --- JD values populated --- */

void test_prayer_jds_populated(void)
{
    prayer_times_layout_t layout = test_layout();
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        /* All JD values should be near the test date (within 1 day) */
        TEST_ASSERT_FLOAT_WITHIN(1.0, TEST_JD_NOON, layout.prayers[i].jd);
    }
}

void test_prayer_jds_ordered(void)
{
    /* Prayer times should be in chronological order */
    prayer_times_layout_t layout = test_layout();
    for (int i = 1; i < PT_PRAYER_COUNT; i++) {
        TEST_ASSERT_TRUE(layout.prayers[i].jd >= layout.prayers[i - 1].jd);
    }
}

/* --- Next prayer marking --- */

void test_exactly_one_is_next(void)
{
    prayer_times_layout_t layout = test_layout();
    int count = 0;
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        if (layout.prayers[i].is_next) {
            count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- Accessor --- */

void test_next_name_not_null(void)
{
    prayer_times_layout_t layout = test_layout();
    const char *name = prayer_times_next_name(&layout);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_next_name_null_layout(void)
{
    TEST_ASSERT_EQUAL_STRING("?", prayer_times_next_name(NULL));
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    prayer_times_layout_t a = test_layout();
    prayer_times_layout_t b = test_layout();
    TEST_ASSERT_EQUAL_INT(a.prayer_count, b.prayer_count);
    for (int i = 0; i < PT_PRAYER_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.0001, a.prayers[i].jd, b.prayers[i].jd);
        TEST_ASSERT_EQUAL_INT(a.prayers[i].is_next, b.prayers[i].is_next);
    }
}

/* --- Invalid method clamp --- */

void test_invalid_method_clamps(void)
{
    /* Method out of range should not crash */
    prayer_times_layout_t layout = prayer_times_layout_compute(
        TEST_JD_NOON, TEST_LAT, TEST_LON, 99, TEST_JD_NOON);
    TEST_ASSERT_EQUAL_INT(6, layout.prayer_count);
    TEST_ASSERT_NOT_NULL(layout.method_name);
}

int main(void)
{
    UNITY_BEGIN();
    /* Structure */
    RUN_TEST(test_prayer_count);
    RUN_TEST(test_prayer_indices);
    /* Prayer names */
    RUN_TEST(test_prayer_name_fajr);
    RUN_TEST(test_prayer_name_sunrise);
    RUN_TEST(test_prayer_name_dhuhr);
    RUN_TEST(test_prayer_name_asr);
    RUN_TEST(test_prayer_name_maghrib);
    RUN_TEST(test_prayer_name_isha);
    RUN_TEST(test_all_names_not_null);
    /* Slots */
    RUN_TEST(test_slots_stacked_vertically);
    RUN_TEST(test_slots_same_x);
    RUN_TEST(test_slots_same_width);
    RUN_TEST(test_slots_same_height);
    /* Title and method */
    RUN_TEST(test_title_slot);
    RUN_TEST(test_method_slot);
    RUN_TEST(test_method_name_not_null);
    /* Card frame */
    RUN_TEST(test_card_bounds);
    /* JD values */
    RUN_TEST(test_prayer_jds_populated);
    RUN_TEST(test_prayer_jds_ordered);
    /* Next prayer */
    RUN_TEST(test_exactly_one_is_next);
    RUN_TEST(test_next_name_not_null);
    RUN_TEST(test_next_name_null_layout);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    /* Edge cases */
    RUN_TEST(test_invalid_method_clamps);
    return UNITY_END();
}
