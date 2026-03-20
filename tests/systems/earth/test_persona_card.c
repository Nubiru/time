/* test_persona_card.c — Tests for persona_card adapter module.
 * TDD: adapts PERSONA data structs to card_content_t for the card pipeline. */

#include "unity.h"
#include "systems/earth/persona_card.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== pc_empty_card ===== */

void test_empty_card_title_empty(void) {
    card_content_t c = pc_empty_card();
    TEST_ASSERT_EQUAL_STRING("", c.title);
}

void test_empty_card_line1_empty(void) {
    card_content_t c = pc_empty_card();
    TEST_ASSERT_EQUAL_STRING("", c.line1);
}

void test_empty_card_line2_empty(void) {
    card_content_t c = pc_empty_card();
    TEST_ASSERT_EQUAL_STRING("", c.line2);
}

void test_empty_card_line3_empty(void) {
    card_content_t c = pc_empty_card();
    TEST_ASSERT_EQUAL_STRING("", c.line3);
}

void test_empty_card_detail_empty(void) {
    card_content_t c = pc_empty_card();
    TEST_ASSERT_EQUAL_STRING("", c.detail);
}

/* ===== pc_today_card — normal case ===== */

void test_today_card_title(void) {
    card_content_t c = pc_today_card("Good morning", "Your birth seal echoes today",
                                     "Active", 3, 10, 0);
    TEST_ASSERT_EQUAL_STRING("Today for You", c.title);
}

void test_today_card_line1_greeting(void) {
    card_content_t c = pc_today_card("Good morning", "headline", "Active", 3, 10, 0);
    TEST_ASSERT_EQUAL_STRING("Good morning", c.line1);
}

void test_today_card_line2_headline(void) {
    card_content_t c = pc_today_card("Good morning", "Your birth seal echoes today",
                                     "Active", 3, 10, 0);
    TEST_ASSERT_EQUAL_STRING("Your birth seal echoes today", c.line2);
}

void test_today_card_line3_energy_highlights(void) {
    card_content_t c = pc_today_card("Good morning", "headline",
                                     "Active", 3, 10, 0);
    TEST_ASSERT_EQUAL_STRING("Active \xc2\xb7 3 highlights", c.line3);
}

void test_today_card_detail_no_birthday(void) {
    card_content_t c = pc_today_card("Good morning", "headline",
                                     "Active", 3, 10, 0);
    TEST_ASSERT_EQUAL_STRING("", c.detail);
}

void test_today_card_detail_with_birthday(void) {
    card_content_t c = pc_today_card("Good morning", "headline",
                                     "Active", 3, 10, 1);
    TEST_ASSERT_EQUAL_STRING("Birthday in 10 days", c.detail);
}

void test_today_card_birthday_1_day(void) {
    card_content_t c = pc_today_card("Hi", "headline", "Calm", 1, 1, 1);
    TEST_ASSERT_EQUAL_STRING("Birthday in 1 days", c.detail);
}

void test_today_card_zero_highlights(void) {
    card_content_t c = pc_today_card("Hello", "headline", "Quiet", 0, 5, 0);
    TEST_ASSERT_EQUAL_STRING("Quiet \xc2\xb7 0 highlights", c.line3);
}

void test_today_card_null_greeting(void) {
    card_content_t c = pc_today_card(NULL, "headline", "Active", 3, 10, 0);
    TEST_ASSERT_EQUAL_STRING("", c.line1);
}

void test_today_card_null_headline(void) {
    card_content_t c = pc_today_card("Hi", NULL, "Active", 3, 10, 0);
    TEST_ASSERT_EQUAL_STRING("", c.line2);
}

void test_today_card_null_energy(void) {
    card_content_t c = pc_today_card("Hi", "headline", NULL, 3, 10, 0);
    /* Should produce " · 3 highlights" with empty energy name */
    TEST_ASSERT_NOT_NULL(strstr(c.line3, "3 highlights"));
}

