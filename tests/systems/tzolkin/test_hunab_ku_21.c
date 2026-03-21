#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/hunab_ku_21.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================== */
/* Count                                                              */
/* ================================================================== */

static void test_archetype_count_is_21(void)
{
    TEST_ASSERT_EQUAL_INT(21, hk21_archetype_count());
}

/* ================================================================== */
/* Ring 1 — Gates of Light (seals 1-4)                                */
/* ================================================================== */

static void test_seal1_dragon_name(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(1);
    TEST_ASSERT_EQUAL_STRING("Dragon", a.seal_name);
}

static void test_seal1_dragon_hk_number(void)
{
    TEST_ASSERT_EQUAL_INT(108, hk21_hk_number_for_seal(1));
}

static void test_seal1_dragon_role(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(1);
    TEST_ASSERT_EQUAL_INT(HK21_GATE_OF_LIGHT, a.role);
}

static void test_seal1_dragon_domain(void)
{
    TEST_ASSERT_EQUAL_INT(HK21_KNOWLEDGE, hk21_domain_for_seal(1));
}

static void test_seal1_dragon_planet(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(1);
    TEST_ASSERT_EQUAL_STRING("Neptune", a.planet);
}

static void test_seal1_dragon_flow(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(1);
    TEST_ASSERT_EQUAL_INT(HK21_GALACTIC_KARMIC, a.flow);
}

static void test_seal1_dragon_statement(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(1);
    TEST_ASSERT_EQUAL_STRING("Gate of Being", a.statement);
}

