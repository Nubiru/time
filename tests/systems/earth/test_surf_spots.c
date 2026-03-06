/* test_surf_spots.c — Tests for world surf spots data module.
 * TDD RED phase: all tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/surf_spots.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* --- Count --- */

void test_surf_spot_count_is_50(void) {
    TEST_ASSERT_EQUAL_INT(50, surf_spot_count());
}

void test_surf_spot_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(SURF_SPOT_COUNT, surf_spot_count());
}

/* --- surf_spot_get: spot 0 (Pipeline) --- */

void test_get_pipeline_id(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_EQUAL_INT(0, s.id);
}

void test_get_pipeline_name(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_EQUAL_STRING("Pipeline", s.name);
}

void test_get_pipeline_country(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_EQUAL_STRING("USA", s.country);
}

void test_get_pipeline_coords(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 21.665f, (float)s.lat);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -158.053f, (float)s.lon);
}

void test_get_pipeline_beach_facing(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 315.0f, (float)s.beach_facing_deg);
}

void test_get_pipeline_best_swell(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 315.0f, (float)s.best_swell_dir_deg);
}

void test_get_pipeline_tide(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_EQUAL_INT(TIDE_LOW, s.best_tide);
}

void test_get_pipeline_season(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_EQUAL_INT(SEASON_WINTER, s.best_season);
}

void test_get_pipeline_wave_type(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_EQUAL_INT(WAVE_REEF, s.wave_type);
}

void test_get_pipeline_description(void) {
    surf_spot_t s = surf_spot_get(0);
    TEST_ASSERT_NOT_NULL(s.description);
    TEST_ASSERT_TRUE(strlen(s.description) > 0);
}

/* --- surf_spot_get: spot 8 (Teahupo'o) --- */

void test_get_teahupoo(void) {
    surf_spot_t s = surf_spot_get(8);
    TEST_ASSERT_EQUAL_STRING("Teahupo'o", s.name);
    TEST_ASSERT_EQUAL_STRING("French Polynesia", s.country);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -17.857f, (float)s.lat);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -149.256f, (float)s.lon);
    TEST_ASSERT_EQUAL_INT(WAVE_REEF, s.wave_type);
}

/* --- surf_spot_get: spot 11 (Jeffreys Bay) --- */

void test_get_jeffreys_bay(void) {
    surf_spot_t s = surf_spot_get(11);
    TEST_ASSERT_EQUAL_STRING("Jeffreys Bay", s.name);
    TEST_ASSERT_EQUAL_STRING("South Africa", s.country);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -33.969f, (float)s.lat);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 25.532f, (float)s.lon);
    TEST_ASSERT_EQUAL_INT(WAVE_POINT, s.wave_type);
}

/* --- surf_spot_get: spot 17 (Mundaka, river mouth) --- */

void test_get_mundaka(void) {
    surf_spot_t s = surf_spot_get(17);
    TEST_ASSERT_EQUAL_STRING("Mundaka", s.name);
    TEST_ASSERT_EQUAL_STRING("Spain", s.country);
    TEST_ASSERT_EQUAL_INT(WAVE_RIVER_MOUTH, s.wave_type);
}

/* --- surf_spot_get: spot 49 (Canggu, last spot) --- */

void test_get_canggu(void) {
    surf_spot_t s = surf_spot_get(49);
    TEST_ASSERT_EQUAL_INT(49, s.id);
    TEST_ASSERT_EQUAL_STRING("Canggu", s.name);
    TEST_ASSERT_EQUAL_STRING("Indonesia", s.country);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -8.649f, (float)s.lat);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 115.123f, (float)s.lon);
    TEST_ASSERT_EQUAL_INT(WAVE_BEACH, s.wave_type);
}

/* --- surf_spot_get: out of bounds --- */

void test_get_negative_index_returns_id_minus1(void) {
    surf_spot_t s = surf_spot_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
}

void test_get_over_max_returns_id_minus1(void) {
    surf_spot_t s = surf_spot_get(50);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
}

/* --- surf_spot_name --- */

