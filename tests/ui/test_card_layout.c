#include "../unity/unity.h"
#include "../../src/ui/card_layout.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* Layout positions within screen: 1-4 */

void test_visible_cards_in_bounds(void)
{
    int mask = card_all_mask();
    card_layout_t layout = card_layout_compute(mask, 16.0f / 9.0f);
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        card_rect_t c = layout.cards[i];
        TEST_ASSERT_TRUE(c.x >= 0.0f);
        TEST_ASSERT_TRUE(c.x + c.w <= 1.01f); /* small tolerance */
        TEST_ASSERT_TRUE(c.y >= 0.0f);
        TEST_ASSERT_TRUE(c.visible);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, c.opacity);
    }
}

void test_hidden_cards_offscreen(void)
{
    card_layout_t layout = card_layout_compute(0, 16.0f / 9.0f);
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        TEST_ASSERT_FALSE(layout.cards[i].visible);
        TEST_ASSERT_TRUE(layout.cards[i].x >= 1.0f);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, layout.cards[i].opacity);
    }
}

void test_single_card_visible(void)
{
    int mask = 1 << CARD_TZOLKIN;
    card_layout_t layout = card_layout_compute(mask, 16.0f / 9.0f);
    TEST_ASSERT_TRUE(layout.cards[CARD_TZOLKIN].visible);
    TEST_ASSERT_FALSE(layout.cards[CARD_ICHING].visible);
    TEST_ASSERT_TRUE(layout.cards[CARD_TZOLKIN].x < 1.0f);
}

void test_cards_stacked_vertically(void)
{
    int mask = card_all_mask();
    card_layout_t layout = card_layout_compute(mask, 16.0f / 9.0f);
    /* Each visible card's Y should be greater than the previous */
    for (int i = 1; i < CARD_TYPE_COUNT; i++) {
        TEST_ASSERT_TRUE(layout.cards[i].y > layout.cards[i - 1].y);
    }
}

/* Aspect ratio: 5-6 */

void test_wide_screen_narrow_cards(void)
{
    card_layout_t layout = card_layout_compute(card_all_mask(), 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.22f, layout.cards[0].w);
}

void test_narrow_screen_wider_cards(void)
{
    card_layout_t layout = card_layout_compute(card_all_mask(), 1.2f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.35f, layout.cards[0].w);
}

/* Toggle: 7-10 */

void test_toggle_on(void)
{
    int mask = card_toggle(0, CARD_TZOLKIN);
    TEST_ASSERT_TRUE(card_is_visible(mask, CARD_TZOLKIN));
    TEST_ASSERT_FALSE(card_is_visible(mask, CARD_ICHING));
}

void test_toggle_off(void)
{
    int mask = card_all_mask();
    mask = card_toggle(mask, CARD_HD);
    TEST_ASSERT_FALSE(card_is_visible(mask, CARD_HD));
    TEST_ASSERT_TRUE(card_is_visible(mask, CARD_TZOLKIN));
}

void test_toggle_roundtrip(void)
{
    int mask = 0;
    mask = card_toggle(mask, CARD_CHINESE);
    mask = card_toggle(mask, CARD_CHINESE);
    TEST_ASSERT_EQUAL_INT(0, mask);
}

void test_is_visible_invalid(void)
{
    TEST_ASSERT_FALSE(card_is_visible(card_all_mask(), -1));
    TEST_ASSERT_FALSE(card_is_visible(card_all_mask(), 99));
}

/* Type names: 11-12 */

void test_type_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Tzolkin", card_type_name(CARD_TZOLKIN));
    TEST_ASSERT_EQUAL_STRING("I Ching", card_type_name(CARD_ICHING));
    TEST_ASSERT_EQUAL_STRING("Chinese", card_type_name(CARD_CHINESE));
    TEST_ASSERT_EQUAL_STRING("Human Design", card_type_name(CARD_HD));
    TEST_ASSERT_EQUAL_STRING("Astrology", card_type_name(CARD_ASTROLOGY));
}

void test_type_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", card_type_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", card_type_name(99));
}

/* Toggle keys: 13-14 */

void test_toggle_keys(void)
{
    TEST_ASSERT_EQUAL_CHAR('K', card_toggle_key(CARD_TZOLKIN));
    TEST_ASSERT_EQUAL_CHAR('I', card_toggle_key(CARD_ICHING));
    TEST_ASSERT_EQUAL_CHAR('C', card_toggle_key(CARD_CHINESE));
    TEST_ASSERT_EQUAL_CHAR('D', card_toggle_key(CARD_HD));
    TEST_ASSERT_EQUAL_CHAR('A', card_toggle_key(CARD_ASTROLOGY));
}

