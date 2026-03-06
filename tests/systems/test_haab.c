#include "../unity/unity.h"
#include "../../src/systems/tzolkin/haab.h"
#include "../../src/systems/tzolkin/tzolkin.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== 1. Epoch date ===== */

void test_haab_epoch_date(void)
{
    /* JD 584283 (Long Count 0.0.0.0.0) = 8 Kumk'u (month 17, day 8) */
    haab_date_t h = haab_from_jd(584283.0);
    TEST_ASSERT_EQUAL_INT(17, h.month);
    TEST_ASSERT_EQUAL_INT(8, h.day);
}

/* ===== 2. Next day from epoch ===== */

void test_haab_epoch_next_day(void)
{
    /* JD 584284 = 9 Kumk'u */
    haab_date_t h = haab_from_jd(584284.0);
    TEST_ASSERT_EQUAL_INT(17, h.month);
    TEST_ASSERT_EQUAL_INT(9, h.day);
}

/* ===== 3. Dec 21, 2012 ===== */

void test_haab_2012_dec_21(void)
{
    /* JD 2456283.0 (noon Dec 21, 2012).
     * day_count = 2456283 - 584283 = 1872000
     * 1872000 % 365 = 280
     * (280 + 348) % 365 = 628 % 365 = 263
     * 263 / 20 = 13 (month), 263 % 20 = 3 (day)
     * = 3 K'ank'in */
    haab_date_t h = haab_from_jd(2456283.0);
    TEST_ASSERT_EQUAL_INT(13, h.month);
    TEST_ASSERT_EQUAL_INT(3, h.day);
}

/* ===== 4. Pop 0 (New Year) ===== */

void test_haab_pop_0_new_year(void)
{
    /* From epoch: position=0 when day_count=17 (584283 + 17 = 584300)
     * (17 % 365 + 348) % 365 = 365 % 365 = 0 => 0 Pop */
    haab_date_t h = haab_from_jd(584300.0);
    TEST_ASSERT_EQUAL_INT(0, h.month);
    TEST_ASSERT_EQUAL_INT(0, h.day);
}

/* ===== 5. Wayeb boundary ===== */

void test_haab_kumku_last_day(void)
{
    /* day-of-year 359 = month 17, day 19 = last day of Kumk'u
     * Need position=359: day_count such that (day_count%365 + 348)%365 = 359
     * day_count = 359 - 348 = 11 => JD = 584283 + 11 = 584294 */
    haab_date_t h = haab_from_jd(584294.0);
    TEST_ASSERT_EQUAL_INT(17, h.month);
    TEST_ASSERT_EQUAL_INT(19, h.day);
}

void test_haab_wayeb_first_day(void)
{
    /* day-of-year 360 = Wayeb day 0
     * day_count = 360 - 348 = 12 => JD = 584283 + 12 = 584295 */
    haab_date_t h = haab_from_jd(584295.0);
    TEST_ASSERT_EQUAL_INT(18, h.month);
    TEST_ASSERT_EQUAL_INT(0, h.day);
}

/* ===== 6. End of Wayeb ===== */

void test_haab_wayeb_last_day(void)
{
    /* day-of-year 364 = Wayeb day 4 (last day of Haab year)
     * day_count = 364 - 348 = 16 => JD = 584283 + 16 = 584299 */
    haab_date_t h = haab_from_jd(584299.0);
    TEST_ASSERT_EQUAL_INT(18, h.month);
    TEST_ASSERT_EQUAL_INT(4, h.day);
}

/* ===== 7. Month names ===== */

void test_haab_month_names_all(void)
{
    const char *expected[] = {
        "Pop", "Wo", "Sip", "Sotz'", "Sek",
        "Xul", "Yaxk'in", "Mol", "Ch'en", "Yax",
        "Sak", "Keh", "Mak", "K'ank'in", "Muwan",
        "Pax", "K'ayab", "Kumk'u", "Wayeb"
    };
    for (int i = 0; i < 19; i++) {
        TEST_ASSERT_EQUAL_STRING(expected[i], haab_month_name(i));
    }
}

/* ===== 8. Month lengths ===== */

void test_haab_month_length_regular(void)
{
    for (int i = 0; i < 18; i++) {
        TEST_ASSERT_EQUAL_INT(20, haab_month_length(i));
    }
}

void test_haab_month_length_wayeb(void)
{
    TEST_ASSERT_EQUAL_INT(5, haab_month_length(18));
}

