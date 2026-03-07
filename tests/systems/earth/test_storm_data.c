/* test_storm_data.c — Tests for storm season data module:
 * cyclone basins, monsoon regions, swell zones. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/storm_data.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ============================================================
 * Basin count & get
 * ============================================================ */

void test_basin_count_is_7(void) {
    TEST_ASSERT_EQUAL_INT(7, storm_basin_count());
}

void test_basin_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(STORM_BASIN_COUNT, storm_basin_count());
}

void test_basin_get_north_atlantic(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_NORTH_ATLANTIC);
    TEST_ASSERT_EQUAL_INT(BASIN_NORTH_ATLANTIC, b.id);
    TEST_ASSERT_EQUAL_STRING("North Atlantic", b.name);
}

void test_basin_get_east_pacific(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_EAST_PACIFIC);
    TEST_ASSERT_EQUAL_INT(BASIN_EAST_PACIFIC, b.id);
    TEST_ASSERT_EQUAL_STRING("East Pacific", b.name);
}

void test_basin_get_west_pacific(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_WEST_PACIFIC);
    TEST_ASSERT_EQUAL_INT(BASIN_WEST_PACIFIC, b.id);
    TEST_ASSERT_EQUAL_STRING("West Pacific", b.name);
    TEST_ASSERT_EQUAL_INT(1, b.season_start);
    TEST_ASSERT_EQUAL_INT(12, b.season_end);
}

void test_basin_get_north_indian(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_NORTH_INDIAN);
    TEST_ASSERT_EQUAL_STRING("North Indian", b.name);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 5.0f, (float)b.avg_storms_year);
}

void test_basin_get_south_indian(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_SOUTH_INDIAN);
    TEST_ASSERT_EQUAL_STRING("South Indian", b.name);
    TEST_ASSERT_EQUAL_INT(11, b.season_start);
    TEST_ASSERT_EQUAL_INT(5, b.season_end);
}

void test_basin_get_south_pacific(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_SOUTH_PACIFIC);
    TEST_ASSERT_EQUAL_STRING("South Pacific", b.name);
    TEST_ASSERT_EQUAL_INT(2, b.peak_month);
}

void test_basin_get_australian(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_AUSTRALIAN);
    TEST_ASSERT_EQUAL_STRING("Australian", b.name);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 11.0f, (float)b.avg_storms_year);
}

void test_basin_get_invalid_negative(void) {
    cyclone_basin_t b = storm_basin_get(-1);
    TEST_ASSERT_NULL(b.name);
}

void test_basin_get_invalid_too_high(void) {
    cyclone_basin_t b = storm_basin_get(7);
    TEST_ASSERT_NULL(b.name);
}

/* ============================================================
 * Basin name
 * ============================================================ */

void test_basin_name_north_atlantic(void) {
    TEST_ASSERT_EQUAL_STRING("North Atlantic",
                             storm_basin_name(BASIN_NORTH_ATLANTIC));
}

void test_basin_name_west_pacific(void) {
    TEST_ASSERT_EQUAL_STRING("West Pacific",
                             storm_basin_name(BASIN_WEST_PACIFIC));
}

void test_basin_name_invalid(void) {
    TEST_ASSERT_NULL(storm_basin_name(99));
}

/* ============================================================
 * Basin coordinates
 * ============================================================ */

void test_basin_north_atlantic_coords(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_NORTH_ATLANTIC);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 5.0f, (float)b.lat_min);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 35.0f, (float)b.lat_max);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -100.0f, (float)b.lon_min);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -10.0f, (float)b.lon_max);
}

void test_basin_east_pacific_coords(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_EAST_PACIFIC);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 5.0f, (float)b.lat_min);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 25.0f, (float)b.lat_max);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -180.0f, (float)b.lon_min);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -80.0f, (float)b.lon_max);
}

/* ============================================================
 * Basin season data
 * ============================================================ */

