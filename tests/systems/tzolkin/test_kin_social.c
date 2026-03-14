#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/kin_social.h"
#include "../../../src/systems/tzolkin/dreamspell.h"

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * Kin Social Compatibility — Dreamspell kin relationship tests
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
 * Castle: (kin-1)/52 + 1 (1-5), 52 consecutive kins each
 *
 * Oracle strengths: Guide=95, Analog=90, Occult=85, Hidden=85,
 *                   Antipode=80, Same=100
 * Social bonuses: Wavespell +10, Tribe +8, Earth Family +7,
 *                 Color +5, Castle +5
 * Total capped at 100
 * ============================================================ */

/* --- ks_match: Same Kin --- */

void test_match_same_kin_1(void) {
    ks_match_t m = ks_match(1, 1);
    TEST_ASSERT_TRUE(m.count >= 1);
    /* First item should be SAME_KIN */
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_SAME_KIN) {
            found = 1;
            TEST_ASSERT_EQUAL_INT(100, m.items[i].strength);
        }
    }
    TEST_ASSERT_TRUE(found);
    TEST_ASSERT_EQUAL_INT(100, m.total_score);
}

void test_match_same_kin_260(void) {
    ks_match_t m = ks_match(260, 260);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_SAME_KIN) found = 1;
    }
    TEST_ASSERT_TRUE(found);
    TEST_ASSERT_EQUAL_INT(100, m.total_score);
}

void test_match_same_kin_130(void) {
    ks_match_t m = ks_match(130, 130);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_SAME_KIN) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

/* --- ks_match: Analog oracle relationship --- */

void test_match_analog_kin1(void) {
    /* Kin 1 analog = kin 40 */
    dreamspell_oracle_t o = dreamspell_oracle(1);
    ks_match_t m = ks_match(1, o.analog);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_ANALOG) {
            found = 1;
            TEST_ASSERT_EQUAL_INT(90, m.items[i].strength);
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_match_analog_kin44(void) {
    dreamspell_oracle_t o = dreamspell_oracle(44);
    ks_match_t m = ks_match(44, o.analog);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_ANALOG) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

void test_match_analog_symmetric(void) {
    /* Analog relationship is symmetric */
    dreamspell_oracle_t o = dreamspell_oracle(1);
    ks_match_t m1 = ks_match(1, o.analog);
    ks_match_t m2 = ks_match(o.analog, 1);
    int found1 = 0, found2 = 0;
    for (int i = 0; i < m1.count; i++)
        if (m1.items[i].type == KS_REL_ANALOG) found1 = 1;
    for (int i = 0; i < m2.count; i++)
        if (m2.items[i].type == KS_REL_ANALOG) found2 = 1;
    TEST_ASSERT_TRUE(found1);
    TEST_ASSERT_TRUE(found2);
}

/* --- ks_match: Antipode oracle relationship --- */

void test_match_antipode_kin1(void) {
    /* Kin 1 antipode = kin 131 */
    dreamspell_oracle_t o = dreamspell_oracle(1);
    ks_match_t m = ks_match(1, o.antipode);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_ANTIPODE) {
            found = 1;
            TEST_ASSERT_EQUAL_INT(80, m.items[i].strength);
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_match_antipode_symmetric(void) {
    dreamspell_oracle_t o = dreamspell_oracle(50);
    ks_match_t m1 = ks_match(50, o.antipode);
    ks_match_t m2 = ks_match(o.antipode, 50);
    int f1 = 0, f2 = 0;
    for (int i = 0; i < m1.count; i++)
        if (m1.items[i].type == KS_REL_ANTIPODE) f1 = 1;
    for (int i = 0; i < m2.count; i++)
        if (m2.items[i].type == KS_REL_ANTIPODE) f2 = 1;
    TEST_ASSERT_TRUE(f1);
    TEST_ASSERT_TRUE(f2);
}

/* --- ks_match: Occult oracle relationship --- */

void test_match_occult_kin1(void) {
    /* Kin 1 occult = kin 260 (261-1) */
    dreamspell_oracle_t o = dreamspell_oracle(1);
    ks_match_t m = ks_match(1, o.occult);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_OCCULT) {
            found = 1;
            TEST_ASSERT_EQUAL_INT(85, m.items[i].strength);
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_match_occult_symmetric(void) {
    /* 261 - (261 - k) = k, so occult is always symmetric */
    dreamspell_oracle_t o = dreamspell_oracle(100);
    ks_match_t m1 = ks_match(100, o.occult);
    ks_match_t m2 = ks_match(o.occult, 100);
    int f1 = 0, f2 = 0;
    for (int i = 0; i < m1.count; i++)
        if (m1.items[i].type == KS_REL_OCCULT) f1 = 1;
    for (int i = 0; i < m2.count; i++)
        if (m2.items[i].type == KS_REL_OCCULT) f2 = 1;
    TEST_ASSERT_TRUE(f1);
    TEST_ASSERT_TRUE(f2);
}

/* --- ks_match: Guide oracle relationship --- */

void test_match_guide_kin2(void) {
    /* Kin 2 guide = kin 54 (tone 2, guide offset 12) */
    dreamspell_oracle_t o = dreamspell_oracle(2);
    /* Ensure guide differs from other oracle positions */
    TEST_ASSERT_TRUE(o.guide != o.analog);
    TEST_ASSERT_TRUE(o.guide != o.antipode);
    TEST_ASSERT_TRUE(o.guide != o.occult);
    TEST_ASSERT_TRUE(o.guide != 2);  /* not self-guiding */
    ks_match_t m = ks_match(2, o.guide);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_GUIDE) {
            found = 1;
            TEST_ASSERT_EQUAL_INT(95, m.items[i].strength);
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* Self-guiding kins (tone 1,6,11): guide==destiny, so same_kin fires instead */
void test_match_guide_self_is_same_kin(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(1, o.guide); /* tone 1 self-guides */
    ks_match_t m = ks_match(1, 1);
    int found_same = 0, found_guide = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_SAME_KIN) found_same = 1;
        if (m.items[i].type == KS_REL_GUIDE) found_guide = 1;
    }
    TEST_ASSERT_TRUE(found_same);
    /* Guide should not appear separately when it equals self */
    TEST_ASSERT_FALSE(found_guide);
}

/* --- ks_match: Hidden teacher --- */

void test_match_hidden_kin44(void) {
    /* In dreamspell, occult == hidden == 261-kin.
     * Since both are the same kin number, ks_match should detect
     * occult (or hidden). They overlap, so we just check one fires. */
    dreamspell_oracle_t o = dreamspell_oracle(44);
    TEST_ASSERT_EQUAL_INT(o.occult, o.hidden);
    ks_match_t m = ks_match(44, o.hidden);
    int found_occult = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_OCCULT) found_occult = 1;
    }
    TEST_ASSERT_TRUE(found_occult);
}

