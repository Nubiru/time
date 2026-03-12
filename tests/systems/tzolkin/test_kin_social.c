#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/kin_social.h"
#include "../../../src/systems/tzolkin/dreamspell.h"

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * Kin Social — Dreamspell kin relationship & compatibility
 *
 * Seal indexing (0-19): Dragon(0), Wind(1), Night(2), Seed(3),
 *   Serpent(4), World-Bridger(5), Hand(6), Star(7), Moon(8),
 *   Dog(9), Monkey(10), Human(11), Skywalker(12), Wizard(13),
 *   Eagle(14), Warrior(15), Earth(16), Mirror(17), Storm(18), Sun(19)
 *
 * Color: seal % 4 -> 0=Red, 1=White, 2=Blue, 3=Yellow
 * Earth family: seal % 5 -> 0=Cardinal, 1=Core, 2=Signal,
 *                           3=Gateway, 4=Polar
 * Wavespell: (kin-1)/13 + 1 (1-20), 13 consecutive kins each
 * ============================================================ */

/* --- kin_relationship: SAME --- */

void test_relationship_same_kin(void) {
    kin_relationship_t r = kin_relationship(1, 1);
    TEST_ASSERT_EQUAL_INT(KIN_REL_SAME, r.type);
    TEST_ASSERT_EQUAL_INT(100, r.compatibility_score);
}

void test_relationship_same_kin260(void) {
    kin_relationship_t r = kin_relationship(260, 260);
    TEST_ASSERT_EQUAL_INT(KIN_REL_SAME, r.type);
    TEST_ASSERT_EQUAL_INT(100, r.compatibility_score);
}

void test_relationship_same_kin130(void) {
    kin_relationship_t r = kin_relationship(130, 130);
    TEST_ASSERT_EQUAL_INT(KIN_REL_SAME, r.type);
}

/* --- kin_relationship: ANALOG --- */

/* Kin 1 (Dragon tone 1): analog = kin 40 (Sun tone 1) */
void test_relationship_analog_kin1(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    kin_relationship_t r = kin_relationship(1, o.analog);
    TEST_ASSERT_EQUAL_INT(KIN_REL_ANALOG, r.type);
    TEST_ASSERT_EQUAL_INT(80, r.compatibility_score);
}

/* Kin 44: analog = kin 57 */
void test_relationship_analog_kin44(void) {
    dreamspell_oracle_t o = dreamspell_oracle(44);
    kin_relationship_t r = kin_relationship(44, o.analog);
    TEST_ASSERT_EQUAL_INT(KIN_REL_ANALOG, r.type);
}

/* Analog is symmetric: rel(a,b) == rel(b,a) for analog */
void test_relationship_analog_symmetric(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    kin_relationship_t r1 = kin_relationship(1, o.analog);
    kin_relationship_t r2 = kin_relationship(o.analog, 1);
    TEST_ASSERT_EQUAL_INT(KIN_REL_ANALOG, r1.type);
    TEST_ASSERT_EQUAL_INT(KIN_REL_ANALOG, r2.type);
}

/* --- kin_relationship: ANTIPODE --- */

/* Kin 1: antipode = kin 131 */
void test_relationship_antipode_kin1(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    kin_relationship_t r = kin_relationship(1, o.antipode);
    TEST_ASSERT_EQUAL_INT(KIN_REL_ANTIPODE, r.type);
    TEST_ASSERT_EQUAL_INT(80, r.compatibility_score);
}

/* Antipode is symmetric */
void test_relationship_antipode_symmetric(void) {
    dreamspell_oracle_t o = dreamspell_oracle(44);
    kin_relationship_t r1 = kin_relationship(44, o.antipode);
    kin_relationship_t r2 = kin_relationship(o.antipode, 44);
    TEST_ASSERT_EQUAL_INT(KIN_REL_ANTIPODE, r1.type);
    TEST_ASSERT_EQUAL_INT(KIN_REL_ANTIPODE, r2.type);
}

/* --- kin_relationship: OCCULT --- */

/* Kin 1: occult = kin 260 */
void test_relationship_occult_kin1(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    kin_relationship_t r = kin_relationship(1, o.occult);
    TEST_ASSERT_EQUAL_INT(KIN_REL_OCCULT, r.type);
    TEST_ASSERT_EQUAL_INT(80, r.compatibility_score);
}

