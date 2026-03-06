#include "../unity/unity.h"
#include "../../src/systems/human_design/bodygraph.h"

void setUp(void) {}
void tearDown(void) {}

/* ===== CENTER DATA TESTS ===== */

/* 1. Head center has correct name */
void test_center_name_head(void)
{
    const char *name = bodygraph_center_name(BG_HEAD);
    TEST_ASSERT_EQUAL_STRING("Head", name);
}

/* 2. Root center has correct name */
void test_center_name_root(void)
{
    const char *name = bodygraph_center_name(BG_ROOT);
    TEST_ASSERT_EQUAL_STRING("Root", name);
}

/* 3. Invalid center returns "Unknown" */
void test_center_name_invalid(void)
{
    const char *name = bodygraph_center_name(BG_CENTER_COUNT);
    TEST_ASSERT_EQUAL_STRING("Unknown", name);
}

/* 4. Head center position */
void test_center_position_head(void)
{
    bg_point_t pos = bodygraph_center_position(BG_HEAD);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.05f, pos.y);
}

/* 5. Root center position */
void test_center_position_root(void)
{
    bg_point_t pos = bodygraph_center_position(BG_ROOT);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, pos.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.92f, pos.y);
}

/* 6. Heart center is a motor */
void test_center_heart_is_motor(void)
{
    bg_center_t c = bodygraph_center(BG_HEART);
    TEST_ASSERT_EQUAL_INT(1, c.is_motor);
}

/* 7. Ajna center is awareness */
void test_center_ajna_is_awareness(void)
{
    bg_center_t c = bodygraph_center(BG_AJNA);
    TEST_ASSERT_EQUAL_INT(1, c.is_awareness);
}

/* 8. Solar Plexus is both motor and awareness */
void test_center_solar_plexus_motor_and_awareness(void)
{
    bg_center_t c = bodygraph_center(BG_SOLAR_PLEXUS);
    TEST_ASSERT_EQUAL_INT(1, c.is_motor);
    TEST_ASSERT_EQUAL_INT(1, c.is_awareness);
}

/* 9. Head and Root are pressure centers */
void test_center_pressure(void)
{
    bg_center_t head = bodygraph_center(BG_HEAD);
    bg_center_t root = bodygraph_center(BG_ROOT);
    bg_center_t throat = bodygraph_center(BG_THROAT);
    TEST_ASSERT_EQUAL_INT(1, head.is_pressure);
    TEST_ASSERT_EQUAL_INT(1, root.is_pressure);
    TEST_ASSERT_EQUAL_INT(0, throat.is_pressure);
}

/* 10. Throat is neither motor, awareness, nor pressure */
void test_center_throat_no_flags(void)
{
    bg_center_t c = bodygraph_center(BG_THROAT);
    TEST_ASSERT_EQUAL_INT(0, c.is_motor);
    TEST_ASSERT_EQUAL_INT(0, c.is_awareness);
    TEST_ASSERT_EQUAL_INT(0, c.is_pressure);
}

/* ===== CHANNEL DATA TESTS ===== */

/* 11. Channel count is 36 */
void test_channel_count(void)
{
    TEST_ASSERT_EQUAL_INT(36, bodygraph_channel_count());
}

/* 12. First channel: 64-47 Head-Ajna */
void test_channel_0_is_64_47(void)
{
    bg_channel_t ch = bodygraph_channel(0);
    TEST_ASSERT_EQUAL_INT(64, ch.gate_a);
    TEST_ASSERT_EQUAL_INT(47, ch.gate_b);
    TEST_ASSERT_EQUAL_INT(BG_HEAD, ch.center_a);
    TEST_ASSERT_EQUAL_INT(BG_AJNA, ch.center_b);
}

/* 13. Channel 61-24: Head-Ajna */
void test_channel_61_24(void)
{
    bg_channel_t ch = bodygraph_channel(1);
    TEST_ASSERT_EQUAL_INT(61, ch.gate_a);
    TEST_ASSERT_EQUAL_INT(24, ch.gate_b);
    TEST_ASSERT_EQUAL_INT(BG_HEAD, ch.center_a);
    TEST_ASSERT_EQUAL_INT(BG_AJNA, ch.center_b);
}

