/* test_tarot.c — Tarot-Calendar Mapping module tests (Thoth deck)
 * TDD RED phase: tests written before implementation.
 * Source: Aleister Crowley "777 and Other Qabalistic Writings",
 *         Golden Dawn correspondence tables. */

#include "../../unity/unity.h"
#include "../../../src/systems/tarot/tarot.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Major Arcana count ===== */

void test_major_count(void)
{
    TEST_ASSERT_EQUAL_INT(22, tarot_major_count());
}

void test_decan_card_count(void)
{
    TEST_ASSERT_EQUAL_INT(36, tarot_decan_card_count());
}

/* ===== Major Arcana names (Thoth deck) ===== */

void test_major_name_fool(void)
{
    TEST_ASSERT_EQUAL_STRING("The Fool", tarot_major_name(0));
}

void test_major_name_magus(void)
{
    TEST_ASSERT_EQUAL_STRING("The Magus", tarot_major_name(1));
}

void test_major_name_priestess(void)
{
    TEST_ASSERT_EQUAL_STRING("The Priestess", tarot_major_name(2));
}

void test_major_name_empress(void)
{
    TEST_ASSERT_EQUAL_STRING("The Empress", tarot_major_name(3));
}

void test_major_name_emperor(void)
{
    TEST_ASSERT_EQUAL_STRING("The Emperor", tarot_major_name(4));
}

void test_major_name_hierophant(void)
{
    TEST_ASSERT_EQUAL_STRING("The Hierophant", tarot_major_name(5));
}

void test_major_name_lovers(void)
{
    TEST_ASSERT_EQUAL_STRING("The Lovers", tarot_major_name(6));
}

void test_major_name_chariot(void)
{
    TEST_ASSERT_EQUAL_STRING("The Chariot", tarot_major_name(7));
}

void test_major_name_adjustment(void)
{
    /* Thoth: card 8 = Adjustment (RWS: Justice) */
    TEST_ASSERT_EQUAL_STRING("Adjustment", tarot_major_name(8));
}

void test_major_name_hermit(void)
{
    TEST_ASSERT_EQUAL_STRING("The Hermit", tarot_major_name(9));
}

void test_major_name_fortune(void)
{
    TEST_ASSERT_EQUAL_STRING("Fortune", tarot_major_name(10));
}

void test_major_name_lust(void)
{
    /* Thoth: card 11 = Lust (RWS: Strength) */
    TEST_ASSERT_EQUAL_STRING("Lust", tarot_major_name(11));
}

void test_major_name_hanged_man(void)
{
    TEST_ASSERT_EQUAL_STRING("The Hanged Man", tarot_major_name(12));
}

void test_major_name_death(void)
{
    TEST_ASSERT_EQUAL_STRING("Death", tarot_major_name(13));
}

void test_major_name_art(void)
{
    /* Thoth: card 14 = Art (RWS: Temperance) */
    TEST_ASSERT_EQUAL_STRING("Art", tarot_major_name(14));
}

void test_major_name_devil(void)
{
    TEST_ASSERT_EQUAL_STRING("The Devil", tarot_major_name(15));
}

void test_major_name_tower(void)
{
    TEST_ASSERT_EQUAL_STRING("The Tower", tarot_major_name(16));
}

void test_major_name_star(void)
{
    TEST_ASSERT_EQUAL_STRING("The Star", tarot_major_name(17));
}

void test_major_name_moon(void)
{
    TEST_ASSERT_EQUAL_STRING("The Moon", tarot_major_name(18));
}

void test_major_name_sun(void)
{
    TEST_ASSERT_EQUAL_STRING("The Sun", tarot_major_name(19));
}

void test_major_name_aeon(void)
{
    /* Thoth: card 20 = The Aeon (RWS: Judgement) */
    TEST_ASSERT_EQUAL_STRING("The Aeon", tarot_major_name(20));
}

void test_major_name_universe(void)
{
    /* Thoth: card 21 = The Universe (RWS: The World) */
    TEST_ASSERT_EQUAL_STRING("The Universe", tarot_major_name(21));
}

void test_major_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tarot_major_name(-1));
    TEST_ASSERT_EQUAL_STRING("Unknown", tarot_major_name(22));
}

/* ===== Hebrew Letters ===== */

void test_hebrew_letter_fool(void)
{
    TEST_ASSERT_EQUAL_STRING("Aleph", tarot_hebrew_letter(0));
}

void test_hebrew_letter_magus(void)
{
    TEST_ASSERT_EQUAL_STRING("Beth", tarot_hebrew_letter(1));
}

void test_hebrew_letter_adjustment(void)
{
    /* Thoth 8 = Lamed (Libra) */
    TEST_ASSERT_EQUAL_STRING("Lamed", tarot_hebrew_letter(8));
}

void test_hebrew_letter_lust(void)
{
    /* Thoth 11 = Teth (Leo) */
    TEST_ASSERT_EQUAL_STRING("Teth", tarot_hebrew_letter(11));
}