static void test_seal2_wind(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(2);
    TEST_ASSERT_EQUAL_STRING("Wind", a.seal_name);
    TEST_ASSERT_EQUAL_INT(144, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_GATE_OF_LIGHT, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_PROPHECY, a.domain);
    TEST_ASSERT_EQUAL_STRING("Uranus", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_GALACTIC_KARMIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Gate of Spirit", a.statement);
}

static void test_seal3_night(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(3);
    TEST_ASSERT_EQUAL_STRING("Night", a.seal_name);
    TEST_ASSERT_EQUAL_INT(126, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_GATE_OF_LIGHT, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_LOVE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Saturn", a.planet);
    TEST_ASSERT_EQUAL_STRING("Gate of Dreaming", a.statement);
}

static void test_seal4_seed(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(4);
    TEST_ASSERT_EQUAL_STRING("Seed", a.seal_name);
    TEST_ASSERT_EQUAL_INT(90, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_GATE_OF_LIGHT, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_INTELLIGENCE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Jupiter", a.planet);
    TEST_ASSERT_EQUAL_STRING("Gate of Consciousness", a.statement);
}

/* ================================================================== */
/* Ring 2 — Gates of Power (seals 5, 8, 11, 14)                      */
/* ================================================================== */

static void test_seal5_serpent(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(5);
    TEST_ASSERT_EQUAL_STRING("Serpent", a.seal_name);
    TEST_ASSERT_EQUAL_INT(288, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_GATE_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_KNOWLEDGE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Maldek", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_GALACTIC_KARMIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Sex initiates Knowledge", a.statement);
}

static void test_seal8_star(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(8);
    TEST_ASSERT_EQUAL_STRING("Star", a.seal_name);
    TEST_ASSERT_EQUAL_INT(300, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_GATE_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_LOVE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Venus", a.planet);
    TEST_ASSERT_EQUAL_STRING("Art initiates Love", a.statement);
}

static void test_seal11_hand(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(11);
    TEST_ASSERT_EQUAL_STRING("Hand", a.seal_name);
    TEST_ASSERT_EQUAL_INT(312, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_GATE_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_PROPHECY, a.domain);
    TEST_ASSERT_EQUAL_STRING("Venus", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_SOLAR_PROPHETIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Magic initiates Prophecy", a.statement);
}

static void test_seal14_wizard(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(14);
    TEST_ASSERT_EQUAL_STRING("Wizard", a.seal_name);
    TEST_ASSERT_EQUAL_INT(276, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_GATE_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_INTELLIGENCE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Maldek", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_SOLAR_PROPHETIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Timelessness initiates Intelligence", a.statement);
}

/* ================================================================== */
/* Ring 3 — Sources of Power (seals 6, 9, 12, 15)                    */
/* ================================================================== */

static void test_seal6_worldbridger(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(6);
    TEST_ASSERT_EQUAL_STRING("Worldbridger", a.seal_name);
    TEST_ASSERT_EQUAL_INT(294, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_SOURCE_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_KNOWLEDGE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Mars", a.planet);
    TEST_ASSERT_EQUAL_STRING("Death is the Source of Knowledge", a.statement);
}

static void test_seal9_moon(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(9);
    TEST_ASSERT_EQUAL_STRING("Moon", a.seal_name);
    TEST_ASSERT_EQUAL_INT(306, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_SOURCE_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_LOVE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Mercury", a.planet);
    TEST_ASSERT_EQUAL_STRING("Purification is the Source of Love", a.statement);
}

static void test_seal12_human(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(12);
    TEST_ASSERT_EQUAL_STRING("Human", a.seal_name);
    TEST_ASSERT_EQUAL_INT(318, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_SOURCE_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_PROPHECY, a.domain);
    TEST_ASSERT_EQUAL_STRING("Earth", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_SOLAR_PROPHETIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Wisdom is the Source of Prophecy", a.statement);
}

static void test_seal15_eagle(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(15);
    TEST_ASSERT_EQUAL_STRING("Eagle", a.seal_name);
    TEST_ASSERT_EQUAL_INT(282, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_SOURCE_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_INTELLIGENCE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Jupiter", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_SOLAR_PROPHETIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Vision is the Source of Intelligence", a.statement);
}

/* ================================================================== */
/* Ring 4 — Seats of Power (seals 7, 10, 13, 16)                     */
/* ================================================================== */

static void test_seal7_monkey(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(7);
    TEST_ASSERT_EQUAL_STRING("Monkey", a.seal_name);
    TEST_ASSERT_EQUAL_INT(291, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_SEAT_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_KNOWLEDGE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Earth", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_GALACTIC_KARMIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Cosmic Power of Knowledge", a.statement);
}

static void test_seal10_dog(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(10);
    TEST_ASSERT_EQUAL_STRING("Dog", a.seal_name);
    TEST_ASSERT_EQUAL_INT(303, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_SEAT_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_LOVE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Mercury", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_SOLAR_PROPHETIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Cosmic Power of Love", a.statement);
}

static void test_seal13_skywalker(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(13);
    TEST_ASSERT_EQUAL_STRING("Skywalker", a.seal_name);
    TEST_ASSERT_EQUAL_INT(315, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_SEAT_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_PROPHECY, a.domain);
    TEST_ASSERT_EQUAL_STRING("Mars", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_SOLAR_PROPHETIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Cosmic Power of Prophecy", a.statement);
}

static void test_seal16_warrior(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(16);
    TEST_ASSERT_EQUAL_STRING("Warrior", a.seal_name);
    TEST_ASSERT_EQUAL_INT(279, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_SEAT_OF_POWER, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_INTELLIGENCE, a.domain);
    TEST_ASSERT_EQUAL_STRING("Saturn", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_SOLAR_PROPHETIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Cosmic Power of Intelligence", a.statement);
}

/* ================================================================== */
/* Ring 5 — Portal Matrices (seals 17-20)                             */
/* ================================================================== */

static void test_seal17_earth(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(17);
    TEST_ASSERT_EQUAL_STRING("Earth", a.seal_name);
    TEST_ASSERT_EQUAL_INT(396, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_PORTAL_MATRIX, a.role);
    TEST_ASSERT_EQUAL_INT(HK21_UNITY, a.domain);
    TEST_ASSERT_EQUAL_STRING("Uranus", a.planet);
    TEST_ASSERT_EQUAL_INT(HK21_SOLAR_PROPHETIC, a.flow);
    TEST_ASSERT_EQUAL_STRING("Navigation", a.statement);
}

static void test_seal18_mirror(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(18);
    TEST_ASSERT_EQUAL_STRING("Mirror", a.seal_name);
    TEST_ASSERT_EQUAL_INT(402, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_PORTAL_MATRIX, a.role);
    TEST_ASSERT_EQUAL_STRING("Neptune", a.planet);
    TEST_ASSERT_EQUAL_STRING("Meditation", a.statement);
}

static void test_seal19_storm(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(19);
    TEST_ASSERT_EQUAL_STRING("Storm", a.seal_name);
    TEST_ASSERT_EQUAL_INT(408, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_PORTAL_MATRIX, a.role);
    TEST_ASSERT_EQUAL_STRING("Pluto", a.planet);
    TEST_ASSERT_EQUAL_STRING("Self-Generation", a.statement);
}

static void test_seal20_sun(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(20);
    TEST_ASSERT_EQUAL_STRING("Sun", a.seal_name);
    TEST_ASSERT_EQUAL_INT(414, a.hk_number);
    TEST_ASSERT_EQUAL_INT(HK21_PORTAL_MATRIX, a.role);
    TEST_ASSERT_EQUAL_STRING("Pluto", a.planet);
    TEST_ASSERT_EQUAL_STRING("Illumination", a.statement);
}

/* ================================================================== */
/* Center — Hunab Ku (seal 21)                                        */
/* ================================================================== */

static void test_seal21_hunab_ku_name(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(21);
    TEST_ASSERT_EQUAL_STRING("Hunab Ku", a.seal_name);
}

static void test_seal21_hunab_ku_hk_number(void)
{
    TEST_ASSERT_EQUAL_INT(441, hk21_hk_number_for_seal(21));
}

static void test_seal21_hunab_ku_role(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(21);
    TEST_ASSERT_EQUAL_INT(HK21_CENTER, a.role);
}

static void test_seal21_hunab_ku_domain(void)
{
    TEST_ASSERT_EQUAL_INT(HK21_UNITY, hk21_domain_for_seal(21));
}

static void test_seal21_hunab_ku_flow(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(21);
    TEST_ASSERT_EQUAL_INT(HK21_FLOW_NONE, a.flow);
}

static void test_seal21_hunab_ku_statement(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(21);
    TEST_ASSERT_EQUAL_STRING("Coordinates and radializes Totality", a.statement);
}

static void test_seal21_hunab_ku_planet(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(21);
    TEST_ASSERT_EQUAL_STRING("Center", a.planet);
}

/* ================================================================== */
/* HK number 441 = 21 squared                                        */
/* ================================================================== */

static void test_hk_441_is_21_squared(void)
{
    TEST_ASSERT_EQUAL_INT(441, 21 * 21);
    TEST_ASSERT_EQUAL_INT(441, hk21_hk_number_for_seal(21));
}

/* ================================================================== */
/* Lens data                                                          */
/* ================================================================== */

static void test_lens1_cosmic_standard(void)
{
    TEST_ASSERT_EQUAL_INT(108, hk21_lens_number(1));
    TEST_ASSERT_EQUAL_STRING("Cosmic Standard", hk21_lens_name(1));
}

static void test_lens2_cosmic_harmonic(void)
{
    TEST_ASSERT_EQUAL_INT(144, hk21_lens_number(2));
    TEST_ASSERT_EQUAL_STRING("Cosmic Harmonic", hk21_lens_name(2));
}

static void test_lens3_cosmic_cube(void)
{
    TEST_ASSERT_EQUAL_INT(216, hk21_lens_number(3));
    TEST_ASSERT_EQUAL_STRING("Cosmic Cube", hk21_lens_name(3));
}

static void test_lens4_ultimate_sphere(void)
{
    TEST_ASSERT_EQUAL_INT(288, hk21_lens_number(4));
    TEST_ASSERT_EQUAL_STRING("Ultimate Sphere", hk21_lens_name(4));
}

/* ================================================================== */
/* Lens boundaries                                                    */
/* ================================================================== */

static void test_lens0_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, hk21_lens_number(0));
    TEST_ASSERT_NULL(hk21_lens_name(0));
}

static void test_lens5_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, hk21_lens_number(5));
    TEST_ASSERT_NULL(hk21_lens_name(5));
}

static void test_lens_negative_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, hk21_lens_number(-1));
    TEST_ASSERT_NULL(hk21_lens_name(-1));
}

/* ================================================================== */
/* Role name strings                                                  */
/* ================================================================== */

static void test_role_name_gate_of_light(void)
{
    TEST_ASSERT_EQUAL_STRING("Gate of Light", hk21_role_name(HK21_GATE_OF_LIGHT));
}

static void test_role_name_gate_of_power(void)
{
    TEST_ASSERT_EQUAL_STRING("Gate of Power", hk21_role_name(HK21_GATE_OF_POWER));
}

static void test_role_name_source_of_power(void)
{
    TEST_ASSERT_EQUAL_STRING("Source of Power", hk21_role_name(HK21_SOURCE_OF_POWER));
}

static void test_role_name_seat_of_power(void)
{
    TEST_ASSERT_EQUAL_STRING("Seat of Power", hk21_role_name(HK21_SEAT_OF_POWER));
}

static void test_role_name_portal_matrix(void)
{
    TEST_ASSERT_EQUAL_STRING("Portal Matrix", hk21_role_name(HK21_PORTAL_MATRIX));
}

static void test_role_name_center(void)
{
    TEST_ASSERT_EQUAL_STRING("Center", hk21_role_name(HK21_CENTER));
}

/* ================================================================== */
/* Domain name strings                                                */
/* ================================================================== */

static void test_domain_name_knowledge(void)
{
    TEST_ASSERT_EQUAL_STRING("Knowledge", hk21_domain_name(HK21_KNOWLEDGE));
}

static void test_domain_name_love(void)
{
    TEST_ASSERT_EQUAL_STRING("Love", hk21_domain_name(HK21_LOVE));
}

static void test_domain_name_prophecy(void)
{
    TEST_ASSERT_EQUAL_STRING("Prophecy", hk21_domain_name(HK21_PROPHECY));
}

static void test_domain_name_intelligence(void)
{
    TEST_ASSERT_EQUAL_STRING("Intelligence", hk21_domain_name(HK21_INTELLIGENCE));
}

static void test_domain_name_unity(void)
{
    TEST_ASSERT_EQUAL_STRING("Unity", hk21_domain_name(HK21_UNITY));
}

/* ================================================================== */
/* Flow name strings                                                  */
/* ================================================================== */

static void test_flow_name_galactic_karmic(void)
{
    TEST_ASSERT_EQUAL_STRING("Galactic-Karmic", hk21_flow_name(HK21_GALACTIC_KARMIC));
}

static void test_flow_name_solar_prophetic(void)
{
    TEST_ASSERT_EQUAL_STRING("Solar-Prophetic", hk21_flow_name(HK21_SOLAR_PROPHETIC));
}

static void test_flow_name_none(void)
{
    TEST_ASSERT_EQUAL_STRING("None", hk21_flow_name(HK21_FLOW_NONE));
}

/* ================================================================== */
/* Boundary: seal 0 returns zeroed                                    */
/* ================================================================== */

static void test_seal0_returns_zeroed(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(0);
    TEST_ASSERT_EQUAL_INT(0, a.seal_code);
    TEST_ASSERT_NULL(a.seal_name);
    TEST_ASSERT_EQUAL_INT(0, a.hk_number);
    TEST_ASSERT_NULL(a.planet);
    TEST_ASSERT_NULL(a.statement);
}

static void test_seal0_hk_number_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, hk21_hk_number_for_seal(0));
}

static void test_seal0_domain_is_unity(void)
{
    TEST_ASSERT_EQUAL_INT(HK21_UNITY, hk21_domain_for_seal(0));
}

/* ================================================================== */
/* Boundary: seal 22 returns zeroed                                   */
/* ================================================================== */

static void test_seal22_returns_zeroed(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(22);
    TEST_ASSERT_EQUAL_INT(0, a.seal_code);
    TEST_ASSERT_NULL(a.seal_name);
    TEST_ASSERT_EQUAL_INT(0, a.hk_number);
    TEST_ASSERT_NULL(a.planet);
    TEST_ASSERT_NULL(a.statement);
}

static void test_seal22_hk_number_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, hk21_hk_number_for_seal(22));
}

