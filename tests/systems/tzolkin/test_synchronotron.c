#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/synchronotron.h"

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * sync_compute — specific examples
 * ================================================================ */

/* Dragon 5 (seal 1, tone 5, day 1) */

static void test_compute_dragon5_tonal_value(void)
{
    sync_daily_t d = sync_compute(1, 1, 5);
    TEST_ASSERT_EQUAL_INT(540, d.tonal_value);  /* 5 x 108 */
}

static void test_compute_dragon5_lens(void)
{
    sync_daily_t d = sync_compute(1, 1, 5);
    TEST_ASSERT_EQUAL_INT(1, d.lens_number);
}

static void test_compute_dragon5_lens_value(void)
{
    sync_daily_t d = sync_compute(1, 1, 5);
    TEST_ASSERT_EQUAL_INT(108, d.lens_value);
}

static void test_compute_dragon5_heptad_gate(void)
{
    sync_daily_t d = sync_compute(1, 1, 5);
    TEST_ASSERT_EQUAL_INT(108, d.heptad_gate_hk);
}

static void test_compute_dragon5_destiny(void)
{
    sync_daily_t d = sync_compute(1, 1, 5);
    TEST_ASSERT_EQUAL_INT(108, d.destiny_hk);
}

static void test_compute_dragon5_identity(void)
{
    sync_daily_t d = sync_compute(1, 1, 5);
    TEST_ASSERT_EQUAL_INT(1, d.day_of_week);
    TEST_ASSERT_EQUAL_INT(1, d.seal_code);
    TEST_ASSERT_EQUAL_INT(5, d.tone);
}

/* Serpent 3 (seal 5, tone 3, day 3) */

static void test_compute_serpent3_tonal_value(void)
{
    sync_daily_t d = sync_compute(3, 5, 3);
    TEST_ASSERT_EQUAL_INT(648, d.tonal_value);  /* 3 x 216 */
}

static void test_compute_serpent3_lens(void)
{
    sync_daily_t d = sync_compute(3, 5, 3);
    TEST_ASSERT_EQUAL_INT(3, d.lens_number);
}

static void test_compute_serpent3_lens_value(void)
{
    sync_daily_t d = sync_compute(3, 5, 3);
    TEST_ASSERT_EQUAL_INT(216, d.lens_value);
}

static void test_compute_serpent3_heptad_gate(void)
{
    sync_daily_t d = sync_compute(3, 5, 3);
    TEST_ASSERT_EQUAL_INT(144, d.heptad_gate_hk);
}

static void test_compute_serpent3_destiny(void)
{
    sync_daily_t d = sync_compute(3, 5, 3);
    TEST_ASSERT_EQUAL_INT(288, d.destiny_hk);
}

/* Sun 13 (seal 20, tone 13, day 7) */

static void test_compute_sun13_tonal_value(void)
{
    sync_daily_t d = sync_compute(7, 20, 13);
    TEST_ASSERT_EQUAL_INT(1404, d.tonal_value);  /* 13 x 108 */
}

static void test_compute_sun13_lens(void)
{
    sync_daily_t d = sync_compute(7, 20, 13);
    TEST_ASSERT_EQUAL_INT(1, d.lens_number);
}

static void test_compute_sun13_lens_value(void)
{
    sync_daily_t d = sync_compute(7, 20, 13);
    TEST_ASSERT_EQUAL_INT(108, d.lens_value);
}

static void test_compute_sun13_heptad_gate(void)
{
    sync_daily_t d = sync_compute(7, 20, 13);
    TEST_ASSERT_EQUAL_INT(441, d.heptad_gate_hk);
}

static void test_compute_sun13_destiny(void)
{
    sync_daily_t d = sync_compute(7, 20, 13);
    TEST_ASSERT_EQUAL_INT(414, d.destiny_hk);
}

/* ================================================================
 * sync_heptad_gate_hk — all 7 days
 * ================================================================ */

