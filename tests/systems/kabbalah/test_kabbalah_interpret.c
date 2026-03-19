/* test_kabbalah_interpret.c — Kabbalah interpretation data module tests
 * TDD RED phase: tests written before implementation.
 * Source: Traditional Kabbalistic meditations, Golden Dawn attributions. */

#include "../../unity/unity.h"
#include "../../../src/systems/kabbalah/kabbalah_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Count functions ===== */

void test_sefirah_count_returns_10(void)
{
    TEST_ASSERT_EQUAL_INT(10, ki_sefirah_count());
}

void test_path_count_returns_22(void)
{
    TEST_ASSERT_EQUAL_INT(22, ki_path_count());
}

/* ===== ki_sefirah_data — Keter (0) ===== */

void test_sefirah_keter_index(void)
{
    ki_sefirah_t s = ki_sefirah_data(0);
    TEST_ASSERT_EQUAL_INT(0, s.index);
}

void test_sefirah_keter_meditation(void)
{
    ki_sefirah_t s = ki_sefirah_data(0);
    TEST_ASSERT_EQUAL_STRING(
        "Crown — the point before thought, pure will",
        s.meditation);
}

void test_sefirah_keter_question(void)
{
    ki_sefirah_t s = ki_sefirah_data(0);
    TEST_ASSERT_EQUAL_STRING(
        "What exists before I think?",
        s.question);
}

void test_sefirah_keter_brief(void)
{
    ki_sefirah_t s = ki_sefirah_data(0);
    TEST_ASSERT_EQUAL_STRING(
        "The unknowable origin — divine will before manifestation",
        s.brief);
}

/* ===== ki_sefirah_data — Tiferet (5) ===== */

void test_sefirah_tiferet_index(void)
{
    ki_sefirah_t s = ki_sefirah_data(5);
    TEST_ASSERT_EQUAL_INT(5, s.index);
}

void test_sefirah_tiferet_meditation(void)
{
    ki_sefirah_t s = ki_sefirah_data(5);
    TEST_ASSERT_EQUAL_STRING(
        "Beauty — the heart that balances mercy and judgment",
        s.meditation);
}

void test_sefirah_tiferet_question(void)
{
    ki_sefirah_t s = ki_sefirah_data(5);
    TEST_ASSERT_EQUAL_STRING(
        "How do I harmonize opposites?",
        s.question);
}

void test_sefirah_tiferet_brief(void)
{
    ki_sefirah_t s = ki_sefirah_data(5);
    TEST_ASSERT_EQUAL_STRING(
        "The heart center — beauty born from the marriage of giving and restraint",
        s.brief);
}

/* ===== ki_sefirah_data — Malkuth (9) ===== */

void test_sefirah_malkuth_index(void)
{
    ki_sefirah_t s = ki_sefirah_data(9);
    TEST_ASSERT_EQUAL_INT(9, s.index);
}

void test_sefirah_malkuth_meditation(void)
{
    ki_sefirah_t s = ki_sefirah_data(9);
    TEST_ASSERT_EQUAL_STRING(
        "Kingdom — the physical world, embodiment, presence",
        s.meditation);
}

void test_sefirah_malkuth_question(void)
{
    ki_sefirah_t s = ki_sefirah_data(9);
    TEST_ASSERT_EQUAL_STRING(
        "Am I fully here?",
        s.question);
}

void test_sefirah_malkuth_brief(void)
{
    ki_sefirah_t s = ki_sefirah_data(9);
    TEST_ASSERT_EQUAL_STRING(
        "The kingdom of the present moment — divinity in matter",
        s.brief);
}

/* ===== ki_sefirah_data — all 10 loop ===== */

void test_sefirah_all_have_nonempty_strings(void)
{
    for (int i = 0; i < 10; i++) {
        ki_sefirah_t s = ki_sefirah_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.index);
        TEST_ASSERT_NOT_NULL(s.meditation);
        TEST_ASSERT_TRUE(strlen(s.meditation) > 0);
        TEST_ASSERT_NOT_NULL(s.question);
        TEST_ASSERT_TRUE(strlen(s.question) > 0);
        TEST_ASSERT_NOT_NULL(s.brief);
        TEST_ASSERT_TRUE(strlen(s.brief) > 0);
    }
}

