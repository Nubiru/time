/* test_sefirot.c — Kabbalah Tree of Life Sefirot data module tests
 * TDD RED phase: tests written before implementation.
 * Source: Traditional Kabbalistic Tree of Life, Golden Dawn color scales. */

#include "../../unity/unity.h"
#include "../../../src/systems/kabbalah/sefirot.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Constants ===== */

void test_sefirot_count(void)
{
    TEST_ASSERT_EQUAL_INT(10, SEFIROT_COUNT);
}

void test_sefirot_count_with_daat(void)
{
    TEST_ASSERT_EQUAL_INT(11, SEFIROT_COUNT_WITH_DAAT);
}

/* ===== Keter (0) ===== */

void test_keter_name(void)
{
    sefirah_t s = sefirot_get(0);
    TEST_ASSERT_EQUAL_STRING("Keter", s.name);
}

void test_keter_meaning(void)
{
    sefirah_t s = sefirot_get(0);
    TEST_ASSERT_EQUAL_STRING("Crown", s.meaning);
}

/* ===== Malkuth (9) ===== */

void test_malkuth_name(void)
{
    sefirah_t s = sefirot_get(9);
    TEST_ASSERT_EQUAL_STRING("Malkuth", s.name);
}

void test_malkuth_meaning(void)
{
    sefirah_t s = sefirot_get(9);
    TEST_ASSERT_EQUAL_STRING("Kingdom", s.meaning);
}

/* ===== Planet mappings ===== */

void test_tiferet_planet(void)
{
    /* Tiferet = Sun, not in planets.h => -1 */
    sefirah_t s = sefirot_get(5);
    TEST_ASSERT_EQUAL_INT(-1, s.planet_index);
}

void test_chesed_planet(void)
{
    /* Chesed = Jupiter = PLANET_JUPITER (4) */
    sefirah_t s = sefirot_get(3);
    TEST_ASSERT_EQUAL_INT(4, s.planet_index);
}

void test_gevurah_planet(void)
{
    /* Gevurah = Mars = PLANET_MARS (3) */
    sefirah_t s = sefirot_get(4);
    TEST_ASSERT_EQUAL_INT(3, s.planet_index);
}

void test_hod_planet(void)
{
    /* Hod = Mercury = PLANET_MERCURY (0) */
    sefirah_t s = sefirot_get(7);
    TEST_ASSERT_EQUAL_INT(0, s.planet_index);
}

/* ===== Planet-to-Sefirah reverse lookup ===== */

void test_planet_to_sefirah_mercury(void)
{
    /* Mercury (0) -> Hod (7) */
    sefirah_t s = sefirot_for_planet(0);
    TEST_ASSERT_EQUAL_INT(7, s.id);
}

void test_planet_to_sefirah_venus(void)
{
    /* Venus (1) -> Netzach (6) */
    sefirah_t s = sefirot_for_planet(1);
    TEST_ASSERT_EQUAL_INT(6, s.id);
}

void test_planet_to_sefirah_earth(void)
{
    /* Earth (2) -> Malkuth (9) */
    sefirah_t s = sefirot_for_planet(2);
    TEST_ASSERT_EQUAL_INT(9, s.id);
}

void test_planet_to_sefirah_jupiter(void)
{
    /* Jupiter (4) -> Chesed (3) */
    sefirah_t s = sefirot_for_planet(4);
    TEST_ASSERT_EQUAL_INT(3, s.id);
}

void test_planet_to_sefirah_saturn(void)
{
    /* Saturn (5) -> Binah (2) */
    sefirah_t s = sefirot_for_planet(5);
    TEST_ASSERT_EQUAL_INT(2, s.id);
}

void test_planet_to_sefirah_neptune(void)
{
    /* Neptune (7) -> Keter (0) */
    sefirah_t s = sefirot_for_planet(7);
    TEST_ASSERT_EQUAL_INT(0, s.id);
}

void test_planet_to_sefirah_mars(void)
{
    /* Mars (3) -> Gevurah (4) */
    sefirah_t s = sefirot_for_planet(3);
    TEST_ASSERT_EQUAL_INT(4, s.id);
}

void test_planet_to_sefirah_uranus(void)
{
    /* Uranus (6) -> Chokmah (1) */
    sefirah_t s = sefirot_for_planet(6);
    TEST_ASSERT_EQUAL_INT(1, s.id);
}

