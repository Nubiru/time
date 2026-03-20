/* test_author_card.c — Tests for Author Card data module.
 * TDD: RED phase first, then GREEN with implementation. */

#include "unity.h"
#include "systems/earth/author_card.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== ac_build — basic fields ===== */

void test_build_name(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_EQUAL_STRING("Gabriel", card.name);
}

void test_build_title(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_EQUAL_STRING("Creator of Time", card.title);
}

void test_build_statement_not_empty(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_TRUE(strlen(card.statement) > 0);
}

void test_build_statement_content(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_NOT_NULL(strstr(card.statement, "artwork"));
}

void test_build_tech_line(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_NOT_NULL(strstr(card.tech_line, "WebGL2"));
    TEST_ASSERT_NOT_NULL(strstr(card.tech_line, "WebAssembly"));
}

void test_build_philosophy_not_empty(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_TRUE(strlen(card.philosophy) > 0);
}

void test_build_philosophy_content(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_NOT_NULL(strstr(card.philosophy, "No frameworks"));
}

void test_build_contributor_count(void) {
    ac_card_t card = ac_build(179);
    TEST_ASSERT_EQUAL_INT(179, card.contributor_count);
}

void test_build_contributor_count_zero(void) {
    ac_card_t card = ac_build(0);
    TEST_ASSERT_EQUAL_INT(0, card.contributor_count);
}

void test_build_credit_count(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_EQUAL_INT(5, card.credit_count);
}

/* ===== ac_build — credits ===== */

void test_credit_0_claude(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_EQUAL_STRING("Claude", card.credits[0].name);
    TEST_ASSERT_EQUAL_STRING("AI Collaborator", card.credits[0].role);
}

void test_credit_1_meeus(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_EQUAL_STRING("Jean Meeus", card.credits[1].name);
    TEST_ASSERT_EQUAL_STRING("Astronomical Algorithms", card.credits[1].role);
}

void test_credit_2_arguelles(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_EQUAL_STRING("José Argüelles", card.credits[2].name);
    TEST_ASSERT_EQUAL_STRING("Dreamspell / 13:20 Frequency", card.credits[2].role);
}

void test_credit_3_wilhelm(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_EQUAL_STRING("Richard Wilhelm", card.credits[3].name);
    TEST_ASSERT_EQUAL_STRING("I Ching Translation", card.credits[3].role);
}

void test_credit_4_reingold_dershowitz(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_EQUAL_STRING("Edward Reingold & Nachum Dershowitz",
                             card.credits[4].name);
    TEST_ASSERT_EQUAL_STRING("Calendrical Calculations", card.credits[4].role);
}

/* ===== Accessor functions ===== */

void test_ac_name_returns_name(void) {
    ac_card_t card = ac_build(100);
    const char *n = ac_name(&card);
    TEST_ASSERT_NOT_NULL(n);
    TEST_ASSERT_EQUAL_STRING("Gabriel", n);
}

void test_ac_title_returns_title(void) {
    ac_card_t card = ac_build(100);
    const char *t = ac_title(&card);
    TEST_ASSERT_NOT_NULL(t);
    TEST_ASSERT_EQUAL_STRING("Creator of Time", t);
}

void test_ac_statement_returns_statement(void) {
    ac_card_t card = ac_build(100);
    const char *s = ac_statement(&card);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(strlen(s) > 0);
}

void test_ac_tech_returns_tech(void) {
    ac_card_t card = ac_build(100);
    const char *t = ac_tech(&card);
    TEST_ASSERT_NOT_NULL(t);
    TEST_ASSERT_NOT_NULL(strstr(t, "Every pixel earned"));
}

void test_ac_philosophy_returns_philosophy(void) {
    ac_card_t card = ac_build(100);
    const char *p = ac_philosophy(&card);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_TRUE(strlen(p) > 0);
}

/* ===== ac_credit — valid and invalid indices ===== */

void test_ac_credit_valid_index_0(void) {
    ac_card_t card = ac_build(100);
    const ac_credit_t *c = ac_credit(&card, 0);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_STRING("Claude", c->name);
}

void test_ac_credit_valid_index_4(void) {
    ac_card_t card = ac_build(100);
    const ac_credit_t *c = ac_credit(&card, 4);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_EQUAL_STRING("Calendrical Calculations", c->role);
}

void test_ac_credit_negative_index(void) {
    ac_card_t card = ac_build(100);
    const ac_credit_t *c = ac_credit(&card, -1);
    TEST_ASSERT_NULL(c);
}

void test_ac_credit_out_of_range(void) {
    ac_card_t card = ac_build(100);
    const ac_credit_t *c = ac_credit(&card, 5);
    TEST_ASSERT_NULL(c);
}

void test_ac_credit_far_out_of_range(void) {
    ac_card_t card = ac_build(100);
    const ac_credit_t *c = ac_credit(&card, 999);
    TEST_ASSERT_NULL(c);
}

/* ===== ac_credit_count ===== */

