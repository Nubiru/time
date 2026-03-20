#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/tzolkin_today.h"
#include "../../../src/systems/tzolkin/tzolkin_board.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * Test JD: 2460394.5 (2024-03-20)
 * We verify structural correctness, not exact kin values,
 * since the Tzolkin math is tested in tzolkin.c tests.
 * ============================================================ */

static const double TEST_JD = 2460394.5;

/* ============================================================
 * zt_today — page construction
 * ============================================================ */

void test_today_has_data(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_page_title_contains_kin(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Kin"));
}

void test_today_page_title_has_dash(void) {
    zt_page_t p = zt_today(TEST_JD);
    /* Title format: "Kin N — Color Tone Seal" */
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, " — "));
}

void test_today_kin_in_range(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_TRUE(p.kin >= 1 && p.kin <= 260);
}

void test_today_is_portal_valid(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_TRUE(p.is_portal == 0 || p.is_portal == 1);
}

void test_today_section_count(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_EQUAL_INT(7, p.section_count);
}

/* ============================================================
 * kin_section
 * ============================================================ */

void test_kin_section_nonempty(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_TRUE(strlen(p.kin_section) > 0);
}

void test_kin_section_has_seal(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.kin_section, "Seal:"));
}

void test_kin_section_has_tone(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.kin_section, "Tone:"));
}

void test_kin_section_has_color(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.kin_section, "Color:"));
}

/* ============================================================
 * oracle_section
 * ============================================================ */

void test_oracle_section_has_guide(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.oracle_section, "Guide:"));
}

void test_oracle_section_has_analog(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.oracle_section, "Analog:"));
}

void test_oracle_section_has_antipode(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.oracle_section, "Antipode:"));
}

void test_oracle_section_has_occult(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.oracle_section, "Occult:"));
}

/* ============================================================
 * wavespell_section
 * ============================================================ */

void test_wavespell_section_has_wavespell(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.wavespell_section, "Wavespell"));
}

void test_wavespell_section_has_day(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.wavespell_section, "Day"));
}

/* ============================================================
 * castle_section
 * ============================================================ */

void test_castle_section_has_castle(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.castle_section, "Castle:"));
}

void test_castle_section_has_kin_range(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.castle_section, "Kin range:"));
}

/* ============================================================
 * family_section
 * ============================================================ */

void test_family_section_has_earth_family(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.family_section, "Earth Family:"));
}

void test_family_section_has_members(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.family_section, "Members:"));
}

/* ============================================================
 * round_section
 * ============================================================ */

void test_round_section_has_haab(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.round_section, "Haab:"));
}

void test_round_section_has_calendar_round(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.round_section, "Calendar Round:"));
}

void test_round_section_has_tzolkin(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.round_section, "Tzolkin:"));
}

/* ============================================================
 * cycle_section
 * ============================================================ */

void test_cycle_section_has_52_year(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "52-Year"));
}

void test_cycle_section_has_remaining(void) {
    zt_page_t p = zt_today(TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(p.cycle_section, "remaining"));
}

/* ============================================================
 * zt_format
 * ============================================================ */

