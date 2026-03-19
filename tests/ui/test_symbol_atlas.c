#include "../unity/unity.h"
#include "../../src/ui/symbol_atlas.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Category counts: 1-6 --- */

void test_zodiac_count(void)
{
    TEST_ASSERT_EQUAL_INT(12, symbol_atlas_count(SYM_CAT_ZODIAC));
}

void test_planet_count(void)
{
    TEST_ASSERT_EQUAL_INT(8, symbol_atlas_count(SYM_CAT_PLANET));
}

void test_aspect_count(void)
{
    TEST_ASSERT_EQUAL_INT(5, symbol_atlas_count(SYM_CAT_ASPECT));
}

void test_trigram_count(void)
{
    TEST_ASSERT_EQUAL_INT(8, symbol_atlas_count(SYM_CAT_TRIGRAM));
}

void test_mayan_seal_count(void)
{
    TEST_ASSERT_EQUAL_INT(20, symbol_atlas_count(SYM_CAT_MAYAN_SEAL));
}

void test_lunar_phase_count(void)
{
    TEST_ASSERT_EQUAL_INT(8, symbol_atlas_count(SYM_CAT_LUNAR_PHASE));
}

/* --- Total symbol count: 7 --- */

void test_total_count(void)
{
    TEST_ASSERT_EQUAL_INT(61, symbol_atlas_total());
}

/* --- Glyph retrieval (non-null for valid): 8-13 --- */

void test_zodiac_glyphs_valid(void)
{
    for (int i = 0; i < 12; i++) {
        TEST_ASSERT_NOT_NULL(symbol_atlas_glyph(SYM_CAT_ZODIAC, i));
    }
}

void test_planet_glyphs_valid(void)
{
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_NOT_NULL(symbol_atlas_glyph(SYM_CAT_PLANET, i));
    }
}

void test_aspect_glyphs_valid(void)
{
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_NOT_NULL(symbol_atlas_glyph(SYM_CAT_ASPECT, i));
    }
}

void test_trigram_glyphs_valid(void)
{
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_NOT_NULL(symbol_atlas_glyph(SYM_CAT_TRIGRAM, i));
    }
}

void test_mayan_seal_glyphs_valid(void)
{
    for (int i = 0; i < 20; i++) {
        TEST_ASSERT_NOT_NULL(symbol_atlas_glyph(SYM_CAT_MAYAN_SEAL, i));
    }
}

void test_lunar_phase_glyphs_valid(void)
{
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_NOT_NULL(symbol_atlas_glyph(SYM_CAT_LUNAR_PHASE, i));
    }
}

/* --- Invalid indices return NULL: 14-16 --- */

void test_invalid_index_returns_null(void)
{
    TEST_ASSERT_NULL(symbol_atlas_glyph(SYM_CAT_ZODIAC, -1));
    TEST_ASSERT_NULL(symbol_atlas_glyph(SYM_CAT_ZODIAC, 12));
    TEST_ASSERT_NULL(symbol_atlas_glyph(SYM_CAT_PLANET, 8));
    TEST_ASSERT_NULL(symbol_atlas_glyph(SYM_CAT_TRIGRAM, -1));
    TEST_ASSERT_NULL(symbol_atlas_glyph(SYM_CAT_MAYAN_SEAL, 20));
    TEST_ASSERT_NULL(symbol_atlas_glyph(SYM_CAT_LUNAR_PHASE, 8));
}

void test_invalid_category_returns_null(void)
{
    TEST_ASSERT_NULL(symbol_atlas_glyph(-1, 0));
    TEST_ASSERT_NULL(symbol_atlas_glyph(99, 0));
}

void test_invalid_category_count_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, symbol_atlas_count(-1));
    TEST_ASSERT_EQUAL_INT(0, symbol_atlas_count(99));
}

/* --- Bitmap data integrity: 17-19 --- */

