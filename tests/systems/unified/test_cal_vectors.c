/* test_cal_vectors.c — Tests for Calendar Test Vector database
 * Cross-system reference dates verified against historical sources.
 *
 * Pure data module: static const vectors, no conversion logic. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/cal_vectors.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ========== cv_count ========== */

static void test_count_minimum(void)
{
    TEST_ASSERT_GREATER_OR_EQUAL(10, cv_count());
}

static void test_count_positive(void)
{
    TEST_ASSERT_GREATER_THAN(0, cv_count());
}

/* ========== cv_get — valid indices ========== */

static void test_get_index_0_valid(void)
{
    cv_vector_t v = cv_get(0);
    TEST_ASSERT_NOT_NULL(v.name);
}

static void test_get_index_9_valid(void)
{
    cv_vector_t v = cv_get(9);
    TEST_ASSERT_NOT_NULL(v.name);
}

static void test_get_all_valid(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        cv_vector_t v = cv_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(v.name, "Vector name should not be NULL");
    }
}

/* ========== cv_get — invalid indices ========== */

static void test_get_negative_index(void)
{
    cv_vector_t v = cv_get(-1);
    TEST_ASSERT_NULL(v.name);
}

static void test_get_too_large_index(void)
{
    cv_vector_t v = cv_get(99);
    TEST_ASSERT_NULL(v.name);
}

static void test_get_sentinel_description_null(void)
{
    cv_vector_t v = cv_get(-1);
    TEST_ASSERT_NULL(v.description);
}

static void test_get_sentinel_entry_count_zero(void)
{
    cv_vector_t v = cv_get(-1);
    TEST_ASSERT_EQUAL_INT(0, v.entry_count);
}

/* ========== cv_jd ========== */

static void test_jd_j2000(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2451545.0f, (float)cv_jd(0));
}

static void test_jd_invalid_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)cv_jd(-1));
}

static void test_jd_too_large_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)cv_jd(99));
}

static void test_jd_all_positive(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_TRUE(cv_jd(i) > 0.0);
    }
}

static void test_jd_all_reasonable(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        double jd = cv_jd(i);
        TEST_ASSERT_TRUE(jd > 0.0);
        TEST_ASSERT_TRUE(jd < 3000000.0);
    }
}

/* ========== cv_name ========== */

static void test_name_j2000(void)
{
    const char *name = cv_name(0);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_GREATER_THAN(0, (int)strlen(name));
}