void test_today_card_empty_on_zero_highlights_false(void) {
    /* highlight_count=0 is valid; card is NOT empty */
    card_content_t c = pc_today_card("Hello", "headline", "Quiet", 0, 5, 0);
    TEST_ASSERT_EQUAL_STRING("Today for You", c.title);
}

/* ===== pc_fingerprint_card ===== */

void test_fingerprint_card_title_system(void) {
    card_content_t c = pc_fingerprint_card("Tzolkin", "Yellow Rhythmic Warrior",
                                            "Kin 196, Wavespell 15", 5);
    TEST_ASSERT_EQUAL_STRING("Tzolkin", c.title);
}

void test_fingerprint_card_line1_headline(void) {
    card_content_t c = pc_fingerprint_card("Tzolkin", "Yellow Rhythmic Warrior",
                                            "Kin 196, Wavespell 15", 5);
    TEST_ASSERT_EQUAL_STRING("Yellow Rhythmic Warrior", c.line1);
}

void test_fingerprint_card_line2_subtitle(void) {
    card_content_t c = pc_fingerprint_card("Tzolkin", "Yellow Rhythmic Warrior",
                                            "Kin 196, Wavespell 15", 5);
    TEST_ASSERT_EQUAL_STRING("Kin 196, Wavespell 15", c.line2);
}

void test_fingerprint_card_line3_reserved(void) {
    card_content_t c = pc_fingerprint_card("Tzolkin", "Yellow Rhythmic Warrior",
                                            "Kin 196, Wavespell 15", 5);
    TEST_ASSERT_EQUAL_STRING("", c.line3);
}

void test_fingerprint_card_detail_with_glyph(void) {
    card_content_t c = pc_fingerprint_card("Tzolkin", "headline", "sub", 5);
    TEST_ASSERT_EQUAL_STRING("Glyph 5", c.detail);
}

void test_fingerprint_card_detail_negative_glyph(void) {
    card_content_t c = pc_fingerprint_card("Astrology", "Pisces Sun", "sub", -1);
    TEST_ASSERT_EQUAL_STRING("", c.detail);
}

void test_fingerprint_card_null_system(void) {
    card_content_t c = pc_fingerprint_card(NULL, "headline", "sub", 3);
    TEST_ASSERT_EQUAL_STRING("", c.title);
}

void test_fingerprint_card_null_headline(void) {
    card_content_t c = pc_fingerprint_card("Tzolkin", NULL, "sub", 3);
    TEST_ASSERT_EQUAL_STRING("", c.line1);
}

void test_fingerprint_card_null_subtitle(void) {
    card_content_t c = pc_fingerprint_card("Tzolkin", "headline", NULL, 3);
    TEST_ASSERT_EQUAL_STRING("", c.line2);
}

/* ===== pc_author_card ===== */

void test_author_card_title_name(void) {
    card_content_t c = pc_author_card("Gabriel", "Creator of Time",
                                       "Statement here", "C + WebGL2", 179);
    TEST_ASSERT_EQUAL_STRING("Gabriel", c.title);
}

void test_author_card_line1_title(void) {
    card_content_t c = pc_author_card("Gabriel", "Creator of Time",
                                       "Statement", "C + WebGL2", 179);
    TEST_ASSERT_EQUAL_STRING("Creator of Time", c.line1);
}

void test_author_card_line2_tech(void) {
    card_content_t c = pc_author_card("Gabriel", "Creator of Time",
                                       "Statement", "C + WebGL2", 179);
    TEST_ASSERT_EQUAL_STRING("C + WebGL2", c.line2);
}

void test_author_card_line3_contributors(void) {
    card_content_t c = pc_author_card("Gabriel", "Creator of Time",
                                       "Statement", "C + WebGL2", 179);
    TEST_ASSERT_EQUAL_STRING("Built with 179 contributors", c.line3);
}

void test_author_card_line3_one_contributor(void) {
    card_content_t c = pc_author_card("Gabriel", "title", "s", "t", 1);
    TEST_ASSERT_EQUAL_STRING("Built with 1 contributors", c.line3);
}

