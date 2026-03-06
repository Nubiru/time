#include "../unity/unity.h"
#include "../../src/systems/human_design/human_design.h"

void setUp(void) {}
void tearDown(void) {}

/* 1. 0 ecliptic = Gate 17 (first gate in sequence) */
void test_gate_at_zero_degrees(void)
{
    TEST_ASSERT_EQUAL_INT(17, hd_gate_at_degree(0.0));
}

/* 2. 5.625 = Gate 21 (second gate) */
void test_gate_at_5_625_degrees(void)
{
    TEST_ASSERT_EQUAL_INT(21, hd_gate_at_degree(5.625));
}

/* 3. 354.375 = Gate 25 (last gate before wrap) */
void test_gate_at_354_375_degrees(void)
{
    TEST_ASSERT_EQUAL_INT(25, hd_gate_at_degree(354.375));
}

/* 4. 359.99 = Gate 25 (still in last gate) */
void test_gate_at_359_99_degrees(void)
{
    TEST_ASSERT_EQUAL_INT(25, hd_gate_at_degree(359.99));
}

/* 5. 180 = Gate 18 (position 32 in gate sequence)
 *    Note: task spec said Gate 44 here but 180/5.625=32, sequence[32]=18. */
void test_gate_at_180_degrees(void)
{
    TEST_ASSERT_EQUAL_INT(18, hd_gate_at_degree(180.0));
}

/* 6. First sixth of a gate = line 1 */
void test_line_first_sixth(void)
{
    hd_gate_t g = hd_gate_from_longitude(0.0);
    TEST_ASSERT_EQUAL_INT(1, g.line);
}

/* 7. Last sixth of a gate = line 6 */
void test_line_last_sixth(void)
{
    hd_gate_t g = hd_gate_from_longitude(5.624);
    TEST_ASSERT_EQUAL_INT(6, g.line);
}

/* 8. Exact gate boundary = line 1 of next gate */
void test_line_at_gate_boundary(void)
{
    hd_gate_t g = hd_gate_from_longitude(5.625);
    TEST_ASSERT_EQUAL_INT(21, g.gate);
    TEST_ASSERT_EQUAL_INT(1, g.line);
}

/* 9. Sun at 0 -> Earth at 180 (opposite gate) */
void test_incarnation_sun_zero(void)
{
    hd_incarnation_t ic = hd_incarnation(0.0);
    TEST_ASSERT_EQUAL_INT(17, ic.sun.gate);
    TEST_ASSERT_EQUAL_INT(18, ic.earth.gate);
}

/* 10. Sun at 100 -> Earth at 280 */
void test_incarnation_sun_100(void)
{
    hd_incarnation_t ic = hd_incarnation(100.0);
    /* 100/5.625 = 17.78 -> pos 17 = Gate 39 */
    TEST_ASSERT_EQUAL_INT(39, ic.sun.gate);
    /* 280/5.625 = 49.78 -> pos 49 = Gate 38 */
    TEST_ASSERT_EQUAL_INT(38, ic.earth.gate);
}

/* 11. hd_gate_name(1) = "The Creative" */
void test_gate_name_1(void)
{
    TEST_ASSERT_EQUAL_STRING("The Creative", hd_gate_name(1));
}

/* 12. hd_gate_name(64) = "Confusion" */
void test_gate_name_64(void)
{
    TEST_ASSERT_EQUAL_STRING("Confusion", hd_gate_name(64));
}

/* 13. hd_gate_keyword(17) = "Following" */
void test_gate_keyword_17(void)
{
    TEST_ASSERT_EQUAL_STRING("Following", hd_gate_keyword(17));
}

/* 14. Invalid gate returns "?" */
void test_invalid_gate(void)
{
    TEST_ASSERT_EQUAL_STRING("?", hd_gate_name(0));
    TEST_ASSERT_EQUAL_STRING("?", hd_gate_name(65));
    TEST_ASSERT_EQUAL_STRING("?", hd_gate_keyword(-1));
    TEST_ASSERT_EQUAL_STRING("?", hd_gate_keyword(100));
}

/* 15. All 64 gates reachable by stepping in 5.625 increments */
void test_all_64_gates_reachable(void)
{
    int seen[65] = {0};
    int unique = 0;
    for (int i = 0; i < 64; i++) {
        int gate = hd_gate_at_degree(i * 5.625);
        TEST_ASSERT_TRUE(gate >= 1);
        TEST_ASSERT_TRUE(gate <= 64);
        if (!seen[gate]) {
            seen[gate] = 1;
            unique++;
        }
    }
    TEST_ASSERT_EQUAL_INT(64, unique);
}

/* 16. Gate number always in range 1-64 for various inputs */
void test_gate_always_valid_range(void)
{
    double test_lons[] = {0.0, 45.0, 90.0, 135.0, 180.0, 225.0, 270.0, 315.0, 359.99, -30.0, 720.0};
    for (int i = 0; i < 11; i++) {
        int gate = hd_gate_at_degree(test_lons[i]);
        TEST_ASSERT_TRUE(gate >= 1);
        TEST_ASSERT_TRUE(gate <= 64);
    }
}

/* 17. Purity: same input gives same output */
void test_purity(void)
{
    hd_gate_t a = hd_gate_from_longitude(123.456);
    hd_gate_t b = hd_gate_from_longitude(123.456);
    TEST_ASSERT_EQUAL_INT(a.gate, b.gate);
    TEST_ASSERT_EQUAL_INT(a.line, b.line);
}

/* 18. Negative longitude wraps correctly */
void test_negative_longitude(void)
{
    /* -5.625 should wrap to 354.375 -> position 63 -> Gate 25 */
    TEST_ASSERT_EQUAL_INT(25, hd_gate_at_degree(-5.625));
}

/* 19. Longitude > 360 wraps correctly */
void test_overflow_longitude(void)
{
    /* 365.625 wraps to 5.625 -> position 1 -> Gate 21 */
    TEST_ASSERT_EQUAL_INT(21, hd_gate_at_degree(365.625));
}

/* 20. Original longitude preserved in result */
void test_longitude_preserved(void)
{
    hd_gate_t g = hd_gate_from_longitude(-45.0);
    TEST_ASSERT_TRUE(g.longitude == -45.0);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_gate_at_zero_degrees);
    RUN_TEST(test_gate_at_5_625_degrees);
    RUN_TEST(test_gate_at_354_375_degrees);
    RUN_TEST(test_gate_at_359_99_degrees);
    RUN_TEST(test_gate_at_180_degrees);
    RUN_TEST(test_line_first_sixth);
    RUN_TEST(test_line_last_sixth);
    RUN_TEST(test_line_at_gate_boundary);
    RUN_TEST(test_incarnation_sun_zero);
    RUN_TEST(test_incarnation_sun_100);
    RUN_TEST(test_gate_name_1);
    RUN_TEST(test_gate_name_64);
    RUN_TEST(test_gate_keyword_17);
    RUN_TEST(test_invalid_gate);
    RUN_TEST(test_all_64_gates_reachable);
    RUN_TEST(test_gate_always_valid_range);
    RUN_TEST(test_purity);
    RUN_TEST(test_negative_longitude);
    RUN_TEST(test_overflow_longitude);
    RUN_TEST(test_longitude_preserved);
    return UNITY_END();
}