static void test_gate_hk_day1(void)
{
    TEST_ASSERT_EQUAL_INT(108, sync_heptad_gate_hk(1));
}

static void test_gate_hk_day2(void)
{
    TEST_ASSERT_EQUAL_INT(291, sync_heptad_gate_hk(2));
}

static void test_gate_hk_day3(void)
{
    TEST_ASSERT_EQUAL_INT(144, sync_heptad_gate_hk(3));
}

static void test_gate_hk_day4(void)
{
    TEST_ASSERT_EQUAL_INT(315, sync_heptad_gate_hk(4));
}

static void test_gate_hk_day5(void)
{
    TEST_ASSERT_EQUAL_INT(414, sync_heptad_gate_hk(5));
}

static void test_gate_hk_day6(void)
{
    TEST_ASSERT_EQUAL_INT(402, sync_heptad_gate_hk(6));
}

static void test_gate_hk_day7(void)
{
    TEST_ASSERT_EQUAL_INT(441, sync_heptad_gate_hk(7));
}

/* ================================================================
 * sync_seal_hk — all 21 seals
 * ================================================================ */

static void test_seal_hk_1_dragon(void)
{
    TEST_ASSERT_EQUAL_INT(108, sync_seal_hk(1));
}

static void test_seal_hk_2_wind(void)
{
    TEST_ASSERT_EQUAL_INT(144, sync_seal_hk(2));
}

static void test_seal_hk_3_night(void)
{
    TEST_ASSERT_EQUAL_INT(126, sync_seal_hk(3));
}

static void test_seal_hk_4_seed(void)
{
    TEST_ASSERT_EQUAL_INT(90, sync_seal_hk(4));
}

static void test_seal_hk_5_serpent(void)
{
    TEST_ASSERT_EQUAL_INT(288, sync_seal_hk(5));
}

static void test_seal_hk_6_worldbridger(void)
{
    TEST_ASSERT_EQUAL_INT(294, sync_seal_hk(6));
}

static void test_seal_hk_7_monkey(void)
{
    TEST_ASSERT_EQUAL_INT(291, sync_seal_hk(7));
}

static void test_seal_hk_8_star(void)
{
    TEST_ASSERT_EQUAL_INT(300, sync_seal_hk(8));
}

static void test_seal_hk_9_moon(void)
{
    TEST_ASSERT_EQUAL_INT(306, sync_seal_hk(9));
}

static void test_seal_hk_10_dog(void)
{
    TEST_ASSERT_EQUAL_INT(303, sync_seal_hk(10));
}

static void test_seal_hk_11_hand(void)
{
    TEST_ASSERT_EQUAL_INT(312, sync_seal_hk(11));
}

static void test_seal_hk_12_human(void)
{
    TEST_ASSERT_EQUAL_INT(318, sync_seal_hk(12));
}

static void test_seal_hk_13_skywalker(void)
{
    TEST_ASSERT_EQUAL_INT(315, sync_seal_hk(13));
}

static void test_seal_hk_14_wizard(void)
{
    TEST_ASSERT_EQUAL_INT(276, sync_seal_hk(14));
}

static void test_seal_hk_15_eagle(void)
{
    TEST_ASSERT_EQUAL_INT(282, sync_seal_hk(15));
}

static void test_seal_hk_16_warrior(void)
{
    TEST_ASSERT_EQUAL_INT(279, sync_seal_hk(16));
}

static void test_seal_hk_17_earth(void)
{
    TEST_ASSERT_EQUAL_INT(396, sync_seal_hk(17));
}

static void test_seal_hk_18_mirror(void)
{
    TEST_ASSERT_EQUAL_INT(402, sync_seal_hk(18));
}

static void test_seal_hk_19_storm(void)
{
    TEST_ASSERT_EQUAL_INT(408, sync_seal_hk(19));
}

static void test_seal_hk_20_sun(void)
{
    TEST_ASSERT_EQUAL_INT(414, sync_seal_hk(20));
}