/* ================================================================== */
/* Boundary: negative seal returns zeroed                             */
/* ================================================================== */

static void test_negative_seal_returns_zeroed(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(-1);
    TEST_ASSERT_EQUAL_INT(0, a.seal_code);
    TEST_ASSERT_NULL(a.seal_name);
    TEST_ASSERT_EQUAL_INT(0, a.hk_number);
}

static void test_negative_seal_hk_number_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, hk21_hk_number_for_seal(-99));
}

/* ================================================================== */
/* Purity: same input gives same output                               */
/* ================================================================== */

static void test_purity_same_result(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(10);
    hk21_archetype_t b = hk21_archetype_for_seal(10);
    TEST_ASSERT_EQUAL_INT(a.seal_code, b.seal_code);
    TEST_ASSERT_EQUAL_INT(a.hk_number, b.hk_number);
    TEST_ASSERT_EQUAL_STRING(a.seal_name, b.seal_name);
    TEST_ASSERT_EQUAL_STRING(a.planet, b.planet);
    TEST_ASSERT_EQUAL_STRING(a.statement, b.statement);
    TEST_ASSERT_EQUAL_INT(a.role, b.role);
    TEST_ASSERT_EQUAL_INT(a.domain, b.domain);
    TEST_ASSERT_EQUAL_INT(a.flow, b.flow);
}

