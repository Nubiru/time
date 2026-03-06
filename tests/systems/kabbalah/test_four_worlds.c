/* test_four_worlds.c — Kabbalah Four Worlds data module tests
 * TDD RED phase: tests written before implementation.
 * Source: Traditional Kabbalistic Four Worlds (Olamot),
 *         mapped to camera scale levels for visual rendering. */

#include "../../unity/unity.h"
#include "../../../src/systems/kabbalah/four_worlds.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Constants ===== */

void test_worlds_count(void)
{
    TEST_ASSERT_EQUAL_INT(4, FOUR_WORLDS_COUNT);
}

/* ===== Atzilut (0) — Emanation ===== */

void test_atzilut_name(void)
{
    kabbalistic_world_t w = four_worlds_get(0);
    TEST_ASSERT_EQUAL_STRING("Atzilut", w.name);
}

void test_atzilut_meaning(void)
{
    kabbalistic_world_t w = four_worlds_get(0);
    TEST_ASSERT_EQUAL_STRING("Emanation", w.meaning);
}

void test_atzilut_scale(void)
{
    kabbalistic_world_t w = four_worlds_get(0);
    TEST_ASSERT_EQUAL_INT(6, w.scale_min);
    TEST_ASSERT_EQUAL_INT(6, w.scale_max);
}

/* ===== Beriah (1) — Creation ===== */

void test_beriah_name(void)
{
    kabbalistic_world_t w = four_worlds_get(1);
    TEST_ASSERT_EQUAL_STRING("Beriah", w.name);
}

void test_beriah_scale(void)
{
    kabbalistic_world_t w = four_worlds_get(1);
    TEST_ASSERT_EQUAL_INT(4, w.scale_min);
    TEST_ASSERT_EQUAL_INT(5, w.scale_max);
}

/* ===== Yetzirah (2) — Formation ===== */

void test_yetzirah_name(void)
{
    kabbalistic_world_t w = four_worlds_get(2);
    TEST_ASSERT_EQUAL_STRING("Yetzirah", w.name);
}

void test_yetzirah_scale(void)
{
    kabbalistic_world_t w = four_worlds_get(2);
    TEST_ASSERT_EQUAL_INT(2, w.scale_min);
    TEST_ASSERT_EQUAL_INT(3, w.scale_max);
}

/* ===== Assiah (3) — Action ===== */

void test_assiah_name(void)
{
    kabbalistic_world_t w = four_worlds_get(3);
    TEST_ASSERT_EQUAL_STRING("Assiah", w.name);
}

void test_assiah_scale(void)
{
    kabbalistic_world_t w = four_worlds_get(3);
    TEST_ASSERT_EQUAL_INT(0, w.scale_min);
    TEST_ASSERT_EQUAL_INT(1, w.scale_max);
}

/* ===== Scale-to-World mapping ===== */

void test_at_scale_0(void)
{
    /* Personal -> Assiah */
    kabbalistic_world_t w = four_worlds_at_scale(0);
    TEST_ASSERT_EQUAL_STRING("Assiah", w.name);
    TEST_ASSERT_EQUAL_INT(3, w.id);
}

void test_at_scale_1(void)
{
    /* Earth-Moon -> Assiah */
    kabbalistic_world_t w = four_worlds_at_scale(1);
    TEST_ASSERT_EQUAL_STRING("Assiah", w.name);
}

void test_at_scale_2(void)
{
    /* Inner Solar System -> Yetzirah */
    kabbalistic_world_t w = four_worlds_at_scale(2);
    TEST_ASSERT_EQUAL_STRING("Yetzirah", w.name);
    TEST_ASSERT_EQUAL_INT(2, w.id);
}

void test_at_scale_3(void)
{
    /* Solar System -> Yetzirah */
    kabbalistic_world_t w = four_worlds_at_scale(3);
    TEST_ASSERT_EQUAL_STRING("Yetzirah", w.name);
}

void test_at_scale_4(void)
{
    /* Solar Neighborhood -> Beriah */
    kabbalistic_world_t w = four_worlds_at_scale(4);
    TEST_ASSERT_EQUAL_STRING("Beriah", w.name);
    TEST_ASSERT_EQUAL_INT(1, w.id);
}

void test_at_scale_5(void)
{
    /* Milky Way -> Beriah */
    kabbalistic_world_t w = four_worlds_at_scale(5);
    TEST_ASSERT_EQUAL_STRING("Beriah", w.name);
}

void test_at_scale_6(void)
{
    /* Observable Universe -> Atzilut */
    kabbalistic_world_t w = four_worlds_at_scale(6);
    TEST_ASSERT_EQUAL_STRING("Atzilut", w.name);
    TEST_ASSERT_EQUAL_INT(0, w.id);
}

