/* test_daily_narrative.c — Tests for the Daily Narrative module.
 * Validates dn_compose, dn_default_input, dn_headline_type across
 * all narrative composition paths: date-based, significance-based,
 * convergence-based headlines, personal resonance detection, and
 * wisdom quote formatting. */

#include "unity.h"
#include "systems/unified/daily_narrative.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* === Helper: build a minimal input with just a date === */
static dn_input_t make_date_input(void)
{
    dn_input_t in = dn_default_input();
    in.gregorian_year = 2026;
    in.gregorian_month = 3;
    in.gregorian_day = 19;
    in.weekday = "Thursday";
    return in;
}

/* === dn_default_input tests === */

static void test_default_input_zeroed(void)
{
    dn_input_t in = dn_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.gregorian_year);
    TEST_ASSERT_EQUAL_INT(0, in.gregorian_month);
    TEST_ASSERT_EQUAL_INT(0, in.gregorian_day);
    TEST_ASSERT_NULL(in.weekday);
    TEST_ASSERT_EQUAL_INT(0, in.insight_count);
    TEST_ASSERT_EQUAL_INT(0, in.convergence_score);
    TEST_ASSERT_NULL(in.convergence_desc);
    TEST_ASSERT_EQUAL_INT(0, in.has_birth);
    TEST_ASSERT_EQUAL_INT(0, in.birth_kin);
    TEST_ASSERT_NULL(in.birth_seal);
    TEST_ASSERT_EQUAL_INT(0, in.birth_tone);
    TEST_ASSERT_EQUAL_INT(0, in.birth_hexagram);
    TEST_ASSERT_EQUAL_INT(0, in.birth_hebrew_month);
    TEST_ASSERT_EQUAL_INT(0, in.today_kin);
    TEST_ASSERT_EQUAL_INT(0, in.today_seal);
    TEST_ASSERT_EQUAL_INT(0, in.today_tone);
    TEST_ASSERT_EQUAL_INT(0, in.today_hexagram);
    TEST_ASSERT_EQUAL_INT(0, in.today_hebrew_month);
    TEST_ASSERT_NULL(in.wisdom_quote);
    TEST_ASSERT_NULL(in.wisdom_author);
}

/* === dn_compose with NULL input === */

static void test_compose_null_input(void)
{
    daily_narrative_t n = dn_compose(NULL);
    TEST_ASSERT_EQUAL_STRING("", n.headline);
    TEST_ASSERT_EQUAL_STRING("", n.personal);
    TEST_ASSERT_EQUAL_STRING("", n.systems);
    TEST_ASSERT_EQUAL_STRING("", n.wisdom);
    TEST_ASSERT_EQUAL_INT(0, n.insight_count);
    TEST_ASSERT_EQUAL_INT(0, n.resonance_count);
    TEST_ASSERT_EQUAL_INT(0, n.convergence_score);
}

/* === Date-based headline tests === */

static void test_compose_date_only_no_insights(void)
{
    dn_input_t in = make_date_input();
    daily_narrative_t n = dn_compose(&in);

    /* Headline should be date-based: "Thursday, March 19 — 0 systems speak today" */
    TEST_ASSERT_NOT_EQUAL(0, strlen(n.headline));
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "Thursday"));
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "0 systems speak today"));
}

static void test_compose_with_weekday_in_headline(void)
{
    dn_input_t in = make_date_input();
    in.weekday = "Monday";
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "Monday"));
}

static void test_compose_no_weekday_uses_date(void)
{
    dn_input_t in = make_date_input();
    in.weekday = NULL;
    daily_narrative_t n = dn_compose(&in);
    /* Should still produce a headline with the date */
    TEST_ASSERT_NOT_EQUAL(0, strlen(n.headline));
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "March 19"));
}

/* === Insight tests === */

