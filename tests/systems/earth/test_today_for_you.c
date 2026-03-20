/* test_today_for_you.c — Tests for Today For You daily briefing module.
 * TDD: RED phase first, then GREEN with implementation. */

#include "unity.h"
#include "systems/earth/today_for_you.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== tfy_default_input ===== */

void test_default_input_zeroed(void) {
    tfy_input_t input = tfy_default_input();
    TEST_ASSERT_EQUAL_INT(0, input.year);
    TEST_ASSERT_EQUAL_INT(0, input.month);
    TEST_ASSERT_EQUAL_INT(0, input.day);
    TEST_ASSERT_EQUAL_INT(0, input.hour);
    TEST_ASSERT_EQUAL_INT(0, input.days_alive);
    TEST_ASSERT_EQUAL_INT(0, input.age_years);
    TEST_ASSERT_EQUAL_INT(0, input.days_until_birthday);
    TEST_ASSERT_EQUAL_INT(0, input.cycle_count);
    TEST_ASSERT_EQUAL_INT(0, input.resonance_count);
    TEST_ASSERT_EQUAL_INT(0, input.strongest_resonance);
    TEST_ASSERT_EQUAL_INT(0, input.milestone_count);
}

/* ===== tfy_greeting_for_hour ===== */

void test_greeting_morning_5(void) {
    const char *g = tfy_greeting_for_hour(5);
    TEST_ASSERT_EQUAL_STRING("Good morning", g);
}

void test_greeting_morning_8(void) {
    const char *g = tfy_greeting_for_hour(8);
    TEST_ASSERT_EQUAL_STRING("Good morning", g);
}

void test_greeting_morning_11(void) {
    const char *g = tfy_greeting_for_hour(11);
    TEST_ASSERT_EQUAL_STRING("Good morning", g);
}

void test_greeting_afternoon_12(void) {
    const char *g = tfy_greeting_for_hour(12);
    TEST_ASSERT_EQUAL_STRING("Good afternoon", g);
}

void test_greeting_afternoon_15(void) {
    const char *g = tfy_greeting_for_hour(15);
    TEST_ASSERT_EQUAL_STRING("Good afternoon", g);
}

void test_greeting_afternoon_17(void) {
    const char *g = tfy_greeting_for_hour(17);
    TEST_ASSERT_EQUAL_STRING("Good afternoon", g);
}

void test_greeting_evening_18(void) {
    const char *g = tfy_greeting_for_hour(18);
    TEST_ASSERT_EQUAL_STRING("Good evening", g);
}

void test_greeting_evening_23(void) {
    const char *g = tfy_greeting_for_hour(23);
    TEST_ASSERT_EQUAL_STRING("Good evening", g);
}

void test_greeting_evening_0(void) {
    const char *g = tfy_greeting_for_hour(0);
    TEST_ASSERT_EQUAL_STRING("Good evening", g);
}

void test_greeting_evening_4(void) {
    const char *g = tfy_greeting_for_hour(4);
    TEST_ASSERT_EQUAL_STRING("Good evening", g);
}

void test_greeting_negative_hour(void) {
    const char *g = tfy_greeting_for_hour(-1);
    TEST_ASSERT_EQUAL_STRING("Good evening", g);
}

void test_greeting_hour_24(void) {
    const char *g = tfy_greeting_for_hour(24);
    TEST_ASSERT_EQUAL_STRING("Good evening", g);
}

/* ===== tfy_energy_name ===== */

void test_energy_name_quiet(void) {
    TEST_ASSERT_EQUAL_STRING("Quiet", tfy_energy_name(TFY_ENERGY_QUIET));
}

void test_energy_name_gentle(void) {
    TEST_ASSERT_EQUAL_STRING("Gentle", tfy_energy_name(TFY_ENERGY_GENTLE));
}

void test_energy_name_active(void) {
    TEST_ASSERT_EQUAL_STRING("Active", tfy_energy_name(TFY_ENERGY_ACTIVE));
}

void test_energy_name_intense(void) {
    TEST_ASSERT_EQUAL_STRING("Intense", tfy_energy_name(TFY_ENERGY_INTENSE));
}

void test_energy_name_extraordinary(void) {
    TEST_ASSERT_EQUAL_STRING("Extraordinary",
                             tfy_energy_name(TFY_ENERGY_EXTRAORDINARY));
}

void test_energy_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", tfy_energy_name(TFY_ENERGY_COUNT));
}

void test_energy_name_negative(void) {
    TEST_ASSERT_EQUAL_STRING("?", tfy_energy_name((tfy_energy_t)-1));
}

