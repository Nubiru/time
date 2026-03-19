#include "../unity/unity.h"
#include "../../src/ui/share_template.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- Helper: create a minimal test summary ---- */

static ts_summary_t make_summary(void)
{
    ts_summary_t s = { .jd = 2460500.0, .entry_count = 3, .convergence_score = 3 };
    snprintf(s.headline, TS_NOTE_MAX, "3 systems converge");

    s.entries[0].system = TS_SYS_GREGORIAN;
    snprintf(s.entries[0].date_str, TS_DATE_MAX, "March 19, 2026");
    s.entries[0].note[0] = '\0';
    s.entries[0].significance = 0;
    s.entries[0].active = 1;

    s.entries[1].system = TS_SYS_TZOLKIN;
    snprintf(s.entries[1].date_str, TS_DATE_MAX, "Kin 207 Blue Crystal Hand");
    snprintf(s.entries[1].note, TS_NOTE_MAX, "Portal Day");
    s.entries[1].significance = 2;
    s.entries[1].active = 1;

    s.entries[2].system = TS_SYS_CHINESE;
    snprintf(s.entries[2].date_str, TS_DATE_MAX, "Year of the Snake");
    s.entries[2].note[0] = '\0';
    s.entries[2].significance = 1;
    s.entries[2].active = 1;

    return s;
}

/* ==== Platform names ==== */

void test_platform_name_twitter(void)
{
    TEST_ASSERT_EQUAL_STRING("Twitter", sht_platform_name(SHT_TWITTER));
}

void test_platform_name_instagram(void)
{
    TEST_ASSERT_EQUAL_STRING("Instagram", sht_platform_name(SHT_INSTAGRAM));
}

void test_platform_name_whatsapp(void)
{
    TEST_ASSERT_EQUAL_STRING("WhatsApp", sht_platform_name(SHT_WHATSAPP));
}

void test_platform_name_clipboard(void)
{
    TEST_ASSERT_EQUAL_STRING("Clipboard", sht_platform_name(SHT_CLIPBOARD));
}

void test_platform_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("Unknown", sht_platform_name(SHT_PLATFORM_COUNT));
}

/* ==== Character limits ==== */

void test_char_limit_twitter(void)
{
    TEST_ASSERT_EQUAL_INT(280, sht_char_limit(SHT_TWITTER));
}

void test_char_limit_instagram(void)
{
    TEST_ASSERT_EQUAL_INT(2200, sht_char_limit(SHT_INSTAGRAM));
}

void test_char_limit_whatsapp(void)
{
    TEST_ASSERT_EQUAL_INT(1000, sht_char_limit(SHT_WHATSAPP));
}

void test_char_limit_clipboard(void)
{
    TEST_ASSERT_EQUAL_INT(4096, sht_char_limit(SHT_CLIPBOARD));
}

/* ==== Twitter formatting ==== */

void test_twitter_fits_in_280(void)
{
    ts_summary_t s = make_summary();
    sht_result_t r = sht_format(SHT_TWITTER, &s);
    TEST_ASSERT_TRUE(r.text_len > 0);
    TEST_ASSERT_TRUE(r.text_len <= 280);
}

void test_twitter_has_content(void)
{
    ts_summary_t s = make_summary();
    sht_result_t r = sht_format(SHT_TWITTER, &s);
    TEST_ASSERT_TRUE(strlen(r.text) > 0);
}

void test_twitter_includes_system_data(void)
{
    ts_summary_t s = make_summary();
    sht_result_t r = sht_format(SHT_TWITTER, &s);
    /* Should include at least one system's data */
    int found = (strstr(r.text, "March 19") != NULL ||
                 strstr(r.text, "Kin 207") != NULL ||
                 strstr(r.text, "Snake") != NULL);
    TEST_ASSERT_TRUE(found);
}

/* ==== Instagram formatting ==== */

void test_instagram_fits_in_2200(void)
{
    ts_summary_t s = make_summary();
    sht_result_t r = sht_format(SHT_INSTAGRAM, &s);
    TEST_ASSERT_TRUE(r.text_len > 0);
    TEST_ASSERT_TRUE(r.text_len <= 2200);
}

void test_instagram_richer_than_twitter(void)
{
    ts_summary_t s = make_summary();
    sht_result_t tw = sht_format(SHT_TWITTER, &s);
    sht_result_t ig = sht_format(SHT_INSTAGRAM, &s);
    TEST_ASSERT_TRUE(ig.text_len >= tw.text_len);
}