void test_hebrew_letter_universe(void)
{
    TEST_ASSERT_EQUAL_STRING("Tav", tarot_hebrew_letter(21));
}

void test_hebrew_letter_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tarot_hebrew_letter(-1));
    TEST_ASSERT_EQUAL_STRING("Unknown", tarot_hebrew_letter(22));
}

/* ===== Attribution Types ===== */

void test_attr_fool_element(void)
{
    tarot_major_t m = tarot_major_get(0);
    TEST_ASSERT_EQUAL_INT(TAROT_ATTR_ELEMENT, m.attr_type);
}

void test_attr_magus_planet(void)
{
    tarot_major_t m = tarot_major_get(1);
    TEST_ASSERT_EQUAL_INT(TAROT_ATTR_PLANET, m.attr_type);
}

void test_attr_emperor_zodiac(void)
{
    tarot_major_t m = tarot_major_get(4);
    TEST_ASSERT_EQUAL_INT(TAROT_ATTR_ZODIAC, m.attr_type);
}

/* ===== Attribution Indices ===== */

void test_attr_index_fool_air(void)
{
    tarot_major_t m = tarot_major_get(0);
    /* Air element = TAROT_ELEM_AIR = 2 */
    TEST_ASSERT_EQUAL_INT(TAROT_ELEM_AIR, m.attr_index);
}

void test_attr_index_magus_mercury(void)
{
    /* Mercury = planet index 0 in 7 classical planet order */
    tarot_major_t m = tarot_major_get(1);
    TEST_ASSERT_EQUAL_INT(0, m.attr_index);
}

void test_attr_index_priestess_moon(void)
{
    /* Moon = planet index 1 */
    tarot_major_t m = tarot_major_get(2);
    TEST_ASSERT_EQUAL_INT(1, m.attr_index);
}

void test_attr_index_emperor_aries(void)
{
    /* Aries = zodiac sign 0 */
    tarot_major_t m = tarot_major_get(4);
    TEST_ASSERT_EQUAL_INT(0, m.attr_index);
}

void test_attr_index_hierophant_taurus(void)
{
    tarot_major_t m = tarot_major_get(5);
    TEST_ASSERT_EQUAL_INT(1, m.attr_index);
}

void test_attr_index_adjustment_libra(void)
{
    /* Thoth 8 = Libra = zodiac sign 6 */
    tarot_major_t m = tarot_major_get(8);
    TEST_ASSERT_EQUAL_INT(6, m.attr_index);
}

void test_attr_index_lust_leo(void)
{
    /* Thoth 11 = Leo = zodiac sign 4 */
    tarot_major_t m = tarot_major_get(11);
    TEST_ASSERT_EQUAL_INT(4, m.attr_index);
}

void test_attr_index_hanged_man_water(void)
{
    tarot_major_t m = tarot_major_get(12);
    TEST_ASSERT_EQUAL_INT(TAROT_ELEM_WATER, m.attr_index);
}

void test_attr_index_aeon_fire(void)
{
    tarot_major_t m = tarot_major_get(20);
    TEST_ASSERT_EQUAL_INT(TAROT_ELEM_FIRE, m.attr_index);
}

void test_attr_index_universe_saturn(void)
{
    /* Saturn = planet index 6 (TAROT_PLANET_SATURN) */
    tarot_major_t m = tarot_major_get(21);
    TEST_ASSERT_EQUAL_INT(TAROT_PLANET_SATURN, m.attr_index);
}

/* ===== Tree Paths ===== */

void test_tree_path_fool(void)
{
    TEST_ASSERT_EQUAL_INT(0, tarot_path_on_tree(0));
}

void test_tree_path_universe(void)
{
    TEST_ASSERT_EQUAL_INT(21, tarot_path_on_tree(21));
}

void test_tree_path_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, tarot_path_on_tree(-1));
    TEST_ASSERT_EQUAL_INT(-1, tarot_path_on_tree(22));
}

/* ===== Major Arcana struct ===== */

void test_major_get_number(void)
{
    tarot_major_t m = tarot_major_get(10);
    TEST_ASSERT_EQUAL_INT(10, m.number);
}

void test_major_get_invalid(void)
{
    tarot_major_t m = tarot_major_get(99);
    TEST_ASSERT_EQUAL_INT(-1, m.number);
}

/* ===== Major for Zodiac Sign ===== */

void test_major_for_aries(void)
{
    /* Aries -> Emperor (4) */
    TEST_ASSERT_EQUAL_INT(4, tarot_major_for_sign(0));
}

void test_major_for_taurus(void)
{
    /* Taurus -> Hierophant (5) */
    TEST_ASSERT_EQUAL_INT(5, tarot_major_for_sign(1));
}

void test_major_for_gemini(void)
{
    /* Gemini -> Lovers (6) */
    TEST_ASSERT_EQUAL_INT(6, tarot_major_for_sign(2));
}

void test_major_for_cancer(void)
{
    /* Cancer -> Chariot (7) */
    TEST_ASSERT_EQUAL_INT(7, tarot_major_for_sign(3));
}

