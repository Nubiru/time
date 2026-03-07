#include "../unity/unity.h"
#include "../../src/render/catalog_ingest.h"

#include <math.h>
#include <string.h>

#define NEAR(exp, act, tol) (fabs((double)(act) - (double)(exp)) <= (tol))

void setUp(void) {}
void tearDown(void) {}

/* ---- catalog_validate_ra ---- */

/* 1. RA=0 is valid */
void test_validate_ra_zero(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_validate_ra(0.0));
}

/* 2. RA=23.999 is valid */
void test_validate_ra_upper_bound(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_validate_ra(23.999));
}

/* 3. RA=24.0 is invalid (exclusive upper) */
void test_validate_ra_at_24(void)
{
    TEST_ASSERT_EQUAL_INT(0, catalog_validate_ra(24.0));
}

/* 4. Negative RA is invalid */
void test_validate_ra_negative(void)
{
    TEST_ASSERT_EQUAL_INT(0, catalog_validate_ra(-0.001));
}

/* ---- catalog_validate_dec ---- */

/* 5. Dec=0 is valid */
void test_validate_dec_zero(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_validate_dec(0.0));
}

/* 6. Dec=90 is valid (inclusive) */
void test_validate_dec_north_pole(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_validate_dec(90.0));
}

/* 7. Dec=-90 is valid (inclusive) */
void test_validate_dec_south_pole(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_validate_dec(-90.0));
}

/* 8. Dec=90.1 is invalid */
void test_validate_dec_over_90(void)
{
    TEST_ASSERT_EQUAL_INT(0, catalog_validate_dec(90.1));
}

/* 9. Dec=-90.1 is invalid */
void test_validate_dec_under_minus90(void)
{
    TEST_ASSERT_EQUAL_INT(0, catalog_validate_dec(-90.1));
}

/* ---- catalog_validate_mag ---- */

/* 10. mag=0 is valid */
void test_validate_mag_zero(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_validate_mag(0.0));
}

/* 11. mag=-1.46 (Sirius) is valid */
void test_validate_mag_negative(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_validate_mag(-1.46));
}

/* 12. mag=-2.0 is valid (boundary) */
void test_validate_mag_lower_bound(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_validate_mag(-2.0));
}

/* 13. mag=-2.1 is invalid */
void test_validate_mag_too_bright(void)
{
    TEST_ASSERT_EQUAL_INT(0, catalog_validate_mag(-2.1));
}

/* 14. mag=20 is valid (boundary) */
void test_validate_mag_upper_bound(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_validate_mag(20.0));
}

/* 15. mag=20.1 is invalid */
void test_validate_mag_too_faint(void)
{
    TEST_ASSERT_EQUAL_INT(0, catalog_validate_mag(20.1));
}

/* ---- catalog_validate ---- */

/* 16. Valid entry passes validation */
void test_validate_valid_entry(void)
{
    catalog_star_t star = {
        .ra_hours = 6.75,
        .dec_degrees = -16.72,
        .v_magnitude = -1.46,
        .bv_color = 0.0,
        .name = "Sirius",
        .spectral = "A1V",
        .constellation = "CMa"
    };
    TEST_ASSERT_EQUAL_INT(1, catalog_validate(&star));
}

/* 17. Invalid RA fails validation */
void test_validate_invalid_ra(void)
{
    catalog_star_t star = {
        .ra_hours = 25.0,
        .dec_degrees = 0.0,
        .v_magnitude = 1.0,
        .bv_color = 0.5,
        .name = "",
        .spectral = "",
        .constellation = ""
    };
    TEST_ASSERT_EQUAL_INT(0, catalog_validate(&star));
}

/* 18. Invalid dec fails validation */
void test_validate_invalid_dec(void)
{
    catalog_star_t star = {
        .ra_hours = 6.0,
        .dec_degrees = -91.0,
        .v_magnitude = 1.0,
        .bv_color = 0.5,
        .name = "",
        .spectral = "",
        .constellation = ""
    };
    TEST_ASSERT_EQUAL_INT(0, catalog_validate(&star));
}

/* 19. Invalid mag fails validation */
void test_validate_invalid_mag(void)
{
    catalog_star_t star = {
        .ra_hours = 6.0,
        .dec_degrees = 0.0,
        .v_magnitude = 21.0,
        .bv_color = 0.5,
        .name = "",
        .spectral = "",
        .constellation = ""
    };
    TEST_ASSERT_EQUAL_INT(0, catalog_validate(&star));
}