static void test_name_invalid_returns_unknown(void)
{
    const char *name = cv_name(-1);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

static void test_name_too_large_returns_unknown(void)
{
    const char *name = cv_name(99);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

static void test_name_all_non_empty(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        const char *name = cv_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_GREATER_THAN(0, (int)strlen(name));
    }
}

/* ========== cv_find_entry ========== */

static void test_find_entry_gregorian_j2000(void)
{
    cv_vector_t v = cv_get(0);
    cv_entry_t e = cv_find_entry(v, 0); /* CAL_GREGORIAN */
    TEST_ASSERT_EQUAL_INT(0, e.system);
    TEST_ASSERT_EQUAL_INT(2000, e.year);
    TEST_ASSERT_EQUAL_INT(1, e.month);
    TEST_ASSERT_EQUAL_INT(1, e.day);
}

static void test_find_entry_hebrew_j2000(void)
{
    cv_vector_t v = cv_get(0);
    cv_entry_t e = cv_find_entry(v, 5); /* CAL_HEBREW */
    TEST_ASSERT_EQUAL_INT(5, e.system);
    TEST_ASSERT_EQUAL_INT(5760, e.year);
    TEST_ASSERT_EQUAL_INT(10, e.month);
    TEST_ASSERT_EQUAL_INT(23, e.day);
}

static void test_find_entry_islamic_j2000(void)
{
    cv_vector_t v = cv_get(0);
    cv_entry_t e = cv_find_entry(v, 6); /* CAL_ISLAMIC */
    TEST_ASSERT_EQUAL_INT(6, e.system);
    TEST_ASSERT_EQUAL_INT(1420, e.year);
    TEST_ASSERT_EQUAL_INT(9, e.month);
    TEST_ASSERT_EQUAL_INT(25, e.day);
}

static void test_find_entry_persian_j2000(void)
{
    cv_vector_t v = cv_get(0);
    cv_entry_t e = cv_find_entry(v, 7); /* CAL_PERSIAN */
    TEST_ASSERT_EQUAL_INT(7, e.system);
    TEST_ASSERT_EQUAL_INT(1378, e.year);
    TEST_ASSERT_EQUAL_INT(10, e.month);
    TEST_ASSERT_EQUAL_INT(11, e.day);
}

static void test_find_entry_coptic_j2000(void)
{
    cv_vector_t v = cv_get(0);
    cv_entry_t e = cv_find_entry(v, 8); /* CAL_COPTIC */
    TEST_ASSERT_EQUAL_INT(8, e.system);
    TEST_ASSERT_EQUAL_INT(1716, e.year);
    TEST_ASSERT_EQUAL_INT(4, e.month);
    TEST_ASSERT_EQUAL_INT(22, e.day);
}

static void test_find_entry_not_found(void)
{
    cv_vector_t v = cv_get(1); /* Gregorian Reform — few entries */
    cv_entry_t e = cv_find_entry(v, 13); /* CAL_BAHAI — not in this vector */
    TEST_ASSERT_EQUAL_INT(-1, e.system);
}

static void test_find_entry_system_minus1_sentinel(void)
{
    cv_vector_t v = cv_get(0);
    cv_entry_t e = cv_find_entry(v, 99); /* nonexistent system */
    TEST_ASSERT_EQUAL_INT(-1, e.system);
}

/* ========== cv_has_system ========== */

static void test_has_system_gregorian_in_j2000(void)
{
    cv_vector_t v = cv_get(0);
    TEST_ASSERT_EQUAL_INT(1, cv_has_system(v, 0));
}

static void test_has_system_hebrew_in_j2000(void)
{
    cv_vector_t v = cv_get(0);
    TEST_ASSERT_EQUAL_INT(1, cv_has_system(v, 5));
}

static void test_has_system_absent(void)
{
    cv_vector_t v = cv_get(1);
    /* Gregorian Reform vector has only Gregorian */
    TEST_ASSERT_EQUAL_INT(0, cv_has_system(v, 13));
}

static void test_has_system_invalid(void)
{
    cv_vector_t v = cv_get(0);
    TEST_ASSERT_EQUAL_INT(0, cv_has_system(v, 99));
}

/* ========== All vectors have Gregorian ========== */

static void test_all_vectors_have_gregorian(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        cv_vector_t v = cv_get(i);
        TEST_ASSERT_EQUAL_INT_MESSAGE(1, cv_has_system(v, 0),
            "Every vector must include a Gregorian date");
    }
}

/* ========== Entry counts valid ========== */

static void test_entry_counts_within_range(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        cv_vector_t v = cv_get(i);
        TEST_ASSERT_GREATER_OR_EQUAL(1, v.entry_count);
        TEST_ASSERT_LESS_OR_EQUAL(CV_MAX_ENTRIES, v.entry_count);
    }
}

/* ========== Specific vector data checks ========== */

/* Vector 1: Gregorian Reform */
static void test_vector1_gregorian_reform(void)
{
    cv_vector_t v = cv_get(1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2299161.0f, (float)v.jd);
    cv_entry_t e = cv_find_entry(v, 0);
    TEST_ASSERT_EQUAL_INT(1582, e.year);
    TEST_ASSERT_EQUAL_INT(10, e.month);
    TEST_ASSERT_EQUAL_INT(15, e.day);
}

