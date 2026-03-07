/* test_achievement.c — Historical Achievement Data tests
 * TDD RED phase: tests written before implementation.
 * Tests the achievement lookup module for ancient astronomers' precision data. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/achievement.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== achievement_count ===== */

void test_count_at_least_17(void)
{
    TEST_ASSERT_TRUE(achievement_count() >= 17);
}

/* ===== achievement_get — basic retrieval ===== */

void test_get_first_entry(void)
{
    achievement_t a = achievement_get(0);
    TEST_ASSERT_EQUAL_INT(0, a.id);
    TEST_ASSERT_NOT_NULL(a.person);
    TEST_ASSERT_NOT_NULL(a.culture);
    TEST_ASSERT_NOT_NULL(a.description);
    TEST_ASSERT_NOT_NULL(a.unit);
    TEST_ASSERT_NOT_NULL(a.tools_available);
    TEST_ASSERT_NOT_NULL(a.tools_NOT_available);
    TEST_ASSERT_NOT_NULL(a.significance);
}

void test_get_last_entry(void)
{
    int count = achievement_count();
    achievement_t a = achievement_get(count - 1);
    TEST_ASSERT_EQUAL_INT(count - 1, a.id);
    TEST_ASSERT_NOT_NULL(a.person);
}

void test_get_invalid_negative(void)
{
    achievement_t a = achievement_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, a.id);
}

void test_get_invalid_too_large(void)
{
    achievement_t a = achievement_get(9999);
    TEST_ASSERT_EQUAL_INT(-1, a.id);
}

/* ===== Specific achievement data validation ===== */

void test_eratosthenes_data(void)
{
    achievement_t a = achievement_get(0);
    TEST_ASSERT_EQUAL_STRING("Eratosthenes", a.person);
    TEST_ASSERT_EQUAL_INT(-240, a.year);
    TEST_ASSERT_EQUAL_STRING("Greek", a.culture);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 39375.0, a.value_computed);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 40075.0, a.modern_value);
    TEST_ASSERT_EQUAL_STRING("km", a.unit);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_GEOMETRIC, a.category);
}

void test_eratosthenes_error(void)
{
    achievement_t a = achievement_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 700.0, a.absolute_error);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.747, a.relative_error_pct);
}

void test_hipparchus_tropical_year(void)
{
    achievement_t a = achievement_get(1);
    TEST_ASSERT_EQUAL_STRING("Hipparchus", a.person);
    TEST_ASSERT_EQUAL_INT(-130, a.year);
    TEST_ASSERT_EQUAL_STRING("Greek", a.culture);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 365.2467, a.value_computed);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 365.2422, a.modern_value);
    TEST_ASSERT_EQUAL_STRING("days", a.unit);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_ORBITAL, a.category);
}

void test_hipparchus_precession(void)
{
    achievement_t a = achievement_get(2);
    TEST_ASSERT_EQUAL_STRING("Hipparchus", a.person);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 46.0, a.value_computed);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 50.3, a.modern_value);
    TEST_ASSERT_EQUAL_STRING("arcsec/year", a.unit);
}

void test_ptolemy_obliquity(void)
{
    achievement_t a = achievement_get(3);
    TEST_ASSERT_EQUAL_STRING("Ptolemy", a.person);
    TEST_ASSERT_EQUAL_INT(150, a.year);
    TEST_ASSERT_EQUAL_STRING("Greco-Egyptian", a.culture);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_GEOMETRIC, a.category);
}

void test_aryabhata_circumference(void)
{
    achievement_t a = achievement_get(4);
    TEST_ASSERT_EQUAL_STRING("Aryabhata", a.person);
    TEST_ASSERT_EQUAL_INT(499, a.year);
    TEST_ASSERT_EQUAL_STRING("Indian", a.culture);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 39968.0, a.value_computed);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.267, a.relative_error_pct);
}

void test_aryabhata_sidereal_year(void)
{
    achievement_t a = achievement_get(5);
    TEST_ASSERT_EQUAL_STRING("Aryabhata", a.person);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 365.25858, a.value_computed);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_ORBITAL, a.category);
}

void test_aryabhata_lunar_month(void)
{
    achievement_t a = achievement_get(6);
    TEST_ASSERT_EQUAL_STRING("Aryabhata", a.person);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 29.5306, a.value_computed);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_LUNAR, a.category);
}

