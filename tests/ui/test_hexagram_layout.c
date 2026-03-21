#include "../unity/unity.h"
#include "../../src/ui/hexagram_layout.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Hexagram 1: The Creative (Heaven over Heaven) --- */

void test_creative_king_wen(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_EQUAL_INT(1, layout.king_wen);
}

void test_creative_name(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_EQUAL_STRING("The Creative", layout.name);
}

void test_creative_all_yang(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(1, layout.lines[i].is_yang);
    }
}

void test_creative_upper_trigram(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_EQUAL_INT(0, layout.upper_trigram);  /* Qian = 0 */
    TEST_ASSERT_EQUAL_STRING("Qian", layout.upper_name);
}

void test_creative_lower_trigram(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_EQUAL_INT(0, layout.lower_trigram);  /* Qian = 0 */
    TEST_ASSERT_EQUAL_STRING("Qian", layout.lower_name);
}

void test_creative_judgment_not_null(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_NOT_NULL(layout.judgment);
    TEST_ASSERT_TRUE(strlen(layout.judgment) > 0);
}

void test_creative_image_not_null(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_NOT_NULL(layout.image);
    TEST_ASSERT_TRUE(strlen(layout.image) > 0);
}

void test_creative_keywords_not_null(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_NOT_NULL(layout.keywords);
    TEST_ASSERT_TRUE(strlen(layout.keywords) > 0);
}

/* --- Hexagram 2: The Receptive (Earth over Earth) --- */

void test_receptive_king_wen(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(2);
    TEST_ASSERT_EQUAL_INT(2, layout.king_wen);
}

void test_receptive_name(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(2);
    TEST_ASSERT_EQUAL_STRING("The Receptive", layout.name);
}

void test_receptive_all_yin(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(2);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(0, layout.lines[i].is_yang);
    }
}

void test_receptive_trigrams(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(2);
    TEST_ASSERT_EQUAL_INT(7, layout.upper_trigram);  /* Kun = 7 */
    TEST_ASSERT_EQUAL_INT(7, layout.lower_trigram);  /* Kun = 7 */
    TEST_ASSERT_EQUAL_STRING("Kun", layout.upper_name);
    TEST_ASSERT_EQUAL_STRING("Kun", layout.lower_name);
}

/* --- Line positions --- */

void test_line_index_order(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.lines[i].line_index);
    }
}

void test_line_bottom_y_highest(void)
{
    /* Bottom line (index 0) should have the largest y value (lowest on screen) */
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.60f, layout.lines[0].y);
}

void test_line_top_y_lowest(void)
{
    /* Top line (index 5) should have the smallest y value (highest on screen) */
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.20f, layout.lines[5].y);
}

void test_lines_spaced_evenly(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    float expected_step = 0.05f + 0.03f;  /* LINE_H + LINE_GAP */
    for (int i = 1; i < 6; i++) {
        float gap = layout.lines[i - 1].y - layout.lines[i].y;
        TEST_ASSERT_FLOAT_WITHIN(0.001f, expected_step, gap);
    }
}

void test_line_width(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.lines[i].w);
    }
}

void test_line_height(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.05f, layout.lines[i].h);
    }
}

void test_line_x_position(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, layout.lines[i].x);
    }
}

/* --- Text label positions --- */

void test_name_position(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.name_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, layout.name_y);
}

void test_number_position(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.number_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.18f, layout.number_y);
}

void test_upper_label_position(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.upper_label_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.35f, layout.upper_label_y);
}

void test_lower_label_position(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.80f, layout.lower_label_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.52f, layout.lower_label_y);
}

void test_judgment_position(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.10f, layout.judgment_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.72f, layout.judgment_y);
}

/* --- Lines block bounds --- */

void test_lines_block_position(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, layout.lines_x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.20f, layout.lines_y);
}

void test_lines_block_size(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.lines_w);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.45f, layout.lines_h);
}

/* --- Accessor: hexagram_layout_line --- */

