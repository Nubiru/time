/* test_convergence_interpret.c -- Convergence interpretation tests
 *
 * TDD: tests written before implementation.
 * Tests interpretive narratives for cross-system convergence events. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/convergence_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===================================================================
 * cvi_strength_count / cvi_system_count / cvi_pattern_count
 * =================================================================== */

void test_strength_count_returns_4(void)
{
    TEST_ASSERT_EQUAL_INT(4, cvi_strength_count());
}

void test_system_count_returns_21(void)
{
    TEST_ASSERT_EQUAL_INT(CD_SYS_COUNT, cvi_system_count());
    TEST_ASSERT_EQUAL_INT(21, cvi_system_count());
}

void test_pattern_count_returns_8(void)
{
    TEST_ASSERT_EQUAL_INT(8, cvi_pattern_count());
}

/* ===================================================================
 * cvi_strength_data — all 4 levels
 * =================================================================== */

void test_strength_data_minor(void)
{
    cvi_strength_t s = cvi_strength_data(CD_STRENGTH_MINOR);
    TEST_ASSERT_EQUAL_INT(CD_STRENGTH_MINOR, s.strength);
    TEST_ASSERT_EQUAL_STRING("Minor Alignment", s.name);
    TEST_ASSERT_NOT_NULL(s.description);
    TEST_ASSERT_TRUE(strlen(s.description) > 0);
    TEST_ASSERT_NOT_NULL(s.significance);
    TEST_ASSERT_TRUE(strlen(s.significance) > 0);
}

void test_strength_data_notable(void)
{
    cvi_strength_t s = cvi_strength_data(CD_STRENGTH_NOTABLE);
    TEST_ASSERT_EQUAL_INT(CD_STRENGTH_NOTABLE, s.strength);
    TEST_ASSERT_EQUAL_STRING("Notable Convergence", s.name);
    TEST_ASSERT_NOT_NULL(s.description);
    TEST_ASSERT_TRUE(strlen(s.description) > 0);
    TEST_ASSERT_NOT_NULL(s.significance);
}

void test_strength_data_major(void)
{
    cvi_strength_t s = cvi_strength_data(CD_STRENGTH_MAJOR);
    TEST_ASSERT_EQUAL_INT(CD_STRENGTH_MAJOR, s.strength);
    TEST_ASSERT_EQUAL_STRING("Major Convergence", s.name);
    TEST_ASSERT_NOT_NULL(s.description);
    TEST_ASSERT_NOT_NULL(s.significance);
}

void test_strength_data_rare(void)
{
    cvi_strength_t s = cvi_strength_data(CD_STRENGTH_RARE);
    TEST_ASSERT_EQUAL_INT(CD_STRENGTH_RARE, s.strength);
    TEST_ASSERT_EQUAL_STRING("Rare Convergence", s.name);
    TEST_ASSERT_NOT_NULL(s.description);
    TEST_ASSERT_NOT_NULL(s.significance);
}

