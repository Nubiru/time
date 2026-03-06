#include "../unity/unity.h"
#include "../../src/render/deep_sky.h"

#include <string.h>

#define TOL 0.1
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* 1. Catalog has exactly 110 Messier objects */
void test_catalog_count_is_110(void)
{
    TEST_ASSERT_EQUAL_INT(110, dso_catalog_count());
}

/* 2. Index 0 is M1 Crab Nebula, type SNR */
void test_m1_crab_nebula(void)
{
    dso_entry_t e = dso_catalog_get(0);
    TEST_ASSERT_NOT_NULL(e.designation);
    TEST_ASSERT_EQUAL_STRING("M1", e.designation);
    TEST_ASSERT_NOT_NULL(e.name);
    TEST_ASSERT_EQUAL_STRING("Crab Nebula", e.name);
    TEST_ASSERT_EQUAL_INT(DSO_SUPERNOVA_REMNANT, e.type);
}

/* 3. Index 109 is M110, valid entry */
void test_m110_valid(void)
{
    dso_entry_t e = dso_catalog_get(109);
    TEST_ASSERT_NOT_NULL(e.designation);
    TEST_ASSERT_EQUAL_STRING("M110", e.designation);
    TEST_ASSERT_EQUAL_INT(DSO_ELLIPTICAL_GALAXY, e.type);
}

/* 4. Index -1 returns null designation */
void test_invalid_negative_index(void)
{
    dso_entry_t e = dso_catalog_get(-1);
    TEST_ASSERT_NULL(e.designation);
}

/* 5. Index 110 returns null designation */
void test_invalid_past_end_index(void)
{
    dso_entry_t e = dso_catalog_get(110);
    TEST_ASSERT_NULL(e.designation);
}

/* 6. M31 Andromeda — check RA, Dec, type, magnitude */
void test_m31_andromeda(void)
{
    int idx = dso_find_messier(31);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_STRING("M31", e.designation);
    TEST_ASSERT_NOT_NULL(e.name);
    TEST_ASSERT_TRUE(NEAR(0.71, e.ra_hours));
    TEST_ASSERT_TRUE(NEAR(41.3, e.dec_deg));
    TEST_ASSERT_EQUAL_INT(DSO_SPIRAL_GALAXY, e.type);
    TEST_ASSERT_TRUE(NEAR(3.4, e.apparent_mag));
}

/* 7. M42 Orion Nebula — type emission, size ~85' */
void test_m42_orion_nebula(void)
{
    int idx = dso_find_messier(42);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_INT(DSO_EMISSION_NEBULA, e.type);
    TEST_ASSERT_TRUE(NEAR(85.0, e.angular_size_arcmin));
}

/* 8. M45 Pleiades — type open cluster, mag ~1.6 */
void test_m45_pleiades(void)
{
    int idx = dso_find_messier(45);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_INT(DSO_OPEN_CLUSTER, e.type);
    TEST_ASSERT_TRUE(NEAR(1.6, e.apparent_mag));
}

/* 9. M13 Great Globular — type globular */
void test_m13_great_globular(void)
{
    int idx = dso_find_messier(13);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_INT(DSO_GLOBULAR_CLUSTER, e.type);
}

/* 10. M57 Ring Nebula — type planetary nebula */
void test_m57_ring_nebula(void)
{
    int idx = dso_find_messier(57);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_INT(DSO_PLANETARY_NEBULA, e.type);
}

/* 11. M78 — type reflection nebula */
void test_m78_reflection(void)
{
    int idx = dso_find_messier(78);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_INT(DSO_REFLECTION_NEBULA, e.type);
}

/* 12. dso_type_name for all 10 types */
void test_type_names_valid(void)
{
    TEST_ASSERT_EQUAL_STRING("Open Cluster", dso_type_name(DSO_OPEN_CLUSTER));
    TEST_ASSERT_EQUAL_STRING("Globular Cluster", dso_type_name(DSO_GLOBULAR_CLUSTER));
    TEST_ASSERT_EQUAL_STRING("Emission Nebula", dso_type_name(DSO_EMISSION_NEBULA));
    TEST_ASSERT_EQUAL_STRING("Reflection Nebula", dso_type_name(DSO_REFLECTION_NEBULA));
    TEST_ASSERT_EQUAL_STRING("Planetary Nebula", dso_type_name(DSO_PLANETARY_NEBULA));
    TEST_ASSERT_EQUAL_STRING("Dark Nebula", dso_type_name(DSO_DARK_NEBULA));
    TEST_ASSERT_EQUAL_STRING("Supernova Remnant", dso_type_name(DSO_SUPERNOVA_REMNANT));
    TEST_ASSERT_EQUAL_STRING("Spiral Galaxy", dso_type_name(DSO_SPIRAL_GALAXY));
    TEST_ASSERT_EQUAL_STRING("Elliptical Galaxy", dso_type_name(DSO_ELLIPTICAL_GALAXY));
    TEST_ASSERT_EQUAL_STRING("Irregular Galaxy", dso_type_name(DSO_IRREGULAR_GALAXY));
}