void test_brahmagupta_lunar(void)
{
    achievement_t a = achievement_get(7);
    TEST_ASSERT_EQUAL_STRING("Brahmagupta", a.person);
    TEST_ASSERT_EQUAL_INT(628, a.year);
    TEST_ASSERT_EQUAL_STRING("Indian", a.culture);
    TEST_ASSERT_FLOAT_WITHIN(0.000001, 29.530582, a.value_computed);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_LUNAR, a.category);
}

void test_hillel_molad(void)
{
    achievement_t a = achievement_get(8);
    TEST_ASSERT_EQUAL_STRING("Hillel II", a.person);
    TEST_ASSERT_EQUAL_INT(359, a.year);
    TEST_ASSERT_EQUAL_STRING("Jewish", a.culture);
    TEST_ASSERT_FLOAT_WITHIN(0.000001, 29.530594, a.value_computed);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_CALENDAR, a.category);
}

void test_al_khwarizmi_sine(void)
{
    achievement_t a = achievement_get(9);
    TEST_ASSERT_EQUAL_STRING("Al-Khwarizmi", a.person);
    TEST_ASSERT_EQUAL_INT(820, a.year);
    TEST_ASSERT_EQUAL_STRING("Arab", a.culture);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_MATHEMATICAL, a.category);
}

void test_maya_venus(void)
{
    achievement_t a = achievement_get(10);
    TEST_ASSERT_EQUAL_STRING("Maya astronomers", a.person);
    TEST_ASSERT_EQUAL_INT(800, a.year);
    TEST_ASSERT_EQUAL_STRING("Maya", a.culture);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 583.92, a.value_computed);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_PLANETARY, a.category);
}

void test_maya_lunar_405(void)
{
    achievement_t a = achievement_get(11);
    TEST_ASSERT_EQUAL_STRING("Maya astronomers", a.person);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 11959.888, a.value_computed);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_LUNAR, a.category);
}

void test_ulugh_beg_sidereal(void)
{
    achievement_t a = achievement_get(12);
    TEST_ASSERT_EQUAL_STRING("Ulugh Beg", a.person);
    TEST_ASSERT_EQUAL_INT(1437, a.year);
    TEST_ASSERT_EQUAL_STRING("Timurid", a.culture);
    TEST_ASSERT_FLOAT_WITHIN(0.00001, 365.25636, a.value_computed);
    TEST_ASSERT_FLOAT_WITHIN(0.00001, 365.25636, a.modern_value);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, a.absolute_error);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_ORBITAL, a.category);
}

void test_su_song_tropical(void)
{
    achievement_t a = achievement_get(13);
    TEST_ASSERT_EQUAL_STRING("Su Song", a.person);
    TEST_ASSERT_EQUAL_INT(1088, a.year);
    TEST_ASSERT_EQUAL_STRING("Chinese", a.culture);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_ORBITAL, a.category);
}

void test_copernicus_sidereal(void)
{
    achievement_t a = achievement_get(14);
    TEST_ASSERT_EQUAL_STRING("Copernicus", a.person);
    TEST_ASSERT_EQUAL_INT(1543, a.year);
    TEST_ASSERT_EQUAL_STRING("Polish", a.culture);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_ORBITAL, a.category);
}

void test_aristarchus_distance(void)
{
    achievement_t a = achievement_get(15);
    TEST_ASSERT_EQUAL_STRING("Aristarchus", a.person);
    TEST_ASSERT_EQUAL_INT(-270, a.year);
    TEST_ASSERT_EQUAL_STRING("Greek", a.culture);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 19.0, a.value_computed);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 389.0, a.modern_value);
    TEST_ASSERT_EQUAL_STRING("lunar distances", a.unit);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_GEOMETRIC, a.category);
}

void test_tycho_brahe_star_positions(void)
{
    achievement_t a = achievement_get(16);
    TEST_ASSERT_EQUAL_STRING("Tycho Brahe", a.person);
    TEST_ASSERT_EQUAL_INT(1590, a.year);
    TEST_ASSERT_EQUAL_STRING("Danish", a.culture);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0, a.value_computed);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.017, a.modern_value);
    TEST_ASSERT_EQUAL_STRING("arcminutes", a.unit);
    TEST_ASSERT_EQUAL_INT(ACH_CAT_GEOMETRIC, a.category);
}