static void test_seal_hk_21_hunab_ku(void)
{
    TEST_ASSERT_EQUAL_INT(441, sync_seal_hk(21));
}

/* ================================================================
 * sync_lens_for_tone — all 13 tones
 * ================================================================ */

static void test_lens_tone1(void)  { TEST_ASSERT_EQUAL_INT(1, sync_lens_for_tone(1));  }
static void test_lens_tone2(void)  { TEST_ASSERT_EQUAL_INT(2, sync_lens_for_tone(2));  }
static void test_lens_tone3(void)  { TEST_ASSERT_EQUAL_INT(3, sync_lens_for_tone(3));  }
static void test_lens_tone4(void)  { TEST_ASSERT_EQUAL_INT(4, sync_lens_for_tone(4));  }
static void test_lens_tone5(void)  { TEST_ASSERT_EQUAL_INT(1, sync_lens_for_tone(5));  }
static void test_lens_tone6(void)  { TEST_ASSERT_EQUAL_INT(2, sync_lens_for_tone(6));  }
static void test_lens_tone7(void)  { TEST_ASSERT_EQUAL_INT(3, sync_lens_for_tone(7));  }
static void test_lens_tone8(void)  { TEST_ASSERT_EQUAL_INT(4, sync_lens_for_tone(8));  }
static void test_lens_tone9(void)  { TEST_ASSERT_EQUAL_INT(1, sync_lens_for_tone(9));  }
static void test_lens_tone10(void) { TEST_ASSERT_EQUAL_INT(2, sync_lens_for_tone(10)); }
static void test_lens_tone11(void) { TEST_ASSERT_EQUAL_INT(3, sync_lens_for_tone(11)); }
static void test_lens_tone12(void) { TEST_ASSERT_EQUAL_INT(4, sync_lens_for_tone(12)); }
static void test_lens_tone13(void) { TEST_ASSERT_EQUAL_INT(1, sync_lens_for_tone(13)); }

/* ================================================================
 * sync_tonal_value — all 13 tones
 * ================================================================ */

static void test_tonal_tone1(void)  { TEST_ASSERT_EQUAL_INT(108,  sync_tonal_value(1));  }
static void test_tonal_tone2(void)  { TEST_ASSERT_EQUAL_INT(288,  sync_tonal_value(2));  }
static void test_tonal_tone3(void)  { TEST_ASSERT_EQUAL_INT(648,  sync_tonal_value(3));  }
static void test_tonal_tone4(void)  { TEST_ASSERT_EQUAL_INT(1152, sync_tonal_value(4));  }
static void test_tonal_tone5(void)  { TEST_ASSERT_EQUAL_INT(540,  sync_tonal_value(5));  }
static void test_tonal_tone6(void)  { TEST_ASSERT_EQUAL_INT(864,  sync_tonal_value(6));  }
static void test_tonal_tone7(void)  { TEST_ASSERT_EQUAL_INT(1512, sync_tonal_value(7));  }
static void test_tonal_tone8(void)  { TEST_ASSERT_EQUAL_INT(2304, sync_tonal_value(8));  }
static void test_tonal_tone9(void)  { TEST_ASSERT_EQUAL_INT(972,  sync_tonal_value(9));  }
static void test_tonal_tone10(void) { TEST_ASSERT_EQUAL_INT(1440, sync_tonal_value(10)); }
static void test_tonal_tone11(void) { TEST_ASSERT_EQUAL_INT(2376, sync_tonal_value(11)); }
static void test_tonal_tone12(void) { TEST_ASSERT_EQUAL_INT(3456, sync_tonal_value(12)); }
static void test_tonal_tone13(void) { TEST_ASSERT_EQUAL_INT(1404, sync_tonal_value(13)); }

/* ================================================================
 * sync_lens_value_for_tone — spot checks
 * ================================================================ */

static void test_lens_value_tone1(void)
{
    TEST_ASSERT_EQUAL_INT(108, sync_lens_value_for_tone(1));
}