static void test_compose_one_insight(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = 1;
    in.insights[0].system_id = 4; /* TS_SYS_HEBREW */
    in.insights[0].system_name = "Hebrew";
    snprintf(in.insights[0].summary, sizeof(in.insights[0].summary),
             "5786 Tevet 15");
    in.insights[0].significance = 1;

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_INT(1, n.insight_count);
    TEST_ASSERT_NOT_NULL(strstr(n.systems, "Hebrew"));
    TEST_ASSERT_NOT_NULL(strstr(n.systems, "5786 Tevet 15"));
}

static void test_compose_three_insights(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = 3;

    in.insights[0].system_id = 0;
    in.insights[0].system_name = "Gregorian";
    snprintf(in.insights[0].summary, sizeof(in.insights[0].summary),
             "Spring Equinox");
    in.insights[0].significance = 1;

    in.insights[1].system_id = 1;
    in.insights[1].system_name = "Tzolkin";
    snprintf(in.insights[1].summary, sizeof(in.insights[1].summary),
             "Kin 207 Blue Crystal Hand");
    in.insights[1].significance = 1;

    in.insights[2].system_id = 4;
    in.insights[2].system_name = "Hebrew";
    snprintf(in.insights[2].summary, sizeof(in.insights[2].summary),
             "5786 Adar 19");
    in.insights[2].significance = 0;

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_INT(3, n.insight_count);
    TEST_ASSERT_NOT_NULL(strstr(n.systems, "Gregorian"));
    TEST_ASSERT_NOT_NULL(strstr(n.systems, "Tzolkin"));
    TEST_ASSERT_NOT_NULL(strstr(n.systems, "Hebrew"));
}

static void test_compose_insight_count_matches(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = 5;
    for (int i = 0; i < 5; i++) {
        in.insights[i].system_id = i;
        in.insights[i].system_name = "System";
        snprintf(in.insights[i].summary, sizeof(in.insights[i].summary),
                 "Entry %d", i);
        in.insights[i].significance = 0;
    }
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_INT(5, n.insight_count);
}

/* === Significance-based headline tests === */

static void test_compose_high_significance_headline(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = 2;
    in.insights[0].system_id = 0;
    in.insights[0].system_name = "Gregorian";
    snprintf(in.insights[0].summary, sizeof(in.insights[0].summary),
             "Regular day");
    in.insights[0].significance = 0;

    in.insights[1].system_id = 1;
    in.insights[1].system_name = "Tzolkin";
    snprintf(in.insights[1].summary, sizeof(in.insights[1].summary),
             "Galactic Activation Portal");
    in.insights[1].significance = 2;

    daily_narrative_t n = dn_compose(&in);
    /* Should use the significant insight as headline */
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "Galactic Activation Portal"));
}

static void test_compose_significance_threshold(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = 1;
    in.insights[0].system_id = 0;
    in.insights[0].system_name = "Gregorian";
    snprintf(in.insights[0].summary, sizeof(in.insights[0].summary),
             "Just a normal day");
    in.insights[0].significance = 1;  /* below threshold of 2 */

    daily_narrative_t n = dn_compose(&in);
    /* significance 1 is NOT >= 2, so date-based headline */
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "1 systems speak today"));
}

/* === Convergence-based headline tests === */

static void test_compose_convergence_headline(void)
{
    dn_input_t in = make_date_input();
    in.convergence_score = 5;
    in.convergence_desc = "3 lunar traditions align";

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "3 lunar traditions align"));
}

static void test_compose_convergence_beats_significance(void)
{
    dn_input_t in = make_date_input();
    in.convergence_score = 4;
    in.convergence_desc = "Convergence event";
    in.insight_count = 1;
    in.insights[0].system_id = 0;
    in.insights[0].system_name = "Tzolkin";
    snprintf(in.insights[0].summary, sizeof(in.insights[0].summary),
             "Major portal");
    in.insights[0].significance = 3;

    daily_narrative_t n = dn_compose(&in);
    /* Convergence takes priority */
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "Convergence event"));
}

