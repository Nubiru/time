#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/tzolkin_board.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Kin formula: tone and seal from kin number --- */

static void test_kin1_is_tone1_seal0(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(1);
    TEST_ASSERT_EQUAL_INT(1, k.kin);
    TEST_ASSERT_EQUAL_INT(1, k.tone);
    TEST_ASSERT_EQUAL_INT(0, k.seal);
}

static void test_kin2_is_tone2_seal1(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(2);
    TEST_ASSERT_EQUAL_INT(2, k.tone);
    TEST_ASSERT_EQUAL_INT(1, k.seal);
}

static void test_kin13_is_tone13_seal12(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(13);
    TEST_ASSERT_EQUAL_INT(13, k.tone);
    TEST_ASSERT_EQUAL_INT(12, k.seal);
}

static void test_kin14_is_tone1_seal13(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(14);
    TEST_ASSERT_EQUAL_INT(1, k.tone);
    TEST_ASSERT_EQUAL_INT(13, k.seal);
}

static void test_kin20_is_tone7_seal19(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(20);
    TEST_ASSERT_EQUAL_INT(7, k.tone);
    TEST_ASSERT_EQUAL_INT(19, k.seal);
}

static void test_kin260_is_tone13_seal19(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(260);
    TEST_ASSERT_EQUAL_INT(13, k.tone);
    TEST_ASSERT_EQUAL_INT(19, k.seal);
}

/* --- Invalid kin numbers --- */

static void test_kin0_returns_zeroed(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(0);
    TEST_ASSERT_EQUAL_INT(0, k.kin);
    TEST_ASSERT_EQUAL_INT(0, k.tone);
}

static void test_kin261_returns_zeroed(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(261);
    TEST_ASSERT_EQUAL_INT(0, k.kin);
}

static void test_negative_kin_returns_zeroed(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(-5);
    TEST_ASSERT_EQUAL_INT(0, k.kin);
}

/* --- CRT: tone+seal to kin --- */

static void test_crt_tone1_seal0_gives_kin1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tzolkin_board_kin_number(1, 0));
}

static void test_crt_tone2_seal1_gives_kin2(void)
{
    TEST_ASSERT_EQUAL_INT(2, tzolkin_board_kin_number(2, 1));
}

static void test_crt_tone13_seal12_gives_kin13(void)
{
    TEST_ASSERT_EQUAL_INT(13, tzolkin_board_kin_number(13, 12));
}

static void test_crt_tone1_seal13_gives_kin14(void)
{
    TEST_ASSERT_EQUAL_INT(14, tzolkin_board_kin_number(1, 13));
}

static void test_crt_tone7_seal19_gives_kin20(void)
{
    TEST_ASSERT_EQUAL_INT(20, tzolkin_board_kin_number(7, 19));
}

static void test_crt_tone13_seal19_gives_kin260(void)
{
    TEST_ASSERT_EQUAL_INT(260, tzolkin_board_kin_number(13, 19));
}

static void test_crt_invalid_tone_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tzolkin_board_kin_number(0, 0));
    TEST_ASSERT_EQUAL_INT(0, tzolkin_board_kin_number(14, 0));
}

static void test_crt_invalid_seal_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tzolkin_board_kin_number(1, -1));
    TEST_ASSERT_EQUAL_INT(0, tzolkin_board_kin_number(1, 20));
}

/* --- CRT roundtrip for all 260 kins --- */

static void test_crt_roundtrip_all_260(void)
{
    for (int n = 1; n <= 260; n++) {
        tzolkin_kin_t k = tzolkin_board_kin(n);
        int recovered = tzolkin_board_kin_number(k.tone, k.seal);
        TEST_ASSERT_EQUAL_INT_MESSAGE(n, recovered,
            "CRT roundtrip failed");
    }
}

/* --- by_tone_seal --- */

static void test_by_tone_seal_matches_kin(void)
{
    tzolkin_kin_t a = tzolkin_board_kin(42);
    tzolkin_kin_t b = tzolkin_board_by_tone_seal(a.tone, a.seal);
    TEST_ASSERT_EQUAL_INT(a.kin, b.kin);
    TEST_ASSERT_EQUAL_INT(a.tone, b.tone);
    TEST_ASSERT_EQUAL_INT(a.seal, b.seal);
}

/* --- Color --- */

static void test_seal0_is_red(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_COLOR_RED,
                          tzolkin_board_seal_color(0));
}