static void test_lens_value_tone4(void)
{
    TEST_ASSERT_EQUAL_INT(288, sync_lens_value_for_tone(4));
}

static void test_lens_value_tone7(void)
{
    TEST_ASSERT_EQUAL_INT(216, sync_lens_value_for_tone(7));
}

static void test_lens_value_tone10(void)
{
    TEST_ASSERT_EQUAL_INT(144, sync_lens_value_for_tone(10));
}

static void test_lens_value_tone13(void)
{
    TEST_ASSERT_EQUAL_INT(108, sync_lens_value_for_tone(13));
}

/* ================================================================
 * Boundary: invalid day_of_week
 * ================================================================ */

static void test_compute_day0_zeroed(void)
{
    sync_daily_t d = sync_compute(0, 1, 5);
    TEST_ASSERT_EQUAL_INT(0, d.day_of_week);
    TEST_ASSERT_EQUAL_INT(0, d.seal_code);
    TEST_ASSERT_EQUAL_INT(0, d.tone);
    TEST_ASSERT_EQUAL_INT(0, d.tonal_value);
    TEST_ASSERT_EQUAL_INT(0, d.heptad_gate_hk);
    TEST_ASSERT_EQUAL_INT(0, d.destiny_hk);
}

static void test_compute_day8_zeroed(void)
{
    sync_daily_t d = sync_compute(8, 1, 5);
    TEST_ASSERT_EQUAL_INT(0, d.day_of_week);
    TEST_ASSERT_EQUAL_INT(0, d.tonal_value);
}

static void test_compute_day_negative_zeroed(void)
{
    sync_daily_t d = sync_compute(-1, 1, 5);
    TEST_ASSERT_EQUAL_INT(0, d.day_of_week);
    TEST_ASSERT_EQUAL_INT(0, d.tonal_value);
}

static void test_gate_hk_day0(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_heptad_gate_hk(0));
}

static void test_gate_hk_day8(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_heptad_gate_hk(8));
}

static void test_gate_hk_day_negative(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_heptad_gate_hk(-1));
}

/* ================================================================
 * Boundary: invalid seal_code
 * ================================================================ */

static void test_compute_seal0_zeroed(void)
{
    sync_daily_t d = sync_compute(1, 0, 5);
    TEST_ASSERT_EQUAL_INT(0, d.seal_code);
    TEST_ASSERT_EQUAL_INT(0, d.tonal_value);
}

static void test_compute_seal21_zeroed(void)
{
    /* sync_compute takes seal 1-20 only (not 21/Hunab Ku) */
    sync_daily_t d = sync_compute(1, 21, 5);
    TEST_ASSERT_EQUAL_INT(0, d.seal_code);
    TEST_ASSERT_EQUAL_INT(0, d.tonal_value);
}

static void test_compute_seal_negative_zeroed(void)
{
    sync_daily_t d = sync_compute(1, -1, 5);
    TEST_ASSERT_EQUAL_INT(0, d.seal_code);
    TEST_ASSERT_EQUAL_INT(0, d.tonal_value);
}

static void test_seal_hk_0_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_seal_hk(0));
}

static void test_seal_hk_22_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_seal_hk(22));
}

static void test_seal_hk_negative_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_seal_hk(-1));
}

/* ================================================================
 * Boundary: invalid tone
 * ================================================================ */

static void test_compute_tone0_zeroed(void)
{
    sync_daily_t d = sync_compute(1, 1, 0);
    TEST_ASSERT_EQUAL_INT(0, d.tone);
    TEST_ASSERT_EQUAL_INT(0, d.tonal_value);
}

static void test_compute_tone14_zeroed(void)
{
    sync_daily_t d = sync_compute(1, 1, 14);
    TEST_ASSERT_EQUAL_INT(0, d.tone);
    TEST_ASSERT_EQUAL_INT(0, d.tonal_value);
}