/* 14. Last channel: 41-30 Root-Solar Plexus */
void test_channel_last(void)
{
    bg_channel_t ch = bodygraph_channel(35);
    TEST_ASSERT_EQUAL_INT(41, ch.gate_a);
    TEST_ASSERT_EQUAL_INT(30, ch.gate_b);
    TEST_ASSERT_EQUAL_INT(BG_ROOT, ch.center_a);
    TEST_ASSERT_EQUAL_INT(BG_SOLAR_PLEXUS, ch.center_b);
}

/* 15. Invalid channel index returns gate_a = 0 */
void test_channel_invalid_index(void)
{
    bg_channel_t ch = bodygraph_channel(36);
    TEST_ASSERT_EQUAL_INT(0, ch.gate_a);
    TEST_ASSERT_EQUAL_INT(0, ch.gate_b);
}

/* 16. Negative channel index */
void test_channel_negative_index(void)
{
    bg_channel_t ch = bodygraph_channel(-1);
    TEST_ASSERT_EQUAL_INT(0, ch.gate_a);
}

/* ===== GATE CENTER ASSIGNMENT TESTS ===== */

/* 17. Gate 64 belongs to Head */
void test_gate_64_is_head(void)
{
    TEST_ASSERT_EQUAL_INT(BG_HEAD, bodygraph_gate_center(64));
}

/* 18. Gate 20 belongs to Throat */
void test_gate_20_is_throat(void)
{
    TEST_ASSERT_EQUAL_INT(BG_THROAT, bodygraph_gate_center(20));
}

/* 19. Gate 34 belongs to Sacral */
void test_gate_34_is_sacral(void)
{
    TEST_ASSERT_EQUAL_INT(BG_SACRAL, bodygraph_gate_center(34));
}

/* 20. Invalid gate returns BG_CENTER_COUNT */
void test_gate_center_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(BG_CENTER_COUNT, bodygraph_gate_center(0));
    TEST_ASSERT_EQUAL_INT(BG_CENTER_COUNT, bodygraph_gate_center(65));
}

/* 21. Gate 57 belongs to Spleen */
void test_gate_57_is_spleen(void)
{
    TEST_ASSERT_EQUAL_INT(BG_SPLEEN, bodygraph_gate_center(57));
}

/* 22. Gate 10 belongs to G Center */
void test_gate_10_is_g_center(void)
{
    TEST_ASSERT_EQUAL_INT(BG_G_CENTER, bodygraph_gate_center(10));
}

/* ===== CHANNEL LOOKUP TESTS ===== */

/* 23. Find channel 64-47 */
void test_channel_for_gates_64_47(void)
{
    int idx = bodygraph_channel_for_gates(64, 47);
    TEST_ASSERT_EQUAL_INT(0, idx);
}

/* 24. Find channel in reverse order (47-64) */
void test_channel_for_gates_reversed(void)
{
    int idx = bodygraph_channel_for_gates(47, 64);
    TEST_ASSERT_EQUAL_INT(0, idx);
}

/* 25. Invalid gate pair returns -1 */
void test_channel_for_gates_invalid(void)
{
    int idx = bodygraph_channel_for_gates(1, 2);
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

/* 26. Channel 20-34 (Throat-Sacral) exists */
void test_channel_for_gates_20_34(void)
{
    int idx = bodygraph_channel_for_gates(20, 34);
    TEST_ASSERT_TRUE(idx >= 0);
    TEST_ASSERT_TRUE(idx < 36);
}

/* 27. Channel 10-57 (G-Spleen) exists */
void test_channel_for_gates_10_57(void)
{
    int idx = bodygraph_channel_for_gates(10, 57);
    TEST_ASSERT_TRUE(idx >= 0);
}

/* 28. Channel 34-57 (Sacral-Spleen) exists */
void test_channel_for_gates_34_57(void)
{
    int idx = bodygraph_channel_for_gates(34, 57);
    TEST_ASSERT_TRUE(idx >= 0);
}

/* ===== DEFINED CENTERS TESTS ===== */

/* 29. No active gates = no defined centers */
void test_defined_centers_none(void)
{
    int active[65] = {0};
    int defined[BODYGRAPH_CENTER_COUNT] = {0};
    bodygraph_defined_centers(active, defined);
    for (int i = 0; i < BODYGRAPH_CENTER_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, defined[i]);
    }
}

