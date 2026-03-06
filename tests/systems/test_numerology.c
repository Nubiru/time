#include "../unity/unity.h"
#include "../../src/systems/numerology/numerology.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Basic reduction: 1-5 */

void test_reduce_single_digit(void)
{
    num_root_t r = num_reduce(5);
    TEST_ASSERT_EQUAL_INT(5, r.root);
    TEST_ASSERT_EQUAL_INT(0, r.is_master);
}

void test_reduce_123(void)
{
    num_root_t r = num_reduce(123);
    TEST_ASSERT_EQUAL_INT(6, r.root); /* 1+2+3=6 */
    TEST_ASSERT_EQUAL_INT(0, r.is_master);
}

void test_reduce_99(void)
{
    num_root_t r = num_reduce(99);
    TEST_ASSERT_EQUAL_INT(9, r.root); /* 9+9=18, 1+8=9 */
}

void test_reduce_987(void)
{
    num_root_t r = num_reduce(987);
    TEST_ASSERT_EQUAL_INT(6, r.root); /* 9+8+7=24, 2+4=6 */
}

void test_reduce_zero(void)
{
    num_root_t r = num_reduce(0);
    TEST_ASSERT_EQUAL_INT(0, r.root);
    TEST_ASSERT_EQUAL_INT(0, r.is_master);
}

/* Master numbers: 6-10 */

void test_master_11(void)
{
    num_root_t r = num_reduce(11);
    TEST_ASSERT_EQUAL_INT(11, r.root);
    TEST_ASSERT_EQUAL_INT(1, r.is_master);
}

void test_master_22(void)
{
    num_root_t r = num_reduce(22);
    TEST_ASSERT_EQUAL_INT(22, r.root);
    TEST_ASSERT_EQUAL_INT(1, r.is_master);
}

void test_master_33(void)
{
    num_root_t r = num_reduce(33);
    TEST_ASSERT_EQUAL_INT(33, r.root);
    TEST_ASSERT_EQUAL_INT(1, r.is_master);
}

void test_reduce_29_to_master_11(void)
{
    num_root_t r = num_reduce(29);
    TEST_ASSERT_EQUAL_INT(11, r.root); /* 2+9=11, master */
    TEST_ASSERT_EQUAL_INT(1, r.is_master);
}

void test_44_not_master(void)
{
    num_root_t r = num_reduce(44);
    TEST_ASSERT_EQUAL_INT(8, r.root); /* 4+4=8 */
    TEST_ASSERT_EQUAL_INT(0, r.is_master);
}

/* Digit sum: 11-12 */

void test_digit_sum_123(void)
{
    TEST_ASSERT_EQUAL_INT(6, num_digit_sum(123));
}

void test_digit_sum_9999(void)
{
    TEST_ASSERT_EQUAL_INT(36, num_digit_sum(9999));
}

/* Life path: 13-15 */

void test_life_path_1990_5_15(void)
{
    /* year: 1+9+9+0=19 -> 1+9=10 -> 1+0=1
     * month: 5
     * day: 1+5=6
     * total: 1+5+6 = 12 -> 1+2 = 3 */
    num_root_t lp = num_life_path(1990, 5, 15);
    TEST_ASSERT_EQUAL_INT(3, lp.root);
}

void test_life_path_master_components(void)
{
    /* 1990, 11, 29:
     * year: 1+9+9+0=19 -> 1+9=10 -> 1+0=1
     * month: 11 -> master 11 (preserved)
     * day: 29 -> 2+9=11 -> master 11 (preserved)
     * total: 1+11+11 = 23 -> 2+3 = 5 */
    num_root_t lp = num_life_path(1990, 11, 29);
    TEST_ASSERT_EQUAL_INT(5, lp.root);
}

void test_life_path_2000_1_1(void)
{
    /* year: 2+0+0+0=2
     * month: 1
     * day: 1
     * total: 2+1+1 = 4 */
    num_root_t lp = num_life_path(2000, 1, 1);
    TEST_ASSERT_EQUAL_INT(4, lp.root);
}

/* Universal year: 16-17 */

void test_universal_year_2026(void)
{
    num_root_t uy = num_universal_year(2026);
    /* 2+0+2+6 = 10 -> 1+0 = 1 */
    TEST_ASSERT_EQUAL_INT(1, uy.root);
}

void test_universal_year_2025(void)
{
    num_root_t uy = num_universal_year(2025);
    /* 2+0+2+5 = 9 */
    TEST_ASSERT_EQUAL_INT(9, uy.root);
}

/* Personal year: 18 */