/* ===== Pillars ===== */

void test_pillar_keter(void)
{
    /* Keter = Middle pillar (0) */
    sefirah_t s = sefirot_get(0);
    TEST_ASSERT_EQUAL_INT(0, s.pillar);
}

void test_pillar_chokmah(void)
{
    /* Chokmah = Mercy pillar (1) */
    sefirah_t s = sefirot_get(1);
    TEST_ASSERT_EQUAL_INT(1, s.pillar);
}

void test_pillar_binah(void)
{
    /* Binah = Severity pillar (2) */
    sefirah_t s = sefirot_get(2);
    TEST_ASSERT_EQUAL_INT(2, s.pillar);
}

/* ===== Triads ===== */

void test_triad_keter(void)
{
    /* Keter = Supernal triad (0) */
    sefirah_t s = sefirot_get(0);
    TEST_ASSERT_EQUAL_INT(0, s.triad);
}

void test_triad_chesed(void)
{
    /* Chesed = Moral triad (1) */
    sefirah_t s = sefirot_get(3);
    TEST_ASSERT_EQUAL_INT(1, s.triad);
}

void test_triad_netzach(void)
{
    /* Netzach = Material triad (2) */
    sefirah_t s = sefirot_get(6);
    TEST_ASSERT_EQUAL_INT(2, s.triad);
}

void test_triad_malkuth(void)
{
    /* Malkuth = Kingdom triad (3) */
    sefirah_t s = sefirot_get(9);
    TEST_ASSERT_EQUAL_INT(3, s.triad);
}

/* ===== Middle pillar check ===== */

void test_middle_pillar_keter(void)
{
    TEST_ASSERT_TRUE(sefirot_is_middle_pillar(0));
}

void test_middle_pillar_chokmah(void)
{
    /* Chokmah is on the Mercy pillar, not Middle */
    TEST_ASSERT_FALSE(sefirot_is_middle_pillar(1));
}

void test_middle_pillar_tiferet(void)
{
    TEST_ASSERT_TRUE(sefirot_is_middle_pillar(5));
}

void test_middle_pillar_yesod(void)
{
    TEST_ASSERT_TRUE(sefirot_is_middle_pillar(8));
}

void test_middle_pillar_malkuth(void)
{
    TEST_ASSERT_TRUE(sefirot_is_middle_pillar(9));
}

void test_not_middle_pillar_binah(void)
{
    TEST_ASSERT_FALSE(sefirot_is_middle_pillar(2));
}

void test_not_middle_pillar_gevurah(void)
{
    TEST_ASSERT_FALSE(sefirot_is_middle_pillar(4));
}

/* ===== Pillar name strings ===== */

void test_pillar_name_balance(void)
{
    TEST_ASSERT_EQUAL_STRING("Balance", sefirot_pillar_name(0));
}

void test_pillar_name_mercy(void)
{
    TEST_ASSERT_EQUAL_STRING("Mercy", sefirot_pillar_name(1));
}

void test_pillar_name_severity(void)
{
    TEST_ASSERT_EQUAL_STRING("Severity", sefirot_pillar_name(2));
}

/* ===== Triad name strings ===== */

void test_triad_name_supernal(void)
{
    TEST_ASSERT_EQUAL_STRING("Supernal", sefirot_triad_name(0));
}

void test_triad_name_moral(void)
{
    TEST_ASSERT_EQUAL_STRING("Moral", sefirot_triad_name(1));
}

void test_triad_name_material(void)
{
    TEST_ASSERT_EQUAL_STRING("Material", sefirot_triad_name(2));
}

void test_triad_name_kingdom(void)
{
    TEST_ASSERT_EQUAL_STRING("Kingdom", sefirot_triad_name(3));
}

/* ===== Da'at (hidden 11th Sefirah) ===== */

void test_daat_name(void)
{
    sefirah_t d = sefirot_get_daat();
    TEST_ASSERT_EQUAL_STRING("Da'at", d.name);
}

void test_daat_id(void)
{
    sefirah_t d = sefirot_get_daat();
    TEST_ASSERT_EQUAL_INT(10, d.id);
}

void test_daat_meaning(void)
{
    sefirah_t d = sefirot_get_daat();
    TEST_ASSERT_EQUAL_STRING("Knowledge", d.meaning);
}