/* ================================================================== */
/* Sweep: all 21 archetypes have valid data                           */
/* ================================================================== */

static void test_all_archetypes_have_valid_data(void)
{
    for (int s = 1; s <= 21; s++) {
        hk21_archetype_t a = hk21_archetype_for_seal(s);
        TEST_ASSERT_EQUAL_INT(s, a.seal_code);
        TEST_ASSERT_NOT_NULL(a.seal_name);
        TEST_ASSERT_NOT_NULL(a.planet);
        TEST_ASSERT_NOT_NULL(a.statement);
        TEST_ASSERT_TRUE(a.hk_number > 0);
        TEST_ASSERT_TRUE(strlen(a.seal_name) > 0);
        TEST_ASSERT_TRUE(strlen(a.planet) > 0);
        TEST_ASSERT_TRUE(strlen(a.statement) > 0);
    }
}

/* ================================================================== */
/* HK numbers are all unique                                          */
/* ================================================================== */

static void test_hk_numbers_unique(void)
{
    int nums[21];
    for (int s = 1; s <= 21; s++) {
        nums[s - 1] = hk21_hk_number_for_seal(s);
    }
    for (int i = 0; i < 21; i++) {
        for (int j = i + 1; j < 21; j++) {
            TEST_ASSERT_NOT_EQUAL(nums[i], nums[j]);
        }
    }
}

