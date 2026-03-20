/* test_lao_interpret.c -- Lao calendar interpretation tests
 * TDD: tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/lao/lao_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Month data ===== */

void test_month_meesaa(void)
{
    li_lao_month_t m = lai_month_data(4);
    TEST_ASSERT_EQUAL_INT(4, m.month);
    TEST_ASSERT_EQUAL_STRING("Meesaa", m.lao);
    TEST_ASSERT_NOT_NULL(strstr(m.origin, "Aries"));
}

void test_month_all_12_valid(void)
{
    for (int i = 1; i <= 12; i++) {
        li_lao_month_t m = lai_month_data(i);
        TEST_ASSERT_EQUAL_INT(i, m.month);
        TEST_ASSERT_NOT_NULL(m.lao);
        TEST_ASSERT_NOT_NULL(m.origin);
        TEST_ASSERT_NOT_NULL(m.season);
        TEST_ASSERT_NOT_NULL(m.brief);
    }
}

void test_month_invalid_zero(void)
{
    li_lao_month_t m = lai_month_data(0);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
    TEST_ASSERT_EQUAL_STRING("?", m.lao);
}

void test_month_invalid_13(void)
{
    li_lao_month_t m = lai_month_data(13);
    TEST_ASSERT_EQUAL_INT(-1, m.month);
}

/* ===== Festival data ===== */

void test_festival_pi_mai(void)
{
    li_lao_festival_t f = lai_festival_data(1);
    TEST_ASSERT_EQUAL_INT(1, f.festival);
    TEST_ASSERT_NOT_NULL(strstr(f.name, "Pi Mai"));
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "Water"));
}

void test_festival_all_3_valid(void)
{
    for (int i = 1; i <= 3; i++) {
        li_lao_festival_t f = lai_festival_data(i);
        TEST_ASSERT_EQUAL_INT(i, f.festival);
        TEST_ASSERT_NOT_NULL(f.name);
        TEST_ASSERT_NOT_NULL(f.theme);
        TEST_ASSERT_NOT_NULL(f.practice);
        TEST_ASSERT_NOT_NULL(f.brief);
    }
}

void test_festival_invalid(void)
{
    li_lao_festival_t f = lai_festival_data(0);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

/* ===== Tiered interpretation ===== */

void test_interpret_glyph(void)
{
    lao_interp_t r = lai_interpret(2569, 4, 14, 1);
    TEST_ASSERT_EQUAL_STRING("Mee", r.glyph);
}

void test_interpret_glance_festival(void)
{
    lao_interp_t r = lai_interpret(2569, 4, 14, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "BE 2569"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Pi Mai"));
}

void test_interpret_glance_no_festival(void)
{
    lao_interp_t r = lai_interpret(2569, 7, 15, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Kawlakot"));
    TEST_ASSERT_NULL(strstr(r.glance, "Pi Mai"));
}

void test_interpret_detail_sanskrit(void)
{
    lao_interp_t r = lai_interpret(2569, 4, 14, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Sanskrit"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Aries"));
}

void test_interpret_detail_festival(void)
{
    lao_interp_t r = lai_interpret(2569, 4, 14, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Pi Mai"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Water"));
}

void test_interpret_invalid_month(void)
{
    lao_interp_t r = lai_interpret(2569, 0, 1, 0);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
}

/* ===== Counts ===== */

void test_month_count(void)
{
    TEST_ASSERT_EQUAL_INT(12, lai_month_count());
}

void test_festival_count(void)
{
    TEST_ASSERT_EQUAL_INT(3, lai_festival_count());
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_month_meesaa);
    RUN_TEST(test_month_all_12_valid);
    RUN_TEST(test_month_invalid_zero);
    RUN_TEST(test_month_invalid_13);

    RUN_TEST(test_festival_pi_mai);
    RUN_TEST(test_festival_all_3_valid);
    RUN_TEST(test_festival_invalid);

    RUN_TEST(test_interpret_glyph);
    RUN_TEST(test_interpret_glance_festival);
    RUN_TEST(test_interpret_glance_no_festival);
    RUN_TEST(test_interpret_detail_sanskrit);
    RUN_TEST(test_interpret_detail_festival);
    RUN_TEST(test_interpret_invalid_month);

    RUN_TEST(test_month_count);
    RUN_TEST(test_festival_count);

    return UNITY_END();
}
