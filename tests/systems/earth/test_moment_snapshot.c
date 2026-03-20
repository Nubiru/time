/* test_moment_snapshot.c — Tests for Moment Snapshot module.
 * TDD: RED phase first, then GREEN with implementation. */

#include "unity.h"
#include "systems/earth/moment_snapshot.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Helper: build a full input with all systems populated ===== */

static ms_input_t full_input(void) {
    ms_input_t in = ms_default_input();
    in.jd = 2460724.5;
    in.year = 2026;
    in.month = 3;
    in.day = 20;
    in.hour = 14;
    in.minute = 30;

    /* Gregorian */
    in.weekday = "Friday";
    in.day_of_year = 79;

    /* Tzolkin */
    in.kin = 196;
    in.seal = 16;
    in.tone = 6;
    in.seal_name = "Warrior";
    in.tone_name = "Rhythmic";
    in.wavespell = 15;

    /* Astrology */
    in.sun_sign = 11;
    in.sun_sign_name = "Pisces";

    /* Chinese */
    in.animal = 3;
    in.element = 2;
    in.animal_name = "Rabbit";
    in.element_name = "Water";

    /* I Ching */
    in.hexagram = 23;
    in.hexagram_name = "Splitting Apart";

    /* Hebrew */
    in.hebrew_year = 5786;
    in.hebrew_month = 12;
    in.hebrew_day = 20;
    in.hebrew_month_name = "Adar";

    /* Islamic */
    in.islamic_year = 1447;
    in.islamic_month = 9;
    in.islamic_day = 20;
    in.islamic_month_name = "Ramadan";

    /* Buddhist */
    in.buddhist_year = 2569;

    /* Moon */
    in.moon_phase = "Waxing Gibbous";

    /* Note */
    in.note = "Spring equinox!";

    return in;
}

/* ===== ms_default_input ===== */

void test_default_input_jd_zero(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_TRUE(in.jd == 0.0);
}

void test_default_input_date_zero(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.year);
    TEST_ASSERT_EQUAL_INT(0, in.month);
    TEST_ASSERT_EQUAL_INT(0, in.day);
}

void test_default_input_time_zero(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.hour);
    TEST_ASSERT_EQUAL_INT(0, in.minute);
}

void test_default_input_weekday_null(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_NULL(in.weekday);
}

void test_default_input_kin_zero(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.kin);
    TEST_ASSERT_EQUAL_INT(-1, in.seal);
    TEST_ASSERT_EQUAL_INT(0, in.tone);
    TEST_ASSERT_NULL(in.seal_name);
    TEST_ASSERT_NULL(in.tone_name);
}

void test_default_input_sun_sign_unknown(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_EQUAL_INT(-1, in.sun_sign);
    TEST_ASSERT_NULL(in.sun_sign_name);
}

void test_default_input_chinese_unknown(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_EQUAL_INT(-1, in.animal);
    TEST_ASSERT_EQUAL_INT(-1, in.element);
    TEST_ASSERT_NULL(in.animal_name);
    TEST_ASSERT_NULL(in.element_name);
}

void test_default_input_hexagram_zero(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.hexagram);
    TEST_ASSERT_NULL(in.hexagram_name);
}

void test_default_input_hebrew_zero(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.hebrew_year);
    TEST_ASSERT_EQUAL_INT(0, in.hebrew_month);
    TEST_ASSERT_EQUAL_INT(0, in.hebrew_day);
    TEST_ASSERT_NULL(in.hebrew_month_name);
}

void test_default_input_islamic_zero(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.islamic_year);
    TEST_ASSERT_EQUAL_INT(0, in.islamic_month);
    TEST_ASSERT_EQUAL_INT(0, in.islamic_day);
    TEST_ASSERT_NULL(in.islamic_month_name);
}

void test_default_input_buddhist_zero(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.buddhist_year);
}

void test_default_input_moon_null(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_NULL(in.moon_phase);
}

void test_default_input_note_null(void) {
    ms_input_t in = ms_default_input();
    TEST_ASSERT_NULL(in.note);
}

/* ===== ms_capture — NULL input ===== */