void test_toggle_key_invalid(void)
{
    TEST_ASSERT_EQUAL_CHAR('?', card_toggle_key(-1));
}

/* Masks: 15-16 */

void test_default_mask(void)
{
    TEST_ASSERT_EQUAL_INT(0, card_default_mask());
}

void test_all_mask(void)
{
    TEST_ASSERT_EQUAL_INT(31, card_all_mask()); /* 2^5 - 1 */
}

/* Transitions: 17-20 */

void test_transition_opacity_endpoints(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, card_transition_opacity(0.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, card_transition_opacity(1.0f));
}

void test_transition_opacity_monotonic(void)
{
    float prev = 0.0f;
    for (float t = 0.0f; t <= 1.0f; t += 0.1f) {
        float val = card_transition_opacity(t);
        TEST_ASSERT_TRUE(val >= prev - 0.001f);
        prev = val;
    }
}

void test_transition_slide_endpoints(void)
{
    TEST_ASSERT_TRUE(card_transition_slide(0.0f) < -0.1f); /* off-screen */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, card_transition_slide(1.0f)); /* final */
}

void test_transition_slide_monotonic(void)
{
    float prev = -1.0f;
    for (float t = 0.0f; t <= 1.0f; t += 0.1f) {
        float val = card_transition_slide(t);
        TEST_ASSERT_TRUE(val >= prev - 0.001f); /* increasing toward 0 */
        prev = val;
    }
}

/* Card type field: 21 */

void test_card_type_field(void)
{
    card_layout_t layout = card_layout_compute(card_all_mask(), 1.5f);
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.cards[i].type);
    }
}

/* Purity: 22 */

void test_purity(void)
{
    card_layout_t a = card_layout_compute(card_all_mask(), 1.78f);
    card_layout_t b = card_layout_compute(card_all_mask(), 1.78f);
    for (int i = 0; i < CARD_TYPE_COUNT; i++) {
        TEST_ASSERT_EQUAL_FLOAT(a.cards[i].x, b.cards[i].x);
        TEST_ASSERT_EQUAL_FLOAT(a.cards[i].y, b.cards[i].y);
        TEST_ASSERT_EQUAL_FLOAT(a.cards[i].opacity, b.cards[i].opacity);
    }
}

/* Partial visibility: 23 */

void test_partial_mask_layout(void)
{
    /* Only Tzolkin and Astrology visible */
    int mask = (1 << CARD_TZOLKIN) | (1 << CARD_ASTROLOGY);
    card_layout_t layout = card_layout_compute(mask, 1.78f);
    TEST_ASSERT_TRUE(layout.cards[CARD_TZOLKIN].visible);
    TEST_ASSERT_FALSE(layout.cards[CARD_ICHING].visible);
    TEST_ASSERT_TRUE(layout.cards[CARD_ASTROLOGY].visible);
    /* Astrology should be stacked below Tzolkin */
    TEST_ASSERT_TRUE(layout.cards[CARD_ASTROLOGY].y > layout.cards[CARD_TZOLKIN].y);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_visible_cards_in_bounds);
    RUN_TEST(test_hidden_cards_offscreen);
    RUN_TEST(test_single_card_visible);
    RUN_TEST(test_cards_stacked_vertically);
    RUN_TEST(test_wide_screen_narrow_cards);
    RUN_TEST(test_narrow_screen_wider_cards);
    RUN_TEST(test_toggle_on);
    RUN_TEST(test_toggle_off);
    RUN_TEST(test_toggle_roundtrip);
    RUN_TEST(test_is_visible_invalid);
    RUN_TEST(test_type_names);
    RUN_TEST(test_type_name_invalid);
    RUN_TEST(test_toggle_keys);
    RUN_TEST(test_toggle_key_invalid);
    RUN_TEST(test_default_mask);
    RUN_TEST(test_all_mask);
    RUN_TEST(test_transition_opacity_endpoints);
    RUN_TEST(test_transition_opacity_monotonic);
    RUN_TEST(test_transition_slide_endpoints);
    RUN_TEST(test_transition_slide_monotonic);
    RUN_TEST(test_card_type_field);
    RUN_TEST(test_purity);
    RUN_TEST(test_partial_mask_layout);
    return UNITY_END();
}