void test_all_glyphs_binary_values(void)
{
    for (int cat = 0; cat < SYM_CAT_COUNT; cat++) {
        int n = symbol_atlas_count(cat);
        for (int i = 0; i < n; i++) {
            const unsigned char *g = symbol_atlas_glyph(cat, i);
            TEST_ASSERT_NOT_NULL(g);
            for (int j = 0; j < GLYPH_BYTES; j++) {
                TEST_ASSERT_TRUE(g[j] == 0 || g[j] == 1);
            }
        }
    }
}

void test_all_glyphs_have_pixels(void)
{
    for (int cat = 0; cat < SYM_CAT_COUNT; cat++) {
        int n = symbol_atlas_count(cat);
        for (int i = 0; i < n; i++) {
            const unsigned char *g = symbol_atlas_glyph(cat, i);
            int filled = 0;
            for (int j = 0; j < GLYPH_BYTES; j++) {
                if (g[j]) filled++;
            }
            TEST_ASSERT_TRUE(filled > 0);
        }
    }
}

void test_no_glyph_fully_filled(void)
{
    for (int cat = 0; cat < SYM_CAT_COUNT; cat++) {
        int n = symbol_atlas_count(cat);
        for (int i = 0; i < n; i++) {
            const unsigned char *g = symbol_atlas_glyph(cat, i);
            int filled = 0;
            for (int j = 0; j < GLYPH_BYTES; j++) {
                if (g[j]) filled++;
            }
            TEST_ASSERT_TRUE(filled < GLYPH_BYTES);
        }
    }
}

/* --- Distinctness within categories: 20-22 --- */

void test_trigram_glyphs_distinct(void)
{
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            const unsigned char *a = symbol_atlas_glyph(SYM_CAT_TRIGRAM, i);
            const unsigned char *b = symbol_atlas_glyph(SYM_CAT_TRIGRAM, j);
            int same = 1;
            for (int k = 0; k < GLYPH_BYTES; k++) {
                if (a[k] != b[k]) { same = 0; break; }
            }
            TEST_ASSERT_FALSE(same);
        }
    }
}

void test_mayan_seal_glyphs_distinct(void)
{
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            const unsigned char *a = symbol_atlas_glyph(SYM_CAT_MAYAN_SEAL, i);
            const unsigned char *b = symbol_atlas_glyph(SYM_CAT_MAYAN_SEAL, j);
            int same = 1;
            for (int k = 0; k < GLYPH_BYTES; k++) {
                if (a[k] != b[k]) { same = 0; break; }
            }
            TEST_ASSERT_FALSE(same);
        }
    }
}

void test_lunar_phase_glyphs_distinct(void)
{
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            const unsigned char *a = symbol_atlas_glyph(SYM_CAT_LUNAR_PHASE, i);
            const unsigned char *b = symbol_atlas_glyph(SYM_CAT_LUNAR_PHASE, j);
            int same = 1;
            for (int k = 0; k < GLYPH_BYTES; k++) {
                if (a[k] != b[k]) { same = 0; break; }
            }
            TEST_ASSERT_FALSE(same);
        }
    }
}

/* --- Atlas UV coordinates: 23-28 --- */

void test_atlas_dimensions(void)
{
    symbol_atlas_info_t info = symbol_atlas_info();
    /* 61 symbols in an 8-column grid = 8 rows (64 cells) */
    TEST_ASSERT_EQUAL_INT(8, info.cols);
    TEST_ASSERT_EQUAL_INT(8, info.rows);
    TEST_ASSERT_EQUAL_INT(GLYPH_WIDTH, info.cell_w);
    TEST_ASSERT_EQUAL_INT(GLYPH_HEIGHT, info.cell_h);
}