/* ===== tfy_compose — empty input (quiet day) ===== */

void test_compose_empty_quiet(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 9;
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL(TFY_ENERGY_QUIET, b.energy);
}

void test_compose_empty_greeting(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 9;
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_STRING("Good morning", b.greeting);
}

void test_compose_empty_headline(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 14;
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_STRING("A quiet day in the cycles.", b.headline);
}

void test_compose_empty_no_highlights(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(0, b.highlight_count);
}

/* ===== tfy_compose — 1 weak resonance (gentle) ===== */

void test_compose_one_weak_resonance_gentle(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 10;
    input.days_until_birthday = 180;
    input.resonance_count = 1;
    input.strongest_resonance = 2;
    snprintf(input.resonances[0].description, 256,
             "Sun in your natal sign");
    input.resonances[0].strength = 2;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL(TFY_ENERGY_GENTLE, b.energy);
}

void test_compose_one_weak_resonance_headline(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 10;
    input.days_until_birthday = 180;
    input.resonance_count = 1;
    input.strongest_resonance = 2;
    snprintf(input.resonances[0].description, 256,
             "Sun in your natal sign");
    input.resonances[0].strength = 2;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_STRING("Sun in your natal sign", b.headline);
}

/* ===== tfy_compose — milestone only (gentle) ===== */

void test_compose_milestone_only_gentle(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 15;
    input.days_until_birthday = 180;
    input.milestone_count = 1;
    snprintf(input.milestones[0].headline, 128,
             "10,000 days alive");
    input.milestones[0].score = 0.8;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL(TFY_ENERGY_GENTLE, b.energy);
}

void test_compose_milestone_headline_priority(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 15;
    input.days_until_birthday = 180;
    input.milestone_count = 1;
    snprintf(input.milestones[0].headline, 128,
             "10,000 days alive");
    input.milestones[0].score = 0.8;

    tfy_briefing_t b = tfy_compose(&input);
    /* Milestone should be the headline when no resonances */
    TEST_ASSERT_EQUAL_STRING("10,000 days alive", b.headline);
}

/* ===== tfy_compose — 2 resonances (active) ===== */

void test_compose_two_resonances_active(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 10;
    input.days_until_birthday = 180;
    input.resonance_count = 2;
    input.strongest_resonance = 2;
    snprintf(input.resonances[0].description, 256, "Moon trine natal Sun");
    input.resonances[0].strength = 2;
    snprintf(input.resonances[1].description, 256, "Wavespell resonance");
    input.resonances[1].strength = 1;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL(TFY_ENERGY_ACTIVE, b.energy);
}

/* ===== tfy_compose — 1 resonance strength 3 (active) ===== */

void test_compose_strength3_active(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.resonance_count = 1;
    input.strongest_resonance = 3;
    snprintf(input.resonances[0].description, 256, "Portal day resonance");
    input.resonances[0].strength = 3;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL(TFY_ENERGY_ACTIVE, b.energy);
}

/* ===== tfy_compose — strong resonance strength 4 (intense) ===== */

void test_compose_strength4_intense(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.resonance_count = 1;
    input.strongest_resonance = 4;
    snprintf(input.resonances[0].description, 256, "Galactic activation portal");
    input.resonances[0].strength = 4;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL(TFY_ENERGY_INTENSE, b.energy);
}

/* ===== tfy_compose — 2+ milestones (intense) ===== */

void test_compose_two_milestones_intense(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.milestone_count = 2;
    snprintf(input.milestones[0].headline, 128, "10,000 days alive");
    input.milestones[0].score = 0.8;
    snprintf(input.milestones[1].headline, 128, "Year of the Dragon begins");
    input.milestones[1].score = 0.6;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL(TFY_ENERGY_INTENSE, b.energy);
}

/* ===== tfy_compose — kin return strength 5 (extraordinary) ===== */

void test_compose_strength5_extraordinary(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.resonance_count = 1;
    input.strongest_resonance = 5;
    snprintf(input.resonances[0].description, 256,
             "Galactic birthday — Kin return!");
    input.resonances[0].strength = 5;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL(TFY_ENERGY_EXTRAORDINARY, b.energy);
}

/* ===== tfy_compose — birthday today (extraordinary + special greeting) ===== */

void test_compose_birthday_today_extraordinary(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 9;
    input.days_until_birthday = 0;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL(TFY_ENERGY_EXTRAORDINARY, b.energy);
}