/* 13. dso_type_name for invalid type returns "Unknown" */
void test_type_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", dso_type_name((dso_type_t)99));
    TEST_ASSERT_EQUAL_STRING("Unknown", dso_type_name((dso_type_t)-1));
}

/* 14. dso_type_color returns valid colors (r,g,b all in 0-1 range) */
void test_type_color_valid_range(void)
{
    for (int t = 0; t < DSO_TYPE_COUNT; t++) {
        dso_color_t c = dso_type_color((dso_type_t)t);
        TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
        TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
        TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
    }
}

/* 15. More than 20 spiral galaxies */
void test_spiral_galaxy_count(void)
{
    int count = dso_count_by_type(DSO_SPIRAL_GALAXY);
    TEST_ASSERT_TRUE(count > 20);
}

/* 16. More than 25 globular clusters */
void test_globular_cluster_count(void)
{
    int count = dso_count_by_type(DSO_GLOBULAR_CLUSTER);
    TEST_ASSERT_TRUE(count > 25);
}

/* 17. Exactly 1 supernova remnant (M1) */
void test_supernova_remnant_count(void)
{
    int count = dso_count_by_type(DSO_SUPERNOVA_REMNANT);
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* 18. dso_find_messier(1) returns valid index (M1) */
void test_find_messier_1(void)
{
    int idx = dso_find_messier(1);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_STRING("M1", e.designation);
}

/* 19. dso_find_messier(31) returns valid index (M31) */
void test_find_messier_31(void)
{
    int idx = dso_find_messier(31);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_STRING("M31", e.designation);
}

/* 20. dso_find_messier(0) returns -1 */
void test_find_messier_zero(void)
{
    TEST_ASSERT_EQUAL_INT(-1, dso_find_messier(0));
}

/* 21. dso_find_messier(111) returns -1 */
void test_find_messier_111(void)
{
    TEST_ASSERT_EQUAL_INT(-1, dso_find_messier(111));
}

/* 22. dso_find_by_name("Orion") returns M42's index */
void test_find_by_name_orion(void)
{
    int idx = dso_find_by_name("Orion");
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_STRING("M42", e.designation);
}

/* 23. dso_find_by_name("Andromeda") returns M31's index */
void test_find_by_name_andromeda(void)
{
    int idx = dso_find_by_name("Andromeda");
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_STRING("M31", e.designation);
}

/* 24. dso_find_by_name("nonexistent") returns -1 */
void test_find_by_name_nonexistent(void)
{
    TEST_ASSERT_EQUAL_INT(-1, dso_find_by_name("nonexistent"));
}

/* 25. All RA values in 0-24 range */
void test_all_ra_in_range(void)
{
    int n = dso_catalog_count();
    for (int i = 0; i < n; i++) {
        dso_entry_t e = dso_catalog_get(i);
        TEST_ASSERT_TRUE(e.ra_hours >= 0.0 && e.ra_hours < 24.0);
    }
}

/* 26. All Dec values in -90 to +90 range */
void test_all_dec_in_range(void)
{
    int n = dso_catalog_count();
    for (int i = 0; i < n; i++) {
        dso_entry_t e = dso_catalog_get(i);
        TEST_ASSERT_TRUE(e.dec_deg >= -90.0 && e.dec_deg <= 90.0);
    }
}

/* 27. Purity test: same inputs always same outputs */
void test_purity(void)
{
    dso_entry_t a = dso_catalog_get(30);
    dso_entry_t b = dso_catalog_get(30);
    TEST_ASSERT_EQUAL_STRING(a.designation, b.designation);
    TEST_ASSERT_TRUE(a.ra_hours == b.ra_hours);
    TEST_ASSERT_TRUE(a.dec_deg == b.dec_deg);
    TEST_ASSERT_EQUAL_INT(a.type, b.type);
    TEST_ASSERT_TRUE(a.apparent_mag == b.apparent_mag);
}

/* 28. Case-insensitive find by name */
void test_find_by_name_case_insensitive(void)
{
    int idx1 = dso_find_by_name("orion");
    int idx2 = dso_find_by_name("ORION");
    int idx3 = dso_find_by_name("Orion");
    TEST_ASSERT_TRUE(idx1 >= 0);
    TEST_ASSERT_EQUAL_INT(idx1, idx2);
    TEST_ASSERT_EQUAL_INT(idx1, idx3);
}

/* 29. dso_find_by_name with NULL returns -1 */
void test_find_by_name_null(void)
{
    TEST_ASSERT_EQUAL_INT(-1, dso_find_by_name((void *)0));
}

/* 30. All entries have valid type enum values */
void test_all_types_valid(void)
{
    int n = dso_catalog_count();
    for (int i = 0; i < n; i++) {
        dso_entry_t e = dso_catalog_get(i);
        TEST_ASSERT_TRUE(e.type >= 0 && e.type < DSO_TYPE_COUNT);
    }
}

/* 31. All entries have positive angular size */
void test_all_angular_size_positive(void)
{
    int n = dso_catalog_count();
    for (int i = 0; i < n; i++) {
        dso_entry_t e = dso_catalog_get(i);
        TEST_ASSERT_TRUE(e.angular_size_arcmin > 0.0);
    }
}

/* 32. All entries have color components in valid range */
void test_all_colors_valid(void)
{
    int n = dso_catalog_count();
    for (int i = 0; i < n; i++) {
        dso_entry_t e = dso_catalog_get(i);
        TEST_ASSERT_TRUE(e.color.r >= 0.0f && e.color.r <= 1.0f);
        TEST_ASSERT_TRUE(e.color.g >= 0.0f && e.color.g <= 1.0f);
        TEST_ASSERT_TRUE(e.color.b >= 0.0f && e.color.b <= 1.0f);
    }
}

/* 33. M82 Cigar Galaxy is irregular */
void test_m82_irregular(void)
{
    int idx = dso_find_messier(82);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_INT(DSO_IRREGULAR_GALAXY, e.type);
}

/* 34. M87 Virgo A is elliptical */
void test_m87_elliptical(void)
{
    int idx = dso_find_messier(87);
    TEST_ASSERT_TRUE(idx >= 0);
    dso_entry_t e = dso_catalog_get(idx);
    TEST_ASSERT_EQUAL_INT(DSO_ELLIPTICAL_GALAXY, e.type);
}

/* 35. Open cluster count (should be 27) */
void test_open_cluster_count(void)
{
    int count = dso_count_by_type(DSO_OPEN_CLUSTER);
    TEST_ASSERT_TRUE(count >= 25);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_catalog_count_is_110);
    RUN_TEST(test_m1_crab_nebula);
    RUN_TEST(test_m110_valid);
    RUN_TEST(test_invalid_negative_index);
    RUN_TEST(test_invalid_past_end_index);
    RUN_TEST(test_m31_andromeda);
    RUN_TEST(test_m42_orion_nebula);
    RUN_TEST(test_m45_pleiades);
    RUN_TEST(test_m13_great_globular);
    RUN_TEST(test_m57_ring_nebula);
    RUN_TEST(test_m78_reflection);
    RUN_TEST(test_type_names_valid);
    RUN_TEST(test_type_name_invalid);
    RUN_TEST(test_type_color_valid_range);
    RUN_TEST(test_spiral_galaxy_count);
    RUN_TEST(test_globular_cluster_count);
    RUN_TEST(test_supernova_remnant_count);
    RUN_TEST(test_find_messier_1);
    RUN_TEST(test_find_messier_31);
    RUN_TEST(test_find_messier_zero);
    RUN_TEST(test_find_messier_111);
    RUN_TEST(test_find_by_name_orion);
    RUN_TEST(test_find_by_name_andromeda);
    RUN_TEST(test_find_by_name_nonexistent);
    RUN_TEST(test_all_ra_in_range);
    RUN_TEST(test_all_dec_in_range);
    RUN_TEST(test_purity);
    RUN_TEST(test_find_by_name_case_insensitive);
    RUN_TEST(test_find_by_name_null);
    RUN_TEST(test_all_types_valid);
    RUN_TEST(test_all_angular_size_positive);
    RUN_TEST(test_all_colors_valid);
    RUN_TEST(test_m82_irregular);
    RUN_TEST(test_m87_elliptical);
    RUN_TEST(test_open_cluster_count);
    return UNITY_END();
}