void test_haab_month_length_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, haab_month_length(-1));
    TEST_ASSERT_EQUAL_INT(0, haab_month_length(19));
}

/* ===== 9. Day-of-year ===== */

void test_haab_day_of_year_pop_0(void)
{
    haab_date_t h = {0, 0};
    TEST_ASSERT_EQUAL_INT(0, haab_day_of_year(h));
}

void test_haab_day_of_year_pop_19(void)
{
    haab_date_t h = {0, 19};
    TEST_ASSERT_EQUAL_INT(19, haab_day_of_year(h));
}

void test_haab_day_of_year_wo_0(void)
{
    haab_date_t h = {1, 0};
    TEST_ASSERT_EQUAL_INT(20, haab_day_of_year(h));
}

void test_haab_day_of_year_kumku_19(void)
{
    haab_date_t h = {17, 19};
    TEST_ASSERT_EQUAL_INT(359, haab_day_of_year(h));
}

void test_haab_day_of_year_wayeb_0(void)
{
    haab_date_t h = {18, 0};
    TEST_ASSERT_EQUAL_INT(360, haab_day_of_year(h));
}

void test_haab_day_of_year_wayeb_4(void)
{
    haab_date_t h = {18, 4};
    TEST_ASSERT_EQUAL_INT(364, haab_day_of_year(h));
}

/* ===== 10. Formatting ===== */

