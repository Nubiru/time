#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/clear_signs.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Count --- */

static void test_clear_sign_count_is_13(void)
{
    TEST_ASSERT_EQUAL_INT(13, clear_sign_count());
}

/* --- Kin 20: Yellow Resonant Sun (Group 2, East) --- */

static void test_kin20_kin_number(void)
{
    clear_sign_t s = clear_sign_get(0);
    TEST_ASSERT_EQUAL_INT(20, s.kin_number);
}

static void test_kin20_seal(void)
{
    clear_sign_t s = clear_sign_get(0);
    TEST_ASSERT_EQUAL_INT(0, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Sun", s.seal_name);
}

static void test_kin20_tone(void)
{
    clear_sign_t s = clear_sign_get(0);
    TEST_ASSERT_EQUAL_INT(7, s.tone);
}

static void test_kin20_group_and_border(void)
{
    clear_sign_t s = clear_sign_get(0);
    TEST_ASSERT_EQUAL_INT(2, s.group);
    TEST_ASSERT_EQUAL_STRING("East", s.border);
}

/* --- Kin 26: White Cosmic Worldbridger (Group 6, no border) --- */

static void test_kin26_kin_number(void)
{
    clear_sign_t s = clear_sign_get(1);
    TEST_ASSERT_EQUAL_INT(26, s.kin_number);
}

static void test_kin26_seal(void)
{
    clear_sign_t s = clear_sign_get(1);
    TEST_ASSERT_EQUAL_INT(6, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Worldbridger", s.seal_name);
}

static void test_kin26_tone(void)
{
    clear_sign_t s = clear_sign_get(1);
    TEST_ASSERT_EQUAL_INT(13, s.tone);
}

static void test_kin26_group_and_border(void)
{
    clear_sign_t s = clear_sign_get(1);
    TEST_ASSERT_EQUAL_INT(6, s.group);
    TEST_ASSERT_EQUAL_STRING("", s.border);
}

/* --- Kin 30: White Self-Existing Dog (Group 5, West) --- */

static void test_kin30_kin_number(void)
{
    clear_sign_t s = clear_sign_get(2);
    TEST_ASSERT_EQUAL_INT(30, s.kin_number);
}

static void test_kin30_seal(void)
{
    clear_sign_t s = clear_sign_get(2);
    TEST_ASSERT_EQUAL_INT(10, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Dog", s.seal_name);
}

static void test_kin30_tone(void)
{
    clear_sign_t s = clear_sign_get(2);
    TEST_ASSERT_EQUAL_INT(4, s.tone);
}

static void test_kin30_group_and_border(void)
{
    clear_sign_t s = clear_sign_get(2);
    TEST_ASSERT_EQUAL_INT(5, s.group);
    TEST_ASSERT_EQUAL_STRING("West", s.border);
}

/* --- Kin 40: Yellow Magnetic Sun (Group 6, no border) --- */

static void test_kin40_fields(void)
{
    clear_sign_t s = clear_sign_get(3);
    TEST_ASSERT_EQUAL_INT(40, s.kin_number);
    TEST_ASSERT_EQUAL_INT(0, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Sun", s.seal_name);
    TEST_ASSERT_EQUAL_INT(1, s.tone);
    TEST_ASSERT_EQUAL_INT(6, s.group);
    TEST_ASSERT_EQUAL_STRING("", s.border);
}

/* --- Kin 57: Red Overtone Earth (Group 2, East) --- */

static void test_kin57_fields(void)
{
    clear_sign_t s = clear_sign_get(4);
    TEST_ASSERT_EQUAL_INT(57, s.kin_number);
    TEST_ASSERT_EQUAL_INT(17, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Earth", s.seal_name);
    TEST_ASSERT_EQUAL_INT(5, s.tone);
    TEST_ASSERT_EQUAL_INT(2, s.group);
    TEST_ASSERT_EQUAL_STRING("East", s.border);
}

/* --- Kin 58: White Rhythmic Mirror (Group 1, South) --- */

static void test_kin58_fields(void)
{
    clear_sign_t s = clear_sign_get(5);
    TEST_ASSERT_EQUAL_INT(58, s.kin_number);
    TEST_ASSERT_EQUAL_INT(18, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Mirror", s.seal_name);
    TEST_ASSERT_EQUAL_INT(6, s.tone);
    TEST_ASSERT_EQUAL_INT(1, s.group);
    TEST_ASSERT_EQUAL_STRING("South", s.border);
}

/* --- Kin 60: Yellow Galactic Sun — Pacal Votan (Group 1, South) --- */

static void test_kin60_fields(void)
{
    clear_sign_t s = clear_sign_get(6);
    TEST_ASSERT_EQUAL_INT(60, s.kin_number);
    TEST_ASSERT_EQUAL_INT(0, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Sun", s.seal_name);
    TEST_ASSERT_EQUAL_INT(8, s.tone);
    TEST_ASSERT_EQUAL_INT(1, s.group);
    TEST_ASSERT_EQUAL_STRING("South", s.border);
}

/* --- Kin 87: Blue Solar Hand (Group 2, East) --- */

static void test_kin87_fields(void)
{
    clear_sign_t s = clear_sign_get(7);
    TEST_ASSERT_EQUAL_INT(87, s.kin_number);
    TEST_ASSERT_EQUAL_INT(7, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Hand", s.seal_name);
    TEST_ASSERT_EQUAL_INT(9, s.tone);
    TEST_ASSERT_EQUAL_INT(2, s.group);
    TEST_ASSERT_EQUAL_STRING("East", s.border);
}

/* --- Kin 106: White Lunar Worldbridger (Group 4, North) --- */

static void test_kin106_fields(void)
{
    clear_sign_t s = clear_sign_get(8);
    TEST_ASSERT_EQUAL_INT(106, s.kin_number);
    TEST_ASSERT_EQUAL_INT(6, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Worldbridger", s.seal_name);
    TEST_ASSERT_EQUAL_INT(2, s.tone);
    TEST_ASSERT_EQUAL_INT(4, s.group);
    TEST_ASSERT_EQUAL_STRING("North", s.border);
}

/* --- Kin 132: Yellow Lunar Human (Group 3, no border) --- */

static void test_kin132_fields(void)
{
    clear_sign_t s = clear_sign_get(9);
    TEST_ASSERT_EQUAL_INT(132, s.kin_number);
    TEST_ASSERT_EQUAL_INT(12, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Human", s.seal_name);
    TEST_ASSERT_EQUAL_INT(2, s.tone);
    TEST_ASSERT_EQUAL_INT(3, s.group);
    TEST_ASSERT_EQUAL_STRING("", s.border);
}

/* --- Kin 176: Yellow Resonant Warrior (Group 2, East) --- */

static void test_kin176_fields(void)
{
    clear_sign_t s = clear_sign_get(10);
    TEST_ASSERT_EQUAL_INT(176, s.kin_number);
    TEST_ASSERT_EQUAL_INT(16, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Warrior", s.seal_name);
    TEST_ASSERT_EQUAL_INT(7, s.tone);
    TEST_ASSERT_EQUAL_INT(2, s.group);
    TEST_ASSERT_EQUAL_STRING("East", s.border);
}

/* --- Kin 211: Blue Electric Monkey (Group 5, West) --- */

static void test_kin211_fields(void)
{
    clear_sign_t s = clear_sign_get(11);
    TEST_ASSERT_EQUAL_INT(211, s.kin_number);
    TEST_ASSERT_EQUAL_INT(11, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Monkey", s.seal_name);
    TEST_ASSERT_EQUAL_INT(3, s.tone);
    TEST_ASSERT_EQUAL_INT(5, s.group);
    TEST_ASSERT_EQUAL_STRING("West", s.border);
}

/* --- Kin 245: Red Spectral Serpent (Group 3, no border) --- */

static void test_kin245_fields(void)
{
    clear_sign_t s = clear_sign_get(12);
    TEST_ASSERT_EQUAL_INT(245, s.kin_number);
    TEST_ASSERT_EQUAL_INT(5, s.seal_code);
    TEST_ASSERT_EQUAL_STRING("Serpent", s.seal_name);
    TEST_ASSERT_EQUAL_INT(11, s.tone);
    TEST_ASSERT_EQUAL_INT(3, s.group);
    TEST_ASSERT_EQUAL_STRING("", s.border);
}

/* --- is_clear_sign: positive --- */

static void test_is_clear_sign_kin20(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(20));
}

static void test_is_clear_sign_kin26(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(26));
}

static void test_is_clear_sign_kin30(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(30));
}

static void test_is_clear_sign_kin40(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(40));
}

static void test_is_clear_sign_kin57(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(57));
}

static void test_is_clear_sign_kin58(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(58));
}

static void test_is_clear_sign_kin60(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(60));
}

static void test_is_clear_sign_kin87(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(87));
}

static void test_is_clear_sign_kin106(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(106));
}

static void test_is_clear_sign_kin132(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(132));
}

static void test_is_clear_sign_kin176(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(176));
}

static void test_is_clear_sign_kin211(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(211));
}