/* ===== Sefirah containment ===== */

void test_contains_sefirah_keter_atzilut(void)
{
    /* Keter (0) belongs to Atzilut (0) */
    TEST_ASSERT_TRUE(four_worlds_contains_sefirah(0, 0));
}

void test_contains_sefirah_keter_beriah(void)
{
    /* Keter (0) does NOT belong to Beriah (1) */
    TEST_ASSERT_FALSE(four_worlds_contains_sefirah(1, 0));
}

void test_contains_sefirah_malkuth_assiah(void)
{
    /* Malkuth (9) belongs to Assiah (3) */
    TEST_ASSERT_TRUE(four_worlds_contains_sefirah(3, 9));
}

void test_contains_sefirah_chesed_yetzirah(void)
{
    /* Chesed (3) belongs to Yetzirah (2) */
    TEST_ASSERT_TRUE(four_worlds_contains_sefirah(2, 3));
}

void test_contains_sefirah_chokmah_beriah(void)
{
    /* Chokmah (1) belongs to Beriah (1) */
    TEST_ASSERT_TRUE(four_worlds_contains_sefirah(1, 1));
}

void test_contains_sefirah_binah_beriah(void)
{
    /* Binah (2) belongs to Beriah (1) */
    TEST_ASSERT_TRUE(four_worlds_contains_sefirah(1, 2));
}

void test_contains_sefirah_yesod_yetzirah(void)
{
    /* Yesod (8) belongs to Yetzirah (2) */
    TEST_ASSERT_TRUE(four_worlds_contains_sefirah(2, 8));
}

void test_contains_sefirah_malkuth_yetzirah(void)
{
    /* Malkuth (9) does NOT belong to Yetzirah (2) */
    TEST_ASSERT_FALSE(four_worlds_contains_sefirah(2, 9));
}

/* ===== Sefirah-to-World reverse lookup ===== */

void test_for_sefirah_keter(void)
{
    /* Keter (0) -> Atzilut (0) */
    TEST_ASSERT_EQUAL_INT(0, four_worlds_for_sefirah(0));
}

void test_for_sefirah_chokmah(void)
{
    /* Chokmah (1) -> Beriah (1) */
    TEST_ASSERT_EQUAL_INT(1, four_worlds_for_sefirah(1));
}

void test_for_sefirah_binah(void)
{
    /* Binah (2) -> Beriah (1) */
    TEST_ASSERT_EQUAL_INT(1, four_worlds_for_sefirah(2));
}

void test_for_sefirah_chesed(void)
{
    /* Chesed (3) -> Yetzirah (2) */
    TEST_ASSERT_EQUAL_INT(2, four_worlds_for_sefirah(3));
}

void test_for_sefirah_gevurah(void)
{
    /* Gevurah (4) -> Yetzirah (2) */
    TEST_ASSERT_EQUAL_INT(2, four_worlds_for_sefirah(4));
}

void test_for_sefirah_yesod(void)
{
    /* Yesod (8) -> Yetzirah (2) */
    TEST_ASSERT_EQUAL_INT(2, four_worlds_for_sefirah(8));
}

void test_for_sefirah_malkuth(void)
{
    /* Malkuth (9) -> Assiah (3) */
    TEST_ASSERT_EQUAL_INT(3, four_worlds_for_sefirah(9));
}

/* ===== Sefirot count per world ===== */

void test_sefirot_count_atzilut(void)
{
    /* Atzilut: only Keter (1) */
    TEST_ASSERT_EQUAL_INT(1, four_worlds_sefirot_count(0));
}

void test_sefirot_count_beriah(void)
{
    /* Beriah: Chokmah + Binah (2) */
    TEST_ASSERT_EQUAL_INT(2, four_worlds_sefirot_count(1));
}

void test_sefirot_count_yetzirah(void)
{
    /* Yetzirah: Chesed through Yesod (6) */
    TEST_ASSERT_EQUAL_INT(6, four_worlds_sefirot_count(2));
}

void test_sefirot_count_assiah(void)
{
    /* Assiah: only Malkuth (1) */
    TEST_ASSERT_EQUAL_INT(1, four_worlds_sefirot_count(3));
}

/* ===== Name/meaning accessor functions ===== */

void test_name_atzilut(void)
{
    TEST_ASSERT_EQUAL_STRING("Atzilut", four_worlds_name(0));
}

void test_name_beriah(void)
{
    TEST_ASSERT_EQUAL_STRING("Beriah", four_worlds_name(1));
}

void test_meaning_atzilut(void)
{
    TEST_ASSERT_EQUAL_STRING("Emanation", four_worlds_meaning(0));
}