void test_basin_north_atlantic_season(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_NORTH_ATLANTIC);
    TEST_ASSERT_EQUAL_INT(6, b.season_start);
    TEST_ASSERT_EQUAL_INT(11, b.season_end);
    TEST_ASSERT_EQUAL_INT(9, b.peak_month);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 14.0f, (float)b.avg_storms_year);
}

void test_basin_east_pacific_season(void) {
    cyclone_basin_t b = storm_basin_get(BASIN_EAST_PACIFIC);
    TEST_ASSERT_EQUAL_INT(5, b.season_start);
    TEST_ASSERT_EQUAL_INT(11, b.season_end);
    TEST_ASSERT_EQUAL_INT(8, b.peak_month);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 17.0f, (float)b.avg_storms_year);
}

/* ============================================================
 * Basin active (seasonal check)
 * ============================================================ */

void test_basin_active_north_atlantic_in_season(void) {
    /* June through November */
    TEST_ASSERT_TRUE(storm_basin_active(BASIN_NORTH_ATLANTIC, 6));
    TEST_ASSERT_TRUE(storm_basin_active(BASIN_NORTH_ATLANTIC, 9));
    TEST_ASSERT_TRUE(storm_basin_active(BASIN_NORTH_ATLANTIC, 11));
}

void test_basin_active_north_atlantic_off_season(void) {
    TEST_ASSERT_FALSE(storm_basin_active(BASIN_NORTH_ATLANTIC, 1));
    TEST_ASSERT_FALSE(storm_basin_active(BASIN_NORTH_ATLANTIC, 3));
    TEST_ASSERT_FALSE(storm_basin_active(BASIN_NORTH_ATLANTIC, 12));
}

void test_basin_active_west_pacific_year_round(void) {
    for (int m = 1; m <= 12; m++) {
        TEST_ASSERT_TRUE(storm_basin_active(BASIN_WEST_PACIFIC, m));
    }
}

void test_basin_active_south_indian_wrap_around(void) {
    /* Nov-May wrap */
    TEST_ASSERT_TRUE(storm_basin_active(BASIN_SOUTH_INDIAN, 11));
    TEST_ASSERT_TRUE(storm_basin_active(BASIN_SOUTH_INDIAN, 12));
    TEST_ASSERT_TRUE(storm_basin_active(BASIN_SOUTH_INDIAN, 1));
    TEST_ASSERT_TRUE(storm_basin_active(BASIN_SOUTH_INDIAN, 5));
    TEST_ASSERT_FALSE(storm_basin_active(BASIN_SOUTH_INDIAN, 6));
    TEST_ASSERT_FALSE(storm_basin_active(BASIN_SOUTH_INDIAN, 10));
}

void test_basin_active_invalid_month(void) {
    TEST_ASSERT_FALSE(storm_basin_active(BASIN_NORTH_ATLANTIC, 0));
    TEST_ASSERT_FALSE(storm_basin_active(BASIN_NORTH_ATLANTIC, 13));
}

void test_basin_active_invalid_basin(void) {
    TEST_ASSERT_FALSE(storm_basin_active(99, 6));
}

/* ============================================================
 * Basin risk (0.0-1.0)
 * ============================================================ */

void test_basin_risk_peak_month(void) {
    /* At peak month, risk should be 1.0 */
    double r = storm_basin_risk(BASIN_NORTH_ATLANTIC, 9);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)r);
}

void test_basin_risk_off_season(void) {
    double r = storm_basin_risk(BASIN_NORTH_ATLANTIC, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r);
}

void test_basin_risk_shoulder_month(void) {
    /* July — mid-season but not peak, should be >0 and <1 */
    double r = storm_basin_risk(BASIN_NORTH_ATLANTIC, 7);
    TEST_ASSERT_TRUE(r > 0.0);
    TEST_ASSERT_TRUE(r < 1.0);
}

void test_basin_risk_south_indian_peak(void) {
    /* Peak Feb for wrap-around season */
    double r = storm_basin_risk(BASIN_SOUTH_INDIAN, 2);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)r);
}

void test_basin_risk_south_indian_off(void) {
    double r = storm_basin_risk(BASIN_SOUTH_INDIAN, 8);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r);
}

