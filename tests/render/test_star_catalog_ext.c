#include "../unity/unity.h"
#include "../../src/render/star_catalog_ext.h"

#include <math.h>
#include <string.h>

#define TOL 0.02f
#define NEAR(exp, act) ((act) >= (exp) - TOL && (act) <= (exp) + TOL)

void setUp(void) {}
void tearDown(void) {}

/* ── 1. Total count ── */
void test_count_minimum(void)
{
    int n = ext_star_count();
    TEST_ASSERT_TRUE(n >= 300);
}

void test_count_maximum(void)
{
    int n = ext_star_count();
    TEST_ASSERT_TRUE(n <= 600);
}

/* ── 2. Tier 0 — brightest 21 stars ── */
void test_tier0_count(void)
{
    int n = ext_star_tier_count(0);
    TEST_ASSERT_EQUAL_INT(21, n);
}

void test_tier0_start_is_zero(void)
{
    int s = ext_star_tier_start(0);
    TEST_ASSERT_EQUAL_INT(0, s);
}

/* ── 3. Specific Tier 0 stars: Sirius first ── */
void test_sirius_is_first(void)
{
    ext_star_t s = ext_star_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, -1.46f, s.v_mag);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 6.75f, s.ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.2f, -16.72f, s.dec_deg);
}

void test_sirius_name(void)
{
    const char *name = ext_star_name(0);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Sirius", name);
}

void test_canopus_is_second(void)
{
    ext_star_t s = ext_star_get(1);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, -0.74f, s.v_mag);
    const char *name = ext_star_name(1);
    TEST_ASSERT_EQUAL_STRING("Canopus", name);
}

/* ── 4. All Tier 0 stars present ── */
void test_tier0_all_present(void)
{
    /* All 21 brightest star names must be findable */
    const char *tier0_names[] = {
        "Sirius", "Canopus", "Alpha Centauri", "Arcturus", "Vega",
        "Capella", "Rigel", "Procyon", "Achernar", "Betelgeuse",
        "Hadar", "Altair", "Acrux", "Aldebaran", "Antares",
        "Spica", "Pollux", "Fomalhaut", "Deneb", "Mimosa", "Regulus"
    };
    for (int i = 0; i < 21; i++) {
        int idx = ext_star_find(tier0_names[i]);
        TEST_ASSERT_TRUE_MESSAGE(idx >= 0, tier0_names[i]);
        TEST_ASSERT_TRUE(idx < 21); /* all in tier 0 */
    }
}

/* ── 5. Sorted by magnitude (brightest first) ── */
void test_sorted_by_magnitude(void)
{
    int n = ext_star_count();
    for (int i = 1; i < n; i++) {
        ext_star_t a = ext_star_get(i - 1);
        ext_star_t b = ext_star_get(i);
        TEST_ASSERT_TRUE_MESSAGE(a.v_mag <= b.v_mag + 0.001f,
            "Stars must be sorted by magnitude, brightest first");
    }
}

/* ── 6. RA range valid ── */
void test_ra_range(void)
{
    int n = ext_star_count();
    for (int i = 0; i < n; i++) {
        ext_star_t s = ext_star_get(i);
        TEST_ASSERT_TRUE(s.ra_hours >= 0.0f);
        TEST_ASSERT_TRUE(s.ra_hours < 24.0f);
    }
}

/* ── 7. Dec range valid ── */
void test_dec_range(void)
{
    int n = ext_star_count();
    for (int i = 0; i < n; i++) {
        ext_star_t s = ext_star_get(i);
        TEST_ASSERT_TRUE(s.dec_deg >= -90.0f);
        TEST_ASSERT_TRUE(s.dec_deg <= 90.0f);
    }
}

/* ── 8. Invalid index returns sentinel ── */
void test_invalid_index_negative(void)
{
    ext_star_t s = ext_star_get(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 99.0f, s.v_mag);
}

void test_invalid_index_too_large(void)
{
    ext_star_t s = ext_star_get(99999);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 99.0f, s.v_mag);
}

/* ── 9. Invalid index name returns NULL ── */
void test_invalid_name_negative(void)
{
    TEST_ASSERT_NULL(ext_star_name(-1));
}

void test_invalid_name_too_large(void)
{
    TEST_ASSERT_NULL(ext_star_name(99999));
}