void test_compose_birthday_today_special_greeting(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 9;
    input.days_until_birthday = 0;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_NOT_NULL(strstr(b.greeting, "birthday"));
}

void test_compose_birthday_365_special_greeting(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 14;
    input.days_until_birthday = 365;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_NOT_NULL(strstr(b.greeting, "birthday"));
}

void test_compose_birthday_366_special_greeting(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 20;
    input.days_until_birthday = 366;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_NOT_NULL(strstr(b.greeting, "birthday"));
}

/* ===== Headline priority: milestone > resonance > cycle ===== */

void test_compose_headline_milestone_over_resonance(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.resonance_count = 1;
    input.strongest_resonance = 3;
    snprintf(input.resonances[0].description, 256, "Resonance text");
    input.resonances[0].strength = 3;
    input.milestone_count = 1;
    snprintf(input.milestones[0].headline, 128, "Milestone text");
    input.milestones[0].score = 0.9;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_STRING("Milestone text", b.headline);
}

void test_compose_headline_resonance_over_cycle(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.resonance_count = 1;
    input.strongest_resonance = 2;
    snprintf(input.resonances[0].description, 256, "Resonance text");
    input.resonances[0].strength = 2;
    /* Add a cycle that transitions soon */
    input.cycle_count = 1;
    snprintf(input.cycles[0].name, TFY_LABEL_MAX, "Wavespell");
    snprintf(input.cycles[0].value, TFY_VALUE_MAX, "Red Dragon");
    input.cycles[0].days_remaining = 1;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_STRING("Resonance text", b.headline);
}

void test_compose_headline_cycle_fallback(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    /* No resonances, no milestones, but a cycle transition soon */
    input.cycle_count = 1;
    snprintf(input.cycles[0].name, TFY_LABEL_MAX, "Wavespell");
    snprintf(input.cycles[0].value, TFY_VALUE_MAX, "Red Dragon");
    input.cycles[0].days_remaining = 2;

    tfy_briefing_t b = tfy_compose(&input);
    /* Should mention the cycle name or value */
    TEST_ASSERT_NOT_NULL(strstr(b.headline, "Wavespell"));
}

/* ===== Highlights sorted by strength desc ===== */

void test_compose_highlights_sorted_by_strength(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.resonance_count = 3;
    input.strongest_resonance = 4;
    snprintf(input.resonances[0].description, 256, "Weak one");
    input.resonances[0].strength = 1;
    snprintf(input.resonances[1].description, 256, "Strong one");
    input.resonances[1].strength = 4;
    snprintf(input.resonances[2].description, 256, "Medium one");
    input.resonances[2].strength = 2;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_TRUE(b.highlight_count >= 3);
    /* First highlight should be strongest */
    TEST_ASSERT_EQUAL_INT(4, b.highlights[0].strength);
    /* Second should be medium */
    TEST_ASSERT_EQUAL_INT(2, b.highlights[1].strength);
    /* Third should be weakest */
    TEST_ASSERT_EQUAL_INT(1, b.highlights[2].strength);
}

void test_compose_highlights_cap_at_max(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    /* Fill resonances + milestones beyond TFY_MAX_HIGHLIGHTS */
    input.resonance_count = TFY_MAX_HIGHLIGHTS;
    input.strongest_resonance = 3;
    for (int i = 0; i < TFY_MAX_HIGHLIGHTS; i++) {
        snprintf(input.resonances[i].description, 256, "Res %d", i);
        input.resonances[i].strength = 2;
    }
    input.milestone_count = 2;
    snprintf(input.milestones[0].headline, 128, "Mile 0");
    input.milestones[0].score = 0.5;
    snprintf(input.milestones[1].headline, 128, "Mile 1");
    input.milestones[1].score = 0.3;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(TFY_MAX_HIGHLIGHTS, b.highlight_count);
}

void test_compose_highlights_milestones_as_type1(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.milestone_count = 1;
    snprintf(input.milestones[0].headline, 128, "10,000 days");
    input.milestones[0].score = 0.9;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(1, b.highlight_count);
    TEST_ASSERT_EQUAL_INT(1, b.highlights[0].type); /* milestone type */
}

void test_compose_highlights_resonances_as_type0(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.resonance_count = 1;
    input.strongest_resonance = 2;
    snprintf(input.resonances[0].description, 256, "Natal Sun");
    input.resonances[0].strength = 2;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(1, b.highlight_count);
    TEST_ASSERT_EQUAL_INT(0, b.highlights[0].type); /* resonance type */
}