void test_capture_null_returns_empty(void) {
    ms_snapshot_t snap = ms_capture(NULL);
    TEST_ASSERT_EQUAL_INT(0, snap.system_count);
    TEST_ASSERT_EQUAL_INT(0, snap.available_count);
    TEST_ASSERT_TRUE(snap.jd == 0.0);
}

/* ===== ms_capture — default (empty) input ===== */

void test_capture_default_has_no_systems(void) {
    ms_input_t in = ms_default_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(0, snap.available_count);
}

void test_capture_default_copies_jd(void) {
    ms_input_t in = ms_default_input();
    in.jd = 2460724.5;
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_TRUE(snap.jd == 2460724.5);
}

/* ===== ms_capture — full input ===== */

void test_capture_full_populates_all_9_systems(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(9, snap.available_count);
    TEST_ASSERT_EQUAL_INT(9, snap.system_count);
}

void test_capture_full_copies_date(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(2026, snap.year);
    TEST_ASSERT_EQUAL_INT(3, snap.month);
    TEST_ASSERT_EQUAL_INT(20, snap.day);
}

void test_capture_full_copies_time(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(14, snap.hour);
    TEST_ASSERT_EQUAL_INT(30, snap.minute);
}

void test_capture_full_copies_jd(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_TRUE(snap.jd == 2460724.5);
}

void test_capture_full_copies_note(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_STRING("Spring equinox!", snap.note);
}

void test_capture_null_note_empty(void) {
    ms_input_t in = full_input();
    in.note = NULL;
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_STRING("", snap.note);
}

/* ===== ms_capture — partial input ===== */

void test_capture_partial_tzolkin_gregorian(void) {
    ms_input_t in = ms_default_input();
    in.year = 2026;
    in.month = 3;
    in.day = 20;
    in.weekday = "Friday";
    in.day_of_year = 79;
    in.kin = 196;
    in.seal = 16;
    in.tone = 6;
    in.seal_name = "Warrior";
    in.tone_name = "Rhythmic";
    in.wavespell = 15;

    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(2, snap.available_count);
}

void test_capture_partial_only_moon(void) {
    ms_input_t in = ms_default_input();
    in.moon_phase = "Full Moon";
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(1, snap.available_count);
}

/* ===== System text: Gregorian (system_id=0) ===== */

void test_gregorian_primary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 0);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_STRING("Gregorian", sys->system_name);
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "Friday"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "March"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "20"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "2026"));
}

void test_gregorian_secondary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 0);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_NOT_NULL(strstr(sys->secondary, "79"));
}

/* ===== System text: Tzolkin (system_id=1) ===== */

void test_tzolkin_primary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 1);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_STRING("Tzolkin", sys->system_name);
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "Kin 196"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "Rhythmic"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "Warrior"));
}

void test_tzolkin_secondary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 1);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_NOT_NULL(strstr(sys->secondary, "Wavespell 15"));
    TEST_ASSERT_NOT_NULL(strstr(sys->secondary, "Tone 6"));
}

/* ===== System text: Astrology (system_id=2) ===== */

void test_astrology_primary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 2);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_STRING("Astrology", sys->system_name);
    TEST_ASSERT_EQUAL_STRING("Pisces", sys->primary);
}

/* ===== System text: Chinese (system_id=3) ===== */

void test_chinese_primary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 3);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_STRING("Chinese", sys->system_name);
    TEST_ASSERT_EQUAL_STRING("Water Rabbit", sys->primary);
}

/* ===== System text: I Ching (system_id=4) ===== */

void test_iching_primary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 4);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_STRING("I Ching", sys->system_name);
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "23"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "Splitting Apart"));
}

/* ===== System text: Hebrew (system_id=5) ===== */

void test_hebrew_primary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 5);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_STRING("Hebrew", sys->system_name);
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "Adar"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "20"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "5786"));
}

/* ===== System text: Islamic (system_id=6) ===== */

void test_islamic_primary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 6);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_STRING("Islamic", sys->system_name);
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "Ramadan"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "20"));
    TEST_ASSERT_NOT_NULL(strstr(sys->primary, "1447"));
}

/* ===== System text: Buddhist (system_id=7) ===== */