void test_major_for_leo(void)
{
    /* Leo -> Lust (11) — Thoth swap */
    TEST_ASSERT_EQUAL_INT(11, tarot_major_for_sign(4));
}

void test_major_for_virgo(void)
{
    /* Virgo -> Hermit (9) */
    TEST_ASSERT_EQUAL_INT(9, tarot_major_for_sign(5));
}

void test_major_for_libra(void)
{
    /* Libra -> Adjustment (8) — Thoth swap */
    TEST_ASSERT_EQUAL_INT(8, tarot_major_for_sign(6));
}

void test_major_for_scorpio(void)
{
    /* Scorpio -> Death (13) */
    TEST_ASSERT_EQUAL_INT(13, tarot_major_for_sign(7));
}

void test_major_for_sagittarius(void)
{
    /* Sagittarius -> Art (14) */
    TEST_ASSERT_EQUAL_INT(14, tarot_major_for_sign(8));
}

void test_major_for_capricorn(void)
{
    /* Capricorn -> Devil (15) */
    TEST_ASSERT_EQUAL_INT(15, tarot_major_for_sign(9));
}

void test_major_for_aquarius(void)
{
    /* Aquarius -> Star (17) */
    TEST_ASSERT_EQUAL_INT(17, tarot_major_for_sign(10));
}

void test_major_for_pisces(void)
{
    /* Pisces -> Moon (18) */
    TEST_ASSERT_EQUAL_INT(18, tarot_major_for_sign(11));
}

void test_major_for_sign_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, tarot_major_for_sign(-1));
    TEST_ASSERT_EQUAL_INT(-1, tarot_major_for_sign(12));
}

/* ===== Major for Planet ===== */

void test_major_for_mercury(void)
{
    /* Mercury -> Magus (1) */
    TEST_ASSERT_EQUAL_INT(1, tarot_major_for_planet(0));
}

void test_major_for_moon(void)
{
    /* Moon -> Priestess (2) */
    TEST_ASSERT_EQUAL_INT(2, tarot_major_for_planet(1));
}

void test_major_for_venus(void)
{
    /* Venus -> Empress (3) */
    TEST_ASSERT_EQUAL_INT(3, tarot_major_for_planet(2));
}

void test_major_for_jupiter(void)
{
    /* Jupiter -> Fortune (10) */
    TEST_ASSERT_EQUAL_INT(10, tarot_major_for_planet(3));
}

void test_major_for_mars(void)
{
    /* Mars -> Tower (16) */
    TEST_ASSERT_EQUAL_INT(16, tarot_major_for_planet(4));
}

void test_major_for_sun(void)
{
    /* Sun -> Sun (19) */
    TEST_ASSERT_EQUAL_INT(19, tarot_major_for_planet(5));
}

void test_major_for_saturn(void)
{
    /* Saturn -> Universe (21) */
    TEST_ASSERT_EQUAL_INT(21, tarot_major_for_planet(6));
}

void test_major_for_planet_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, tarot_major_for_planet(-1));
    TEST_ASSERT_EQUAL_INT(-1, tarot_major_for_planet(7));
}

/* ===== Decan Cards ===== */