void test_author_card_detail_statement(void) {
    card_content_t c = pc_author_card("Gabriel", "Creator of Time",
                                       "My statement", "tech", 100);
    TEST_ASSERT_EQUAL_STRING("My statement", c.detail);
}

void test_author_card_detail_truncation(void) {
    /* detail buffer is 256 chars; statement over 255 must truncate */
    char long_stmt[512];
    memset(long_stmt, 'A', 511);
    long_stmt[511] = '\0';
    card_content_t c = pc_author_card("Gabriel", "title", long_stmt, "tech", 100);
    TEST_ASSERT_TRUE(strlen(c.detail) <= 255);
    TEST_ASSERT_EQUAL_UINT8('A', (unsigned char)c.detail[0]);
}

void test_author_card_null_name(void) {
    card_content_t c = pc_author_card(NULL, "title", "s", "t", 10);
    TEST_ASSERT_EQUAL_STRING("", c.title);
}

void test_author_card_null_statement(void) {
    card_content_t c = pc_author_card("Gabriel", "title", NULL, "tech", 10);
    TEST_ASSERT_EQUAL_STRING("", c.detail);
}

/* ===== pc_cycle_card ===== */

void test_cycle_card_title(void) {
    card_content_t c = pc_cycle_card("Wavespell", "Red Dragon", 10, 0.73);
    TEST_ASSERT_EQUAL_STRING("Cycle Status", c.title);
}

void test_cycle_card_line1_name(void) {
    card_content_t c = pc_cycle_card("Wavespell", "Red Dragon", 10, 0.73);
    TEST_ASSERT_EQUAL_STRING("Wavespell", c.line1);
}

void test_cycle_card_line2_value(void) {
    card_content_t c = pc_cycle_card("Wavespell", "Red Dragon", 10, 0.73);
    TEST_ASSERT_EQUAL_STRING("Red Dragon", c.line2);
}

void test_cycle_card_line3_days(void) {
    card_content_t c = pc_cycle_card("Wavespell", "Red Dragon", 10, 0.73);
    TEST_ASSERT_EQUAL_STRING("10 days remaining", c.line3);
}

void test_cycle_card_line3_zero_days(void) {
    card_content_t c = pc_cycle_card("Wavespell", "Red Dragon", 0, 1.0);
    TEST_ASSERT_EQUAL_STRING("0 days remaining", c.line3);
}

void test_cycle_card_detail_progress(void) {
    card_content_t c = pc_cycle_card("Wavespell", "Red Dragon", 10, 0.73);
    TEST_ASSERT_EQUAL_STRING("Progress: 73%", c.detail);
}

void test_cycle_card_detail_progress_zero(void) {
    card_content_t c = pc_cycle_card("Moon", "New", 28, 0.0);
    TEST_ASSERT_EQUAL_STRING("Progress: 0%", c.detail);
}

void test_cycle_card_detail_progress_full(void) {
    card_content_t c = pc_cycle_card("Moon", "Full", 0, 1.0);
    TEST_ASSERT_EQUAL_STRING("Progress: 100%", c.detail);
}

void test_cycle_card_null_name(void) {
    card_content_t c = pc_cycle_card(NULL, "Red Dragon", 10, 0.5);
    TEST_ASSERT_EQUAL_STRING("", c.line1);
}

/* ===== pc_notification_card ===== */

void test_notification_card_title_category(void) {
    card_content_t c = pc_notification_card("Birthday", "Birthday Approaching",
                                             "Your birthday is in 3 days",
                                             "Medium", 3);
    TEST_ASSERT_EQUAL_STRING("Birthday", c.title);
}

void test_notification_card_line1_title(void) {
    card_content_t c = pc_notification_card("Birthday", "Birthday Approaching",
                                             "body", "Medium", 3);
    TEST_ASSERT_EQUAL_STRING("Birthday Approaching", c.line1);
}

void test_notification_card_line2_body(void) {
    card_content_t c = pc_notification_card("Birthday", "title",
                                             "Your birthday is in 3 days",
                                             "Medium", 3);
    TEST_ASSERT_EQUAL_STRING("Your birthday is in 3 days", c.line2);
}

