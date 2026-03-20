/* test_kabbalah_today.c — Tests for Kabbalah "Today" page module.
 * TDD RED phase: tests written before implementation.
 *
 * Test date:
 *   JD 2460394.5 => (int)jd = 2460394
 *     sefirah_index = 2460394 % 10 = 4 (Gevurah / Strength)
 *     path_index    = 2460394 % 22 = 2 (Gimel / The High Priestess / Moon)
 *   Gevurah: pillar=Severity(2), triad=Moral(1), planet_index=3 (Mars)
 *   World for sefirah 4 = Yetzirah(2) — Formation
 *   Middle pillar: no (pillar=2)
 *   Path 2: from Keter(0) to Tiferet(5), letter=Gimel, tarot=High Priestess
 */

#include "../../unity/unity.h"
#include "../../../src/systems/kabbalah/kabbalah_today.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* === Known JD constant === */

static const double JD_TEST = 2460394.5;

/* ===== kt_today — basic fields ===== */

void test_today_has_data(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_section_count(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

void test_today_sefirah_index_in_range(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_TRUE(p.sefirah_index >= 0);
    TEST_ASSERT_TRUE(p.sefirah_index <= 9);
}

void test_today_path_index_in_range(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_TRUE(p.path_index >= 0);
    TEST_ASSERT_TRUE(p.path_index <= 21);
}

void test_today_sefirah_index_known(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_EQUAL_INT(4, p.sefirah_index);  /* Gevurah */
}

void test_today_path_index_known(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_EQUAL_INT(2, p.path_index);  /* Gimel / The High Priestess */
}

/* ===== page_title ===== */

void test_title_contains_kabbalah(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Kabbalah"));
}

void test_title_contains_sefirah_name(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Gevurah"));
}

void test_title_contains_meaning(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Strength"));
}

/* ===== sefirah_section ===== */

void test_sefirah_section_contains_sefirah(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.sefirah_section, "Sefirah:"));
}

void test_sefirah_section_contains_pillar(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.sefirah_section, "Pillar:"));
}

void test_sefirah_section_contains_triad(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.sefirah_section, "Triad:"));
}

void test_sefirah_section_contains_planet(void)
{
    kt_page_t p = kt_today(JD_TEST);
    /* Gevurah planet_index=3 => Mars */
    TEST_ASSERT_NOT_NULL(strstr(p.sefirah_section, "Planet:"));
    TEST_ASSERT_NOT_NULL(strstr(p.sefirah_section, "Mars"));
}

void test_sefirah_section_severity_pillar(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.sefirah_section, "Severity"));
}

void test_sefirah_section_moral_triad(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.sefirah_section, "Moral"));
}

/* ===== world_section ===== */

void test_world_section_contains_world(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.world_section, "World:"));
}

void test_world_section_yetzirah(void)
{
    kt_page_t p = kt_today(JD_TEST);
    /* Sefirah 4 (Gevurah) => world 2 (Yetzirah) */
    TEST_ASSERT_NOT_NULL(strstr(p.world_section, "Yetzirah"));
}

void test_world_section_contains_meaning(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.world_section, "Formation"));
}

/* ===== path_section ===== */

void test_path_section_contains_path_header(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.path_section, "Path of the Day:"));
}

void test_path_section_contains_hebrew_letter(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.path_section, "Hebrew Letter:"));
}

void test_path_section_contains_tarot(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.path_section, "Tarot:"));
}

void test_path_section_gimel_priestess(void)
{
    kt_page_t p = kt_today(JD_TEST);
    /* Path 2: Gimel, The High Priestess */
    TEST_ASSERT_NOT_NULL(strstr(p.path_section, "Gimel"));
    TEST_ASSERT_NOT_NULL(strstr(p.path_section, "The High Priestess"));
}

void test_path_section_contains_attribution(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.path_section, "Attribution:"));
}

