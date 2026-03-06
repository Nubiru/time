#include "../unity/unity.h"
#include "../../src/render/star_catalog.h"

#include <math.h>

#define TOL 0.02f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)
#define PI 3.14159265358979323846

void setUp(void) {}
void tearDown(void) {}

/* 1. Catalog has expected count (190-250 for expanded catalog) */
void test_catalog_count(void)
{
    int count = star_catalog_count();
    TEST_ASSERT_TRUE(count >= 190);
    TEST_ASSERT_TRUE(count <= 250);
}

/* 2. First entry is Sirius (brightest star) */
void test_first_is_sirius(void)
{
    star_entry_t s = star_catalog_entry(0);
    TEST_ASSERT_TRUE(s.magnitude < 0.0f); /* Sirius: -1.46 */
    TEST_ASSERT_TRUE(NEAR(6.75f, s.ra_hours));
}

/* 3. Sirius has a name */
void test_sirius_named(void)
{
    const char *name = star_catalog_name(0);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(name[0] == 'S'); /* "Sirius" */
}

/* 4. Invalid index returns sentinel values */
void test_invalid_index(void)
{
    star_entry_t s = star_catalog_entry(-1);
    TEST_ASSERT_TRUE(s.magnitude > 90.0f); /* sentinel */
    s = star_catalog_entry(9999);
    TEST_ASSERT_TRUE(s.magnitude > 90.0f);
}

/* 5. Invalid index name returns NULL */
void test_invalid_name(void)
{
    TEST_ASSERT_NULL(star_catalog_name(-1));
    TEST_ASSERT_NULL(star_catalog_name(9999));
}

/* 6. All entries have valid RA range */
void test_ra_range(void)
{
    int n = star_catalog_count();
    for (int i = 0; i < n; i++) {
        star_entry_t s = star_catalog_entry(i);
        TEST_ASSERT_TRUE(s.ra_hours >= 0.0f && s.ra_hours < 24.0f);
    }
}

/* 7. All entries have valid Dec range */
void test_dec_range(void)
{
    int n = star_catalog_count();
    for (int i = 0; i < n; i++) {
        star_entry_t s = star_catalog_entry(i);
        TEST_ASSERT_TRUE(s.dec_deg >= -90.0f && s.dec_deg <= 90.0f);
    }
}

/* 8. Original catalog (0-98) sorted by magnitude (brightest first) */
void test_sorted_by_magnitude(void)
{
    /* Original 99 stars sorted by magnitude */
    int limit = 99;
    int n = star_catalog_count();
    if (limit > n) limit = n;
    for (int i = 1; i < limit; i++) {
        star_entry_t prev = star_catalog_entry(i - 1);
        star_entry_t cur = star_catalog_entry(i);
        TEST_ASSERT_TRUE(cur.magnitude >= prev.magnitude - 0.01f);
    }
}

/* 9. Equatorial conversion: north pole -> (0, 0, 1) */
void test_equatorial_north_pole(void)
{
    star_xyz_t p = star_to_equatorial_xyz(0.0f, 90.0f);
    TEST_ASSERT_TRUE(NEAR(0.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.y));
    TEST_ASSERT_TRUE(NEAR(1.0f, p.z));
}

/* 10. Equatorial conversion: RA=0, Dec=0 -> (1, 0, 0) */
void test_equatorial_origin(void)
{
    star_xyz_t p = star_to_equatorial_xyz(0.0f, 0.0f);
    TEST_ASSERT_TRUE(NEAR(1.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.y));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.z));
}

/* 11. Equatorial conversion: RA=6h, Dec=0 -> (0, 1, 0) */
void test_equatorial_ra6(void)
{
    star_xyz_t p = star_to_equatorial_xyz(6.0f, 0.0f);
    TEST_ASSERT_TRUE(NEAR(0.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(1.0f, p.y));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.z));
}

/* 12. Equatorial: result on unit sphere */
void test_equatorial_unit_sphere(void)
{
    int n = star_catalog_count();
    for (int i = 0; i < n; i++) {
        star_entry_t s = star_catalog_entry(i);
        star_xyz_t p = star_to_equatorial_xyz(s.ra_hours, s.dec_deg);
        float len = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
        TEST_ASSERT_TRUE(NEAR(1.0f, len));
    }
}