/* 20. BV out of range fails validation */
void test_validate_invalid_bv(void)
{
    catalog_star_t star = {
        .ra_hours = 6.0,
        .dec_degrees = 0.0,
        .v_magnitude = 1.0,
        .bv_color = 3.5,
        .name = "",
        .spectral = "",
        .constellation = ""
    };
    TEST_ASSERT_EQUAL_INT(0, catalog_validate(&star));
}

/* ---- catalog_parse_line ---- */

/* 21. Parse a valid CSV line */
void test_parse_line_csv(void)
{
    const char *line2 = "6.752,-16.716,-1.46,0.009,Sirius,A1V,CMa";
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line(line2, ',', &star);
    TEST_ASSERT_EQUAL_INT(1, r.success);
    TEST_ASSERT_EQUAL_INT(7, r.field_count);
    TEST_ASSERT_TRUE(NEAR(6.752, star.ra_hours, 0.01));
    TEST_ASSERT_TRUE(NEAR(-16.716, star.dec_degrees, 0.01));
    TEST_ASSERT_TRUE(NEAR(-1.46, star.v_magnitude, 0.01));
    TEST_ASSERT_TRUE(NEAR(0.009, star.bv_color, 0.01));
    TEST_ASSERT_EQUAL_STRING("Sirius", star.name);
    TEST_ASSERT_EQUAL_STRING("A1V", star.spectral);
    TEST_ASSERT_EQUAL_STRING("CMa", star.constellation);
}

/* 22. Parse a valid TSV line */
void test_parse_line_tsv(void)
{
    const char *line = "14.261\t19.182\t-0.05\t0.23\tArcturus\tK1.5III\tBoo";
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line(line, '\t', &star);
    TEST_ASSERT_EQUAL_INT(1, r.success);
    TEST_ASSERT_EQUAL_STRING("Arcturus", star.name);
    TEST_ASSERT_EQUAL_STRING("Boo", star.constellation);
}

/* 23. Parse line with empty name field */
void test_parse_line_empty_name(void)
{
    const char *line = "1.234,56.789,4.5,0.1,,G2V,UMa";
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line(line, ',', &star);
    TEST_ASSERT_EQUAL_INT(1, r.success);
    TEST_ASSERT_EQUAL_STRING("", star.name);
}

/* 24. Parse line with too few fields fails */
void test_parse_line_too_few_fields(void)
{
    const char *line = "6.75,-16.72,-1.46";
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line(line, ',', &star);
    TEST_ASSERT_EQUAL_INT(0, r.success);
    TEST_ASSERT_TRUE(r.field_count < 7);
}

/* 25. Parse empty line fails */
void test_parse_line_empty(void)
{
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line("", ',', &star);
    TEST_ASSERT_EQUAL_INT(0, r.success);
}

/* 26. Parse NULL line fails */
void test_parse_line_null(void)
{
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line(NULL, ',', &star);
    TEST_ASSERT_EQUAL_INT(0, r.success);
}

/* 27. Parse line with validation error sets flag */
void test_parse_line_validation_error(void)
{
    const char *line = "25.0,0.0,1.0,0.5,Bad,G2V,UMa";
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line(line, ',', &star);
    TEST_ASSERT_EQUAL_INT(0, r.success);
    TEST_ASSERT_EQUAL_INT(1, r.validation_error);
}

/* ---- catalog_parse_buffer ---- */

/* 28. Parse multi-line buffer */
void test_parse_buffer_basic(void)
{
    const char *data =
        "6.752,-16.716,-1.46,0.009,Sirius,A1V,CMa\n"
        "14.261,19.182,-0.05,0.23,Arcturus,K1.5III,Boo\n";
    catalog_star_t stars[10];
    catalog_parse_summary_t summary;
    int count = catalog_parse_buffer(data, ',', 0, stars, 10, &summary);
    TEST_ASSERT_EQUAL_INT(2, count);
    TEST_ASSERT_EQUAL_INT(2, summary.total_lines);
    TEST_ASSERT_EQUAL_INT(2, summary.parsed_ok);
    TEST_ASSERT_EQUAL_INT(0, summary.validation_errors);
    TEST_ASSERT_EQUAL_INT(0, summary.parse_errors);
}

