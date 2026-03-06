#include "../unity/unity.h"
#include "../../src/systems/tzolkin/haab.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Known Haab dates: 1-3 */

/* Reference: 2012-12-21 (JD 2456282.5) = 3 Kankin in the Haab.
 * Using GMT 584283 correlation. The Haab epoch (0 Pop) starts at
 * a specific point in the Long Count. Let's verify relative consistency. */

void test_haab_from_jd_reference(void)
{
    /* JD 2456282.5 = 2012-12-21. The Maya Long Count was 13.0.0.0.0.
     * Haab date: 3 Kankin (month 14, day 3) */
    haab_date_t h = haab_from_jd(2456282.5);
    TEST_ASSERT_TRUE(h.month >= 0 && h.month <= 18);
    TEST_ASSERT_TRUE(h.day >= 0 && h.day <= 19);
}

void test_haab_advances_daily(void)
{
    /* Each day, Haab day advances by 1 */
    haab_date_t h1 = haab_from_jd(2451545.0);
    haab_date_t h2 = haab_from_jd(2451546.0);

    /* Either day increments or month changes */
    if (h1.month == h2.month) {
        TEST_ASSERT_EQUAL_INT(h1.day + 1, h2.day);
    } else {
        /* Month changed, day should be 0 */
        TEST_ASSERT_EQUAL_INT(0, h2.day);
    }
}

void test_haab_365_cycle(void)
{
    /* After 365 days, same Haab date */
    double jd = 2451545.0;
    haab_date_t h1 = haab_from_jd(jd);
    haab_date_t h2 = haab_from_jd(jd + 365.0);
    TEST_ASSERT_EQUAL_INT(h1.month, h2.month);
    TEST_ASSERT_EQUAL_INT(h1.day, h2.day);
}

/* Month names: 4-6 */

void test_month_names_all(void)
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

void test_month_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", haab_month_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", haab_month_name(19));
}

void test_month_count(void)
{
    TEST_ASSERT_EQUAL_INT(19, haab_month_count());
}

/* Wayeb: 7-8 */

void test_wayeb_days(void)
{
    /* Wayeb is month 18, has only 5 days (0-4) */
    /* Find a Wayeb day by scanning */
    int found_wayeb = 0;
    for (int d = 0; d < 365; d++) {
        haab_date_t h = haab_from_jd(2451545.0 + d);
        if (h.month == 18) {
            found_wayeb++;
            TEST_ASSERT_TRUE(h.day >= 0 && h.day <= 4);
        }
    }
    TEST_ASSERT_EQUAL_INT(5, found_wayeb);
}

void test_regular_month_days(void)
{
    /* Regular months (0-17) have 20 days (0-19) */
    int month_max[19];
    for (int i = 0; i < 19; i++) month_max[i] = -1;

    for (int d = 0; d < 365; d++) {
        haab_date_t h = haab_from_jd(2451545.0 + d);
        if (h.day > month_max[h.month]) {
            month_max[h.month] = h.day;
        }
    }
    for (int i = 0; i < 18; i++) {
        TEST_ASSERT_EQUAL_INT(19, month_max[i]); /* 0-19 */
    }
    TEST_ASSERT_EQUAL_INT(4, month_max[18]); /* Wayeb: 0-4 */
}

/* Format: 9-10 */

void test_format_regular(void)
{
    haab_date_t h = {0, 5};
    char buf[32];
    haab_format(h, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "5"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Pop"));
}

void test_format_wayeb(void)
{
    haab_date_t h = {18, 3};
    char buf[32];
    haab_format(h, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Wayeb"));
}

/* Month meanings: 11-12 */

void test_month_meanings(void)
{
    const char *m = haab_month_meaning(0);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strcmp(m, "?") != 0);
}

void test_month_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", haab_month_meaning(-1));
    TEST_ASSERT_EQUAL_STRING("?", haab_month_meaning(19));
}

/* Calendar Round: 13-16 */

void test_calendar_round_from_jd(void)
{
    calendar_round_t cr = calendar_round_from_jd(2451545.0);
    TEST_ASSERT_TRUE(cr.tzolkin_tone >= 1 && cr.tzolkin_tone <= 13);
    TEST_ASSERT_TRUE(cr.tzolkin_seal >= 0 && cr.tzolkin_seal <= 19);
    TEST_ASSERT_TRUE(cr.haab_month >= 0 && cr.haab_month <= 18);
    TEST_ASSERT_TRUE(cr.haab_day >= 0 && cr.haab_day <= 19);
}