void test_meaning_assiah(void)
{
    TEST_ASSERT_EQUAL_STRING("Action", four_worlds_meaning(3));
}

/* ===== Invalid/boundary inputs ===== */

void test_invalid_world_index(void)
{
    kabbalistic_world_t w = four_worlds_get(10);
    TEST_ASSERT_NULL(w.name);
    TEST_ASSERT_NULL(w.meaning);
    TEST_ASSERT_NULL(w.description);
    TEST_ASSERT_EQUAL_INT(0, w.id);
}

void test_invalid_world_negative(void)
{
    kabbalistic_world_t w = four_worlds_get(-1);
    TEST_ASSERT_NULL(w.name);
}

void test_invalid_scale_high(void)
{
    /* Scale 7 is out of range; should clamp to Atzilut (highest) */
    kabbalistic_world_t w = four_worlds_at_scale(7);
    TEST_ASSERT_EQUAL_STRING("Atzilut", w.name);
}

void test_invalid_scale_negative(void)
{
    /* Scale -1 is out of range; should clamp to Assiah (lowest) */
    kabbalistic_world_t w = four_worlds_at_scale(-1);
    TEST_ASSERT_EQUAL_STRING("Assiah", w.name);
}

void test_invalid_sefirah(void)
{
    TEST_ASSERT_EQUAL_INT(-1, four_worlds_for_sefirah(10));
}

void test_invalid_sefirah_negative(void)
{
    TEST_ASSERT_EQUAL_INT(-1, four_worlds_for_sefirah(-1));
}

void test_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", four_worlds_name(-1));
}

void test_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", four_worlds_name(4));
}

void test_meaning_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", four_worlds_meaning(-1));
}

void test_meaning_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", four_worlds_meaning(4));
}

void test_sefirot_count_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, four_worlds_sefirot_count(-1));
}

void test_sefirot_count_invalid_high(void)
{
    TEST_ASSERT_EQUAL_INT(0, four_worlds_sefirot_count(4));
}

void test_contains_sefirah_invalid_world(void)
{
    TEST_ASSERT_FALSE(four_worlds_contains_sefirah(5, 0));
}

void test_contains_sefirah_invalid_sefirah(void)
{
    TEST_ASSERT_FALSE(four_worlds_contains_sefirah(0, 15));
}

/* ===== Completeness checks ===== */

void test_all_worlds_have_names(void)
{
    for (int i = 0; i < FOUR_WORLDS_COUNT; i++) {
        kabbalistic_world_t w = four_worlds_get(i);
        TEST_ASSERT_NOT_NULL(w.name);
    }
}

void test_all_worlds_have_meanings(void)
{
    for (int i = 0; i < FOUR_WORLDS_COUNT; i++) {
        kabbalistic_world_t w = four_worlds_get(i);
        TEST_ASSERT_NOT_NULL(w.meaning);
    }
}

void test_descriptions_non_null(void)
{
    for (int i = 0; i < FOUR_WORLDS_COUNT; i++) {
        kabbalistic_world_t w = four_worlds_get(i);
        TEST_ASSERT_NOT_NULL(w.description);
    }
}

void test_all_worlds_have_ids(void)
{
    for (int i = 0; i < FOUR_WORLDS_COUNT; i++) {
        kabbalistic_world_t w = four_worlds_get(i);
        TEST_ASSERT_EQUAL_INT(i, w.id);
    }
}

void test_colors_nonzero(void)
{
    /* All worlds should have at least some color channels > 0 */
    for (int i = 0; i < FOUR_WORLDS_COUNT; i++) {
        kabbalistic_world_t w = four_worlds_get(i);
        int has_color = (w.color.r > 0.0f || w.color.g > 0.0f ||
                         w.color.b > 0.0f);
        TEST_ASSERT_TRUE(has_color);
    }
}

/* ===== Color spot checks ===== */

void test_atzilut_color_gold_white(void)
{
    kabbalistic_world_t w = four_worlds_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, w.color.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.95f, w.color.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.7f, w.color.b);
}

void test_beriah_color_blue(void)
{
    kabbalistic_world_t w = four_worlds_get(1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.3f, w.color.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.4f, w.color.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.8f, w.color.b);
}

/* ===== Scale range consistency ===== */

void test_scale_ranges_valid(void)
{
    /* Each world's scale_min <= scale_max */
    for (int i = 0; i < FOUR_WORLDS_COUNT; i++) {
        kabbalistic_world_t w = four_worlds_get(i);
        TEST_ASSERT_TRUE(w.scale_min <= w.scale_max);
    }
}