void test_name_returns_pipeline(void) {
    TEST_ASSERT_EQUAL_STRING("Pipeline", surf_spot_name(0));
}

void test_name_returns_canggu(void) {
    TEST_ASSERT_EQUAL_STRING("Canggu", surf_spot_name(49));
}

void test_name_returns_null_for_invalid(void) {
    TEST_ASSERT_NULL(surf_spot_name(-1));
    TEST_ASSERT_NULL(surf_spot_name(50));
}

/* --- surf_spot_nearest (Haversine) --- */

void test_nearest_to_pipeline_coords(void) {
    /* Pipeline is at 21.665, -158.053 */
    int idx = surf_spot_nearest(21.665, -158.053);
    /* Could be 0 (Pipeline) or 39 (Backdoor) — same coords */
    surf_spot_t s = surf_spot_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 21.665f, (float)s.lat);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -158.053f, (float)s.lon);
}

void test_nearest_to_jeffreys_bay(void) {
    /* Slightly off from J-Bay: -34.0, 25.5 */
    int idx = surf_spot_nearest(-34.0, 25.5);
    TEST_ASSERT_EQUAL_INT(11, idx);
}

void test_nearest_to_bells_beach(void) {
    /* Near Bells Beach, Australia */
    int idx = surf_spot_nearest(-38.4, 144.3);
    TEST_ASSERT_EQUAL_INT(25, idx);
}

void test_nearest_to_nazare(void) {
    /* Near Nazare, Portugal */
    int idx = surf_spot_nearest(39.6, -9.1);
    TEST_ASSERT_EQUAL_INT(14, idx);
}

/* --- surf_spot_tide_quality --- */

void test_tide_quality_all_always_1(void) {
    /* Sunset Beach (index 1) has TIDE_ALL */
    float q = (float)surf_spot_tide_quality(1, -1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, q);
    q = (float)surf_spot_tide_quality(1, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, q);
    q = (float)surf_spot_tide_quality(1, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, q);
}

void test_tide_quality_low_peaks_at_minus1(void) {
    /* Pipeline (index 0) prefers TIDE_LOW */
    float q = (float)surf_spot_tide_quality(0, -1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, q);
}

void test_tide_quality_low_zero_at_high(void) {
    /* Pipeline (index 0) prefers TIDE_LOW */
    float q = (float)surf_spot_tide_quality(0, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, q);
}

void test_tide_quality_mid_peaks_at_zero(void) {
    /* Mavericks (index 4) prefers TIDE_MID */
    float q = (float)surf_spot_tide_quality(4, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, q);
}

void test_tide_quality_mid_zero_at_extremes(void) {
    /* Mavericks (index 4) prefers TIDE_MID */
    float q_low = (float)surf_spot_tide_quality(4, -1.0);
    float q_high = (float)surf_spot_tide_quality(4, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, q_low);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, q_high);
}

void test_tide_quality_high_peaks_at_plus1(void) {
    /* There are no TIDE_HIGH spots in the dataset, but test formula via out-of-bounds -> returns 0 */
    /* Instead, test clamping: invalid index returns 0 */
    float q = (float)surf_spot_tide_quality(-1, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, q);
}

void test_tide_quality_clamped_0_to_1(void) {
    /* TIDE_LOW at mid tide should be 0.0 (clamped, not negative) */
    float q = (float)surf_spot_tide_quality(0, 0.0);
    TEST_ASSERT_TRUE(q >= 0.0f && q <= 1.0f);
}

/* --- surf_spot_swell_quality --- */

void test_swell_quality_perfect_match(void) {
    /* Pipeline (index 0), best_swell_dir = 315, send 315 */
    float q = (float)surf_spot_swell_quality(0, 315.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, q);
}

void test_swell_quality_45_off(void) {
    /* Pipeline (index 0), best_swell_dir = 315, send 0 (diff = 45) */
    float q = (float)surf_spot_swell_quality(0, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, q);
}

void test_swell_quality_90_off_is_zero(void) {
    /* Pipeline (index 0), best_swell_dir = 315, send 225 (diff = 90) */
    float q = (float)surf_spot_swell_quality(0, 225.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, q);
}

