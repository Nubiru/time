#include "../unity/unity.h"
#include "../../src/ui/kin_earth_family.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Today's family assignment --- */

void test_seal_0_dragon_family_cardinal(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    TEST_ASSERT_EQUAL_INT(0, ef.today_seal);
    TEST_ASSERT_EQUAL_INT(0, ef.today_family);
    TEST_ASSERT_EQUAL_STRING("Cardinal", ef.families[0].name);
    TEST_ASSERT_EQUAL_INT(1, ef.families[0].contains_today);
}

void test_seal_1_wind_family_core(void)
{
    kin_earth_family_t ef = kin_family_compute(1);
    TEST_ASSERT_EQUAL_INT(1, ef.today_family);
    TEST_ASSERT_EQUAL_STRING("Core", ef.families[1].name);
    TEST_ASSERT_EQUAL_INT(1, ef.families[1].contains_today);
}

void test_seal_4_serpent_family_polar(void)
{
    kin_earth_family_t ef = kin_family_compute(4);
    TEST_ASSERT_EQUAL_INT(4, ef.today_family);
    TEST_ASSERT_EQUAL_STRING("Polar", ef.families[4].name);
    TEST_ASSERT_EQUAL_INT(1, ef.families[4].contains_today);
}

void test_seal_10_monkey_family_cardinal(void)
{
    /* seal 10 % 5 = 0 → Cardinal (same as Dragon) */
    kin_earth_family_t ef = kin_family_compute(10);
    TEST_ASSERT_EQUAL_INT(0, ef.today_family);
    TEST_ASSERT_EQUAL_INT(1, ef.families[0].contains_today);
}

void test_seal_19_sun_family_polar(void)
{
    /* seal 19 % 5 = 4 → Polar */
    kin_earth_family_t ef = kin_family_compute(19);
    TEST_ASSERT_EQUAL_INT(4, ef.today_family);
    TEST_ASSERT_EQUAL_INT(1, ef.families[4].contains_today);
}

/* --- Only one family contains today --- */

void test_only_one_family_contains_today(void)
{
    kin_earth_family_t ef = kin_family_compute(7);
    int count = 0;
    for (int i = 0; i < KIN_FAMILY_COUNT; i++) {
        count += ef.families[i].contains_today;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- Family names valid --- */

void test_all_families_have_valid_names(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    for (int i = 0; i < KIN_FAMILY_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(ef.families[i].name);
        TEST_ASSERT_TRUE(strcmp(ef.families[i].name, "?") != 0);
    }
}

void test_all_families_have_valid_directions(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    for (int i = 0; i < KIN_FAMILY_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(ef.families[i].direction);
        TEST_ASSERT_TRUE(strcmp(ef.families[i].direction, "?") != 0);
    }
}

/* --- 20 seals covered, no duplicates --- */

void test_all_20_seals_covered_no_duplicates(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    int seen[20];
    memset(seen, 0, sizeof(seen));

    for (int i = 0; i < KIN_FAMILY_COUNT; i++) {
        for (int m = 0; m < KIN_FAMILY_MEMBERS; m++) {
            int s = ef.families[i].seals[m];
            TEST_ASSERT_TRUE(s >= 0 && s <= 19);
            TEST_ASSERT_EQUAL_INT(0, seen[s]); /* no duplicate */
            seen[s] = 1;
        }
    }

    /* All 20 covered */
    for (int s = 0; s < 20; s++) {
        TEST_ASSERT_EQUAL_INT(1, seen[s]);
    }
}

/* --- Pentacle positions --- */

void test_family_0_position_top(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, ef.families[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, ef.families[0].y);
}

void test_family_1_position_upper_right(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.88f, ef.families[1].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.38f, ef.families[1].y);
}

void test_family_2_position_lower_right(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.74f, ef.families[2].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.85f, ef.families[2].y);
}

void test_family_3_position_lower_left(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.26f, ef.families[3].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.85f, ef.families[3].y);
}

void test_family_4_position_upper_left(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.12f, ef.families[4].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.38f, ef.families[4].y);
}

/* --- kin_family_for_seal --- */

void test_family_for_seal_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_family_for_seal(0));
}

void test_family_for_seal_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_family_for_seal(1));
}

void test_family_for_seal_5(void)
{
    /* seal 5 % 5 = 0 → Cardinal */
    TEST_ASSERT_EQUAL_INT(0, kin_family_for_seal(5));
}

void test_family_for_seal_19(void)
{
    /* seal 19 % 5 = 4 → Polar */
    TEST_ASSERT_EQUAL_INT(4, kin_family_for_seal(19));
}

void test_family_for_seal_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_family_for_seal(-1));
}

void test_family_for_seal_invalid_20(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_family_for_seal(20));
}

/* --- kin_family_from_kin --- */

void test_from_kin_1_seal_0_cardinal(void)
{
    /* kin 1: seal = (1-1)%20 = 0 → Cardinal */
    kin_earth_family_t ef = kin_family_from_kin(1);
    TEST_ASSERT_EQUAL_INT(0, ef.today_seal);
    TEST_ASSERT_EQUAL_INT(0, ef.today_family);
}