void test_decan_aries_1(void)
{
    /* 5 deg Aries -> 2 of Wands */
    tarot_minor_t c = tarot_decan_card(5.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(2, c.rank);
    TEST_ASSERT_EQUAL_INT(0, c.zodiac_sign); /* Aries */
}

void test_decan_aries_2(void)
{
    /* 15 deg Aries -> 3 of Wands */
    tarot_minor_t c = tarot_decan_card(15.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(3, c.rank);
}

void test_decan_aries_3(void)
{
    /* 25 deg Aries -> 4 of Wands */
    tarot_minor_t c = tarot_decan_card(25.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(4, c.rank);
}

void test_decan_taurus_1(void)
{
    /* 35 deg = Taurus decan 1 -> 5 of Disks */
    tarot_minor_t c = tarot_decan_card(35.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_DISKS, c.suit);
    TEST_ASSERT_EQUAL_INT(5, c.rank);
    TEST_ASSERT_EQUAL_INT(1, c.zodiac_sign); /* Taurus */
}

void test_decan_gemini_1(void)
{
    /* 65 deg = Gemini decan 1 -> 8 of Swords */
    tarot_minor_t c = tarot_decan_card(65.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_SWORDS, c.suit);
    TEST_ASSERT_EQUAL_INT(8, c.rank);
    TEST_ASSERT_EQUAL_INT(2, c.zodiac_sign); /* Gemini */
}

void test_decan_cancer_1(void)
{
    /* 95 deg = Cancer decan 1 -> 2 of Cups */
    tarot_minor_t c = tarot_decan_card(95.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_CUPS, c.suit);
    TEST_ASSERT_EQUAL_INT(2, c.rank);
    TEST_ASSERT_EQUAL_INT(3, c.zodiac_sign); /* Cancer */
}

void test_decan_leo_1(void)
{
    /* 125 deg = Leo decan 1 -> 5 of Wands */
    tarot_minor_t c = tarot_decan_card(125.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(5, c.rank);
    TEST_ASSERT_EQUAL_INT(4, c.zodiac_sign); /* Leo */
}

void test_decan_virgo_1(void)
{
    /* 155 deg = Virgo decan 1 -> 8 of Disks */
    tarot_minor_t c = tarot_decan_card(155.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_DISKS, c.suit);
    TEST_ASSERT_EQUAL_INT(8, c.rank);
    TEST_ASSERT_EQUAL_INT(5, c.zodiac_sign); /* Virgo */
}

void test_decan_libra_1(void)
{
    /* 185 deg = Libra decan 1 -> 2 of Swords */
    tarot_minor_t c = tarot_decan_card(185.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_SWORDS, c.suit);
    TEST_ASSERT_EQUAL_INT(2, c.rank);
    TEST_ASSERT_EQUAL_INT(6, c.zodiac_sign); /* Libra */
}

void test_decan_scorpio_1(void)
{
    /* 215 deg = Scorpio decan 1 -> 5 of Cups */
    tarot_minor_t c = tarot_decan_card(215.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_CUPS, c.suit);
    TEST_ASSERT_EQUAL_INT(5, c.rank);
    TEST_ASSERT_EQUAL_INT(7, c.zodiac_sign); /* Scorpio */
}

void test_decan_sagittarius_1(void)
{
    /* 245 deg = Sagittarius decan 1 -> 8 of Wands */
    tarot_minor_t c = tarot_decan_card(245.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(8, c.rank);
    TEST_ASSERT_EQUAL_INT(8, c.zodiac_sign); /* Sagittarius */
}

void test_decan_capricorn_1(void)
{
    /* 275 deg = Capricorn decan 1 -> 2 of Disks */
    tarot_minor_t c = tarot_decan_card(275.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_DISKS, c.suit);
    TEST_ASSERT_EQUAL_INT(2, c.rank);
    TEST_ASSERT_EQUAL_INT(9, c.zodiac_sign); /* Capricorn */
}

void test_decan_aquarius_1(void)
{
    /* 305 deg = Aquarius decan 1 -> 5 of Swords */
    tarot_minor_t c = tarot_decan_card(305.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_SWORDS, c.suit);
    TEST_ASSERT_EQUAL_INT(5, c.rank);
    TEST_ASSERT_EQUAL_INT(10, c.zodiac_sign); /* Aquarius */
}

void test_decan_pisces_1(void)
{
    /* 335 deg = Pisces decan 1 -> 8 of Cups */
    tarot_minor_t c = tarot_decan_card(335.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_CUPS, c.suit);
    TEST_ASSERT_EQUAL_INT(8, c.rank);
    TEST_ASSERT_EQUAL_INT(11, c.zodiac_sign); /* Pisces */
}

/* ===== Decan card boundaries ===== */

void test_decan_boundary_0_deg(void)
{
    /* 0 deg exactly = Aries decan 1 -> 2 of Wands */
    tarot_minor_t c = tarot_decan_card(0.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(2, c.rank);
}

void test_decan_boundary_10_deg(void)
{
    /* 10 deg exactly = Aries decan 2 -> 3 of Wands */
    tarot_minor_t c = tarot_decan_card(10.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(3, c.rank);
}

void test_decan_boundary_30_deg(void)
{
    /* 30 deg exactly = Taurus decan 1 -> 5 of Disks */
    tarot_minor_t c = tarot_decan_card(30.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_DISKS, c.suit);
    TEST_ASSERT_EQUAL_INT(5, c.rank);
}

void test_decan_boundary_360_deg(void)
{
    /* 360 deg wraps to 0 = Aries decan 1 -> 2 of Wands */
    tarot_minor_t c = tarot_decan_card(360.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(2, c.rank);
}

void test_decan_negative_longitude(void)
{
    /* -10 deg wraps to 350 = Pisces decan 3 (350-360) -> 10 of Cups */
    tarot_minor_t c = tarot_decan_card(-10.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_CUPS, c.suit);
    TEST_ASSERT_EQUAL_INT(10, c.rank);
}

void test_decan_card_name_aries_1(void)
{
    tarot_minor_t c = tarot_decan_card(5.0);
    TEST_ASSERT_EQUAL_STRING("2 of Wands", c.name);
}

void test_decan_card_name_pisces_3(void)
{
    tarot_minor_t c = tarot_decan_card(355.0);
    TEST_ASSERT_EQUAL_STRING("10 of Cups", c.name);
}

/* ===== Court Cards ===== */

void test_court_knight_wands(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_WANDS, 11);
    TEST_ASSERT_EQUAL_STRING("Knight of Wands", c.title);
    TEST_ASSERT_EQUAL_INT(TAROT_ELEM_FIRE, c.element);
}

void test_court_queen_wands(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_WANDS, 12);
    TEST_ASSERT_EQUAL_STRING("Queen of Wands", c.title);
}

void test_court_prince_wands(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_WANDS, 13);
    TEST_ASSERT_EQUAL_STRING("Prince of Wands", c.title);
}

void test_court_princess_wands(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_WANDS, 14);
    TEST_ASSERT_EQUAL_STRING("Princess of Wands", c.title);
    /* Princess rules a quadrant, no specific zodiac degrees */
    TEST_ASSERT_EQUAL_INT(-1, c.zodiac_start);
    TEST_ASSERT_EQUAL_INT(-1, c.zodiac_end);
}

void test_court_knight_cups(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_CUPS, 11);
    TEST_ASSERT_EQUAL_STRING("Knight of Cups", c.title);
    TEST_ASSERT_EQUAL_INT(TAROT_ELEM_WATER, c.element);
}

void test_court_queen_cups(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_CUPS, 12);
    TEST_ASSERT_EQUAL_STRING("Queen of Cups", c.title);
}

void test_court_prince_cups(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_CUPS, 13);
    TEST_ASSERT_EQUAL_STRING("Prince of Cups", c.title);
}

void test_court_princess_cups(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_CUPS, 14);
    TEST_ASSERT_EQUAL_STRING("Princess of Cups", c.title);
}