/* 13. Ecliptic conversion: RA=0, Dec=0, obl=0 -> project (1, 0, 0) */
void test_ecliptic_no_obliquity(void)
{
    star_xyz_t p = star_to_ecliptic_xyz(0.0f, 0.0f, 0.0f);
    TEST_ASSERT_TRUE(NEAR(1.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.y));
    TEST_ASSERT_TRUE(NEAR(0.0f, p.z));
}

/* 14. Ecliptic: result on unit sphere */
void test_ecliptic_unit_sphere(void)
{
    int n = star_catalog_count();
    for (int i = 0; i < n; i++) {
        star_entry_t s = star_catalog_entry(i);
        star_xyz_t p = star_to_ecliptic_xyz(s.ra_hours, s.dec_deg, 23.44f);
        float len = sqrtf(p.x * p.x + p.y * p.y + p.z * p.z);
        TEST_ASSERT_TRUE(NEAR(1.0f, len));
    }
}

/* 15. Ecliptic: equatorial north pole with obliquity tilts into ecliptic */
void test_ecliptic_north_pole(void)
{
    /* Equatorial north pole (Dec=90) -> ecliptic: tilted by obliquity */
    star_xyz_t p = star_to_ecliptic_xyz(0.0f, 90.0f, 23.44f);
    /* Ecliptic north: project Y should be cos(23.44) ~ 0.917 */
    /* Ecliptic Z should be sin(23.44) ~ 0.398 (from equatorial pole tilt) */
    float obl_rad = 23.44f * (float)PI / 180.0f;
    TEST_ASSERT_TRUE(NEAR(0.0f, p.x));
    TEST_ASSERT_TRUE(NEAR(cosf(obl_rad), p.y));
    /* p.z comes from the obliquity rotation */
    float expected_z = sinf(obl_rad);
    TEST_ASSERT_TRUE(NEAR(expected_z, p.z));
}

/* 16. B-V to RGB: blue star (BV < 0) has blue > red */
void test_bv_blue_star(void)
{
    float r, g, b;
    star_bv_to_rgb(-0.20f, &r, &g, &b);
    TEST_ASSERT_TRUE(b > r);
}

/* 17. B-V to RGB: red star (BV > 1.5) has red > blue */
void test_bv_red_star(void)
{
    float r, g, b;
    star_bv_to_rgb(1.8f, &r, &g, &b);
    TEST_ASSERT_TRUE(r > b);
}

/* 18. B-V to RGB: white star (BV ~ 0) nearly equal channels */
void test_bv_white_star(void)
{
    float r, g, b;
    star_bv_to_rgb(0.0f, &r, &g, &b);
    TEST_ASSERT_TRUE(fabsf(r - b) < 0.2f);
    TEST_ASSERT_TRUE(r > 0.5f);
}

/* 19. B-V to RGB: all components in [0, 1] */
void test_bv_rgb_range(void)
{
    for (float bv = -0.4f; bv <= 2.0f; bv += 0.1f) {
        float r, g, b;
        star_bv_to_rgb(bv, &r, &g, &b);
        TEST_ASSERT_TRUE(r >= 0.0f && r <= 1.0f);
        TEST_ASSERT_TRUE(g >= 0.0f && g <= 1.0f);
        TEST_ASSERT_TRUE(b >= 0.0f && b <= 1.0f);
    }
}

/* 20. Magnitude to size: brighter -> larger */
void test_mag_brighter_larger(void)
{
    float s_bright = star_mag_to_size(-1.0f, 1.0f);
    float s_dim = star_mag_to_size(3.0f, 1.0f);
    TEST_ASSERT_TRUE(s_bright > s_dim);
}

/* 21. Magnitude to size: always positive */
void test_mag_size_positive(void)
{
    for (float m = -2.0f; m <= 6.0f; m += 0.5f) {
        float s = star_mag_to_size(m, 1.0f);
        TEST_ASSERT_TRUE(s > 0.0f);
    }
}

/* 22. Magnitude to size: scales with base_size */
void test_mag_size_scales(void)
{
    float s1 = star_mag_to_size(1.0f, 1.0f);
    float s2 = star_mag_to_size(1.0f, 3.0f);
    TEST_ASSERT_TRUE(NEAR(s1 * 3.0f, s2));
}

