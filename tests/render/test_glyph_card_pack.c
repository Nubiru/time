/*
 * test_glyph_card_pack.c — Tests for system-to-glyph mapping
 */

#include "../unity/unity.h"
#include "../../src/render/glyph_card_pack.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* 1. Astrology maps to zodiac category */
void test_astrology_is_zodiac(void)
{
    TEST_ASSERT_EQUAL_INT(SDF_CAT_ZODIAC, gcp_system_category(9));
}

/* 2. I Ching maps to trigram category */
void test_iching_is_trigram(void)
{
    TEST_ASSERT_EQUAL_INT(SDF_CAT_TRIGRAM, gcp_system_category(8));
}

/* 3. Tzolkin maps to seal category */
void test_tzolkin_is_seal(void)
{
    TEST_ASSERT_EQUAL_INT(SDF_CAT_SEAL, gcp_system_category(1));
}

/* 4. Kabbalah maps to sefirah category */
void test_kabbalah_is_sefirah(void)
{
    TEST_ASSERT_EQUAL_INT(SDF_CAT_SEFIRAH, gcp_system_category(11));
}

/* 5. Human Design maps to planet category */
void test_hd_is_planet(void)
{
    TEST_ASSERT_EQUAL_INT(SDF_CAT_PLANET, gcp_system_category(10));
}

/* 6. Unknown system gets default zodiac */
void test_unknown_is_zodiac(void)
{
    TEST_ASSERT_EQUAL_INT(SDF_CAT_ZODIAC, gcp_system_category(99));
}

/* 7. Glyph index clamps to valid range */
void test_glyph_index_clamps(void)
{
    int idx = gcp_system_glyph_index(9, 15); /* Astrology, variant 15 */
    int max = sdf_glyph_count(SDF_CAT_ZODIAC);
    TEST_ASSERT_TRUE(idx >= 0);
    TEST_ASSERT_TRUE(idx < max);
}

/* 8. Negative variant clamped to 0 */
void test_negative_variant(void)
{
    int idx = gcp_system_glyph_index(9, -5);
    TEST_ASSERT_EQUAL_INT(0, idx);
}

/* 9. Default variant is 0 for all systems */
void test_default_variant_zero(void)
{
    for (int i = 0; i < 20; i++) {
        TEST_ASSERT_EQUAL_INT(0, gcp_default_variant(i));
    }
}

/* 10. Vertex shader has version string */
void test_vert_source(void)
{
    const char *src = gcp_vert_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

/* 11. Fragment shader has version string */
void test_frag_source(void)
{
    const char *src = gcp_frag_source();
    TEST_ASSERT_NOT_NULL(src);
    TEST_ASSERT_NOT_NULL(strstr(src, "#version 300 es"));
}

/* 12. Fragment shader has discard for transparent pixels */
void test_frag_has_discard(void)
{
    const char *src = gcp_frag_source();
    TEST_ASSERT_NOT_NULL(strstr(src, "discard"));
}

/* 13. Cosmic system maps to planet (Sun symbol) */
void test_cosmic_is_planet(void)
{
    TEST_ASSERT_EQUAL_INT(SDF_CAT_PLANET, gcp_system_category(13));
}

/* 14. Implemented categories return valid glyph counts */
void test_implemented_categories_have_glyphs(void)
{
    /* Zodiac (12) and Planet (10) are implemented */
    TEST_ASSERT_TRUE(sdf_glyph_count(SDF_CAT_ZODIAC) > 0);
    TEST_ASSERT_TRUE(sdf_glyph_count(SDF_CAT_PLANET) > 0);
    /* Seal, Sefirah, Trigram are TODO — may be 0 */
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_astrology_is_zodiac);
    RUN_TEST(test_iching_is_trigram);
    RUN_TEST(test_tzolkin_is_seal);
    RUN_TEST(test_kabbalah_is_sefirah);
    RUN_TEST(test_hd_is_planet);
    RUN_TEST(test_unknown_is_zodiac);
    RUN_TEST(test_glyph_index_clamps);
    RUN_TEST(test_negative_variant);
    RUN_TEST(test_default_variant_zero);
    RUN_TEST(test_vert_source);
    RUN_TEST(test_frag_source);
    RUN_TEST(test_frag_has_discard);
    RUN_TEST(test_cosmic_is_planet);
    RUN_TEST(test_implemented_categories_have_glyphs);
    return UNITY_END();
}