/* Vector 2: French Revolution Calendar Start */
static void test_vector2_french_revolution(void)
{
    cv_vector_t v = cv_get(2);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2375840.0f, (float)v.jd);
    cv_entry_t greg = cv_find_entry(v, 0);
    TEST_ASSERT_EQUAL_INT(1792, greg.year);
    TEST_ASSERT_EQUAL_INT(9, greg.month);
    TEST_ASSERT_EQUAL_INT(22, greg.day);
    cv_entry_t fr = cv_find_entry(v, 11); /* CAL_FRENCH_REPUBLICAN */
    TEST_ASSERT_EQUAL_INT(11, fr.system);
    TEST_ASSERT_EQUAL_INT(1, fr.year);
    TEST_ASSERT_EQUAL_INT(1, fr.month);
    TEST_ASSERT_EQUAL_INT(1, fr.day);
}

/* Vector 3: Islamic Epoch */
static void test_vector3_islamic_epoch(void)
{
    cv_vector_t v = cv_get(3);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1948439.5f, (float)v.jd);
    cv_entry_t greg = cv_find_entry(v, 0);
    TEST_ASSERT_EQUAL_INT(622, greg.year);
    TEST_ASSERT_EQUAL_INT(7, greg.month);
    TEST_ASSERT_EQUAL_INT(19, greg.day);
    cv_entry_t isl = cv_find_entry(v, 6);
    TEST_ASSERT_EQUAL_INT(6, isl.system);
    TEST_ASSERT_EQUAL_INT(1, isl.year);
    TEST_ASSERT_EQUAL_INT(1, isl.month);
    TEST_ASSERT_EQUAL_INT(1, isl.day);
}

/* Vector 4: Unix Epoch */
static void test_vector4_unix_epoch(void)
{
    cv_vector_t v = cv_get(4);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2440587.5f, (float)v.jd);
    cv_entry_t greg = cv_find_entry(v, 0);
    TEST_ASSERT_EQUAL_INT(1970, greg.year);
    TEST_ASSERT_EQUAL_INT(1, greg.month);
    TEST_ASSERT_EQUAL_INT(1, greg.day);
}

/* Vector 5: Year 2000 Leap Day */
static void test_vector5_leap_day(void)
{
    cv_vector_t v = cv_get(5);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2451604.5f, (float)v.jd);
    cv_entry_t greg = cv_find_entry(v, 0);
    TEST_ASSERT_EQUAL_INT(2000, greg.year);
    TEST_ASSERT_EQUAL_INT(2, greg.month);
    TEST_ASSERT_EQUAL_INT(29, greg.day);
}

/* Vector 6: Coptic Epoch */
static void test_vector6_coptic_epoch(void)
{
    cv_vector_t v = cv_get(6);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1825029.5f, (float)v.jd);
    cv_entry_t greg = cv_find_entry(v, 0);
    TEST_ASSERT_EQUAL_INT(284, greg.year);
    TEST_ASSERT_EQUAL_INT(8, greg.month);
    TEST_ASSERT_EQUAL_INT(29, greg.day);
    cv_entry_t cop = cv_find_entry(v, 8);
    TEST_ASSERT_EQUAL_INT(8, cop.system);
    TEST_ASSERT_EQUAL_INT(1, cop.year);
    TEST_ASSERT_EQUAL_INT(1, cop.month);
    TEST_ASSERT_EQUAL_INT(1, cop.day);
}

/* Vector 7: Mayan Long Count End */
static void test_vector7_mayan_end(void)
{
    cv_vector_t v = cv_get(7);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2456282.5f, (float)v.jd);
    cv_entry_t greg = cv_find_entry(v, 0);
    TEST_ASSERT_EQUAL_INT(2012, greg.year);
    TEST_ASSERT_EQUAL_INT(12, greg.month);
    TEST_ASSERT_EQUAL_INT(21, greg.day);
}