/* ================================================================== */
/* HK number patterns                                                 */
/* ================================================================== */

static void test_gates_of_light_multiples_of_18(void)
{
    /* Gates of Light (seals 1-4): HK numbers 108, 144, 126, 90
     * All are multiples of 18: 108=18*6, 144=18*8, 126=18*7, 90=18*5 */
    for (int s = 1; s <= 4; s++) {
        int hk = hk21_hk_number_for_seal(s);
        TEST_ASSERT_EQUAL_INT(0, hk % 18);
    }
}

static void test_portal_matrices_increment_by_6(void)
{
    /* Portal Matrices (seals 17-20): 396, 402, 408, 414
     * Each increments by 6 from the previous */
    int prev = hk21_hk_number_for_seal(17);
    TEST_ASSERT_EQUAL_INT(396, prev);
    for (int s = 18; s <= 20; s++) {
        int cur = hk21_hk_number_for_seal(s);
        TEST_ASSERT_EQUAL_INT(prev + 6, cur);
        prev = cur;
    }
}

static void test_portal_matrices_all_multiples_of_6(void)
{
    for (int s = 17; s <= 20; s++) {
        int hk = hk21_hk_number_for_seal(s);
        TEST_ASSERT_EQUAL_INT(0, hk % 6);
    }
}

/* ================================================================== */
/* Domain grouping: each domain has exactly the right seals           */
/* ================================================================== */

static void test_knowledge_domain_seals(void)
{
    /* Knowledge: seals 1, 5, 6, 7 */
    TEST_ASSERT_EQUAL_INT(HK21_KNOWLEDGE, hk21_domain_for_seal(1));
    TEST_ASSERT_EQUAL_INT(HK21_KNOWLEDGE, hk21_domain_for_seal(5));
    TEST_ASSERT_EQUAL_INT(HK21_KNOWLEDGE, hk21_domain_for_seal(6));
    TEST_ASSERT_EQUAL_INT(HK21_KNOWLEDGE, hk21_domain_for_seal(7));
}

static void test_love_domain_seals(void)
{
    /* Love: seals 3, 8, 9, 10 */
    TEST_ASSERT_EQUAL_INT(HK21_LOVE, hk21_domain_for_seal(3));
    TEST_ASSERT_EQUAL_INT(HK21_LOVE, hk21_domain_for_seal(8));
    TEST_ASSERT_EQUAL_INT(HK21_LOVE, hk21_domain_for_seal(9));
    TEST_ASSERT_EQUAL_INT(HK21_LOVE, hk21_domain_for_seal(10));
}