static void test_seal1_is_white(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_COLOR_WHITE,
                          tzolkin_board_seal_color(1));
}

static void test_seal2_is_blue(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_COLOR_BLUE,
                          tzolkin_board_seal_color(2));
}

static void test_seal3_is_yellow(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_COLOR_YELLOW,
                          tzolkin_board_seal_color(3));
}

static void test_seal4_cycles_back_to_red(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_COLOR_RED,
                          tzolkin_board_seal_color(4));
}

static void test_kin1_color_red_dragon(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(1);
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_COLOR_RED, k.color);
}

static void test_kin2_color_white_wind(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(2);
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_COLOR_WHITE, k.color);
}

/* --- Wavespell --- */

static void test_kin1_wavespell1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tzolkin_board_wavespell(1));
}

static void test_kin13_wavespell1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tzolkin_board_wavespell(13));
}

static void test_kin14_wavespell2(void)
{
    TEST_ASSERT_EQUAL_INT(2, tzolkin_board_wavespell(14));
}

static void test_kin260_wavespell20(void)
{
    TEST_ASSERT_EQUAL_INT(20, tzolkin_board_wavespell(260));
}

static void test_wavespell_position_kin1(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(1);
    TEST_ASSERT_EQUAL_INT(1, k.wavespell_position);
}

static void test_wavespell_position_kin13(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(13);
    TEST_ASSERT_EQUAL_INT(13, k.wavespell_position);
}

static void test_wavespell_position_kin14(void)
{
    tzolkin_kin_t k = tzolkin_board_kin(14);
    TEST_ASSERT_EQUAL_INT(1, k.wavespell_position);
}

/* --- Castle --- */

static void test_kin1_castle_red(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_CASTLE_RED,
                          tzolkin_board_castle(1));
}

static void test_kin52_castle_red(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_CASTLE_RED,
                          tzolkin_board_castle(52));
}

static void test_kin53_castle_white(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_CASTLE_WHITE,
                          tzolkin_board_castle(53));
}

static void test_kin104_castle_white(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_CASTLE_WHITE,
                          tzolkin_board_castle(104));
}

static void test_kin105_castle_blue(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_CASTLE_BLUE,
                          tzolkin_board_castle(105));
}

static void test_kin209_castle_green(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_CASTLE_GREEN,
                          tzolkin_board_castle(209));
}

static void test_kin260_castle_green(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BOARD_CASTLE_GREEN,
                          tzolkin_board_castle(260));
}

/* --- Harmonic --- */

static void test_kin1_harmonic1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tzolkin_board_harmonic(1));
}

static void test_kin4_harmonic1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tzolkin_board_harmonic(4));
}

static void test_kin5_harmonic2(void)
{
    TEST_ASSERT_EQUAL_INT(2, tzolkin_board_harmonic(5));
}

static void test_kin260_harmonic65(void)
{
    TEST_ASSERT_EQUAL_INT(65, tzolkin_board_harmonic(260));
}

/* --- Portal days --- */

static void test_kin1_is_portal(void)
{
    TEST_ASSERT_TRUE(tzolkin_board_is_portal(1));
}

static void test_kin20_is_portal(void)
{
    TEST_ASSERT_TRUE(tzolkin_board_is_portal(20));
}

static void test_kin260_is_portal(void)
{
    TEST_ASSERT_TRUE(tzolkin_board_is_portal(260));
}

static void test_kin241_is_portal(void)
{
    TEST_ASSERT_TRUE(tzolkin_board_is_portal(241));
}

static void test_kin2_is_not_portal(void)
{
    TEST_ASSERT_FALSE(tzolkin_board_is_portal(2));
}

static void test_kin10_is_not_portal(void)
{
    TEST_ASSERT_FALSE(tzolkin_board_is_portal(10));
}

static void test_portal_count_is_52(void)
{
    int count = 0;
    for (int n = 1; n <= 260; n++) {
        if (tzolkin_board_is_portal(n))
            count++;
    }
    TEST_ASSERT_EQUAL_INT(52, count);
}

static void test_portal_count_func(void)
{
    TEST_ASSERT_EQUAL_INT(52, tzolkin_board_portal_count());
}