/* 30. Gates 64+47 active = Head and Ajna defined */
void test_defined_centers_head_ajna(void)
{
    int active[65] = {0};
    active[64] = 1;
    active[47] = 1;
    int defined[BODYGRAPH_CENTER_COUNT] = {0};
    bodygraph_defined_centers(active, defined);
    TEST_ASSERT_EQUAL_INT(1, defined[BG_HEAD]);
    TEST_ASSERT_EQUAL_INT(1, defined[BG_AJNA]);
    TEST_ASSERT_EQUAL_INT(0, defined[BG_THROAT]);
}

/* 31. Single gate active = no defined centers */
void test_defined_centers_single_gate(void)
{
    int active[65] = {0};
    active[64] = 1;
    int defined[BODYGRAPH_CENTER_COUNT] = {0};
    bodygraph_defined_centers(active, defined);
    for (int i = 0; i < BODYGRAPH_CENTER_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, defined[i]);
    }
}

/* ===== ACTIVE CHANNEL COUNT TESTS ===== */

/* 32. No active gates = 0 active channels */
void test_active_channel_count_none(void)
{
    int active[65] = {0};
    TEST_ASSERT_EQUAL_INT(0, bodygraph_active_channel_count(active));
}

/* 33. One complete channel = 1 */
void test_active_channel_count_one(void)
{
    int active[65] = {0};
    active[64] = 1;
    active[47] = 1;
    TEST_ASSERT_EQUAL_INT(1, bodygraph_active_channel_count(active));
}

/* 34. Two complete channels */
void test_active_channel_count_two(void)
{
    int active[65] = {0};
    active[64] = 1;
    active[47] = 1;
    active[61] = 1;
    active[24] = 1;
    TEST_ASSERT_EQUAL_INT(2, bodygraph_active_channel_count(active));
}

/* ===== GATE POSITION TESTS ===== */

/* 35. Gate position returns correct gate number */
void test_gate_position_number(void)
{
    bg_gate_pos_t gp = bodygraph_gate_position(64);
    TEST_ASSERT_EQUAL_INT(64, gp.gate_number);
    TEST_ASSERT_EQUAL_INT(BG_HEAD, gp.center);
}

/* 36. Invalid gate position returns gate_number 0 */
void test_gate_position_invalid(void)
{
    bg_gate_pos_t gp = bodygraph_gate_position(0);
    TEST_ASSERT_EQUAL_INT(0, gp.gate_number);
}

/* 37. Gate position is near its center */
void test_gate_position_near_center(void)
{
    bg_gate_pos_t gp = bodygraph_gate_position(64);
    bg_point_t center_pos = bodygraph_center_position(BG_HEAD);
    /* gate should be within 0.3 of its center */
    float dx = gp.position.x - center_pos.x;
    float dy = gp.position.y - center_pos.y;
    float dist = dx * dx + dy * dy;
    TEST_ASSERT_TRUE(dist < 0.3f * 0.3f);
}

/* ===== INTEGRATION / CROSS-CHECK TESTS ===== */

/* 38. All 64 gates have valid center assignments */
void test_all_gates_have_centers(void)
{
    for (int g = 1; g <= 64; g++) {
        bg_center_id_t c = bodygraph_gate_center(g);
        TEST_ASSERT_TRUE_MESSAGE(c < BG_CENTER_COUNT,
            "Gate should have valid center");
    }
}

/* 39. All 36 channels reference valid centers */
void test_all_channels_valid_centers(void)
{
    for (int i = 0; i < 36; i++) {
        bg_channel_t ch = bodygraph_channel(i);
        TEST_ASSERT_TRUE(ch.center_a < BG_CENTER_COUNT);
        TEST_ASSERT_TRUE(ch.center_b < BG_CENTER_COUNT);
        TEST_ASSERT_TRUE(ch.center_a != ch.center_b);
    }
}