void test_swell_quality_wraps_around_360(void) {
    /* Pipeline (index 0), best_swell_dir = 315, send 350 (diff = 35) */
    float q = (float)surf_spot_swell_quality(0, 350.0);
    TEST_ASSERT_TRUE(q > 0.5f);
}

void test_swell_quality_invalid_index(void) {
    float q = (float)surf_spot_swell_quality(-1, 315.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, q);
}

/* --- surf_spots_by_type --- */

void test_by_type_reef_count(void) {
    int indices[SURF_SPOT_COUNT];
    int n = surf_spots_by_type(WAVE_REEF, indices, SURF_SPOT_COUNT);
    /* Reef: 0,1,3,4,8,9,10,12,18,19,20,21,22,23,24,25,27,28,30,33,35,36,37,38,39,42,44,45,46,48 = 30 */
    TEST_ASSERT_EQUAL_INT(30, n);
}

void test_by_type_beach_count(void) {
    int indices[SURF_SPOT_COUNT];
    int n = surf_spots_by_type(WAVE_BEACH, indices, SURF_SPOT_COUNT);
    /* Beach: 2,13,14,15,16,29,43,49 = 8 */
    TEST_ASSERT_EQUAL_INT(8, n);
}

void test_by_type_point_count(void) {
    int indices[SURF_SPOT_COUNT];
    int n = surf_spots_by_type(WAVE_POINT, indices, SURF_SPOT_COUNT);
    /* Point: 5,6,7,11,26,31,32,34,40,41,47 = 11 */
    TEST_ASSERT_EQUAL_INT(11, n);
}

void test_by_type_river_mouth_count(void) {
    int indices[SURF_SPOT_COUNT];
    int n = surf_spots_by_type(WAVE_RIVER_MOUTH, indices, SURF_SPOT_COUNT);
    /* River mouth: 17 = 1 */
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_INT(17, indices[0]);
}

void test_by_type_limited_output(void) {
    int indices[3];
    int n = surf_spots_by_type(WAVE_REEF, indices, 3);
    TEST_ASSERT_EQUAL_INT(3, n);
}

/* --- surf_spots_in_region --- */

void test_region_hawaii(void) {
    /* Hawaii region: lat 20-22, lon -159 to -156 */
    int indices[SURF_SPOT_COUNT];
    int n = surf_spots_in_region(20.0, 22.0, -159.0, -156.0,
                                 indices, SURF_SPOT_COUNT);
    /* Spots in Hawaii: 0,1,2,3,39,40 — check at least Pipeline, Sunset, Waimea */
    TEST_ASSERT_TRUE(n >= 5);
}

void test_region_bali(void) {
    /* Bali region: lat -9.0 to -8.5, lon 115.0 to 115.5 */
    int indices[SURF_SPOT_COUNT];
    int n = surf_spots_in_region(-9.0, -8.5, 115.0, 115.5,
                                 indices, SURF_SPOT_COUNT);
    /* Uluwatu(20), Padang(21), Keramas(36), Canggu(49) */
    TEST_ASSERT_TRUE(n >= 3);
}