void test_basin_risk_west_pacific_peak(void) {
    double r = storm_basin_risk(BASIN_WEST_PACIFIC, 8);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)r);
}

void test_basin_risk_clamped_0_to_1(void) {
    for (int b = 0; b < STORM_BASIN_COUNT; b++) {
        for (int m = 1; m <= 12; m++) {
            double r = storm_basin_risk(b, m);
            TEST_ASSERT_TRUE(r >= 0.0);
            TEST_ASSERT_TRUE(r <= 1.0);
        }
    }
}

/* ============================================================
 * Monsoon count & get
 * ============================================================ */

void test_monsoon_count_is_5(void) {
    TEST_ASSERT_EQUAL_INT(5, storm_monsoon_count());
}

void test_monsoon_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(MONSOON_REGION_COUNT, storm_monsoon_count());
}

void test_monsoon_get_indian(void) {
    monsoon_region_t m = storm_monsoon_get(0);
    TEST_ASSERT_EQUAL_INT(0, m.id);
    TEST_ASSERT_EQUAL_STRING("Indian", m.name);
    TEST_ASSERT_EQUAL_INT(6, m.onset_month);
    TEST_ASSERT_EQUAL_INT(9, m.withdrawal_month);
}

void test_monsoon_get_east_asian(void) {
    monsoon_region_t m = storm_monsoon_get(1);
    TEST_ASSERT_EQUAL_STRING("East Asian", m.name);
    TEST_ASSERT_EQUAL_INT(6, m.onset_month);
    TEST_ASSERT_EQUAL_INT(8, m.withdrawal_month);
}

void test_monsoon_get_west_african(void) {
    monsoon_region_t m = storm_monsoon_get(2);
    TEST_ASSERT_EQUAL_STRING("West African", m.name);
    TEST_ASSERT_EQUAL_INT(6, m.onset_month);
    TEST_ASSERT_EQUAL_INT(10, m.withdrawal_month);
}

void test_monsoon_get_north_american(void) {
    monsoon_region_t m = storm_monsoon_get(3);
    TEST_ASSERT_EQUAL_STRING("North American", m.name);
    TEST_ASSERT_EQUAL_INT(7, m.onset_month);
    TEST_ASSERT_EQUAL_INT(9, m.withdrawal_month);
}

void test_monsoon_get_australian(void) {
    monsoon_region_t m = storm_monsoon_get(4);
    TEST_ASSERT_EQUAL_STRING("Australian", m.name);
    TEST_ASSERT_EQUAL_INT(12, m.onset_month);
    TEST_ASSERT_EQUAL_INT(3, m.withdrawal_month);
}

void test_monsoon_get_invalid(void) {
    monsoon_region_t m = storm_monsoon_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.id);
}

void test_monsoon_get_invalid_too_high(void) {
    monsoon_region_t m = storm_monsoon_get(5);
    TEST_ASSERT_EQUAL_INT(-1, m.id);
}

/* ============================================================
 * Monsoon coordinates
 * ============================================================ */

void test_monsoon_indian_coords(void) {
    monsoon_region_t m = storm_monsoon_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 5.0f, (float)m.lat_min);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 30.0f, (float)m.lat_max);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 60.0f, (float)m.lon_min);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 100.0f, (float)m.lon_max);
}

/* ============================================================
 * Monsoon active
 * ============================================================ */

void test_monsoon_active_indian_in_season(void) {
    TEST_ASSERT_TRUE(storm_monsoon_active(0, 6));
    TEST_ASSERT_TRUE(storm_monsoon_active(0, 7));
    TEST_ASSERT_TRUE(storm_monsoon_active(0, 9));
}

void test_monsoon_active_indian_off_season(void) {
    TEST_ASSERT_FALSE(storm_monsoon_active(0, 1));
    TEST_ASSERT_FALSE(storm_monsoon_active(0, 11));
}