void test_buddhist_primary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 7);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_STRING("Buddhist", sys->system_name);
    TEST_ASSERT_EQUAL_STRING("BE 2569", sys->primary);
}

/* ===== System text: Moon (system_id=8) ===== */

void test_moon_primary_format(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 8);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_STRING("Moon", sys->system_name);
    TEST_ASSERT_EQUAL_STRING("Waxing Gibbous", sys->primary);
}

/* ===== ms_system — index access ===== */

void test_system_valid_index(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_system(&snap, 0);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_INT(1, sys->available);
}

void test_system_negative_index_returns_null(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_NULL(ms_system(&snap, -1));
}

void test_system_out_of_range_returns_null(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_NULL(ms_system(&snap, snap.system_count));
}

/* ===== ms_find_system — by system_id ===== */

void test_find_system_existing(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    const ms_system_state_t *sys = ms_find_system(&snap, 1);
    TEST_ASSERT_NOT_NULL(sys);
    TEST_ASSERT_EQUAL_INT(1, sys->system_id);
}

void test_find_system_absent(void) {
    ms_input_t in = ms_default_input();
    in.moon_phase = "Full Moon";
    ms_snapshot_t snap = ms_capture(&in);
    /* Only Moon (8) is populated, so system_id 0 should be absent */
    TEST_ASSERT_NULL(ms_find_system(&snap, 0));
}

/* ===== ms_available_count ===== */

void test_available_count_full(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(9, ms_available_count(&snap));
}

void test_available_count_empty(void) {
    ms_input_t in = ms_default_input();
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(0, ms_available_count(&snap));
}

void test_available_count_partial(void) {
    ms_input_t in = ms_default_input();
    in.buddhist_year = 2569;
    in.moon_phase = "New Moon";
    in.hexagram = 1;
    in.hexagram_name = "The Creative";
    ms_snapshot_t snap = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(3, ms_available_count(&snap));
}

/* ===== ms_format ===== */

void test_format_nonempty(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[2048];
    int n = ms_format(&snap, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
}

void test_format_contains_date(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[2048];
    ms_format(&snap, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "2026"));
}

