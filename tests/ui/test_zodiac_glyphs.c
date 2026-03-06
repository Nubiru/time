#include "../unity/unity.h"
#include "../../src/ui/zodiac_glyphs.h"

void setUp(void) {}
void tearDown(void) {}

/* Zodiac glyphs: 1-5 */

void test_zodiac_all_non_null(void)
{
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_NOT_NULL(zodiac_glyph(i));
    }
}

void test_zodiac_invalid_returns_null(void)
{
    TEST_ASSERT_NULL(zodiac_glyph(-1));
    TEST_ASSERT_NULL(zodiac_glyph(12));
    TEST_ASSERT_NULL(zodiac_glyph(99));
}

void test_zodiac_glyph_count(void)
{
    TEST_ASSERT_EQUAL_INT(12, zodiac_glyph_count());
}

void test_zodiac_glyphs_have_pixels(void)
{
    for (int i = 0; i < 12; i++) {
        const unsigned char *g = zodiac_glyph(i);
        int count = 0;
        for (int j = 0; j < GLYPH_BYTES; j++) {
            if (g[j]) count++;
        }
        TEST_ASSERT_TRUE(count > 0);
    }
}

void test_zodiac_glyphs_not_all_filled(void)
{
    for (int i = 0; i < 12; i++) {
        const unsigned char *g = zodiac_glyph(i);
        int count = 0;
        for (int j = 0; j < GLYPH_BYTES; j++) {
            if (g[j]) count++;
        }
        TEST_ASSERT_TRUE(count < GLYPH_BYTES);
    }
}

/* Planet glyphs: 6-9 */

void test_planet_all_non_null(void)
{
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_NOT_NULL(planet_glyph(i));
    }
}

void test_planet_invalid_returns_null(void)
{
    TEST_ASSERT_NULL(planet_glyph(-1));
    TEST_ASSERT_NULL(planet_glyph(8));
}

void test_planet_glyph_count(void)
{
    TEST_ASSERT_EQUAL_INT(8, planet_glyph_count());
}

void test_planet_glyphs_have_pixels(void)
{
    for (int i = 0; i < 8; i++) {
        const unsigned char *g = planet_glyph(i);
        int count = 0;
        for (int j = 0; j < GLYPH_BYTES; j++) {
            if (g[j]) count++;
        }
        TEST_ASSERT_TRUE(count > 0);
    }
}

/* Aspect glyphs: 10-12 */

void test_aspect_all_non_null(void)
{
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_NOT_NULL(aspect_glyph(i));
    }
}

void test_aspect_invalid_returns_null(void)
{
    TEST_ASSERT_NULL(aspect_glyph(-1));
    TEST_ASSERT_NULL(aspect_glyph(5));
}

void test_aspect_glyphs_have_pixels(void)
{
    for (int i = 0; i < 5; i++) {
        const unsigned char *g = aspect_glyph(i);
        int count = 0;
        for (int j = 0; j < GLYPH_BYTES; j++) {
            if (g[j]) count++;
        }
        TEST_ASSERT_TRUE(count > 0);
    }
}

/* glyph_pixel: 13-17 */