void test_uv_valid_range(void)
{
    for (int cat = 0; cat < SYM_CAT_COUNT; cat++) {
        int n = symbol_atlas_count(cat);
        for (int i = 0; i < n; i++) {
            glyph_uv_t uv = symbol_atlas_uv(cat, i);
            TEST_ASSERT_TRUE(uv.u0 >= 0.0f && uv.u0 <= 1.0f);
            TEST_ASSERT_TRUE(uv.v0 >= 0.0f && uv.v0 <= 1.0f);
            TEST_ASSERT_TRUE(uv.u1 >= 0.0f && uv.u1 <= 1.0f);
            TEST_ASSERT_TRUE(uv.v1 >= 0.0f && uv.v1 <= 1.0f);
            TEST_ASSERT_TRUE(uv.u1 > uv.u0);
            TEST_ASSERT_TRUE(uv.v1 > uv.v0);
        }
    }
}

void test_uv_invalid_returns_zero(void)
{
    glyph_uv_t uv = symbol_atlas_uv(SYM_CAT_ZODIAC, -1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, uv.u0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, uv.v0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, uv.u1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, uv.v1);
}

void test_uv_no_overlap(void)
{
    /* Each symbol should have unique UV coordinates */
    glyph_uv_t all[64];
    int idx = 0;
    for (int cat = 0; cat < SYM_CAT_COUNT; cat++) {
        int n = symbol_atlas_count(cat);
        for (int i = 0; i < n; i++) {
            all[idx++] = symbol_atlas_uv(cat, i);
        }
    }
    for (int i = 0; i < idx; i++) {
        for (int j = i + 1; j < idx; j++) {
            int same = (all[i].u0 == all[j].u0 && all[i].v0 == all[j].v0);
            TEST_ASSERT_FALSE(same);
        }
    }
}

void test_first_uv_at_origin(void)
{
    glyph_uv_t uv = symbol_atlas_uv(SYM_CAT_ZODIAC, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, uv.u0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, uv.v0);
}

void test_uv_cell_size_consistent(void)
{
    symbol_atlas_info_t info = symbol_atlas_info();
    float expected_w = 1.0f / (float)info.cols;
    float expected_h = 1.0f / (float)info.rows;
    glyph_uv_t uv = symbol_atlas_uv(SYM_CAT_ZODIAC, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected_w, uv.u1 - uv.u0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expected_h, uv.v1 - uv.v0);
}

/* --- Texture pack: 29-31 --- */

void test_pack_buffer_size(void)
{
    symbol_atlas_info_t info = symbol_atlas_info();
    int expected = info.cols * info.cell_w * info.rows * info.cell_h * 4; /* RGBA */
    TEST_ASSERT_EQUAL_INT(expected, symbol_atlas_texture_size());
}

void test_pack_buffer_not_empty(void)
{
    int size = symbol_atlas_texture_size();
    unsigned char buf[size];
    symbol_atlas_pack(buf, size);

    int nonzero = 0;
    for (int i = 0; i < size; i++) {
        if (buf[i] != 0) nonzero++;
    }
    TEST_ASSERT_TRUE(nonzero > 0);
}

void test_pack_alpha_channel(void)
{
    /* Pixels should have alpha=255 where filled, alpha=0 where empty */
    int size = symbol_atlas_texture_size();
    unsigned char buf[size];
    symbol_atlas_pack(buf, size);

    /* Check first glyph (Aries) — row 1 col 2 should be filled */
    symbol_atlas_info_t info = symbol_atlas_info();
    int tex_w = info.cols * info.cell_w;
    /* Aries is at atlas position (0,0). Row 1, Col 2 of glyph = pixel (2, 1) */
    int px = 2;
    int py = 1;
    int offset = (py * tex_w + px) * 4;
    /* Alpha channel is at offset+3 */
    TEST_ASSERT_EQUAL_UINT8(255, buf[offset + 3]);

    /* Corner (0,0) of Aries should be empty */
    offset = 0;
    TEST_ASSERT_EQUAL_UINT8(0, buf[offset + 3]);
}

/* --- Category names: 32-33 --- */