/* ==== WhatsApp formatting ==== */

void test_whatsapp_fits_in_1000(void)
{
    ts_summary_t s = make_summary();
    sht_result_t r = sht_format(SHT_WHATSAPP, &s);
    TEST_ASSERT_TRUE(r.text_len > 0);
    TEST_ASSERT_TRUE(r.text_len <= 1000);
}

/* ==== Clipboard formatting ==== */

void test_clipboard_has_all_entries(void)
{
    ts_summary_t s = make_summary();
    sht_result_t r = sht_format(SHT_CLIPBOARD, &s);
    /* Clipboard should include all system data */
    TEST_ASSERT_TRUE(strstr(r.text, "March 19") != NULL);
    TEST_ASSERT_TRUE(strstr(r.text, "Kin 207") != NULL);
    TEST_ASSERT_TRUE(strstr(r.text, "Snake") != NULL);
}

/* ==== Cosmic haiku ==== */

void test_haiku_produces_output(void)
{
    ts_summary_t s = make_summary();
    char buf[256];
    int len = sht_cosmic_haiku(&s, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_haiku_has_three_lines(void)
{
    ts_summary_t s = make_summary();
    char buf[256];
    sht_cosmic_haiku(&s, buf, sizeof(buf));
    /* Count newlines — should have 2 (3 lines) */
    int newlines = 0;
    for (const char *p = buf; *p; p++) {
        if (*p == '\n') newlines++;
    }
    TEST_ASSERT_EQUAL_INT(2, newlines);
}

void test_haiku_fits_in_buffer(void)
{
    ts_summary_t s = make_summary();
    char buf[64];
    int len = sht_cosmic_haiku(&s, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len < 64);
}

void test_haiku_null_summary(void)
{
    char buf[256];
    int len = sht_cosmic_haiku(NULL, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, len);
}

void test_haiku_null_buffer(void)
{
    ts_summary_t s = make_summary();
    int len = sht_cosmic_haiku(&s, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* ==== Edge cases ==== */

void test_format_null_summary(void)
{
    sht_result_t r = sht_format(SHT_TWITTER, NULL);
    TEST_ASSERT_EQUAL_INT(0, r.text_len);
}

void test_format_empty_summary(void)
{
    ts_summary_t s = { .jd = 2460000.0, .entry_count = 0 };
    s.headline[0] = '\0';
    sht_result_t r = sht_format(SHT_TWITTER, &s);
    /* Should still produce something (at least "Time" attribution) */
    TEST_ASSERT_TRUE(r.text_len >= 0);
}

void test_format_invalid_platform(void)
{
    ts_summary_t s = make_summary();
    sht_result_t r = sht_format(SHT_PLATFORM_COUNT, &s);
    TEST_ASSERT_EQUAL_INT(0, r.text_len);
}

/* ==== main runner ==== */

int main(void)
{
    UNITY_BEGIN();

    /* Platform names */
    RUN_TEST(test_platform_name_twitter);
    RUN_TEST(test_platform_name_instagram);
    RUN_TEST(test_platform_name_whatsapp);
    RUN_TEST(test_platform_name_clipboard);
    RUN_TEST(test_platform_name_invalid);

    /* Char limits */
    RUN_TEST(test_char_limit_twitter);
    RUN_TEST(test_char_limit_instagram);
    RUN_TEST(test_char_limit_whatsapp);
    RUN_TEST(test_char_limit_clipboard);

    /* Twitter */
    RUN_TEST(test_twitter_fits_in_280);
    RUN_TEST(test_twitter_has_content);
    RUN_TEST(test_twitter_includes_system_data);

    /* Instagram */
    RUN_TEST(test_instagram_fits_in_2200);
    RUN_TEST(test_instagram_richer_than_twitter);

    /* WhatsApp */
    RUN_TEST(test_whatsapp_fits_in_1000);

    /* Clipboard */
    RUN_TEST(test_clipboard_has_all_entries);

    /* Cosmic haiku */
    RUN_TEST(test_haiku_produces_output);
    RUN_TEST(test_haiku_has_three_lines);
    RUN_TEST(test_haiku_fits_in_buffer);
    RUN_TEST(test_haiku_null_summary);
    RUN_TEST(test_haiku_null_buffer);

    /* Edge cases */
    RUN_TEST(test_format_null_summary);
    RUN_TEST(test_format_empty_summary);
    RUN_TEST(test_format_invalid_platform);

    return UNITY_END();
}
