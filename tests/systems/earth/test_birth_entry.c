/* test_birth_entry.c — Tests for birth date entry state machine.
 * TDD: RED phase first, then GREEN with implementation. */

#include "unity.h"
#include "systems/earth/birth_entry.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Creation ===== */

void test_create_sets_date_step(void) {
    be_entry_t e = be_create(2026, 3, 19);
    TEST_ASSERT_EQUAL_INT(BE_STEP_DATE, e.step);
}

void test_create_sets_today_date(void) {
    be_entry_t e = be_create(2026, 3, 19);
    TEST_ASSERT_EQUAL_INT(2026, e.date.year);
    TEST_ASSERT_EQUAL_INT(3, e.date.month);
    TEST_ASSERT_EQUAL_INT(19, e.date.day);
}

void test_create_focus_on_day(void) {
    be_entry_t e = be_create(2026, 3, 19);
    TEST_ASSERT_EQUAL_INT(BE_FIELD_DAY, e.date.focus);
}

void test_create_date_valid(void) {
    be_entry_t e = be_create(2026, 3, 19);
    TEST_ASSERT_EQUAL_INT(1, e.date.valid);
}

void test_create_location_not_set(void) {
    be_entry_t e = be_create(2026, 3, 19);
    TEST_ASSERT_EQUAL_INT(0, e.location.set);
    TEST_ASSERT_EQUAL_INT(0, e.location.skipped);
}

void test_create_location_zeroed(void) {
    be_entry_t e = be_create(2026, 3, 19);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)e.location.latitude);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)e.location.longitude);
}

/* ===== Leap Year ===== */

void test_leap_year_2024(void) {
    TEST_ASSERT_EQUAL_INT(1, be_is_leap_year(2024));
}

void test_leap_year_2000(void) {
    TEST_ASSERT_EQUAL_INT(1, be_is_leap_year(2000));
}

void test_not_leap_year_1900(void) {
    TEST_ASSERT_EQUAL_INT(0, be_is_leap_year(1900));
}

void test_not_leap_year_2023(void) {
    TEST_ASSERT_EQUAL_INT(0, be_is_leap_year(2023));
}

void test_leap_year_2100_not_leap(void) {
    TEST_ASSERT_EQUAL_INT(0, be_is_leap_year(2100));
}

/* ===== Days In Month ===== */

void test_days_in_january(void) {
    TEST_ASSERT_EQUAL_INT(31, be_days_in_month(2026, 1));
}

void test_days_in_february_normal(void) {
    TEST_ASSERT_EQUAL_INT(28, be_days_in_month(2023, 2));
}

void test_days_in_february_leap(void) {
    TEST_ASSERT_EQUAL_INT(29, be_days_in_month(2024, 2));
}

void test_days_in_april(void) {
    TEST_ASSERT_EQUAL_INT(30, be_days_in_month(2026, 4));
}

void test_days_in_june(void) {
    TEST_ASSERT_EQUAL_INT(30, be_days_in_month(2026, 6));
}

void test_days_in_september(void) {
    TEST_ASSERT_EQUAL_INT(30, be_days_in_month(2026, 9));
}

void test_days_in_november(void) {
    TEST_ASSERT_EQUAL_INT(30, be_days_in_month(2026, 11));
}

void test_days_in_december(void) {
    TEST_ASSERT_EQUAL_INT(31, be_days_in_month(2026, 12));
}

void test_days_in_month_invalid_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, be_days_in_month(2026, 0));
}

void test_days_in_month_invalid_thirteen(void) {
    TEST_ASSERT_EQUAL_INT(0, be_days_in_month(2026, 13));
}

/* ===== Date Validation ===== */

