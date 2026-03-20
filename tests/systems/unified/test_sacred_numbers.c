/* test_sacred_numbers.c — Sacred Number Registry tests
 * TDD RED phase: tests written before implementation.
 * Tests the cross-system sacred number lookup module. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/sacred_numbers.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== sacred_number_count ===== */

void test_count_at_least_20(void)
{
    TEST_ASSERT_TRUE(sacred_number_count() >= 20);
}

/* ===== sacred_number_get — lookup by value ===== */

void test_get_three(void)
{
    sacred_number_t n = sacred_number_get(3);
    TEST_ASSERT_EQUAL_INT(3, n.value);
    TEST_ASSERT_NOT_NULL(n.name);
    TEST_ASSERT_NOT_NULL(n.significance);
    TEST_ASSERT_TRUE(n.appearance_count >= 3);
}

void test_get_seven(void)
{
    sacred_number_t n = sacred_number_get(7);
    TEST_ASSERT_EQUAL_INT(7, n.value);
    TEST_ASSERT_TRUE(n.appearance_count >= 5);
}

void test_get_twelve(void)
{
    sacred_number_t n = sacred_number_get(12);
    TEST_ASSERT_EQUAL_INT(12, n.value);
    TEST_ASSERT_TRUE(n.appearance_count >= 5);
}

void test_get_sixtyfour(void)
{
    sacred_number_t n = sacred_number_get(64);
    TEST_ASSERT_EQUAL_INT(64, n.value);
    TEST_ASSERT_TRUE(n.appearance_count >= 3);
}

void test_get_260(void)
{
    sacred_number_t n = sacred_number_get(260);
    TEST_ASSERT_EQUAL_INT(260, n.value);
    TEST_ASSERT_TRUE(n.appearance_count >= 2);
}

void test_get_108(void)
{
    sacred_number_t n = sacred_number_get(108);
    TEST_ASSERT_EQUAL_INT(108, n.value);
    TEST_ASSERT_TRUE(n.appearance_count >= 3);
}

void test_get_not_found(void)
{
    sacred_number_t n = sacred_number_get(9999);
    TEST_ASSERT_EQUAL_INT(0, n.value);
    TEST_ASSERT_NULL(n.name);
    TEST_ASSERT_EQUAL_INT(0, n.appearance_count);
}

/* ===== sacred_number_by_index ===== */

void test_by_index_first(void)
{
    sacred_number_t n = sacred_number_by_index(0);
    TEST_ASSERT_EQUAL_INT(3, n.value);
}

void test_by_index_last(void)
{
    int count = sacred_number_count();
    sacred_number_t n = sacred_number_by_index(count - 1);
    TEST_ASSERT_EQUAL_INT(432, n.value);
}

void test_by_index_negative(void)
{
    sacred_number_t n = sacred_number_by_index(-1);
    TEST_ASSERT_EQUAL_INT(0, n.value);
    TEST_ASSERT_NULL(n.name);
}

void test_by_index_out_of_range(void)
{
    sacred_number_t n = sacred_number_by_index(1000);
    TEST_ASSERT_EQUAL_INT(0, n.value);
    TEST_ASSERT_NULL(n.name);
}

/* ===== sacred_number_system_count ===== */

void test_system_count_seven(void)
{
    /* 7 appears in >= 5 systems */
    TEST_ASSERT_TRUE(sacred_number_system_count(7) >= 5);
}

void test_system_count_not_found(void)
{
    TEST_ASSERT_EQUAL_INT(0, sacred_number_system_count(9999));
}

/* ===== sacred_number_in_system ===== */

void test_in_system_7_astronomy(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(7, SYSTEM_ASTRONOMY));
}

void test_in_system_7_hindu(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(7, SYSTEM_HINDU));
}

void test_in_system_64_iching(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(64, SYSTEM_ICHING));
}

void test_in_system_64_human_design(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(64, SYSTEM_HUMAN_DESIGN));
}

void test_in_system_64_biology(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(64, SYSTEM_BIOLOGY));
}

void test_in_system_260_tzolkin(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(260, SYSTEM_TZOLKIN));
}

void test_in_system_not_present(void)
{
    /* 260 is not in Kabbalah */
    TEST_ASSERT_FALSE(sacred_number_in_system(260, SYSTEM_KABBALAH));
}

void test_in_system_value_not_found(void)
{
    TEST_ASSERT_FALSE(sacred_number_in_system(9999, SYSTEM_ASTRONOMY));
}

/* ===== sacred_number_system_name ===== */

void test_system_name_astronomy(void)
{
    TEST_ASSERT_EQUAL_STRING("Astronomy", sacred_number_system_name(SYSTEM_ASTRONOMY));
}

void test_system_name_tzolkin(void)
{
    TEST_ASSERT_EQUAL_STRING("Tzolkin", sacred_number_system_name(SYSTEM_TZOLKIN));
}

void test_system_name_kabbalah(void)
{
    TEST_ASSERT_EQUAL_STRING("Kabbalah", sacred_number_system_name(SYSTEM_KABBALAH));
}

void test_system_name_biology(void)
{
    TEST_ASSERT_EQUAL_STRING("Biology", sacred_number_system_name(SYSTEM_BIOLOGY));
}

void test_system_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", sacred_number_system_name(SYSTEM_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", sacred_number_system_name((knowledge_system_t)-1));
}

/* ===== sacred_number_find_by_system ===== */

void test_find_by_system_tzolkin(void)
{
    int indices[32];
    int found = sacred_number_find_by_system(SYSTEM_TZOLKIN, indices, 32);
    /* Tzolkin should have at least 3 sacred numbers: 13, 20, 260 */
    TEST_ASSERT_TRUE(found >= 3);
}