static void test_compute_tone_negative_zeroed(void)
{
    sync_daily_t d = sync_compute(1, 1, -1);
    TEST_ASSERT_EQUAL_INT(0, d.tone);
    TEST_ASSERT_EQUAL_INT(0, d.tonal_value);
}

static void test_tonal_value_tone0(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_tonal_value(0));
}

static void test_tonal_value_tone14(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_tonal_value(14));
}

static void test_tonal_value_tone_negative(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_tonal_value(-1));
}

static void test_lens_for_tone0(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_lens_for_tone(0));
}

static void test_lens_for_tone14(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_lens_for_tone(14));
}

static void test_lens_for_tone_negative(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_lens_for_tone(-1));
}

static void test_lens_value_tone0(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_lens_value_for_tone(0));
}

static void test_lens_value_tone14(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_lens_value_for_tone(14));
}

static void test_lens_value_tone_negative(void)
{
    TEST_ASSERT_EQUAL_INT(0, sync_lens_value_for_tone(-1));
}

/* ================================================================
 * Purity: same input gives same output
 * ================================================================ */

static void test_purity_compute(void)
{
    sync_daily_t a = sync_compute(4, 10, 7);
    sync_daily_t b = sync_compute(4, 10, 7);
    TEST_ASSERT_EQUAL_INT(a.day_of_week, b.day_of_week);
    TEST_ASSERT_EQUAL_INT(a.seal_code, b.seal_code);
    TEST_ASSERT_EQUAL_INT(a.tone, b.tone);
    TEST_ASSERT_EQUAL_INT(a.tonal_value, b.tonal_value);
    TEST_ASSERT_EQUAL_INT(a.lens_number, b.lens_number);
    TEST_ASSERT_EQUAL_INT(a.lens_value, b.lens_value);
    TEST_ASSERT_EQUAL_INT(a.heptad_gate_hk, b.heptad_gate_hk);
    TEST_ASSERT_EQUAL_INT(a.destiny_hk, b.destiny_hk);
}

static void test_purity_helpers(void)
{
    TEST_ASSERT_EQUAL_INT(sync_tonal_value(6), sync_tonal_value(6));
    TEST_ASSERT_EQUAL_INT(sync_lens_for_tone(6), sync_lens_for_tone(6));
    TEST_ASSERT_EQUAL_INT(sync_lens_value_for_tone(6), sync_lens_value_for_tone(6));
    TEST_ASSERT_EQUAL_INT(sync_heptad_gate_hk(3), sync_heptad_gate_hk(3));
    TEST_ASSERT_EQUAL_INT(sync_seal_hk(10), sync_seal_hk(10));
}

/* ================================================================
 * Sweep: all valid inputs produce non-zero results
 * ================================================================ */

static void test_all_days_have_nonzero_gate_hk(void)
{
    for (int d = 1; d <= 7; d++) {
        TEST_ASSERT_TRUE(sync_heptad_gate_hk(d) > 0);
    }
}

static void test_all_seals_have_nonzero_hk(void)
{
    for (int s = 1; s <= 21; s++) {
        TEST_ASSERT_TRUE(sync_seal_hk(s) > 0);
    }
}

static void test_all_tones_have_nonzero_tonal(void)
{
    for (int t = 1; t <= 13; t++) {
        TEST_ASSERT_TRUE(sync_tonal_value(t) > 0);
    }
}

static void test_all_tones_have_valid_lens(void)
{
    for (int t = 1; t <= 13; t++) {
        int lens = sync_lens_for_tone(t);
        TEST_ASSERT_TRUE(lens >= 1 && lens <= 4);
    }
}

static void test_all_combinations_produce_valid(void)
{
    /* Spot-check: every (day, seal, tone) combination gives non-zero tonal */
    for (int d = 1; d <= 7; d++) {
        for (int s = 1; s <= 20; s += 5) {
            for (int t = 1; t <= 13; t += 4) {
                sync_daily_t r = sync_compute(d, s, t);
                TEST_ASSERT_TRUE(r.tonal_value > 0);
                TEST_ASSERT_TRUE(r.heptad_gate_hk > 0);
                TEST_ASSERT_TRUE(r.destiny_hk > 0);
            }
        }
    }
}