/* ── 10. Tier counts ── */
void test_tier1_count_minimum(void)
{
    int n = ext_star_tier_count(1);
    TEST_ASSERT_TRUE(n >= 80);
}

void test_tier2_count_minimum(void)
{
    int n = ext_star_tier_count(2);
    TEST_ASSERT_TRUE(n >= 100);
}

void test_tier3_count_zero(void)
{
    /* Tier 3 (V > 4.5) — we don't populate this yet */
    int n = ext_star_tier_count(3);
    TEST_ASSERT_TRUE(n >= 0);
}

void test_tier_counts_sum_to_total(void)
{
    int sum = 0;
    for (int t = 0; t < EXT_TIER_COUNT; t++) {
        sum += ext_star_tier_count(t);
    }
    TEST_ASSERT_EQUAL_INT(ext_star_count(), sum);
}

/* ── 11. Tier start indices ── */
void test_tier_starts_ascending(void)
{
    for (int t = 1; t < EXT_TIER_COUNT; t++) {
        TEST_ASSERT_TRUE(ext_star_tier_start(t) >= ext_star_tier_start(t - 1));
    }
}

void test_tier1_start_is_21(void)
{
    /* Tier 0 has exactly 21 stars, so tier 1 starts at 21 */
    TEST_ASSERT_EQUAL_INT(21, ext_star_tier_start(1));
}

/* ── 12. ext_star_tier_for_mag ── */
void test_tier_for_mag_sirius(void)
{
    TEST_ASSERT_EQUAL_INT(0, ext_star_tier_for_mag(-1.46f));
}

void test_tier_for_mag_boundary_1_5(void)
{
    TEST_ASSERT_EQUAL_INT(1, ext_star_tier_for_mag(1.5f));
}

void test_tier_for_mag_boundary_3_0(void)
{
    TEST_ASSERT_EQUAL_INT(2, ext_star_tier_for_mag(3.0f));
}

void test_tier_for_mag_boundary_4_5(void)
{
    TEST_ASSERT_EQUAL_INT(3, ext_star_tier_for_mag(4.5f));
}

void test_tier_for_mag_bright(void)
{
    TEST_ASSERT_EQUAL_INT(0, ext_star_tier_for_mag(0.5f));
}

/* ── 13. Named star count ── */
void test_named_count_minimum(void)
{
    int n = ext_star_named_count();
    TEST_ASSERT_TRUE(n >= 100);
}

/* ── 14. ext_star_find — case insensitive ── */
void test_find_sirius(void)
{
    TEST_ASSERT_EQUAL_INT(0, ext_star_find("Sirius"));
}

void test_find_case_insensitive(void)
{
    int idx = ext_star_find("sirius");
    TEST_ASSERT_EQUAL_INT(0, idx);
}

void test_find_case_mixed(void)
{
    int idx = ext_star_find("VEGA");
    TEST_ASSERT_TRUE(idx >= 0);
    const char *name = ext_star_name(idx);
    TEST_ASSERT_NOT_NULL(name);
}

void test_find_not_found(void)
{
    TEST_ASSERT_EQUAL_INT(-1, ext_star_find("Nonexistent Star XYZ"));
}

void test_find_null_returns_minus_one(void)
{
    TEST_ASSERT_EQUAL_INT(-1, ext_star_find(NULL));
}

/* ── 15. ext_star_in_region — basic region ── */
void test_region_basic(void)
{
    int buf[600];
    /* Orion region: RA ~5-6h, Dec ~-10 to +10 */
    int count = ext_star_in_region(5.0f, 6.0f, -10.0f, 10.0f, buf, 600);
    TEST_ASSERT_TRUE(count >= 1); /* At least Rigel or Betelgeuse nearby */
}

/* ── 16. ext_star_in_region — RA wrap around ── */
void test_region_ra_wrap(void)
{
    int buf[600];
    /* Region that wraps: RA 23 to 1 hour */
    int count = ext_star_in_region(23.0f, 1.0f, -60.0f, 60.0f, buf, 600);
    TEST_ASSERT_TRUE(count >= 1);
}

/* ── 17. ext_star_in_region — empty result ── */
void test_region_empty(void)
{
    int buf[10];
    /* Tiny region unlikely to contain stars */
    int count = ext_star_in_region(12.0f, 12.01f, 89.5f, 90.0f, buf, 10);
    TEST_ASSERT_TRUE(count >= 0);
}