static void test_is_clear_sign_kin245(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(245));
}

/* --- is_clear_sign: negative --- */

static void test_not_clear_sign_kin1(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_is_clear_sign(1));
}

static void test_not_clear_sign_kin100(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_is_clear_sign(100));
}

static void test_not_clear_sign_kin260(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_is_clear_sign(260));
}

static void test_not_clear_sign_kin0(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_is_clear_sign(0));
}

static void test_not_clear_sign_negative(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_is_clear_sign(-5));
}

/* --- Total tone sum --- */

static void test_total_tone_sum_is_78(void)
{
    TEST_ASSERT_EQUAL_INT(78, clear_sign_total_tone_sum());
}

static void test_total_tone_sum_is_6_times_13(void)
{
    TEST_ASSERT_EQUAL_INT(6 * 13, clear_sign_total_tone_sum());
}

/* --- Group tone sums --- */

static void test_group1_tone_sum_14(void)
{
    TEST_ASSERT_EQUAL_INT(14, clear_sign_group_tone_sum(1));
}

static void test_group2_tone_sum_28(void)
{
    TEST_ASSERT_EQUAL_INT(28, clear_sign_group_tone_sum(2));
}

static void test_group3_tone_sum_13(void)
{
    TEST_ASSERT_EQUAL_INT(13, clear_sign_group_tone_sum(3));
}

