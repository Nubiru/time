#include "../../unity/unity.h"
#include "../../../src/systems/celtic/celtic_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * cti_tree_data — archetype data for 14 tree entries
 * ============================================================ */

void test_tree_yew_intercalary(void) {
    cti_tree_t t = cti_tree_data(0);
    TEST_ASSERT_EQUAL_INT(0, t.month);
    TEST_ASSERT_EQUAL_STRING("Yew", t.tree);
    TEST_ASSERT_EQUAL_STRING("Idho", t.ogham);
    TEST_ASSERT_EQUAL_STRING("The Gateway Between Worlds", t.archetype);
}

void test_tree_birch(void) {
    cti_tree_t t = cti_tree_data(1);
    TEST_ASSERT_EQUAL_INT(1, t.month);
    TEST_ASSERT_EQUAL_STRING("Birch", t.tree);
    TEST_ASSERT_EQUAL_STRING("Beith", t.ogham);
    TEST_ASSERT_EQUAL_STRING("The Pioneer", t.archetype);
}

void test_tree_oak(void) {
    cti_tree_t t = cti_tree_data(7);
    TEST_ASSERT_EQUAL_INT(7, t.month);
    TEST_ASSERT_EQUAL_STRING("Oak", t.tree);
    TEST_ASSERT_EQUAL_STRING("Duir", t.ogham);
    TEST_ASSERT_EQUAL_STRING("The Sovereign", t.archetype);
}

void test_tree_holly(void) {
    cti_tree_t t = cti_tree_data(8);
    TEST_ASSERT_EQUAL_INT(8, t.month);
    TEST_ASSERT_EQUAL_STRING("Holly", t.tree);
    TEST_ASSERT_EQUAL_STRING("Tinne", t.ogham);
    TEST_ASSERT_EQUAL_STRING("The Warrior", t.archetype);
}

void test_tree_elder(void) {
    cti_tree_t t = cti_tree_data(13);
    TEST_ASSERT_EQUAL_INT(13, t.month);
    TEST_ASSERT_EQUAL_STRING("Elder", t.tree);
    TEST_ASSERT_EQUAL_STRING("Ruis", t.ogham);
    TEST_ASSERT_EQUAL_STRING("The Witch Tree", t.archetype);
}

void test_tree_all_14_valid(void) {
    for (int i = 0; i <= 13; i++) {
        cti_tree_t t = cti_tree_data(i);
        TEST_ASSERT_EQUAL_INT(i, t.month);
        TEST_ASSERT_NOT_NULL(t.tree);
        TEST_ASSERT_NOT_NULL(t.ogham);
        TEST_ASSERT_NOT_NULL(t.archetype);
        TEST_ASSERT_NOT_NULL(t.personality);
        TEST_ASSERT_NOT_NULL(t.strengths);
        TEST_ASSERT_NOT_NULL(t.challenges);
        TEST_ASSERT_NOT_NULL(t.brief);
        TEST_ASSERT_TRUE(strlen(t.tree) > 0);
        TEST_ASSERT_TRUE(strlen(t.ogham) > 0);
        TEST_ASSERT_TRUE(strlen(t.archetype) > 0);
        TEST_ASSERT_TRUE(strlen(t.personality) > 0);
        TEST_ASSERT_TRUE(strlen(t.strengths) > 0);
        TEST_ASSERT_TRUE(strlen(t.challenges) > 0);
        TEST_ASSERT_TRUE(strlen(t.brief) > 0);
    }
}

void test_tree_invalid_neg1(void) {
    cti_tree_t t = cti_tree_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, t.month);
    TEST_ASSERT_EQUAL_STRING("?", t.tree);
    TEST_ASSERT_EQUAL_STRING("?", t.ogham);
    TEST_ASSERT_EQUAL_STRING("?", t.archetype);
    TEST_ASSERT_EQUAL_STRING("?", t.personality);
    TEST_ASSERT_EQUAL_STRING("?", t.strengths);
    TEST_ASSERT_EQUAL_STRING("?", t.challenges);
    TEST_ASSERT_EQUAL_STRING("?", t.brief);
}

void test_tree_invalid_14(void) {
    cti_tree_t t = cti_tree_data(14);
    TEST_ASSERT_EQUAL_INT(-1, t.month);
    TEST_ASSERT_EQUAL_STRING("?", t.tree);
}

/* ============================================================
 * cti_festival_data — 8 Wheel of the Year festivals
 * ============================================================ */

void test_festival_samhain(void) {
    cti_festival_t f = cti_festival_data(0);
    TEST_ASSERT_EQUAL_INT(0, f.festival);
    TEST_ASSERT_EQUAL_STRING("Samhain", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "ancestors"));
}

void test_festival_yule(void) {
    cti_festival_t f = cti_festival_data(1);
    TEST_ASSERT_EQUAL_INT(1, f.festival);
    TEST_ASSERT_EQUAL_STRING("Yule", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "light"));
}

void test_festival_beltane(void) {
    cti_festival_t f = cti_festival_data(4);
    TEST_ASSERT_EQUAL_INT(4, f.festival);
    TEST_ASSERT_EQUAL_STRING("Beltane", f.name);
    TEST_ASSERT_NOT_NULL(strstr(f.theme, "passion"));
}

void test_festival_mabon(void) {
    cti_festival_t f = cti_festival_data(7);
    TEST_ASSERT_EQUAL_INT(7, f.festival);
    TEST_ASSERT_EQUAL_STRING("Mabon", f.name);
}