/* --- ks_match: Social groupings — wavespell --- */

void test_match_wavespell_kin1_kin5(void) {
    /* Kin 1 and kin 5: same wavespell (WS 1, kins 1-13) */
    /* Kin 5 is not an oracle partner of kin 1 */
    ks_match_t m = ks_match(1, 5);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_WAVESPELL) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

void test_match_not_wavespell_across_boundary(void) {
    /* Kin 13 (WS 1) and kin 14 (WS 2) */
    ks_match_t m = ks_match(13, 14);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_WAVESPELL) found = 1;
    }
    TEST_ASSERT_FALSE(found);
}

/* --- ks_match: Social groupings — tribe --- */

void test_match_seal_tribe(void) {
    /* Seal 0 (Dragon): kins 1, 21, 41, ...
     * Kin 1 and kin 21: same seal, different WS
     * kin 21 is not oracle partner of kin 1 (analog=40, antipode=131, occult=260) */
    ks_match_t m = ks_match(1, 21);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_TRIBE) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

/* --- ks_match: Social groupings — color family --- */

void test_match_color_family(void) {
    /* Kin 1 seal=0 (Red), kin 85 seal=4 (Serpent, Red: 4%4=0)
     * Different seal, different WS, different earth family.
     * Not oracle partners. Same color. */
    ks_match_t m = ks_match(1, 85);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_COLOR) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

/* --- ks_match: Social groupings — earth family --- */

void test_match_earth_family(void) {
    /* Kin 1 seal=0 (Cardinal, seal%5=0), kin 46 seal=5 (WB, seal%5=0)
     * Same earth family. Different seal, different WS, different color. */
    ks_match_t m = ks_match(1, 46);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_EARTH_FAMILY) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

/* --- ks_match: Social groupings — castle --- */

void test_match_castle_same(void) {
    /* Kin 1 and kin 52: both in castle 1 (kins 1-52) */
    ks_match_t m = ks_match(1, 52);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_CASTLE) found = 1;
    }
    TEST_ASSERT_TRUE(found);
}