/* Occult is symmetric: 261 - (261 - k) = k */
void test_relationship_occult_symmetric(void) {
    dreamspell_oracle_t o = dreamspell_oracle(100);
    kin_relationship_t r1 = kin_relationship(100, o.occult);
    kin_relationship_t r2 = kin_relationship(o.occult, 100);
    TEST_ASSERT_EQUAL_INT(KIN_REL_OCCULT, r1.type);
    TEST_ASSERT_EQUAL_INT(KIN_REL_OCCULT, r2.type);
}

/* --- kin_relationship: GUIDE --- */

/* Kin 2: guide = kin 54 (not self-guiding, not same as analog/antipode/occult) */
void test_relationship_guide_kin2(void) {
    dreamspell_oracle_t o = dreamspell_oracle(2);
    /* Guide is 54. Verify it isn't also analog/antipode/occult */
    TEST_ASSERT_TRUE(o.guide != o.analog);
    TEST_ASSERT_TRUE(o.guide != o.antipode);
    TEST_ASSERT_TRUE(o.guide != o.occult);
    kin_relationship_t r = kin_relationship(2, o.guide);
    TEST_ASSERT_EQUAL_INT(KIN_REL_GUIDE, r.type);
    TEST_ASSERT_EQUAL_INT(80, r.compatibility_score);
}

/* Self-guiding kins (tone 1,6,11): guide == destiny, so relationship is SAME */
void test_relationship_guide_self_is_same(void) {
    /* Kin 1: tone 1, guide = self = 1 */
    dreamspell_oracle_t o = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(1, o.guide);
    kin_relationship_t r = kin_relationship(1, 1);
    TEST_ASSERT_EQUAL_INT(KIN_REL_SAME, r.type);
}

/* --- kin_relationship: WAVESPELL --- */

/* Kins 1 and 2 share wavespell 1 (kins 1-13) */
void test_relationship_wavespell_kin1_kin2(void) {
    /* They share WS but kin 2 is not an oracle partner of kin 1 */
    /* kin 1 oracle: analog=40, antipode=131, occult=260, guide=1 */
    /* kin 2 is none of those, but same WS */
    kin_relationship_t r = kin_relationship(1, 5);
    /* kin 5 seal=4 (Serpent), kin 1 seal=0 (Dragon). Different seal/color/earth family.
     * But same wavespell (both in WS 1, kins 1-13). */
    TEST_ASSERT_EQUAL_INT(KIN_REL_WAVESPELL, r.type);
    TEST_ASSERT_EQUAL_INT(35, r.compatibility_score);
}

/* Kins in different wavespells should not be KIN_REL_WAVESPELL */
void test_relationship_not_wavespell_across_boundary(void) {
    /* Kin 13 (WS 1) and kin 14 (WS 2) */
    kin_relationship_t r = kin_relationship(13, 14);
    TEST_ASSERT_TRUE(r.type != KIN_REL_WAVESPELL);
}

/* --- kin_relationship: SEAL_TRIBE --- */

/* Kins with same seal but not same WS or oracle partner */
void test_relationship_seal_tribe(void) {
    /* Seal 0 (Dragon) appears at kins 1, 21, 41, ..., 241 */
    /* Kin 1 and kin 21: same seal (Dragon), different WS */
    /* kin 21 oracle: need to check it's not oracle partner of kin 1 */
    /* kin 1: analog=40, antipode=131, occult=260, guide=1 */
    /* kin 21 is none of those */
    kin_relationship_t r = kin_relationship(1, 21);
    TEST_ASSERT_EQUAL_INT(KIN_REL_SEAL_TRIBE, r.type);
    TEST_ASSERT_EQUAL_INT(40, r.compatibility_score);
}

/* --- kin_relationship: COLOR_FAMILY --- */