/* ===== achievement_by_culture ===== */

void test_by_culture_greek(void)
{
    int indices[32];
    int found = achievement_by_culture("Greek", indices, 32);
    /* Eratosthenes, Hipparchus x2, Aristarchus = 4 */
    TEST_ASSERT_TRUE(found >= 4);
}

void test_by_culture_indian(void)
{
    int indices[32];
    int found = achievement_by_culture("Indian", indices, 32);
    /* Aryabhata x3, Brahmagupta = 4 */
    TEST_ASSERT_TRUE(found >= 4);
}

void test_by_culture_maya(void)
{
    int indices[32];
    int found = achievement_by_culture("Maya", indices, 32);
    TEST_ASSERT_TRUE(found >= 2);
}

void test_by_culture_not_found(void)
{
    int indices[32];
    int found = achievement_by_culture("Atlantean", indices, 32);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_culture_case_sensitive(void)
{
    int indices[32];
    int found = achievement_by_culture("greek", indices, 32);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_by_culture_max_limit(void)
{
    int indices[2];
    int found = achievement_by_culture("Greek", indices, 2);
    TEST_ASSERT_TRUE(found <= 2);
}

/* ===== achievement_by_person ===== */

void test_by_person_aryabhata(void)
{
    int indices[32];
    int found = achievement_by_person("Aryabhata", indices, 32);
    TEST_ASSERT_TRUE(found >= 3);
}

void test_by_person_hipparchus(void)
{
    int indices[32];
    int found = achievement_by_person("Hipparchus", indices, 32);
    TEST_ASSERT_TRUE(found >= 2);
}

void test_by_person_maya(void)
{
    int indices[32];
    int found = achievement_by_person("Maya astronomers", indices, 32);
    TEST_ASSERT_TRUE(found >= 2);
}

void test_by_person_not_found(void)
{
    int indices[32];
    int found = achievement_by_person("Galileo", indices, 32);
    TEST_ASSERT_EQUAL_INT(0, found);
}

/* ===== achievement_by_category ===== */

void test_by_category_orbital(void)
{
    int indices[32];
    int found = achievement_by_category(ACH_CAT_ORBITAL, indices, 32);
    /* Hipparchus tropical, Hipparchus precession, Aryabhata sidereal,
     * Ulugh Beg, Su Song, Copernicus = 6 */
    TEST_ASSERT_TRUE(found >= 6);
}

void test_by_category_geometric(void)
{
    int indices[32];
    int found = achievement_by_category(ACH_CAT_GEOMETRIC, indices, 32);
    /* Eratosthenes, Ptolemy, Aryabhata circ, Aristarchus, Tycho = 5 */
    TEST_ASSERT_TRUE(found >= 5);
}

void test_by_category_lunar(void)
{
    int indices[32];
    int found = achievement_by_category(ACH_CAT_LUNAR, indices, 32);
    /* Aryabhata, Brahmagupta, Maya = 3 */
    TEST_ASSERT_TRUE(found >= 3);
}

void test_by_category_planetary(void)
{
    int indices[32];
    int found = achievement_by_category(ACH_CAT_PLANETARY, indices, 32);
    TEST_ASSERT_TRUE(found >= 1);
}

void test_by_category_calendar(void)
{
    int indices[32];
    int found = achievement_by_category(ACH_CAT_CALENDAR, indices, 32);
    TEST_ASSERT_TRUE(found >= 1);
}

void test_by_category_mathematical(void)
{
    int indices[32];
    int found = achievement_by_category(ACH_CAT_MATHEMATICAL, indices, 32);
    TEST_ASSERT_TRUE(found >= 1);
}

/* ===== achievement_error_ratio ===== */

void test_error_ratio_eratosthenes(void)
{
    /* modern / absolute_error = 40075 / 700 = ~57.25 */
    double ratio = achievement_error_ratio(0);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 57.25, ratio);
}

void test_error_ratio_ulugh_beg_zero_error(void)
{
    /* Zero error should return 0.0 (special case) */
    double ratio = achievement_error_ratio(12);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, ratio);
}

void test_error_ratio_invalid_index(void)
{
    double ratio = achievement_error_ratio(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, ratio);
}