void test_from_kin_131_seal_10_cardinal(void)
{
    /* kin 131: seal = (131-1)%20 = 10 → Monkey → 10%5=0 → Cardinal */
    kin_earth_family_t ef = kin_family_from_kin(131);
    TEST_ASSERT_EQUAL_INT(10, ef.today_seal);
    TEST_ASSERT_EQUAL_INT(0, ef.today_family);
}

void test_from_kin_260_seal_19_polar(void)
{
    /* kin 260: seal = (260-1)%20 = 19 → Sun → 19%5=4 → Polar */
    kin_earth_family_t ef = kin_family_from_kin(260);
    TEST_ASSERT_EQUAL_INT(19, ef.today_seal);
    TEST_ASSERT_EQUAL_INT(4, ef.today_family);
}

/* --- Invalid input --- */

void test_invalid_seal_negative_returns_zeroed(void)
{
    kin_earth_family_t ef = kin_family_compute(-1);
    TEST_ASSERT_EQUAL_INT(0, ef.today_seal);
    TEST_ASSERT_EQUAL_INT(0, ef.today_family);
    TEST_ASSERT_NULL(ef.families[0].name);
}

void test_invalid_seal_20_returns_zeroed(void)
{
    kin_earth_family_t ef = kin_family_compute(20);
    TEST_ASSERT_EQUAL_INT(0, ef.today_seal);
    TEST_ASSERT_NULL(ef.families[0].name);
}

void test_invalid_kin_0_returns_zeroed(void)
{
    kin_earth_family_t ef = kin_family_from_kin(0);
    TEST_ASSERT_EQUAL_INT(0, ef.today_seal);
    TEST_ASSERT_NULL(ef.families[0].name);
}

void test_invalid_kin_261_returns_zeroed(void)
{
    kin_earth_family_t ef = kin_family_from_kin(261);
    TEST_ASSERT_EQUAL_INT(0, ef.today_seal);
    TEST_ASSERT_NULL(ef.families[0].name);
}

/* --- Family member spot checks --- */

void test_cardinal_members_0_5_10_15(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    TEST_ASSERT_EQUAL_INT(0, ef.families[0].seals[0]);
    TEST_ASSERT_EQUAL_INT(5, ef.families[0].seals[1]);
    TEST_ASSERT_EQUAL_INT(10, ef.families[0].seals[2]);
    TEST_ASSERT_EQUAL_INT(15, ef.families[0].seals[3]);
}

void test_polar_members_4_9_14_19(void)
{
    kin_earth_family_t ef = kin_family_compute(0);
    TEST_ASSERT_EQUAL_INT(4, ef.families[4].seals[0]);
    TEST_ASSERT_EQUAL_INT(9, ef.families[4].seals[1]);
    TEST_ASSERT_EQUAL_INT(14, ef.families[4].seals[2]);
    TEST_ASSERT_EQUAL_INT(19, ef.families[4].seals[3]);
}

int main(void)
{
    UNITY_BEGIN();

    /* Today's family assignment */
    RUN_TEST(test_seal_0_dragon_family_cardinal);
    RUN_TEST(test_seal_1_wind_family_core);
    RUN_TEST(test_seal_4_serpent_family_polar);
    RUN_TEST(test_seal_10_monkey_family_cardinal);
    RUN_TEST(test_seal_19_sun_family_polar);

    /* Only one family highlighted */
    RUN_TEST(test_only_one_family_contains_today);

    /* Valid names/directions */
    RUN_TEST(test_all_families_have_valid_names);
    RUN_TEST(test_all_families_have_valid_directions);

    /* Coverage */
    RUN_TEST(test_all_20_seals_covered_no_duplicates);

    /* Pentacle positions */
    RUN_TEST(test_family_0_position_top);
    RUN_TEST(test_family_1_position_upper_right);
    RUN_TEST(test_family_2_position_lower_right);
    RUN_TEST(test_family_3_position_lower_left);
    RUN_TEST(test_family_4_position_upper_left);

    /* kin_family_for_seal */
    RUN_TEST(test_family_for_seal_0);
    RUN_TEST(test_family_for_seal_1);
    RUN_TEST(test_family_for_seal_5);
    RUN_TEST(test_family_for_seal_19);
    RUN_TEST(test_family_for_seal_invalid_negative);
    RUN_TEST(test_family_for_seal_invalid_20);

    /* kin_family_from_kin */
    RUN_TEST(test_from_kin_1_seal_0_cardinal);
    RUN_TEST(test_from_kin_131_seal_10_cardinal);
    RUN_TEST(test_from_kin_260_seal_19_polar);

    /* Invalid input */
    RUN_TEST(test_invalid_seal_negative_returns_zeroed);
    RUN_TEST(test_invalid_seal_20_returns_zeroed);
    RUN_TEST(test_invalid_kin_0_returns_zeroed);
    RUN_TEST(test_invalid_kin_261_returns_zeroed);

    /* Member spot checks */
    RUN_TEST(test_cardinal_members_0_5_10_15);
    RUN_TEST(test_polar_members_4_9_14_19);

    return UNITY_END();
}
