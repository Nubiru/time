#include "../unity/unity.h"
#include "../../src/ui/kin_cell.h"

void setUp(void) {}
void tearDown(void) {}

/* --- kin_cell_from_kin --- */

void test_from_kin_1_red_dragon_tone_1(void)
{
    kin_cell_t c = kin_cell_from_kin(1);
    TEST_ASSERT_EQUAL_INT(1, c.kin);
    TEST_ASSERT_EQUAL_INT(0, c.seal);    /* Dragon */
    TEST_ASSERT_EQUAL_INT(1, c.tone);    /* Magnetic */
    TEST_ASSERT_EQUAL_INT(0, c.color);   /* Red */
    TEST_ASSERT_EQUAL_INT(0, c.highlighted);
}

void test_from_kin_260_yellow_sun_tone_13(void)
{
    kin_cell_t c = kin_cell_from_kin(260);
    TEST_ASSERT_EQUAL_INT(260, c.kin);
    TEST_ASSERT_EQUAL_INT(19, c.seal);   /* Sun */
    TEST_ASSERT_EQUAL_INT(13, c.tone);   /* Cosmic */
    TEST_ASSERT_EQUAL_INT(3, c.color);   /* Yellow */
}

void test_from_kin_131_blue_monkey_tone_1(void)
{
    /* kin 131: seal = (131-1)%20 = 130%20 = 10 (Monkey), tone = 130%13+1 = 1 */
    kin_cell_t c = kin_cell_from_kin(131);
    TEST_ASSERT_EQUAL_INT(131, c.kin);
    TEST_ASSERT_EQUAL_INT(10, c.seal);   /* Monkey */
    TEST_ASSERT_EQUAL_INT(1, c.tone);    /* Magnetic */
    TEST_ASSERT_EQUAL_INT(2, c.color);   /* Blue (10%4=2) */
}

void test_from_kin_defaults_position_zero(void)
{
    kin_cell_t c = kin_cell_from_kin(42);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.h);
}

void test_from_kin_highlighted_default_zero(void)
{
    kin_cell_t c = kin_cell_from_kin(100);
    TEST_ASSERT_EQUAL_INT(0, c.highlighted);
}

/* --- Invalid kin --- */

void test_from_kin_zero_returns_zeroed(void)
{
    kin_cell_t c = kin_cell_from_kin(0);
    TEST_ASSERT_EQUAL_INT(0, c.kin);
    TEST_ASSERT_EQUAL_INT(0, c.seal);
    TEST_ASSERT_EQUAL_INT(0, c.tone);
    TEST_ASSERT_EQUAL_INT(0, c.color);
}

void test_from_kin_261_returns_zeroed(void)
{
    kin_cell_t c = kin_cell_from_kin(261);
    TEST_ASSERT_EQUAL_INT(0, c.kin);
}

void test_from_kin_negative_returns_zeroed(void)
{
    kin_cell_t c = kin_cell_from_kin(-5);
    TEST_ASSERT_EQUAL_INT(0, c.kin);
}

/* --- kin_cell_create --- */

void test_create_sets_position_and_size(void)
{
    kin_cell_t c = kin_cell_create(1, 0.5f, 0.5f, 0.25f, 0.20f);
    TEST_ASSERT_EQUAL_INT(1, c.kin);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, c.x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, c.y);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, c.w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.20f, c.h);
}

void test_create_invalid_kin_returns_zeroed(void)
{
    kin_cell_t c = kin_cell_create(0, 0.5f, 0.5f, 0.25f, 0.20f);
    TEST_ASSERT_EQUAL_INT(0, c.kin);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.x);
}

/* --- kin_cell_color --- */

void test_color_red(void)
{
    TEST_ASSERT_EQUAL_INT(0, kin_cell_color(0));   /* Dragon → Red */
}

void test_color_white(void)
{
    TEST_ASSERT_EQUAL_INT(1, kin_cell_color(1));   /* Wind → White */
}

void test_color_blue(void)
{
    TEST_ASSERT_EQUAL_INT(2, kin_cell_color(2));   /* Night → Blue */
}