/* 40. Channel gates match their center assignments */
void test_channel_gates_match_centers(void)
{
    for (int i = 0; i < 36; i++) {
        bg_channel_t ch = bodygraph_channel(i);
        bg_center_id_t ca = bodygraph_gate_center(ch.gate_a);
        bg_center_id_t cb = bodygraph_gate_center(ch.gate_b);
        TEST_ASSERT_EQUAL_INT(ch.center_a, ca);
        TEST_ASSERT_EQUAL_INT(ch.center_b, cb);
    }
}

/* 41. Sacral is a motor but not awareness */
void test_sacral_motor_not_awareness(void)
{
    bg_center_t c = bodygraph_center(BG_SACRAL);
    TEST_ASSERT_EQUAL_INT(1, c.is_motor);
    TEST_ASSERT_EQUAL_INT(0, c.is_awareness);
}

/* 42. Root is motor and pressure */
void test_root_motor_and_pressure(void)
{
    bg_center_t c = bodygraph_center(BG_ROOT);
    TEST_ASSERT_EQUAL_INT(1, c.is_motor);
    TEST_ASSERT_EQUAL_INT(1, c.is_pressure);
    TEST_ASSERT_EQUAL_INT(0, c.is_awareness);
}

/* 43. Spleen position is left side of bodygraph */
void test_spleen_left_side(void)
{
    bg_point_t pos = bodygraph_center_position(BG_SPLEEN);
    TEST_ASSERT_TRUE(pos.x < 0.5f);
}

/* 44. Solar Plexus is right side of bodygraph */
void test_solar_plexus_right_side(void)
{
    bg_point_t pos = bodygraph_center_position(BG_SOLAR_PLEXUS);
    TEST_ASSERT_TRUE(pos.x > 0.5f);
}

int main(void)
{
    UNITY_BEGIN();

    /* Center data */
    RUN_TEST(test_center_name_head);
    RUN_TEST(test_center_name_root);
    RUN_TEST(test_center_name_invalid);
    RUN_TEST(test_center_position_head);
    RUN_TEST(test_center_position_root);
    RUN_TEST(test_center_heart_is_motor);
    RUN_TEST(test_center_ajna_is_awareness);
    RUN_TEST(test_center_solar_plexus_motor_and_awareness);
    RUN_TEST(test_center_pressure);
    RUN_TEST(test_center_throat_no_flags);

    /* Channel data */
    RUN_TEST(test_channel_count);
    RUN_TEST(test_channel_0_is_64_47);
    RUN_TEST(test_channel_61_24);
    RUN_TEST(test_channel_last);
    RUN_TEST(test_channel_invalid_index);
    RUN_TEST(test_channel_negative_index);

    /* Gate center assignments */
    RUN_TEST(test_gate_64_is_head);
    RUN_TEST(test_gate_20_is_throat);
    RUN_TEST(test_gate_34_is_sacral);
    RUN_TEST(test_gate_center_invalid);
    RUN_TEST(test_gate_57_is_spleen);
    RUN_TEST(test_gate_10_is_g_center);

    /* Channel lookup */
    RUN_TEST(test_channel_for_gates_64_47);
    RUN_TEST(test_channel_for_gates_reversed);
    RUN_TEST(test_channel_for_gates_invalid);
    RUN_TEST(test_channel_for_gates_20_34);
    RUN_TEST(test_channel_for_gates_10_57);
    RUN_TEST(test_channel_for_gates_34_57);

    /* Defined centers */
    RUN_TEST(test_defined_centers_none);
    RUN_TEST(test_defined_centers_head_ajna);
    RUN_TEST(test_defined_centers_single_gate);

    /* Active channel count */
    RUN_TEST(test_active_channel_count_none);
    RUN_TEST(test_active_channel_count_one);
    RUN_TEST(test_active_channel_count_two);

    /* Gate positions */
    RUN_TEST(test_gate_position_number);
    RUN_TEST(test_gate_position_invalid);
    RUN_TEST(test_gate_position_near_center);

    /* Integration / cross-check */
    RUN_TEST(test_all_gates_have_centers);
    RUN_TEST(test_all_channels_valid_centers);
    RUN_TEST(test_channel_gates_match_centers);
    RUN_TEST(test_sacral_motor_not_awareness);
    RUN_TEST(test_root_motor_and_pressure);
    RUN_TEST(test_spleen_left_side);
    RUN_TEST(test_solar_plexus_right_side);

    return UNITY_END();
}