void test_court_knight_swords(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_SWORDS, 11);
    TEST_ASSERT_EQUAL_STRING("Knight of Swords", c.title);
    TEST_ASSERT_EQUAL_INT(TAROT_ELEM_AIR, c.element);
}

void test_court_queen_swords(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_SWORDS, 12);
    TEST_ASSERT_EQUAL_STRING("Queen of Swords", c.title);
}

void test_court_prince_swords(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_SWORDS, 13);
    TEST_ASSERT_EQUAL_STRING("Prince of Swords", c.title);
}

void test_court_princess_swords(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_SWORDS, 14);
    TEST_ASSERT_EQUAL_STRING("Princess of Swords", c.title);
}

void test_court_knight_disks(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_DISKS, 11);
    TEST_ASSERT_EQUAL_STRING("Knight of Disks", c.title);
    TEST_ASSERT_EQUAL_INT(TAROT_ELEM_EARTH, c.element);
}

void test_court_queen_disks(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_DISKS, 12);
    TEST_ASSERT_EQUAL_STRING("Queen of Disks", c.title);
}

void test_court_prince_disks(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_DISKS, 13);
    TEST_ASSERT_EQUAL_STRING("Prince of Disks", c.title);
}

void test_court_princess_disks(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_DISKS, 14);
    TEST_ASSERT_EQUAL_STRING("Princess of Disks", c.title);
}

void test_court_invalid_suit(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_MAJOR, 11);
    TEST_ASSERT_EQUAL_INT(-1, c.rank);
}

void test_court_invalid_rank(void)
{
    tarot_court_t c = tarot_court_get(TAROT_SUIT_WANDS, 10);
    TEST_ASSERT_EQUAL_INT(-1, c.rank);
}

/* ===== tarot_for_longitude ===== */

void test_for_longitude_aries(void)
{
    int major;
    tarot_minor_t minor;
    tarot_for_longitude(5.0, &major, &minor);
    /* Aries -> Emperor (4), decan 1 -> 2 of Wands */
    TEST_ASSERT_EQUAL_INT(4, major);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, minor.suit);
    TEST_ASSERT_EQUAL_INT(2, minor.rank);
}

void test_for_longitude_leo(void)
{
    int major;
    tarot_minor_t minor;
    tarot_for_longitude(125.0, &major, &minor);
    /* Leo -> Lust (11), decan 1 -> 5 of Wands */
    TEST_ASSERT_EQUAL_INT(11, major);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, minor.suit);
    TEST_ASSERT_EQUAL_INT(5, minor.rank);
}

void test_for_longitude_libra(void)
{
    int major;
    tarot_minor_t minor;
    tarot_for_longitude(195.0, &major, &minor);
    /* Libra -> Adjustment (8), decan 2 -> 3 of Swords */
    TEST_ASSERT_EQUAL_INT(8, major);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_SWORDS, minor.suit);
    TEST_ASSERT_EQUAL_INT(3, minor.rank);
}

void test_for_longitude_pisces(void)
{
    int major;
    tarot_minor_t minor;
    tarot_for_longitude(345.0, &major, &minor);
    /* Pisces -> Moon (18), decan 2 -> 9 of Cups */
    TEST_ASSERT_EQUAL_INT(18, major);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_CUPS, minor.suit);
    TEST_ASSERT_EQUAL_INT(9, minor.rank);
}

void test_for_longitude_null_major(void)
{
    /* Should not crash when major_out is NULL */
    tarot_minor_t minor;
    tarot_for_longitude(45.0, NULL, &minor);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_DISKS, minor.suit);
}

void test_for_longitude_null_minor(void)
{
    /* Should not crash when minor_out is NULL */
    int major;
    tarot_for_longitude(45.0, &major, NULL);
    TEST_ASSERT_EQUAL_INT(5, major); /* Taurus -> Hierophant */
}

