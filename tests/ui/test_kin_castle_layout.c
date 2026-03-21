#include "../unity/unity.h"
#include "../../src/ui/kin_castle_layout.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- kin_castle_compute: kin 1 (start of castle 1) --- */

void test_compute_kin_1_castle_number_is_1(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.castle_number);
}

void test_compute_kin_1_name_contains_red(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_NOT_NULL(c.name);
    TEST_ASSERT_NOT_NULL(strstr(c.name, "Red"));
}

void test_compute_kin_1_name_contains_turning(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_NOT_NULL(c.name);
    TEST_ASSERT_NOT_NULL(strstr(c.name, "Turning"));
}

void test_compute_kin_1_start_kin_is_1(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.start_kin);
}

void test_compute_kin_1_end_kin_is_52(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_EQUAL_INT(52, c.end_kin);
}

/* --- kin 52 (end of castle 1) --- */

void test_compute_kin_52_still_castle_1(void)
{
    kin_castle_layout_t c = kin_castle_compute(52);
    TEST_ASSERT_EQUAL_INT(1, c.castle_number);
    TEST_ASSERT_EQUAL_INT(1, c.start_kin);
    TEST_ASSERT_EQUAL_INT(52, c.end_kin);
}

/* --- kin 53 (start of castle 2) --- */

void test_compute_kin_53_castle_2_white(void)
{
    kin_castle_layout_t c = kin_castle_compute(53);
    TEST_ASSERT_EQUAL_INT(2, c.castle_number);
    TEST_ASSERT_NOT_NULL(c.name);
    TEST_ASSERT_NOT_NULL(strstr(c.name, "White"));
}

/* --- kin 260 (end of castle 5) --- */

void test_compute_kin_260_castle_5_green(void)
{
    kin_castle_layout_t c = kin_castle_compute(260);
    TEST_ASSERT_EQUAL_INT(5, c.castle_number);
    TEST_ASSERT_NOT_NULL(c.name);
    TEST_ASSERT_NOT_NULL(strstr(c.name, "Green"));
}

/* --- 4 wavespells with valid start_kin values --- */

void test_compute_kin_1_wavespell_0_start_kin(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.wavespells[0].start_kin);
}

void test_compute_kin_1_wavespell_1_start_kin(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_EQUAL_INT(14, c.wavespells[1].start_kin);
}

void test_compute_kin_1_wavespell_2_start_kin(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_EQUAL_INT(27, c.wavespells[2].start_kin);
}

void test_compute_kin_1_wavespell_3_start_kin(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_EQUAL_INT(40, c.wavespells[3].start_kin);
}

/* --- today's kin produces exactly 1 wavespell with contains_today --- */

void test_compute_exactly_one_wavespell_contains_today(void)
{
    kin_castle_layout_t c = kin_castle_compute(30);
    int count = 0;
    for (int i = 0; i < KIN_CASTLE_WAVESPELLS; i++) {
        if (c.wavespells[i].contains_today)
            count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

/* --- wavespell Y positions increase from top to bottom --- */

void test_compute_wavespell_y_positions_increase(void)
{
    kin_castle_layout_t c = kin_castle_compute(100);
    for (int i = 0; i < KIN_CASTLE_WAVESPELLS - 1; i++) {
        TEST_ASSERT_TRUE(c.wavespells[i].y < c.wavespells[i + 1].y);
    }
}

void test_compute_wavespell_y_values(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, c.wavespells[0].y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.275f, c.wavespells[1].y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, c.wavespells[2].y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.725f, c.wavespells[3].y);
}

/* --- wavespell geometry: x, w, h --- */

void test_compute_wavespell_geometry(void)
{
    kin_castle_layout_t c = kin_castle_compute(42);
    for (int i = 0; i < KIN_CASTLE_WAVESPELLS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, c.wavespells[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.90f, c.wavespells[i].w);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.20f, c.wavespells[i].h);
    }
}