void test_match_castle_different(void) {
    /* Kin 1 (castle 1) and kin 53 (castle 2) */
    ks_match_t m = ks_match(1, 53);
    int found = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_CASTLE) found = 1;
    }
    TEST_ASSERT_FALSE(found);
}

/* --- ks_match: No relationship (NONE) --- */

void test_match_none(void) {
    /* Kin 1 (seal 0, Red, Cardinal, WS 1, Castle 1)
     * Kin 53 (seal=(53-1)%20=12, Blue, Signal, WS 5, Castle 2)
     * Not oracle partners. No shared groups. */
    /* But wait — check castle: kin 53 is castle 2. Different.
     * Color: seal 0 %4=0, seal 12 %4=0. Same color (Red)!
     * So kin 53 DOES share color with kin 1. Pick a different kin. */
    /* Kin 42: seal=(42-1)%20=1 (Wind, White, Core, WS 4, Castle 1)
     * Oracle of 1: analog=40, antipode=131, occult=260, guide=1.
     * Same castle? kin 42 castle = (42-1)/52+1=1. Same castle!
     * Pick another: kin 62: seal=(62-1)%20=1 (Wind, White, Core, WS 5, Castle 2)
     * Color: 1%4=1. Kin 1 color: 0%4=0. Different.
     * Earth family: 1%5=1. Kin 1 ef: 0%5=0. Different.
     * Castle: (62-1)/52+1=2. Kin 1 castle=1. Different.
     * WS: (62-1)/13+1=5. Kin 1 WS=1. Different.
     * Oracle 1: 40,131,260,1 — 62 not in those. Good. */
    ks_match_t m = ks_match(1, 62);
    TEST_ASSERT_EQUAL_INT(0, m.count);
    TEST_ASSERT_EQUAL_INT(0, m.total_score);
}

/* --- ks_match: Multiple overlapping relationships --- */

void test_match_multiple_relationships(void) {
    /* Kin 1 and kin 21: same seal (0), same color (Red), same earth family (Cardinal),
     * same castle (both castle 1). Not oracle partners.
     * Should have TRIBE + COLOR + EARTH_FAMILY + CASTLE = 4 groupings. */
    ks_match_t m = ks_match(1, 21);
    TEST_ASSERT_TRUE(m.count >= 2);  /* at least tribe + one more */
    int has_tribe = 0, has_color = 0, has_earth = 0, has_castle = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_TRIBE) has_tribe = 1;
        if (m.items[i].type == KS_REL_COLOR) has_color = 1;
        if (m.items[i].type == KS_REL_EARTH_FAMILY) has_earth = 1;
        if (m.items[i].type == KS_REL_CASTLE) has_castle = 1;
    }
    TEST_ASSERT_TRUE(has_tribe);
    TEST_ASSERT_TRUE(has_color);
    TEST_ASSERT_TRUE(has_earth);
    TEST_ASSERT_TRUE(has_castle);
}

void test_match_oracle_plus_social(void) {
    /* Kin 1 analog = kin 40. Check if social groupings also appear.
     * Kin 1: seal=0, WS=1, Castle=1. Kin 40: seal=19, WS=4, Castle=1.
     * Same castle? (40-1)/52+1 = 1. Yes! Same color? 0%4=0, 19%4=3. No.
     * Same earth family? 0%5=0, 19%5=4. No. Same seal? 0!=19. No. */
    dreamspell_oracle_t o = dreamspell_oracle(1);
    ks_match_t m = ks_match(1, o.analog);
    int has_analog = 0, has_castle = 0;
    for (int i = 0; i < m.count; i++) {
        if (m.items[i].type == KS_REL_ANALOG) has_analog = 1;
        if (m.items[i].type == KS_REL_CASTLE) has_castle = 1;
    }
    TEST_ASSERT_TRUE(has_analog);
    TEST_ASSERT_TRUE(has_castle);
}

/* --- ks_match: Invalid inputs --- */

void test_match_invalid_zero(void) {
    ks_match_t m = ks_match(0, 1);
    TEST_ASSERT_EQUAL_INT(0, m.count);
    TEST_ASSERT_EQUAL_INT(0, m.total_score);
}

void test_match_invalid_261(void) {
    ks_match_t m = ks_match(1, 261);
    TEST_ASSERT_EQUAL_INT(0, m.count);
    TEST_ASSERT_EQUAL_INT(0, m.total_score);
}

