#include "../unity/unity.h"
#include "../../src/systems/iching/iching.h"

void setUp(void) {}
void tearDown(void) {}

/* 1. Hexagram 1 = all yang, lower=Qian(0), upper=Qian(0) */
void test_hexagram_1_all_yang(void)
{
    hexagram_t h = iching_hexagram(1);
    TEST_ASSERT_EQUAL_INT(1, h.king_wen);
    TEST_ASSERT_EQUAL_INT(0, h.lower_trigram);
    TEST_ASSERT_EQUAL_INT(0, h.upper_trigram);
    for (int i = 0; i < 6; i++)
        TEST_ASSERT_EQUAL_INT(1, h.lines[i]);
}

/* 2. Hexagram 2 = all yin, lower=Kun(7), upper=Kun(7) */
void test_hexagram_2_all_yin(void)
{
    hexagram_t h = iching_hexagram(2);
    TEST_ASSERT_EQUAL_INT(2, h.king_wen);
    TEST_ASSERT_EQUAL_INT(7, h.lower_trigram);
    TEST_ASSERT_EQUAL_INT(7, h.upper_trigram);
    for (int i = 0; i < 6; i++)
        TEST_ASSERT_EQUAL_INT(0, h.lines[i]);
}

/* 3. Hexagram 29 = Kan/Kan, Hexagram 30 = Li/Li */
void test_hexagram_29_kan_kan(void)
{
    hexagram_t h = iching_hexagram(29);
    TEST_ASSERT_EQUAL_INT(5, h.lower_trigram);
    TEST_ASSERT_EQUAL_INT(5, h.upper_trigram);
}

void test_hexagram_30_li_li(void)
{
    hexagram_t h = iching_hexagram(30);
    TEST_ASSERT_EQUAL_INT(2, h.lower_trigram);
    TEST_ASSERT_EQUAL_INT(2, h.upper_trigram);
}

/* 4. Hexagram 63 (Ji Ji) = lower Li(2), upper Kan(5) — Water over Fire */
void test_hexagram_63_ji_ji(void)
{
    hexagram_t h = iching_hexagram(63);
    TEST_ASSERT_EQUAL_INT(2, h.lower_trigram);
    TEST_ASSERT_EQUAL_INT(5, h.upper_trigram);
}

/* 5. Hexagram 64 (Wei Ji) = lower Kan(5), upper Li(2) — Fire over Water */
void test_hexagram_64_wei_ji(void)
{
    hexagram_t h = iching_hexagram(64);
    TEST_ASSERT_EQUAL_INT(5, h.lower_trigram);
    TEST_ASSERT_EQUAL_INT(2, h.upper_trigram);
}

/* 6. Hexagram names */
void test_hexagram_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Qian", iching_hexagram_name(1));
    TEST_ASSERT_EQUAL_STRING("Kun", iching_hexagram_name(2));
    TEST_ASSERT_EQUAL_STRING("Wei Ji", iching_hexagram_name(64));
}

/* 7. Trigram names */
void test_trigram_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Qian", iching_trigram_name(0));
    TEST_ASSERT_EQUAL_STRING("Kun", iching_trigram_name(7));
    TEST_ASSERT_EQUAL_STRING("Li", iching_trigram_name(2));
    TEST_ASSERT_EQUAL_STRING("Kan", iching_trigram_name(5));
}