void test_notification_card_line3_urgency_days(void) {
    card_content_t c = pc_notification_card("Birthday", "title", "body",
                                             "Medium", 3);
    TEST_ASSERT_EQUAL_STRING("Medium \xc2\xb7 in 3 days", c.line3);
}

void test_notification_card_detail_empty(void) {
    card_content_t c = pc_notification_card("Birthday", "title", "body",
                                             "High", 1);
    TEST_ASSERT_EQUAL_STRING("", c.detail);
}

void test_notification_card_null_category(void) {
    card_content_t c = pc_notification_card(NULL, "title", "body", "High", 1);
    TEST_ASSERT_EQUAL_STRING("", c.title);
}

void test_notification_card_null_urgency(void) {
    card_content_t c = pc_notification_card("Birthday", "title", "body", NULL, 3);
    TEST_ASSERT_NOT_NULL(strstr(c.line3, "in 3 days"));
}

/* ===== pc_snapshot_card ===== */

void test_snapshot_card_title(void) {
    card_content_t c = pc_snapshot_card(2026, 3, 20, "Kin 196 \xc2\xb7 Pisces",
                                         "A special day", 5);
    TEST_ASSERT_EQUAL_STRING("Bookmarked Moment", c.title);
}

void test_snapshot_card_line1_date(void) {
    card_content_t c = pc_snapshot_card(2026, 3, 20, "summary", "note", 5);
    TEST_ASSERT_EQUAL_STRING("2026-03-20", c.line1);
}

void test_snapshot_card_line1_date_padding(void) {
    card_content_t c = pc_snapshot_card(2026, 1, 5, "summary", "note", 3);
    TEST_ASSERT_EQUAL_STRING("2026-01-05", c.line1);
}

void test_snapshot_card_line2_summary(void) {
    card_content_t c = pc_snapshot_card(2026, 3, 20,
                                         "Kin 196 \xc2\xb7 Pisces \xc2\xb7 Water Rabbit",
                                         "note", 5);
    TEST_ASSERT_EQUAL_STRING("Kin 196 \xc2\xb7 Pisces \xc2\xb7 Water Rabbit", c.line2);
}

void test_snapshot_card_line3_systems(void) {
    card_content_t c = pc_snapshot_card(2026, 3, 20, "summary", "note", 5);
    TEST_ASSERT_EQUAL_STRING("5 systems captured", c.line3);
}

void test_snapshot_card_detail_note(void) {
    card_content_t c = pc_snapshot_card(2026, 3, 20, "summary", "A special day", 5);
    TEST_ASSERT_EQUAL_STRING("A special day", c.detail);
}

void test_snapshot_card_null_note(void) {
    card_content_t c = pc_snapshot_card(2026, 3, 20, "summary", NULL, 5);
    TEST_ASSERT_EQUAL_STRING("", c.detail);
}

void test_snapshot_card_empty_note(void) {
    card_content_t c = pc_snapshot_card(2026, 3, 20, "summary", "", 5);
    TEST_ASSERT_EQUAL_STRING("", c.detail);
}

void test_snapshot_card_null_summary(void) {
    card_content_t c = pc_snapshot_card(2026, 3, 20, NULL, "note", 5);
    TEST_ASSERT_EQUAL_STRING("", c.line2);
}

/* ===== pc_birth_entry_card ===== */

void test_birth_entry_card_title(void) {
    card_content_t c = pc_birth_entry_card("Date", 2000, 1, 15, 1, 0, 0);
    TEST_ASSERT_EQUAL_STRING("Birth Data", c.title);
}

void test_birth_entry_card_line1_step(void) {
    card_content_t c = pc_birth_entry_card("Date", 2000, 1, 15, 1, 0, 0);
    TEST_ASSERT_EQUAL_STRING("Date", c.line1);
}

void test_birth_entry_card_line2_valid_date(void) {
    card_content_t c = pc_birth_entry_card("Date", 2000, 1, 15, 1, 0, 0);
    TEST_ASSERT_EQUAL_STRING("2000-01-15", c.line2);
}