/* Same color (seal%4) but different seal, not oracle/WS/tribe */
void test_relationship_color_family(void) {
    /* Kin 1 seal=0 (Red), kin 85 seal=4 (Serpent, Red: 4%4=0) */
    /* Are they in same WS? WS of 85 = (85-1)/13+1 = 84/13+1 = 6+1 = 7. No. */
    /* Same seal? 0 != 4. No. Same earth family? 0%5=0, 4%5=4. No. */
    /* Oracle partners of kin 1: analog=40, antipode=131, occult=260, guide=1 */
    /* 85 is none. So COLOR_FAMILY. */
    kin_relationship_t r = kin_relationship(1, 85);
    TEST_ASSERT_EQUAL_INT(KIN_REL_COLOR_FAMILY, r.type);
    TEST_ASSERT_EQUAL_INT(15, r.compatibility_score);
}

/* --- kin_relationship: EARTH_FAMILY --- */

/* Same earth family (seal%5) but different seal, not oracle/WS/tribe/color */
void test_relationship_earth_family(void) {
    /* Kin 1 seal=0, earth family = Cardinal (seal%5=0): seals {0,5,10,15}
     * Kin with seal=5 (World-Bridger): e.g. kin 46 seal=(46-1)%20=45%20=5
     * Check: WS of 46 = (45)/13+1 = 3+1 = 4. WS of 1 = 1. Different WS.
     * Oracle: kin1 analog=40, antipode=131, occult=260, guide=1. 46 is none.
     * Same seal? 0 != 5. No.
     * Same color? 0%4=0, 5%4=1. No.
     * Same earth family? 0%5=0, 5%5=0. Yes. */
    kin_relationship_t r = kin_relationship(1, 46);
    TEST_ASSERT_EQUAL_INT(KIN_REL_EARTH_FAMILY, r.type);
    TEST_ASSERT_EQUAL_INT(25, r.compatibility_score);
}

/* --- kin_relationship: NONE --- */

void test_relationship_none(void) {
    /* Find two kins with no relationship.
     * Kin 1 seal=0 (Red Dragon, Cardinal)
     * Kin 42 seal=(42-1)%20=41%20=1 (Wind, White, Core)
     * WS of 42 = (41)/13+1 = 3+1 = 4. Different WS.
     * Oracle of kin 1: analog=40, antipode=131, occult=260, guide=1. 42 is none.
     * Same seal? 0!=1. Same color? 0!=1. Same earth family? 0%5=0, 1%5=1. No.
     * Result: NONE */
    kin_relationship_t r = kin_relationship(1, 42);
    TEST_ASSERT_EQUAL_INT(KIN_REL_NONE, r.type);
    TEST_ASSERT_EQUAL_INT(0, r.compatibility_score);
}

/* --- kin_relationship: invalid inputs --- */

void test_relationship_invalid_zero(void) {
    kin_relationship_t r = kin_relationship(0, 1);
    TEST_ASSERT_EQUAL_INT(KIN_REL_NONE, r.type);
    TEST_ASSERT_EQUAL_INT(0, r.compatibility_score);
}

void test_relationship_invalid_261(void) {
    kin_relationship_t r = kin_relationship(1, 261);
    TEST_ASSERT_EQUAL_INT(KIN_REL_NONE, r.type);
}

void test_relationship_invalid_negative(void) {
    kin_relationship_t r = kin_relationship(-5, 100);
    TEST_ASSERT_EQUAL_INT(KIN_REL_NONE, r.type);
}

/* --- kin_relationship: shared flags --- */

void test_relationship_shared_seal_flag(void) {
    /* Same seal: kin 1 and kin 21 (both seal 0) */
    kin_relationship_t r = kin_relationship(1, 21);
    TEST_ASSERT_EQUAL_INT(1, r.shared_seal);
}

void test_relationship_shared_wavespell_flag(void) {
    /* Same WS: kin 1 and kin 5 (both WS 1) */
    kin_relationship_t r = kin_relationship(1, 5);
    TEST_ASSERT_EQUAL_INT(1, r.shared_wavespell);
}

void test_relationship_shared_color_flag(void) {
    /* Same color: kin 1 (seal 0, Red) and kin 85 (seal 4, Red) */
    kin_relationship_t r = kin_relationship(1, 85);
    TEST_ASSERT_EQUAL_INT(1, r.shared_color);
}

void test_relationship_shared_earth_family_flag(void) {
    /* Same earth family: kin 1 (seal 0, Cardinal) and kin 46 (seal 5, Cardinal) */
    kin_relationship_t r = kin_relationship(1, 46);
    TEST_ASSERT_EQUAL_INT(1, r.shared_earth_family);
}