void test_find_by_system_iching(void)
{
    int indices[32];
    int found = sacred_number_find_by_system(SYSTEM_ICHING, indices, 32);
    /* I Ching: at least 3, 8, 64 */
    TEST_ASSERT_TRUE(found >= 3);
}

void test_find_by_system_max_results_limit(void)
{
    int indices[2];
    int found = sacred_number_find_by_system(SYSTEM_ASTRONOMY, indices, 2);
    /* Should cap at max_results */
    TEST_ASSERT_TRUE(found <= 2);
}

/* ===== sacred_number_factor_of ===== */

void test_factor_of_exact_match(void)
{
    /* 64 is itself a sacred number */
    int result = sacred_number_factor_of(64);
    TEST_ASSERT_EQUAL_INT(64, result);
}

void test_factor_of_multiple_of_sacred(void)
{
    /* 9 is itself sacred (Ennead), so exact match returns 9. */
    int result = sacred_number_factor_of(9);
    TEST_ASSERT_EQUAL_INT(9, result);

    /* 6 = 2 * 3, and 3 is the smallest sacred factor of 6 */
    int result2 = sacred_number_factor_of(6);
    TEST_ASSERT_EQUAL_INT(3, result2);
}

void test_factor_of_factor_of_sacred(void)
{
    /* 2 divides 4 (sacred), so sacred_number_factor_of(2) = 4 */
    int result = sacred_number_factor_of(2);
    TEST_ASSERT_EQUAL_INT(4, result);
}

void test_factor_of_no_relation(void)
{
    /* A large prime not related to any sacred number */
    int result = sacred_number_factor_of(997);
    TEST_ASSERT_EQUAL_INT(0, result);
}

/* ===== Specific number data validation ===== */

void test_number_4_appearances(void)
{
    sacred_number_t n = sacred_number_get(4);
    TEST_ASSERT_EQUAL_INT(4, n.value);
    TEST_ASSERT_TRUE(n.appearance_count >= 3);
}

void test_number_22_kabbalah(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(22, SYSTEM_KABBALAH));
}

void test_number_22_hebrew(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(22, SYSTEM_HEBREW));
}

void test_number_19_astronomy(void)
{
    /* Metonic cycle */
    TEST_ASSERT_TRUE(sacred_number_in_system(19, SYSTEM_ASTRONOMY));
}

void test_number_360_mathematics(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(360, SYSTEM_MATHEMATICS));
}

void test_number_10_kabbalah(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(10, SYSTEM_KABBALAH));
}

void test_number_36_human_design(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(36, SYSTEM_HUMAN_DESIGN));
}

void test_number_72_astronomy(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(72, SYSTEM_ASTRONOMY));
}

void test_number_432_hindu(void)
{
    TEST_ASSERT_TRUE(sacred_number_in_system(432, SYSTEM_HINDU));
}

/* ===== Sorted order validation ===== */

void test_numbers_sorted_ascending(void)
{
    int count = sacred_number_count();
    for (int i = 1; i < count; i++) {
        sacred_number_t prev = sacred_number_by_index(i - 1);
        sacred_number_t curr = sacred_number_by_index(i);
        TEST_ASSERT_TRUE(prev.value < curr.value);
    }
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_count_at_least_20);

    /* Get by value */
    RUN_TEST(test_get_three);
    RUN_TEST(test_get_seven);
    RUN_TEST(test_get_twelve);
    RUN_TEST(test_get_sixtyfour);
    RUN_TEST(test_get_260);
    RUN_TEST(test_get_108);
    RUN_TEST(test_get_not_found);

    /* Get by index */
    RUN_TEST(test_by_index_first);
    RUN_TEST(test_by_index_last);
    RUN_TEST(test_by_index_negative);
    RUN_TEST(test_by_index_out_of_range);

    /* System count */
    RUN_TEST(test_system_count_seven);
    RUN_TEST(test_system_count_not_found);

    /* In system */
    RUN_TEST(test_in_system_7_astronomy);
    RUN_TEST(test_in_system_7_hindu);
    RUN_TEST(test_in_system_64_iching);
    RUN_TEST(test_in_system_64_human_design);
    RUN_TEST(test_in_system_64_biology);
    RUN_TEST(test_in_system_260_tzolkin);
    RUN_TEST(test_in_system_not_present);
    RUN_TEST(test_in_system_value_not_found);

    /* System name */
    RUN_TEST(test_system_name_astronomy);
    RUN_TEST(test_system_name_tzolkin);
    RUN_TEST(test_system_name_kabbalah);
    RUN_TEST(test_system_name_biology);
    RUN_TEST(test_system_name_invalid);

    /* Find by system */
    RUN_TEST(test_find_by_system_tzolkin);
    RUN_TEST(test_find_by_system_iching);
    RUN_TEST(test_find_by_system_max_results_limit);

    /* Factor of */
    RUN_TEST(test_factor_of_exact_match);
    RUN_TEST(test_factor_of_multiple_of_sacred);
    RUN_TEST(test_factor_of_factor_of_sacred);
    RUN_TEST(test_factor_of_no_relation);

    /* Data validation */
    RUN_TEST(test_number_4_appearances);
    RUN_TEST(test_number_22_kabbalah);
    RUN_TEST(test_number_22_hebrew);
    RUN_TEST(test_number_19_astronomy);
    RUN_TEST(test_number_360_mathematics);
    RUN_TEST(test_number_10_kabbalah);
    RUN_TEST(test_number_36_human_design);
    RUN_TEST(test_number_72_astronomy);
    RUN_TEST(test_number_432_hindu);

    /* Sorted order */
    RUN_TEST(test_numbers_sorted_ascending);

    return UNITY_END();
}