void test_ac_credit_count_returns_5(void) {
    ac_card_t card = ac_build(100);
    TEST_ASSERT_EQUAL_INT(5, ac_credit_count(&card));
}

/* ===== ac_format_contributors ===== */

void test_format_contributors_normal(void) {
    ac_card_t card = ac_build(179);
    char buf[256];
    int len = ac_format_contributors(&card, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_EQUAL_STRING(
        "Built with contributions from 179 humans", buf);
}

void test_format_contributors_zero(void) {
    ac_card_t card = ac_build(0);
    char buf[256];
    int len = ac_format_contributors(&card, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_EQUAL_STRING(
        "Built with contributions from 0 humans", buf);
}

void test_format_contributors_one(void) {
    ac_card_t card = ac_build(1);
    char buf[256];
    int len = ac_format_contributors(&card, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_EQUAL_STRING(
        "Built with contributions from 1 humans", buf);
}

void test_format_contributors_small_buffer(void) {
    ac_card_t card = ac_build(179);
    char buf[10];
    int len = ac_format_contributors(&card, buf, (int)sizeof(buf));
    /* Should truncate, not overflow */
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len <= 9);
    TEST_ASSERT_TRUE(strlen(buf) < 10);
}

void test_format_contributors_zero_buffer(void) {
    ac_card_t card = ac_build(100);
    char buf[1] = {'X'};
    int len = ac_format_contributors(&card, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* ===== ac_format_full ===== */

void test_format_full_not_empty(void) {
    ac_card_t card = ac_build(179);
    char buf[2048];
    int len = ac_format_full(&card, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_full_contains_name(void) {
    ac_card_t card = ac_build(100);
    char buf[2048];
    ac_format_full(&card, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Gabriel"));
}

void test_format_full_contains_title(void) {
    ac_card_t card = ac_build(100);
    char buf[2048];
    ac_format_full(&card, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Creator of Time"));
}

void test_format_full_contains_tech(void) {
    ac_card_t card = ac_build(100);
    char buf[2048];
    ac_format_full(&card, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Every pixel earned"));
}

void test_format_full_contains_contributor_line(void) {
    ac_card_t card = ac_build(42);
    char buf[2048];
    ac_format_full(&card, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "42 humans"));
}

void test_format_full_small_buffer(void) {
    ac_card_t card = ac_build(100);
    char buf[20];
    int len = ac_format_full(&card, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len <= 19);
    TEST_ASSERT_TRUE(strlen(buf) < 20);
}

void test_format_full_zero_buffer(void) {
    ac_card_t card = ac_build(100);
    char buf[1] = {'X'};
    int len = ac_format_full(&card, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* ac_build — basic fields */
    RUN_TEST(test_build_name);
    RUN_TEST(test_build_title);
    RUN_TEST(test_build_statement_not_empty);
    RUN_TEST(test_build_statement_content);
    RUN_TEST(test_build_tech_line);
    RUN_TEST(test_build_philosophy_not_empty);
    RUN_TEST(test_build_philosophy_content);
    RUN_TEST(test_build_contributor_count);
    RUN_TEST(test_build_contributor_count_zero);
    RUN_TEST(test_build_credit_count);

    /* ac_build — credits */
    RUN_TEST(test_credit_0_claude);
    RUN_TEST(test_credit_1_meeus);
    RUN_TEST(test_credit_2_arguelles);
    RUN_TEST(test_credit_3_wilhelm);
    RUN_TEST(test_credit_4_reingold_dershowitz);

    /* Accessor functions */
    RUN_TEST(test_ac_name_returns_name);
    RUN_TEST(test_ac_title_returns_title);
    RUN_TEST(test_ac_statement_returns_statement);
    RUN_TEST(test_ac_tech_returns_tech);
    RUN_TEST(test_ac_philosophy_returns_philosophy);

    /* ac_credit — bounds */
    RUN_TEST(test_ac_credit_valid_index_0);
    RUN_TEST(test_ac_credit_valid_index_4);
    RUN_TEST(test_ac_credit_negative_index);
    RUN_TEST(test_ac_credit_out_of_range);
    RUN_TEST(test_ac_credit_far_out_of_range);

    /* ac_credit_count */
    RUN_TEST(test_ac_credit_count_returns_5);

    /* ac_format_contributors */
    RUN_TEST(test_format_contributors_normal);
    RUN_TEST(test_format_contributors_zero);
    RUN_TEST(test_format_contributors_one);
    RUN_TEST(test_format_contributors_small_buffer);
    RUN_TEST(test_format_contributors_zero_buffer);

    /* ac_format_full */
    RUN_TEST(test_format_full_not_empty);
    RUN_TEST(test_format_full_contains_name);
    RUN_TEST(test_format_full_contains_title);
    RUN_TEST(test_format_full_contains_tech);
    RUN_TEST(test_format_full_contains_contributor_line);
    RUN_TEST(test_format_full_small_buffer);
    RUN_TEST(test_format_full_zero_buffer);

    return UNITY_END();
}