/* Vector 8: Bahai Epoch */
static void test_vector8_bahai_epoch(void)
{
    cv_vector_t v = cv_get(8);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2394647.0f, (float)v.jd);
    cv_entry_t greg = cv_find_entry(v, 0);
    TEST_ASSERT_EQUAL_INT(1844, greg.year);
    TEST_ASSERT_EQUAL_INT(3, greg.month);
    TEST_ASSERT_EQUAL_INT(21, greg.day);
    cv_entry_t bah = cv_find_entry(v, 13);
    TEST_ASSERT_EQUAL_INT(13, bah.system);
    TEST_ASSERT_EQUAL_INT(1, bah.year);
    TEST_ASSERT_EQUAL_INT(1, bah.month);
    TEST_ASSERT_EQUAL_INT(1, bah.day);
}

/* Vector 9: Persian Epoch */
static void test_vector9_persian_epoch(void)
{
    cv_vector_t v = cv_get(9);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1952063.0f, (float)v.jd);
    cv_entry_t greg = cv_find_entry(v, 0);
    TEST_ASSERT_EQUAL_INT(622, greg.year);
    TEST_ASSERT_EQUAL_INT(3, greg.month);
    TEST_ASSERT_EQUAL_INT(22, greg.day);
    cv_entry_t per = cv_find_entry(v, 7);
    TEST_ASSERT_EQUAL_INT(7, per.system);
    TEST_ASSERT_EQUAL_INT(1, per.year);
    TEST_ASSERT_EQUAL_INT(1, per.month);
    TEST_ASSERT_EQUAL_INT(1, per.day);
}

/* ========== Epoch vectors: year=1, month=1, day=1 ========== */

static void test_islamic_epoch_year1(void)
{
    cv_vector_t v = cv_get(3);
    cv_entry_t e = cv_find_entry(v, 6);
    TEST_ASSERT_EQUAL_INT(1, e.year);
    TEST_ASSERT_EQUAL_INT(1, e.month);
    TEST_ASSERT_EQUAL_INT(1, e.day);
}

static void test_coptic_epoch_year1(void)
{
    cv_vector_t v = cv_get(6);
    cv_entry_t e = cv_find_entry(v, 8);
    TEST_ASSERT_EQUAL_INT(1, e.year);
    TEST_ASSERT_EQUAL_INT(1, e.month);
    TEST_ASSERT_EQUAL_INT(1, e.day);
}

static void test_bahai_epoch_year1(void)
{
    cv_vector_t v = cv_get(8);
    cv_entry_t e = cv_find_entry(v, 13);
    TEST_ASSERT_EQUAL_INT(1, e.year);
    TEST_ASSERT_EQUAL_INT(1, e.month);
    TEST_ASSERT_EQUAL_INT(1, e.day);
}

static void test_persian_epoch_year1(void)
{
    cv_vector_t v = cv_get(9);
    cv_entry_t e = cv_find_entry(v, 7);
    TEST_ASSERT_EQUAL_INT(1, e.year);
    TEST_ASSERT_EQUAL_INT(1, e.month);
    TEST_ASSERT_EQUAL_INT(1, e.day);
}

/* ========== Descriptions non-NULL ========== */

static void test_all_descriptions_non_null(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        cv_vector_t v = cv_get(i);
        TEST_ASSERT_NOT_NULL_MESSAGE(v.description,
            "Every vector should have a description");
    }
}

/* ========== JD consistency: cv_jd matches cv_get.jd ========== */

static void test_jd_matches_get(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)cv_get(i).jd, (float)cv_jd(i));
    }
}

/* ========== Name consistency: cv_name matches cv_get.name ========== */

static void test_name_matches_get(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        cv_vector_t v = cv_get(i);
        TEST_ASSERT_EQUAL_STRING(v.name, cv_name(i));
    }
}

/* ========== French Republican epoch: year 1 ========== */

static void test_french_republican_epoch_year1(void)
{
    cv_vector_t v = cv_get(2);
    cv_entry_t e = cv_find_entry(v, 11);
    TEST_ASSERT_EQUAL_INT(1, e.year);
    TEST_ASSERT_EQUAL_INT(1, e.month);
    TEST_ASSERT_EQUAL_INT(1, e.day);
}