static void test_compose_convergence_needs_desc(void)
{
    dn_input_t in = make_date_input();
    in.convergence_score = 5;
    in.convergence_desc = NULL;  /* no description */
    in.insight_count = 1;
    in.insights[0].system_id = 0;
    in.insights[0].system_name = "Tzolkin";
    snprintf(in.insights[0].summary, sizeof(in.insights[0].summary),
             "Important day");
    in.insights[0].significance = 2;

    daily_narrative_t n = dn_compose(&in);
    /* convergence_desc is NULL, so falls to significance */
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "Important day"));
}

static void test_compose_convergence_needs_score(void)
{
    dn_input_t in = make_date_input();
    in.convergence_score = 2;  /* below threshold of 3 */
    in.convergence_desc = "Some alignment";

    daily_narrative_t n = dn_compose(&in);
    /* Score < 3, so not convergence headline */
    TEST_ASSERT_NULL(strstr(n.headline, "Some alignment"));
}

static void test_convergence_score_echoed(void)
{
    dn_input_t in = make_date_input();
    in.convergence_score = 7;
    in.convergence_desc = "Major event";
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_INT(7, n.convergence_score);
}

/* === Personal section: birth resonance tests === */

static void test_compose_birth_seal_match(void)
{
    dn_input_t in = make_date_input();
    in.has_birth = 1;
    in.birth_seal = "Hand";
    in.today_seal = 6;  /* Hand = seal index 6 in Dreamspell 0-19 */
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_NOT_NULL(strstr(n.personal, "birth seal"));
    TEST_ASSERT_TRUE(n.resonance_count >= 1);
}

static void test_compose_birth_tone_match(void)
{
    dn_input_t in = make_date_input();
    in.has_birth = 1;
    in.birth_tone = 9;
    in.today_tone = 9;  /* same tone */

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_NOT_NULL(strstr(n.personal, "tone"));
    TEST_ASSERT_TRUE(n.resonance_count >= 1);
}

static void test_compose_birth_hexagram_match(void)
{
    dn_input_t in = make_date_input();
    in.has_birth = 1;
    in.birth_hexagram = 42;
    in.today_hexagram = 42;

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_NOT_NULL(strstr(n.personal, "hexagram"));
    TEST_ASSERT_TRUE(n.resonance_count >= 1);
}

static void test_compose_birth_hebrew_month_match(void)
{
    dn_input_t in = make_date_input();
    in.has_birth = 1;
    in.birth_hebrew_month = 6;
    in.today_hebrew_month = 6;

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_NOT_NULL(strstr(n.personal, "birth month"));
    TEST_ASSERT_TRUE(n.resonance_count >= 1);
}

static void test_compose_birth_no_matches(void)
{
    dn_input_t in = make_date_input();
    in.has_birth = 1;
    in.birth_seal = "Eagle";
    in.birth_tone = 5;
    in.birth_hexagram = 12;
    in.birth_hebrew_month = 3;
    in.today_seal = 0;   /* different */
    in.today_tone = 10;
    in.today_hexagram = 50;
    in.today_hebrew_month = 7;

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_NOT_NULL(strstr(n.personal, "No direct resonances"));
    TEST_ASSERT_EQUAL_INT(0, n.resonance_count);
}

static void test_compose_birth_multiple_matches(void)
{
    dn_input_t in = make_date_input();
    in.has_birth = 1;
    in.birth_seal = "Hand";
    in.today_seal = 6;  /* Hand = index 6 */
    in.birth_tone = 4;
    in.today_tone = 4;  /* same */
    in.birth_hexagram = 22;
    in.today_hexagram = 22; /* same */
    in.birth_hebrew_month = 0; /* unknown */
    in.today_hebrew_month = 0;

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_TRUE(n.resonance_count >= 2);
}

static void test_compose_no_birth_data(void)
{
    dn_input_t in = make_date_input();
    in.has_birth = 0;
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_STRING("", n.personal);
    TEST_ASSERT_EQUAL_INT(0, n.resonance_count);
}

/* === Wisdom quote tests === */

static void test_compose_wisdom_present(void)
{
    dn_input_t in = make_date_input();
    in.wisdom_quote = "Time is the fire in which we burn";
    in.wisdom_author = "Delmore Schwartz";

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_NOT_NULL(strstr(n.wisdom, "Time is the fire in which we burn"));
    TEST_ASSERT_NOT_NULL(strstr(n.wisdom, "Delmore Schwartz"));
}