void test_sefirot_ranges_valid(void)
{
    /* Each world's sefirot_start <= sefirot_end */
    for (int i = 0; i < FOUR_WORLDS_COUNT; i++) {
        kabbalistic_world_t w = four_worlds_get(i);
        TEST_ASSERT_TRUE(w.sefirot_start <= w.sefirot_end);
    }
}

void test_total_sefirot_coverage(void)
{
    /* Sum of all sefirot counts should be 10 (1+2+6+1) */
    int total = 0;
    for (int i = 0; i < FOUR_WORLDS_COUNT; i++) {
        total += four_worlds_sefirot_count(i);
    }
    TEST_ASSERT_EQUAL_INT(10, total);
}

int main(void)
{
    UNITY_BEGIN();

    /* Constants (1) */
    RUN_TEST(test_worlds_count);

    /* Atzilut (3) */
    RUN_TEST(test_atzilut_name);
    RUN_TEST(test_atzilut_meaning);
    RUN_TEST(test_atzilut_scale);

    /* Beriah (2) */
    RUN_TEST(test_beriah_name);
    RUN_TEST(test_beriah_scale);

    /* Yetzirah (2) */
    RUN_TEST(test_yetzirah_name);
    RUN_TEST(test_yetzirah_scale);

    /* Assiah (2) */
    RUN_TEST(test_assiah_name);
    RUN_TEST(test_assiah_scale);

    /* Scale-to-World (7) */
    RUN_TEST(test_at_scale_0);
    RUN_TEST(test_at_scale_1);
    RUN_TEST(test_at_scale_2);
    RUN_TEST(test_at_scale_3);
    RUN_TEST(test_at_scale_4);
    RUN_TEST(test_at_scale_5);
    RUN_TEST(test_at_scale_6);

    /* Sefirah containment (8) */
    RUN_TEST(test_contains_sefirah_keter_atzilut);
    RUN_TEST(test_contains_sefirah_keter_beriah);
    RUN_TEST(test_contains_sefirah_malkuth_assiah);
    RUN_TEST(test_contains_sefirah_chesed_yetzirah);
    RUN_TEST(test_contains_sefirah_chokmah_beriah);
    RUN_TEST(test_contains_sefirah_binah_beriah);
    RUN_TEST(test_contains_sefirah_yesod_yetzirah);
    RUN_TEST(test_contains_sefirah_malkuth_yetzirah);

    /* Sefirah-to-World reverse (7) */
    RUN_TEST(test_for_sefirah_keter);
    RUN_TEST(test_for_sefirah_chokmah);
    RUN_TEST(test_for_sefirah_binah);
    RUN_TEST(test_for_sefirah_chesed);
    RUN_TEST(test_for_sefirah_gevurah);
    RUN_TEST(test_for_sefirah_yesod);
    RUN_TEST(test_for_sefirah_malkuth);

    /* Sefirot count (4) */
    RUN_TEST(test_sefirot_count_atzilut);
    RUN_TEST(test_sefirot_count_beriah);
    RUN_TEST(test_sefirot_count_yetzirah);
    RUN_TEST(test_sefirot_count_assiah);

    /* Name/meaning accessors (4) */
    RUN_TEST(test_name_atzilut);
    RUN_TEST(test_name_beriah);
    RUN_TEST(test_meaning_atzilut);
    RUN_TEST(test_meaning_assiah);

    /* Invalid inputs (14) */
    RUN_TEST(test_invalid_world_index);
    RUN_TEST(test_invalid_world_negative);
    RUN_TEST(test_invalid_scale_high);
    RUN_TEST(test_invalid_scale_negative);
    RUN_TEST(test_invalid_sefirah);
    RUN_TEST(test_invalid_sefirah_negative);
    RUN_TEST(test_name_invalid);
    RUN_TEST(test_name_invalid_high);
    RUN_TEST(test_meaning_invalid);
    RUN_TEST(test_meaning_invalid_high);
    RUN_TEST(test_sefirot_count_invalid);
    RUN_TEST(test_sefirot_count_invalid_high);
    RUN_TEST(test_contains_sefirah_invalid_world);
    RUN_TEST(test_contains_sefirah_invalid_sefirah);

    /* Completeness (5) */
    RUN_TEST(test_all_worlds_have_names);
    RUN_TEST(test_all_worlds_have_meanings);
    RUN_TEST(test_descriptions_non_null);
    RUN_TEST(test_all_worlds_have_ids);
    RUN_TEST(test_colors_nonzero);

    /* Color spot checks (2) */
    RUN_TEST(test_atzilut_color_gold_white);
    RUN_TEST(test_beriah_color_blue);

    /* Range consistency (3) */
    RUN_TEST(test_scale_ranges_valid);
    RUN_TEST(test_sefirot_ranges_valid);
    RUN_TEST(test_total_sefirot_coverage);

    return UNITY_END();
}