void test_error_ratio_brahmagupta(void)
{
    /* 29.530589 / 0.000007 = ~4218655.57 — extremely impressive */
    double ratio = achievement_error_ratio(7);
    TEST_ASSERT_TRUE(ratio > 4000000.0);
}

/* ===== achievement_accuracy_pct ===== */

void test_accuracy_eratosthenes(void)
{
    /* 100.0 - 1.747 = ~98.25 */
    double acc = achievement_accuracy_pct(0);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 98.25, acc);
}

void test_accuracy_ulugh_beg(void)
{
    /* 100.0 - 0.0 = 100.0 */
    double acc = achievement_accuracy_pct(12);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 100.0, acc);
}

void test_accuracy_invalid(void)
{
    double acc = achievement_accuracy_pct(-1);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, acc);
}

void test_accuracy_brahmagupta(void)
{
    /* Extremely close to 100% */
    double acc = achievement_accuracy_pct(7);
    TEST_ASSERT_TRUE(acc > 99.999);
}

/* ===== achievement_category_name ===== */

void test_category_name_orbital(void)
{
    TEST_ASSERT_EQUAL_STRING("Orbital", achievement_category_name(ACH_CAT_ORBITAL));
}

void test_category_name_geometric(void)
{
    TEST_ASSERT_EQUAL_STRING("Geometric", achievement_category_name(ACH_CAT_GEOMETRIC));
}

void test_category_name_lunar(void)
{
    TEST_ASSERT_EQUAL_STRING("Lunar", achievement_category_name(ACH_CAT_LUNAR));
}

void test_category_name_planetary(void)
{
    TEST_ASSERT_EQUAL_STRING("Planetary", achievement_category_name(ACH_CAT_PLANETARY));
}

void test_category_name_calendar(void)
{
    TEST_ASSERT_EQUAL_STRING("Calendar", achievement_category_name(ACH_CAT_CALENDAR));
}

void test_category_name_mathematical(void)
{
    TEST_ASSERT_EQUAL_STRING("Mathematical", achievement_category_name(ACH_CAT_MATHEMATICAL));
}

void test_category_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", achievement_category_name(ACH_CAT_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", achievement_category_name((achievement_category_t)-1));
}

/* ===== achievement_culture_count / culture_get ===== */

void test_culture_count(void)
{
    /* Greek, Greco-Egyptian, Indian, Jewish, Arab, Maya, Timurid,
     * Chinese, Polish, Danish = 10 distinct cultures */
    int count = achievement_culture_count();
    TEST_ASSERT_TRUE(count >= 10);
}

void test_culture_get_valid(void)
{
    const char *c = achievement_culture_get(0);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_TRUE(strlen(c) > 0);
}

void test_culture_get_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", achievement_culture_get(-1));
    TEST_ASSERT_EQUAL_STRING("?", achievement_culture_get(9999));
}

/* ===== achievement_person_count ===== */

void test_person_count(void)
{
    /* Eratosthenes, Hipparchus, Ptolemy, Aryabhata, Brahmagupta,
     * Hillel II, Al-Khwarizmi, Maya astronomers, Ulugh Beg,
     * Su Song, Copernicus, Aristarchus, Tycho Brahe = 13 */
    int count = achievement_person_count();
    TEST_ASSERT_TRUE(count >= 13);
}

/* ===== Cross-checks: absolute_error and relative_error_pct consistency ===== */

void test_all_errors_nonnegative(void)
{
    int count = achievement_count();
    for (int i = 0; i < count; i++) {
        achievement_t a = achievement_get(i);
        TEST_ASSERT_TRUE(a.absolute_error >= 0.0);
        TEST_ASSERT_TRUE(a.relative_error_pct >= 0.0);
    }
}

void test_all_entries_have_strings(void)
{
    int count = achievement_count();
    for (int i = 0; i < count; i++) {
        achievement_t a = achievement_get(i);
        TEST_ASSERT_NOT_NULL(a.person);
        TEST_ASSERT_NOT_NULL(a.culture);
        TEST_ASSERT_NOT_NULL(a.description);
        TEST_ASSERT_NOT_NULL(a.unit);
        TEST_ASSERT_NOT_NULL(a.significance);
        TEST_ASSERT_NOT_NULL(a.tools_available);
        TEST_ASSERT_NOT_NULL(a.tools_NOT_available);
    }
}