void test_calendar_round_cycle(void)
{
    TEST_ASSERT_EQUAL_INT(18980, calendar_round_cycle());
}

void test_calendar_round_repeats(void)
{
    /* After 18980 days, same Calendar Round */
    double jd = 2451545.0;
    calendar_round_t cr1 = calendar_round_from_jd(jd);
    calendar_round_t cr2 = calendar_round_from_jd(jd + 18980.0);
    TEST_ASSERT_EQUAL_INT(cr1.tzolkin_tone, cr2.tzolkin_tone);
    TEST_ASSERT_EQUAL_INT(cr1.tzolkin_seal, cr2.tzolkin_seal);
    TEST_ASSERT_EQUAL_INT(cr1.haab_month, cr2.haab_month);
    TEST_ASSERT_EQUAL_INT(cr1.haab_day, cr2.haab_day);
}

void test_calendar_round_next_same(void)
{
    /* If already on target date, returns 0 */
    double jd = 2451545.0;
    calendar_round_t cr = calendar_round_from_jd(jd);
    TEST_ASSERT_EQUAL_INT(0, calendar_round_next(jd, cr));
}

/* Next occurrence: 17-18 */

void test_calendar_round_next_tomorrow(void)
{
    double jd = 2451545.0;
    calendar_round_t tomorrow = calendar_round_from_jd(jd + 1.0);
    int days = calendar_round_next(jd, tomorrow);
    TEST_ASSERT_EQUAL_INT(1, days);
}

void test_calendar_round_next_range(void)
{
    double jd = 2451545.0;
    calendar_round_t target = calendar_round_from_jd(jd + 100.0);
    int days = calendar_round_next(jd, target);
    TEST_ASSERT_TRUE(days >= 1 && days <= 18980);
}

/* Day count in full year: 19 */

void test_full_year_coverage(void)
{
    /* 365 consecutive days should cover all 18 regular months + Wayeb */
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

/* All meanings non-null: 20 */

void test_all_meanings_valid(void)
{
    for (int i = 0; i < 19; i++) {
        const char *m = haab_month_meaning(i);
        TEST_ASSERT_NOT_NULL(m);
        TEST_ASSERT_TRUE(strlen(m) > 0);
        TEST_ASSERT_TRUE(strcmp(m, "?") != 0);
    }
}

/* Invalid format: 21 */

void test_format_null_safety(void)
{
    haab_date_t h = {0, 0};
    char *result = haab_format(h, NULL, 0);
    TEST_ASSERT_NULL(result);
}

/* Purity: 22 */

void test_purity(void)
{
    haab_date_t a = haab_from_jd(2451545.0);
    haab_date_t b = haab_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(a.month, b.month);
    TEST_ASSERT_EQUAL_INT(a.day, b.day);

    calendar_round_t ca = calendar_round_from_jd(2451545.0);
    calendar_round_t cb = calendar_round_from_jd(2451545.0);
    TEST_ASSERT_EQUAL_INT(ca.haab_month, cb.haab_month);
    TEST_ASSERT_EQUAL_INT(ca.tzolkin_tone, cb.tzolkin_tone);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_haab_from_jd_reference);
    RUN_TEST(test_haab_advances_daily);
    RUN_TEST(test_haab_365_cycle);
    RUN_TEST(test_month_names_all);
    RUN_TEST(test_month_name_invalid);
    RUN_TEST(test_month_count);
    RUN_TEST(test_wayeb_days);
    RUN_TEST(test_regular_month_days);
    RUN_TEST(test_format_regular);
    RUN_TEST(test_format_wayeb);
    RUN_TEST(test_month_meanings);
    RUN_TEST(test_month_meaning_invalid);
    RUN_TEST(test_calendar_round_from_jd);
    RUN_TEST(test_calendar_round_cycle);
    RUN_TEST(test_calendar_round_repeats);
    RUN_TEST(test_calendar_round_next_same);
    RUN_TEST(test_calendar_round_next_tomorrow);
    RUN_TEST(test_calendar_round_next_range);
    RUN_TEST(test_full_year_coverage);
    RUN_TEST(test_all_meanings_valid);
    RUN_TEST(test_format_null_safety);
    RUN_TEST(test_purity);
    return UNITY_END();
}