/* ===== Suit Names ===== */

void test_suit_name_wands(void)
{
    TEST_ASSERT_EQUAL_STRING("Wands", tarot_suit_name(TAROT_SUIT_WANDS));
}

void test_suit_name_cups(void)
{
    TEST_ASSERT_EQUAL_STRING("Cups", tarot_suit_name(TAROT_SUIT_CUPS));
}

void test_suit_name_swords(void)
{
    TEST_ASSERT_EQUAL_STRING("Swords", tarot_suit_name(TAROT_SUIT_SWORDS));
}

void test_suit_name_disks(void)
{
    TEST_ASSERT_EQUAL_STRING("Disks", tarot_suit_name(TAROT_SUIT_DISKS));
}

void test_suit_name_major(void)
{
    TEST_ASSERT_EQUAL_STRING("Major Arcana", tarot_suit_name(TAROT_SUIT_MAJOR));
}

void test_suit_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tarot_suit_name(TAROT_SUIT_COUNT));
}

/* ===== Element Names ===== */

void test_element_name_fire(void)
{
    TEST_ASSERT_EQUAL_STRING("Fire", tarot_element_name(TAROT_ELEM_FIRE));
}

void test_element_name_water(void)
{
    TEST_ASSERT_EQUAL_STRING("Water", tarot_element_name(TAROT_ELEM_WATER));
}

void test_element_name_air(void)
{
    TEST_ASSERT_EQUAL_STRING("Air", tarot_element_name(TAROT_ELEM_AIR));
}

void test_element_name_earth(void)
{
    TEST_ASSERT_EQUAL_STRING("Earth", tarot_element_name(TAROT_ELEM_EARTH));
}

void test_element_name_spirit(void)
{
    TEST_ASSERT_EQUAL_STRING("Spirit", tarot_element_name(TAROT_ELEM_SPIRIT));
}

void test_element_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", tarot_element_name(TAROT_ELEM_COUNT));
}

/* ===== Decan card decan_index field ===== */

void test_decan_index_aries_1(void)
{
    tarot_minor_t c = tarot_decan_card(5.0);
    TEST_ASSERT_EQUAL_INT(0, c.decan_index); /* first decan overall */
}

void test_decan_index_pisces_3(void)
{
    tarot_minor_t c = tarot_decan_card(355.0);
    TEST_ASSERT_EQUAL_INT(35, c.decan_index); /* last decan overall */
}

/* ===== Comprehensive decan: verify every zodiac sign maps to correct suit ===== */

void test_decan_taurus_3(void)
{
    /* 55 deg = Taurus decan 3 -> 7 of Disks */
    tarot_minor_t c = tarot_decan_card(55.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_DISKS, c.suit);
    TEST_ASSERT_EQUAL_INT(7, c.rank);
}

void test_decan_gemini_3(void)
{
    /* 85 deg = Gemini decan 3 -> 10 of Swords */
    tarot_minor_t c = tarot_decan_card(85.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_SWORDS, c.suit);
    TEST_ASSERT_EQUAL_INT(10, c.rank);
}

void test_decan_cancer_3(void)
{
    /* 115 deg = Cancer decan 3 -> 4 of Cups */
    tarot_minor_t c = tarot_decan_card(115.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_CUPS, c.suit);
    TEST_ASSERT_EQUAL_INT(4, c.rank);
}