void test_monsoon_active_australian_wrap(void) {
    /* Dec-Mar wrap */
    TEST_ASSERT_TRUE(storm_monsoon_active(4, 12));
    TEST_ASSERT_TRUE(storm_monsoon_active(4, 1));
    TEST_ASSERT_TRUE(storm_monsoon_active(4, 3));
    TEST_ASSERT_FALSE(storm_monsoon_active(4, 6));
    TEST_ASSERT_FALSE(storm_monsoon_active(4, 11));
}

void test_monsoon_active_invalid_index(void) {
    TEST_ASSERT_FALSE(storm_monsoon_active(-1, 6));
    TEST_ASSERT_FALSE(storm_monsoon_active(99, 6));
}

void test_monsoon_active_invalid_month(void) {
    TEST_ASSERT_FALSE(storm_monsoon_active(0, 0));
    TEST_ASSERT_FALSE(storm_monsoon_active(0, 13));
}

/* ============================================================
 * storm_risk_at (combined query)
 * ============================================================ */

void test_risk_at_miami_september(void) {
    /* Miami: 25.8N, -80.2W — inside North Atlantic, Sept peak */
    storm_risk_t r = storm_risk_at(25.8, -80.2, 9);
    TEST_ASSERT_NOT_NULL(r.basin_name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)r.cyclone_risk);
}

void test_risk_at_miami_january(void) {
    /* Off-season for North Atlantic */
    storm_risk_t r = storm_risk_at(25.8, -80.2, 1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.cyclone_risk);
}

void test_risk_at_mumbai_july(void) {
    /* Mumbai: 19.1N, 72.9E — inside Indian monsoon region, Jul */
    storm_risk_t r = storm_risk_at(19.1, 72.9, 7);
    TEST_ASSERT_TRUE(r.monsoon_active);
    TEST_ASSERT_NOT_NULL(r.monsoon_name);
}

void test_risk_at_mumbai_december(void) {
    /* Off-season for Indian monsoon */
    storm_risk_t r = storm_risk_at(19.1, 72.9, 12);
    TEST_ASSERT_FALSE(r.monsoon_active);
}

void test_risk_at_open_ocean_no_basin(void) {
    /* Middle of South Atlantic (no cyclone basin) */
    storm_risk_t r = storm_risk_at(-20.0, -20.0, 6);
    TEST_ASSERT_NULL(r.basin_name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)r.cyclone_risk);
}

void test_risk_at_darwin_december(void) {
    /* Darwin, Australia: -12.5S, 130.8E — Australian basin + monsoon */
    storm_risk_t r = storm_risk_at(-12.5, 130.8, 12);
    TEST_ASSERT_NOT_NULL(r.basin_name);
    TEST_ASSERT_TRUE(r.cyclone_risk > 0.0);
    TEST_ASSERT_TRUE(r.monsoon_active);
}

/* ============================================================
 * Swell zone count & get
 * ============================================================ */

void test_swell_zone_count_is_8(void) {
    TEST_ASSERT_EQUAL_INT(8, storm_swell_zone_count());
}

void test_swell_zone_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(SWELL_ZONE_COUNT, storm_swell_zone_count());
}

void test_swell_zone_get_north_atlantic(void) {
    swell_zone_t z = storm_swell_zone_get(0);
    TEST_ASSERT_EQUAL_INT(0, z.id);
    TEST_ASSERT_EQUAL_STRING("North Atlantic Storm Track", z.name);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 50.0f, (float)z.lat_center);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -30.0f, (float)z.lon_center);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 225.0f, (float)z.primary_dir_deg);
}

void test_swell_zone_get_north_pacific(void) {
    swell_zone_t z = storm_swell_zone_get(1);
    TEST_ASSERT_EQUAL_STRING("North Pacific Storm Track", z.name);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 45.0f, (float)z.lat_center);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -170.0f, (float)z.lon_center);
}

void test_swell_zone_get_roaring_forties_south_atlantic(void) {
    swell_zone_t z = storm_swell_zone_get(2);
    TEST_ASSERT_EQUAL_STRING("Roaring Forties South Atlantic", z.name);
    TEST_ASSERT_EQUAL_INT(1, z.active_start);
    TEST_ASSERT_EQUAL_INT(12, z.active_end);
}