void test_daat_pillar(void)
{
    sefirah_t d = sefirot_get_daat();
    TEST_ASSERT_EQUAL_INT(0, d.pillar); /* Middle pillar */
}

void test_daat_triad(void)
{
    sefirah_t d = sefirot_get_daat();
    TEST_ASSERT_EQUAL_INT(0, d.triad); /* Supernal */
}

/* ===== Name/meaning accessor functions ===== */

void test_name_function(void)
{
    TEST_ASSERT_EQUAL_STRING("Tiferet", sefirot_name(5));
}

void test_meaning_function(void)
{
    TEST_ASSERT_EQUAL_STRING("Beauty", sefirot_meaning(5));
}

void test_name_function_all(void)
{
    /* Verify each Sefirah name is accessible via the function */
    TEST_ASSERT_EQUAL_STRING("Keter", sefirot_name(0));
    TEST_ASSERT_EQUAL_STRING("Chokmah", sefirot_name(1));
    TEST_ASSERT_EQUAL_STRING("Binah", sefirot_name(2));
    TEST_ASSERT_EQUAL_STRING("Chesed", sefirot_name(3));
    TEST_ASSERT_EQUAL_STRING("Gevurah", sefirot_name(4));
    TEST_ASSERT_EQUAL_STRING("Netzach", sefirot_name(6));
    TEST_ASSERT_EQUAL_STRING("Hod", sefirot_name(7));
    TEST_ASSERT_EQUAL_STRING("Yesod", sefirot_name(8));
    TEST_ASSERT_EQUAL_STRING("Malkuth", sefirot_name(9));
}

/* ===== Invalid/boundary inputs ===== */

void test_invalid_index(void)
{
    sefirah_t s = sefirot_get(15);
    TEST_ASSERT_NULL(s.name);
}

void test_invalid_index_negative(void)
{
    sefirah_t s = sefirot_get(-1);
    TEST_ASSERT_NULL(s.name);
}

void test_invalid_planet(void)
{
    sefirah_t s = sefirot_for_planet(99);
    TEST_ASSERT_NULL(s.name);
}

void test_invalid_planet_negative(void)
{
    sefirah_t s = sefirot_for_planet(-1);
    TEST_ASSERT_NULL(s.name);
}

void test_invalid_name(void)
{
    TEST_ASSERT_EQUAL_STRING("?", sefirot_name(-1));
}

void test_invalid_name_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", sefirot_name(10));
}

void test_invalid_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("?", sefirot_meaning(-1));
}

void test_invalid_pillar_name(void)
{
    TEST_ASSERT_EQUAL_STRING("?", sefirot_pillar_name(5));
}

void test_invalid_triad_name(void)
{
    TEST_ASSERT_EQUAL_STRING("?", sefirot_triad_name(7));
}

/* ===== Positions ===== */

void test_positions_keter_top(void)
{
    sefirah_t s = sefirot_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, s.pos_x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, s.pos_y);
}

void test_positions_malkuth_bottom(void)
{
    sefirah_t s = sefirot_get(9);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, s.pos_x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, s.pos_y);
}

void test_positions_chokmah_right(void)
{
    sefirah_t s = sefirot_get(1);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, s.pos_x);
}

void test_positions_binah_left(void)
{
    sefirah_t s = sefirot_get(2);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, -0.5f, s.pos_x);
}

/* ===== Colors ===== */

void test_colors_keter_white(void)
{
    sefirah_t s = sefirot_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, s.color.r);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, s.color.g);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, s.color.b);
}

void test_colors_nonzero(void)
{
    /* At least some Sefirot have non-zero color channels */
    int has_color = 0;
    for (int i = 0; i < SEFIROT_COUNT; i++) {
        sefirah_t s = sefirot_get(i);
        if (s.color.r > 0.0f || s.color.g > 0.0f || s.color.b > 0.0f) {
            has_color++;
        }
    }
    /* All 10 should have some color (even Binah "black" has 0.1) */
    TEST_ASSERT_EQUAL_INT(SEFIROT_COUNT, has_color);
}

/* ===== All Sefirot completeness ===== */

void test_all_sefirot_have_names(void)
{
    for (int i = 0; i < SEFIROT_COUNT; i++) {
        sefirah_t s = sefirot_get(i);
        TEST_ASSERT_NOT_NULL(s.name);
    }
}

void test_all_sefirot_have_meanings(void)
{
    for (int i = 0; i < SEFIROT_COUNT; i++) {
        sefirah_t s = sefirot_get(i);
        TEST_ASSERT_NOT_NULL(s.meaning);
    }
}