void test_category_name_valid(void)
{
    TEST_ASSERT_NOT_NULL(symbol_atlas_category_name(SYM_CAT_ZODIAC));
    TEST_ASSERT_NOT_NULL(symbol_atlas_category_name(SYM_CAT_PLANET));
    TEST_ASSERT_NOT_NULL(symbol_atlas_category_name(SYM_CAT_TRIGRAM));
    TEST_ASSERT_NOT_NULL(symbol_atlas_category_name(SYM_CAT_MAYAN_SEAL));
    TEST_ASSERT_NOT_NULL(symbol_atlas_category_name(SYM_CAT_LUNAR_PHASE));
}

void test_category_name_invalid(void)
{
    TEST_ASSERT_NULL(symbol_atlas_category_name(-1));
    TEST_ASSERT_NULL(symbol_atlas_category_name(99));
}

/* --- Symbol names: 34-36 --- */

void test_zodiac_symbol_names(void)
{
    const char *name = symbol_atlas_symbol_name(SYM_CAT_ZODIAC, 0);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Aries", name);
}

void test_mayan_seal_names(void)
{
    const char *name = symbol_atlas_symbol_name(SYM_CAT_MAYAN_SEAL, 0);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Dragon", name);
}

void test_symbol_name_invalid(void)
{
    TEST_ASSERT_NULL(symbol_atlas_symbol_name(SYM_CAT_ZODIAC, -1));
    TEST_ASSERT_NULL(symbol_atlas_symbol_name(SYM_CAT_ZODIAC, 12));
    TEST_ASSERT_NULL(symbol_atlas_symbol_name(-1, 0));
}

/* --- Zodiac delegates to zodiac_glyphs.h: 37 --- */

void test_zodiac_delegates_correctly(void)
{
    /* symbol_atlas zodiac data should be identical to zodiac_glyphs.h data */
    const unsigned char *atlas = symbol_atlas_glyph(SYM_CAT_ZODIAC, 0);
    const unsigned char *orig = zodiac_glyph(0);
    TEST_ASSERT_NOT_NULL(atlas);
    TEST_ASSERT_NOT_NULL(orig);
    for (int j = 0; j < GLYPH_BYTES; j++) {
        TEST_ASSERT_EQUAL_UINT8(orig[j], atlas[j]);
    }
}

/* --- Purity: 38 --- */

void test_purity(void)
{
    const unsigned char *a = symbol_atlas_glyph(SYM_CAT_TRIGRAM, 3);
    const unsigned char *b = symbol_atlas_glyph(SYM_CAT_TRIGRAM, 3);
    TEST_ASSERT_EQUAL_PTR(a, b);
    TEST_ASSERT_EQUAL_INT(symbol_atlas_total(), symbol_atlas_total());
    TEST_ASSERT_EQUAL_INT(symbol_atlas_count(SYM_CAT_ZODIAC),
                          symbol_atlas_count(SYM_CAT_ZODIAC));
}

/* --- Trigram specifics: 39-40 --- */

void test_trigram_qian_three_solid(void)
{
    /* Qian (Heaven, trigram 0) = three solid lines */
    const unsigned char *g = symbol_atlas_glyph(SYM_CAT_TRIGRAM, 0);
    TEST_ASSERT_NOT_NULL(g);
    /* Should have filled pixels in the middle columns */
    int filled = 0;
    for (int j = 0; j < GLYPH_BYTES; j++) {
        if (g[j]) filled++;
    }
    TEST_ASSERT_TRUE(filled >= 18); /* Three solid lines across */
}

void test_trigram_kun_three_broken(void)
{
    /* Kun (Earth, trigram 7) = three broken lines */
    const unsigned char *g = symbol_atlas_glyph(SYM_CAT_TRIGRAM, 7);
    TEST_ASSERT_NOT_NULL(g);
    int filled = 0;
    for (int j = 0; j < GLYPH_BYTES; j++) {
        if (g[j]) filled++;
    }
    TEST_ASSERT_TRUE(filled >= 12); /* Three broken lines */
    /* Kun should have fewer filled pixels than Qian */
    const unsigned char *qian = symbol_atlas_glyph(SYM_CAT_TRIGRAM, 0);
    int qian_filled = 0;
    for (int j = 0; j < GLYPH_BYTES; j++) {
        if (qian[j]) qian_filled++;
    }
    TEST_ASSERT_TRUE(filled < qian_filled);
}