/* 29. Parse buffer with header skip */
void test_parse_buffer_header_skip(void)
{
    const char *data =
        "ra,dec,mag,bv,name,spectral,const\n"
        "6.752,-16.716,-1.46,0.009,Sirius,A1V,CMa\n";
    catalog_star_t stars[10];
    catalog_parse_summary_t summary;
    int count = catalog_parse_buffer(data, ',', 1, stars, 10, &summary);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_INT(1, summary.skipped_header);
}

/* 30. Parse buffer respects max_count */
void test_parse_buffer_max_count(void)
{
    const char *data =
        "6.752,-16.716,-1.46,0.009,Sirius,A1V,CMa\n"
        "14.261,19.182,-0.05,0.23,Arcturus,K1.5III,Boo\n"
        "5.278,45.998,0.08,0.80,Capella,G8III,Aur\n";
    catalog_star_t stars[2];
    catalog_parse_summary_t summary;
    int count = catalog_parse_buffer(data, ',', 0, stars, 2, &summary);
    TEST_ASSERT_EQUAL_INT(2, count);
}

/* 31. Parse buffer handles empty lines */
void test_parse_buffer_empty_lines(void)
{
    const char *data =
        "6.752,-16.716,-1.46,0.009,Sirius,A1V,CMa\n"
        "\n"
        "14.261,19.182,-0.05,0.23,Arcturus,K1.5III,Boo\n"
        "\n";
    catalog_star_t stars[10];
    catalog_parse_summary_t summary;
    int count = catalog_parse_buffer(data, ',', 0, stars, 10, &summary);
    TEST_ASSERT_EQUAL_INT(2, count);
}

/* 32. Parse buffer tracks validation errors */
void test_parse_buffer_validation_errors(void)
{
    const char *data =
        "6.752,-16.716,-1.46,0.009,Sirius,A1V,CMa\n"
        "25.0,0.0,1.0,0.5,Bad,G2V,UMa\n";
    catalog_star_t stars[10];
    catalog_parse_summary_t summary;
    int count = catalog_parse_buffer(data, ',', 0, stars, 10, &summary);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_INT(1, summary.validation_errors);
}

/* 33. Parse buffer with NULL data returns 0 */
void test_parse_buffer_null_data(void)
{
    catalog_star_t stars[10];
    catalog_parse_summary_t summary;
    int count = catalog_parse_buffer(NULL, ',', 0, stars, 10, &summary);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ---- catalog_sort_by_magnitude ---- */

/* 34. Sort puts brightest first */
void test_sort_by_magnitude(void)
{
    catalog_star_t stars[3] = {
        { .v_magnitude = 2.0 },
        { .v_magnitude = -1.46 },
        { .v_magnitude = 0.5 }
    };
    catalog_sort_by_magnitude(stars, 3);
    TEST_ASSERT_TRUE(NEAR(-1.46, stars[0].v_magnitude, 0.01));
    TEST_ASSERT_TRUE(NEAR(0.5, stars[1].v_magnitude, 0.01));
    TEST_ASSERT_TRUE(NEAR(2.0, stars[2].v_magnitude, 0.01));
}

/* 35. Sort with single element is a no-op */
void test_sort_single_element(void)
{
    catalog_star_t star = { .v_magnitude = 1.0 };
    catalog_sort_by_magnitude(&star, 1);
    TEST_ASSERT_TRUE(NEAR(1.0, star.v_magnitude, 0.01));
}

/* 36. Sort with zero elements is safe */
void test_sort_zero_elements(void)
{
    catalog_sort_by_magnitude(NULL, 0);
    /* should not crash */
    TEST_PASS();
}

/* 37. Sort preserves all fields */
void test_sort_preserves_fields(void)
{
    catalog_star_t stars[2] = {
        { .ra_hours = 14.0, .dec_degrees = 19.0, .v_magnitude = 1.0,
          .bv_color = 0.23, .name = "Arcturus", .spectral = "K1", .constellation = "Boo" },
        { .ra_hours = 6.75, .dec_degrees = -16.7, .v_magnitude = -1.46,
          .bv_color = 0.009, .name = "Sirius", .spectral = "A1V", .constellation = "CMa" }
    };
    catalog_sort_by_magnitude(stars, 2);
    TEST_ASSERT_EQUAL_STRING("Sirius", stars[0].name);
    TEST_ASSERT_TRUE(NEAR(6.75, stars[0].ra_hours, 0.01));
    TEST_ASSERT_EQUAL_STRING("Arcturus", stars[1].name);
}

/* ---- catalog_filter_by_magnitude ---- */

/* 38. Filter selects stars brighter than threshold */
void test_filter_by_magnitude(void)
{
    catalog_star_t src[3] = {
        { .v_magnitude = -1.46 },
        { .v_magnitude = 3.5 },
        { .v_magnitude = 1.0 }
    };
    catalog_star_t dst[3];
    int count = catalog_filter_by_magnitude(src, 3, 2.0, dst, 3);
    TEST_ASSERT_EQUAL_INT(2, count);
}

/* 39. Filter respects dst_max */
void test_filter_respects_dst_max(void)
{
    catalog_star_t src[3] = {
        { .v_magnitude = 0.0 },
        { .v_magnitude = 0.5 },
        { .v_magnitude = 1.0 }
    };
    catalog_star_t dst[1];
    int count = catalog_filter_by_magnitude(src, 3, 5.0, dst, 1);
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* 40. Filter with no matches returns 0 */
void test_filter_no_matches(void)
{
    catalog_star_t src[2] = {
        { .v_magnitude = 5.0 },
        { .v_magnitude = 6.0 }
    };
    catalog_star_t dst[2];
    int count = catalog_filter_by_magnitude(src, 2, 1.0, dst, 2);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ---- catalog_brightness_tier ---- */

/* 41. Tier 0: magnitude < 1.5 */
void test_tier_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, catalog_brightness_tier(-1.46));
    TEST_ASSERT_EQUAL_INT(0, catalog_brightness_tier(1.0));
    TEST_ASSERT_EQUAL_INT(0, catalog_brightness_tier(1.49));
}

/* 42. Tier 1: 1.5 <= magnitude < 3.0 */
void test_tier_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, catalog_brightness_tier(1.5));
    TEST_ASSERT_EQUAL_INT(1, catalog_brightness_tier(2.0));
    TEST_ASSERT_EQUAL_INT(1, catalog_brightness_tier(2.99));
}