static void test_prophecy_domain_seals(void)
{
    /* Prophecy: seals 2, 11, 12, 13 */
    TEST_ASSERT_EQUAL_INT(HK21_PROPHECY, hk21_domain_for_seal(2));
    TEST_ASSERT_EQUAL_INT(HK21_PROPHECY, hk21_domain_for_seal(11));
    TEST_ASSERT_EQUAL_INT(HK21_PROPHECY, hk21_domain_for_seal(12));
    TEST_ASSERT_EQUAL_INT(HK21_PROPHECY, hk21_domain_for_seal(13));
}

static void test_intelligence_domain_seals(void)
{
    /* Intelligence: seals 4, 14, 15, 16 */
    TEST_ASSERT_EQUAL_INT(HK21_INTELLIGENCE, hk21_domain_for_seal(4));
    TEST_ASSERT_EQUAL_INT(HK21_INTELLIGENCE, hk21_domain_for_seal(14));
    TEST_ASSERT_EQUAL_INT(HK21_INTELLIGENCE, hk21_domain_for_seal(15));
    TEST_ASSERT_EQUAL_INT(HK21_INTELLIGENCE, hk21_domain_for_seal(16));
}

static void test_unity_domain_seals(void)
{
    /* Unity: seals 17-21 */
    for (int s = 17; s <= 21; s++) {
        TEST_ASSERT_EQUAL_INT(HK21_UNITY, hk21_domain_for_seal(s));
    }
}

/* ================================================================== */
/* Flow direction: GK for inner, SP for outer, none for center        */
/* ================================================================== */

static void test_galactic_karmic_seals(void)
{
    /* GK: seals 1, 2, 3, 4, 5, 6, 7, 8, 9 */
    int gk_seals[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 9; i++) {
        hk21_archetype_t a = hk21_archetype_for_seal(gk_seals[i]);
        TEST_ASSERT_EQUAL_INT_MESSAGE(HK21_GALACTIC_KARMIC, a.flow,
                                      a.seal_name);
    }
}

static void test_solar_prophetic_seals(void)
{
    /* SP: seals 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 */
    for (int s = 10; s <= 20; s++) {
        hk21_archetype_t a = hk21_archetype_for_seal(s);
        TEST_ASSERT_EQUAL_INT_MESSAGE(HK21_SOLAR_PROPHETIC, a.flow,
                                      a.seal_name);
    }
}

static void test_hunab_ku_flow_none(void)
{
    hk21_archetype_t a = hk21_archetype_for_seal(21);
    TEST_ASSERT_EQUAL_INT(HK21_FLOW_NONE, a.flow);
}

/* ================================================================== */
/* Role name invalid                                                  */
/* ================================================================== */

static void test_role_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", hk21_role_name((hk21_role_t)99));
}

static void test_domain_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", hk21_domain_name((hk21_domain_t)99));
}

static void test_flow_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", hk21_flow_name((hk21_flow_t)99));
}

/* ================================================================== */
/* Seal code stored correctly                                         */
/* ================================================================== */

static void test_seal_codes_sequential(void)
{
    for (int s = 1; s <= 21; s++) {
        hk21_archetype_t a = hk21_archetype_for_seal(s);
        TEST_ASSERT_EQUAL_INT(s, a.seal_code);
    }
}