void test_strength_data_invalid_zero(void)
{
    cvi_strength_t s = cvi_strength_data(0);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

void test_strength_data_invalid_five(void)
{
    cvi_strength_t s = cvi_strength_data(5);
    TEST_ASSERT_EQUAL_STRING("?", s.name);
}

/* ===================================================================
 * cvi_system_data — spot checks
 * =================================================================== */

void test_system_data_astronomy(void)
{
    cvi_system_t s = cvi_system_data(CD_SYS_ASTRONOMY);
    TEST_ASSERT_EQUAL_INT(CD_SYS_ASTRONOMY, s.system);
    TEST_ASSERT_EQUAL_STRING("Astronomy", s.system_name);
    TEST_ASSERT_EQUAL_STRING("Scientific observation", s.tradition);
    TEST_ASSERT_NOT_NULL(s.what_aligns);
    TEST_ASSERT_TRUE(strlen(s.what_aligns) > 0);
}

void test_system_data_hebrew(void)
{
    cvi_system_t s = cvi_system_data(CD_SYS_HEBREW);
    TEST_ASSERT_EQUAL_INT(CD_SYS_HEBREW, s.system);
    TEST_ASSERT_EQUAL_STRING("Hebrew Calendar", s.system_name);
    TEST_ASSERT_EQUAL_STRING("Jewish", s.tradition);
    TEST_ASSERT_NOT_NULL(s.what_aligns);
}

void test_system_data_buddhist(void)
{
    cvi_system_t s = cvi_system_data(CD_SYS_BUDDHIST);
    TEST_ASSERT_EQUAL_INT(CD_SYS_BUDDHIST, s.system);
    TEST_ASSERT_EQUAL_STRING("Buddhist Calendar", s.system_name);
    TEST_ASSERT_EQUAL_STRING("Theravada", s.tradition);
    TEST_ASSERT_NOT_NULL(s.what_aligns);
}

void test_system_data_zoroastrian(void)
{
    cvi_system_t s = cvi_system_data(CD_SYS_ZOROASTRIAN);
    TEST_ASSERT_EQUAL_INT(CD_SYS_ZOROASTRIAN, s.system);
    TEST_ASSERT_EQUAL_STRING("Zoroastrian Calendar", s.system_name);
    TEST_ASSERT_EQUAL_STRING("Mazdayasna", s.tradition);
    TEST_ASSERT_NOT_NULL(s.what_aligns);
}

void test_system_data_all_21_non_empty(void)
{
    for (int i = 0; i < CD_SYS_COUNT; i++) {
        cvi_system_t s = cvi_system_data((cd_system_t)i);
        TEST_ASSERT_NOT_NULL(s.system_name);
        TEST_ASSERT_TRUE(strlen(s.system_name) > 0);
        TEST_ASSERT_NOT_NULL(s.tradition);
        TEST_ASSERT_TRUE(strlen(s.tradition) > 0);
        TEST_ASSERT_NOT_NULL(s.what_aligns);
        TEST_ASSERT_TRUE(strlen(s.what_aligns) > 0);
    }
}

void test_system_data_invalid_21(void)
{
    cvi_system_t s = cvi_system_data(21);
    TEST_ASSERT_EQUAL_STRING("?", s.system_name);
}

void test_system_data_invalid_negative(void)
{
    cvi_system_t s = cvi_system_data((cd_system_t)-1);
    TEST_ASSERT_EQUAL_STRING("?", s.system_name);
}

/* ===================================================================
 * cvi_pattern_data — spot checks
 * =================================================================== */

void test_pattern_data_index_0(void)
{
    cvi_pattern_t p = cvi_pattern_data(0);
    TEST_ASSERT_EQUAL_STRING("Lunar Triple", p.name);
    TEST_ASSERT_NOT_NULL(p.description);
    TEST_ASSERT_TRUE(strlen(p.description) > 0);
    /* Systems: HEBREW, ISLAMIC, CHINESE */
    TEST_ASSERT_EQUAL_INT(CD_SYS_HEBREW, p.systems[0]);
    TEST_ASSERT_EQUAL_INT(CD_SYS_ISLAMIC, p.systems[1]);
    TEST_ASSERT_EQUAL_INT(CD_SYS_CHINESE, p.systems[2]);
    TEST_ASSERT_EQUAL_INT(-1, p.systems[3]);
}

void test_pattern_data_index_3(void)
{
    cvi_pattern_t p = cvi_pattern_data(3);
    TEST_ASSERT_EQUAL_STRING("New Year Cluster", p.name);
    TEST_ASSERT_NOT_NULL(p.description);
    /* Systems: PERSIAN, BAHAI, CHINESE */
    TEST_ASSERT_EQUAL_INT(CD_SYS_PERSIAN, p.systems[0]);
    TEST_ASSERT_EQUAL_INT(CD_SYS_BAHAI, p.systems[1]);
    TEST_ASSERT_EQUAL_INT(CD_SYS_CHINESE, p.systems[2]);
    TEST_ASSERT_EQUAL_INT(-1, p.systems[3]);
}

void test_pattern_data_index_7(void)
{
    cvi_pattern_t p = cvi_pattern_data(7);
    TEST_ASSERT_EQUAL_STRING("Universal Renewal", p.name);
    TEST_ASSERT_NOT_NULL(p.description);
    /* Systems: HEBREW, ISLAMIC, CHINESE, PERSIAN */
    TEST_ASSERT_EQUAL_INT(CD_SYS_HEBREW, p.systems[0]);
    TEST_ASSERT_EQUAL_INT(CD_SYS_ISLAMIC, p.systems[1]);
    TEST_ASSERT_EQUAL_INT(CD_SYS_CHINESE, p.systems[2]);
    TEST_ASSERT_EQUAL_INT(CD_SYS_PERSIAN, p.systems[3]);
    TEST_ASSERT_EQUAL_INT(-1, p.systems[4]);
}

void test_pattern_data_all_8_non_empty(void)
{
    for (int i = 0; i < 8; i++) {
        cvi_pattern_t p = cvi_pattern_data(i);
        TEST_ASSERT_NOT_NULL(p.name);
        TEST_ASSERT_TRUE(strlen(p.name) > 0);
        TEST_ASSERT_NOT_NULL(p.description);
        TEST_ASSERT_TRUE(strlen(p.description) > 0);
    }
}

void test_pattern_data_invalid_8(void)
{
    cvi_pattern_t p = cvi_pattern_data(8);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
}

void test_pattern_data_invalid_negative(void)
{
    cvi_pattern_t p = cvi_pattern_data(-1);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
}

/* ===================================================================
 * cvi_interpret — glyph, glance, detail
 * =================================================================== */

void test_interpret_minor_2_systems(void)
{
    cd_system_t sys[] = { CD_SYS_ASTRONOMY, CD_SYS_CELTIC };
    convergence_interp_t r = cvi_interpret(CD_STRENGTH_MINOR, sys, 2);

    TEST_ASSERT_EQUAL_STRING("*", r.glyph);

    /* Glance: "Minor Alignment -- 2 traditions align" */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Minor Alignment"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "2 traditions align"));

    /* Detail should contain strength description and system names */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Astronomy"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Celtic Wheel"));
}