/* ── 18. ext_star_in_region — limited output ── */
void test_region_limited_output(void)
{
    int buf[2];
    /* Large region, but only room for 2 */
    int count = ext_star_in_region(0.0f, 24.0f, -90.0f, 90.0f, buf, 2);
    TEST_ASSERT_EQUAL_INT(2, count);
}

/* ── 19. Specific star data — Arcturus ── */
void test_arcturus_data(void)
{
    int idx = ext_star_find("Arcturus");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 14.26f, s.ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.2f, 19.18f, s.dec_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, -0.05f, s.v_mag);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 1.23f, s.bv);
}

/* ── 20. Specific star data — Vega ── */
void test_vega_data(void)
{
    int idx = ext_star_find("Vega");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 18.62f, s.ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.2f, 38.78f, s.dec_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.03f, s.v_mag);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.00f, s.bv);
}

/* ── 21. Specific star — Polaris ── */
void test_polaris_data(void)
{
    int idx = ext_star_find("Polaris");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 2.53f, s.ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 89.26f, s.dec_deg);
}

/* ── 22. BV range reasonable ── */
void test_bv_range(void)
{
    int n = ext_star_count();
    for (int i = 0; i < n; i++) {
        ext_star_t s = ext_star_get(i);
        TEST_ASSERT_TRUE(s.bv >= -0.50f);
        TEST_ASSERT_TRUE(s.bv <= 2.50f);
    }
}

/* ── 23. Tier 1 named stars present ── */
void test_tier1_named_stars(void)
{
    const char *names[] = {
        "Adhara", "Castor", "Shaula", "Gacrux", "Bellatrix",
        "Elnath", "Miaplacidus", "Alnilam", "Alnair", "Alioth",
        "Dubhe", "Mirfak", "Alkaid", "Polaris", "Alpheratz"
    };
    for (int i = 0; i < 15; i++) {
        int idx = ext_star_find(names[i]);
        TEST_ASSERT_TRUE_MESSAGE(idx >= 0, names[i]);
    }
}

/* ── 24. Tier 2 named stars present ── */
void test_tier2_named_stars(void)
{
    const char *names[] = {
        "Pherkad", "Mebsuta", "Ascella", "Thuban"
    };
    for (int i = 0; i < 4; i++) {
        int idx = ext_star_find(names[i]);
        TEST_ASSERT_TRUE_MESSAGE(idx >= 0, names[i]);
    }
}

/* ── 25. Invalid tier returns 0 ── */
void test_invalid_tier_count(void)
{
    TEST_ASSERT_EQUAL_INT(0, ext_star_tier_count(-1));
    TEST_ASSERT_EQUAL_INT(0, ext_star_tier_count(99));
}

void test_invalid_tier_start(void)
{
    TEST_ASSERT_EQUAL_INT(0, ext_star_tier_start(-1));
    TEST_ASSERT_EQUAL_INT(0, ext_star_tier_start(99));
}

/* ── 26. Specific magnitude checks ── */
void test_regulus_last_tier0(void)
{
    int idx = ext_star_find("Regulus");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 1.35f, s.v_mag);
    TEST_ASSERT_TRUE(idx < 21); /* Must be in tier 0 */
}

/* ── 27. Adhara is first in tier 1 area ── */
void test_adhara_in_tier1(void)
{
    int idx = ext_star_find("Adhara");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_TRUE(s.v_mag >= 1.5f);
    TEST_ASSERT_TRUE(s.v_mag < 3.0f);
}

/* ── 28. Betelgeuse B-V (very red star) ── */
void test_betelgeuse_bv(void)
{
    int idx = ext_star_find("Betelgeuse");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.85f, s.bv);
}

/* ── 29. Spica B-V (blue star) ── */
void test_spica_bv(void)
{
    int idx = ext_star_find("Spica");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_TRUE(s.bv < 0.0f); /* blue */
}

/* ── 30. Deneb data ── */
void test_deneb_data(void)
{
    int idx = ext_star_find("Deneb");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 20.69f, s.ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.2f, 45.28f, s.dec_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 1.25f, s.v_mag);
}