void test_valid_date_normal(void) {
    be_date_t d = { .day = 15, .month = 6, .year = 1990, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(1, be_date_valid(&d));
}

void test_invalid_date_feb_30(void) {
    be_date_t d = { .day = 30, .month = 2, .year = 2026, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(0, be_date_valid(&d));
}

void test_invalid_date_apr_31(void) {
    be_date_t d = { .day = 31, .month = 4, .year = 2026, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(0, be_date_valid(&d));
}

void test_valid_date_feb_29_leap(void) {
    be_date_t d = { .day = 29, .month = 2, .year = 2024, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(1, be_date_valid(&d));
}

void test_invalid_date_feb_29_nonleap(void) {
    be_date_t d = { .day = 29, .month = 2, .year = 2023, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(0, be_date_valid(&d));
}

void test_invalid_date_day_zero(void) {
    be_date_t d = { .day = 0, .month = 3, .year = 2026, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(0, be_date_valid(&d));
}

void test_invalid_date_month_zero(void) {
    be_date_t d = { .day = 15, .month = 0, .year = 2026, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(0, be_date_valid(&d));
}

void test_invalid_date_month_thirteen(void) {
    be_date_t d = { .day = 15, .month = 13, .year = 2026, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(0, be_date_valid(&d));
}

void test_invalid_date_year_below_range(void) {
    be_date_t d = { .day = 15, .month = 6, .year = 1899, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(0, be_date_valid(&d));
}

void test_invalid_date_year_above_range(void) {
    be_date_t d = { .day = 15, .month = 6, .year = 2101, .focus = BE_FIELD_DAY, .valid = 0 };
    TEST_ASSERT_EQUAL_INT(0, be_date_valid(&d));
}

/* ===== Focus ===== */

void test_focus_set_month(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_focus(e, BE_FIELD_MONTH);
    TEST_ASSERT_EQUAL_INT(BE_FIELD_MONTH, e.date.focus);
}

void test_focus_set_year(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_focus(e, BE_FIELD_YEAR);
    TEST_ASSERT_EQUAL_INT(BE_FIELD_YEAR, e.date.focus);
}

void test_focus_set_day(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_focus(e, BE_FIELD_YEAR);
    e = be_focus(e, BE_FIELD_DAY);
    TEST_ASSERT_EQUAL_INT(BE_FIELD_DAY, e.date.focus);
}

/* ===== Spin — Day ===== */

void test_spin_day_up(void) {
    be_entry_t e = be_create(2026, 3, 15);
    e = be_focus(e, BE_FIELD_DAY);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(16, e.date.day);
}

void test_spin_day_down(void) {
    be_entry_t e = be_create(2026, 3, 15);
    e = be_focus(e, BE_FIELD_DAY);
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(14, e.date.day);
}

void test_spin_day_wrap_up(void) {
    be_entry_t e = be_create(2026, 3, 31);
    e = be_focus(e, BE_FIELD_DAY);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(1, e.date.day);
}

void test_spin_day_wrap_down(void) {
    be_entry_t e = be_create(2026, 3, 1);
    e = be_focus(e, BE_FIELD_DAY);
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(31, e.date.day);
}

void test_spin_day_wrap_down_feb(void) {
    be_entry_t e = be_create(2024, 2, 1);
    e = be_focus(e, BE_FIELD_DAY);
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(29, e.date.day);
}

void test_spin_day_wrap_down_feb_nonleap(void) {
    be_entry_t e = be_create(2023, 2, 1);
    e = be_focus(e, BE_FIELD_DAY);
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(28, e.date.day);
}

/* ===== Spin — Month ===== */

void test_spin_month_up(void) {
    be_entry_t e = be_create(2026, 3, 15);
    e = be_focus(e, BE_FIELD_MONTH);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(4, e.date.month);
}

void test_spin_month_down(void) {
    be_entry_t e = be_create(2026, 3, 15);
    e = be_focus(e, BE_FIELD_MONTH);
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(2, e.date.month);
}

void test_spin_month_wrap_up(void) {
    be_entry_t e = be_create(2026, 12, 15);
    e = be_focus(e, BE_FIELD_MONTH);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(1, e.date.month);
}

void test_spin_month_wrap_down(void) {
    be_entry_t e = be_create(2026, 1, 15);
    e = be_focus(e, BE_FIELD_MONTH);
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(12, e.date.month);
}

void test_spin_month_clamps_day(void) {
    /* March 31 -> spin month up -> April only has 30 days -> day clamps to 30 */
    be_entry_t e = be_create(2026, 3, 31);
    e = be_focus(e, BE_FIELD_MONTH);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(4, e.date.month);
    TEST_ASSERT_EQUAL_INT(30, e.date.day);
}

void test_spin_month_clamps_day_to_feb(void) {
    /* Jan 31 -> spin month up -> February has 28 days -> day clamps to 28 */
    be_entry_t e = be_create(2023, 1, 31);
    e = be_focus(e, BE_FIELD_MONTH);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(2, e.date.month);
    TEST_ASSERT_EQUAL_INT(28, e.date.day);
}

void test_spin_month_clamps_day_to_feb_leap(void) {
    /* Jan 31 -> spin month up -> February leap has 29 days -> day clamps to 29 */
    be_entry_t e = be_create(2024, 1, 31);
    e = be_focus(e, BE_FIELD_MONTH);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(2, e.date.month);
    TEST_ASSERT_EQUAL_INT(29, e.date.day);
}

/* ===== Spin — Year ===== */

void test_spin_year_up(void) {
    be_entry_t e = be_create(2026, 3, 15);
    e = be_focus(e, BE_FIELD_YEAR);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(2027, e.date.year);
}

void test_spin_year_down(void) {
    be_entry_t e = be_create(2026, 3, 15);
    e = be_focus(e, BE_FIELD_YEAR);
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(2025, e.date.year);
}

void test_spin_year_clamp_at_max(void) {
    be_entry_t e = be_create(2100, 3, 15);
    e = be_focus(e, BE_FIELD_YEAR);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(2100, e.date.year);
}

void test_spin_year_clamp_at_min(void) {
    be_entry_t e = be_create(1900, 3, 15);
    e = be_focus(e, BE_FIELD_YEAR);
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(1900, e.date.year);
}

void test_spin_year_clamps_feb_29(void) {
    /* 2024 Feb 29 -> spin year up -> 2025 is not leap -> day clamps to 28 */
    be_entry_t e = be_create(2024, 2, 29);
    e = be_focus(e, BE_FIELD_YEAR);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(2025, e.date.year);
    TEST_ASSERT_EQUAL_INT(28, e.date.day);
}

/* ===== Spin updates valid flag ===== */

void test_spin_updates_valid_flag(void) {
    be_entry_t e = be_create(2026, 3, 15);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(1, e.date.valid);
}

/* ===== Set Date ===== */

void test_set_date_valid(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_set_date(e, 1990, 7, 4);
    TEST_ASSERT_EQUAL_INT(1990, e.date.year);
    TEST_ASSERT_EQUAL_INT(7, e.date.month);
    TEST_ASSERT_EQUAL_INT(4, e.date.day);
    TEST_ASSERT_EQUAL_INT(1, e.date.valid);
}

void test_set_date_invalid(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_set_date(e, 2026, 2, 30);
    TEST_ASSERT_EQUAL_INT(2026, e.date.year);
    TEST_ASSERT_EQUAL_INT(2, e.date.month);
    TEST_ASSERT_EQUAL_INT(30, e.date.day);
    TEST_ASSERT_EQUAL_INT(0, e.date.valid);
}

/* ===== Step Transitions ===== */

void test_confirm_date_valid_advances(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_LOCATION, e.step);
}

void test_confirm_date_invalid_stays(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_set_date(e, 2026, 2, 30);
    e = be_confirm_date(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_DATE, e.step);
}

void test_confirm_date_only_from_date_step(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_LOCATION, e.step);
    /* Confirm again from LOCATION should not change step */
    e = be_confirm_date(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_LOCATION, e.step);
}

void test_set_location_advances_to_ready(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_set_location(e, 40.7128, -74.0060);
    TEST_ASSERT_EQUAL_INT(BE_STEP_READY, e.step);
    TEST_ASSERT_EQUAL_INT(1, e.location.set);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 40.7128f, (float)e.location.latitude);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -74.006f, (float)e.location.longitude);
}

void test_set_location_only_from_location_step(void) {
    be_entry_t e = be_create(2026, 3, 19);
    /* Still on DATE step */
    e = be_set_location(e, 40.7128, -74.0060);
    TEST_ASSERT_EQUAL_INT(BE_STEP_DATE, e.step);
    TEST_ASSERT_EQUAL_INT(0, e.location.set);
}

void test_skip_location_advances_to_ready(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_skip_location(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_READY, e.step);
    TEST_ASSERT_EQUAL_INT(1, e.location.skipped);
    TEST_ASSERT_EQUAL_INT(0, e.location.set);
}

void test_skip_location_only_from_location_step(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_skip_location(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_DATE, e.step);
    TEST_ASSERT_EQUAL_INT(0, e.location.skipped);
}

/* ===== Back Navigation ===== */

void test_back_from_location_to_date(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_LOCATION, e.step);
    e = be_back(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_DATE, e.step);
}

void test_back_from_ready_to_location(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_skip_location(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_READY, e.step);
    e = be_back(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_LOCATION, e.step);
}

void test_back_from_date_stays(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_back(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_DATE, e.step);
}

/* ===== Queries ===== */

void test_is_ready_true_at_ready(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_set_location(e, 40.0, -74.0);
    TEST_ASSERT_EQUAL_INT(1, be_is_ready(&e));
}

void test_is_ready_false_at_date(void) {
    be_entry_t e = be_create(2026, 3, 19);
    TEST_ASSERT_EQUAL_INT(0, be_is_ready(&e));
}

void test_is_ready_false_at_location(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    TEST_ASSERT_EQUAL_INT(0, be_is_ready(&e));
}

void test_step_name_date(void) {
    TEST_ASSERT_EQUAL_STRING("Date", be_step_name(BE_STEP_DATE));
}

void test_step_name_location(void) {
    TEST_ASSERT_EQUAL_STRING("Location", be_step_name(BE_STEP_LOCATION));
}

void test_step_name_ready(void) {
    TEST_ASSERT_EQUAL_STRING("Ready", be_step_name(BE_STEP_READY));
}

void test_step_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("Unknown", be_step_name(BE_STEP_COUNT));
}

/* ===== Format Date ===== */

void test_format_date_normal(void) {
    be_entry_t e = be_create(2026, 3, 19);
    char buf[64];
    int len = be_format_date(&e, buf, 64);
    TEST_ASSERT_EQUAL_STRING("19 March 2026", buf);
    TEST_ASSERT_EQUAL_INT(13, len);
}

void test_format_date_single_digit_day(void) {
    be_entry_t e = be_create(2026, 1, 5);
    char buf[64];
    int len = be_format_date(&e, buf, 64);
    TEST_ASSERT_EQUAL_STRING("05 January 2026", buf);
    TEST_ASSERT_EQUAL_INT(15, len);
}

void test_format_date_buffer_too_small(void) {
    be_entry_t e = be_create(2026, 3, 19);
    char buf[5];
    int len = be_format_date(&e, buf, 5);
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_format_date_all_months(void) {
    char buf[64];
    const char *months[] = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };
    for (int m = 1; m <= 12; m++) {
        be_entry_t e = be_create(2026, m, 15);
        be_format_date(&e, buf, 64);
        /* Verify the month name appears in the formatted string */
        TEST_ASSERT_NOT_NULL(strstr(buf, months[m - 1]));
    }
}

/* ===== Location Clamping ===== */

void test_set_location_clamps_latitude(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_set_location(e, 95.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 90.0f, (float)e.location.latitude);
}

void test_set_location_clamps_latitude_negative(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_set_location(e, -95.0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -90.0f, (float)e.location.latitude);
}

void test_set_location_clamps_longitude(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_set_location(e, 0.0, 200.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 180.0f, (float)e.location.longitude);
}

void test_set_location_clamps_longitude_negative(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_set_location(e, 0.0, -200.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, -180.0f, (float)e.location.longitude);
}

/* ===== Full Flow ===== */

void test_full_flow_with_location(void) {
    be_entry_t e = be_create(2026, 3, 19);
    TEST_ASSERT_EQUAL_INT(BE_STEP_DATE, e.step);

    /* Spin day down to 18 */
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(18, e.date.day);

    /* Focus month, spin to April */
    e = be_focus(e, BE_FIELD_MONTH);
    e = be_spin(e, 1);
    TEST_ASSERT_EQUAL_INT(4, e.date.month);

    /* Focus year, spin down */
    e = be_focus(e, BE_FIELD_YEAR);
    e = be_spin(e, -1);
    TEST_ASSERT_EQUAL_INT(2025, e.date.year);

    /* Confirm date */
    e = be_confirm_date(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_LOCATION, e.step);

    /* Set location */
    e = be_set_location(e, 34.0522, -118.2437);
    TEST_ASSERT_EQUAL_INT(BE_STEP_READY, e.step);
    TEST_ASSERT_EQUAL_INT(1, be_is_ready(&e));
}

void test_full_flow_skip_location(void) {
    be_entry_t e = be_create(1990, 7, 4);
    TEST_ASSERT_EQUAL_INT(BE_STEP_DATE, e.step);

    /* Confirm date */
    e = be_confirm_date(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_LOCATION, e.step);

    /* Skip location */
    e = be_skip_location(e);
    TEST_ASSERT_EQUAL_INT(BE_STEP_READY, e.step);
    TEST_ASSERT_EQUAL_INT(1, be_is_ready(&e));
    TEST_ASSERT_EQUAL_INT(1, e.location.skipped);
}

/* ===== Back resets location state ===== */

void test_back_from_ready_resets_location_set(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_set_location(e, 40.0, -74.0);
    TEST_ASSERT_EQUAL_INT(1, e.location.set);
    e = be_back(e);
    TEST_ASSERT_EQUAL_INT(0, e.location.set);
    TEST_ASSERT_EQUAL_INT(0, e.location.skipped);
}

void test_back_from_ready_resets_location_skipped(void) {
    be_entry_t e = be_create(2026, 3, 19);
    e = be_confirm_date(e);
    e = be_skip_location(e);
    TEST_ASSERT_EQUAL_INT(1, e.location.skipped);
    e = be_back(e);
    TEST_ASSERT_EQUAL_INT(0, e.location.skipped);
}

int main(void) {
    UNITY_BEGIN();

    /* Creation (6) */
    RUN_TEST(test_create_sets_date_step);
    RUN_TEST(test_create_sets_today_date);
    RUN_TEST(test_create_focus_on_day);
    RUN_TEST(test_create_date_valid);
    RUN_TEST(test_create_location_not_set);
    RUN_TEST(test_create_location_zeroed);

    /* Leap Year (5) */
    RUN_TEST(test_leap_year_2024);
    RUN_TEST(test_leap_year_2000);
    RUN_TEST(test_not_leap_year_1900);
    RUN_TEST(test_not_leap_year_2023);
    RUN_TEST(test_leap_year_2100_not_leap);

    /* Days In Month (10) */
    RUN_TEST(test_days_in_january);
    RUN_TEST(test_days_in_february_normal);
    RUN_TEST(test_days_in_february_leap);
    RUN_TEST(test_days_in_april);
    RUN_TEST(test_days_in_june);
    RUN_TEST(test_days_in_september);
    RUN_TEST(test_days_in_november);
    RUN_TEST(test_days_in_december);
    RUN_TEST(test_days_in_month_invalid_zero);
    RUN_TEST(test_days_in_month_invalid_thirteen);

    /* Date Validation (10) */
    RUN_TEST(test_valid_date_normal);
    RUN_TEST(test_invalid_date_feb_30);
    RUN_TEST(test_invalid_date_apr_31);
    RUN_TEST(test_valid_date_feb_29_leap);
    RUN_TEST(test_invalid_date_feb_29_nonleap);
    RUN_TEST(test_invalid_date_day_zero);
    RUN_TEST(test_invalid_date_month_zero);
    RUN_TEST(test_invalid_date_month_thirteen);
    RUN_TEST(test_invalid_date_year_below_range);
    RUN_TEST(test_invalid_date_year_above_range);

    /* Focus (3) */
    RUN_TEST(test_focus_set_month);
    RUN_TEST(test_focus_set_year);
    RUN_TEST(test_focus_set_day);

    /* Spin — Day (6) */
    RUN_TEST(test_spin_day_up);
    RUN_TEST(test_spin_day_down);
    RUN_TEST(test_spin_day_wrap_up);
    RUN_TEST(test_spin_day_wrap_down);
    RUN_TEST(test_spin_day_wrap_down_feb);
    RUN_TEST(test_spin_day_wrap_down_feb_nonleap);

    /* Spin — Month (7) */
    RUN_TEST(test_spin_month_up);
    RUN_TEST(test_spin_month_down);
    RUN_TEST(test_spin_month_wrap_up);
    RUN_TEST(test_spin_month_wrap_down);
    RUN_TEST(test_spin_month_clamps_day);
    RUN_TEST(test_spin_month_clamps_day_to_feb);
    RUN_TEST(test_spin_month_clamps_day_to_feb_leap);

    /* Spin — Year (5) */
    RUN_TEST(test_spin_year_up);
    RUN_TEST(test_spin_year_down);
    RUN_TEST(test_spin_year_clamp_at_max);
    RUN_TEST(test_spin_year_clamp_at_min);
    RUN_TEST(test_spin_year_clamps_feb_29);

    /* Spin valid flag (1) */
    RUN_TEST(test_spin_updates_valid_flag);

    /* Set Date (2) */
    RUN_TEST(test_set_date_valid);
    RUN_TEST(test_set_date_invalid);

    /* Step Transitions (7) */
    RUN_TEST(test_confirm_date_valid_advances);
    RUN_TEST(test_confirm_date_invalid_stays);
    RUN_TEST(test_confirm_date_only_from_date_step);
    RUN_TEST(test_set_location_advances_to_ready);
    RUN_TEST(test_set_location_only_from_location_step);
    RUN_TEST(test_skip_location_advances_to_ready);
    RUN_TEST(test_skip_location_only_from_location_step);

    /* Back Navigation (3) */
    RUN_TEST(test_back_from_location_to_date);
    RUN_TEST(test_back_from_ready_to_location);
    RUN_TEST(test_back_from_date_stays);

    /* Queries (7) */
    RUN_TEST(test_is_ready_true_at_ready);
    RUN_TEST(test_is_ready_false_at_date);
    RUN_TEST(test_is_ready_false_at_location);
    RUN_TEST(test_step_name_date);
    RUN_TEST(test_step_name_location);
    RUN_TEST(test_step_name_ready);
    RUN_TEST(test_step_name_invalid);

    /* Format Date (4) */
    RUN_TEST(test_format_date_normal);
    RUN_TEST(test_format_date_single_digit_day);
    RUN_TEST(test_format_date_buffer_too_small);
    RUN_TEST(test_format_date_all_months);

    /* Location Clamping (4) */
    RUN_TEST(test_set_location_clamps_latitude);
    RUN_TEST(test_set_location_clamps_latitude_negative);
    RUN_TEST(test_set_location_clamps_longitude);
    RUN_TEST(test_set_location_clamps_longitude_negative);

    /* Full Flow (2) */
    RUN_TEST(test_full_flow_with_location);
    RUN_TEST(test_full_flow_skip_location);

    /* Back resets location (2) */
    RUN_TEST(test_back_from_ready_resets_location_set);
    RUN_TEST(test_back_from_ready_resets_location_skipped);

    return UNITY_END();
}