void test_match_invalid_negative(void) {
    ks_match_t m = ks_match(-5, 100);
    TEST_ASSERT_EQUAL_INT(0, m.count);
    TEST_ASSERT_EQUAL_INT(0, m.total_score);
}

void test_match_both_invalid(void) {
    ks_match_t m = ks_match(0, 261);
    TEST_ASSERT_EQUAL_INT(0, m.count);
    TEST_ASSERT_EQUAL_INT(0, m.total_score);
}

/* --- ks_match: Score ordering: analog > tribe > color > castle --- */

void test_score_ordering_analog_gt_tribe(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    ks_match_t m_analog = ks_match(1, o.analog);
    /* Kin 1 tribe partner (non-oracle): kin 21 */
    ks_match_t m_tribe = ks_match(1, 21);
    TEST_ASSERT_TRUE(m_analog.total_score > m_tribe.total_score);
}

void test_score_ordering_tribe_gt_color(void) {
    /* Kin 1 tribe partner: kin 21 (same seal, same color, same ef, same castle)
     * Kin 1 color-only partner: kin 85 (seal 4, color Red, ef Polar, castle 2)
     * Actually kin 85 may also have castle overlap. Let me check:
     * kin 85 castle = (85-1)/52+1 = 84/52+1 = 1+1 = 2. Different from kin 1 castle=1. */
    ks_match_t m_tribe = ks_match(1, 21);
    ks_match_t m_color = ks_match(1, 85);
    TEST_ASSERT_TRUE(m_tribe.total_score > m_color.total_score);
}

void test_score_ordering_color_gt_castle(void) {
    /* Find a castle-only pair. Kin 1 castle=1 (kins 1-52).
     * Need a kin in 1-52 with no oracle, no shared seal/color/ef/ws with kin 1.
     * Kin 42: seal=1, color=1, ef=1, WS=4. All different from kin 1.
     * Castle=(42-1)/52+1=1. Same castle!
     * Oracle of 1: analog=40, antipode=131, occult=260, guide=1. 42 not in those. Good. */
    ks_match_t m_color = ks_match(1, 85);
    ks_match_t m_castle = ks_match(1, 42);
    TEST_ASSERT_TRUE(m_color.total_score > m_castle.total_score);
}

/* --- ks_match: Total score capped at 100 --- */

void test_match_total_score_capped(void) {
    /* Same kin always = 100 */
    ks_match_t m = ks_match(1, 1);
    TEST_ASSERT_TRUE(m.total_score <= 100);
}

/* --- ks_match: All items have non-null names and descriptions --- */

void test_match_items_have_names(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    ks_match_t m = ks_match(1, o.analog);
    for (int i = 0; i < m.count; i++) {
        TEST_ASSERT_NOT_NULL(m.items[i].name);
        TEST_ASSERT_NOT_NULL(m.items[i].description);
    }
}

/* --- ks_match: Exhaustive — all 260 kins produce valid results with kin 1 --- */

void test_match_all_kins_valid(void) {
    for (int k = 1; k <= 260; k++) {
        ks_match_t m = ks_match(1, k);
        TEST_ASSERT_TRUE(m.count >= 0 && m.count <= KS_MAX_RELATIONSHIPS);
        TEST_ASSERT_TRUE(m.total_score >= 0 && m.total_score <= 100);
        for (int i = 0; i < m.count; i++) {
            TEST_ASSERT_NOT_NULL(m.items[i].name);
            TEST_ASSERT_NOT_NULL(m.items[i].description);
            TEST_ASSERT_TRUE(m.items[i].strength >= 0 && m.items[i].strength <= 100);
        }
    }
}

/* --- ks_tribe_members --- */