/* ========== System IDs within valid range ========== */

static void test_all_system_ids_valid(void)
{
    int n = cv_count();
    for (int i = 0; i < n; i++) {
        cv_vector_t v = cv_get(i);
        for (int j = 0; j < v.entry_count; j++) {
            TEST_ASSERT_GREATER_OR_EQUAL(0, v.entries[j].system);
            TEST_ASSERT_LESS_OR_EQUAL(20, v.entries[j].system);
        }
    }
}

/* ========== Runner ========== */

int main(void)
{
    UNITY_BEGIN();

    /* cv_count */
    RUN_TEST(test_count_minimum);
    RUN_TEST(test_count_positive);

    /* cv_get — valid */
    RUN_TEST(test_get_index_0_valid);
    RUN_TEST(test_get_index_9_valid);
    RUN_TEST(test_get_all_valid);

    /* cv_get — invalid */
    RUN_TEST(test_get_negative_index);
    RUN_TEST(test_get_too_large_index);
    RUN_TEST(test_get_sentinel_description_null);
    RUN_TEST(test_get_sentinel_entry_count_zero);

    /* cv_jd */
    RUN_TEST(test_jd_j2000);
    RUN_TEST(test_jd_invalid_returns_zero);
    RUN_TEST(test_jd_too_large_returns_zero);
    RUN_TEST(test_jd_all_positive);
    RUN_TEST(test_jd_all_reasonable);

    /* cv_name */
    RUN_TEST(test_name_j2000);
    RUN_TEST(test_name_invalid_returns_unknown);
    RUN_TEST(test_name_too_large_returns_unknown);
    RUN_TEST(test_name_all_non_empty);

    /* cv_find_entry */
    RUN_TEST(test_find_entry_gregorian_j2000);
    RUN_TEST(test_find_entry_hebrew_j2000);
    RUN_TEST(test_find_entry_islamic_j2000);
    RUN_TEST(test_find_entry_persian_j2000);
    RUN_TEST(test_find_entry_coptic_j2000);
    RUN_TEST(test_find_entry_not_found);
    RUN_TEST(test_find_entry_system_minus1_sentinel);

    /* cv_has_system */
    RUN_TEST(test_has_system_gregorian_in_j2000);
    RUN_TEST(test_has_system_hebrew_in_j2000);
    RUN_TEST(test_has_system_absent);
    RUN_TEST(test_has_system_invalid);

    /* All vectors have Gregorian */
    RUN_TEST(test_all_vectors_have_gregorian);

    /* Entry counts */
    RUN_TEST(test_entry_counts_within_range);

    /* Specific vectors */
    RUN_TEST(test_vector1_gregorian_reform);
    RUN_TEST(test_vector2_french_revolution);
    RUN_TEST(test_vector3_islamic_epoch);
    RUN_TEST(test_vector4_unix_epoch);
    RUN_TEST(test_vector5_leap_day);
    RUN_TEST(test_vector6_coptic_epoch);
    RUN_TEST(test_vector7_mayan_end);
    RUN_TEST(test_vector8_bahai_epoch);
    RUN_TEST(test_vector9_persian_epoch);

    /* Epoch year=1 checks */
    RUN_TEST(test_islamic_epoch_year1);
    RUN_TEST(test_coptic_epoch_year1);
    RUN_TEST(test_bahai_epoch_year1);
    RUN_TEST(test_persian_epoch_year1);
    RUN_TEST(test_french_republican_epoch_year1);

    /* Descriptions */
    RUN_TEST(test_all_descriptions_non_null);

    /* Consistency */
    RUN_TEST(test_jd_matches_get);
    RUN_TEST(test_name_matches_get);

    /* System IDs */
    RUN_TEST(test_all_system_ids_valid);

    return UNITY_END();
}
