/* test_human_design_today.c — Tests for Human Design "Today" transit page
 *
 * 25+ tests covering hdt_today() and hdt_format(). */

#include "../../unity/unity.h"
#include "../../../src/systems/human_design/human_design_today.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * hdt_today — basic validity at sun_lon=359.0
 * Gate 25 "The Spirit of the Self", Earth Gate 46 "Discovery"
 * ============================================================ */

void test_today_has_data(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_EQUAL_INT(1, p.has_data);
}

void test_today_section_count(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_EQUAL_INT(5, p.section_count);
}

void test_today_sun_gate_valid(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_TRUE(p.sun_gate >= 1 && p.sun_gate <= 64);
}

void test_today_earth_gate_valid(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_TRUE(p.earth_gate >= 1 && p.earth_gate <= 64);
}

void test_today_sun_gate_359(void) {
    hdt_page_t p = hdt_today(359.0);
    /* 359.0 degrees -> position 63 -> GATE_SEQUENCE[63] = 25 */
    TEST_ASSERT_EQUAL_INT(25, p.sun_gate);
}

void test_today_earth_gate_359(void) {
    hdt_page_t p = hdt_today(359.0);
    /* Earth = sun + 180 = 539 -> normalized 179.0 -> pos 31 -> gate 46 */
    TEST_ASSERT_EQUAL_INT(46, p.earth_gate);
}

/* ============================================================
 * page_title
 * ============================================================ */

void test_today_title_contains_human_design(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Human Design"));
}

void test_today_title_contains_gate(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "Gate"));
}

void test_today_title_contains_gate_name(void) {
    hdt_page_t p = hdt_today(359.0);
    /* Gate 25 = "The Spirit of the Self" */
    TEST_ASSERT_NOT_NULL(strstr(p.page_title, "The Spirit of the Self"));
}

/* ============================================================
 * gate_section
 * ============================================================ */

void test_today_gate_section_sun_gate(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.gate_section, "Sun Gate:"));
}

void test_today_gate_section_earth_gate(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.gate_section, "Earth Gate:"));
}

void test_today_gate_section_keyword(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.gate_section, "Keyword:"));
}

void test_today_gate_section_sun_name(void) {
    hdt_page_t p = hdt_today(359.0);
    /* Gate 25 name */
    TEST_ASSERT_NOT_NULL(strstr(p.gate_section, "The Spirit of the Self"));
}

void test_today_gate_section_earth_name(void) {
    hdt_page_t p = hdt_today(359.0);
    /* Gate 46 name = "Discovery" */
    TEST_ASSERT_NOT_NULL(strstr(p.gate_section, "Discovery"));
}

/* ============================================================
 * line_section
 * ============================================================ */

void test_today_line_section_sun_line(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.line_section, "Sun Line:"));
}

void test_today_line_section_earth_line(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.line_section, "Earth Line:"));
}

void test_today_line_section_profile(void) {
    hdt_page_t p = hdt_today(359.0);
    /* sun.line=5, earth.line=5 -> profile 5/5 is invalid -> shows "5/5" */
    TEST_ASSERT_NOT_NULL(strstr(p.line_section, "Profile:"));
}

/* ============================================================
 * type_section
 * ============================================================ */

void test_today_type_section_transit_energy(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.type_section, "Transit Energy:"));
}

void test_today_type_section_strategy(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.type_section, "Strategy:"));
}

void test_today_type_section_signature(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.type_section, "Signature:"));
}

void test_today_type_section_generator(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.type_section, "Generator"));
}

void test_today_type_section_not_self(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.type_section, "Not-Self:"));
}

/* ============================================================
 * center_section
 * ============================================================ */

void test_today_center_section_header(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.center_section, "Centers:"));
}

void test_today_center_section_head(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.center_section, "Head:"));
}

void test_today_center_section_ajna(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.center_section, "Ajna:"));
}

void test_today_center_section_throat(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.center_section, "Throat:"));
}

/* ============================================================
 * cross_section
 * ============================================================ */

void test_today_cross_section_iching(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.cross_section, "I Ching"));
}

void test_today_cross_section_hexagram(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.cross_section, "Hexagram"));
}

void test_today_cross_section_64(void) {
    hdt_page_t p = hdt_today(359.0);
    TEST_ASSERT_NOT_NULL(strstr(p.cross_section, "64"));
}

/* ============================================================
 * Different sun_lon values produce different gates
 * ============================================================ */