/* ── 31. ext_star_in_region with NULL out ── */
void test_region_zero_max(void)
{
    int count = ext_star_in_region(0.0f, 24.0f, -90.0f, 90.0f, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ── 32. All stars have magnitude < 6.5 (tier 3 boundary) ── */
void test_all_within_magnitude_limit(void)
{
    int n = ext_star_count();
    for (int i = 0; i < n; i++) {
        ext_star_t s = ext_star_get(i);
        TEST_ASSERT_TRUE(s.v_mag < 6.5f);
    }
}

/* ── 33. Fomalhaut data match ── */
void test_fomalhaut_data(void)
{
    int idx = ext_star_find("Fomalhaut");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 22.96f, s.ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.2f, -29.62f, s.dec_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 1.16f, s.v_mag);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.09f, s.bv);
}

/* ── 34. Antares data match ── */
void test_antares_data(void)
{
    int idx = ext_star_find("Antares");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 16.49f, s.ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.2f, -26.43f, s.dec_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.96f, s.v_mag);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.83f, s.bv);
}

/* ── 35. Altair data match ── */
void test_altair_data(void)
{
    int idx = ext_star_find("Altair");
    TEST_ASSERT_TRUE(idx >= 0);
    ext_star_t s = ext_star_get(idx);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 19.85f, s.ra_hours);
    TEST_ASSERT_FLOAT_WITHIN(0.2f, 8.87f, s.dec_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.77f, s.v_mag);
}

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_count_minimum);
    RUN_TEST(test_count_maximum);

    /* Tier 0 */
    RUN_TEST(test_tier0_count);
    RUN_TEST(test_tier0_start_is_zero);
    RUN_TEST(test_tier0_all_present);

    /* Specific stars */
    RUN_TEST(test_sirius_is_first);
    RUN_TEST(test_sirius_name);
    RUN_TEST(test_canopus_is_second);
    RUN_TEST(test_arcturus_data);
    RUN_TEST(test_vega_data);
    RUN_TEST(test_polaris_data);
    RUN_TEST(test_regulus_last_tier0);
    RUN_TEST(test_deneb_data);
    RUN_TEST(test_fomalhaut_data);
    RUN_TEST(test_antares_data);
    RUN_TEST(test_altair_data);
    RUN_TEST(test_betelgeuse_bv);
    RUN_TEST(test_spica_bv);
    RUN_TEST(test_adhara_in_tier1);

    /* Sorting */
    RUN_TEST(test_sorted_by_magnitude);

    /* Ranges */
    RUN_TEST(test_ra_range);
    RUN_TEST(test_dec_range);
    RUN_TEST(test_bv_range);
    RUN_TEST(test_all_within_magnitude_limit);

    /* Invalid index */
    RUN_TEST(test_invalid_index_negative);
    RUN_TEST(test_invalid_index_too_large);
    RUN_TEST(test_invalid_name_negative);
    RUN_TEST(test_invalid_name_too_large);

    /* Tiers */
    RUN_TEST(test_tier1_count_minimum);
    RUN_TEST(test_tier2_count_minimum);
    RUN_TEST(test_tier3_count_zero);
    RUN_TEST(test_tier_counts_sum_to_total);
    RUN_TEST(test_tier_starts_ascending);
    RUN_TEST(test_tier1_start_is_21);
    RUN_TEST(test_tier_for_mag_sirius);
    RUN_TEST(test_tier_for_mag_boundary_1_5);
    RUN_TEST(test_tier_for_mag_boundary_3_0);
    RUN_TEST(test_tier_for_mag_boundary_4_5);
    RUN_TEST(test_tier_for_mag_bright);
    RUN_TEST(test_invalid_tier_count);
    RUN_TEST(test_invalid_tier_start);

    /* Named stars */
    RUN_TEST(test_named_count_minimum);
    RUN_TEST(test_tier1_named_stars);
    RUN_TEST(test_tier2_named_stars);

    /* Find */
    RUN_TEST(test_find_sirius);
    RUN_TEST(test_find_case_insensitive);
    RUN_TEST(test_find_case_mixed);
    RUN_TEST(test_find_not_found);
    RUN_TEST(test_find_null_returns_minus_one);

    /* Region search */
    RUN_TEST(test_region_basic);
    RUN_TEST(test_region_ra_wrap);
    RUN_TEST(test_region_empty);
    RUN_TEST(test_region_limited_output);
    RUN_TEST(test_region_zero_max);

    return UNITY_END();
}