static void test_portal_symmetry(void)
{
    /* The pattern is symmetric on the 13x20 harmonic grid:
     * col c <-> col 12-c, row r <-> row 19-r */
    for (int n = 1; n <= 260; n++) {
        int col = (n - 1) / 20;
        int row = (n - 1) % 20;
        int mirror_col = 12 - col;
        int mirror_row = 19 - row;
        int mirror_kin = mirror_col * 20 + mirror_row + 1;
        TEST_ASSERT_EQUAL_MESSAGE(
            tzolkin_board_is_portal(n),
            tzolkin_board_is_portal(mirror_kin),
            "Portal symmetry broken");
    }
}

static void test_portal_col5_band(void)
{
    /* Column 5 (kins 101-120): rows 5-14 are all portals */
    for (int row = 0; row < 20; row++) {
        int kin = 5 * 20 + row + 1; /* 101-120 */
        if (row >= 5 && row <= 14) {
            TEST_ASSERT_TRUE_MESSAGE(tzolkin_board_is_portal(kin),
                "Col 5 band should be portal");
        } else {
            TEST_ASSERT_FALSE_MESSAGE(tzolkin_board_is_portal(kin),
                "Col 5 outside band should not be portal");
        }
    }
}

static void test_portal_center_column_empty(void)
{
    /* Column 6 (kins 121-140): no portals */
    for (int row = 0; row < 20; row++) {
        int kin = 6 * 20 + row + 1;
        TEST_ASSERT_FALSE_MESSAGE(tzolkin_board_is_portal(kin),
            "Center column should have no portals");
    }
}

static void test_portal_col2_four_portals(void)
{
    /* Column 2 (kins 41-60): portals at rows 2, 9, 10, 17 */
    int expected[] = {43, 50, 51, 58};
    int count = 0;
    for (int row = 0; row < 20; row++) {
        int kin = 2 * 20 + row + 1;
        if (tzolkin_board_is_portal(kin))
            count++;
    }
    TEST_ASSERT_EQUAL_INT(4, count);
    for (int i = 0; i < 4; i++)
        TEST_ASSERT_TRUE(tzolkin_board_is_portal(expected[i]));
}

static void test_portal_invalid_kin(void)
{
    TEST_ASSERT_FALSE(tzolkin_board_is_portal(0));
    TEST_ASSERT_FALSE(tzolkin_board_is_portal(261));
    TEST_ASSERT_FALSE(tzolkin_board_is_portal(-1));
}

/* --- Name strings --- */

static void test_color_name_red(void)
{
    TEST_ASSERT_EQUAL_STRING("Red",
        tzolkin_board_color_name(TZOLKIN_BOARD_COLOR_RED));
}

static void test_color_name_yellow(void)
{
    TEST_ASSERT_EQUAL_STRING("Yellow",
        tzolkin_board_color_name(TZOLKIN_BOARD_COLOR_YELLOW));
}

static void test_castle_name_red(void)
{
    TEST_ASSERT_EQUAL_STRING("Red Castle of Turning",
        tzolkin_board_castle_name(TZOLKIN_BOARD_CASTLE_RED));
}

static void test_castle_name_green(void)
{
    TEST_ASSERT_EQUAL_STRING("Green Castle of Enchantment",
        tzolkin_board_castle_name(TZOLKIN_BOARD_CASTLE_GREEN));
}

/* --- Invalid inputs for helper functions --- */

static void test_wavespell_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tzolkin_board_wavespell(0));
    TEST_ASSERT_EQUAL_INT(0, tzolkin_board_wavespell(261));
}

static void test_harmonic_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(0, tzolkin_board_harmonic(0));
    TEST_ASSERT_EQUAL_INT(0, tzolkin_board_harmonic(261));
}

/* --- Comprehensive sweep --- */

static void test_all_260_kins_valid(void)
{
    for (int n = 1; n <= 260; n++) {
        tzolkin_kin_t k = tzolkin_board_kin(n);
        TEST_ASSERT_EQUAL_INT(n, k.kin);
        TEST_ASSERT_TRUE(k.tone >= 1 && k.tone <= 13);
        TEST_ASSERT_TRUE(k.seal >= 0 && k.seal <= 19);
        TEST_ASSERT_TRUE(k.color >= 0 &&
                         k.color < TZOLKIN_BOARD_COLOR_COUNT);
        TEST_ASSERT_TRUE(k.wavespell >= 1 && k.wavespell <= 20);
        TEST_ASSERT_TRUE(k.wavespell_position >= 1 &&
                         k.wavespell_position <= 13);
        TEST_ASSERT_TRUE(k.castle >= 0 &&
                         k.castle < TZOLKIN_BOARD_CASTLE_COUNT);
        TEST_ASSERT_TRUE(k.harmonic >= 1 && k.harmonic <= 65);
    }
}