void test_all_ids_sequential(void)
{
    int count = achievement_count();
    for (int i = 0; i < count; i++) {
        achievement_t a = achievement_get(i);
        TEST_ASSERT_EQUAL_INT(i, a.id);
    }
}

void test_all_categories_valid(void)
{
    int count = achievement_count();
    for (int i = 0; i < count; i++) {
        achievement_t a = achievement_get(i);
        TEST_ASSERT_TRUE(a.category >= 0);
        TEST_ASSERT_TRUE(a.category < ACH_CAT_COUNT);
    }
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_count_at_least_17);

    /* Get — basic */
    RUN_TEST(test_get_first_entry);
    RUN_TEST(test_get_last_entry);
    RUN_TEST(test_get_invalid_negative);
    RUN_TEST(test_get_invalid_too_large);

    /* Specific entries */
    RUN_TEST(test_eratosthenes_data);
    RUN_TEST(test_eratosthenes_error);
    RUN_TEST(test_hipparchus_tropical_year);
    RUN_TEST(test_hipparchus_precession);
    RUN_TEST(test_ptolemy_obliquity);
    RUN_TEST(test_aryabhata_circumference);
    RUN_TEST(test_aryabhata_sidereal_year);
    RUN_TEST(test_aryabhata_lunar_month);
    RUN_TEST(test_brahmagupta_lunar);
    RUN_TEST(test_hillel_molad);
    RUN_TEST(test_al_khwarizmi_sine);
    RUN_TEST(test_maya_venus);
    RUN_TEST(test_maya_lunar_405);
    RUN_TEST(test_ulugh_beg_sidereal);
    RUN_TEST(test_su_song_tropical);
    RUN_TEST(test_copernicus_sidereal);
    RUN_TEST(test_aristarchus_distance);
    RUN_TEST(test_tycho_brahe_star_positions);

    /* By culture */
    RUN_TEST(test_by_culture_greek);
    RUN_TEST(test_by_culture_indian);
    RUN_TEST(test_by_culture_maya);
    RUN_TEST(test_by_culture_not_found);
    RUN_TEST(test_by_culture_case_sensitive);
    RUN_TEST(test_by_culture_max_limit);

    /* By person */
    RUN_TEST(test_by_person_aryabhata);
    RUN_TEST(test_by_person_hipparchus);
    RUN_TEST(test_by_person_maya);
    RUN_TEST(test_by_person_not_found);

    /* By category */
    RUN_TEST(test_by_category_orbital);
    RUN_TEST(test_by_category_geometric);
    RUN_TEST(test_by_category_lunar);
    RUN_TEST(test_by_category_planetary);
    RUN_TEST(test_by_category_calendar);
    RUN_TEST(test_by_category_mathematical);

    /* Error ratio */
    RUN_TEST(test_error_ratio_eratosthenes);
    RUN_TEST(test_error_ratio_ulugh_beg_zero_error);
    RUN_TEST(test_error_ratio_invalid_index);
    RUN_TEST(test_error_ratio_brahmagupta);

    /* Accuracy */
    RUN_TEST(test_accuracy_eratosthenes);
    RUN_TEST(test_accuracy_ulugh_beg);
    RUN_TEST(test_accuracy_invalid);
    RUN_TEST(test_accuracy_brahmagupta);

    /* Category names */
    RUN_TEST(test_category_name_orbital);
    RUN_TEST(test_category_name_geometric);
    RUN_TEST(test_category_name_lunar);
    RUN_TEST(test_category_name_planetary);
    RUN_TEST(test_category_name_calendar);
    RUN_TEST(test_category_name_mathematical);
    RUN_TEST(test_category_name_invalid);

    /* Culture count/get */
    RUN_TEST(test_culture_count);
    RUN_TEST(test_culture_get_valid);
    RUN_TEST(test_culture_get_invalid);

    /* Person count */
    RUN_TEST(test_person_count);

    /* Cross-checks */
    RUN_TEST(test_all_errors_nonnegative);
    RUN_TEST(test_all_entries_have_strings);
    RUN_TEST(test_all_ids_sequential);
    RUN_TEST(test_all_categories_valid);

    return UNITY_END();
}