/* ===== Cycle transition_soon flag ===== */

void test_compose_cycle_transition_soon_3days(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.cycle_count = 1;
    snprintf(input.cycles[0].name, TFY_LABEL_MAX, "Wavespell");
    snprintf(input.cycles[0].value, TFY_VALUE_MAX, "Red Dragon");
    input.cycles[0].days_remaining = 3;
    input.cycles[0].progress = 0.8;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(1, b.cycle_count);
    TEST_ASSERT_EQUAL_INT(1, b.cycles[0].transition_soon);
}

void test_compose_cycle_transition_soon_1day(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.cycle_count = 1;
    snprintf(input.cycles[0].name, TFY_LABEL_MAX, "Zodiac");
    snprintf(input.cycles[0].value, TFY_VALUE_MAX, "Aries");
    input.cycles[0].days_remaining = 1;
    input.cycles[0].progress = 0.97;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(1, b.cycles[0].transition_soon);
}

void test_compose_cycle_transition_not_soon(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.cycle_count = 1;
    snprintf(input.cycles[0].name, TFY_LABEL_MAX, "Wavespell");
    snprintf(input.cycles[0].value, TFY_VALUE_MAX, "White Wizard");
    input.cycles[0].days_remaining = 10;
    input.cycles[0].progress = 0.2;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(0, b.cycles[0].transition_soon);
}

/* ===== Birthday countdown ===== */

void test_compose_birthday_countdown_shown_30(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 30;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(1, b.show_birthday_countdown);
    TEST_ASSERT_EQUAL_INT(30, b.days_until_birthday);
}

void test_compose_birthday_countdown_shown_1(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 1;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(1, b.show_birthday_countdown);
}

void test_compose_birthday_countdown_hidden_31(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 31;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(0, b.show_birthday_countdown);
}

void test_compose_birthday_countdown_hidden_200(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 200;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(0, b.show_birthday_countdown);
}

/* ===== tfy_highlight_count / tfy_cycle_count ===== */

void test_highlight_count_empty(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(0, tfy_highlight_count(&b));
}

void test_highlight_count_with_data(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.resonance_count = 2;
    input.strongest_resonance = 3;
    snprintf(input.resonances[0].description, 256, "A");
    input.resonances[0].strength = 3;
    snprintf(input.resonances[1].description, 256, "B");
    input.resonances[1].strength = 1;

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(2, tfy_highlight_count(&b));
}

void test_cycle_count_empty(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(0, tfy_cycle_count(&b));
}

void test_cycle_count_with_data(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    input.cycle_count = 3;
    for (int i = 0; i < 3; i++) {
        snprintf(input.cycles[i].name, TFY_LABEL_MAX, "C%d", i);
        snprintf(input.cycles[i].value, TFY_VALUE_MAX, "V%d", i);
        input.cycles[i].days_remaining = 10;
    }

    tfy_briefing_t b = tfy_compose(&input);
    TEST_ASSERT_EQUAL_INT(3, tfy_cycle_count(&b));
}

/* ===== tfy_format ===== */