static void test_compose_no_wisdom(void)
{
    dn_input_t in = make_date_input();
    in.wisdom_quote = NULL;
    in.wisdom_author = NULL;
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_STRING("", n.wisdom);
}

static void test_compose_wisdom_quote_only_no_author(void)
{
    dn_input_t in = make_date_input();
    in.wisdom_quote = "A quote";
    in.wisdom_author = NULL;
    daily_narrative_t n = dn_compose(&in);
    /* Both quote and author needed */
    TEST_ASSERT_EQUAL_STRING("", n.wisdom);
}

/* === dn_headline_type tests === */

static void test_headline_type_date(void)
{
    dn_input_t in = make_date_input();
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_INT(0, dn_headline_type(&n));
}

static void test_headline_type_significance(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = 1;
    in.insights[0].system_id = 0;
    in.insights[0].system_name = "Tzolkin";
    snprintf(in.insights[0].summary, sizeof(in.insights[0].summary),
             "Portal Day");
    in.insights[0].significance = 2;

    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_INT(1, dn_headline_type(&n));
}

static void test_headline_type_convergence(void)
{
    dn_input_t in = make_date_input();
    in.convergence_score = 5;
    in.convergence_desc = "Stars align";
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_INT(2, dn_headline_type(&n));
}

static void test_headline_type_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, dn_headline_type(NULL));
}

/* === Edge cases === */

static void test_compose_max_insights(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = DN_MAX_SYSTEMS;
    for (int i = 0; i < DN_MAX_SYSTEMS; i++) {
        in.insights[i].system_id = i;
        in.insights[i].system_name = "Sys";
        snprintf(in.insights[i].summary, sizeof(in.insights[i].summary),
                 "Entry %d", i);
        in.insights[i].significance = 0;
    }
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_EQUAL_INT(DN_MAX_SYSTEMS, n.insight_count);
}

static void test_compose_insight_count_clamped(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = DN_MAX_SYSTEMS + 5;  /* exceeds max */
    for (int i = 0; i < DN_MAX_SYSTEMS; i++) {
        in.insights[i].system_id = i;
        in.insights[i].system_name = "Sys";
        snprintf(in.insights[i].summary, sizeof(in.insights[i].summary),
                 "Entry %d", i);
        in.insights[i].significance = 0;
    }
    daily_narrative_t n = dn_compose(&in);
    /* Should be clamped to DN_MAX_SYSTEMS */
    TEST_ASSERT_EQUAL_INT(DN_MAX_SYSTEMS, n.insight_count);
}

static void test_compose_systems_format_separator(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = 2;
    in.insights[0].system_id = 0;
    in.insights[0].system_name = "Gregorian";
    snprintf(in.insights[0].summary, sizeof(in.insights[0].summary),
             "Spring Equinox");
    in.insights[0].significance = 0;
    in.insights[1].system_id = 1;
    in.insights[1].system_name = "Tzolkin";
    snprintf(in.insights[1].summary, sizeof(in.insights[1].summary),
             "Kin 207");
    in.insights[1].significance = 0;

    daily_narrative_t n = dn_compose(&in);
    /* Entries separated by ". " */
    TEST_ASSERT_NOT_NULL(strstr(n.systems, ". "));
}

static void test_compose_date_headline_with_insights_count(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = 4;
    for (int i = 0; i < 4; i++) {
        in.insights[i].system_id = i;
        in.insights[i].system_name = "Sys";
        snprintf(in.insights[i].summary, sizeof(in.insights[i].summary),
                 "Entry %d", i);
        in.insights[i].significance = 0;
    }
    daily_narrative_t n = dn_compose(&in);
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "4 systems speak today"));
}

