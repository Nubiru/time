#include "../unity/unity.h"
#include "../../src/ui/chinese_fmt.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Reference years:
 * 2024 = Jia-Chen, Wood Dragon, Yang, cycle year 41
 * 1966 = Bing-Wu, Fire Horse, Yang, cycle year 43
 * 2000 = Geng-Chen, Metal Dragon, Yang, cycle year 17
 * 2023 = Gui-Mao, Water Rabbit, Yin, cycle year 40
 * 1900 = Geng-Zi, Metal Rat, Yang, cycle year 37
 */

#define JD_2024 2460310.5   /* ~2024-01-01 */
#define JD_2000 2451544.5   /* ~2000-01-01 */

/* Display from known years: 1-4 */

void test_display_2024_dragon(void)
{
    chinese_display_t d = chinese_display_from_year(2024);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Dragon"));
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Wood"));
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "Dragon"));
}

void test_display_1966_horse(void)
{
    chinese_display_t d = chinese_display_from_year(1966);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Horse"));
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Fire"));
}

void test_display_2000_dragon(void)
{
    chinese_display_t d = chinese_display_from_year(2000);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Dragon"));
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Metal"));
}

void test_display_2023_rabbit(void)
{
    chinese_display_t d = chinese_display_from_year(2023);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Rabbit"));
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Water"));
}

/* Title formatting: 5-6 */

void test_title_format(void)
{
    char buf[48];
    chinese_fmt_title(4, 0, buf, sizeof(buf));  /* Wood Dragon */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Dragon"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Wood"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Year"));
}

void test_title_rat(void)
{
    char buf[48];
    chinese_fmt_title(0, 3, buf, sizeof(buf));  /* Metal Rat */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Rat"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Metal"));
}

/* Stem-branch: 7-9 */

void test_stem_branch_bing_wu(void)
{
    char buf[32];
    chinese_fmt_stem_branch(2, 6, buf, sizeof(buf));  /* Bing-Wu */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Bing"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Wu"));
}

void test_stem_branch_jia_zi(void)
{
    char buf[32];
    chinese_fmt_stem_branch(0, 0, buf, sizeof(buf));  /* Jia-Zi */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Jia"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Zi"));
}

void test_stem_branch_gui_hai(void)
{
    char buf[32];
    chinese_fmt_stem_branch(9, 11, buf, sizeof(buf));  /* Gui-Hai */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Gui"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Hai"));
}

/* Element + polarity: 10-12 */

void test_element_fire_yang(void)
{
    char buf[32];
    chinese_fmt_element(1, 0, buf, sizeof(buf));  /* Fire Yang */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Fire"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Yang"));
}

void test_element_water_yin(void)
{
    char buf[32];
    chinese_fmt_element(4, 1, buf, sizeof(buf));  /* Water Yin */
    TEST_ASSERT_NOT_NULL(strstr(buf, "Water"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Yin"));
}

void test_element_wood_yang(void)
{
    char buf[32];
    chinese_fmt_element(0, 0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Wood"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Yang"));
}

/* Cycle position: 13-14 */

void test_cycle_year_1(void)
{
    char buf[32];
    chinese_fmt_cycle(1, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "1"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "60"));
}

void test_cycle_year_43(void)
{
    char buf[32];
    chinese_fmt_cycle(43, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "43"));
}

/* Animal line: 15-16 */

void test_animal_dragon(void)
{
    char buf[32];
    chinese_fmt_animal(4, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Dragon"));
}

void test_animal_pig(void)
{
    char buf[32];
    chinese_fmt_animal(11, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Pig"));
}

/* Summary: 17-18 */

void test_summary_has_animal(void)
{
    chinese_display_t d = chinese_display_from_year(2024);
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "Dragon"));
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "Wood"));
}

void test_summary_has_stem_branch(void)
{
    chinese_display_t d = chinese_display_from_year(1966);
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "Bing"));
    TEST_ASSERT_NOT_NULL(strstr(d.summary, "Wu"));
}

/* JD input: 19-20 */

void test_display_from_jd(void)
{
    chinese_display_t d = chinese_display_from_jd(JD_2024);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Dragon"));
    TEST_ASSERT_TRUE(strlen(d.summary) > 0);
}

void test_display_from_jd_2000(void)
{
    chinese_display_t d = chinese_display_from_jd(JD_2000);
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Dragon"));
    TEST_ASSERT_NOT_NULL(strstr(d.title, "Metal"));
}

/* All fields populated: 21 */

void test_all_fields_populated(void)
{
    chinese_display_t d = chinese_display_from_year(2024);
    TEST_ASSERT_TRUE(strlen(d.title) > 0);
    TEST_ASSERT_TRUE(strlen(d.stem_branch) > 0);
    TEST_ASSERT_TRUE(strlen(d.element_line) > 0);
    TEST_ASSERT_TRUE(strlen(d.cycle_line) > 0);
    TEST_ASSERT_TRUE(strlen(d.animal_line) > 0);
    TEST_ASSERT_TRUE(strlen(d.summary) > 0);
}

/* Purity: 22 */

void test_purity(void)
{
    chinese_display_t a = chinese_display_from_year(2024);
    chinese_display_t b = chinese_display_from_year(2024);
    TEST_ASSERT_EQUAL_STRING(a.title, b.title);
    TEST_ASSERT_EQUAL_STRING(a.summary, b.summary);
    TEST_ASSERT_EQUAL_STRING(a.stem_branch, b.stem_branch);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_display_2024_dragon);
    RUN_TEST(test_display_1966_horse);
    RUN_TEST(test_display_2000_dragon);
    RUN_TEST(test_display_2023_rabbit);
    RUN_TEST(test_title_format);
    RUN_TEST(test_title_rat);
    RUN_TEST(test_stem_branch_bing_wu);
    RUN_TEST(test_stem_branch_jia_zi);
    RUN_TEST(test_stem_branch_gui_hai);
    RUN_TEST(test_element_fire_yang);
    RUN_TEST(test_element_water_yin);
    RUN_TEST(test_element_wood_yang);
    RUN_TEST(test_cycle_year_1);
    RUN_TEST(test_cycle_year_43);
    RUN_TEST(test_animal_dragon);
    RUN_TEST(test_animal_pig);
    RUN_TEST(test_summary_has_animal);
    RUN_TEST(test_summary_has_stem_branch);
    RUN_TEST(test_display_from_jd);
    RUN_TEST(test_display_from_jd_2000);
    RUN_TEST(test_all_fields_populated);
    RUN_TEST(test_purity);
    return UNITY_END();
}