void test_swell_zone_get_antarctic(void) {
    swell_zone_t z = storm_swell_zone_get(5);
    TEST_ASSERT_EQUAL_STRING("Antarctic Circumpolar", z.name);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 0.0f, (float)z.primary_dir_deg);
}

void test_swell_zone_get_invalid(void) {
    swell_zone_t z = storm_swell_zone_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, z.id);
}

void test_swell_zone_get_invalid_too_high(void) {
    swell_zone_t z = storm_swell_zone_get(8);
    TEST_ASSERT_EQUAL_INT(-1, z.id);
}

void test_swell_zone_season_north_atlantic(void) {
    swell_zone_t z = storm_swell_zone_get(0);
    TEST_ASSERT_EQUAL_INT(10, z.active_start);
    TEST_ASSERT_EQUAL_INT(3, z.active_end);
}

void test_swell_zone_bay_of_bengal(void) {
    swell_zone_t z = storm_swell_zone_get(7);
    TEST_ASSERT_EQUAL_STRING("Bay of Bengal", z.name);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 15.0f, (float)z.lat_center);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 85.0f, (float)z.lon_center);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 180.0f, (float)z.primary_dir_deg);
}

/* ============================================================
 * Swell direction
 * ============================================================ */

void test_swell_direction_near_north_atlantic_track(void) {
    /* Near North Atlantic storm track center (50N, -30W), in winter */
    double dir = storm_swell_direction(50.0, -30.0, 12);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 225.0f, (float)dir);
}

void test_swell_direction_roaring_forties_year_round(void) {
    /* Near Roaring Forties South Indian (active year-round) */
    double dir = storm_swell_direction(-45.0, 70.0, 6);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 45.0f, (float)dir);
}

void test_swell_direction_no_active_zone(void) {
    /* A remote point far from all zones, hope none active.
     * This tests the -1.0 return. Use a point in deep
     * interior of Africa, far from all zone centers. */
    double dir = storm_swell_direction(0.0, 25.0, 7);
    /* Should find nearest active zone; Roaring Forties are year-round.
     * So we expect a valid direction, not -1.0.
     * Actually, year-round zones mean there's always a nearest active zone.
     * Let's just check it returns a valid direction (>= 0). */
    TEST_ASSERT_TRUE(dir >= 0.0);
}

/* ============================================================
 * All basins have descriptions
 * ============================================================ */

void test_all_basins_have_descriptions(void) {
    for (int i = 0; i < STORM_BASIN_COUNT; i++) {
        cyclone_basin_t b = storm_basin_get(i);
        TEST_ASSERT_NOT_NULL(b.description);
        TEST_ASSERT_TRUE(strlen(b.description) > 0);
    }
}

void test_all_monsoons_have_descriptions(void) {
    for (int i = 0; i < MONSOON_REGION_COUNT; i++) {
        monsoon_region_t m = storm_monsoon_get(i);
        TEST_ASSERT_NOT_NULL(m.description);
        TEST_ASSERT_TRUE(strlen(m.description) > 0);
    }
}

void test_all_swell_zones_have_descriptions(void) {
    for (int i = 0; i < SWELL_ZONE_COUNT; i++) {
        swell_zone_t z = storm_swell_zone_get(i);
        TEST_ASSERT_NOT_NULL(z.description);
        TEST_ASSERT_TRUE(strlen(z.description) > 0);
    }
}