static void test_group4_tone_sum_2(void)
{
    TEST_ASSERT_EQUAL_INT(2, clear_sign_group_tone_sum(4));
}

static void test_group5_tone_sum_7(void)
{
    TEST_ASSERT_EQUAL_INT(7, clear_sign_group_tone_sum(5));
}

static void test_group6_tone_sum_14(void)
{
    TEST_ASSERT_EQUAL_INT(14, clear_sign_group_tone_sum(6));
}

static void test_group_invalid_0_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_group_tone_sum(0));
}

static void test_group_invalid_7_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_group_tone_sum(7));
}

static void test_group_invalid_negative_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_group_tone_sum(-1));
}

static void test_group1_plus_group6_equals_28(void)
{
    int combined = clear_sign_group_tone_sum(1) + clear_sign_group_tone_sum(6);
    TEST_ASSERT_EQUAL_INT(28, combined);
}

static void test_all_group_sums_equal_total(void)
{
    int sum = 0;
    for (int g = 1; g <= 6; g++) {
        sum += clear_sign_group_tone_sum(g);
    }
    TEST_ASSERT_EQUAL_INT(clear_sign_total_tone_sum(), sum);
}

/* --- Pairs --- */

static void test_pair_20_87(void)
{
    TEST_ASSERT_EQUAL_INT(87, clear_sign_pair(20));
    TEST_ASSERT_EQUAL_INT(20, clear_sign_pair(87));
}

