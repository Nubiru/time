#include "../unity/unity.h"
#include "../../src/ui/kin_color_compass.h"

void setUp(void) {}
void tearDown(void) {}

/* --- seal 0 (Dragon): Red, East --- */

void test_compute_seal_0_today_color_is_red(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_INT(0, c.today_color);
}

void test_compute_seal_0_red_quadrant_contains_today(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_INT(1, c.quadrants[0].contains_today);
}

void test_compute_seal_0_today_seal(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_INT(0, c.today_seal);
}

/* --- seal 1 (Wind): White, North --- */

void test_compute_seal_1_today_color_is_white(void)
{
    kin_color_compass_t c = kin_compass_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.today_color);
}

void test_compute_seal_1_white_quadrant_contains_today(void)
{
    kin_color_compass_t c = kin_compass_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.quadrants[1].contains_today);
}

/* --- Red quadrant: East, Initiates, seals {0,4,8,12,16} --- */

void test_red_quadrant_direction_east(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_STRING("East", c.quadrants[0].direction);
}

void test_red_quadrant_action_initiates(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_STRING("Initiates", c.quadrants[0].action);
}

void test_red_quadrant_seals(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_INT(0, c.quadrants[0].seals[0]);
    TEST_ASSERT_EQUAL_INT(4, c.quadrants[0].seals[1]);
    TEST_ASSERT_EQUAL_INT(8, c.quadrants[0].seals[2]);
    TEST_ASSERT_EQUAL_INT(12, c.quadrants[0].seals[3]);
    TEST_ASSERT_EQUAL_INT(16, c.quadrants[0].seals[4]);
}

/* --- White quadrant: North, Refines, seals {1,5,9,13,17} --- */

void test_white_quadrant_direction_north(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_STRING("North", c.quadrants[1].direction);
}

void test_white_quadrant_seals(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_INT(1, c.quadrants[1].seals[0]);
    TEST_ASSERT_EQUAL_INT(5, c.quadrants[1].seals[1]);
    TEST_ASSERT_EQUAL_INT(9, c.quadrants[1].seals[2]);
    TEST_ASSERT_EQUAL_INT(13, c.quadrants[1].seals[3]);
    TEST_ASSERT_EQUAL_INT(17, c.quadrants[1].seals[4]);
}

/* --- Blue quadrant: West, Transforms, seals {2,6,10,14,18} --- */

void test_blue_quadrant_direction_west(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_STRING("West", c.quadrants[2].direction);
}

void test_blue_quadrant_seals(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_INT(2, c.quadrants[2].seals[0]);
    TEST_ASSERT_EQUAL_INT(6, c.quadrants[2].seals[1]);
    TEST_ASSERT_EQUAL_INT(10, c.quadrants[2].seals[2]);
    TEST_ASSERT_EQUAL_INT(14, c.quadrants[2].seals[3]);
    TEST_ASSERT_EQUAL_INT(18, c.quadrants[2].seals[4]);
}

/* --- Yellow quadrant: South, Ripens, seals {3,7,11,15,19} --- */

void test_yellow_quadrant_direction_south(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_STRING("South", c.quadrants[3].direction);
}

void test_yellow_quadrant_seals(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_EQUAL_INT(3, c.quadrants[3].seals[0]);
    TEST_ASSERT_EQUAL_INT(7, c.quadrants[3].seals[1]);
    TEST_ASSERT_EQUAL_INT(11, c.quadrants[3].seals[2]);
    TEST_ASSERT_EQUAL_INT(15, c.quadrants[3].seals[3]);
    TEST_ASSERT_EQUAL_INT(19, c.quadrants[3].seals[4]);
}

/* --- Positions: Red at (0.85, 0.5), White at (0.5, 0.15) --- */

void test_red_quadrant_position(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.85f, c.quadrants[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, c.quadrants[0].y);
}

void test_white_quadrant_position(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, c.quadrants[1].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, c.quadrants[1].y);
}

void test_blue_quadrant_position(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, c.quadrants[2].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, c.quadrants[2].y);
}

void test_yellow_quadrant_position(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, c.quadrants[3].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.85f, c.quadrants[3].y);
}

/* --- Angles: Red=90, White=0, Blue=270, Yellow=180 --- */

void test_quadrant_angles(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 90.0f, c.quadrants[0].angle);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.quadrants[1].angle);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 270.0f, c.quadrants[2].angle);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 180.0f, c.quadrants[3].angle);
}

/* --- Only 1 quadrant has contains_today=1 --- */

void test_compute_exactly_one_quadrant_contains_today(void)
{
    for (int seal = 0; seal < 20; seal++) {
        kin_color_compass_t c = kin_compass_compute(seal);
        int count = 0;
        for (int i = 0; i < KIN_COMPASS_DIRECTIONS; i++) {
            if (c.quadrants[i].contains_today)
                count++;
        }
        TEST_ASSERT_EQUAL_INT(1, count);
    }
}

/* --- All 20 seals covered across 4 quadrants (no duplicates) --- */

void test_all_20_seals_covered_no_duplicates(void)
{
    kin_color_compass_t c = kin_compass_compute(0);
    int seen[20];
    for (int i = 0; i < 20; i++)
        seen[i] = 0;

    for (int q = 0; q < KIN_COMPASS_DIRECTIONS; q++) {
        for (int s = 0; s < KIN_COMPASS_SEALS_PER; s++) {
            int seal = c.quadrants[q].seals[s];
            TEST_ASSERT_TRUE(seal >= 0 && seal <= 19);
            TEST_ASSERT_EQUAL_INT(0, seen[seal]); /* no duplicate */
            seen[seal] = 1;
        }
    }

    /* All 20 seen */
    for (int i = 0; i < 20; i++) {
        TEST_ASSERT_EQUAL_INT(1, seen[i]);
    }
}