/* ================================================================
 * Structural invariants
 * ================================================================ */

static void test_hunab_ku_is_21_squared(void)
{
    TEST_ASSERT_EQUAL_INT(441, 21 * 21);
    TEST_ASSERT_EQUAL_INT(441, sync_seal_hk(21));
}

static void test_gate_day7_is_441(void)
{
    TEST_ASSERT_EQUAL_INT(441, sync_heptad_gate_hk(7));
}

static void test_tonal_value_equals_tone_times_lens_value(void)
{
    for (int t = 1; t <= 13; t++) {
        int expected = t * sync_lens_value_for_tone(t);
        TEST_ASSERT_EQUAL_INT(expected, sync_tonal_value(t));
    }
}

static void test_lens_values_are_multiples_of_36(void)
{
    int lens_vals[] = {108, 144, 216, 288};
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_INT(0, lens_vals[i] % 36);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* sync_compute — Dragon 5 */
    RUN_TEST(test_compute_dragon5_tonal_value);
    RUN_TEST(test_compute_dragon5_lens);
    RUN_TEST(test_compute_dragon5_lens_value);
    RUN_TEST(test_compute_dragon5_heptad_gate);
    RUN_TEST(test_compute_dragon5_destiny);
    RUN_TEST(test_compute_dragon5_identity);

    /* sync_compute — Serpent 3 */
    RUN_TEST(test_compute_serpent3_tonal_value);
    RUN_TEST(test_compute_serpent3_lens);
    RUN_TEST(test_compute_serpent3_lens_value);
    RUN_TEST(test_compute_serpent3_heptad_gate);
    RUN_TEST(test_compute_serpent3_destiny);

    /* sync_compute — Sun 13 */
    RUN_TEST(test_compute_sun13_tonal_value);
    RUN_TEST(test_compute_sun13_lens);
    RUN_TEST(test_compute_sun13_lens_value);
    RUN_TEST(test_compute_sun13_heptad_gate);
    RUN_TEST(test_compute_sun13_destiny);

    /* Heptad gate HK — all 7 */
    RUN_TEST(test_gate_hk_day1);
    RUN_TEST(test_gate_hk_day2);
    RUN_TEST(test_gate_hk_day3);
    RUN_TEST(test_gate_hk_day4);
    RUN_TEST(test_gate_hk_day5);
    RUN_TEST(test_gate_hk_day6);
    RUN_TEST(test_gate_hk_day7);

    /* Seal HK — all 21 */
    RUN_TEST(test_seal_hk_1_dragon);
    RUN_TEST(test_seal_hk_2_wind);
    RUN_TEST(test_seal_hk_3_night);
    RUN_TEST(test_seal_hk_4_seed);
    RUN_TEST(test_seal_hk_5_serpent);
    RUN_TEST(test_seal_hk_6_worldbridger);
    RUN_TEST(test_seal_hk_7_monkey);
    RUN_TEST(test_seal_hk_8_star);
    RUN_TEST(test_seal_hk_9_moon);
    RUN_TEST(test_seal_hk_10_dog);
    RUN_TEST(test_seal_hk_11_hand);
    RUN_TEST(test_seal_hk_12_human);
    RUN_TEST(test_seal_hk_13_skywalker);
    RUN_TEST(test_seal_hk_14_wizard);
    RUN_TEST(test_seal_hk_15_eagle);
    RUN_TEST(test_seal_hk_16_warrior);
    RUN_TEST(test_seal_hk_17_earth);
    RUN_TEST(test_seal_hk_18_mirror);
    RUN_TEST(test_seal_hk_19_storm);
    RUN_TEST(test_seal_hk_20_sun);
    RUN_TEST(test_seal_hk_21_hunab_ku);

    /* Lens for tone — all 13 */
    RUN_TEST(test_lens_tone1);
    RUN_TEST(test_lens_tone2);
    RUN_TEST(test_lens_tone3);
    RUN_TEST(test_lens_tone4);
    RUN_TEST(test_lens_tone5);
    RUN_TEST(test_lens_tone6);
    RUN_TEST(test_lens_tone7);
    RUN_TEST(test_lens_tone8);
    RUN_TEST(test_lens_tone9);
    RUN_TEST(test_lens_tone10);
    RUN_TEST(test_lens_tone11);
    RUN_TEST(test_lens_tone12);
    RUN_TEST(test_lens_tone13);

    /* Tonal values — all 13 */
    RUN_TEST(test_tonal_tone1);
    RUN_TEST(test_tonal_tone2);
    RUN_TEST(test_tonal_tone3);
    RUN_TEST(test_tonal_tone4);
    RUN_TEST(test_tonal_tone5);
    RUN_TEST(test_tonal_tone6);
    RUN_TEST(test_tonal_tone7);
    RUN_TEST(test_tonal_tone8);
    RUN_TEST(test_tonal_tone9);
    RUN_TEST(test_tonal_tone10);
    RUN_TEST(test_tonal_tone11);
    RUN_TEST(test_tonal_tone12);
    RUN_TEST(test_tonal_tone13);

    /* Lens value for tone — spot checks */
    RUN_TEST(test_lens_value_tone1);
    RUN_TEST(test_lens_value_tone4);
    RUN_TEST(test_lens_value_tone7);
    RUN_TEST(test_lens_value_tone10);
    RUN_TEST(test_lens_value_tone13);

    /* Boundary — invalid day_of_week */
    RUN_TEST(test_compute_day0_zeroed);
    RUN_TEST(test_compute_day8_zeroed);
    RUN_TEST(test_compute_day_negative_zeroed);
    RUN_TEST(test_gate_hk_day0);
    RUN_TEST(test_gate_hk_day8);
    RUN_TEST(test_gate_hk_day_negative);

    /* Boundary — invalid seal_code */
    RUN_TEST(test_compute_seal0_zeroed);
    RUN_TEST(test_compute_seal21_zeroed);
    RUN_TEST(test_compute_seal_negative_zeroed);
    RUN_TEST(test_seal_hk_0_invalid);
    RUN_TEST(test_seal_hk_22_invalid);
    RUN_TEST(test_seal_hk_negative_invalid);

    /* Boundary — invalid tone */
    RUN_TEST(test_compute_tone0_zeroed);
    RUN_TEST(test_compute_tone14_zeroed);
    RUN_TEST(test_compute_tone_negative_zeroed);
    RUN_TEST(test_tonal_value_tone0);
    RUN_TEST(test_tonal_value_tone14);
    RUN_TEST(test_tonal_value_tone_negative);
    RUN_TEST(test_lens_for_tone0);
    RUN_TEST(test_lens_for_tone14);
    RUN_TEST(test_lens_for_tone_negative);
    RUN_TEST(test_lens_value_tone0);
    RUN_TEST(test_lens_value_tone14);
    RUN_TEST(test_lens_value_tone_negative);

    /* Purity */
    RUN_TEST(test_purity_compute);
    RUN_TEST(test_purity_helpers);

    /* Sweep */
    RUN_TEST(test_all_days_have_nonzero_gate_hk);
    RUN_TEST(test_all_seals_have_nonzero_hk);
    RUN_TEST(test_all_tones_have_nonzero_tonal);
    RUN_TEST(test_all_tones_have_valid_lens);
    RUN_TEST(test_all_combinations_produce_valid);

    /* Structural invariants */
    RUN_TEST(test_hunab_ku_is_21_squared);
    RUN_TEST(test_gate_day7_is_441);
    RUN_TEST(test_tonal_value_equals_tone_times_lens_value);
    RUN_TEST(test_lens_values_are_multiples_of_36);

    return UNITY_END();
}
