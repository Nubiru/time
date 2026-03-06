#include "../unity/unity.h"
#include "../../src/ui/location_presets.h"

#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Count and access: 1-3 */

void test_count_positive(void)
{
    TEST_ASSERT_TRUE(location_preset_count() > 0);
    TEST_ASSERT_TRUE(location_preset_count() >= 20);
}

void test_all_presets_have_names(void)
{
    int n = location_preset_count();
    for (int i = 0; i < n; i++) {
        location_preset_t loc = location_preset_get(i);
        TEST_ASSERT_NOT_NULL(loc.name);
        TEST_ASSERT_NOT_NULL(loc.country);
        TEST_ASSERT_TRUE(strlen(loc.name) > 0);
    }
}

void test_invalid_index(void)
{
    location_preset_t loc = location_preset_get(-1);
    TEST_ASSERT_NULL(loc.name);
    loc = location_preset_get(9999);
    TEST_ASSERT_NULL(loc.name);
}

/* Default: 4-5 */

void test_default_is_greenwich(void)
{
    int idx = location_preset_default();
    location_preset_t loc = location_preset_get(idx);
    TEST_ASSERT_EQUAL_STRING("Greenwich", loc.name);
}

void test_greenwich_coordinates(void)
{
    location_preset_t loc = location_preset_get(location_preset_default());
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 51.48f, (float)loc.latitude);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 0.0f, (float)loc.longitude);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)loc.utc_offset);
}

/* Known coordinates: 6-9 */

void test_new_york_coords(void)
{
    int idx = location_preset_find("New York");
    TEST_ASSERT_TRUE(idx >= 0);
    location_preset_t loc = location_preset_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 40.71f, (float)loc.latitude);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, -74.01f, (float)loc.longitude);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, -5.0f, (float)loc.utc_offset);
}

void test_tokyo_coords(void)
{
    int idx = location_preset_find("Tokyo");
    TEST_ASSERT_TRUE(idx >= 0);
    location_preset_t loc = location_preset_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 35.68f, (float)loc.latitude);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 139.65f, (float)loc.longitude);
}

void test_cairo_coords(void)
{
    int idx = location_preset_find("Cairo");
    TEST_ASSERT_TRUE(idx >= 0);
    location_preset_t loc = location_preset_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 30.04f, (float)loc.latitude);
}

void test_mexico_city_coords(void)
{
    int idx = location_preset_find("Mexico");
    TEST_ASSERT_TRUE(idx >= 0);
    location_preset_t loc = location_preset_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 19.43f, (float)loc.latitude);
}

/* Find by name: 10-13 */

void test_find_exact(void)
{
    TEST_ASSERT_TRUE(location_preset_find("Greenwich") >= 0);
    TEST_ASSERT_TRUE(location_preset_find("Tokyo") >= 0);
    TEST_ASSERT_TRUE(location_preset_find("Sydney") >= 0);
}

void test_find_partial(void)
{
    TEST_ASSERT_TRUE(location_preset_find("york") >= 0);
    TEST_ASSERT_TRUE(location_preset_find("ang") >= 0); /* Los Angeles or Bangkok */
}

void test_find_case_insensitive(void)
{
    TEST_ASSERT_TRUE(location_preset_find("TOKYO") >= 0);
    TEST_ASSERT_TRUE(location_preset_find("greenwich") >= 0);
    TEST_ASSERT_TRUE(location_preset_find("nEW yORK") >= 0);
}

void test_find_not_found(void)
{
    TEST_ASSERT_EQUAL_INT(-1, location_preset_find("Atlantis"));
    TEST_ASSERT_EQUAL_INT(-1, location_preset_find(NULL));
    TEST_ASSERT_EQUAL_INT(-1, location_preset_find(""));
}

/* Find by country: 14 */

void test_find_by_country(void)
{
    int idx = location_preset_find("Japan");
    TEST_ASSERT_TRUE(idx >= 0);
    location_preset_t loc = location_preset_get(idx);
    TEST_ASSERT_EQUAL_STRING("Tokyo", loc.name);
}

/* Coordinate ranges: 15 */