/* --- Lunar phase specifics: 41-42 --- */

void test_lunar_new_moon_mostly_empty(void)
{
    /* New Moon (phase 0) should have minimal fill (thin circle outline) */
    const unsigned char *g = symbol_atlas_glyph(SYM_CAT_LUNAR_PHASE, 0);
    int filled = 0;
    for (int j = 0; j < GLYPH_BYTES; j++) {
        if (g[j]) filled++;
    }
    TEST_ASSERT_TRUE(filled < GLYPH_BYTES / 2);
}

void test_lunar_full_moon_most_filled(void)
{
    /* Full Moon (phase 4) should be the most filled lunar phase */
    const unsigned char *full = symbol_atlas_glyph(SYM_CAT_LUNAR_PHASE, 4);
    int full_count = 0;
    for (int j = 0; j < GLYPH_BYTES; j++) {
        if (full[j]) full_count++;
    }
    /* Full moon should have significant fill */
    TEST_ASSERT_TRUE(full_count > 20);
}

int main(void)
{
    UNITY_BEGIN();
    /* Category counts */
    RUN_TEST(test_zodiac_count);
    RUN_TEST(test_planet_count);
    RUN_TEST(test_aspect_count);
    RUN_TEST(test_trigram_count);
    RUN_TEST(test_mayan_seal_count);
    RUN_TEST(test_lunar_phase_count);
    RUN_TEST(test_total_count);
    /* Glyph retrieval */
    RUN_TEST(test_zodiac_glyphs_valid);
    RUN_TEST(test_planet_glyphs_valid);
    RUN_TEST(test_aspect_glyphs_valid);
    RUN_TEST(test_trigram_glyphs_valid);
    RUN_TEST(test_mayan_seal_glyphs_valid);
    RUN_TEST(test_lunar_phase_glyphs_valid);
    /* Invalid indices */
    RUN_TEST(test_invalid_index_returns_null);
    RUN_TEST(test_invalid_category_returns_null);
    RUN_TEST(test_invalid_category_count_zero);
    /* Bitmap integrity */
    RUN_TEST(test_all_glyphs_binary_values);
    RUN_TEST(test_all_glyphs_have_pixels);
    RUN_TEST(test_no_glyph_fully_filled);
    /* Distinctness */
    RUN_TEST(test_trigram_glyphs_distinct);
    RUN_TEST(test_mayan_seal_glyphs_distinct);
    RUN_TEST(test_lunar_phase_glyphs_distinct);
    /* Atlas UV */
    RUN_TEST(test_atlas_dimensions);
    RUN_TEST(test_uv_valid_range);
    RUN_TEST(test_uv_invalid_returns_zero);
    RUN_TEST(test_uv_no_overlap);
    RUN_TEST(test_first_uv_at_origin);
    RUN_TEST(test_uv_cell_size_consistent);
    /* Texture pack */
    RUN_TEST(test_pack_buffer_size);
    RUN_TEST(test_pack_buffer_not_empty);
    RUN_TEST(test_pack_alpha_channel);
    /* Category names */
    RUN_TEST(test_category_name_valid);
    RUN_TEST(test_category_name_invalid);
    /* Symbol names */
    RUN_TEST(test_zodiac_symbol_names);
    RUN_TEST(test_mayan_seal_names);
    RUN_TEST(test_symbol_name_invalid);
    /* Delegation */
    RUN_TEST(test_zodiac_delegates_correctly);
    /* Purity */
    RUN_TEST(test_purity);
    /* Trigram specifics */
    RUN_TEST(test_trigram_qian_three_solid);
    RUN_TEST(test_trigram_kun_three_broken);
    /* Lunar specifics */
    RUN_TEST(test_lunar_new_moon_mostly_empty);
    RUN_TEST(test_lunar_full_moon_most_filled);
    return UNITY_END();
}