void test_tribe_seal0_dragon(void) {
    int out[13];
    int count = ks_tribe_members(0, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(1, out[0]);
    TEST_ASSERT_EQUAL_INT(21, out[1]);
    TEST_ASSERT_EQUAL_INT(41, out[2]);
    TEST_ASSERT_EQUAL_INT(241, out[12]);
}

void test_tribe_seal19_sun(void) {
    int out[13];
    int count = ks_tribe_members(19, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(20, out[0]);
    TEST_ASSERT_EQUAL_INT(40, out[1]);
    TEST_ASSERT_EQUAL_INT(260, out[12]);
}

void test_tribe_seal10_monkey(void) {
    int out[13];
    int count = ks_tribe_members(10, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(11, out[0]);
    TEST_ASSERT_EQUAL_INT(31, out[1]);
    TEST_ASSERT_EQUAL_INT(251, out[12]);
}

void test_tribe_invalid_seal_negative(void) {
    int out[13];
    TEST_ASSERT_EQUAL_INT(0, ks_tribe_members(-1, out, 13));
}

void test_tribe_invalid_seal_20(void) {
    int out[13];
    TEST_ASSERT_EQUAL_INT(0, ks_tribe_members(20, out, 13));
}

void test_tribe_small_buffer(void) {
    int out[3];
    int count = ks_tribe_members(0, out, 3);
    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_INT(1, out[0]);
    TEST_ASSERT_EQUAL_INT(21, out[1]);
    TEST_ASSERT_EQUAL_INT(41, out[2]);
}

/* --- ks_wavespell_members --- */

void test_wavespell_members_ws1(void) {
    int out[13];
    int count = ks_wavespell_members(1, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(1, out[0]);
    TEST_ASSERT_EQUAL_INT(13, out[12]);
}

void test_wavespell_members_ws20(void) {
    int out[13];
    int count = ks_wavespell_members(20, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(248, out[0]);
    TEST_ASSERT_EQUAL_INT(260, out[12]);
}

void test_wavespell_members_ws10(void) {
    int out[13];
    int count = ks_wavespell_members(10, out, 13);
    TEST_ASSERT_EQUAL_INT(13, count);
    TEST_ASSERT_EQUAL_INT(118, out[0]);  /* (10-1)*13+1=118 */
    TEST_ASSERT_EQUAL_INT(130, out[12]);
}

void test_wavespell_members_invalid_0(void) {
    int out[13];
    TEST_ASSERT_EQUAL_INT(0, ks_wavespell_members(0, out, 13));
}

void test_wavespell_members_invalid_21(void) {
    int out[13];
    TEST_ASSERT_EQUAL_INT(0, ks_wavespell_members(21, out, 13));
}

void test_wavespell_members_small_buffer(void) {
    int out[5];
    int count = ks_wavespell_members(1, out, 5);
    TEST_ASSERT_EQUAL_INT(5, count);
    TEST_ASSERT_EQUAL_INT(1, out[0]);
    TEST_ASSERT_EQUAL_INT(5, out[4]);
}

/* --- ks_color_family_members --- */

void test_color_family_red(void) {
    /* Color 0 (Red): seals 0,4,8,12,16 — each has 13 kins = 65 total */
    int out[65];
    int count = ks_color_family_members(0, out, 65);
    TEST_ASSERT_EQUAL_INT(65, count);
    TEST_ASSERT_EQUAL_INT(1, out[0]);  /* seal 0, first kin */
}

void test_color_family_white(void) {
    int out[65];
    int count = ks_color_family_members(1, out, 65);
    TEST_ASSERT_EQUAL_INT(65, count);
}

void test_color_family_blue(void) {
    int out[65];
    int count = ks_color_family_members(2, out, 65);
    TEST_ASSERT_EQUAL_INT(65, count);
}

void test_color_family_yellow(void) {
    int out[65];
    int count = ks_color_family_members(3, out, 65);
    TEST_ASSERT_EQUAL_INT(65, count);
}

void test_color_family_invalid_negative(void) {
    int out[65];
    TEST_ASSERT_EQUAL_INT(0, ks_color_family_members(-1, out, 65));
}

void test_color_family_invalid_4(void) {
    int out[65];
    TEST_ASSERT_EQUAL_INT(0, ks_color_family_members(4, out, 65));
}

void test_color_family_small_buffer(void) {
    int out[10];
    int count = ks_color_family_members(0, out, 10);
    TEST_ASSERT_EQUAL_INT(10, count);
}

/* --- ks_same_wavespell --- */

void test_same_wavespell_true(void) {
    TEST_ASSERT_EQUAL_INT(1, ks_same_wavespell(1, 13));  /* both WS 1 */
}

void test_same_wavespell_false(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_same_wavespell(13, 14)); /* WS 1 vs 2 */
}

void test_same_wavespell_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_same_wavespell(0, 1));
}

/* --- ks_same_tribe --- */

void test_same_tribe_true(void) {
    TEST_ASSERT_EQUAL_INT(1, ks_same_tribe(1, 21));  /* both seal 0 */
}

void test_same_tribe_false(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_same_tribe(1, 2));   /* seal 0 vs 1 */
}

void test_same_tribe_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_same_tribe(0, 1));
}