void test_coordinate_ranges(void)
{
    int n = location_preset_count();
    for (int i = 0; i < n; i++) {
        location_preset_t loc = location_preset_get(i);
        TEST_ASSERT_TRUE(loc.latitude >= -90.0 && loc.latitude <= 90.0);
        TEST_ASSERT_TRUE(loc.longitude >= -180.0 && loc.longitude <= 180.0);
        TEST_ASSERT_TRUE(loc.utc_offset >= -12.0 && loc.utc_offset <= 14.0);
    }
}

/* Formatting: 16-19 */

void test_format_coords_north_west(void)
{
    char buf[64];
    location_format_coords(51.48, -0.0005, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "N") != NULL);
    TEST_ASSERT_TRUE(strstr(buf, "W") != NULL);
    TEST_ASSERT_TRUE(strstr(buf, "51.48") != NULL);
}

void test_format_coords_south_east(void)
{
    char buf[64];
    location_format_coords(-33.87, 151.21, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "S") != NULL);
    TEST_ASSERT_TRUE(strstr(buf, "E") != NULL);
}

void test_format_full(void)
{
    location_preset_t loc = location_preset_get(0); /* Greenwich */
    char buf[128];
    location_format_full(loc, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strstr(buf, "Greenwich") != NULL);
    TEST_ASSERT_TRUE(strstr(buf, "UK") != NULL);
}

void test_format_null_safe(void)
{
    location_format_coords(0.0, 0.0, NULL, 0);
    location_preset_t loc = {NULL, NULL, 0.0, 0.0, 0.0};
    location_format_full(loc, NULL, 0);
    /* Should not crash */
}

/* Southern hemisphere cities: 20 */

void test_southern_hemisphere(void)
{
    int idx = location_preset_find("Sydney");
    TEST_ASSERT_TRUE(idx >= 0);
    location_preset_t loc = location_preset_get(idx);
    TEST_ASSERT_TRUE(loc.latitude < 0.0);

    idx = location_preset_find("Cape Town");
    TEST_ASSERT_TRUE(idx >= 0);
    loc = location_preset_get(idx);
    TEST_ASSERT_TRUE(loc.latitude < 0.0);
}

/* Purity: 21 */

void test_purity(void)
{
    location_preset_t a = location_preset_get(0);
    location_preset_t b = location_preset_get(0);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_FLOAT((float)a.latitude, (float)b.latitude);
    TEST_ASSERT_EQUAL_FLOAT((float)a.longitude, (float)b.longitude);

    int fa = location_preset_find("Tokyo");
    int fb = location_preset_find("Tokyo");
    TEST_ASSERT_EQUAL_INT(fa, fb);
}

/* High-latitude city: 22 */

void test_high_latitude_reykjavik(void)
{
    int idx = location_preset_find("Reykjavik");
    TEST_ASSERT_TRUE(idx >= 0);
    location_preset_t loc = location_preset_get(idx);
    TEST_ASSERT_TRUE(loc.latitude > 60.0); /* Arctic-adjacent */
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_count_positive);
    RUN_TEST(test_all_presets_have_names);
    RUN_TEST(test_invalid_index);
    RUN_TEST(test_default_is_greenwich);
    RUN_TEST(test_greenwich_coordinates);
    RUN_TEST(test_new_york_coords);
    RUN_TEST(test_tokyo_coords);
    RUN_TEST(test_cairo_coords);
    RUN_TEST(test_mexico_city_coords);
    RUN_TEST(test_find_exact);
    RUN_TEST(test_find_partial);
    RUN_TEST(test_find_case_insensitive);
    RUN_TEST(test_find_not_found);
    RUN_TEST(test_find_by_country);
    RUN_TEST(test_coordinate_ranges);
    RUN_TEST(test_format_coords_north_west);
    RUN_TEST(test_format_coords_south_east);
    RUN_TEST(test_format_full);
    RUN_TEST(test_format_null_safe);
    RUN_TEST(test_southern_hemisphere);
    RUN_TEST(test_purity);
    RUN_TEST(test_high_latitude_reykjavik);
    return UNITY_END();
}