void test_accessor_line_valid(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    hex_line_t line = hexagram_layout_line(&layout, 0);
    TEST_ASSERT_EQUAL_INT(0, line.line_index);
    TEST_ASSERT_EQUAL_INT(1, line.is_yang);
}

void test_accessor_line_top(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    hex_line_t line = hexagram_layout_line(&layout, 5);
    TEST_ASSERT_EQUAL_INT(5, line.line_index);
    TEST_ASSERT_EQUAL_INT(1, line.is_yang);
}

void test_accessor_line_invalid_negative(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    hex_line_t line = hexagram_layout_line(&layout, -1);
    TEST_ASSERT_EQUAL_INT(-1, line.line_index);
}

void test_accessor_line_invalid_high(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    hex_line_t line = hexagram_layout_line(&layout, 6);
    TEST_ASSERT_EQUAL_INT(-1, line.line_index);
}

void test_accessor_line_null_layout(void)
{
    hex_line_t line = hexagram_layout_line(NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, line.line_index);
}

/* --- Accessor: hexagram_layout_trigram_name --- */

void test_trigram_name_upper(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_EQUAL_STRING("Qian",
        hexagram_layout_trigram_name(&layout, 1));
}

void test_trigram_name_lower(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_EQUAL_STRING("Qian",
        hexagram_layout_trigram_name(&layout, 0));
}

void test_trigram_name_null_layout(void)
{
    TEST_ASSERT_EQUAL_STRING("?",
        hexagram_layout_trigram_name(NULL, 1));
}

/* --- Trigram symbols --- */

void test_upper_symbol_not_null(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_NOT_NULL(layout.upper_symbol);
}

void test_lower_symbol_not_null(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(1);
    TEST_ASSERT_NOT_NULL(layout.lower_symbol);
}

/* --- Mixed hexagram (hex 3: Thunder over Water) --- */

void test_hex3_mixed_lines(void)
{
    /* Hexagram 3 = Zhun. Lower=Zhen(Thunder,idx3), Upper=Kan(Water,idx5) */
    hexagram_layout_t layout = hexagram_layout_compute(3);
    /* Zhen (Thunder) lines bottom-to-top: 1,0,0 */
    TEST_ASSERT_EQUAL_INT(1, layout.lines[0].is_yang);
    TEST_ASSERT_EQUAL_INT(0, layout.lines[1].is_yang);
    TEST_ASSERT_EQUAL_INT(0, layout.lines[2].is_yang);
    /* Kan (Water) lines bottom-to-top: 0,1,0 */
    TEST_ASSERT_EQUAL_INT(0, layout.lines[3].is_yang);
    TEST_ASSERT_EQUAL_INT(1, layout.lines[4].is_yang);
    TEST_ASSERT_EQUAL_INT(0, layout.lines[5].is_yang);
}

void test_hex3_trigram_names(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(3);
    TEST_ASSERT_EQUAL_STRING("Zhen", layout.lower_name);
    TEST_ASSERT_EQUAL_STRING("Kan", layout.upper_name);
}

/* --- Invalid input --- */

void test_invalid_zero(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(0);
    TEST_ASSERT_EQUAL_INT(0, layout.king_wen);
}

void test_invalid_negative(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(-5);
    TEST_ASSERT_EQUAL_INT(0, layout.king_wen);
}

void test_invalid_too_high(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(65);
    TEST_ASSERT_EQUAL_INT(0, layout.king_wen);
}

void test_invalid_zeroed_struct(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(0);
    TEST_ASSERT_EQUAL_INT(0, layout.king_wen);
    TEST_ASSERT_EQUAL_INT(0, layout.upper_trigram);
    TEST_ASSERT_EQUAL_INT(0, layout.lower_trigram);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, layout.lines[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, layout.lines[i].y);
    }
}

/* --- Boundary: hex 64 (last hexagram) --- */