static void test_pair_26_106(void)
{
    TEST_ASSERT_EQUAL_INT(106, clear_sign_pair(26));
    TEST_ASSERT_EQUAL_INT(26, clear_sign_pair(106));
}

static void test_pair_30_132(void)
{
    TEST_ASSERT_EQUAL_INT(132, clear_sign_pair(30));
    TEST_ASSERT_EQUAL_INT(30, clear_sign_pair(132));
}

static void test_pair_40_176(void)
{
    TEST_ASSERT_EQUAL_INT(176, clear_sign_pair(40));
    TEST_ASSERT_EQUAL_INT(40, clear_sign_pair(176));
}

static void test_pair_57_211(void)
{
    TEST_ASSERT_EQUAL_INT(211, clear_sign_pair(57));
    TEST_ASSERT_EQUAL_INT(57, clear_sign_pair(211));
}

static void test_pair_58_245(void)
{
    TEST_ASSERT_EQUAL_INT(245, clear_sign_pair(58));
    TEST_ASSERT_EQUAL_INT(58, clear_sign_pair(245));
}

static void test_pair_60_unpaired(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_pair(60));
}

static void test_pair_non_clear_sign_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, clear_sign_pair(1));
    TEST_ASSERT_EQUAL_INT(0, clear_sign_pair(100));
    TEST_ASSERT_EQUAL_INT(0, clear_sign_pair(260));
}

/* --- Pacal Votan --- */

static void test_pacal_votan_kin_is_60(void)
{
    TEST_ASSERT_EQUAL_INT(60, clear_sign_pacal_votan_kin());
}

static void test_pacal_votan_is_clear_sign(void)
{
    TEST_ASSERT_EQUAL_INT(1, clear_sign_is_clear_sign(clear_sign_pacal_votan_kin()));
}

static void test_pacal_votan_is_sun_8(void)
{
    int pv = clear_sign_pacal_votan_kin();
    /* Find Pacal Votan in the array */
    for (int i = 0; i < clear_sign_count(); i++) {
        clear_sign_t s = clear_sign_get(i);
        if (s.kin_number == pv) {
            TEST_ASSERT_EQUAL_STRING("Sun", s.seal_name);
            TEST_ASSERT_EQUAL_INT(8, s.tone);
            return;
        }
    }
    TEST_FAIL_MESSAGE("Pacal Votan kin not found among clear signs");
}

/* --- Boundary: invalid index --- */

static void test_index_negative1_returns_zeroed(void)
{
    clear_sign_t s = clear_sign_get(-1);
    TEST_ASSERT_EQUAL_INT(0, s.kin_number);
    TEST_ASSERT_EQUAL_INT(0, s.seal_code);
    TEST_ASSERT_NULL(s.seal_name);
    TEST_ASSERT_EQUAL_INT(0, s.tone);
    TEST_ASSERT_EQUAL_INT(0, s.group);
    TEST_ASSERT_NULL(s.border);
}

static void test_index_13_returns_zeroed(void)
{
    clear_sign_t s = clear_sign_get(13);
    TEST_ASSERT_EQUAL_INT(0, s.kin_number);
    TEST_ASSERT_EQUAL_INT(0, s.seal_code);
    TEST_ASSERT_NULL(s.seal_name);
    TEST_ASSERT_EQUAL_INT(0, s.tone);
    TEST_ASSERT_EQUAL_INT(0, s.group);
    TEST_ASSERT_NULL(s.border);
}

static void test_index_99_returns_zeroed(void)
{
    clear_sign_t s = clear_sign_get(99);
    TEST_ASSERT_EQUAL_INT(0, s.kin_number);
    TEST_ASSERT_NULL(s.seal_name);
}

/* --- Purity: same input gives same output --- */