/* --- ks_same_color --- */

void test_same_color_true(void) {
    /* Kin 1 seal=0 (Red), kin 85 seal=4 (Red, 4%4=0) */
    TEST_ASSERT_EQUAL_INT(1, ks_same_color(1, 85));
}

void test_same_color_false(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_same_color(1, 2)); /* seal 0 Red vs seal 1 White */
}

void test_same_color_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_same_color(0, 1));
}

/* --- ks_same_earth_family --- */

void test_same_earth_family_true(void) {
    /* Kin 1 seal=0 (Cardinal, 0%5=0), kin 46 seal=5 (WB, 5%5=0) */
    TEST_ASSERT_EQUAL_INT(1, ks_same_earth_family(1, 46));
}

void test_same_earth_family_false(void) {
    /* Kin 1 seal=0 (0%5=0), kin 2 seal=1 (1%5=1) */
    TEST_ASSERT_EQUAL_INT(0, ks_same_earth_family(1, 2));
}

void test_same_earth_family_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_same_earth_family(0, 1));
}

/* --- ks_same_castle --- */

void test_same_castle_true(void) {
    TEST_ASSERT_EQUAL_INT(1, ks_same_castle(1, 52));   /* both castle 1 */
}

void test_same_castle_false(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_same_castle(1, 53));   /* castle 1 vs 2 */
}

void test_same_castle_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_same_castle(0, 1));
}

/* --- ks_seal_from_kin --- */

void test_seal_from_kin_1(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_seal_from_kin(1));   /* Dragon */
}

void test_seal_from_kin_20(void) {
    TEST_ASSERT_EQUAL_INT(19, ks_seal_from_kin(20));  /* Sun */
}

void test_seal_from_kin_21(void) {
    TEST_ASSERT_EQUAL_INT(0, ks_seal_from_kin(21));   /* Dragon again */
}

void test_seal_from_kin_260(void) {
    TEST_ASSERT_EQUAL_INT(19, ks_seal_from_kin(260)); /* Sun */
}

void test_seal_from_kin_invalid_0(void) {
    TEST_ASSERT_EQUAL_INT(-1, ks_seal_from_kin(0));
}

void test_seal_from_kin_invalid_261(void) {
    TEST_ASSERT_EQUAL_INT(-1, ks_seal_from_kin(261));
}

void test_seal_from_kin_invalid_negative(void) {
    TEST_ASSERT_EQUAL_INT(-1, ks_seal_from_kin(-1));
}

/* --- ks_color_from_kin --- */

void test_color_from_kin_1(void) {
    /* Seal 0, color = 0%4 = 0 (Red) */
    TEST_ASSERT_EQUAL_INT(0, ks_color_from_kin(1));
}

void test_color_from_kin_2(void) {
    /* Seal 1, color = 1%4 = 1 (White) */
    TEST_ASSERT_EQUAL_INT(1, ks_color_from_kin(2));
}

void test_color_from_kin_3(void) {
    /* Seal 2, color = 2%4 = 2 (Blue) */
    TEST_ASSERT_EQUAL_INT(2, ks_color_from_kin(3));
}

void test_color_from_kin_4(void) {
    /* Seal 3, color = 3%4 = 3 (Yellow) */
    TEST_ASSERT_EQUAL_INT(3, ks_color_from_kin(4));
}

void test_color_from_kin_invalid(void) {
    TEST_ASSERT_EQUAL_INT(-1, ks_color_from_kin(0));
    TEST_ASSERT_EQUAL_INT(-1, ks_color_from_kin(261));
}

/* --- ks_relationship_name --- */

void test_rel_name_none(void) {
    TEST_ASSERT_EQUAL_STRING("None", ks_relationship_name(KS_REL_NONE));
}

void test_rel_name_analog(void) {
    TEST_ASSERT_EQUAL_STRING("Analog", ks_relationship_name(KS_REL_ANALOG));
}

void test_rel_name_antipode(void) {
    TEST_ASSERT_EQUAL_STRING("Antipode", ks_relationship_name(KS_REL_ANTIPODE));
}

void test_rel_name_occult(void) {
    TEST_ASSERT_EQUAL_STRING("Occult", ks_relationship_name(KS_REL_OCCULT));
}