/* ===== ki_sefirah_data — invalid ===== */

void test_sefirah_invalid_high(void)
{
    ki_sefirah_t s = ki_sefirah_data(10);
    TEST_ASSERT_EQUAL_INT(-1, s.index);
    TEST_ASSERT_EQUAL_STRING("?", s.meditation);
    TEST_ASSERT_EQUAL_STRING("?", s.question);
    TEST_ASSERT_EQUAL_STRING("?", s.brief);
}

void test_sefirah_invalid_negative(void)
{
    ki_sefirah_t s = ki_sefirah_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.index);
    TEST_ASSERT_EQUAL_STRING("?", s.meditation);
    TEST_ASSERT_EQUAL_STRING("?", s.question);
    TEST_ASSERT_EQUAL_STRING("?", s.brief);
}

/* ===== ki_path_data — specific paths ===== */

void test_path_0_aleph_fool(void)
{
    ki_path_t p = ki_path_data(0);
    TEST_ASSERT_EQUAL_INT(0, p.index);
    TEST_ASSERT_EQUAL_STRING(
        "Stepping into the void with perfect trust",
        p.journey);
    TEST_ASSERT_EQUAL_STRING(
        "Faith requires no ground to stand on",
        p.lesson);
}

void test_path_6_zayin_lovers(void)
{
    ki_path_t p = ki_path_data(6);
    TEST_ASSERT_EQUAL_INT(6, p.index);
    TEST_ASSERT_EQUAL_STRING(
        "Understanding choosing beauty over severity",
        p.journey);
    TEST_ASSERT_EQUAL_STRING(
        "The first choice: love or fear — always love",
        p.lesson);
}

void test_path_21_tav_universe(void)
{
    ki_path_t p = ki_path_data(21);
    TEST_ASSERT_EQUAL_INT(21, p.index);
    TEST_ASSERT_EQUAL_STRING(
        "Foundation becoming kingdom",
        p.journey);
    TEST_ASSERT_EQUAL_STRING(
        "The last step: making the invisible visible",
        p.lesson);
}

/* ===== ki_path_data — all 22 loop ===== */

void test_path_all_have_nonempty_strings(void)
{
    for (int i = 0; i < 22; i++) {
        ki_path_t p = ki_path_data(i);
        TEST_ASSERT_EQUAL_INT(i, p.index);
        TEST_ASSERT_NOT_NULL(p.journey);
        TEST_ASSERT_TRUE(strlen(p.journey) > 0);
        TEST_ASSERT_NOT_NULL(p.lesson);
        TEST_ASSERT_TRUE(strlen(p.lesson) > 0);
    }
}

/* ===== ki_path_data — invalid ===== */

void test_path_invalid_high(void)
{
    ki_path_t p = ki_path_data(22);
    TEST_ASSERT_EQUAL_INT(-1, p.index);
    TEST_ASSERT_EQUAL_STRING("?", p.journey);
    TEST_ASSERT_EQUAL_STRING("?", p.lesson);
}

void test_path_invalid_negative(void)
{
    ki_path_t p = ki_path_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, p.index);
    TEST_ASSERT_EQUAL_STRING("?", p.journey);
    TEST_ASSERT_EQUAL_STRING("?", p.lesson);
}

/* ===== ki_interpret_sefirah — glyph format ===== */

void test_interpret_glyph_keter(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(0, NULL);
    TEST_ASSERT_EQUAL_STRING("Ket", r.glyph);
}

void test_interpret_glyph_tiferet(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(5, NULL);
    TEST_ASSERT_EQUAL_STRING("Tif", r.glyph);
}

void test_interpret_glyph_hod(void)
{
    /* Hod is only 3 chars — should still work */
    kabbalah_interp_t r = ki_interpret_sefirah(7, NULL);
    TEST_ASSERT_EQUAL_STRING("Hod", r.glyph);
}

void test_interpret_glyph_length(void)
{
    /* All glyphs should be exactly 3 chars */
    for (int i = 0; i < 10; i++) {
        kabbalah_interp_t r = ki_interpret_sefirah(i, NULL);
        TEST_ASSERT_EQUAL_INT(3, (int)strlen(r.glyph));
    }
}