void test_region_empty(void) {
    /* Middle of the Sahara */
    int indices[SURF_SPOT_COUNT];
    int n = surf_spots_in_region(25.0, 26.0, 10.0, 11.0,
                                 indices, SURF_SPOT_COUNT);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_region_limited_output(void) {
    int indices[2];
    int n = surf_spots_in_region(-90.0, 90.0, -180.0, 180.0,
                                 indices, 2);
    TEST_ASSERT_EQUAL_INT(2, n);
}

/* --- Name lookups for enums --- */

void test_tide_name_low(void) {
    TEST_ASSERT_EQUAL_STRING("Low", surf_tide_name(TIDE_LOW));
}

void test_tide_name_mid(void) {
    TEST_ASSERT_EQUAL_STRING("Mid", surf_tide_name(TIDE_MID));
}

void test_tide_name_high(void) {
    TEST_ASSERT_EQUAL_STRING("High", surf_tide_name(TIDE_HIGH));
}

void test_tide_name_all(void) {
    TEST_ASSERT_EQUAL_STRING("All", surf_tide_name(TIDE_ALL));
}

void test_season_name_winter(void) {
    TEST_ASSERT_EQUAL_STRING("Winter", surf_season_name(SEASON_WINTER));
}

void test_season_name_spring(void) {
    TEST_ASSERT_EQUAL_STRING("Spring", surf_season_name(SEASON_SPRING));
}

void test_season_name_summer(void) {
    TEST_ASSERT_EQUAL_STRING("Summer", surf_season_name(SEASON_SUMMER));
}

void test_season_name_autumn(void) {
    TEST_ASSERT_EQUAL_STRING("Autumn", surf_season_name(SEASON_AUTUMN));
}

void test_season_name_all(void) {
    TEST_ASSERT_EQUAL_STRING("All", surf_season_name(SEASON_ALL));
}

void test_wave_type_name_reef(void) {
    TEST_ASSERT_EQUAL_STRING("Reef", surf_wave_type_name(WAVE_REEF));
}

void test_wave_type_name_beach(void) {
    TEST_ASSERT_EQUAL_STRING("Beach", surf_wave_type_name(WAVE_BEACH));
}

void test_wave_type_name_point(void) {
    TEST_ASSERT_EQUAL_STRING("Point", surf_wave_type_name(WAVE_POINT));
}

void test_wave_type_name_river_mouth(void) {
    TEST_ASSERT_EQUAL_STRING("River Mouth", surf_wave_type_name(WAVE_RIVER_MOUTH));
}

/* --- All spots have valid data --- */

void test_all_spots_have_names(void) {
    for (int i = 0; i < SURF_SPOT_COUNT; i++) {
        surf_spot_t s = surf_spot_get(i);
        TEST_ASSERT_NOT_NULL(s.name);
        TEST_ASSERT_TRUE(strlen(s.name) > 0);
    }
}

void test_all_spots_have_countries(void) {
    for (int i = 0; i < SURF_SPOT_COUNT; i++) {
        surf_spot_t s = surf_spot_get(i);
        TEST_ASSERT_NOT_NULL(s.country);
        TEST_ASSERT_TRUE(strlen(s.country) > 0);
    }
}

void test_all_spots_valid_coords(void) {
    for (int i = 0; i < SURF_SPOT_COUNT; i++) {
        surf_spot_t s = surf_spot_get(i);
        TEST_ASSERT_TRUE(s.lat >= -90.0 && s.lat <= 90.0);
        TEST_ASSERT_TRUE(s.lon >= -180.0 && s.lon <= 180.0);
    }
}

void test_all_spots_valid_directions(void) {
    for (int i = 0; i < SURF_SPOT_COUNT; i++) {
        surf_spot_t s = surf_spot_get(i);
        TEST_ASSERT_TRUE(s.beach_facing_deg >= 0.0 && s.beach_facing_deg < 360.0);
        TEST_ASSERT_TRUE(s.best_swell_dir_deg >= 0.0 && s.best_swell_dir_deg < 360.0);
    }
}

void test_all_spots_have_descriptions(void) {
    for (int i = 0; i < SURF_SPOT_COUNT; i++) {
        surf_spot_t s = surf_spot_get(i);
        TEST_ASSERT_NOT_NULL(s.description);
        TEST_ASSERT_TRUE(strlen(s.description) > 0);
    }
}

void test_all_type_counts_sum_to_50(void) {
    int indices[SURF_SPOT_COUNT];
    int reef = surf_spots_by_type(WAVE_REEF, indices, SURF_SPOT_COUNT);
    int beach = surf_spots_by_type(WAVE_BEACH, indices, SURF_SPOT_COUNT);
    int point = surf_spots_by_type(WAVE_POINT, indices, SURF_SPOT_COUNT);
    int river = surf_spots_by_type(WAVE_RIVER_MOUTH, indices, SURF_SPOT_COUNT);
    TEST_ASSERT_EQUAL_INT(SURF_SPOT_COUNT, reef + beach + point + river);
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_surf_spot_count_is_50);
    RUN_TEST(test_surf_spot_count_matches_define);

    /* Get: Pipeline */
    RUN_TEST(test_get_pipeline_id);
    RUN_TEST(test_get_pipeline_name);
    RUN_TEST(test_get_pipeline_country);
    RUN_TEST(test_get_pipeline_coords);
    RUN_TEST(test_get_pipeline_beach_facing);
    RUN_TEST(test_get_pipeline_best_swell);
    RUN_TEST(test_get_pipeline_tide);
    RUN_TEST(test_get_pipeline_season);
    RUN_TEST(test_get_pipeline_wave_type);
    RUN_TEST(test_get_pipeline_description);

    /* Get: specific spots */
    RUN_TEST(test_get_teahupoo);
    RUN_TEST(test_get_jeffreys_bay);
    RUN_TEST(test_get_mundaka);
    RUN_TEST(test_get_canggu);

    /* Get: bounds */
    RUN_TEST(test_get_negative_index_returns_id_minus1);
    RUN_TEST(test_get_over_max_returns_id_minus1);

    /* Name */
    RUN_TEST(test_name_returns_pipeline);
    RUN_TEST(test_name_returns_canggu);
    RUN_TEST(test_name_returns_null_for_invalid);

    /* Nearest */
    RUN_TEST(test_nearest_to_pipeline_coords);
    RUN_TEST(test_nearest_to_jeffreys_bay);
    RUN_TEST(test_nearest_to_bells_beach);
    RUN_TEST(test_nearest_to_nazare);

    /* Tide quality */
    RUN_TEST(test_tide_quality_all_always_1);
    RUN_TEST(test_tide_quality_low_peaks_at_minus1);
    RUN_TEST(test_tide_quality_low_zero_at_high);
    RUN_TEST(test_tide_quality_mid_peaks_at_zero);
    RUN_TEST(test_tide_quality_mid_zero_at_extremes);
    RUN_TEST(test_tide_quality_high_peaks_at_plus1);
    RUN_TEST(test_tide_quality_clamped_0_to_1);

    /* Swell quality */
    RUN_TEST(test_swell_quality_perfect_match);
    RUN_TEST(test_swell_quality_45_off);
    RUN_TEST(test_swell_quality_90_off_is_zero);
    RUN_TEST(test_swell_quality_wraps_around_360);
    RUN_TEST(test_swell_quality_invalid_index);

    /* By type */
    RUN_TEST(test_by_type_reef_count);
    RUN_TEST(test_by_type_beach_count);
    RUN_TEST(test_by_type_point_count);
    RUN_TEST(test_by_type_river_mouth_count);
    RUN_TEST(test_by_type_limited_output);

    /* Region */
    RUN_TEST(test_region_hawaii);
    RUN_TEST(test_region_bali);
    RUN_TEST(test_region_empty);
    RUN_TEST(test_region_limited_output);

    /* Enum names */
    RUN_TEST(test_tide_name_low);
    RUN_TEST(test_tide_name_mid);
    RUN_TEST(test_tide_name_high);
    RUN_TEST(test_tide_name_all);
    RUN_TEST(test_season_name_winter);
    RUN_TEST(test_season_name_spring);
    RUN_TEST(test_season_name_summer);
    RUN_TEST(test_season_name_autumn);
    RUN_TEST(test_season_name_all);
    RUN_TEST(test_wave_type_name_reef);
    RUN_TEST(test_wave_type_name_beach);
    RUN_TEST(test_wave_type_name_point);
    RUN_TEST(test_wave_type_name_river_mouth);

    /* Data integrity */
    RUN_TEST(test_all_spots_have_names);
    RUN_TEST(test_all_spots_have_countries);
    RUN_TEST(test_all_spots_valid_coords);
    RUN_TEST(test_all_spots_valid_directions);
    RUN_TEST(test_all_spots_have_descriptions);
    RUN_TEST(test_all_type_counts_sum_to_50);

    return UNITY_END();
}