void test_relationship_no_shared_flags(void) {
    /* Kin 1 and kin 42: no shared groups */
    kin_relationship_t r = kin_relationship(1, 42);
    TEST_ASSERT_EQUAL_INT(0, r.shared_seal);
    TEST_ASSERT_EQUAL_INT(0, r.shared_wavespell);
    TEST_ASSERT_EQUAL_INT(0, r.shared_color);
    TEST_ASSERT_EQUAL_INT(0, r.shared_earth_family);
}

/* --- kin_relationship: description is non-null --- */

void test_relationship_description_not_null(void) {
    kin_relationship_t r = kin_relationship(1, 1);
    TEST_ASSERT_NOT_NULL(r.description);
    r = kin_relationship(1, 42);
    TEST_ASSERT_NOT_NULL(r.description);
}

/* --- kin_tribe_members --- */

void test_tribe_seal0_dragon(void) {
    /* Seal 0 (Dragon): kins 1, 21, 41, 61, ..., 241 — 13 total */
    int out[13];
    int count = kin_tribe_members(0, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(1, out[0]);
    TEST_ASSERT_EQUAL_INT(21, out[1]);
    TEST_ASSERT_EQUAL_INT(41, out[2]);
    TEST_ASSERT_EQUAL_INT(241, out[12]);
}

void test_tribe_seal19_sun(void) {
    /* Seal 19 (Sun): kins 20, 40, 60, ..., 260 — 13 total */
    int out[13];
    int count = kin_tribe_members(19, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(20, out[0]);
    TEST_ASSERT_EQUAL_INT(40, out[1]);
    TEST_ASSERT_EQUAL_INT(260, out[12]);
}

void test_tribe_seal10_monkey(void) {
    /* Seal 10 (Monkey): kins 11, 31, 51, ..., 251 */
    int out[13];
    int count = kin_tribe_members(10, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(11, out[0]);
    TEST_ASSERT_EQUAL_INT(31, out[1]);
    TEST_ASSERT_EQUAL_INT(251, out[12]);
}

void test_tribe_invalid_seal(void) {
    int out[13];
    int count = kin_tribe_members(-1, out, 13);
    TEST_ASSERT_EQUAL_INT(0, count);
    count = kin_tribe_members(20, out, 13);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_tribe_small_buffer(void) {
    int out[3];
    int count = kin_tribe_members(0, out, 3);
    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_INT(1, out[0]);
    TEST_ASSERT_EQUAL_INT(21, out[1]);
    TEST_ASSERT_EQUAL_INT(41, out[2]);
}

/* --- kin_wavespell_members --- */

void test_wavespell_members_ws1(void) {
    /* WS 1: kins 1-13 */
    int out[13];
    int count = kin_wavespell_members(1, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(1, out[0]);
    TEST_ASSERT_EQUAL_INT(13, out[12]);
}

void test_wavespell_members_ws20(void) {
    /* WS 20: kins 248-260 */
    int out[13];
    int count = kin_wavespell_members(20, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(248, out[0]);
    TEST_ASSERT_EQUAL_INT(260, out[12]);
}

void test_wavespell_members_invalid(void) {
    int out[13];
    int count = kin_wavespell_members(0, out, 13);
    TEST_ASSERT_EQUAL_INT(0, count);
    count = kin_wavespell_members(21, out, 13);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_wavespell_members_small_buffer(void) {
    int out[5];
    int count = kin_wavespell_members(1, out, 5);
    TEST_ASSERT_EQUAL_INT(5, count);
    TEST_ASSERT_EQUAL_INT(1, out[0]);
    TEST_ASSERT_EQUAL_INT(5, out[4]);
}

/* --- kin_color_family --- */

void test_color_family_red(void) {
    /* Color 0 (Red): seals 0,4,8,12,16 — each appears 13 times = 65 kins */
    int out[65];
    int count = kin_color_family(0, out, 65);
    TEST_ASSERT_EQUAL_INT(65, count);
    /* First kin should be 1 (seal 0) */
    TEST_ASSERT_EQUAL_INT(1, out[0]);
}

void test_color_family_yellow(void) {
    /* Color 3 (Yellow): seals 3,7,11,15,19 */
    int out[65];
    int count = kin_color_family(3, out, 65);
    TEST_ASSERT_EQUAL_INT(65, count);
}

void test_color_family_invalid(void) {
    int out[65];
    int count = kin_color_family(-1, out, 65);
    TEST_ASSERT_EQUAL_INT(0, count);
    count = kin_color_family(4, out, 65);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_color_family_small_buffer(void) {
    int out[10];
    int count = kin_color_family(0, out, 10);
    TEST_ASSERT_EQUAL_INT(10, count);
}

/* --- kin_earth_family_members --- */

void test_earth_family_cardinal(void) {
    /* Cardinal (family 0): seals {0,5,10,15}, each 13 kins = 52 */
    int out[52];
    int count = kin_earth_family_members(0, out, 52);
    TEST_ASSERT_EQUAL_INT(52, count);
    /* First kin: seal 0 = kin 1 */
    TEST_ASSERT_EQUAL_INT(1, out[0]);
}

void test_earth_family_polar(void) {
    /* Polar (family 4): seals {4,9,14,19}, each 13 kins = 52 */
    int out[52];
    int count = kin_earth_family_members(4, out, 52);
    TEST_ASSERT_EQUAL_INT(52, count);
}

void test_earth_family_invalid(void) {
    int out[52];
    int count = kin_earth_family_members(-1, out, 52);
    TEST_ASSERT_EQUAL_INT(0, count);
    count = kin_earth_family_members(5, out, 52);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_earth_family_small_buffer(void) {
    int out[5];
    int count = kin_earth_family_members(0, out, 5);
    TEST_ASSERT_EQUAL_INT(5, count);
}

/* --- kin_compatibility_score --- */

void test_compatibility_same(void) {
    TEST_ASSERT_EQUAL_INT(100, kin_compatibility_score(1, 1));
}

void test_compatibility_analog(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(80, kin_compatibility_score(1, o.analog));
}

void test_compatibility_antipode(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(80, kin_compatibility_score(1, o.antipode));
}

void test_compatibility_occult(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(80, kin_compatibility_score(1, o.occult));
}

void test_compatibility_none(void) {
    TEST_ASSERT_EQUAL_INT(0, kin_compatibility_score(1, 42));
}

void test_compatibility_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, kin_compatibility_score(0, 1));
    TEST_ASSERT_EQUAL_INT(0, kin_compatibility_score(1, 261));
}

/* --- kin_rel_type_name --- */

void test_rel_type_name_none(void) {
    TEST_ASSERT_EQUAL_STRING("None", kin_rel_type_name(KIN_REL_NONE));
}

void test_rel_type_name_same(void) {
    TEST_ASSERT_EQUAL_STRING("Same Kin", kin_rel_type_name(KIN_REL_SAME));
}

void test_rel_type_name_analog(void) {
    TEST_ASSERT_EQUAL_STRING("Analog", kin_rel_type_name(KIN_REL_ANALOG));
}

void test_rel_type_name_antipode(void) {
    TEST_ASSERT_EQUAL_STRING("Antipode", kin_rel_type_name(KIN_REL_ANTIPODE));
}

void test_rel_type_name_occult(void) {
    TEST_ASSERT_EQUAL_STRING("Occult", kin_rel_type_name(KIN_REL_OCCULT));
}

void test_rel_type_name_guide(void) {
    TEST_ASSERT_EQUAL_STRING("Guide", kin_rel_type_name(KIN_REL_GUIDE));
}

void test_rel_type_name_hidden(void) {
    TEST_ASSERT_EQUAL_STRING("Hidden Teacher", kin_rel_type_name(KIN_REL_HIDDEN));
}

void test_rel_type_name_wavespell(void) {
    TEST_ASSERT_EQUAL_STRING("Wavespell", kin_rel_type_name(KIN_REL_WAVESPELL));
}

void test_rel_type_name_seal_tribe(void) {
    TEST_ASSERT_EQUAL_STRING("Seal Tribe", kin_rel_type_name(KIN_REL_SEAL_TRIBE));
}

void test_rel_type_name_color_family(void) {
    TEST_ASSERT_EQUAL_STRING("Color Family", kin_rel_type_name(KIN_REL_COLOR_FAMILY));
}

void test_rel_type_name_earth_family(void) {
    TEST_ASSERT_EQUAL_STRING("Earth Family", kin_rel_type_name(KIN_REL_EARTH_FAMILY));
}

void test_rel_type_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", kin_rel_type_name(KIN_REL_TYPE_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", kin_rel_type_name(-1));
}

/* --- kin_shared_groups --- */

void test_shared_groups_same_kin(void) {
    /* Kin 1 with itself: same seal, same WS, same color, same earth family = 4 */
    int groups = kin_shared_groups(1, 1);
    TEST_ASSERT_EQUAL_INT(4, groups);
}

void test_shared_groups_none(void) {
    /* Kin 1 and kin 42: no shared groups */
    int groups = kin_shared_groups(1, 42);
    TEST_ASSERT_EQUAL_INT(0, groups);
}

void test_shared_groups_same_seal_different_ws(void) {
    /* Kin 1 and kin 21: same seal (0), same color (Red), same earth family (Cardinal) */
    int groups = kin_shared_groups(1, 21);
    TEST_ASSERT_EQUAL_INT(3, groups);
}

void test_shared_groups_same_ws_only(void) {
    /* Find two kins in same WS with different seal/color/earth family.
     * WS 1: kins 1-13. Kin 1 seal=0, kin 2 seal=1.
     * color: 0!=1, earth family: 0!=1. Just wavespell. */
    int groups = kin_shared_groups(1, 2);
    TEST_ASSERT_EQUAL_INT(1, groups);
}

void test_shared_groups_invalid(void) {
    int groups = kin_shared_groups(0, 1);
    TEST_ASSERT_EQUAL_INT(0, groups);
}

/* --- Priority: oracle > seal_tribe > wavespell > earth_family > color_family --- */

/* When a kin is both oracle partner AND shares a seal, oracle wins */
void test_relationship_priority_oracle_over_tribe(void) {
    /* Kin 1 analog=40. Seal of 40 = (40-1)%20 = 19. Seal of 1 = 0. Different seal.
     * That's typical. Let's check: analog shares tone, different seal by definition. */
    dreamspell_oracle_t o = dreamspell_oracle(1);
    kin_relationship_t r = kin_relationship(1, o.analog);
    TEST_ASSERT_EQUAL_INT(KIN_REL_ANALOG, r.type);
}

/* --- Exhaustive coverage: all 260 kins have valid relationship with kin 1 --- */

void test_relationship_all_kins_valid(void) {
    for (int k = 1; k <= 260; k++) {
        kin_relationship_t r = kin_relationship(1, k);
        TEST_ASSERT_TRUE(r.type >= KIN_REL_NONE && r.type < KIN_REL_TYPE_COUNT);
        TEST_ASSERT_NOT_NULL(r.description);
    }
}

/* --- Oracle positions distinguish guide from hidden --- */
/* In dreamspell.c, occult == hidden == 261 - kin.
 * But for kin_relationship, the guide is a separate oracle position.
 * Since occult == hidden in the oracle struct, KIN_REL_OCCULT takes precedence
 * and KIN_REL_HIDDEN is only used when we detect it separately.
 * Actually, since occult and hidden are the same kin, the oracle check for
 * occult will fire first, so KIN_REL_HIDDEN is redundant here.
 * We'll test that occult/hidden detection works via the occult path. */
void test_relationship_occult_equals_hidden_in_oracle(void) {
    dreamspell_oracle_t o = dreamspell_oracle(44);
    TEST_ASSERT_EQUAL_INT(o.occult, o.hidden);
    kin_relationship_t r = kin_relationship(44, o.occult);
    /* Should be OCCULT (checked first) */
    TEST_ASSERT_EQUAL_INT(KIN_REL_OCCULT, r.type);
}

int main(void) {
    UNITY_BEGIN();

    /* SAME */
    RUN_TEST(test_relationship_same_kin);
    RUN_TEST(test_relationship_same_kin260);
    RUN_TEST(test_relationship_same_kin130);

    /* ANALOG */
    RUN_TEST(test_relationship_analog_kin1);
    RUN_TEST(test_relationship_analog_kin44);
    RUN_TEST(test_relationship_analog_symmetric);

    /* ANTIPODE */
    RUN_TEST(test_relationship_antipode_kin1);
    RUN_TEST(test_relationship_antipode_symmetric);

    /* OCCULT */
    RUN_TEST(test_relationship_occult_kin1);
    RUN_TEST(test_relationship_occult_symmetric);

    /* GUIDE */
    RUN_TEST(test_relationship_guide_kin2);
    RUN_TEST(test_relationship_guide_self_is_same);

    /* WAVESPELL */
    RUN_TEST(test_relationship_wavespell_kin1_kin2);
    RUN_TEST(test_relationship_not_wavespell_across_boundary);

    /* SEAL_TRIBE */
    RUN_TEST(test_relationship_seal_tribe);

    /* COLOR_FAMILY */
    RUN_TEST(test_relationship_color_family);

    /* EARTH_FAMILY */
    RUN_TEST(test_relationship_earth_family);

    /* NONE */
    RUN_TEST(test_relationship_none);

    /* Invalid inputs */
    RUN_TEST(test_relationship_invalid_zero);
    RUN_TEST(test_relationship_invalid_261);
    RUN_TEST(test_relationship_invalid_negative);

    /* Shared flags */
    RUN_TEST(test_relationship_shared_seal_flag);
    RUN_TEST(test_relationship_shared_wavespell_flag);
    RUN_TEST(test_relationship_shared_color_flag);
    RUN_TEST(test_relationship_shared_earth_family_flag);
    RUN_TEST(test_relationship_no_shared_flags);
    RUN_TEST(test_relationship_description_not_null);

    /* Tribe members */
    RUN_TEST(test_tribe_seal0_dragon);
    RUN_TEST(test_tribe_seal19_sun);
    RUN_TEST(test_tribe_seal10_monkey);
    RUN_TEST(test_tribe_invalid_seal);
    RUN_TEST(test_tribe_small_buffer);

    /* Wavespell members */
    RUN_TEST(test_wavespell_members_ws1);
    RUN_TEST(test_wavespell_members_ws20);
    RUN_TEST(test_wavespell_members_invalid);
    RUN_TEST(test_wavespell_members_small_buffer);

    /* Color family */
    RUN_TEST(test_color_family_red);
    RUN_TEST(test_color_family_yellow);
    RUN_TEST(test_color_family_invalid);
    RUN_TEST(test_color_family_small_buffer);

    /* Earth family members */
    RUN_TEST(test_earth_family_cardinal);
    RUN_TEST(test_earth_family_polar);
    RUN_TEST(test_earth_family_invalid);
    RUN_TEST(test_earth_family_small_buffer);

    /* Compatibility score */
    RUN_TEST(test_compatibility_same);
    RUN_TEST(test_compatibility_analog);
    RUN_TEST(test_compatibility_antipode);
    RUN_TEST(test_compatibility_occult);
    RUN_TEST(test_compatibility_none);
    RUN_TEST(test_compatibility_invalid);

    /* Rel type name */
    RUN_TEST(test_rel_type_name_none);
    RUN_TEST(test_rel_type_name_same);
    RUN_TEST(test_rel_type_name_analog);
    RUN_TEST(test_rel_type_name_antipode);
    RUN_TEST(test_rel_type_name_occult);
    RUN_TEST(test_rel_type_name_guide);
    RUN_TEST(test_rel_type_name_hidden);
    RUN_TEST(test_rel_type_name_wavespell);
    RUN_TEST(test_rel_type_name_seal_tribe);
    RUN_TEST(test_rel_type_name_color_family);
    RUN_TEST(test_rel_type_name_earth_family);
    RUN_TEST(test_rel_type_name_invalid);

    /* Shared groups */
    RUN_TEST(test_shared_groups_same_kin);
    RUN_TEST(test_shared_groups_none);
    RUN_TEST(test_shared_groups_same_seal_different_ws);
    RUN_TEST(test_shared_groups_same_ws_only);
    RUN_TEST(test_shared_groups_invalid);

    /* Priority */
    RUN_TEST(test_relationship_priority_oracle_over_tribe);

    /* Exhaustive */
    RUN_TEST(test_relationship_all_kins_valid);

    /* Occult equals hidden */
    RUN_TEST(test_relationship_occult_equals_hidden_in_oracle);

    return UNITY_END();
}