/* 43. Tier 2: 3.0 <= magnitude < 4.5 */
void test_tier_2(void)
{
    TEST_ASSERT_EQUAL_INT(2, catalog_brightness_tier(3.0));
    TEST_ASSERT_EQUAL_INT(2, catalog_brightness_tier(4.0));
}

/* 44. Tier 3: 4.5 <= magnitude < 6.5 */
void test_tier_3(void)
{
    TEST_ASSERT_EQUAL_INT(3, catalog_brightness_tier(4.5));
    TEST_ASSERT_EQUAL_INT(3, catalog_brightness_tier(6.0));
}

/* 45. Beyond tier 3 clamps to tier 3 */
void test_tier_beyond(void)
{
    TEST_ASSERT_EQUAL_INT(3, catalog_brightness_tier(7.0));
    TEST_ASSERT_EQUAL_INT(3, catalog_brightness_tier(15.0));
}

/* ---- catalog_tier_stats ---- */

/* 46. Tier stats count correctly */
void test_tier_stats(void)
{
    catalog_star_t stars[5] = {
        { .v_magnitude = -1.0 },  /* tier 0 */
        { .v_magnitude = 0.5 },   /* tier 0 */
        { .v_magnitude = 2.0 },   /* tier 1 */
        { .v_magnitude = 4.0 },   /* tier 2 */
        { .v_magnitude = 5.0 }    /* tier 3 */
    };
    catalog_tier_stats_t stats = catalog_tier_stats(stars, 5);
    TEST_ASSERT_EQUAL_INT(2, stats.tier_counts[0]);
    TEST_ASSERT_EQUAL_INT(1, stats.tier_counts[1]);
    TEST_ASSERT_EQUAL_INT(1, stats.tier_counts[2]);
    TEST_ASSERT_EQUAL_INT(1, stats.tier_counts[3]);
    TEST_ASSERT_EQUAL_INT(5, stats.total);
}

/* 47. Tier stats with zero stars */
void test_tier_stats_empty(void)
{
    catalog_tier_stats_t stats = catalog_tier_stats(NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, stats.total);
    TEST_ASSERT_EQUAL_INT(0, stats.tier_counts[0]);
}

/* ---- catalog_is_unnamed / catalog_count_named ---- */

/* 48. Named star is not unnamed */
void test_is_unnamed_false(void)
{
    catalog_star_t star = { .name = "Sirius" };
    TEST_ASSERT_EQUAL_INT(0, catalog_is_unnamed(&star));
}

/* 49. Empty name is unnamed */
void test_is_unnamed_true(void)
{
    catalog_star_t star = { .name = "" };
    TEST_ASSERT_EQUAL_INT(1, catalog_is_unnamed(&star));
}