void test_path_section_from_to(void)
{
    kt_page_t p = kt_today(JD_TEST);
    /* Path 2: from Keter(0) to Tiferet(5) */
    TEST_ASSERT_NOT_NULL(strstr(p.path_section, "Keter"));
    TEST_ASSERT_NOT_NULL(strstr(p.path_section, "Tiferet"));
}

/* ===== meditation_section ===== */

void test_meditation_section_contains_meditation(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.meditation_section, "Meditation:"));
}

void test_meditation_section_contains_question(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.meditation_section, "Question:"));
}

void test_meditation_section_contains_essence(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.meditation_section, "Essence:"));
}

/* ===== tree_section ===== */

void test_tree_section_contains_middle_pillar(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.tree_section, "Middle Pillar:"));
}

void test_tree_section_contains_position(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_NOT_NULL(strstr(p.tree_section, "Position:"));
}

void test_tree_section_gevurah_not_middle(void)
{
    kt_page_t p = kt_today(JD_TEST);
    /* Gevurah pillar=2, not middle */
    TEST_ASSERT_EQUAL_INT(0, p.is_middle_pillar);
    TEST_ASSERT_NOT_NULL(strstr(p.tree_section, "Middle Pillar: No"));
}

/* ===== is_middle_pillar field ===== */

void test_is_middle_pillar_zero_or_one(void)
{
    kt_page_t p = kt_today(JD_TEST);
    TEST_ASSERT_TRUE(p.is_middle_pillar == 0 || p.is_middle_pillar == 1);
}

/* ===== Cycling behavior ===== */

void test_different_jds_different_sefirah(void)
{
    kt_page_t p1 = kt_today(JD_TEST);
    kt_page_t p2 = kt_today(JD_TEST + 1.0);
    TEST_ASSERT_NOT_EQUAL(p1.sefirah_index, p2.sefirah_index);
}

void test_different_jds_different_path(void)
{
    kt_page_t p1 = kt_today(JD_TEST);
    kt_page_t p2 = kt_today(JD_TEST + 1.0);
    TEST_ASSERT_NOT_EQUAL(p1.path_index, p2.path_index);
}

void test_sefirah_cycles_every_10(void)
{
    /* JD and JD+10 should give the same sefirah */
    kt_page_t p1 = kt_today(JD_TEST);
    kt_page_t p2 = kt_today(JD_TEST + 10.0);
    TEST_ASSERT_EQUAL_INT(p1.sefirah_index, p2.sefirah_index);
}

void test_path_cycles_every_22(void)
{
    /* JD and JD+22 should give the same path */
    kt_page_t p1 = kt_today(JD_TEST);
    kt_page_t p2 = kt_today(JD_TEST + 22.0);
    TEST_ASSERT_EQUAL_INT(p1.path_index, p2.path_index);
}

/* ===== Middle pillar sefirah test ===== */

void test_middle_pillar_for_keter(void)
{
    /* Keter = index 0 => need JD where (int)jd % 10 == 0 */
    /* 2460390 % 10 = 0 */
    kt_page_t p = kt_today(2460390.5);
    TEST_ASSERT_EQUAL_INT(0, p.sefirah_index);
    TEST_ASSERT_EQUAL_INT(1, p.is_middle_pillar);
    TEST_ASSERT_NOT_NULL(strstr(p.tree_section, "Middle Pillar: Yes"));
    TEST_ASSERT_NOT_NULL(strstr(p.tree_section, "central axis"));
}

/* ===== Planet "None" test ===== */

void test_sefirah_with_no_planet(void)
{
    /* Tiferet = index 5, planet_index=-1 => need JD where (int)jd % 10 == 5 */
    /* 2460395 % 10 = 5 */
    kt_page_t p = kt_today(2460395.5);
    TEST_ASSERT_EQUAL_INT(5, p.sefirah_index);
    TEST_ASSERT_NOT_NULL(strstr(p.sefirah_section, "Planet: None"));
}

/* ===== kt_format ===== */