void test_decan_leo_3(void)
{
    /* 145 deg = Leo decan 3 -> 7 of Wands */
    tarot_minor_t c = tarot_decan_card(145.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(7, c.rank);
}

void test_decan_virgo_3(void)
{
    /* 175 deg = Virgo decan 3 -> 10 of Disks */
    tarot_minor_t c = tarot_decan_card(175.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_DISKS, c.suit);
    TEST_ASSERT_EQUAL_INT(10, c.rank);
}

void test_decan_libra_3(void)
{
    /* 205 deg = Libra decan 3 -> 4 of Swords */
    tarot_minor_t c = tarot_decan_card(205.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_SWORDS, c.suit);
    TEST_ASSERT_EQUAL_INT(4, c.rank);
}

void test_decan_scorpio_3(void)
{
    /* 235 deg = Scorpio decan 3 -> 7 of Cups */
    tarot_minor_t c = tarot_decan_card(235.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_CUPS, c.suit);
    TEST_ASSERT_EQUAL_INT(7, c.rank);
}

void test_decan_sagittarius_3(void)
{
    /* 265 deg = Sagittarius decan 3 -> 10 of Wands */
    tarot_minor_t c = tarot_decan_card(265.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(10, c.rank);
}

void test_decan_capricorn_3(void)
{
    /* 295 deg = Capricorn decan 3 -> 4 of Disks */
    tarot_minor_t c = tarot_decan_card(295.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_DISKS, c.suit);
    TEST_ASSERT_EQUAL_INT(4, c.rank);
}

void test_decan_aquarius_3(void)
{
    /* 325 deg = Aquarius decan 3 -> 7 of Swords */
    tarot_minor_t c = tarot_decan_card(325.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_SWORDS, c.suit);
    TEST_ASSERT_EQUAL_INT(7, c.rank);
}

void test_decan_pisces_3(void)
{
    /* 355 deg = Pisces decan 3 -> 10 of Cups */
    tarot_minor_t c = tarot_decan_card(355.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_CUPS, c.suit);
    TEST_ASSERT_EQUAL_INT(10, c.rank);
}

/* ===== Court card zodiac ranges ===== */

void test_court_knight_wands_zodiac(void)
{
    /* Knight of Wands: Scorpio 20 - Sagittarius 20 */
    tarot_court_t c = tarot_court_get(TAROT_SUIT_WANDS, 11);
    TEST_ASSERT_TRUE(c.zodiac_start >= 0);
    TEST_ASSERT_TRUE(c.zodiac_end >= 0);
}

void test_court_princess_zodiac_none(void)
{
    /* All Princesses have zodiac_start = zodiac_end = -1 */
    for (int s = 0; s < 4; s++) {
        tarot_court_t c = tarot_court_get(s, 14);
        TEST_ASSERT_EQUAL_INT(-1, c.zodiac_start);
        TEST_ASSERT_EQUAL_INT(-1, c.zodiac_end);
    }
}

/* ===== Large longitude wrapping ===== */

void test_decan_large_longitude(void)
{
    /* 725 deg = 725 - 2*360 = 5 deg = Aries decan 1 */
    tarot_minor_t c = tarot_decan_card(725.0);
    TEST_ASSERT_EQUAL_INT(TAROT_SUIT_WANDS, c.suit);
    TEST_ASSERT_EQUAL_INT(2, c.rank);
}

int main(void)
{
    UNITY_BEGIN();

    /* Major Arcana counts */
    RUN_TEST(test_major_count);
    RUN_TEST(test_decan_card_count);

    /* Major Arcana names (all 22) */
    RUN_TEST(test_major_name_fool);
    RUN_TEST(test_major_name_magus);
    RUN_TEST(test_major_name_priestess);
    RUN_TEST(test_major_name_empress);
    RUN_TEST(test_major_name_emperor);
    RUN_TEST(test_major_name_hierophant);
    RUN_TEST(test_major_name_lovers);
    RUN_TEST(test_major_name_chariot);
    RUN_TEST(test_major_name_adjustment);
    RUN_TEST(test_major_name_hermit);
    RUN_TEST(test_major_name_fortune);
    RUN_TEST(test_major_name_lust);
    RUN_TEST(test_major_name_hanged_man);
    RUN_TEST(test_major_name_death);
    RUN_TEST(test_major_name_art);
    RUN_TEST(test_major_name_devil);
    RUN_TEST(test_major_name_tower);
    RUN_TEST(test_major_name_star);
    RUN_TEST(test_major_name_moon);
    RUN_TEST(test_major_name_sun);
    RUN_TEST(test_major_name_aeon);
    RUN_TEST(test_major_name_universe);
    RUN_TEST(test_major_name_invalid);

    /* Hebrew letters */
    RUN_TEST(test_hebrew_letter_fool);
    RUN_TEST(test_hebrew_letter_magus);
    RUN_TEST(test_hebrew_letter_adjustment);
    RUN_TEST(test_hebrew_letter_lust);
    RUN_TEST(test_hebrew_letter_universe);
    RUN_TEST(test_hebrew_letter_invalid);

    /* Attribution types */
    RUN_TEST(test_attr_fool_element);
    RUN_TEST(test_attr_magus_planet);
    RUN_TEST(test_attr_emperor_zodiac);

    /* Attribution indices */
    RUN_TEST(test_attr_index_fool_air);
    RUN_TEST(test_attr_index_magus_mercury);
    RUN_TEST(test_attr_index_priestess_moon);
    RUN_TEST(test_attr_index_emperor_aries);
    RUN_TEST(test_attr_index_hierophant_taurus);
    RUN_TEST(test_attr_index_adjustment_libra);
    RUN_TEST(test_attr_index_lust_leo);
    RUN_TEST(test_attr_index_hanged_man_water);
    RUN_TEST(test_attr_index_aeon_fire);
    RUN_TEST(test_attr_index_universe_saturn);

    /* Tree paths */
    RUN_TEST(test_tree_path_fool);
    RUN_TEST(test_tree_path_universe);
    RUN_TEST(test_tree_path_invalid);

    /* Major get struct */
    RUN_TEST(test_major_get_number);
    RUN_TEST(test_major_get_invalid);

    /* Major for zodiac sign (all 12) */
    RUN_TEST(test_major_for_aries);
    RUN_TEST(test_major_for_taurus);
    RUN_TEST(test_major_for_gemini);
    RUN_TEST(test_major_for_cancer);
    RUN_TEST(test_major_for_leo);
    RUN_TEST(test_major_for_virgo);
    RUN_TEST(test_major_for_libra);
    RUN_TEST(test_major_for_scorpio);
    RUN_TEST(test_major_for_sagittarius);
    RUN_TEST(test_major_for_capricorn);
    RUN_TEST(test_major_for_aquarius);
    RUN_TEST(test_major_for_pisces);
    RUN_TEST(test_major_for_sign_invalid);

    /* Major for planet (all 7) */
    RUN_TEST(test_major_for_mercury);
    RUN_TEST(test_major_for_moon);
    RUN_TEST(test_major_for_venus);
    RUN_TEST(test_major_for_jupiter);
    RUN_TEST(test_major_for_mars);
    RUN_TEST(test_major_for_sun);
    RUN_TEST(test_major_for_saturn);
    RUN_TEST(test_major_for_planet_invalid);

    /* Decan cards (one per zodiac sign) */
    RUN_TEST(test_decan_aries_1);
    RUN_TEST(test_decan_aries_2);
    RUN_TEST(test_decan_aries_3);
    RUN_TEST(test_decan_taurus_1);
    RUN_TEST(test_decan_gemini_1);
    RUN_TEST(test_decan_cancer_1);
    RUN_TEST(test_decan_leo_1);
    RUN_TEST(test_decan_virgo_1);
    RUN_TEST(test_decan_libra_1);
    RUN_TEST(test_decan_scorpio_1);
    RUN_TEST(test_decan_sagittarius_1);
    RUN_TEST(test_decan_capricorn_1);
    RUN_TEST(test_decan_aquarius_1);
    RUN_TEST(test_decan_pisces_1);

    /* Decan boundaries */
    RUN_TEST(test_decan_boundary_0_deg);
    RUN_TEST(test_decan_boundary_10_deg);
    RUN_TEST(test_decan_boundary_30_deg);
    RUN_TEST(test_decan_boundary_360_deg);
    RUN_TEST(test_decan_negative_longitude);

    /* Decan card names */
    RUN_TEST(test_decan_card_name_aries_1);
    RUN_TEST(test_decan_card_name_pisces_3);

    /* Court cards (all 4 suits x 4 ranks) */
    RUN_TEST(test_court_knight_wands);
    RUN_TEST(test_court_queen_wands);
    RUN_TEST(test_court_prince_wands);
    RUN_TEST(test_court_princess_wands);
    RUN_TEST(test_court_knight_cups);
    RUN_TEST(test_court_queen_cups);
    RUN_TEST(test_court_prince_cups);
    RUN_TEST(test_court_princess_cups);
    RUN_TEST(test_court_knight_swords);
    RUN_TEST(test_court_queen_swords);
    RUN_TEST(test_court_prince_swords);
    RUN_TEST(test_court_princess_swords);
    RUN_TEST(test_court_knight_disks);
    RUN_TEST(test_court_queen_disks);
    RUN_TEST(test_court_prince_disks);
    RUN_TEST(test_court_princess_disks);
    RUN_TEST(test_court_invalid_suit);
    RUN_TEST(test_court_invalid_rank);

    /* tarot_for_longitude */
    RUN_TEST(test_for_longitude_aries);
    RUN_TEST(test_for_longitude_leo);
    RUN_TEST(test_for_longitude_libra);
    RUN_TEST(test_for_longitude_pisces);
    RUN_TEST(test_for_longitude_null_major);
    RUN_TEST(test_for_longitude_null_minor);

    /* Suit names */
    RUN_TEST(test_suit_name_wands);
    RUN_TEST(test_suit_name_cups);
    RUN_TEST(test_suit_name_swords);
    RUN_TEST(test_suit_name_disks);
    RUN_TEST(test_suit_name_major);
    RUN_TEST(test_suit_name_invalid);

    /* Element names */
    RUN_TEST(test_element_name_fire);
    RUN_TEST(test_element_name_water);
    RUN_TEST(test_element_name_air);
    RUN_TEST(test_element_name_earth);
    RUN_TEST(test_element_name_spirit);
    RUN_TEST(test_element_name_invalid);

    /* Decan index field */
    RUN_TEST(test_decan_index_aries_1);
    RUN_TEST(test_decan_index_pisces_3);

    /* Comprehensive decan-suit mapping */
    RUN_TEST(test_decan_taurus_3);
    RUN_TEST(test_decan_gemini_3);
    RUN_TEST(test_decan_cancer_3);
    RUN_TEST(test_decan_leo_3);
    RUN_TEST(test_decan_virgo_3);
    RUN_TEST(test_decan_libra_3);
    RUN_TEST(test_decan_scorpio_3);
    RUN_TEST(test_decan_sagittarius_3);
    RUN_TEST(test_decan_capricorn_3);
    RUN_TEST(test_decan_aquarius_3);
    RUN_TEST(test_decan_pisces_3);

    /* Court zodiac ranges */
    RUN_TEST(test_court_knight_wands_zodiac);
    RUN_TEST(test_court_princess_zodiac_none);

    /* Large longitude wrapping */
    RUN_TEST(test_decan_large_longitude);

    return UNITY_END();
}