void test_format_nonempty(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 9;
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    char buf[2048];
    int len = tfy_format(&b, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_contains_greeting(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 14;
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    char buf[2048];
    tfy_format(&b, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Good afternoon"));
}

void test_format_contains_headline(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 9;
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    char buf[2048];
    tfy_format(&b, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "A quiet day in the cycles."));
}

void test_format_contains_energy(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 9;
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    char buf[2048];
    tfy_format(&b, buf, (int)sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Quiet"));
}

void test_format_small_buffer(void) {
    tfy_input_t input = tfy_default_input();
    input.hour = 9;
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    char buf[10];
    int len = tfy_format(&b, buf, (int)sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len <= 9);
    TEST_ASSERT_TRUE(strlen(buf) < 10);
}

void test_format_zero_buffer(void) {
    tfy_input_t input = tfy_default_input();
    input.days_until_birthday = 180;
    tfy_briefing_t b = tfy_compose(&input);
    char buf[1] = {'X'};
    int len = tfy_format(&b, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* ===== tfy_compose — NULL input ===== */

void test_compose_null_input_empty(void) {
    tfy_briefing_t b = tfy_compose(NULL);
    TEST_ASSERT_EQUAL(TFY_ENERGY_QUIET, b.energy);
    TEST_ASSERT_EQUAL_INT(0, b.highlight_count);
    TEST_ASSERT_EQUAL_INT(0, b.cycle_count);
    TEST_ASSERT_TRUE(strlen(b.greeting) == 0 || strlen(b.greeting) > 0);
    /* At minimum, the struct should be safe to read */
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* tfy_default_input */
    RUN_TEST(test_default_input_zeroed);

    /* tfy_greeting_for_hour */
    RUN_TEST(test_greeting_morning_5);
    RUN_TEST(test_greeting_morning_8);
    RUN_TEST(test_greeting_morning_11);
    RUN_TEST(test_greeting_afternoon_12);
    RUN_TEST(test_greeting_afternoon_15);
    RUN_TEST(test_greeting_afternoon_17);
    RUN_TEST(test_greeting_evening_18);
    RUN_TEST(test_greeting_evening_23);
    RUN_TEST(test_greeting_evening_0);
    RUN_TEST(test_greeting_evening_4);
    RUN_TEST(test_greeting_negative_hour);
    RUN_TEST(test_greeting_hour_24);

    /* tfy_energy_name */
    RUN_TEST(test_energy_name_quiet);
    RUN_TEST(test_energy_name_gentle);
    RUN_TEST(test_energy_name_active);
    RUN_TEST(test_energy_name_intense);
    RUN_TEST(test_energy_name_extraordinary);
    RUN_TEST(test_energy_name_invalid);
    RUN_TEST(test_energy_name_negative);

    /* tfy_compose — empty input */
    RUN_TEST(test_compose_empty_quiet);
    RUN_TEST(test_compose_empty_greeting);
    RUN_TEST(test_compose_empty_headline);
    RUN_TEST(test_compose_empty_no_highlights);

    /* tfy_compose — 1 weak resonance */
    RUN_TEST(test_compose_one_weak_resonance_gentle);
    RUN_TEST(test_compose_one_weak_resonance_headline);

    /* tfy_compose — milestone only */
    RUN_TEST(test_compose_milestone_only_gentle);
    RUN_TEST(test_compose_milestone_headline_priority);

    /* tfy_compose — 2 resonances */
    RUN_TEST(test_compose_two_resonances_active);

    /* tfy_compose — strength 3 */
    RUN_TEST(test_compose_strength3_active);

    /* tfy_compose — strength 4 */
    RUN_TEST(test_compose_strength4_intense);

    /* tfy_compose — 2+ milestones */
    RUN_TEST(test_compose_two_milestones_intense);

    /* tfy_compose — kin return */
    RUN_TEST(test_compose_strength5_extraordinary);

    /* tfy_compose — birthday */
    RUN_TEST(test_compose_birthday_today_extraordinary);
    RUN_TEST(test_compose_birthday_today_special_greeting);
    RUN_TEST(test_compose_birthday_365_special_greeting);
    RUN_TEST(test_compose_birthday_366_special_greeting);

    /* Headline priority */
    RUN_TEST(test_compose_headline_milestone_over_resonance);
    RUN_TEST(test_compose_headline_resonance_over_cycle);
    RUN_TEST(test_compose_headline_cycle_fallback);

    /* Highlights sorting */
    RUN_TEST(test_compose_highlights_sorted_by_strength);
    RUN_TEST(test_compose_highlights_cap_at_max);
    RUN_TEST(test_compose_highlights_milestones_as_type1);
    RUN_TEST(test_compose_highlights_resonances_as_type0);

    /* Cycle transition_soon */
    RUN_TEST(test_compose_cycle_transition_soon_3days);
    RUN_TEST(test_compose_cycle_transition_soon_1day);
    RUN_TEST(test_compose_cycle_transition_not_soon);

    /* Birthday countdown */
    RUN_TEST(test_compose_birthday_countdown_shown_30);
    RUN_TEST(test_compose_birthday_countdown_shown_1);
    RUN_TEST(test_compose_birthday_countdown_hidden_31);
    RUN_TEST(test_compose_birthday_countdown_hidden_200);

    /* tfy_highlight_count / tfy_cycle_count */
    RUN_TEST(test_highlight_count_empty);
    RUN_TEST(test_highlight_count_with_data);
    RUN_TEST(test_cycle_count_empty);
    RUN_TEST(test_cycle_count_with_data);

    /* tfy_format */
    RUN_TEST(test_format_nonempty);
    RUN_TEST(test_format_contains_greeting);
    RUN_TEST(test_format_contains_headline);
    RUN_TEST(test_format_contains_energy);
    RUN_TEST(test_format_small_buffer);
    RUN_TEST(test_format_zero_buffer);

    /* NULL input */
    RUN_TEST(test_compose_null_input_empty);

    return UNITY_END();
}