/* 23. Purity — same input produces same output */
void test_purity(void)
{
    star_entry_t a = star_catalog_entry(5);
    star_entry_t b = star_catalog_entry(5);
    TEST_ASSERT_TRUE(a.ra_hours == b.ra_hours);
    TEST_ASSERT_TRUE(a.dec_deg == b.dec_deg);
    TEST_ASSERT_TRUE(a.magnitude == b.magnitude);

    star_xyz_t pa = star_to_ecliptic_xyz(6.0f, 30.0f, 23.44f);
    star_xyz_t pb = star_to_ecliptic_xyz(6.0f, 30.0f, 23.44f);
    TEST_ASSERT_TRUE(pa.x == pb.x);
    TEST_ASSERT_TRUE(pa.y == pb.y);
    TEST_ASSERT_TRUE(pa.z == pb.z);
}

/* 24. Last entries have valid data */
void test_last_entries_valid(void)
{
    int n = star_catalog_count();
    for (int i = n - 3; i < n; i++) {
        star_entry_t s = star_catalog_entry(i);
        TEST_ASSERT_TRUE(s.ra_hours >= 0.0f && s.ra_hours < 24.0f);
        TEST_ASSERT_TRUE(s.dec_deg >= -90.0f && s.dec_deg <= 90.0f);
        TEST_ASSERT_TRUE(s.magnitude < 10.0f);
        const char *name = star_catalog_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '\0');
    }
}

/* 25. No two stars share exact RA+Dec (no duplicate positions) */
void test_no_duplicate_positions(void)
{
    int n = star_catalog_count();
    for (int i = 0; i < n; i++) {
        star_entry_t si = star_catalog_entry(i);
        for (int j = i + 1; j < n; j++) {
            star_entry_t sj = star_catalog_entry(j);
            int same_ra = (fabsf(si.ra_hours - sj.ra_hours) < 0.001f);
            int same_dec = (fabsf(si.dec_deg - sj.dec_deg) < 0.001f);
            /* Fail if both RA and Dec match exactly */
            TEST_ASSERT_FALSE(same_ra && same_dec);
        }
    }
}

/* 26. Index 1 is still Canopus */
void test_canopus_is_second(void)
{
    star_entry_t s = star_catalog_entry(1);
    TEST_ASSERT_TRUE(NEAR(6.40f, s.ra_hours));
    TEST_ASSERT_TRUE(NEAR(-52.70f, s.dec_deg));
    TEST_ASSERT_TRUE(s.magnitude < 0.0f); /* Canopus: -0.74 */
    const char *name = star_catalog_name(1);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(name[0] == 'C'); /* "Canopus" */
}

/* 27. All names are non-null */
void test_all_names_valid(void)
{
    int n = star_catalog_count();
    for (int i = 0; i < n; i++) {
        const char *name = star_catalog_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(name[0] != '\0');
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_catalog_count);
    RUN_TEST(test_first_is_sirius);
    RUN_TEST(test_sirius_named);
    RUN_TEST(test_invalid_index);
    RUN_TEST(test_invalid_name);
    RUN_TEST(test_ra_range);
    RUN_TEST(test_dec_range);
    RUN_TEST(test_sorted_by_magnitude);
    RUN_TEST(test_equatorial_north_pole);
    RUN_TEST(test_equatorial_origin);
    RUN_TEST(test_equatorial_ra6);
    RUN_TEST(test_equatorial_unit_sphere);
    RUN_TEST(test_ecliptic_no_obliquity);
    RUN_TEST(test_ecliptic_unit_sphere);
    RUN_TEST(test_ecliptic_north_pole);
    RUN_TEST(test_bv_blue_star);
    RUN_TEST(test_bv_red_star);
    RUN_TEST(test_bv_white_star);
    RUN_TEST(test_bv_rgb_range);
    RUN_TEST(test_mag_brighter_larger);
    RUN_TEST(test_mag_size_positive);
    RUN_TEST(test_mag_size_scales);
    RUN_TEST(test_purity);
    RUN_TEST(test_last_entries_valid);
    RUN_TEST(test_no_duplicate_positions);
    RUN_TEST(test_canopus_is_second);
    RUN_TEST(test_all_names_valid);
    return UNITY_END();
}