static void test_purity_get_same_result(void)
{
    clear_sign_t a = clear_sign_get(6);
    clear_sign_t b = clear_sign_get(6);
    TEST_ASSERT_EQUAL_INT(a.kin_number, b.kin_number);
    TEST_ASSERT_EQUAL_INT(a.seal_code, b.seal_code);
    TEST_ASSERT_EQUAL_INT(a.tone, b.tone);
    TEST_ASSERT_EQUAL_INT(a.group, b.group);
    TEST_ASSERT_EQUAL_STRING(a.seal_name, b.seal_name);
    TEST_ASSERT_EQUAL_STRING(a.border, b.border);
}

static void test_purity_is_clear_sign_consistent(void)
{
    for (int k = 1; k <= 260; k++) {
        int a = clear_sign_is_clear_sign(k);
        int b = clear_sign_is_clear_sign(k);
        TEST_ASSERT_EQUAL_INT(a, b);
    }
}

/* --- Sweep: all 13 signs have valid data --- */

static void test_all_signs_have_valid_data(void)
{
    for (int i = 0; i < 13; i++) {
        clear_sign_t s = clear_sign_get(i);
        TEST_ASSERT_TRUE(s.kin_number >= 1);
        TEST_ASSERT_TRUE(s.kin_number <= 260);
        TEST_ASSERT_TRUE(s.seal_code >= 0);
        TEST_ASSERT_TRUE(s.seal_code <= 19);
        TEST_ASSERT_NOT_NULL(s.seal_name);
        TEST_ASSERT_TRUE(strlen(s.seal_name) > 0);
        TEST_ASSERT_TRUE(s.tone >= 1);
        TEST_ASSERT_TRUE(s.tone <= 13);
        TEST_ASSERT_TRUE(s.group >= 1);
        TEST_ASSERT_TRUE(s.group <= 6);
        TEST_ASSERT_NOT_NULL(s.border);
    }
}

/* --- Sorted by kin number --- */

static void test_signs_sorted_by_kin_number(void)
{
    for (int i = 0; i < 12; i++) {
        clear_sign_t a = clear_sign_get(i);
        clear_sign_t b = clear_sign_get(i + 1);
        TEST_ASSERT_TRUE(a.kin_number < b.kin_number);
    }
}

/* --- Kin numbers are unique --- */

static void test_kin_numbers_unique(void)
{
    int nums[13];
    for (int i = 0; i < 13; i++) {
        nums[i] = clear_sign_get(i).kin_number;
    }
    for (int i = 0; i < 13; i++) {
        for (int j = i + 1; j < 13; j++) {
            TEST_ASSERT_NOT_EQUAL(nums[i], nums[j]);
        }
    }
}

/* --- Exactly 13 clear signs in range 1-260 --- */