void test_birth_entry_card_line2_invalid_date(void) {
    card_content_t c = pc_birth_entry_card("Date", 0, 0, 0, 0, 0, 0);
    TEST_ASSERT_EQUAL_STRING("Enter your birth date", c.line2);
}

void test_birth_entry_card_line3_location_set(void) {
    card_content_t c = pc_birth_entry_card("Location", 2000, 1, 15, 1, 1, 0);
    TEST_ASSERT_EQUAL_STRING("Location set", c.line3);
}

void test_birth_entry_card_line3_location_skipped(void) {
    card_content_t c = pc_birth_entry_card("Location", 2000, 1, 15, 1, 0, 1);
    TEST_ASSERT_EQUAL_STRING("Location skipped", c.line3);
}

void test_birth_entry_card_line3_enter_location(void) {
    card_content_t c = pc_birth_entry_card("Location", 2000, 1, 15, 1, 0, 0);
    TEST_ASSERT_EQUAL_STRING("Enter location", c.line3);
}

void test_birth_entry_card_detail_empty(void) {
    card_content_t c = pc_birth_entry_card("Ready", 2000, 1, 15, 1, 1, 0);
    TEST_ASSERT_EQUAL_STRING("", c.detail);
}

void test_birth_entry_card_null_step(void) {
    card_content_t c = pc_birth_entry_card(NULL, 2000, 1, 15, 1, 0, 0);
    TEST_ASSERT_EQUAL_STRING("", c.line1);
}

/* ===== Truncation safety ===== */

void test_today_card_long_greeting_truncated(void) {
    char long_str[256];
    memset(long_str, 'X', 255);
    long_str[255] = '\0';
    card_content_t c = pc_today_card(long_str, "headline", "Active", 1, 0, 0);
    TEST_ASSERT_TRUE(strlen(c.line1) <= 127);
}

void test_fingerprint_card_long_headline_truncated(void) {
    char long_str[256];
    memset(long_str, 'Y', 255);
    long_str[255] = '\0';
    card_content_t c = pc_fingerprint_card("System", long_str, "sub", 1);
    TEST_ASSERT_TRUE(strlen(c.line1) <= 127);
}

void test_snapshot_card_long_summary_truncated(void) {
    char long_str[256];
    memset(long_str, 'Z', 255);
    long_str[255] = '\0';
    card_content_t c = pc_snapshot_card(2026, 3, 20, long_str, "note", 5);
    TEST_ASSERT_TRUE(strlen(c.line2) <= 127);
}

void test_notification_card_long_body_truncated(void) {
    char long_str[256];
    memset(long_str, 'W', 255);
    long_str[255] = '\0';
    card_content_t c = pc_notification_card("Cat", "title", long_str, "High", 1);
    TEST_ASSERT_TRUE(strlen(c.line2) <= 127);
}