void test_pixel_valid_coords(void)
{
    const unsigned char *g = zodiac_glyph(0); /* Aries */
    /* Row 1, col 2 should be set (Aries has pixels at row 1) */
    int found = 0;
    for (int y = 0; y < GLYPH_HEIGHT; y++) {
        for (int x = 0; x < GLYPH_WIDTH; x++) {
            if (glyph_pixel(g, x, y)) found = 1;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_pixel_out_of_bounds_x(void)
{
    const unsigned char *g = zodiac_glyph(0);
    TEST_ASSERT_EQUAL_INT(0, glyph_pixel(g, -1, 0));
    TEST_ASSERT_EQUAL_INT(0, glyph_pixel(g, GLYPH_WIDTH, 0));
    TEST_ASSERT_EQUAL_INT(0, glyph_pixel(g, 100, 5));
}

void test_pixel_out_of_bounds_y(void)
{
    const unsigned char *g = zodiac_glyph(0);
    TEST_ASSERT_EQUAL_INT(0, glyph_pixel(g, 0, -1));
    TEST_ASSERT_EQUAL_INT(0, glyph_pixel(g, 0, GLYPH_HEIGHT));
    TEST_ASSERT_EQUAL_INT(0, glyph_pixel(g, 3, 999));
}

void test_pixel_null_glyph(void)
{
    TEST_ASSERT_EQUAL_INT(0, glyph_pixel(NULL, 0, 0));
    TEST_ASSERT_EQUAL_INT(0, glyph_pixel(NULL, 3, 5));
}

void test_pixel_corner_empty(void)
{
    /* Most glyphs have empty corners (0,0) */
    const unsigned char *g = zodiac_glyph(0);
    TEST_ASSERT_EQUAL_INT(0, glyph_pixel(g, 0, 0));
}

/* Distinctness: 18-19 */

void test_zodiac_glyphs_distinct(void)
{
    /* No two zodiac glyphs should be identical */
    for (int i = 0; i < 12; i++) {
        for (int j = i + 1; j < 12; j++) {
            const unsigned char *a = zodiac_glyph(i);
            const unsigned char *b = zodiac_glyph(j);
            int same = 1;
            for (int k = 0; k < GLYPH_BYTES; k++) {
                if (a[k] != b[k]) { same = 0; break; }
            }
            TEST_ASSERT_FALSE(same);
        }
    }
}

void test_planet_glyphs_distinct(void)
{
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            const unsigned char *a = planet_glyph(i);
            const unsigned char *b = planet_glyph(j);
            int same = 1;
            for (int k = 0; k < GLYPH_BYTES; k++) {
                if (a[k] != b[k]) { same = 0; break; }
            }
            TEST_ASSERT_FALSE(same);
        }
    }
}

/* Purity: 20 */

void test_purity(void)
{
    const unsigned char *a = zodiac_glyph(5);
    const unsigned char *b = zodiac_glyph(5);
    TEST_ASSERT_EQUAL_PTR(a, b);

    TEST_ASSERT_EQUAL_INT(glyph_pixel(a, 3, 3), glyph_pixel(b, 3, 3));
    TEST_ASSERT_EQUAL_INT(zodiac_glyph_count(), zodiac_glyph_count());
    TEST_ASSERT_EQUAL_INT(planet_glyph_count(), planet_glyph_count());
}

/* Glyph data integrity: 21-23 */

void test_glyph_values_binary(void)
{
    /* Every byte should be 0 or 1 */
    for (int i = 0; i < 12; i++) {
        const unsigned char *g = zodiac_glyph(i);
        for (int j = 0; j < GLYPH_BYTES; j++) {
            TEST_ASSERT_TRUE(g[j] == 0 || g[j] == 1);
        }
    }
    for (int i = 0; i < 8; i++) {
        const unsigned char *g = planet_glyph(i);
        for (int j = 0; j < GLYPH_BYTES; j++) {
            TEST_ASSERT_TRUE(g[j] == 0 || g[j] == 1);
        }
    }
    for (int i = 0; i < 5; i++) {
        const unsigned char *g = aspect_glyph(i);
        for (int j = 0; j < GLYPH_BYTES; j++) {
            TEST_ASSERT_TRUE(g[j] == 0 || g[j] == 1);
        }
    }
}

void test_glyph_dimensions(void)
{
    TEST_ASSERT_EQUAL_INT(8, GLYPH_WIDTH);
    TEST_ASSERT_EQUAL_INT(12, GLYPH_HEIGHT);
    TEST_ASSERT_EQUAL_INT(96, GLYPH_BYTES);
}

void test_aries_specific_pixels(void)
{
    /* Aries row 1 has pixels at columns 2 and 5 (the horn tips) */
    const unsigned char *g = zodiac_glyph(0);
    TEST_ASSERT_EQUAL_INT(1, glyph_pixel(g, 2, 1));
    TEST_ASSERT_EQUAL_INT(1, glyph_pixel(g, 5, 1));
    /* Center column 3,4 at bottom rows should be filled (the stem) */
    TEST_ASSERT_EQUAL_INT(1, glyph_pixel(g, 3, 7));
    TEST_ASSERT_EQUAL_INT(1, glyph_pixel(g, 4, 7));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_zodiac_all_non_null);
    RUN_TEST(test_zodiac_invalid_returns_null);
    RUN_TEST(test_zodiac_glyph_count);
    RUN_TEST(test_zodiac_glyphs_have_pixels);
    RUN_TEST(test_zodiac_glyphs_not_all_filled);
    RUN_TEST(test_planet_all_non_null);
    RUN_TEST(test_planet_invalid_returns_null);
    RUN_TEST(test_planet_glyph_count);
    RUN_TEST(test_planet_glyphs_have_pixels);
    RUN_TEST(test_aspect_all_non_null);
    RUN_TEST(test_aspect_invalid_returns_null);
    RUN_TEST(test_aspect_glyphs_have_pixels);
    RUN_TEST(test_pixel_valid_coords);
    RUN_TEST(test_pixel_out_of_bounds_x);
    RUN_TEST(test_pixel_out_of_bounds_y);
    RUN_TEST(test_pixel_null_glyph);
    RUN_TEST(test_pixel_corner_empty);
    RUN_TEST(test_zodiac_glyphs_distinct);
    RUN_TEST(test_planet_glyphs_distinct);
    RUN_TEST(test_purity);
    RUN_TEST(test_glyph_values_binary);
    RUN_TEST(test_glyph_dimensions);
    RUN_TEST(test_aries_specific_pixels);
    return UNITY_END();
}
