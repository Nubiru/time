#include "../unity/unity.h"
#include "../../src/ui/time_hud.h"
#include "../../src/math/julian.h"
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== degrees_to_hms ===== */

void test_degrees_to_hms_zero(void) {
    char buf[16];
    degrees_to_hms(0.0, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("00h 00m 00s", buf);
}

void test_degrees_to_hms_270_5(void) {
    /* 270.5 / 15 = 18.0333... -> 18h 02m 00s */
    char buf[16];
    degrees_to_hms(270.5, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("18h 02m 00s", buf);
}

void test_degrees_to_hms_360(void) {
    /* 360 degrees wraps to 0 */
    char buf[16];
    degrees_to_hms(360.0, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("00h 00m 00s", buf);
}

void test_degrees_to_hms_180(void) {
    /* 180 / 15 = 12h exactly */
    char buf[16];
    degrees_to_hms(180.0, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("12h 00m 00s", buf);
}

/* ===== HUD at J2000.0 ===== */

void test_hud_date_j2000(void) {
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, 1.0);
    TEST_ASSERT_EQUAL_STRING("2000-01-01", hud.gregorian_date);
}

void test_hud_time_j2000(void) {
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, 1.0);
    TEST_ASSERT_EQUAL_STRING("12:00:00", hud.gregorian_time);
}

void test_hud_day_j2000(void) {
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, 1.0);
    TEST_ASSERT_EQUAL_STRING("Saturday", hud.day_name);
}

void test_hud_jd_string(void) {
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, 1.0);
    TEST_ASSERT_EQUAL_STRING("2451545.0000", hud.jd_str);
}

void test_hud_gmst_j2000(void) {
    /* GMST at J2000.0 (2000-01-01 12:00 UT) ~ 280.46 deg ~ 18h 41m */
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, 1.0);
    TEST_ASSERT_NOT_NULL(strstr(hud.gmst_str, "18h 41m"));
}

/* ===== Tzolkin ===== */

void test_hud_tzolkin_2012(void) {
    /* 2012-12-21 = Kin 207 (Blue Crystal Hand) */
    double jd = gregorian_to_jd(2012, 12, 21.0);
    time_hud_t hud = time_hud_compute(jd, 0.0, 1.0);
    TEST_ASSERT_NOT_NULL(strstr(hud.tzolkin_str, "Kin 207"));
}

/* ===== Sun sign ===== */

void test_hud_sun_sign_j2000(void) {
    /* Earth helio lon ~ 100 deg -> Sun geocentric ~ 280 deg -> Capricorn */
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, 1.0);
    TEST_ASSERT_NOT_NULL(strstr(hud.sun_sign_str, "Capricorn"));
}

/* ===== Speed labels ===== */

void test_hud_speed_paused(void) {
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, 0.0);
    TEST_ASSERT_EQUAL_STRING("PAUSED", hud.speed_str);
}

void test_hud_speed_1x(void) {
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, 1.0);
    TEST_ASSERT_EQUAL_STRING("1x", hud.speed_str);
}

void test_hud_speed_86400x(void) {
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, 86400.0);
    TEST_ASSERT_EQUAL_STRING("86400x", hud.speed_str);
}

void test_hud_speed_negative(void) {
    time_hud_t hud = time_hud_compute(2451545.0, 0.0, -1.0);
    TEST_ASSERT_EQUAL_STRING("-1x", hud.speed_str);
}

/* ===== LST offset ===== */

void test_hud_lst_offset(void) {
    time_hud_t hud0 = time_hud_compute(2451545.0, 0.0, 1.0);
    time_hud_t hud90 = time_hud_compute(2451545.0, 90.0, 1.0);
    /* LST at different longitudes should differ */
    TEST_ASSERT_TRUE(strcmp(hud0.lst_str, hud90.lst_str) != 0);
    /* LST at lon=0 should equal GAST */
    TEST_ASSERT_EQUAL_STRING(hud0.gast_str, hud0.lst_str);
}

/* ===== All strings non-empty ===== */

void test_hud_all_strings_nonempty(void) {
    /* Arbitrary JD: all fields should produce non-empty strings */
    time_hud_t hud = time_hud_compute(2460000.0, -73.935, 60.0);
    TEST_ASSERT_TRUE(strlen(hud.gregorian_date) > 0);
    TEST_ASSERT_TRUE(strlen(hud.gregorian_time) > 0);
    TEST_ASSERT_TRUE(strlen(hud.day_name) > 0);
    TEST_ASSERT_TRUE(strlen(hud.jd_str) > 0);
    TEST_ASSERT_TRUE(strlen(hud.gmst_str) > 0);
    TEST_ASSERT_TRUE(strlen(hud.gast_str) > 0);
    TEST_ASSERT_TRUE(strlen(hud.lst_str) > 0);
    TEST_ASSERT_TRUE(strlen(hud.tzolkin_str) > 0);
    TEST_ASSERT_TRUE(strlen(hud.sun_sign_str) > 0);
    TEST_ASSERT_TRUE(strlen(hud.speed_str) > 0);
}

/* ===== Purity: deterministic ===== */

void test_hud_purity(void) {
    /* Same inputs must produce identical outputs */
    time_hud_t a = time_hud_compute(2451545.0, 15.0, 1.0);
    time_hud_t b = time_hud_compute(2451545.0, 15.0, 1.0);
    TEST_ASSERT_EQUAL_STRING(a.gregorian_date, b.gregorian_date);
    TEST_ASSERT_EQUAL_STRING(a.gregorian_time, b.gregorian_time);
    TEST_ASSERT_EQUAL_STRING(a.day_name, b.day_name);
    TEST_ASSERT_EQUAL_STRING(a.jd_str, b.jd_str);
    TEST_ASSERT_EQUAL_STRING(a.gmst_str, b.gmst_str);
    TEST_ASSERT_EQUAL_STRING(a.gast_str, b.gast_str);
    TEST_ASSERT_EQUAL_STRING(a.lst_str, b.lst_str);
    TEST_ASSERT_EQUAL_STRING(a.tzolkin_str, b.tzolkin_str);
    TEST_ASSERT_EQUAL_STRING(a.sun_sign_str, b.sun_sign_str);
    TEST_ASSERT_EQUAL_STRING(a.speed_str, b.speed_str);
}

int main(void) {
    UNITY_BEGIN();
    /* degrees_to_hms */
    RUN_TEST(test_degrees_to_hms_zero);
    RUN_TEST(test_degrees_to_hms_270_5);
    RUN_TEST(test_degrees_to_hms_360);
    RUN_TEST(test_degrees_to_hms_180);
    /* HUD at J2000.0 */
    RUN_TEST(test_hud_date_j2000);
    RUN_TEST(test_hud_time_j2000);
    RUN_TEST(test_hud_day_j2000);
    RUN_TEST(test_hud_jd_string);
    RUN_TEST(test_hud_gmst_j2000);
    /* Tzolkin */
    RUN_TEST(test_hud_tzolkin_2012);
    /* Sun sign */
    RUN_TEST(test_hud_sun_sign_j2000);
    /* Speed labels */
    RUN_TEST(test_hud_speed_paused);
    RUN_TEST(test_hud_speed_1x);
    RUN_TEST(test_hud_speed_86400x);
    RUN_TEST(test_hud_speed_negative);
    /* LST */
    RUN_TEST(test_hud_lst_offset);
    /* Coverage */
    RUN_TEST(test_hud_all_strings_nonempty);
    RUN_TEST(test_hud_purity);
    return UNITY_END();
}