/* ================================================================== */
/* main                                                               */
/* ================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_archetype_count_is_21);

    /* Ring 1 — Gates of Light */
    RUN_TEST(test_seal1_dragon_name);
    RUN_TEST(test_seal1_dragon_hk_number);
    RUN_TEST(test_seal1_dragon_role);
    RUN_TEST(test_seal1_dragon_domain);
    RUN_TEST(test_seal1_dragon_planet);
    RUN_TEST(test_seal1_dragon_flow);
    RUN_TEST(test_seal1_dragon_statement);
    RUN_TEST(test_seal2_wind);
    RUN_TEST(test_seal3_night);
    RUN_TEST(test_seal4_seed);

    /* Ring 2 — Gates of Power */
    RUN_TEST(test_seal5_serpent);
    RUN_TEST(test_seal8_star);
    RUN_TEST(test_seal11_hand);
    RUN_TEST(test_seal14_wizard);

    /* Ring 3 — Sources of Power */
    RUN_TEST(test_seal6_worldbridger);
    RUN_TEST(test_seal9_moon);
    RUN_TEST(test_seal12_human);
    RUN_TEST(test_seal15_eagle);

    /* Ring 4 — Seats of Power */
    RUN_TEST(test_seal7_monkey);
    RUN_TEST(test_seal10_dog);
    RUN_TEST(test_seal13_skywalker);
    RUN_TEST(test_seal16_warrior);

    /* Ring 5 — Portal Matrices */
    RUN_TEST(test_seal17_earth);
    RUN_TEST(test_seal18_mirror);
    RUN_TEST(test_seal19_storm);
    RUN_TEST(test_seal20_sun);

    /* Center — Hunab Ku */
    RUN_TEST(test_seal21_hunab_ku_name);
    RUN_TEST(test_seal21_hunab_ku_hk_number);
    RUN_TEST(test_seal21_hunab_ku_role);
    RUN_TEST(test_seal21_hunab_ku_domain);
    RUN_TEST(test_seal21_hunab_ku_flow);
    RUN_TEST(test_seal21_hunab_ku_statement);
    RUN_TEST(test_seal21_hunab_ku_planet);
    RUN_TEST(test_hk_441_is_21_squared);

    /* Lenses */
    RUN_TEST(test_lens1_cosmic_standard);
    RUN_TEST(test_lens2_cosmic_harmonic);
    RUN_TEST(test_lens3_cosmic_cube);
    RUN_TEST(test_lens4_ultimate_sphere);
    RUN_TEST(test_lens0_invalid);
    RUN_TEST(test_lens5_invalid);
    RUN_TEST(test_lens_negative_invalid);

    /* Name strings */
    RUN_TEST(test_role_name_gate_of_light);
    RUN_TEST(test_role_name_gate_of_power);
    RUN_TEST(test_role_name_source_of_power);
    RUN_TEST(test_role_name_seat_of_power);
    RUN_TEST(test_role_name_portal_matrix);
    RUN_TEST(test_role_name_center);
    RUN_TEST(test_domain_name_knowledge);
    RUN_TEST(test_domain_name_love);
    RUN_TEST(test_domain_name_prophecy);
    RUN_TEST(test_domain_name_intelligence);
    RUN_TEST(test_domain_name_unity);
    RUN_TEST(test_flow_name_galactic_karmic);
    RUN_TEST(test_flow_name_solar_prophetic);
    RUN_TEST(test_flow_name_none);

    /* Boundary — invalid seals */
    RUN_TEST(test_seal0_returns_zeroed);
    RUN_TEST(test_seal0_hk_number_is_0);
    RUN_TEST(test_seal0_domain_is_unity);
    RUN_TEST(test_seal22_returns_zeroed);
    RUN_TEST(test_seal22_hk_number_is_0);
    RUN_TEST(test_negative_seal_returns_zeroed);
    RUN_TEST(test_negative_seal_hk_number_is_0);

    /* Purity */
    RUN_TEST(test_purity_same_result);

    /* Sweep */
    RUN_TEST(test_all_archetypes_have_valid_data);
    RUN_TEST(test_hk_numbers_unique);
    RUN_TEST(test_seal_codes_sequential);

    /* HK number patterns */
    RUN_TEST(test_gates_of_light_multiples_of_18);
    RUN_TEST(test_portal_matrices_increment_by_6);
    RUN_TEST(test_portal_matrices_all_multiples_of_6);

    /* Domain grouping */
    RUN_TEST(test_knowledge_domain_seals);
    RUN_TEST(test_love_domain_seals);
    RUN_TEST(test_prophecy_domain_seals);
    RUN_TEST(test_intelligence_domain_seals);
    RUN_TEST(test_unity_domain_seals);

    /* Flow direction */
    RUN_TEST(test_galactic_karmic_seals);
    RUN_TEST(test_solar_prophetic_seals);
    RUN_TEST(test_hunab_ku_flow_none);

    /* Invalid enum values */
    RUN_TEST(test_role_name_invalid);
    RUN_TEST(test_domain_name_invalid);
    RUN_TEST(test_flow_name_invalid);

    return UNITY_END();
}