/* ===== ki_interpret_sefirah — glance format ===== */

void test_interpret_glance_contains_name(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(5, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Tiferet"));
}

void test_interpret_glance_contains_brief(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(5, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.glance,
        "The heart center"));
}

void test_interpret_glance_format(void)
{
    /* Format: "{name} — {brief}" */
    kabbalah_interp_t r = ki_interpret_sefirah(0, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Keter"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "\xe2\x80\x94")); /* em dash */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "unknowable origin"));
}

/* ===== ki_interpret_sefirah — detail format ===== */

void test_interpret_detail_contains_meditation(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(5, "Sun");
    TEST_ASSERT_NOT_NULL(strstr(r.detail,
        "Beauty — the heart that balances mercy and judgment"));
}

void test_interpret_detail_contains_question(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(5, "Sun");
    TEST_ASSERT_NOT_NULL(strstr(r.detail,
        "How do I harmonize opposites?"));
}

void test_interpret_detail_with_planet(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(5, "Sun");
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Planet: Sun"));
}

void test_interpret_detail_null_planet(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(5, NULL);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Planet: unassigned"));
}

void test_interpret_detail_another_planet(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(3, "Jupiter");
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Planet: Jupiter"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Chesed"));
}

/* ===== ki_interpret_sefirah — invalid ===== */

void test_interpret_invalid_index(void)
{
    kabbalah_interp_t r = ki_interpret_sefirah(10, NULL);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_EQUAL_STRING("?", r.glance);
    TEST_ASSERT_EQUAL_STRING("?", r.detail);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Counts (2) */
    RUN_TEST(test_sefirah_count_returns_10);
    RUN_TEST(test_path_count_returns_22);

    /* Sefirah data — Keter (4) */
    RUN_TEST(test_sefirah_keter_index);
    RUN_TEST(test_sefirah_keter_meditation);
    RUN_TEST(test_sefirah_keter_question);
    RUN_TEST(test_sefirah_keter_brief);

    /* Sefirah data — Tiferet (4) */
    RUN_TEST(test_sefirah_tiferet_index);
    RUN_TEST(test_sefirah_tiferet_meditation);
    RUN_TEST(test_sefirah_tiferet_question);
    RUN_TEST(test_sefirah_tiferet_brief);

    /* Sefirah data — Malkuth (4) */
    RUN_TEST(test_sefirah_malkuth_index);
    RUN_TEST(test_sefirah_malkuth_meditation);
    RUN_TEST(test_sefirah_malkuth_question);
    RUN_TEST(test_sefirah_malkuth_brief);

    /* Sefirah data — loop all (1) */
    RUN_TEST(test_sefirah_all_have_nonempty_strings);

    /* Sefirah data — invalid (2) */
    RUN_TEST(test_sefirah_invalid_high);
    RUN_TEST(test_sefirah_invalid_negative);

    /* Path data — specific (3) */
    RUN_TEST(test_path_0_aleph_fool);
    RUN_TEST(test_path_6_zayin_lovers);
    RUN_TEST(test_path_21_tav_universe);

    /* Path data — loop all (1) */
    RUN_TEST(test_path_all_have_nonempty_strings);

    /* Path data — invalid (2) */
    RUN_TEST(test_path_invalid_high);
    RUN_TEST(test_path_invalid_negative);

    /* Interpret — glyph (4) */
    RUN_TEST(test_interpret_glyph_keter);
    RUN_TEST(test_interpret_glyph_tiferet);
    RUN_TEST(test_interpret_glyph_hod);
    RUN_TEST(test_interpret_glyph_length);

    /* Interpret — glance (3) */
    RUN_TEST(test_interpret_glance_contains_name);
    RUN_TEST(test_interpret_glance_contains_brief);
    RUN_TEST(test_interpret_glance_format);

    /* Interpret — detail (5) */
    RUN_TEST(test_interpret_detail_contains_meditation);
    RUN_TEST(test_interpret_detail_contains_question);
    RUN_TEST(test_interpret_detail_with_planet);
    RUN_TEST(test_interpret_detail_null_planet);
    RUN_TEST(test_interpret_detail_another_planet);

    /* Interpret — invalid (1) */
    RUN_TEST(test_interpret_invalid_index);

    return UNITY_END();
}