int main(void)
{
    UNITY_BEGIN();

    /* Kin formula */
    RUN_TEST(test_kin1_is_tone1_seal0);
    RUN_TEST(test_kin2_is_tone2_seal1);
    RUN_TEST(test_kin13_is_tone13_seal12);
    RUN_TEST(test_kin14_is_tone1_seal13);
    RUN_TEST(test_kin20_is_tone7_seal19);
    RUN_TEST(test_kin260_is_tone13_seal19);

    /* Invalid kin */
    RUN_TEST(test_kin0_returns_zeroed);
    RUN_TEST(test_kin261_returns_zeroed);
    RUN_TEST(test_negative_kin_returns_zeroed);

    /* CRT */
    RUN_TEST(test_crt_tone1_seal0_gives_kin1);
    RUN_TEST(test_crt_tone2_seal1_gives_kin2);
    RUN_TEST(test_crt_tone13_seal12_gives_kin13);
    RUN_TEST(test_crt_tone1_seal13_gives_kin14);
    RUN_TEST(test_crt_tone7_seal19_gives_kin20);
    RUN_TEST(test_crt_tone13_seal19_gives_kin260);
    RUN_TEST(test_crt_invalid_tone_returns_0);
    RUN_TEST(test_crt_invalid_seal_returns_0);
    RUN_TEST(test_crt_roundtrip_all_260);

    /* by_tone_seal */
    RUN_TEST(test_by_tone_seal_matches_kin);

    /* Color */
    RUN_TEST(test_seal0_is_red);
    RUN_TEST(test_seal1_is_white);
    RUN_TEST(test_seal2_is_blue);
    RUN_TEST(test_seal3_is_yellow);
    RUN_TEST(test_seal4_cycles_back_to_red);
    RUN_TEST(test_kin1_color_red_dragon);
    RUN_TEST(test_kin2_color_white_wind);

    /* Wavespell */
    RUN_TEST(test_kin1_wavespell1);
    RUN_TEST(test_kin13_wavespell1);
    RUN_TEST(test_kin14_wavespell2);
    RUN_TEST(test_kin260_wavespell20);
    RUN_TEST(test_wavespell_position_kin1);
    RUN_TEST(test_wavespell_position_kin13);
    RUN_TEST(test_wavespell_position_kin14);

    /* Castle */
    RUN_TEST(test_kin1_castle_red);
    RUN_TEST(test_kin52_castle_red);
    RUN_TEST(test_kin53_castle_white);
    RUN_TEST(test_kin104_castle_white);
    RUN_TEST(test_kin105_castle_blue);
    RUN_TEST(test_kin209_castle_green);
    RUN_TEST(test_kin260_castle_green);

    /* Harmonic */
    RUN_TEST(test_kin1_harmonic1);
    RUN_TEST(test_kin4_harmonic1);
    RUN_TEST(test_kin5_harmonic2);
    RUN_TEST(test_kin260_harmonic65);

    /* Portal days */
    RUN_TEST(test_kin1_is_portal);
    RUN_TEST(test_kin20_is_portal);
    RUN_TEST(test_kin260_is_portal);
    RUN_TEST(test_kin241_is_portal);
    RUN_TEST(test_kin2_is_not_portal);
    RUN_TEST(test_kin10_is_not_portal);
    RUN_TEST(test_portal_count_is_52);
    RUN_TEST(test_portal_count_func);
    RUN_TEST(test_portal_symmetry);
    RUN_TEST(test_portal_col5_band);
    RUN_TEST(test_portal_center_column_empty);
    RUN_TEST(test_portal_col2_four_portals);
    RUN_TEST(test_portal_invalid_kin);

    /* Name strings */
    RUN_TEST(test_color_name_red);
    RUN_TEST(test_color_name_yellow);
    RUN_TEST(test_castle_name_red);
    RUN_TEST(test_castle_name_green);

    /* Invalid helpers */
    RUN_TEST(test_wavespell_invalid);
    RUN_TEST(test_harmonic_invalid);

    /* Comprehensive */
    RUN_TEST(test_all_260_kins_valid);

    return UNITY_END();
}