void test_festival_all_8_valid(void) {
    for (int i = 0; i < 8; i++) {
        cti_festival_t f = cti_festival_data(i);
        TEST_ASSERT_EQUAL_INT(i, f.festival);
        TEST_ASSERT_NOT_NULL(f.name);
        TEST_ASSERT_NOT_NULL(f.theme);
        TEST_ASSERT_NOT_NULL(f.practice);
        TEST_ASSERT_NOT_NULL(f.reflection);
        TEST_ASSERT_NOT_NULL(f.brief);
        TEST_ASSERT_TRUE(strlen(f.name) > 0);
        TEST_ASSERT_TRUE(strlen(f.theme) > 0);
        TEST_ASSERT_TRUE(strlen(f.practice) > 0);
        TEST_ASSERT_TRUE(strlen(f.reflection) > 0);
        TEST_ASSERT_TRUE(strlen(f.brief) > 0);
    }
}

void test_festival_invalid_neg1(void) {
    cti_festival_t f = cti_festival_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
    TEST_ASSERT_EQUAL_STRING("?", f.theme);
    TEST_ASSERT_EQUAL_STRING("?", f.practice);
    TEST_ASSERT_EQUAL_STRING("?", f.reflection);
    TEST_ASSERT_EQUAL_STRING("?", f.brief);
}

void test_festival_invalid_8(void) {
    cti_festival_t f = cti_festival_data(8);
    TEST_ASSERT_EQUAL_INT(-1, f.festival);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

/* ============================================================
 * cti_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_birch(void) {
    celtic_interp_t r = cti_interpret(1, 5, -1);
    TEST_ASSERT_EQUAL_STRING("Beith", r.glyph);
}

void test_interpret_glyph_oak(void) {
    celtic_interp_t r = cti_interpret(7, 14, -1);
    TEST_ASSERT_EQUAL_STRING("Duir", r.glyph);
}

void test_interpret_glyph_yew(void) {
    celtic_interp_t r = cti_interpret(0, 1, -1);
    TEST_ASSERT_EQUAL_STRING("Idho", r.glyph);
}

void test_interpret_glance_no_festival(void) {
    celtic_interp_t r = cti_interpret(7, 14, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Oak"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Duir"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "The Sovereign"));
}

void test_interpret_glance_with_festival(void) {
    celtic_interp_t r = cti_interpret(7, 1, 5);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Oak"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Litha"));
}

void test_interpret_glance_contains_day(void) {
    celtic_interp_t r = cti_interpret(1, 12, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "day 12"));
}

void test_interpret_detail_contains_tree(void) {
    celtic_interp_t r = cti_interpret(1, 5, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Birch"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Beith"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "The Pioneer"));
}

void test_interpret_detail_contains_personality(void) {
    celtic_interp_t r = cti_interpret(1, 5, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "new beginnings"));
}

void test_interpret_detail_contains_strengths(void) {
    celtic_interp_t r = cti_interpret(1, 5, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Ambition"));
}

void test_interpret_detail_contains_challenges(void) {
    celtic_interp_t r = cti_interpret(1, 5, -1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Impatience"));
}

void test_interpret_detail_with_festival(void) {
    celtic_interp_t r = cti_interpret(7, 1, 5);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Litha"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Abundance"));
}

void test_interpret_detail_festival_reflection(void) {
    celtic_interp_t r = cti_interpret(7, 1, 5);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "celebrating"));
}

void test_interpret_invalid_tree(void) {
    celtic_interp_t r = cti_interpret(99, 1, -1);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "?"));
}

void test_interpret_no_festival(void) {
    celtic_interp_t r = cti_interpret(9, 15, -1);
    /* No festival info in detail */
    TEST_ASSERT_NULL(strstr(r.detail, "Festival:"));
}

void test_interpret_with_festival_has_festival(void) {
    celtic_interp_t r = cti_interpret(9, 15, 6);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Festival:"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Lughnasadh"));
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_tree_count(void) {
    TEST_ASSERT_EQUAL_INT(14, cti_tree_count());
}

void test_festival_count(void) {
    TEST_ASSERT_EQUAL_INT(8, cti_festival_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* cti_tree_data */
    RUN_TEST(test_tree_yew_intercalary);
    RUN_TEST(test_tree_birch);
    RUN_TEST(test_tree_oak);
    RUN_TEST(test_tree_holly);
    RUN_TEST(test_tree_elder);
    RUN_TEST(test_tree_all_14_valid);
    RUN_TEST(test_tree_invalid_neg1);
    RUN_TEST(test_tree_invalid_14);

    /* cti_festival_data */
    RUN_TEST(test_festival_samhain);
    RUN_TEST(test_festival_yule);
    RUN_TEST(test_festival_beltane);
    RUN_TEST(test_festival_mabon);
    RUN_TEST(test_festival_all_8_valid);
    RUN_TEST(test_festival_invalid_neg1);
    RUN_TEST(test_festival_invalid_8);

    /* cti_interpret */
    RUN_TEST(test_interpret_glyph_birch);
    RUN_TEST(test_interpret_glyph_oak);
    RUN_TEST(test_interpret_glyph_yew);
    RUN_TEST(test_interpret_glance_no_festival);
    RUN_TEST(test_interpret_glance_with_festival);
    RUN_TEST(test_interpret_glance_contains_day);
    RUN_TEST(test_interpret_detail_contains_tree);
    RUN_TEST(test_interpret_detail_contains_personality);
    RUN_TEST(test_interpret_detail_contains_strengths);
    RUN_TEST(test_interpret_detail_contains_challenges);
    RUN_TEST(test_interpret_detail_with_festival);
    RUN_TEST(test_interpret_detail_festival_reflection);
    RUN_TEST(test_interpret_invalid_tree);
    RUN_TEST(test_interpret_no_festival);
    RUN_TEST(test_interpret_with_festival_has_festival);

    /* counts */
    RUN_TEST(test_tree_count);
    RUN_TEST(test_festival_count);

    return UNITY_END();
}