void test_format_contains_system(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[2048];
    ms_format(&snap, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Tzolkin"));
}

void test_format_contains_note(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[2048];
    ms_format(&snap, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Spring equinox!"));
}

void test_format_small_buffer(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[16];
    int n = ms_format(&snap, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(n <= 15);
    TEST_ASSERT_EQUAL_UINT8('\0', buf[n]);
}

void test_format_zero_buffer(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[1];
    int n = ms_format(&snap, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ===== ms_format_compact ===== */

void test_format_compact_nonempty(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[512];
    int n = ms_format_compact(&snap, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
}

void test_format_compact_contains_date(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[512];
    ms_format_compact(&snap, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "2026-03-20"));
}

void test_format_compact_has_separator(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[512];
    ms_format_compact(&snap, buf, (int)sizeof(buf));
    /* UTF-8 middle dot: 0xC2 0xB7 */
    TEST_ASSERT_NOT_NULL(strstr(buf, "\xc2\xb7"));
}

void test_format_compact_small_buffer(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    char buf[8];
    int n = ms_format_compact(&snap, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(n <= 7);
}

/* ===== ms_diff_count ===== */

void test_diff_count_identical(void) {
    ms_input_t in = full_input();
    ms_snapshot_t a = ms_capture(&in);
    ms_snapshot_t b = ms_capture(&in);
    TEST_ASSERT_EQUAL_INT(0, ms_diff_count(&a, &b));
}

void test_diff_count_different(void) {
    ms_input_t in_a = full_input();
    ms_input_t in_b = full_input();
    in_b.sun_sign_name = "Aries";
    in_b.moon_phase = "New Moon";
    ms_snapshot_t a = ms_capture(&in_a);
    ms_snapshot_t b = ms_capture(&in_b);
    TEST_ASSERT_EQUAL_INT(2, ms_diff_count(&a, &b));
}

void test_diff_count_one_empty(void) {
    ms_input_t in_a = full_input();
    ms_input_t in_b = ms_default_input();
    ms_snapshot_t a = ms_capture(&in_a);
    ms_snapshot_t b = ms_capture(&in_b);
    /* a has 9 systems, b has 0 — all 9 differ */
    TEST_ASSERT_EQUAL_INT(9, ms_diff_count(&a, &b));
}

/* ===== System available flag ===== */

void test_all_systems_available_flag(void) {
    ms_input_t in = full_input();
    ms_snapshot_t snap = ms_capture(&in);
    for (int i = 0; i < snap.system_count; i++) {
        TEST_ASSERT_EQUAL_INT(1, snap.systems[i].available);
    }
}

/* ===== Runner ===== */

int main(void) {
    UNITY_BEGIN();

    /* ms_default_input */
    RUN_TEST(test_default_input_jd_zero);
    RUN_TEST(test_default_input_date_zero);
    RUN_TEST(test_default_input_time_zero);
    RUN_TEST(test_default_input_weekday_null);
    RUN_TEST(test_default_input_kin_zero);
    RUN_TEST(test_default_input_sun_sign_unknown);
    RUN_TEST(test_default_input_chinese_unknown);
    RUN_TEST(test_default_input_hexagram_zero);
    RUN_TEST(test_default_input_hebrew_zero);
    RUN_TEST(test_default_input_islamic_zero);
    RUN_TEST(test_default_input_buddhist_zero);
    RUN_TEST(test_default_input_moon_null);
    RUN_TEST(test_default_input_note_null);

    /* ms_capture — NULL / default */
    RUN_TEST(test_capture_null_returns_empty);
    RUN_TEST(test_capture_default_has_no_systems);
    RUN_TEST(test_capture_default_copies_jd);

    /* ms_capture — full */
    RUN_TEST(test_capture_full_populates_all_9_systems);
    RUN_TEST(test_capture_full_copies_date);
    RUN_TEST(test_capture_full_copies_time);
    RUN_TEST(test_capture_full_copies_jd);
    RUN_TEST(test_capture_full_copies_note);
    RUN_TEST(test_capture_null_note_empty);

    /* ms_capture — partial */
    RUN_TEST(test_capture_partial_tzolkin_gregorian);
    RUN_TEST(test_capture_partial_only_moon);

    /* System text formatting */
    RUN_TEST(test_gregorian_primary_format);
    RUN_TEST(test_gregorian_secondary_format);
    RUN_TEST(test_tzolkin_primary_format);
    RUN_TEST(test_tzolkin_secondary_format);
    RUN_TEST(test_astrology_primary_format);
    RUN_TEST(test_chinese_primary_format);
    RUN_TEST(test_iching_primary_format);
    RUN_TEST(test_hebrew_primary_format);
    RUN_TEST(test_islamic_primary_format);
    RUN_TEST(test_buddhist_primary_format);
    RUN_TEST(test_moon_primary_format);

    /* ms_system */
    RUN_TEST(test_system_valid_index);
    RUN_TEST(test_system_negative_index_returns_null);
    RUN_TEST(test_system_out_of_range_returns_null);

    /* ms_find_system */
    RUN_TEST(test_find_system_existing);
    RUN_TEST(test_find_system_absent);

    /* ms_available_count */
    RUN_TEST(test_available_count_full);
    RUN_TEST(test_available_count_empty);
    RUN_TEST(test_available_count_partial);

    /* ms_format */
    RUN_TEST(test_format_nonempty);
    RUN_TEST(test_format_contains_date);
    RUN_TEST(test_format_contains_system);
    RUN_TEST(test_format_contains_note);
    RUN_TEST(test_format_small_buffer);
    RUN_TEST(test_format_zero_buffer);

    /* ms_format_compact */
    RUN_TEST(test_format_compact_nonempty);
    RUN_TEST(test_format_compact_contains_date);
    RUN_TEST(test_format_compact_has_separator);
    RUN_TEST(test_format_compact_small_buffer);

    /* ms_diff_count */
    RUN_TEST(test_diff_count_identical);
    RUN_TEST(test_diff_count_different);
    RUN_TEST(test_diff_count_one_empty);

    /* Available flag */
    RUN_TEST(test_all_systems_available_flag);

    return UNITY_END();
}