/* --- kin_compass_from_kin(1) works: seal=0, Red --- */

void test_from_kin_1_seal_0_red(void)
{
    kin_color_compass_t c = kin_compass_from_kin(1);
    TEST_ASSERT_EQUAL_INT(0, c.today_seal);
    TEST_ASSERT_EQUAL_INT(0, c.today_color);
}

/* --- kin_compass_from_kin(131) → seal=10, Blue --- */

void test_from_kin_131_seal_10_blue(void)
{
    /* seal = (131-1) % 20 = 130 % 20 = 10 */
    kin_color_compass_t c = kin_compass_from_kin(131);
    TEST_ASSERT_EQUAL_INT(10, c.today_seal);
    TEST_ASSERT_EQUAL_INT(2, c.today_color); /* Blue */
}

/* --- invalid seal (-1, 20) returns zeroed struct --- */

void test_compute_invalid_seal_neg1_zeroed(void)
{
    kin_color_compass_t c = kin_compass_compute(-1);
    TEST_ASSERT_EQUAL_INT(0, c.today_seal);
    TEST_ASSERT_EQUAL_INT(0, c.today_color);
    TEST_ASSERT_NULL(c.quadrants[0].name);
}

void test_compute_invalid_seal_20_zeroed(void)
{
    kin_color_compass_t c = kin_compass_compute(20);
    TEST_ASSERT_EQUAL_INT(0, c.today_seal);
    TEST_ASSERT_NULL(c.quadrants[0].name);
}

/* --- invalid kin returns zeroed struct --- */

void test_from_kin_0_zeroed(void)
{
    kin_color_compass_t c = kin_compass_from_kin(0);
    TEST_ASSERT_EQUAL_INT(0, c.today_seal);
    TEST_ASSERT_NULL(c.quadrants[0].name);
}

void test_from_kin_261_zeroed(void)
{
    kin_color_compass_t c = kin_compass_from_kin(261);
    TEST_ASSERT_EQUAL_INT(0, c.today_seal);
    TEST_ASSERT_NULL(c.quadrants[0].name);
}

/* --- compass names and actions are not NULL --- */

void test_compass_names_not_null(void)
{
    kin_color_compass_t c = kin_compass_compute(5);
    for (int i = 0; i < KIN_COMPASS_DIRECTIONS; i++) {
        TEST_ASSERT_NOT_NULL(c.quadrants[i].name);
        TEST_ASSERT_NOT_NULL(c.quadrants[i].direction);
        TEST_ASSERT_NOT_NULL(c.quadrants[i].action);
    }
}

/* --- purity: same input, same output --- */

void test_purity_same_input_same_output(void)
{
    kin_color_compass_t a = kin_compass_compute(7);
    kin_color_compass_t b = kin_compass_compute(7);
    TEST_ASSERT_EQUAL_INT(a.today_seal, b.today_seal);
    TEST_ASSERT_EQUAL_INT(a.today_color, b.today_color);
    for (int i = 0; i < KIN_COMPASS_DIRECTIONS; i++) {
        TEST_ASSERT_EQUAL_INT(a.quadrants[i].color, b.quadrants[i].color);
        TEST_ASSERT_EQUAL_INT(a.quadrants[i].contains_today,
                              b.quadrants[i].contains_today);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.quadrants[i].x, b.quadrants[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.quadrants[i].y, b.quadrants[i].y);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* seal 0 (Dragon): Red */
    RUN_TEST(test_compute_seal_0_today_color_is_red);
    RUN_TEST(test_compute_seal_0_red_quadrant_contains_today);
    RUN_TEST(test_compute_seal_0_today_seal);
    /* seal 1 (Wind): White */
    RUN_TEST(test_compute_seal_1_today_color_is_white);
    RUN_TEST(test_compute_seal_1_white_quadrant_contains_today);
    /* Red quadrant */
    RUN_TEST(test_red_quadrant_direction_east);
    RUN_TEST(test_red_quadrant_action_initiates);
    RUN_TEST(test_red_quadrant_seals);
    /* White quadrant */
    RUN_TEST(test_white_quadrant_direction_north);
    RUN_TEST(test_white_quadrant_seals);
    /* Blue quadrant */
    RUN_TEST(test_blue_quadrant_direction_west);
    RUN_TEST(test_blue_quadrant_seals);
    /* Yellow quadrant */
    RUN_TEST(test_yellow_quadrant_direction_south);
    RUN_TEST(test_yellow_quadrant_seals);
    /* positions */
    RUN_TEST(test_red_quadrant_position);
    RUN_TEST(test_white_quadrant_position);
    RUN_TEST(test_blue_quadrant_position);
    RUN_TEST(test_yellow_quadrant_position);
    /* angles */
    RUN_TEST(test_quadrant_angles);
    /* contains_today uniqueness */
    RUN_TEST(test_compute_exactly_one_quadrant_contains_today);
    /* all 20 seals covered */
    RUN_TEST(test_all_20_seals_covered_no_duplicates);
    /* from_kin */
    RUN_TEST(test_from_kin_1_seal_0_red);
    RUN_TEST(test_from_kin_131_seal_10_blue);
    /* invalid */
    RUN_TEST(test_compute_invalid_seal_neg1_zeroed);
    RUN_TEST(test_compute_invalid_seal_20_zeroed);
    RUN_TEST(test_from_kin_0_zeroed);
    RUN_TEST(test_from_kin_261_zeroed);
    /* names not null */
    RUN_TEST(test_compass_names_not_null);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