void test_author_card_long_title_truncated(void) {
    char long_str[256];
    memset(long_str, 'T', 255);
    long_str[255] = '\0';
    card_content_t c = pc_author_card("Gabriel", long_str, "stmt", "tech", 10);
    TEST_ASSERT_TRUE(strlen(c.line1) <= 127);
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* pc_empty_card */
    RUN_TEST(test_empty_card_title_empty);
    RUN_TEST(test_empty_card_line1_empty);
    RUN_TEST(test_empty_card_line2_empty);
    RUN_TEST(test_empty_card_line3_empty);
    RUN_TEST(test_empty_card_detail_empty);

    /* pc_today_card */
    RUN_TEST(test_today_card_title);
    RUN_TEST(test_today_card_line1_greeting);
    RUN_TEST(test_today_card_line2_headline);
    RUN_TEST(test_today_card_line3_energy_highlights);
    RUN_TEST(test_today_card_detail_no_birthday);
    RUN_TEST(test_today_card_detail_with_birthday);
    RUN_TEST(test_today_card_birthday_1_day);
    RUN_TEST(test_today_card_zero_highlights);
    RUN_TEST(test_today_card_null_greeting);
    RUN_TEST(test_today_card_null_headline);
    RUN_TEST(test_today_card_null_energy);
    RUN_TEST(test_today_card_empty_on_zero_highlights_false);

    /* pc_fingerprint_card */
    RUN_TEST(test_fingerprint_card_title_system);
    RUN_TEST(test_fingerprint_card_line1_headline);
    RUN_TEST(test_fingerprint_card_line2_subtitle);
    RUN_TEST(test_fingerprint_card_line3_reserved);
    RUN_TEST(test_fingerprint_card_detail_with_glyph);
    RUN_TEST(test_fingerprint_card_detail_negative_glyph);
    RUN_TEST(test_fingerprint_card_null_system);
    RUN_TEST(test_fingerprint_card_null_headline);
    RUN_TEST(test_fingerprint_card_null_subtitle);

    /* pc_author_card */
    RUN_TEST(test_author_card_title_name);
    RUN_TEST(test_author_card_line1_title);
    RUN_TEST(test_author_card_line2_tech);
    RUN_TEST(test_author_card_line3_contributors);
    RUN_TEST(test_author_card_line3_one_contributor);
    RUN_TEST(test_author_card_detail_statement);
    RUN_TEST(test_author_card_detail_truncation);
    RUN_TEST(test_author_card_null_name);
    RUN_TEST(test_author_card_null_statement);

    /* pc_cycle_card */
    RUN_TEST(test_cycle_card_title);
    RUN_TEST(test_cycle_card_line1_name);
    RUN_TEST(test_cycle_card_line2_value);
    RUN_TEST(test_cycle_card_line3_days);
    RUN_TEST(test_cycle_card_line3_zero_days);
    RUN_TEST(test_cycle_card_detail_progress);
    RUN_TEST(test_cycle_card_detail_progress_zero);
    RUN_TEST(test_cycle_card_detail_progress_full);
    RUN_TEST(test_cycle_card_null_name);

    /* pc_notification_card */
    RUN_TEST(test_notification_card_title_category);
    RUN_TEST(test_notification_card_line1_title);
    RUN_TEST(test_notification_card_line2_body);
    RUN_TEST(test_notification_card_line3_urgency_days);
    RUN_TEST(test_notification_card_detail_empty);
    RUN_TEST(test_notification_card_null_category);
    RUN_TEST(test_notification_card_null_urgency);

    /* pc_snapshot_card */
    RUN_TEST(test_snapshot_card_title);
    RUN_TEST(test_snapshot_card_line1_date);
    RUN_TEST(test_snapshot_card_line1_date_padding);
    RUN_TEST(test_snapshot_card_line2_summary);
    RUN_TEST(test_snapshot_card_line3_systems);
    RUN_TEST(test_snapshot_card_detail_note);
    RUN_TEST(test_snapshot_card_null_note);
    RUN_TEST(test_snapshot_card_empty_note);
    RUN_TEST(test_snapshot_card_null_summary);

    /* pc_birth_entry_card */
    RUN_TEST(test_birth_entry_card_title);
    RUN_TEST(test_birth_entry_card_line1_step);
    RUN_TEST(test_birth_entry_card_line2_valid_date);
    RUN_TEST(test_birth_entry_card_line2_invalid_date);
    RUN_TEST(test_birth_entry_card_line3_location_set);
    RUN_TEST(test_birth_entry_card_line3_location_skipped);
    RUN_TEST(test_birth_entry_card_line3_enter_location);
    RUN_TEST(test_birth_entry_card_detail_empty);
    RUN_TEST(test_birth_entry_card_null_step);

    /* Truncation safety */
    RUN_TEST(test_today_card_long_greeting_truncated);
    RUN_TEST(test_fingerprint_card_long_headline_truncated);
    RUN_TEST(test_snapshot_card_long_summary_truncated);
    RUN_TEST(test_notification_card_long_body_truncated);
    RUN_TEST(test_author_card_long_title_truncated);

    return UNITY_END();
}
