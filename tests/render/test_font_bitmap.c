#include "../unity/unity.h"
#include "../../src/render/font_bitmap.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Atlas dimension tests --- */

void test_atlas_width(void) {
    TEST_ASSERT_EQUAL_INT(128, font_bitmap_atlas_width());
}

void test_atlas_height(void) {
    TEST_ASSERT_EQUAL_INT(96, font_bitmap_atlas_height());
}

void test_atlas_size_bytes(void) {
    TEST_ASSERT_EQUAL_INT(128 * 96, font_bitmap_atlas_size());
}

/* --- Glyph data tests --- */

void test_glyph_space_is_blank(void) {
    const uint8_t *g = font_bitmap_glyph_data(' ');
    TEST_ASSERT_NOT_NULL(g);
    /* Space should have all-zero rows */
    for (int i = 0; i < 7; i++) {
        TEST_ASSERT_EQUAL_UINT8(0, g[i]);
    }
}

void test_glyph_A_not_blank(void) {
    const uint8_t *g = font_bitmap_glyph_data('A');
    TEST_ASSERT_NOT_NULL(g);
    /* 'A' should have non-zero pixels */
    int total = 0;
    for (int i = 0; i < 7; i++) total += g[i];
    TEST_ASSERT_TRUE(total > 0);
}

void test_glyph_out_of_range_is_null(void) {
    TEST_ASSERT_NULL(font_bitmap_glyph_data(0));
    TEST_ASSERT_NULL(font_bitmap_glyph_data(127));
}

void test_glyph_count(void) {
    TEST_ASSERT_EQUAL_INT(95, font_bitmap_glyph_count());
}

/* --- Atlas generation tests --- */

void test_generate_fills_buffer(void) {
    static uint8_t buf[128 * 96];
    int result = font_bitmap_generate(buf);
    TEST_ASSERT_EQUAL_INT(0, result);
    /* Buffer should have some non-zero pixels (at least 'A' is not blank) */
    int total = 0;
    for (int i = 0; i < 128 * 96; i++) total += buf[i];
    TEST_ASSERT_TRUE(total > 0);
}

void test_generate_null_fails(void) {
    TEST_ASSERT_NOT_EQUAL(0, font_bitmap_generate(NULL));
}

void test_space_area_is_blank(void) {
    static uint8_t buf[128 * 96];
    font_bitmap_generate(buf);
    /* Space is char 32, first glyph. At grid position (0,0), cell 8x16 px.
     * The cell should be all zeros. */
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 8; col++) {
            TEST_ASSERT_EQUAL_UINT8(0, buf[row * 128 + col]);
        }
    }
}

/* --- Runner --- */

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_atlas_width);
    RUN_TEST(test_atlas_height);
    RUN_TEST(test_atlas_size_bytes);
    RUN_TEST(test_glyph_space_is_blank);
    RUN_TEST(test_glyph_A_not_blank);
    RUN_TEST(test_glyph_out_of_range_is_null);
    RUN_TEST(test_glyph_count);
    RUN_TEST(test_generate_fills_buffer);
    RUN_TEST(test_generate_null_fails);
    RUN_TEST(test_space_area_is_blank);
    return UNITY_END();
}
