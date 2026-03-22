/* test_ring_today.c — Tests for ring_today module.
 * Validates today-index computation for all knowledge systems. */

#include "unity.h"
#include "render/ring_today.h"
#include "systems/unified/today_summary.h"

/* Known Julian Day: 2460676.5 = 2025-01-15 (Wednesday) */
#define TEST_JD 2460676.5

/* Sun longitude ~295 deg (Capricorn) on 2025-01-15 */
#define TEST_SUN_LON 295.0

/* Moon longitude ~180 deg on some arbitrary date */
#define TEST_MOON_LON 180.0

void setUp(void) {}
void tearDown(void) {}

/* --- 1. Gregorian day of week in range 0-6 --- */
void test_gregorian_index_valid(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_GREGORIAN];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(6, idx);
}

/* --- 2. Tzolkin kin range 0-259 --- */
void test_tzolkin_index_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_TZOLKIN];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(259, idx);
}

/* --- 3. I Ching index range 0-63 --- */
void test_iching_index_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_ICHING];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(63, idx);
}

/* --- 4. Chinese cycle_year range 0-59 --- */
void test_chinese_index_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_CHINESE];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(59, idx);
}

/* --- 5. Zodiac from sun longitude --- */
void test_zodiac_from_sun_longitude(void) {
    /* sun_lon=0 -> Aries (0) */
    ring_today_t rt0 = ring_today_compute(TEST_JD, 0.0, TEST_MOON_LON);
    TEST_ASSERT_EQUAL_INT(0, rt0.indices[TS_SYS_ASTROLOGY]);

    /* sun_lon=30 -> Taurus (1) */
    ring_today_t rt1 = ring_today_compute(TEST_JD, 30.0, TEST_MOON_LON);
    TEST_ASSERT_EQUAL_INT(1, rt1.indices[TS_SYS_ASTROLOGY]);
}

/* --- 6. Kabbalah returns -1 --- */
void test_kabbalah_returns_minus_one(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_EQUAL_INT(-1, rt.indices[TS_SYS_KABBALAH]);
}

/* --- 7. Cosmic returns -1 --- */
void test_cosmic_returns_minus_one(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    TEST_ASSERT_EQUAL_INT(-1, rt.indices[TS_SYS_COSMIC]);
}

/* --- 8. All indices in range for a known JD --- */
void test_all_indices_in_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);

    /* Cycle lengths per system (max valid index) */
    const int cycle_max[] = {
        6,    /* GREGORIAN: 0-6 */
        259,  /* TZOLKIN: 0-259 */
        18,   /* HAAB: 0-18 */
        59,   /* CHINESE: 0-59 */
        12,   /* HEBREW: 0-12 */
        11,   /* ISLAMIC: 0-11 */
        4,    /* BUDDHIST: 0-4 */
        26,   /* HINDU: 0-26 */
        63,   /* ICHING: 0-63 */
        11,   /* ASTROLOGY: 0-11 */
        63,   /* HUMAN_DESIGN: 0-63 */
        -1,   /* KABBALAH: always -1 */
        12,   /* COPTIC: 0-12 */
        12,   /* ETHIOPIAN: 0-12 */
        11,   /* PERSIAN: 0-11 */
        11,   /* JAPANESE: 0-11 */
        11,   /* KOREAN: 0-11 */
        11,   /* THAI: 0-11 */
        99,   /* GEOLOGICAL: index depends on era count */
        -1,   /* COSMIC: always -1 */
        6,    /* EARTH: maps to Gregorian ring (0-6) */
    };

    for (int i = 0; i < TS_SYS_COUNT; i++) {
        int idx = rt.indices[i];
        if (cycle_max[i] == -1) {
            TEST_ASSERT_EQUAL_INT_MESSAGE(-1, idx, "Expected -1 for system with no mapping");
        } else {
            TEST_ASSERT_GREATER_OR_EQUAL_INT_MESSAGE(0, idx, "Index below 0");
            TEST_ASSERT_LESS_OR_EQUAL_INT_MESSAGE(cycle_max[i], idx, "Index above max");
        }
    }
}

/* --- 9. ring_today_index accessor matches direct array access --- */
void test_ring_today_index_accessor(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    for (int i = 0; i < TS_SYS_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(rt.indices[i], ring_today_index(&rt, i));
    }
}

/* --- 10. ring_today_for_depth produces 6 entries --- */
void test_ring_today_for_depth_six_entries(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int out[6];
    ring_today_for_depth(&rt, out);
    /* Each must be -1 or >= 0 */
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_GREATER_OR_EQUAL(-1, out[i]);
    }
}

/* --- 11. Hebrew index range 0-12 --- */
void test_hebrew_index_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_HEBREW];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(12, idx);
}

/* --- 12. Islamic index range 0-11 --- */
void test_islamic_index_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_ISLAMIC];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(11, idx);
}

/* --- 13. Nakshatra from moon longitude --- */
void test_nakshatra_from_moon_longitude(void) {
    /* moon_lon=0 -> nakshatra 0 (Ashwini) */
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, 0.0);
    TEST_ASSERT_EQUAL_INT(0, rt.indices[TS_SYS_HINDU]);
}

/* --- 14. HD gate from sun longitude, range 0-63 --- */
void test_hd_gate_from_sun_longitude(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_HUMAN_DESIGN];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(63, idx);
}

/* --- 15. Geological index valid for modern date --- */
void test_geological_index_valid(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_GEOLOGICAL];
    /* Modern dates should map to a valid era index (>= 0) */
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
}

/* --- 16. Coptic index range 0-12 --- */
void test_coptic_index_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_COPTIC];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(12, idx);
}

/* --- 17. Ethiopian index range 0-12 --- */
void test_ethiopian_index_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_ETHIOPIAN];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(12, idx);
}

/* --- 18. Persian index range 0-11 --- */
void test_persian_index_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_PERSIAN];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(11, idx);
}

/* --- 19. Buddhist index range 0-4 --- */
void test_buddhist_index_range(void) {
    ring_today_t rt = ring_today_compute(TEST_JD, TEST_SUN_LON, TEST_MOON_LON);
    int idx = rt.indices[TS_SYS_BUDDHIST];
    TEST_ASSERT_GREATER_OR_EQUAL(0, idx);
    TEST_ASSERT_LESS_OR_EQUAL(4, idx);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_gregorian_index_valid);
    RUN_TEST(test_tzolkin_index_range);
    RUN_TEST(test_iching_index_range);
    RUN_TEST(test_chinese_index_range);
    RUN_TEST(test_zodiac_from_sun_longitude);
    RUN_TEST(test_kabbalah_returns_minus_one);
    RUN_TEST(test_cosmic_returns_minus_one);
    RUN_TEST(test_all_indices_in_range);
    RUN_TEST(test_ring_today_index_accessor);
    RUN_TEST(test_ring_today_for_depth_six_entries);
    RUN_TEST(test_hebrew_index_range);
    RUN_TEST(test_islamic_index_range);
    RUN_TEST(test_nakshatra_from_moon_longitude);
    RUN_TEST(test_hd_gate_from_sun_longitude);
    RUN_TEST(test_geological_index_valid);
    RUN_TEST(test_coptic_index_range);
    RUN_TEST(test_ethiopian_index_range);
    RUN_TEST(test_persian_index_range);
    RUN_TEST(test_buddhist_index_range);
    return UNITY_END();
}