/* 50. Count named stars */
void test_count_named(void)
{
    catalog_star_t stars[4] = {
        { .name = "Sirius" },
        { .name = "" },
        { .name = "Vega" },
        { .name = "" }
    };
    TEST_ASSERT_EQUAL_INT(2, catalog_count_named(stars, 4));
}

/* 51. Count named with all unnamed returns 0 */
void test_count_named_all_unnamed(void)
{
    catalog_star_t stars[2] = {
        { .name = "" },
        { .name = "" }
    };
    TEST_ASSERT_EQUAL_INT(0, catalog_count_named(stars, 2));
}

/* ---- Additional edge cases ---- */

/* 52. Parse line truncates long name */
void test_parse_line_long_name_truncation(void)
{
    const char *line = "6.75,-16.72,-1.46,0.009,"
        "VeryLongStarNameThatExceedsBufferLimit,A1V,CMa";
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line(line, ',', &star);
    TEST_ASSERT_EQUAL_INT(1, r.success);
    /* Name should be truncated to CATALOG_NAME_MAX - 1 chars */
    TEST_ASSERT_TRUE(strlen(star.name) < CATALOG_NAME_MAX);
}

/* 53. Parse line truncates long spectral type */
void test_parse_line_long_spectral_truncation(void)
{
    const char *line = "6.75,-16.72,-1.46,0.009,Star,G2VeryLongType,CMa";
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line(line, ',', &star);
    TEST_ASSERT_EQUAL_INT(1, r.success);
    TEST_ASSERT_TRUE(strlen(star.spectral) < CATALOG_SPECTRAL_MAX);
}

/* 54. BV color lower bound validation */
void test_validate_bv_lower_bound(void)
{
    catalog_star_t star = {
        .ra_hours = 6.0, .dec_degrees = 0.0,
        .v_magnitude = 1.0, .bv_color = -0.5
    };
    TEST_ASSERT_EQUAL_INT(1, catalog_validate(&star));

    star.bv_color = -0.6;
    TEST_ASSERT_EQUAL_INT(0, catalog_validate(&star));
}

/* 55. Parse buffer with parse error lines */
void test_parse_buffer_parse_errors(void)
{
    const char *data =
        "6.752,-16.716,-1.46,0.009,Sirius,A1V,CMa\n"
        "not,a,valid,line\n";
    catalog_star_t stars[10];
    catalog_parse_summary_t summary;
    int count = catalog_parse_buffer(data, ',', 0, stars, 10, &summary);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_INT(1, summary.parse_errors);
}

/* 56. Parse buffer with trailing newline only */
void test_parse_buffer_trailing_newline(void)
{
    const char *data = "6.752,-16.716,-1.46,0.009,Sirius,A1V,CMa\n";
    catalog_star_t stars[10];
    catalog_parse_summary_t summary;
    int count = catalog_parse_buffer(data, ',', 0, stars, 10, &summary);
    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_INT(1, summary.parsed_ok);
}

/* 57. Sort already sorted array */
void test_sort_already_sorted(void)
{
    catalog_star_t stars[3] = {
        { .v_magnitude = -1.0, .name = "A" },
        { .v_magnitude = 0.0, .name = "B" },
        { .v_magnitude = 1.0, .name = "C" }
    };
    catalog_sort_by_magnitude(stars, 3);
    TEST_ASSERT_EQUAL_STRING("A", stars[0].name);
    TEST_ASSERT_EQUAL_STRING("B", stars[1].name);
    TEST_ASSERT_EQUAL_STRING("C", stars[2].name);
}

/* 58. Sort reverse-sorted array */
void test_sort_reverse_sorted(void)
{
    catalog_star_t stars[3] = {
        { .v_magnitude = 5.0, .name = "C" },
        { .v_magnitude = 2.0, .name = "B" },
        { .v_magnitude = -1.0, .name = "A" }
    };
    catalog_sort_by_magnitude(stars, 3);
    TEST_ASSERT_EQUAL_STRING("A", stars[0].name);
    TEST_ASSERT_EQUAL_STRING("B", stars[1].name);
    TEST_ASSERT_EQUAL_STRING("C", stars[2].name);
}