void test_interpret_notable_3_systems(void)
{
    cd_system_t sys[] = { CD_SYS_HEBREW, CD_SYS_ISLAMIC, CD_SYS_CHINESE };
    convergence_interp_t r = cvi_interpret(CD_STRENGTH_NOTABLE, sys, 3);

    TEST_ASSERT_EQUAL_STRING("**", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Notable Convergence"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "3 traditions align"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Hebrew Calendar"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Islamic Calendar"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Chinese Calendar"));
}

void test_interpret_major_4_systems(void)
{
    cd_system_t sys[] = { CD_SYS_ASTRONOMY, CD_SYS_CELTIC, CD_SYS_PERSIAN,
                          CD_SYS_BAHAI };
    convergence_interp_t r = cvi_interpret(CD_STRENGTH_MAJOR, sys, 4);

    TEST_ASSERT_EQUAL_STRING("***", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Major Convergence"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "4 traditions align"));
}

void test_interpret_rare_5_systems(void)
{
    cd_system_t sys[] = { CD_SYS_ASTRONOMY, CD_SYS_HEBREW, CD_SYS_ISLAMIC,
                          CD_SYS_BUDDHIST, CD_SYS_HINDU };
    convergence_interp_t r = cvi_interpret(CD_STRENGTH_RARE, sys, 5);

    TEST_ASSERT_EQUAL_STRING("****", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Rare Convergence"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "5 traditions align"));

    /* Detail should list all 5 systems */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Astronomy"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Hebrew Calendar"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Islamic Calendar"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Buddhist Calendar"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Hindu Calendar"));
}

void test_interpret_null_systems(void)
{
    convergence_interp_t r = cvi_interpret(CD_STRENGTH_MINOR, NULL, 0);

    /* Should produce minimal but valid output */
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

void test_interpret_count_zero(void)
{
    cd_system_t sys[] = { CD_SYS_ASTRONOMY };
    convergence_interp_t r = cvi_interpret(CD_STRENGTH_NOTABLE, sys, 0);

    /* Should produce minimal but valid output */
    TEST_ASSERT_TRUE(strlen(r.glyph) > 0);
    TEST_ASSERT_TRUE(strlen(r.glance) > 0);
    TEST_ASSERT_TRUE(strlen(r.detail) > 0);
}

void test_interpret_detail_contains_significance(void)
{
    cd_system_t sys[] = { CD_SYS_ASTRONOMY, CD_SYS_CELTIC };
    convergence_interp_t r = cvi_interpret(CD_STRENGTH_MINOR, sys, 2);

    /* Detail should end with significance text from strength data */
    cvi_strength_t st = cvi_strength_data(CD_STRENGTH_MINOR);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, st.significance));
}

void test_interpret_detail_contains_what_aligns(void)
{
    cd_system_t sys[] = { CD_SYS_ASTRONOMY, CD_SYS_HEBREW };
    convergence_interp_t r = cvi_interpret(CD_STRENGTH_MINOR, sys, 2);

    cvi_system_t s0 = cvi_system_data(CD_SYS_ASTRONOMY);
    cvi_system_t s1 = cvi_system_data(CD_SYS_HEBREW);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, s0.what_aligns));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, s1.what_aligns));
}

/* ===================================================================
 * main
 * =================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Count functions */
    RUN_TEST(test_strength_count_returns_4);
    RUN_TEST(test_system_count_returns_21);
    RUN_TEST(test_pattern_count_returns_8);

    /* Strength data */
    RUN_TEST(test_strength_data_minor);
    RUN_TEST(test_strength_data_notable);
    RUN_TEST(test_strength_data_major);
    RUN_TEST(test_strength_data_rare);
    RUN_TEST(test_strength_data_invalid_zero);
    RUN_TEST(test_strength_data_invalid_five);

    /* System data */
    RUN_TEST(test_system_data_astronomy);
    RUN_TEST(test_system_data_hebrew);
    RUN_TEST(test_system_data_buddhist);
    RUN_TEST(test_system_data_zoroastrian);
    RUN_TEST(test_system_data_all_21_non_empty);
    RUN_TEST(test_system_data_invalid_21);
    RUN_TEST(test_system_data_invalid_negative);

    /* Pattern data */
    RUN_TEST(test_pattern_data_index_0);
    RUN_TEST(test_pattern_data_index_3);
    RUN_TEST(test_pattern_data_index_7);
    RUN_TEST(test_pattern_data_all_8_non_empty);
    RUN_TEST(test_pattern_data_invalid_8);
    RUN_TEST(test_pattern_data_invalid_negative);

    /* Interpret */
    RUN_TEST(test_interpret_minor_2_systems);
    RUN_TEST(test_interpret_notable_3_systems);
    RUN_TEST(test_interpret_major_4_systems);
    RUN_TEST(test_interpret_rare_5_systems);
    RUN_TEST(test_interpret_null_systems);
    RUN_TEST(test_interpret_count_zero);
    RUN_TEST(test_interpret_detail_contains_significance);
    RUN_TEST(test_interpret_detail_contains_what_aligns);

    return UNITY_END();
}