void test_rel_name_guide(void) {
    TEST_ASSERT_EQUAL_STRING("Guide", ks_relationship_name(KS_REL_GUIDE));
}

void test_rel_name_hidden(void) {
    TEST_ASSERT_EQUAL_STRING("Hidden Teacher", ks_relationship_name(KS_REL_HIDDEN));
}

void test_rel_name_same_kin(void) {
    TEST_ASSERT_EQUAL_STRING("Same Kin", ks_relationship_name(KS_REL_SAME_KIN));
}

void test_rel_name_wavespell(void) {
    TEST_ASSERT_EQUAL_STRING("Wavespell", ks_relationship_name(KS_REL_WAVESPELL));
}

void test_rel_name_tribe(void) {
    TEST_ASSERT_EQUAL_STRING("Tribe", ks_relationship_name(KS_REL_TRIBE));
}

void test_rel_name_color(void) {
    TEST_ASSERT_EQUAL_STRING("Color Family", ks_relationship_name(KS_REL_COLOR));
}

void test_rel_name_earth_family(void) {
    TEST_ASSERT_EQUAL_STRING("Earth Family", ks_relationship_name(KS_REL_EARTH_FAMILY));
}

void test_rel_name_castle(void) {
    TEST_ASSERT_EQUAL_STRING("Castle", ks_relationship_name(KS_REL_CASTLE));
}

void test_rel_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", ks_relationship_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", ks_relationship_name(99));
}

/* --- ks_relationship_meaning --- */

void test_rel_meaning_analog(void) {
    const char *m = ks_relationship_meaning(KS_REL_ANALOG);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(m[0] != '?');
}

void test_rel_meaning_none(void) {
    const char *m = ks_relationship_meaning(KS_REL_NONE);
    TEST_ASSERT_NOT_NULL(m);
}

void test_rel_meaning_all_types_non_null(void) {
    for (int i = 0; i <= KS_REL_CASTLE; i++) {
        const char *m = ks_relationship_meaning((ks_relationship_type_t)i);
        TEST_ASSERT_NOT_NULL(m);
    }
}

void test_rel_meaning_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", ks_relationship_meaning(-1));
    TEST_ASSERT_EQUAL_STRING("?", ks_relationship_meaning(99));
}

/* --- ks_relationship_type_count --- */

void test_relationship_type_count(void) {
    /* 11 types: ANALOG, ANTIPODE, OCCULT, GUIDE, HIDDEN, SAME_KIN,
     * WAVESPELL, TRIBE, COLOR, EARTH_FAMILY, CASTLE */
    TEST_ASSERT_EQUAL_INT(11, ks_relationship_type_count());
}