void test_haab_fmt_kumku(void)
{
    haab_date_t h = {17, 8};
    char buf[32];
    haab_fmt(h, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("8 Kumk'u", buf);
}

void test_haab_fmt_kankin(void)
{
    haab_date_t h = {13, 3};
    char buf[32];
    haab_fmt(h, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("3 K'ank'in", buf);
}

void test_haab_fmt_pop_0(void)
{
    haab_date_t h = {0, 0};
    char buf[32];
    haab_fmt(h, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("0 Pop", buf);
}

void test_haab_fmt_wayeb(void)
{
    haab_date_t h = {18, 2};
    char buf[32];
    haab_fmt(h, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("2 Wayeb", buf);
}

/* ===== 11. Calendar Round from JD ===== */

void test_calendar_round_epoch(void)
{
    /* Long Count 0.0.0.0.0 (JD 584283):
     * Traditional Tzolkin: 4 Ahau, Haab: 8 Kumk'u
     * But this project uses Dreamspell Tzolkin, so Tzolkin values
     * come from tzolkin_from_jd(). Haab should be 8 Kumk'u. */
    calendar_round_t cr = calendar_round_from_jd(584283.0);
    TEST_ASSERT_EQUAL_INT(17, cr.haab_month);
    TEST_ASSERT_EQUAL_INT(8, cr.haab_day);
    /* Tzolkin fields should be valid ranges */
    TEST_ASSERT_TRUE(cr.kin >= 1 && cr.kin <= 260);
    TEST_ASSERT_TRUE(cr.tone >= 1 && cr.tone <= 13);
    TEST_ASSERT_TRUE(cr.seal >= 0 && cr.seal <= 19);
}

void test_calendar_round_2012_dec_21(void)
{
    /* JD 2456283.0: Haab = 3 K'ank'in */
    calendar_round_t cr = calendar_round_from_jd(2456283.0);
    TEST_ASSERT_EQUAL_INT(13, cr.haab_month);
    TEST_ASSERT_EQUAL_INT(3, cr.haab_day);
    TEST_ASSERT_TRUE(cr.kin >= 1 && cr.kin <= 260);
}

/* ===== 12. Calendar Round formatting ===== */

void test_calendar_round_fmt_contains_parts(void)
{
    calendar_round_t cr = calendar_round_from_jd(2456283.0);
    char buf[64];
    calendar_round_fmt(cr, buf, sizeof(buf));
    /* Should contain haab month name */
    TEST_ASSERT_NOT_NULL(strstr(buf, "K'ank'in"));
    /* Should contain the tone number as a string */
    char tone_str[4];
    snprintf(tone_str, sizeof(tone_str), "%d", cr.tone);
    TEST_ASSERT_NOT_NULL(strstr(buf, tone_str));
}

/* ===== 13. Pre-epoch dates ===== */

void test_haab_pre_epoch(void)
{
    /* JD before 584283 (negative day_count) — should still give valid Haab */
    haab_date_t h = haab_from_jd(584200.0);
    TEST_ASSERT_TRUE(h.month >= 0 && h.month <= 18);
    if (h.month < 18) {
        TEST_ASSERT_TRUE(h.day >= 0 && h.day <= 19);
    } else {
        TEST_ASSERT_TRUE(h.day >= 0 && h.day <= 4);
    }
}

void test_haab_pre_epoch_very_early(void)
{
    /* Much earlier date */
    haab_date_t h = haab_from_jd(100000.0);
    TEST_ASSERT_TRUE(h.month >= 0 && h.month <= 18);
    if (h.month < 18) {
        TEST_ASSERT_TRUE(h.day >= 0 && h.day <= 19);
    } else {
        TEST_ASSERT_TRUE(h.day >= 0 && h.day <= 4);
    }
}

/* ===== 14. Large date ===== */

void test_haab_large_date(void)
{
    haab_date_t h = haab_from_jd(2460000.0);
    TEST_ASSERT_TRUE(h.month >= 0 && h.month <= 18);
    if (h.month < 18) {
        TEST_ASSERT_TRUE(h.day >= 0 && h.day <= 19);
    } else {
        TEST_ASSERT_TRUE(h.day >= 0 && h.day <= 4);
    }
}

/* ===== 15. Invalid month name ===== */

void test_haab_month_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", haab_month_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", haab_month_name(19));
    TEST_ASSERT_EQUAL_STRING("?", haab_month_name(100));
}

/* ===== 16. Invalid month length ===== */

void test_haab_month_length_out_of_range(void)
{
    TEST_ASSERT_EQUAL_INT(0, haab_month_length(-1));
    TEST_ASSERT_EQUAL_INT(0, haab_month_length(19));
    TEST_ASSERT_EQUAL_INT(0, haab_month_length(100));
}

/* ===== 17. Purity ===== */

void test_purity_haab(void)
{
    haab_date_t a = haab_from_jd(2451545.0);
    haab_date_t b = haab_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(a.month, b.month);
    TEST_ASSERT_EQUAL_INT(a.day, b.day);
}

void test_purity_calendar_round(void)
{
    calendar_round_t a = calendar_round_from_jd(2451545.0);
    calendar_round_t b = calendar_round_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(a.kin, b.kin);
    TEST_ASSERT_EQUAL_INT(a.tone, b.tone);
    TEST_ASSERT_EQUAL_INT(a.seal, b.seal);
    TEST_ASSERT_EQUAL_INT(a.haab_month, b.haab_month);
    TEST_ASSERT_EQUAL_INT(a.haab_day, b.haab_day);
}

/* ===== 18. 365-day cycle ===== */

void test_haab_365_cycle(void)
{
    double jd = 2451545.0;
    haab_date_t h1 = haab_from_jd(jd);
    haab_date_t h2 = haab_from_jd(jd + 365.0);
    TEST_ASSERT_EQUAL_INT(h1.month, h2.month);
    TEST_ASSERT_EQUAL_INT(h1.day, h2.day);
}

/* ===== 19. Daily advancement ===== */

void test_haab_advances_daily(void)
{
    haab_date_t h1 = haab_from_jd(2451545.0);
    haab_date_t h2 = haab_from_jd(2451546.0);
    if (h1.month == h2.month) {
        TEST_ASSERT_EQUAL_INT(h1.day + 1, h2.day);
    } else {
        TEST_ASSERT_EQUAL_INT(0, h2.day);
    }
}

/* ===== 20. Full year coverage ===== */

void test_haab_full_year_coverage(void)
{
    int months_seen[19];
    for (int i = 0; i < 19; i++) months_seen[i] = 0;
    for (int d = 0; d < 365; d++) {
        haab_date_t h = haab_from_jd(2451545.0 + d);
        months_seen[h.month] = 1;
    }
    for (int i = 0; i < 19; i++) {
        TEST_ASSERT_TRUE(months_seen[i]);
    }
}

/* ===== 21. Wayeb has exactly 5 days in a full cycle ===== */

void test_haab_wayeb_count(void)
{
    int wayeb_count = 0;
    for (int d = 0; d < 365; d++) {
        haab_date_t h = haab_from_jd(2451545.0 + d);
        if (h.month == 18) wayeb_count++;
    }
    TEST_ASSERT_EQUAL_INT(5, wayeb_count);
}

/* ===== 22. Calendar Round cycle ===== */

void test_calendar_round_cycle_value(void)
{
    TEST_ASSERT_EQUAL_INT(18980, calendar_round_cycle());
}

/* ===== 23. Calendar Round repeats after 18980 days ===== */

void test_calendar_round_repeats(void)
{
    double jd = 2451545.0;
    calendar_round_t cr1 = calendar_round_from_jd(jd);
    calendar_round_t cr2 = calendar_round_from_jd(jd + 18980.0);
    TEST_ASSERT_EQUAL_INT(cr1.haab_month, cr2.haab_month);
    TEST_ASSERT_EQUAL_INT(cr1.haab_day, cr2.haab_day);
    /* Note: Tzolkin fields use Dreamspell which handles leap years
     * differently, so we only verify Haab repetition here. */
}

/* ===== 24. Month count ===== */

void test_haab_month_count(void)
{
    TEST_ASSERT_EQUAL_INT(19, haab_month_count());
}

/* ===== 25. Month meanings valid ===== */

void test_haab_month_meanings_all_valid(void)
{
    for (int i = 0; i < 19; i++) {
        const char *m = haab_month_meaning(i);
        TEST_ASSERT_NOT_NULL(m);
        TEST_ASSERT_TRUE(strlen(m) > 0);
        TEST_ASSERT_TRUE(strcmp(m, "?") != 0);
    }
}

void test_haab_month_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", haab_month_meaning(-1));
    TEST_ASSERT_EQUAL_STRING("?", haab_month_meaning(19));
}

/* ===== 27. Day-of-year round-trip with haab_from_jd ===== */

void test_haab_day_of_year_round_trip(void)
{
    /* For any JD, haab_day_of_year(haab_from_jd(jd)) should be 0-364 */
    for (int d = 0; d < 365; d++) {
        haab_date_t h = haab_from_jd(584300.0 + d); /* Start from 0 Pop */
        int doy = haab_day_of_year(h);
        TEST_ASSERT_EQUAL_INT(d, doy);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* Epoch and known dates */
    RUN_TEST(test_haab_epoch_date);
    RUN_TEST(test_haab_epoch_next_day);
    RUN_TEST(test_haab_2012_dec_21);
    RUN_TEST(test_haab_pop_0_new_year);
    /* Wayeb boundary */
    RUN_TEST(test_haab_kumku_last_day);
    RUN_TEST(test_haab_wayeb_first_day);
    RUN_TEST(test_haab_wayeb_last_day);
    /* Month names */
    RUN_TEST(test_haab_month_names_all);
    /* Month lengths */
    RUN_TEST(test_haab_month_length_regular);
    RUN_TEST(test_haab_month_length_wayeb);
    RUN_TEST(test_haab_month_length_invalid);
    /* Day-of-year */
    RUN_TEST(test_haab_day_of_year_pop_0);
    RUN_TEST(test_haab_day_of_year_pop_19);
    RUN_TEST(test_haab_day_of_year_wo_0);
    RUN_TEST(test_haab_day_of_year_kumku_19);
    RUN_TEST(test_haab_day_of_year_wayeb_0);
    RUN_TEST(test_haab_day_of_year_wayeb_4);
    /* Formatting */
    RUN_TEST(test_haab_fmt_kumku);
    RUN_TEST(test_haab_fmt_kankin);
    RUN_TEST(test_haab_fmt_pop_0);
    RUN_TEST(test_haab_fmt_wayeb);
    /* Calendar Round */
    RUN_TEST(test_calendar_round_epoch);
    RUN_TEST(test_calendar_round_2012_dec_21);
    RUN_TEST(test_calendar_round_fmt_contains_parts);
    /* Pre-epoch and edge cases */
    RUN_TEST(test_haab_pre_epoch);
    RUN_TEST(test_haab_pre_epoch_very_early);
    RUN_TEST(test_haab_large_date);
    /* Invalid inputs */
    RUN_TEST(test_haab_month_name_invalid);
    RUN_TEST(test_haab_month_length_out_of_range);
    /* Purity */
    RUN_TEST(test_purity_haab);
    RUN_TEST(test_purity_calendar_round);
    /* Cycle properties */
    RUN_TEST(test_haab_365_cycle);
    RUN_TEST(test_haab_advances_daily);
    RUN_TEST(test_haab_full_year_coverage);
    RUN_TEST(test_haab_wayeb_count);
    RUN_TEST(test_calendar_round_cycle_value);
    RUN_TEST(test_calendar_round_repeats);
    /* Supplementary */
    RUN_TEST(test_haab_month_count);
    RUN_TEST(test_haab_month_meanings_all_valid);
    RUN_TEST(test_haab_month_meaning_invalid);
    RUN_TEST(test_haab_day_of_year_round_trip);
    return UNITY_END();
}