void test_all_sefirot_have_ids(void)
{
    for (int i = 0; i < SEFIROT_COUNT; i++) {
        sefirah_t s = sefirot_get(i);
        TEST_ASSERT_EQUAL_INT(i, s.id);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* Constants (2) */
    RUN_TEST(test_sefirot_count);
    RUN_TEST(test_sefirot_count_with_daat);

    /* Keter (2) */
    RUN_TEST(test_keter_name);
    RUN_TEST(test_keter_meaning);

    /* Malkuth (2) */
    RUN_TEST(test_malkuth_name);
    RUN_TEST(test_malkuth_meaning);

    /* Planet mappings (4) */
    RUN_TEST(test_tiferet_planet);
    RUN_TEST(test_chesed_planet);
    RUN_TEST(test_gevurah_planet);
    RUN_TEST(test_hod_planet);

    /* Planet-to-Sefirah reverse (8) */
    RUN_TEST(test_planet_to_sefirah_mercury);
    RUN_TEST(test_planet_to_sefirah_venus);
    RUN_TEST(test_planet_to_sefirah_earth);
    RUN_TEST(test_planet_to_sefirah_jupiter);
    RUN_TEST(test_planet_to_sefirah_saturn);
    RUN_TEST(test_planet_to_sefirah_neptune);
    RUN_TEST(test_planet_to_sefirah_mars);
    RUN_TEST(test_planet_to_sefirah_uranus);

    /* Pillars (3) */
    RUN_TEST(test_pillar_keter);
    RUN_TEST(test_pillar_chokmah);
    RUN_TEST(test_pillar_binah);

    /* Triads (4) */
    RUN_TEST(test_triad_keter);
    RUN_TEST(test_triad_chesed);
    RUN_TEST(test_triad_netzach);
    RUN_TEST(test_triad_malkuth);

    /* Middle pillar (7) */
    RUN_TEST(test_middle_pillar_keter);
    RUN_TEST(test_middle_pillar_chokmah);
    RUN_TEST(test_middle_pillar_tiferet);
    RUN_TEST(test_middle_pillar_yesod);
    RUN_TEST(test_middle_pillar_malkuth);
    RUN_TEST(test_not_middle_pillar_binah);
    RUN_TEST(test_not_middle_pillar_gevurah);

    /* Pillar names (3) */
    RUN_TEST(test_pillar_name_balance);
    RUN_TEST(test_pillar_name_mercy);
    RUN_TEST(test_pillar_name_severity);

    /* Triad names (4) */
    RUN_TEST(test_triad_name_supernal);
    RUN_TEST(test_triad_name_moral);
    RUN_TEST(test_triad_name_material);
    RUN_TEST(test_triad_name_kingdom);

    /* Da'at (5) */
    RUN_TEST(test_daat_name);
    RUN_TEST(test_daat_id);
    RUN_TEST(test_daat_meaning);
    RUN_TEST(test_daat_pillar);
    RUN_TEST(test_daat_triad);

    /* Name/meaning accessors (3) */
    RUN_TEST(test_name_function);
    RUN_TEST(test_meaning_function);
    RUN_TEST(test_name_function_all);

    /* Invalid inputs (9) */
    RUN_TEST(test_invalid_index);
    RUN_TEST(test_invalid_index_negative);
    RUN_TEST(test_invalid_planet);
    RUN_TEST(test_invalid_planet_negative);
    RUN_TEST(test_invalid_name);
    RUN_TEST(test_invalid_name_high);
    RUN_TEST(test_invalid_meaning);
    RUN_TEST(test_invalid_pillar_name);
    RUN_TEST(test_invalid_triad_name);

    /* Positions (4) */
    RUN_TEST(test_positions_keter_top);
    RUN_TEST(test_positions_malkuth_bottom);
    RUN_TEST(test_positions_chokmah_right);
    RUN_TEST(test_positions_binah_left);

    /* Colors (2) */
    RUN_TEST(test_colors_keter_white);
    RUN_TEST(test_colors_nonzero);

    /* Completeness (3) */
    RUN_TEST(test_all_sefirot_have_names);
    RUN_TEST(test_all_sefirot_have_meanings);
    RUN_TEST(test_all_sefirot_have_ids);

    return UNITY_END();
}