/* --- today_position is 1-52 --- */

void test_compute_today_position_kin_1_is_1(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_EQUAL_INT(1, c.today_position);
}

void test_compute_today_position_kin_52_is_52(void)
{
    kin_castle_layout_t c = kin_castle_compute(52);
    TEST_ASSERT_EQUAL_INT(52, c.today_position);
}

void test_compute_today_position_kin_53_is_1(void)
{
    kin_castle_layout_t c = kin_castle_compute(53);
    TEST_ASSERT_EQUAL_INT(1, c.today_position);
}

/* --- today_wavespell correct index --- */

void test_compute_today_wavespell_kin_1_is_0(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    TEST_ASSERT_EQUAL_INT(0, c.today_wavespell);
}

void test_compute_today_wavespell_kin_14_is_1(void)
{
    kin_castle_layout_t c = kin_castle_compute(14);
    TEST_ASSERT_EQUAL_INT(1, c.today_wavespell);
}

void test_compute_today_wavespell_kin_40_is_3(void)
{
    kin_castle_layout_t c = kin_castle_compute(40);
    TEST_ASSERT_EQUAL_INT(3, c.today_wavespell);
}

/* --- kin_castle_number --- */

void test_castle_number_kin_1_is_1(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_castle_number(1));
}

void test_castle_number_kin_53_is_2(void)
{
    TEST_ASSERT_EQUAL_INT(2, kin_castle_number(53));
}

void test_castle_number_kin_260_is_5(void)
{
    TEST_ASSERT_EQUAL_INT(5, kin_castle_number(260));
}

void test_castle_number_kin_0_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_castle_number(0));
}

void test_castle_number_kin_261_is_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_castle_number(261));
}

/* --- invalid kin returns zeroed struct --- */

void test_compute_kin_0_returns_zeroed(void)
{
    kin_castle_layout_t c = kin_castle_compute(0);
    TEST_ASSERT_EQUAL_INT(0, c.castle_number);
    TEST_ASSERT_EQUAL_INT(0, c.start_kin);
    TEST_ASSERT_EQUAL_INT(0, c.end_kin);
    TEST_ASSERT_EQUAL_INT(0, c.today_kin);
    TEST_ASSERT_NULL(c.name);
    TEST_ASSERT_NULL(c.color);
    TEST_ASSERT_NULL(c.theme);
}

void test_compute_kin_261_returns_zeroed(void)
{
    kin_castle_layout_t c = kin_castle_compute(261);
    TEST_ASSERT_EQUAL_INT(0, c.castle_number);
    TEST_ASSERT_NULL(c.name);
}

/* --- castle theme not NULL --- */

void test_compute_theme_not_null(void)
{
    for (int castle = 1; castle <= 5; castle++) {
        int kin = (castle - 1) * 52 + 1;
        kin_castle_layout_t c = kin_castle_compute(kin);
        TEST_ASSERT_NOT_NULL(c.theme);
    }
}

/* --- wavespell purpose not NULL --- */

void test_compute_wavespell_purpose_not_null(void)
{
    kin_castle_layout_t c = kin_castle_compute(100);
    for (int i = 0; i < KIN_CASTLE_WAVESPELLS; i++) {
        TEST_ASSERT_NOT_NULL(c.wavespells[i].purpose);
    }
}

/* --- wavespell numbers are consecutive --- */

void test_compute_wavespell_numbers_consecutive(void)
{
    kin_castle_layout_t c = kin_castle_compute(1);
    for (int i = 0; i < KIN_CASTLE_WAVESPELLS - 1; i++) {
        TEST_ASSERT_EQUAL_INT(c.wavespells[i].wavespell_number + 1,
                              c.wavespells[i + 1].wavespell_number);
    }
}

/* --- purity: same input, same output --- */