/* 8. Invalid inputs return "?" */
void test_invalid_inputs(void)
{
    TEST_ASSERT_EQUAL_STRING("?", iching_hexagram_name(0));
    TEST_ASSERT_EQUAL_STRING("?", iching_hexagram_name(65));
    TEST_ASSERT_EQUAL_STRING("?", iching_trigram_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", iching_trigram_name(8));
    TEST_ASSERT_EQUAL_STRING("?", iching_trigram_symbol(-1));

    hexagram_t h = iching_hexagram(0);
    TEST_ASSERT_EQUAL_INT(0, h.king_wen);
}

/* 9. Daily hexagram repeats every 64 days */
void test_64_day_cycle(void)
{
    double jd = 2460000.5;
    hexagram_t h1 = iching_from_jd(jd);
    hexagram_t h2 = iching_from_jd(jd + 64.0);
    TEST_ASSERT_EQUAL_INT(h1.king_wen, h2.king_wen);
}

/* 10. Daily hexagram changes day-to-day */
void test_daily_change(void)
{
    double jd = 2460000.5;
    hexagram_t h1 = iching_from_jd(jd);
    hexagram_t h2 = iching_from_jd(jd + 1.0);
    TEST_ASSERT_TRUE(h1.king_wen != h2.king_wen);
}

/* 11. lines_to_king_wen roundtrip for all 64 */
void test_lines_roundtrip(void)
{
    for (int kw = 1; kw <= 64; kw++) {
        hexagram_t h = iching_hexagram(kw);
        int result = iching_lines_to_king_wen(h.lines);
        TEST_ASSERT_EQUAL_INT(kw, result);
    }
}

/* 12. All trigram indices 0-7 produce valid names */
void test_all_trigram_indices(void)
{
    const char *expected[] = {
        "Qian", "Dui", "Li", "Zhen", "Xun", "Kan", "Gen", "Kun"
    };
    for (int i = 0; i < 8; i++)
        TEST_ASSERT_EQUAL_STRING(expected[i], iching_trigram_name(i));
}

/* 13. from_jd returns valid king_wen (1-64) */
void test_from_jd_valid_range(void)
{
    double jds[] = {2451545.0, 2460000.0, 2440000.0, 0.5, 100.5};
    for (int i = 0; i < 5; i++) {
        hexagram_t h = iching_from_jd(jds[i]);
        TEST_ASSERT_TRUE(h.king_wen >= 1);
        TEST_ASSERT_TRUE(h.king_wen <= 64);
    }
}

/* 14. All 64 hexagrams have valid structure */
void test_all_64_valid(void)
{
    for (int kw = 1; kw <= 64; kw++) {
        hexagram_t h = iching_hexagram(kw);
        TEST_ASSERT_EQUAL_INT(kw, h.king_wen);
        TEST_ASSERT_TRUE(h.lower_trigram >= 0);
        TEST_ASSERT_TRUE(h.lower_trigram <= 7);
        TEST_ASSERT_TRUE(h.upper_trigram >= 0);
        TEST_ASSERT_TRUE(h.upper_trigram <= 7);
        for (int i = 0; i < 6; i++)
            TEST_ASSERT_TRUE(h.lines[i] == 0 || h.lines[i] == 1);
    }
}

/* 15. Purity: same input = same output */
void test_purity(void)
{
    hexagram_t a = iching_hexagram(42);
    hexagram_t b = iching_hexagram(42);
    TEST_ASSERT_EQUAL_INT(a.king_wen, b.king_wen);
    TEST_ASSERT_EQUAL_INT(a.lower_trigram, b.lower_trigram);
    TEST_ASSERT_EQUAL_INT(a.upper_trigram, b.upper_trigram);
    for (int i = 0; i < 6; i++)
        TEST_ASSERT_EQUAL_INT(a.lines[i], b.lines[i]);
}

/* 16. Trigram Unicode symbols */
void test_trigram_symbols(void)
{
    TEST_ASSERT_EQUAL_STRING("\u2630", iching_trigram_symbol(0));
    TEST_ASSERT_EQUAL_STRING("\u2637", iching_trigram_symbol(7));
}

/* 17. Hexagram 63 line pattern: 1,0,1,0,1,0 (Li below, Kan above) */
void test_hexagram_63_lines(void)
{
    hexagram_t h = iching_hexagram(63);
    int expected[] = {1, 0, 1, 0, 1, 0};
    for (int i = 0; i < 6; i++)
        TEST_ASSERT_EQUAL_INT(expected[i], h.lines[i]);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_hexagram_1_all_yang);
    RUN_TEST(test_hexagram_2_all_yin);
    RUN_TEST(test_hexagram_29_kan_kan);
    RUN_TEST(test_hexagram_30_li_li);
    RUN_TEST(test_hexagram_63_ji_ji);
    RUN_TEST(test_hexagram_64_wei_ji);
    RUN_TEST(test_hexagram_names);
    RUN_TEST(test_trigram_names);
    RUN_TEST(test_invalid_inputs);
    RUN_TEST(test_64_day_cycle);
    RUN_TEST(test_daily_change);
    RUN_TEST(test_lines_roundtrip);
    RUN_TEST(test_all_trigram_indices);
    RUN_TEST(test_from_jd_valid_range);
    RUN_TEST(test_all_64_valid);
    RUN_TEST(test_purity);
    RUN_TEST(test_trigram_symbols);
    RUN_TEST(test_hexagram_63_lines);
    return UNITY_END();
}