void test_hex64_valid(void)
{
    hexagram_layout_t layout = hexagram_layout_compute(64);
    TEST_ASSERT_EQUAL_INT(64, layout.king_wen);
    TEST_ASSERT_NOT_NULL(layout.name);
    TEST_ASSERT_TRUE(strlen(layout.name) > 0);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    hexagram_layout_t a = hexagram_layout_compute(29);
    hexagram_layout_t b = hexagram_layout_compute(29);
    TEST_ASSERT_EQUAL_INT(a.king_wen, b.king_wen);
    TEST_ASSERT_EQUAL_INT(a.upper_trigram, b.upper_trigram);
    TEST_ASSERT_EQUAL_INT(a.lower_trigram, b.lower_trigram);
    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL_INT(a.lines[i].is_yang, b.lines[i].is_yang);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.lines[i].x, b.lines[i].x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, a.lines[i].y, b.lines[i].y);
    }
}

void test_purity_different_input_different_output(void)
{
    hexagram_layout_t a = hexagram_layout_compute(1);
    hexagram_layout_t b = hexagram_layout_compute(2);
    /* Hex 1 is all yang, hex 2 is all yin — lines must differ */
    int any_diff = 0;
    for (int i = 0; i < 6; i++) {
        if (a.lines[i].is_yang != b.lines[i].is_yang)
            any_diff = 1;
    }
    TEST_ASSERT_EQUAL_INT(1, any_diff);
}

int main(void)
{
    UNITY_BEGIN();
    /* Hexagram 1: The Creative */
    RUN_TEST(test_creative_king_wen);
    RUN_TEST(test_creative_name);
    RUN_TEST(test_creative_all_yang);
    RUN_TEST(test_creative_upper_trigram);
    RUN_TEST(test_creative_lower_trigram);
    RUN_TEST(test_creative_judgment_not_null);
    RUN_TEST(test_creative_image_not_null);
    RUN_TEST(test_creative_keywords_not_null);
    /* Hexagram 2: The Receptive */
    RUN_TEST(test_receptive_king_wen);
    RUN_TEST(test_receptive_name);
    RUN_TEST(test_receptive_all_yin);
    RUN_TEST(test_receptive_trigrams);
    /* Line positions */
    RUN_TEST(test_line_index_order);
    RUN_TEST(test_line_bottom_y_highest);
    RUN_TEST(test_line_top_y_lowest);
    RUN_TEST(test_lines_spaced_evenly);
    RUN_TEST(test_line_width);
    RUN_TEST(test_line_height);
    RUN_TEST(test_line_x_position);
    /* Text label positions */
    RUN_TEST(test_name_position);
    RUN_TEST(test_number_position);
    RUN_TEST(test_upper_label_position);
    RUN_TEST(test_lower_label_position);
    RUN_TEST(test_judgment_position);
    /* Lines block bounds */
    RUN_TEST(test_lines_block_position);
    RUN_TEST(test_lines_block_size);
    /* Accessor: hexagram_layout_line */
    RUN_TEST(test_accessor_line_valid);
    RUN_TEST(test_accessor_line_top);
    RUN_TEST(test_accessor_line_invalid_negative);
    RUN_TEST(test_accessor_line_invalid_high);
    RUN_TEST(test_accessor_line_null_layout);
    /* Accessor: trigram name */
    RUN_TEST(test_trigram_name_upper);
    RUN_TEST(test_trigram_name_lower);
    RUN_TEST(test_trigram_name_null_layout);
    /* Trigram symbols */
    RUN_TEST(test_upper_symbol_not_null);
    RUN_TEST(test_lower_symbol_not_null);
    /* Mixed hexagram */
    RUN_TEST(test_hex3_mixed_lines);
    RUN_TEST(test_hex3_trigram_names);
    /* Invalid input */
    RUN_TEST(test_invalid_zero);
    RUN_TEST(test_invalid_negative);
    RUN_TEST(test_invalid_too_high);
    RUN_TEST(test_invalid_zeroed_struct);
    /* Boundary */
    RUN_TEST(test_hex64_valid);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_input_different_output);
    return UNITY_END();
}