void test_color_yellow(void)
{
    TEST_ASSERT_EQUAL_INT(3, kin_cell_color(3));   /* Seed → Yellow */
}

void test_color_seal_19_yellow(void)
{
    TEST_ASSERT_EQUAL_INT(3, kin_cell_color(19));  /* Sun → 19%4=3 Yellow */
}

void test_color_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_cell_color(-1));
}

void test_color_invalid_20(void)
{
    TEST_ASSERT_EQUAL_INT(-1, kin_cell_color(20));
}

/* --- kin_cell_rgba --- */

void test_rgba_red(void)
{
    float rgba[4] = {0};
    int ok = kin_cell_rgba(0, rgba);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.85f, rgba[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.20f, rgba[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.20f, rgba[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.00f, rgba[3]);
}

void test_rgba_white(void)
{
    float rgba[4] = {0};
    int ok = kin_cell_rgba(1, rgba);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.90f, rgba[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.90f, rgba[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.90f, rgba[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.00f, rgba[3]);
}

void test_rgba_blue(void)
{
    float rgba[4] = {0};
    int ok = kin_cell_rgba(2, rgba);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.20f, rgba[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.40f, rgba[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.80f, rgba[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.00f, rgba[3]);
}

void test_rgba_yellow(void)
{
    float rgba[4] = {0};
    int ok = kin_cell_rgba(3, rgba);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.90f, rgba[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.80f, rgba[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.20f, rgba[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.00f, rgba[3]);
}

void test_rgba_invalid_returns_zero(void)
{
    float rgba[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    int ok = kin_cell_rgba(-1, rgba);
    TEST_ASSERT_EQUAL_INT(0, ok);
    /* rgba should be untouched */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, rgba[0]);
}

void test_rgba_invalid_high_returns_zero(void)
{
    float rgba[4] = {0};
    int ok = kin_cell_rgba(4, rgba);
    TEST_ASSERT_EQUAL_INT(0, ok);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    kin_cell_t a = kin_cell_from_kin(207);
    kin_cell_t b = kin_cell_from_kin(207);
    TEST_ASSERT_EQUAL_INT(a.kin, b.kin);
    TEST_ASSERT_EQUAL_INT(a.seal, b.seal);
    TEST_ASSERT_EQUAL_INT(a.tone, b.tone);
    TEST_ASSERT_EQUAL_INT(a.color, b.color);
}

int main(void)
{
    UNITY_BEGIN();
    /* from_kin */
    RUN_TEST(test_from_kin_1_red_dragon_tone_1);
    RUN_TEST(test_from_kin_260_yellow_sun_tone_13);
    RUN_TEST(test_from_kin_131_blue_monkey_tone_1);
    RUN_TEST(test_from_kin_defaults_position_zero);
    RUN_TEST(test_from_kin_highlighted_default_zero);
    /* invalid kin */
    RUN_TEST(test_from_kin_zero_returns_zeroed);
    RUN_TEST(test_from_kin_261_returns_zeroed);
    RUN_TEST(test_from_kin_negative_returns_zeroed);
    /* create */
    RUN_TEST(test_create_sets_position_and_size);
    RUN_TEST(test_create_invalid_kin_returns_zeroed);
    /* color */
    RUN_TEST(test_color_red);
    RUN_TEST(test_color_white);
    RUN_TEST(test_color_blue);
    RUN_TEST(test_color_yellow);
    RUN_TEST(test_color_seal_19_yellow);
    RUN_TEST(test_color_invalid_negative);
    RUN_TEST(test_color_invalid_20);
    /* rgba */
    RUN_TEST(test_rgba_red);
    RUN_TEST(test_rgba_white);
    RUN_TEST(test_rgba_blue);
    RUN_TEST(test_rgba_yellow);
    RUN_TEST(test_rgba_invalid_returns_zero);
    RUN_TEST(test_rgba_invalid_high_returns_zero);
    /* purity */
    RUN_TEST(test_purity_same_input_same_output);
    return UNITY_END();
}