void test_personal_year_varies(void)
{
    num_root_t py1 = num_personal_year(2026, 1, 1);
    num_root_t py2 = num_personal_year(2026, 6, 15);
    /* Different birth dates should (usually) give different personal years */
    /* At minimum, the function should return valid roots */
    TEST_ASSERT_TRUE(py1.root >= 1 && py1.root <= 33);
    TEST_ASSERT_TRUE(py2.root >= 1 && py2.root <= 33);
}

/* Meanings: 19-21 */

void test_meaning_1(void)
{
    TEST_ASSERT_EQUAL_STRING("Leadership", num_meaning(1));
}

void test_meaning_11(void)
{
    TEST_ASSERT_EQUAL_STRING("Illumination", num_meaning(11));
}

void test_meaning_0_valid(void)
{
    const char *m = num_meaning(0);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_EQUAL_STRING("?", m);
}

/* Profile: 22 */

void test_profile_components(void)
{
    num_profile_t p = num_profile(1990, 5, 15);
    TEST_ASSERT_TRUE(p.life_path.root >= 1 && p.life_path.root <= 33);
    TEST_ASSERT_TRUE(p.birth_year.root >= 1 && p.birth_year.root <= 33);
    TEST_ASSERT_TRUE(p.birth_month.root >= 1 && p.birth_month.root <= 33);
    TEST_ASSERT_TRUE(p.birth_day.root >= 1 && p.birth_day.root <= 33);
    /* birth_month for May = 5 */
    TEST_ASSERT_EQUAL_INT(5, p.birth_month.root);
}

/* Negative input: 23 */

void test_negative_input(void)
{
    num_root_t r = num_reduce(-123);
    TEST_ASSERT_EQUAL_INT(6, r.root); /* abs(123) -> 1+2+3=6 */
}

/* Purity: 24 */

void test_purity(void)
{
    num_root_t a = num_reduce(456);
    num_root_t b = num_reduce(456);
    TEST_ASSERT_EQUAL_INT(a.root, b.root);
    TEST_ASSERT_EQUAL_INT(a.is_master, b.is_master);
}

/* Extra tests */

void test_element_descriptions(void)
{
    TEST_ASSERT_EQUAL_STRING("The Initiator", num_element(1));
    TEST_ASSERT_EQUAL_STRING("The Visionary", num_element(11));
    TEST_ASSERT_EQUAL_STRING("The Architect", num_element(22));
    TEST_ASSERT_EQUAL_STRING("The Healer", num_element(33));
    TEST_ASSERT_EQUAL_STRING("?", num_element(0));
}

void test_all_meanings_valid(void)
{
    for (int i = 1; i <= 9; i++) {
        const char *m = num_meaning(i);
        TEST_ASSERT_NOT_NULL(m);
        TEST_ASSERT_TRUE(strlen(m) > 0);
        TEST_ASSERT_TRUE(strcmp(m, "?") != 0);
    }
}

void test_personal_day(void)
{
    num_root_t pd = num_personal_day(2026, 5, 15, 3, 6);
    TEST_ASSERT_TRUE(pd.root >= 1 && pd.root <= 33);
}

void test_reduce_large_number(void)
{
    num_root_t r = num_reduce(999999);
    /* 9*6=54 -> 5+4=9 */
    TEST_ASSERT_EQUAL_INT(9, r.root);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_reduce_single_digit);
    RUN_TEST(test_reduce_123);
    RUN_TEST(test_reduce_99);
    RUN_TEST(test_reduce_987);
    RUN_TEST(test_reduce_zero);
    RUN_TEST(test_master_11);
    RUN_TEST(test_master_22);
    RUN_TEST(test_master_33);
    RUN_TEST(test_reduce_29_to_master_11);
    RUN_TEST(test_44_not_master);
    RUN_TEST(test_digit_sum_123);
    RUN_TEST(test_digit_sum_9999);
    RUN_TEST(test_life_path_1990_5_15);
    RUN_TEST(test_life_path_master_components);
    RUN_TEST(test_life_path_2000_1_1);
    RUN_TEST(test_universal_year_2026);
    RUN_TEST(test_universal_year_2025);
    RUN_TEST(test_personal_year_varies);
    RUN_TEST(test_meaning_1);
    RUN_TEST(test_meaning_11);
    RUN_TEST(test_meaning_0_valid);
    RUN_TEST(test_profile_components);
    RUN_TEST(test_negative_input);
    RUN_TEST(test_purity);
    RUN_TEST(test_element_descriptions);
    RUN_TEST(test_all_meanings_valid);
    RUN_TEST(test_personal_day);
    RUN_TEST(test_reduce_large_number);
    return UNITY_END();
}