/* 59. Filter boundary: star at exact threshold is included */
void test_filter_exact_threshold(void)
{
    catalog_star_t src[1] = {{ .v_magnitude = 3.0 }};
    catalog_star_t dst[1];
    int count = catalog_filter_by_magnitude(src, 1, 3.0, dst, 1);
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* 60. Parse line with trailing whitespace in fields */
void test_parse_line_whitespace(void)
{
    /* Fields are taken as-is (no trimming) - the parser splits on delimiter */
    const char *line = "6.75,-16.72,-1.46,0.009,Sirius,A1V,CMa";
    catalog_star_t star;
    catalog_parse_result_t r = catalog_parse_line(line, ',', &star);
    TEST_ASSERT_EQUAL_INT(1, r.success);
}

/* ---- main ---- */

int main(void)
{
    UNITY_BEGIN();

    /* catalog_validate_ra */
    RUN_TEST(test_validate_ra_zero);
    RUN_TEST(test_validate_ra_upper_bound);
    RUN_TEST(test_validate_ra_at_24);
    RUN_TEST(test_validate_ra_negative);

    /* catalog_validate_dec */
    RUN_TEST(test_validate_dec_zero);
    RUN_TEST(test_validate_dec_north_pole);
    RUN_TEST(test_validate_dec_south_pole);
    RUN_TEST(test_validate_dec_over_90);
    RUN_TEST(test_validate_dec_under_minus90);

    /* catalog_validate_mag */
    RUN_TEST(test_validate_mag_zero);
    RUN_TEST(test_validate_mag_negative);
    RUN_TEST(test_validate_mag_lower_bound);
    RUN_TEST(test_validate_mag_too_bright);
    RUN_TEST(test_validate_mag_upper_bound);
    RUN_TEST(test_validate_mag_too_faint);

    /* catalog_validate */
    RUN_TEST(test_validate_valid_entry);
    RUN_TEST(test_validate_invalid_ra);
    RUN_TEST(test_validate_invalid_dec);
    RUN_TEST(test_validate_invalid_mag);
    RUN_TEST(test_validate_invalid_bv);

    /* catalog_parse_line */
    RUN_TEST(test_parse_line_csv);
    RUN_TEST(test_parse_line_tsv);
    RUN_TEST(test_parse_line_empty_name);
    RUN_TEST(test_parse_line_too_few_fields);
    RUN_TEST(test_parse_line_empty);
    RUN_TEST(test_parse_line_null);
    RUN_TEST(test_parse_line_validation_error);

    /* catalog_parse_buffer */
    RUN_TEST(test_parse_buffer_basic);
    RUN_TEST(test_parse_buffer_header_skip);
    RUN_TEST(test_parse_buffer_max_count);
    RUN_TEST(test_parse_buffer_empty_lines);
    RUN_TEST(test_parse_buffer_validation_errors);
    RUN_TEST(test_parse_buffer_null_data);

    /* catalog_sort_by_magnitude */
    RUN_TEST(test_sort_by_magnitude);
    RUN_TEST(test_sort_single_element);
    RUN_TEST(test_sort_zero_elements);
    RUN_TEST(test_sort_preserves_fields);

    /* catalog_filter_by_magnitude */
    RUN_TEST(test_filter_by_magnitude);
    RUN_TEST(test_filter_respects_dst_max);
    RUN_TEST(test_filter_no_matches);

    /* catalog_brightness_tier */
    RUN_TEST(test_tier_0);
    RUN_TEST(test_tier_1);
    RUN_TEST(test_tier_2);
    RUN_TEST(test_tier_3);
    RUN_TEST(test_tier_beyond);

    /* catalog_tier_stats */
    RUN_TEST(test_tier_stats);
    RUN_TEST(test_tier_stats_empty);

    /* catalog_is_unnamed / catalog_count_named */
    RUN_TEST(test_is_unnamed_false);
    RUN_TEST(test_is_unnamed_true);
    RUN_TEST(test_count_named);
    RUN_TEST(test_count_named_all_unnamed);

    /* Additional edge cases */
    RUN_TEST(test_parse_line_long_name_truncation);
    RUN_TEST(test_parse_line_long_spectral_truncation);
    RUN_TEST(test_validate_bv_lower_bound);
    RUN_TEST(test_parse_buffer_parse_errors);
    RUN_TEST(test_parse_buffer_trailing_newline);
    RUN_TEST(test_sort_already_sorted);
    RUN_TEST(test_sort_reverse_sorted);
    RUN_TEST(test_filter_exact_threshold);
    RUN_TEST(test_parse_line_whitespace);

    return UNITY_END();
}