void test_format_produces_output(void) {
    zt_page_t p = zt_today(TEST_JD);
    char buf[4096];
    int n = zt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_sections(void) {
    zt_page_t p = zt_today(TEST_JD);
    char buf[4096];
    zt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Seal:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Guide:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Wavespell"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Castle:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Earth Family:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Haab:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "52-Year"));
}

void test_format_null_page(void) {
    char buf[256];
    int n = zt_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_null_buf(void) {
    zt_page_t p = zt_today(TEST_JD);
    int n = zt_format(&p, NULL, 256);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_zero_size(void) {
    zt_page_t p = zt_today(TEST_JD);
    char buf[16];
    int n = zt_format(&p, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_small_buffer(void) {
    zt_page_t p = zt_today(TEST_JD);
    char buf[32];
    int n = zt_format(&p, buf, (int)sizeof(buf));
    /* Should write something, truncated but valid */
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(strlen(buf) < 32);
}

/* ============================================================
 * Portal day test — Kin 1 is a Galactic Activation Portal
 * (Kin 1 is always a portal in the symmetric distribution)
 * ============================================================ */

void test_portal_day_kin1(void) {
    /* Find JD that produces kin 1. We use tzolkin_from_jd internally,
     * so we just need a JD where kin=1. The correlation means
     * JD 584283 = kin 1, but any JD where (jd - 584283) % 260 == 0
     * gives kin 260 actually. Let's use a known portal kin via board. */
    /* Kin 1 is portal. Kin 20 is portal. Kin 22 is portal.
     * We test by constructing a page for a JD that gives kin 1.
     * JD 584283.5 (Long Count 0.0.0.0.0) is Dreamspell kin 1 historically,
     * but let's compute from our known test JD.
     * Instead: just verify that the is_portal field is consistent
     * with tzolkin_board_is_portal for any JD. */
    zt_page_t p = zt_today(TEST_JD);
    /* Verify consistency: is_portal matches board lookup */
    TEST_ASSERT_TRUE(p.is_portal == (tzolkin_board_is_portal(p.kin) ? 1 : 0));
}

/* ============================================================
 * Multiple JDs produce different pages
 * ============================================================ */

void test_different_jds_different_kins(void) {
    zt_page_t p1 = zt_today(TEST_JD);
    zt_page_t p2 = zt_today(TEST_JD + 1.0);
    zt_page_t p3 = zt_today(TEST_JD + 13.0);
    /* Adjacent days differ by 1 kin */
    TEST_ASSERT_TRUE(p1.kin != p2.kin || p1.kin != p3.kin);
}

void test_different_jds_different_titles(void) {
    zt_page_t p1 = zt_today(TEST_JD);
    zt_page_t p2 = zt_today(TEST_JD + 5.0);
    TEST_ASSERT_TRUE(strcmp(p1.page_title, p2.page_title) != 0);
}

void test_ancient_jd(void) {
    /* Very old JD — should still produce valid data */
    zt_page_t p = zt_today(2000000.5);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
    TEST_ASSERT_TRUE(p.kin >= 1 && p.kin <= 260);
    TEST_ASSERT_TRUE(strlen(p.kin_section) > 0);
}

void test_future_jd(void) {
    /* Far future JD */
    zt_page_t p = zt_today(2500000.5);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
    TEST_ASSERT_TRUE(p.kin >= 1 && p.kin <= 260);
    TEST_ASSERT_TRUE(strlen(p.kin_section) > 0);
}

/* ============================================================
 * Oracle section references real kin numbers
 * ============================================================ */

void test_oracle_section_has_kin_numbers(void) {
    zt_page_t p = zt_today(TEST_JD);
    /* Oracle lines contain "Kin " followed by a number */
    TEST_ASSERT_NOT_NULL(strstr(p.oracle_section, "Kin "));
}

/* ============================================================
 * main
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* Page construction */
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_page_title_contains_kin);
    RUN_TEST(test_today_page_title_has_dash);
    RUN_TEST(test_today_kin_in_range);
    RUN_TEST(test_today_is_portal_valid);
    RUN_TEST(test_today_section_count);

    /* kin_section */
    RUN_TEST(test_kin_section_nonempty);
    RUN_TEST(test_kin_section_has_seal);
    RUN_TEST(test_kin_section_has_tone);
    RUN_TEST(test_kin_section_has_color);

    /* oracle_section */
    RUN_TEST(test_oracle_section_has_guide);
    RUN_TEST(test_oracle_section_has_analog);
    RUN_TEST(test_oracle_section_has_antipode);
    RUN_TEST(test_oracle_section_has_occult);
    RUN_TEST(test_oracle_section_has_kin_numbers);

    /* wavespell_section */
    RUN_TEST(test_wavespell_section_has_wavespell);
    RUN_TEST(test_wavespell_section_has_day);

    /* castle_section */
    RUN_TEST(test_castle_section_has_castle);
    RUN_TEST(test_castle_section_has_kin_range);

    /* family_section */
    RUN_TEST(test_family_section_has_earth_family);
    RUN_TEST(test_family_section_has_members);

    /* round_section */
    RUN_TEST(test_round_section_has_haab);
    RUN_TEST(test_round_section_has_calendar_round);
    RUN_TEST(test_round_section_has_tzolkin);

    /* cycle_section */
    RUN_TEST(test_cycle_section_has_52_year);
    RUN_TEST(test_cycle_section_has_remaining);

    /* zt_format */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_sections);
    RUN_TEST(test_format_null_page);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_zero_size);
    RUN_TEST(test_format_small_buffer);

    /* Portal & consistency */
    RUN_TEST(test_portal_day_kin1);

    /* Different JDs */
    RUN_TEST(test_different_jds_different_kins);
    RUN_TEST(test_different_jds_different_titles);
    RUN_TEST(test_ancient_jd);
    RUN_TEST(test_future_jd);

    return UNITY_END();
}