void test_format_produces_output(void)
{
    kt_page_t p = kt_today(JD_TEST);
    char buf[4096];
    int n = kt_format(&p, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_title(void)
{
    kt_page_t p = kt_today(JD_TEST);
    char buf[4096];
    kt_format(&p, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Kabbalah"));
}

void test_format_contains_all_sections(void)
{
    kt_page_t p = kt_today(JD_TEST);
    char buf[4096];
    kt_format(&p, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sefirah:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "World:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Path of the Day:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Meditation:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Middle Pillar:"));
}

void test_format_null_page_returns_zero(void)
{
    char buf[256];
    int n = kt_format(NULL, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_null_buf_returns_zero(void)
{
    kt_page_t p = kt_today(JD_TEST);
    int n = kt_format(&p, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_zero_buf_size_returns_zero(void)
{
    kt_page_t p = kt_today(JD_TEST);
    char buf[16];
    int n = kt_format(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_small_buf_truncates(void)
{
    kt_page_t p = kt_today(JD_TEST);
    char buf[32];
    int n = kt_format(&p, buf, sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(n < 32);
}

void test_format_no_data_returns_zero(void)
{
    kt_page_t p;
    memset(&p, 0, sizeof(p));
    p.has_data = 0;
    char buf[256];
    int n = kt_format(&p, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

int main(void)
{
    UNITY_BEGIN();

    /* Basic fields */
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_section_count);
    RUN_TEST(test_today_sefirah_index_in_range);
    RUN_TEST(test_today_path_index_in_range);
    RUN_TEST(test_today_sefirah_index_known);
    RUN_TEST(test_today_path_index_known);

    /* Page title */
    RUN_TEST(test_title_contains_kabbalah);
    RUN_TEST(test_title_contains_sefirah_name);
    RUN_TEST(test_title_contains_meaning);

    /* Sefirah section */
    RUN_TEST(test_sefirah_section_contains_sefirah);
    RUN_TEST(test_sefirah_section_contains_pillar);
    RUN_TEST(test_sefirah_section_contains_triad);
    RUN_TEST(test_sefirah_section_contains_planet);
    RUN_TEST(test_sefirah_section_severity_pillar);
    RUN_TEST(test_sefirah_section_moral_triad);

    /* World section */
    RUN_TEST(test_world_section_contains_world);
    RUN_TEST(test_world_section_yetzirah);
    RUN_TEST(test_world_section_contains_meaning);

    /* Path section */
    RUN_TEST(test_path_section_contains_path_header);
    RUN_TEST(test_path_section_contains_hebrew_letter);
    RUN_TEST(test_path_section_contains_tarot);
    RUN_TEST(test_path_section_gimel_priestess);
    RUN_TEST(test_path_section_contains_attribution);
    RUN_TEST(test_path_section_from_to);

    /* Meditation section */
    RUN_TEST(test_meditation_section_contains_meditation);
    RUN_TEST(test_meditation_section_contains_question);
    RUN_TEST(test_meditation_section_contains_essence);

    /* Tree section */
    RUN_TEST(test_tree_section_contains_middle_pillar);
    RUN_TEST(test_tree_section_contains_position);
    RUN_TEST(test_tree_section_gevurah_not_middle);

    /* is_middle_pillar */
    RUN_TEST(test_is_middle_pillar_zero_or_one);

    /* Cycling */
    RUN_TEST(test_different_jds_different_sefirah);
    RUN_TEST(test_different_jds_different_path);
    RUN_TEST(test_sefirah_cycles_every_10);
    RUN_TEST(test_path_cycles_every_22);

    /* Special cases */
    RUN_TEST(test_middle_pillar_for_keter);
    RUN_TEST(test_sefirah_with_no_planet);

    /* Format function */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_title);
    RUN_TEST(test_format_contains_all_sections);
    RUN_TEST(test_format_null_page_returns_zero);
    RUN_TEST(test_format_null_buf_returns_zero);
    RUN_TEST(test_format_zero_buf_size_returns_zero);
    RUN_TEST(test_format_small_buf_truncates);
    RUN_TEST(test_format_no_data_returns_zero);

    return UNITY_END();
}