int main(void) {
    UNITY_BEGIN();

    /* SAME KIN */
    RUN_TEST(test_match_same_kin_1);
    RUN_TEST(test_match_same_kin_260);
    RUN_TEST(test_match_same_kin_130);

    /* ANALOG */
    RUN_TEST(test_match_analog_kin1);
    RUN_TEST(test_match_analog_kin44);
    RUN_TEST(test_match_analog_symmetric);

    /* ANTIPODE */
    RUN_TEST(test_match_antipode_kin1);
    RUN_TEST(test_match_antipode_symmetric);

    /* OCCULT */
    RUN_TEST(test_match_occult_kin1);
    RUN_TEST(test_match_occult_symmetric);

    /* GUIDE */
    RUN_TEST(test_match_guide_kin2);
    RUN_TEST(test_match_guide_self_is_same_kin);

    /* HIDDEN */
    RUN_TEST(test_match_hidden_kin44);

    /* WAVESPELL */
    RUN_TEST(test_match_wavespell_kin1_kin5);
    RUN_TEST(test_match_not_wavespell_across_boundary);

    /* TRIBE */
    RUN_TEST(test_match_seal_tribe);

    /* COLOR */
    RUN_TEST(test_match_color_family);

    /* EARTH FAMILY */
    RUN_TEST(test_match_earth_family);

    /* CASTLE */
    RUN_TEST(test_match_castle_same);
    RUN_TEST(test_match_castle_different);

    /* NONE */
    RUN_TEST(test_match_none);

    /* MULTIPLE OVERLAPPING */
    RUN_TEST(test_match_multiple_relationships);
    RUN_TEST(test_match_oracle_plus_social);

    /* INVALID INPUTS */
    RUN_TEST(test_match_invalid_zero);
    RUN_TEST(test_match_invalid_261);
    RUN_TEST(test_match_invalid_negative);
    RUN_TEST(test_match_both_invalid);

    /* SCORE ORDERING */
    RUN_TEST(test_score_ordering_analog_gt_tribe);
    RUN_TEST(test_score_ordering_tribe_gt_color);
    RUN_TEST(test_score_ordering_color_gt_castle);
    RUN_TEST(test_match_total_score_capped);
    RUN_TEST(test_match_items_have_names);

    /* EXHAUSTIVE */
    RUN_TEST(test_match_all_kins_valid);

    /* TRIBE MEMBERS */
    RUN_TEST(test_tribe_seal0_dragon);
    RUN_TEST(test_tribe_seal19_sun);
    RUN_TEST(test_tribe_seal10_monkey);
    RUN_TEST(test_tribe_invalid_seal_negative);
    RUN_TEST(test_tribe_invalid_seal_20);
    RUN_TEST(test_tribe_small_buffer);

    /* WAVESPELL MEMBERS */
    RUN_TEST(test_wavespell_members_ws1);
    RUN_TEST(test_wavespell_members_ws20);
    RUN_TEST(test_wavespell_members_ws10);
    RUN_TEST(test_wavespell_members_invalid_0);
    RUN_TEST(test_wavespell_members_invalid_21);
    RUN_TEST(test_wavespell_members_small_buffer);

    /* COLOR FAMILY MEMBERS */
    RUN_TEST(test_color_family_red);
    RUN_TEST(test_color_family_white);
    RUN_TEST(test_color_family_blue);
    RUN_TEST(test_color_family_yellow);
    RUN_TEST(test_color_family_invalid_negative);
    RUN_TEST(test_color_family_invalid_4);
    RUN_TEST(test_color_family_small_buffer);

    /* SAME_* PREDICATE FUNCTIONS */
    RUN_TEST(test_same_wavespell_true);
    RUN_TEST(test_same_wavespell_false);
    RUN_TEST(test_same_wavespell_invalid);
    RUN_TEST(test_same_tribe_true);
    RUN_TEST(test_same_tribe_false);
    RUN_TEST(test_same_tribe_invalid);
    RUN_TEST(test_same_color_true);
    RUN_TEST(test_same_color_false);
    RUN_TEST(test_same_color_invalid);
    RUN_TEST(test_same_earth_family_true);
    RUN_TEST(test_same_earth_family_false);
    RUN_TEST(test_same_earth_family_invalid);
    RUN_TEST(test_same_castle_true);
    RUN_TEST(test_same_castle_false);
    RUN_TEST(test_same_castle_invalid);

    /* SEAL/COLOR FROM KIN */
    RUN_TEST(test_seal_from_kin_1);
    RUN_TEST(test_seal_from_kin_20);
    RUN_TEST(test_seal_from_kin_21);
    RUN_TEST(test_seal_from_kin_260);
    RUN_TEST(test_seal_from_kin_invalid_0);
    RUN_TEST(test_seal_from_kin_invalid_261);
    RUN_TEST(test_seal_from_kin_invalid_negative);
    RUN_TEST(test_color_from_kin_1);
    RUN_TEST(test_color_from_kin_2);
    RUN_TEST(test_color_from_kin_3);
    RUN_TEST(test_color_from_kin_4);
    RUN_TEST(test_color_from_kin_invalid);

    /* RELATIONSHIP NAME */
    RUN_TEST(test_rel_name_none);
    RUN_TEST(test_rel_name_analog);
    RUN_TEST(test_rel_name_antipode);
    RUN_TEST(test_rel_name_occult);
    RUN_TEST(test_rel_name_guide);
    RUN_TEST(test_rel_name_hidden);
    RUN_TEST(test_rel_name_same_kin);
    RUN_TEST(test_rel_name_wavespell);
    RUN_TEST(test_rel_name_tribe);
    RUN_TEST(test_rel_name_color);
    RUN_TEST(test_rel_name_earth_family);
    RUN_TEST(test_rel_name_castle);
    RUN_TEST(test_rel_name_invalid);

    /* RELATIONSHIP MEANING */
    RUN_TEST(test_rel_meaning_analog);
    RUN_TEST(test_rel_meaning_none);
    RUN_TEST(test_rel_meaning_all_types_non_null);
    RUN_TEST(test_rel_meaning_invalid);

    /* TYPE COUNT */
    RUN_TEST(test_relationship_type_count);

    return UNITY_END();
}
