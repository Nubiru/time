#include "../unity/unity.h"
#include "../../src/ui/hd_card.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Reference longitudes:
 * 0.0 degrees (0 Aries) → gate sequence[0] = gate 17
 * 180.0 degrees → gate sequence[32] = gate 18
 * Gate width = 5.625 degrees
 */

/* Display from longitudes: 1-3 */

void test_display_zero_aries(void)
{
    hd_display_t d = hd_display_from_longitude(0.0);
    TEST_ASSERT_NOT_NULL(strstr(d.sun_gate, "17"));
    TEST_ASSERT_NOT_NULL(strstr(d.sun_gate, "Opinions"));
}

void test_display_earth_opposite(void)
{
    hd_display_t d = hd_display_from_longitude(0.0);
    TEST_ASSERT_NOT_NULL(strstr(d.earth_gate, "18"));
    TEST_ASSERT_NOT_NULL(strstr(d.earth_gate, "Correction"));
}

void test_display_mid_ecliptic(void)
{
    hd_display_t d = hd_display_from_longitude(90.0);
    TEST_ASSERT_TRUE(strlen(d.sun_gate) > 0);
    TEST_ASSERT_TRUE(strlen(d.earth_gate) > 0);
}

/* Gate formatting: 4-7 */

void test_fmt_gate_17(void)
{
    char buf[48];
    hd_fmt_gate(17, 4, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "17"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "4"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Opinions"));
}

void test_fmt_gate_1(void)
{
    char buf[48];
    hd_fmt_gate(1, 1, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "1"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Creative"));
}

void test_fmt_gate_64(void)
{
    char buf[48];
    hd_fmt_gate(64, 6, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "64"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Confusion"));
}

void test_fmt_gate_short(void)
{
    char buf[32];
    hd_fmt_gate_short(17, 4, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "17.4"));
}

/* Cross formatting: 8-10 */

void test_fmt_cross(void)
{
    char buf[64];
    hd_fmt_cross(17, 4, 18, 2, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "17.4"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "18.2"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Earth"));
}

void test_fmt_cross_gate1(void)
{
    char buf[64];
    hd_fmt_cross(1, 1, 2, 1, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "1.1"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "2.1"));
}

void test_fmt_cross_high_gates(void)
{
    char buf[64];
    hd_fmt_cross(63, 3, 64, 3, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "63.3"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "64.3"));
}

/* Keyword formatting: 11-13 */

void test_keyword_gate17(void)
{
    char buf[32];
    hd_fmt_keyword(17, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Following"));
}

void test_keyword_gate1(void)
{
    char buf[32];
    hd_fmt_keyword(1, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Self-Expression"));
}

void test_keyword_gate42(void)
{
    char buf[32];
    hd_fmt_keyword(42, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Growth"));
}

/* Summary: 14-16 */

void test_summary_has_gates(void)
{
    hd_display_t d = hd_display_from_longitude(0.0);
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "17"));
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "18"));
}

void test_summary_has_names(void)
{
    hd_display_t d = hd_display_from_longitude(0.0);
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "Opinions"));
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "Correction"));
}

void test_summary_length(void)
{
    hd_display_t d = hd_display_from_longitude(45.0);
    TEST_ASSERT_TRUE(strlen(d.summary) > 10);
}

/* All fields populated: 17-18 */

void test_all_fields_populated(void)
{
    hd_display_t d = hd_display_from_longitude(120.0);
    TEST_ASSERT_TRUE(strlen(d.sun_gate) > 0);
    TEST_ASSERT_TRUE(strlen(d.earth_gate) > 0);
    TEST_ASSERT_TRUE(strlen(d.cross_line) > 0);
    TEST_ASSERT_TRUE(strlen(d.sun_keyword) > 0);
    TEST_ASSERT_TRUE(strlen(d.earth_keyword) > 0);
    TEST_ASSERT_TRUE(strlen(d.summary) > 0);
}

void test_various_longitudes(void)
{
    double lons[] = {0.0, 45.0, 90.0, 180.0, 270.0, 359.0};
    for (int i = 0; i < 6; i++) {
        hd_display_t d = hd_display_from_longitude(lons[i]);
        TEST_ASSERT_TRUE(strlen(d.sun_gate) > 0);
        TEST_ASSERT_TRUE(strlen(d.summary) > 0);
    }
}

/* Edge cases: 19-20 */

void test_negative_longitude(void)
{
    hd_display_t d = hd_display_from_longitude(-10.0);
    TEST_ASSERT_TRUE(strlen(d.sun_gate) > 0);
    TEST_ASSERT_TRUE(strlen(d.earth_gate) > 0);
}

void test_large_longitude(void)
{
    hd_display_t d = hd_display_from_longitude(720.5);
    TEST_ASSERT_TRUE(strlen(d.sun_gate) > 0);
}

/* Purity: 21-22 */

void test_purity(void)
{
    hd_display_t a = hd_display_from_longitude(100.0);
    hd_display_t b = hd_display_from_longitude(100.0);
    TEST_ASSERT_EQUAL_STRING(a.sun_gate, b.sun_gate);
    TEST_ASSERT_EQUAL_STRING(a.earth_gate, b.earth_gate);
    TEST_ASSERT_EQUAL_STRING(a.summary, b.summary);
}

void test_purity_cross(void)
{
    hd_display_t a = hd_display_from_longitude(200.0);
    hd_display_t b = hd_display_from_longitude(200.0);
    TEST_ASSERT_EQUAL_STRING(a.cross_line, b.cross_line);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_display_zero_aries);
    RUN_TEST(test_display_earth_opposite);
    RUN_TEST(test_display_mid_ecliptic);
    RUN_TEST(test_fmt_gate_17);
    RUN_TEST(test_fmt_gate_1);
    RUN_TEST(test_fmt_gate_64);
    RUN_TEST(test_fmt_gate_short);
    RUN_TEST(test_fmt_cross);
    RUN_TEST(test_fmt_cross_gate1);
    RUN_TEST(test_fmt_cross_high_gates);
    RUN_TEST(test_keyword_gate17);
    RUN_TEST(test_keyword_gate1);
    RUN_TEST(test_keyword_gate42);
    RUN_TEST(test_summary_has_gates);
    RUN_TEST(test_summary_has_names);
    RUN_TEST(test_summary_length);
    RUN_TEST(test_all_fields_populated);
    RUN_TEST(test_various_longitudes);
    RUN_TEST(test_negative_longitude);
    RUN_TEST(test_large_longitude);
    RUN_TEST(test_purity);
    RUN_TEST(test_purity_cross);
    return UNITY_END();
}