void test_today_different_lon_different_gate(void) {
    hdt_page_t p1 = hdt_today(0.0);
    hdt_page_t p2 = hdt_today(180.0);
    /* 0.0 -> gate 17, 180.0 -> gate 18 */
    TEST_ASSERT_NOT_EQUAL(p1.sun_gate, p2.sun_gate);
}

void test_today_lon_0_gate_17(void) {
    hdt_page_t p = hdt_today(0.0);
    /* 0.0 degrees -> position 0 -> GATE_SEQUENCE[0] = 17 */
    TEST_ASSERT_EQUAL_INT(17, p.sun_gate);
}

void test_today_lon_180_gate_18(void) {
    hdt_page_t p = hdt_today(180.0);
    /* 180.0 degrees -> position 32 -> GATE_SEQUENCE[32] = 18 */
    TEST_ASSERT_EQUAL_INT(18, p.sun_gate);
}

/* ============================================================
 * hdt_format — output
 * ============================================================ */

void test_format_produces_output(void) {
    hdt_page_t p = hdt_today(359.0);
    char buf[2048];
    int n = hdt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_title(void) {
    hdt_page_t p = hdt_today(359.0);
    char buf[2048];
    hdt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Human Design"));
}

void test_format_contains_sections(void) {
    hdt_page_t p = hdt_today(359.0);
    char buf[2048];
    hdt_format(&p, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun Gate:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Sun Line:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Transit Energy:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Centers:"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "I Ching"));
}

void test_format_null_buf(void) {
    hdt_page_t p = hdt_today(359.0);
    int n = hdt_format(&p, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_format_small_buf(void) {
    hdt_page_t p = hdt_today(359.0);
    char buf[16];
    int n = hdt_format(&p, buf, (int)sizeof(buf));
    /* Should truncate but not crash */
    TEST_ASSERT_TRUE(n >= 0);
    TEST_ASSERT_TRUE(strlen(buf) < 16);
}

void test_format_null_page(void) {
    char buf[512];
    int n = hdt_format(NULL, buf, (int)sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* hdt_today basic */
    RUN_TEST(test_today_has_data);
    RUN_TEST(test_today_section_count);
    RUN_TEST(test_today_sun_gate_valid);
    RUN_TEST(test_today_earth_gate_valid);
    RUN_TEST(test_today_sun_gate_359);
    RUN_TEST(test_today_earth_gate_359);

    /* page_title */
    RUN_TEST(test_today_title_contains_human_design);
    RUN_TEST(test_today_title_contains_gate);
    RUN_TEST(test_today_title_contains_gate_name);

    /* gate_section */
    RUN_TEST(test_today_gate_section_sun_gate);
    RUN_TEST(test_today_gate_section_earth_gate);
    RUN_TEST(test_today_gate_section_keyword);
    RUN_TEST(test_today_gate_section_sun_name);
    RUN_TEST(test_today_gate_section_earth_name);

    /* line_section */
    RUN_TEST(test_today_line_section_sun_line);
    RUN_TEST(test_today_line_section_earth_line);
    RUN_TEST(test_today_line_section_profile);

    /* type_section */
    RUN_TEST(test_today_type_section_transit_energy);
    RUN_TEST(test_today_type_section_strategy);
    RUN_TEST(test_today_type_section_signature);
    RUN_TEST(test_today_type_section_generator);
    RUN_TEST(test_today_type_section_not_self);

    /* center_section */
    RUN_TEST(test_today_center_section_header);
    RUN_TEST(test_today_center_section_head);
    RUN_TEST(test_today_center_section_ajna);
    RUN_TEST(test_today_center_section_throat);

    /* cross_section */
    RUN_TEST(test_today_cross_section_iching);
    RUN_TEST(test_today_cross_section_hexagram);
    RUN_TEST(test_today_cross_section_64);

    /* Different longitudes */
    RUN_TEST(test_today_different_lon_different_gate);
    RUN_TEST(test_today_lon_0_gate_17);
    RUN_TEST(test_today_lon_180_gate_18);

    /* hdt_format */
    RUN_TEST(test_format_produces_output);
    RUN_TEST(test_format_contains_title);
    RUN_TEST(test_format_contains_sections);
    RUN_TEST(test_format_null_buf);
    RUN_TEST(test_format_small_buf);
    RUN_TEST(test_format_null_page);

    return UNITY_END();
}