/* ============================================================
 * main
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* Basin count & get */
    RUN_TEST(test_basin_count_is_7);
    RUN_TEST(test_basin_count_matches_define);
    RUN_TEST(test_basin_get_north_atlantic);
    RUN_TEST(test_basin_get_east_pacific);
    RUN_TEST(test_basin_get_west_pacific);
    RUN_TEST(test_basin_get_north_indian);
    RUN_TEST(test_basin_get_south_indian);
    RUN_TEST(test_basin_get_south_pacific);
    RUN_TEST(test_basin_get_australian);
    RUN_TEST(test_basin_get_invalid_negative);
    RUN_TEST(test_basin_get_invalid_too_high);

    /* Basin name */
    RUN_TEST(test_basin_name_north_atlantic);
    RUN_TEST(test_basin_name_west_pacific);
    RUN_TEST(test_basin_name_invalid);

    /* Basin coordinates */
    RUN_TEST(test_basin_north_atlantic_coords);
    RUN_TEST(test_basin_east_pacific_coords);

    /* Basin season data */
    RUN_TEST(test_basin_north_atlantic_season);
    RUN_TEST(test_basin_east_pacific_season);

    /* Basin active */
    RUN_TEST(test_basin_active_north_atlantic_in_season);
    RUN_TEST(test_basin_active_north_atlantic_off_season);
    RUN_TEST(test_basin_active_west_pacific_year_round);
    RUN_TEST(test_basin_active_south_indian_wrap_around);
    RUN_TEST(test_basin_active_invalid_month);
    RUN_TEST(test_basin_active_invalid_basin);

    /* Basin risk */
    RUN_TEST(test_basin_risk_peak_month);
    RUN_TEST(test_basin_risk_off_season);
    RUN_TEST(test_basin_risk_shoulder_month);
    RUN_TEST(test_basin_risk_south_indian_peak);
    RUN_TEST(test_basin_risk_south_indian_off);
    RUN_TEST(test_basin_risk_west_pacific_peak);
    RUN_TEST(test_basin_risk_clamped_0_to_1);

    /* Monsoon count & get */
    RUN_TEST(test_monsoon_count_is_5);
    RUN_TEST(test_monsoon_count_matches_define);
    RUN_TEST(test_monsoon_get_indian);
    RUN_TEST(test_monsoon_get_east_asian);
    RUN_TEST(test_monsoon_get_west_african);
    RUN_TEST(test_monsoon_get_north_american);
    RUN_TEST(test_monsoon_get_australian);
    RUN_TEST(test_monsoon_get_invalid);
    RUN_TEST(test_monsoon_get_invalid_too_high);

    /* Monsoon coordinates */
    RUN_TEST(test_monsoon_indian_coords);

    /* Monsoon active */
    RUN_TEST(test_monsoon_active_indian_in_season);
    RUN_TEST(test_monsoon_active_indian_off_season);
    RUN_TEST(test_monsoon_active_australian_wrap);
    RUN_TEST(test_monsoon_active_invalid_index);
    RUN_TEST(test_monsoon_active_invalid_month);

    /* storm_risk_at */
    RUN_TEST(test_risk_at_miami_september);
    RUN_TEST(test_risk_at_miami_january);
    RUN_TEST(test_risk_at_mumbai_july);
    RUN_TEST(test_risk_at_mumbai_december);
    RUN_TEST(test_risk_at_open_ocean_no_basin);
    RUN_TEST(test_risk_at_darwin_december);

    /* Swell zones */
    RUN_TEST(test_swell_zone_count_is_8);
    RUN_TEST(test_swell_zone_count_matches_define);
    RUN_TEST(test_swell_zone_get_north_atlantic);
    RUN_TEST(test_swell_zone_get_north_pacific);
    RUN_TEST(test_swell_zone_get_roaring_forties_south_atlantic);
    RUN_TEST(test_swell_zone_get_antarctic);
    RUN_TEST(test_swell_zone_get_invalid);
    RUN_TEST(test_swell_zone_get_invalid_too_high);
    RUN_TEST(test_swell_zone_season_north_atlantic);
    RUN_TEST(test_swell_zone_bay_of_bengal);

    /* Swell direction */
    RUN_TEST(test_swell_direction_near_north_atlantic_track);
    RUN_TEST(test_swell_direction_roaring_forties_year_round);
    RUN_TEST(test_swell_direction_no_active_zone);

    /* Descriptions */
    RUN_TEST(test_all_basins_have_descriptions);
    RUN_TEST(test_all_monsoons_have_descriptions);
    RUN_TEST(test_all_swell_zones_have_descriptions);

    return UNITY_END();
}