static void test_compose_first_significant_insight_used(void)
{
    dn_input_t in = make_date_input();
    in.insight_count = 3;
    in.insights[0].system_id = 0;
    in.insights[0].system_name = "Gregorian";
    snprintf(in.insights[0].summary, sizeof(in.insights[0].summary),
             "Normal");
    in.insights[0].significance = 0;
    in.insights[1].system_id = 1;
    in.insights[1].system_name = "Tzolkin";
    snprintf(in.insights[1].summary, sizeof(in.insights[1].summary),
             "Portal Day");
    in.insights[1].significance = 2;
    in.insights[2].system_id = 2;
    in.insights[2].system_name = "Hebrew";
    snprintf(in.insights[2].summary, sizeof(in.insights[2].summary),
             "Holy Day");
    in.insights[2].significance = 3;

    daily_narrative_t n = dn_compose(&in);
    /* First insight with significance >= 2 is used */
    TEST_ASSERT_NOT_NULL(strstr(n.headline, "Portal Day"));
}

/* === Main === */

/* === Locale tests === */

static void test_compose_hebrew_month_name(void)
{
    dn_input_t in = make_date_input();
    in.locale = I18N_LOCALE_HE;
    daily_narrative_t n = dn_compose(&in);
    /* Hebrew March = מרץ — should NOT contain English "March" */
    TEST_ASSERT_NULL(strstr(n.headline, "March"));
    /* Should contain the Hebrew month name from content_get */
    TEST_ASSERT_TRUE(strlen(n.headline) > 0);
}

static void test_compose_default_locale_is_en(void)
{
    dn_input_t in = dn_default_input();
    /* locale field is 0 from memset = I18N_LOCALE_EN */
    TEST_ASSERT_EQUAL_INT(I18N_LOCALE_EN, in.locale);
}

int main(void)
{
    UNITY_BEGIN();

    /* dn_default_input */
    RUN_TEST(test_default_input_zeroed);

    /* dn_compose NULL */
    RUN_TEST(test_compose_null_input);

    /* Date-based headlines */
    RUN_TEST(test_compose_date_only_no_insights);
    RUN_TEST(test_compose_with_weekday_in_headline);
    RUN_TEST(test_compose_no_weekday_uses_date);

    /* Insights */
    RUN_TEST(test_compose_one_insight);
    RUN_TEST(test_compose_three_insights);
    RUN_TEST(test_compose_insight_count_matches);

    /* Significance-based headlines */
    RUN_TEST(test_compose_high_significance_headline);
    RUN_TEST(test_compose_significance_threshold);
    RUN_TEST(test_compose_first_significant_insight_used);

    /* Convergence-based headlines */
    RUN_TEST(test_compose_convergence_headline);
    RUN_TEST(test_compose_convergence_beats_significance);
    RUN_TEST(test_compose_convergence_needs_desc);
    RUN_TEST(test_compose_convergence_needs_score);
    RUN_TEST(test_convergence_score_echoed);

    /* Personal / birth resonance */
    RUN_TEST(test_compose_birth_seal_match);
    RUN_TEST(test_compose_birth_tone_match);
    RUN_TEST(test_compose_birth_hexagram_match);
    RUN_TEST(test_compose_birth_hebrew_month_match);
    RUN_TEST(test_compose_birth_no_matches);
    RUN_TEST(test_compose_birth_multiple_matches);
    RUN_TEST(test_compose_no_birth_data);

    /* Wisdom */
    RUN_TEST(test_compose_wisdom_present);
    RUN_TEST(test_compose_no_wisdom);
    RUN_TEST(test_compose_wisdom_quote_only_no_author);

    /* dn_headline_type */
    RUN_TEST(test_headline_type_date);
    RUN_TEST(test_headline_type_significance);
    RUN_TEST(test_headline_type_convergence);
    RUN_TEST(test_headline_type_null);

    /* Edge cases */
    RUN_TEST(test_compose_max_insights);
    RUN_TEST(test_compose_insight_count_clamped);
    RUN_TEST(test_compose_systems_format_separator);
    RUN_TEST(test_compose_date_headline_with_insights_count);

    /* Locale */
    RUN_TEST(test_compose_default_locale_is_en);
    RUN_TEST(test_compose_hebrew_month_name);

    return UNITY_END();
}