static void test_exactly_13_clear_signs_in_range(void)
{
    int count = 0;
    for (int k = 1; k <= 260; k++) {
        if (clear_sign_is_clear_sign(k))
            count++;
    }
    TEST_ASSERT_EQUAL_INT(13, count);
}

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_clear_sign_count_is_13);

    /* Kin 20 — Sun 7, Group 2, East */
    RUN_TEST(test_kin20_kin_number);
    RUN_TEST(test_kin20_seal);
    RUN_TEST(test_kin20_tone);
    RUN_TEST(test_kin20_group_and_border);

    /* Kin 26 — Worldbridger 13, Group 6 */
    RUN_TEST(test_kin26_kin_number);
    RUN_TEST(test_kin26_seal);
    RUN_TEST(test_kin26_tone);
    RUN_TEST(test_kin26_group_and_border);

    /* Kin 30 — Dog 4, Group 5, West */
    RUN_TEST(test_kin30_kin_number);
    RUN_TEST(test_kin30_seal);
    RUN_TEST(test_kin30_tone);
    RUN_TEST(test_kin30_group_and_border);

    /* Kin 40 — Sun 1, Group 6 */
    RUN_TEST(test_kin40_fields);

    /* Kin 57 — Earth 5, Group 2, East */
    RUN_TEST(test_kin57_fields);

    /* Kin 58 — Mirror 6, Group 1, South */
    RUN_TEST(test_kin58_fields);

    /* Kin 60 — Sun 8, Group 1, South (Pacal Votan) */
    RUN_TEST(test_kin60_fields);

    /* Kin 87 — Hand 9, Group 2, East */
    RUN_TEST(test_kin87_fields);

    /* Kin 106 — Worldbridger 2, Group 4, North */
    RUN_TEST(test_kin106_fields);

    /* Kin 132 — Human 2, Group 3 */
    RUN_TEST(test_kin132_fields);

    /* Kin 176 — Warrior 7, Group 2, East */
    RUN_TEST(test_kin176_fields);

    /* Kin 211 — Monkey 3, Group 5, West */
    RUN_TEST(test_kin211_fields);

    /* Kin 245 — Serpent 11, Group 3 */
    RUN_TEST(test_kin245_fields);

    /* is_clear_sign: positive (all 13) */
    RUN_TEST(test_is_clear_sign_kin20);
    RUN_TEST(test_is_clear_sign_kin26);
    RUN_TEST(test_is_clear_sign_kin30);
    RUN_TEST(test_is_clear_sign_kin40);
    RUN_TEST(test_is_clear_sign_kin57);
    RUN_TEST(test_is_clear_sign_kin58);
    RUN_TEST(test_is_clear_sign_kin60);
    RUN_TEST(test_is_clear_sign_kin87);
    RUN_TEST(test_is_clear_sign_kin106);
    RUN_TEST(test_is_clear_sign_kin132);
    RUN_TEST(test_is_clear_sign_kin176);
    RUN_TEST(test_is_clear_sign_kin211);
    RUN_TEST(test_is_clear_sign_kin245);

    /* is_clear_sign: negative */
    RUN_TEST(test_not_clear_sign_kin1);
    RUN_TEST(test_not_clear_sign_kin100);
    RUN_TEST(test_not_clear_sign_kin260);
    RUN_TEST(test_not_clear_sign_kin0);
    RUN_TEST(test_not_clear_sign_negative);

    /* Tone sums */
    RUN_TEST(test_total_tone_sum_is_78);
    RUN_TEST(test_total_tone_sum_is_6_times_13);
    RUN_TEST(test_group1_tone_sum_14);
    RUN_TEST(test_group2_tone_sum_28);
    RUN_TEST(test_group3_tone_sum_13);
    RUN_TEST(test_group4_tone_sum_2);
    RUN_TEST(test_group5_tone_sum_7);
    RUN_TEST(test_group6_tone_sum_14);
    RUN_TEST(test_group_invalid_0_returns_0);
    RUN_TEST(test_group_invalid_7_returns_0);
    RUN_TEST(test_group_invalid_negative_returns_0);
    RUN_TEST(test_group1_plus_group6_equals_28);
    RUN_TEST(test_all_group_sums_equal_total);

    /* Pairs */
    RUN_TEST(test_pair_20_87);
    RUN_TEST(test_pair_26_106);
    RUN_TEST(test_pair_30_132);
    RUN_TEST(test_pair_40_176);
    RUN_TEST(test_pair_57_211);
    RUN_TEST(test_pair_58_245);
    RUN_TEST(test_pair_60_unpaired);
    RUN_TEST(test_pair_non_clear_sign_returns_0);

    /* Pacal Votan */
    RUN_TEST(test_pacal_votan_kin_is_60);
    RUN_TEST(test_pacal_votan_is_clear_sign);
    RUN_TEST(test_pacal_votan_is_sun_8);

    /* Boundary: invalid index */
    RUN_TEST(test_index_negative1_returns_zeroed);
    RUN_TEST(test_index_13_returns_zeroed);
    RUN_TEST(test_index_99_returns_zeroed);

    /* Purity */
    RUN_TEST(test_purity_get_same_result);
    RUN_TEST(test_purity_is_clear_sign_consistent);

    /* Sweep */
    RUN_TEST(test_all_signs_have_valid_data);
    RUN_TEST(test_signs_sorted_by_kin_number);
    RUN_TEST(test_kin_numbers_unique);
    RUN_TEST(test_exactly_13_clear_signs_in_range);

    return UNITY_END();
}