void test_purity_same_input_same_output(void)
{
    kin_castle_layout_t a = kin_castle_compute(77);
    kin_castle_layout_t b = kin_castle_compute(77);
    TEST_ASSERT_EQUAL_INT(a.castle_number, b.castle_number);
    TEST_ASSERT_EQUAL_INT(a.today_kin, b.today_kin);
    TEST_ASSERT_EQUAL_INT(a.today_position, b.today_position);
    TEST_ASSERT_EQUAL_INT(a.today_wavespell, b.today_wavespell);
    for (int i = 0; i < KIN_CASTLE_WAVESPELLS; i++) {
        TEST_ASSERT_EQUAL_INT(a.wavespells[i].start_kin,
                              b.wavespells[i].start_kin);
        TEST_ASSERT_EQUAL_INT(a.wavespells[i].contains_today,
                              b.wavespells[i].contains_today);
    }
}

/* --- today_kin stored correctly --- */

void test_compute_today_kin_stored(void)
{
    kin_castle_layout_t c = kin_castle_compute(137);
    TEST_ASSERT_EQUAL_INT(137, c.today_kin);
}

int main(void)
{
    UNITY_BEGIN();
    /* castle 1: kin 1 */
    RUN_TEST(test_compute_kin_1_castle_number_is_1);
    RUN_TEST(test_compute_kin_1_name_contains_red);
    RUN_TEST(test_compute_kin_1_name_contains_turning);
    RUN_TEST(test_compute_kin_1_start_kin_is_1);
    RUN_TEST(test_compute_kin_1_end_kin_is_52);
    /* castle 1 boundary */
    RUN_TEST(test_compute_kin_52_still_castle_1);
    /* castle 2 */
    RUN_TEST(test_compute_kin_53_castle_2_white);
    /* castle 5 */
    RUN_TEST(test_compute_kin_260_castle_5_green);
    /* wavespell start_kin values */
    RUN_TEST(test_compute_kin_1_wavespell_0_start_kin);
    RUN_TEST(test_compute_kin_1_wavespell_1_start_kin);
    RUN_TEST(test_compute_kin_1_wavespell_2_start_kin);
    RUN_TEST(test_compute_kin_1_wavespell_3_start_kin);
    /* contains_today */
    RUN_TEST(test_compute_exactly_one_wavespell_contains_today);
    /* wavespell geometry */
    RUN_TEST(test_compute_wavespell_y_positions_increase);
    RUN_TEST(test_compute_wavespell_y_values);
    RUN_TEST(test_compute_wavespell_geometry);
    /* today_position */
    RUN_TEST(test_compute_today_position_kin_1_is_1);
    RUN_TEST(test_compute_today_position_kin_52_is_52);
    RUN_TEST(test_compute_today_position_kin_53_is_1);
    /* today_wavespell */
    RUN_TEST(test_compute_today_wavespell_kin_1_is_0);
    RUN_TEST(test_compute_today_wavespell_kin_14_is_1);
    RUN_TEST(test_compute_today_wavespell_kin_40_is_3);
    /* kin_castle_number */
    RUN_TEST(test_castle_number_kin_1_is_1);
    RUN_TEST(test_castle_number_kin_53_is_2);
    RUN_TEST(test_castle_number_kin_260_is_5);
    RUN_TEST(test_castle_number_kin_0_is_0);
    RUN_TEST(test_castle_number_kin_261_is_0);
    /* invalid */
    RUN_TEST(test_compute_kin_0_returns_zeroed);
    RUN_TEST(test_compute_kin_261_returns_zeroed);
    /* theme/purpose not NULL */
    RUN_TEST(test_compute_theme_not_null);
    RUN_TEST(test_compute_wavespell_purpose_not_null);
    /* wavespell numbers consecutive */
    RUN_TEST(test_compute_wavespell_numbers_consecutive);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    /* today_kin stored */
    RUN_TEST(test_compute_today_kin_stored);
    return UNITY_END();
}
