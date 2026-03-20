/* test_notification_data.c — Tests for notification scheduling data module.
 * TDD RED phase: all tests written before implementation.
 * Covers default input, urgency/category names, compute logic for
 * all event types, notification_level filtering, sorting, counting,
 * today detection, NULL safety, and title/body population. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/notification_data.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== nd_default_input ===== */

void test_default_input_values(void) {
    nd_input_t in = nd_default_input();
    TEST_ASSERT_EQUAL_INT(-1, in.wavespell_days_remaining);
    TEST_ASSERT_EQUAL_INT(-1, in.castle_days_remaining);
    TEST_ASSERT_EQUAL_INT(-1, in.zodiac_days_remaining);
    TEST_ASSERT_EQUAL_INT(-1, in.days_until_birthday);
    TEST_ASSERT_EQUAL_INT(-1, in.days_until_galactic_return);
    TEST_ASSERT_EQUAL_INT(0, in.strongest_resonance);
    TEST_ASSERT_EQUAL_INT(1, in.notification_level);
    TEST_ASSERT_EQUAL_STRING("", in.wavespell_name);
    TEST_ASSERT_EQUAL_STRING("", in.castle_name);
    TEST_ASSERT_EQUAL_STRING("", in.zodiac_name);
    TEST_ASSERT_EQUAL_STRING("", in.resonance_desc);
}

/* ===== nd_urgency_name ===== */

void test_urgency_name_none(void) {
    TEST_ASSERT_EQUAL_STRING("None", nd_urgency_name(ND_URGENCY_NONE));
}

void test_urgency_name_low(void) {
    TEST_ASSERT_EQUAL_STRING("Low", nd_urgency_name(ND_URGENCY_LOW));
}

void test_urgency_name_medium(void) {
    TEST_ASSERT_EQUAL_STRING("Medium", nd_urgency_name(ND_URGENCY_MEDIUM));
}

void test_urgency_name_high(void) {
    TEST_ASSERT_EQUAL_STRING("High", nd_urgency_name(ND_URGENCY_HIGH));
}

void test_urgency_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", nd_urgency_name(ND_URGENCY_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", nd_urgency_name((nd_urgency_t)99));
}

/* ===== nd_category_name ===== */

void test_category_name_cycle(void) {
    TEST_ASSERT_EQUAL_STRING("Cycle Transition", nd_category_name(ND_CAT_CYCLE_TRANSITION));
}

void test_category_name_birthday(void) {
    TEST_ASSERT_EQUAL_STRING("Birthday", nd_category_name(ND_CAT_BIRTHDAY));
}

void test_category_name_galactic_return(void) {
    TEST_ASSERT_EQUAL_STRING("Galactic Return", nd_category_name(ND_CAT_GALACTIC_RETURN));
}

void test_category_name_resonance(void) {
    TEST_ASSERT_EQUAL_STRING("Resonance", nd_category_name(ND_CAT_RESONANCE));
}

void test_category_name_eclipse(void) {
    TEST_ASSERT_EQUAL_STRING("Eclipse", nd_category_name(ND_CAT_ECLIPSE));
}

void test_category_name_milestone(void) {
    TEST_ASSERT_EQUAL_STRING("Milestone", nd_category_name(ND_CAT_MILESTONE));
}

void test_category_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", nd_category_name(ND_CAT_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", nd_category_name((nd_category_t)99));
}

/* ===== nd_compute — empty/default ===== */

void test_compute_default_input_no_notifications(void) {
    nd_input_t in = nd_default_input();
    nd_schedule_t s = nd_compute(&in);
    TEST_ASSERT_EQUAL_INT(0, s.count);
}

void test_compute_null_input_empty_schedule(void) {
    nd_schedule_t s = nd_compute(NULL);
    TEST_ASSERT_EQUAL_INT(0, s.count);
}

void test_compute_level_zero_returns_nothing(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 0;
    in.days_until_birthday = 0;  /* would be HIGH */
    in.days_until_galactic_return = 0;  /* would be HIGH */
    nd_schedule_t s = nd_compute(&in);
    TEST_ASSERT_EQUAL_INT(0, s.count);
}

/* ===== nd_compute — birthday ===== */

void test_compute_birthday_today_high(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 0;
    nd_schedule_t s = nd_compute(&in);
    TEST_ASSERT_TRUE(s.count >= 1);
    /* Find birthday notification */
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_BIRTHDAY) {
            TEST_ASSERT_EQUAL_INT(ND_URGENCY_HIGH, s.items[i].urgency);
            TEST_ASSERT_EQUAL_INT(0, s.items[i].days_until);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_compute_birthday_in_3_days_medium(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 3;
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_BIRTHDAY) {
            TEST_ASSERT_EQUAL_INT(ND_URGENCY_MEDIUM, s.items[i].urgency);
            TEST_ASSERT_EQUAL_INT(3, s.items[i].days_until);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_compute_birthday_in_15_days_low(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 15;
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_BIRTHDAY) {
            TEST_ASSERT_EQUAL_INT(ND_URGENCY_LOW, s.items[i].urgency);
            TEST_ASSERT_EQUAL_INT(15, s.items[i].days_until);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_compute_birthday_in_40_days_no_notification(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 40;
    nd_schedule_t s = nd_compute(&in);
    for (int i = 0; i < s.count; i++) {
        TEST_ASSERT_NOT_EQUAL(ND_CAT_BIRTHDAY, s.items[i].category);
    }
}

/* ===== nd_compute — galactic return (kin) ===== */

void test_compute_kin_return_today_high(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_galactic_return = 0;
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_GALACTIC_RETURN) {
            TEST_ASSERT_EQUAL_INT(ND_URGENCY_HIGH, s.items[i].urgency);
            TEST_ASSERT_EQUAL_INT(0, s.items[i].days_until);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_compute_kin_return_tomorrow_medium(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_galactic_return = 1;
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_GALACTIC_RETURN) {
            TEST_ASSERT_EQUAL_INT(ND_URGENCY_MEDIUM, s.items[i].urgency);
            TEST_ASSERT_EQUAL_INT(1, s.items[i].days_until);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_compute_kin_return_in_5_days_no_notification(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_galactic_return = 5;
    nd_schedule_t s = nd_compute(&in);
    for (int i = 0; i < s.count; i++) {
        TEST_ASSERT_NOT_EQUAL(ND_CAT_GALACTIC_RETURN, s.items[i].category);
    }
}

/* ===== nd_compute — wavespell ===== */

void test_compute_wavespell_ending_tomorrow_medium(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.wavespell_days_remaining = 1;
    strncpy(in.wavespell_name, "Red Dragon", 63);
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_CYCLE_TRANSITION &&
            s.items[i].urgency == ND_URGENCY_MEDIUM) {
            TEST_ASSERT_EQUAL_INT(1, s.items[i].days_until);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_compute_wavespell_ending_today_low(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.wavespell_days_remaining = 0;
    strncpy(in.wavespell_name, "White Wizard", 63);
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_CYCLE_TRANSITION &&
            s.items[i].urgency == ND_URGENCY_LOW) {
            TEST_ASSERT_EQUAL_INT(0, s.items[i].days_until);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ===== nd_compute — castle ===== */

void test_compute_castle_ending_in_2_days_low(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.castle_days_remaining = 2;
    strncpy(in.castle_name, "Red Eastern", 63);
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_CYCLE_TRANSITION &&
            s.items[i].urgency == ND_URGENCY_LOW) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ===== nd_compute — resonance ===== */

void test_compute_resonance_4_medium(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.strongest_resonance = 4;
    strncpy(in.resonance_desc, "Strong harmonic alignment", 127);
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_RESONANCE) {
            TEST_ASSERT_EQUAL_INT(ND_URGENCY_MEDIUM, s.items[i].urgency);
            TEST_ASSERT_EQUAL_INT(0, s.items[i].days_until);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_compute_resonance_5_high(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.strongest_resonance = 5;
    strncpy(in.resonance_desc, "Extraordinary convergence", 127);
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_RESONANCE) {
            TEST_ASSERT_EQUAL_INT(ND_URGENCY_HIGH, s.items[i].urgency);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_compute_resonance_3_no_notification(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.strongest_resonance = 3;
    nd_schedule_t s = nd_compute(&in);
    for (int i = 0; i < s.count; i++) {
        TEST_ASSERT_NOT_EQUAL(ND_CAT_RESONANCE, s.items[i].category);
    }
}

/* ===== nd_compute — notification_level filtering ===== */

void test_compute_level_1_high_only(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 1;
    in.days_until_birthday = 0;       /* HIGH */
    in.wavespell_days_remaining = 1;  /* MEDIUM */
    in.castle_days_remaining = 2;     /* LOW */
    nd_schedule_t s = nd_compute(&in);
    for (int i = 0; i < s.count; i++) {
        TEST_ASSERT_EQUAL_INT(ND_URGENCY_HIGH, s.items[i].urgency);
    }
    TEST_ASSERT_TRUE(s.count >= 1);
}

void test_compute_level_2_high_and_medium(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 2;
    in.days_until_birthday = 0;       /* HIGH */
    in.wavespell_days_remaining = 1;  /* MEDIUM */
    strncpy(in.wavespell_name, "Blue Storm", 63);
    in.castle_days_remaining = 2;     /* LOW */
    strncpy(in.castle_name, "White Northern", 63);
    nd_schedule_t s = nd_compute(&in);
    for (int i = 0; i < s.count; i++) {
        TEST_ASSERT_TRUE(s.items[i].urgency >= ND_URGENCY_MEDIUM);
    }
    TEST_ASSERT_TRUE(s.count >= 2);
}

void test_compute_level_3_shows_all(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 0;       /* HIGH */
    in.wavespell_days_remaining = 1;  /* MEDIUM */
    strncpy(in.wavespell_name, "Yellow Seed", 63);
    in.castle_days_remaining = 2;     /* LOW */
    strncpy(in.castle_name, "Green Central", 63);
    nd_schedule_t s = nd_compute(&in);
    TEST_ASSERT_TRUE(s.count >= 3);
}

/* ===== nd_compute — multiple events combined ===== */

void test_compute_multiple_events_combined(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 5;            /* MEDIUM */
    in.strongest_resonance = 4;            /* MEDIUM */
    strncpy(in.resonance_desc, "Alignment", 127);
    in.wavespell_days_remaining = 0;       /* LOW */
    strncpy(in.wavespell_name, "Red Moon", 63);
    nd_schedule_t s = nd_compute(&in);
    TEST_ASSERT_TRUE(s.count >= 3);
}

/* ===== nd_compute — sorting ===== */

void test_compute_sorted_high_before_medium_before_low(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 0;            /* HIGH */
    in.days_until_galactic_return = 1;     /* MEDIUM */
    in.castle_days_remaining = 2;          /* LOW */
    strncpy(in.castle_name, "Blue Western", 63);
    nd_schedule_t s = nd_compute(&in);
    /* Verify descending urgency order */
    for (int i = 1; i < s.count; i++) {
        TEST_ASSERT_TRUE(s.items[i - 1].urgency >= s.items[i].urgency);
    }
}

void test_compute_sorted_same_urgency_by_days(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 3;         /* MEDIUM */
    in.days_until_galactic_return = 1;  /* MEDIUM */
    nd_schedule_t s = nd_compute(&in);
    /* Both MEDIUM — kin return (1 day) should come before birthday (3 days) */
    int kin_idx = -1;
    int bday_idx = -1;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_GALACTIC_RETURN) kin_idx = i;
        if (s.items[i].category == ND_CAT_BIRTHDAY) bday_idx = i;
    }
    TEST_ASSERT_TRUE(kin_idx >= 0);
    TEST_ASSERT_TRUE(bday_idx >= 0);
    TEST_ASSERT_TRUE(kin_idx < bday_idx);
}

/* ===== nd_count_above ===== */

void test_count_above_empty_schedule(void) {
    nd_schedule_t s;
    memset(&s, 0, sizeof(s));
    TEST_ASSERT_EQUAL_INT(0, nd_count_above(&s, ND_URGENCY_LOW));
}

void test_count_above_mixed_urgencies(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 0;            /* HIGH */
    in.days_until_galactic_return = 1;     /* MEDIUM */
    in.castle_days_remaining = 2;          /* LOW */
    strncpy(in.castle_name, "Yellow Southern", 63);
    nd_schedule_t s = nd_compute(&in);

    int high_count = nd_count_above(&s, ND_URGENCY_HIGH);
    int med_count = nd_count_above(&s, ND_URGENCY_MEDIUM);
    int low_count = nd_count_above(&s, ND_URGENCY_LOW);

    TEST_ASSERT_TRUE(high_count >= 1);
    TEST_ASSERT_TRUE(med_count >= high_count);
    TEST_ASSERT_TRUE(low_count >= med_count);
}

void test_count_above_null_schedule(void) {
    TEST_ASSERT_EQUAL_INT(0, nd_count_above(NULL, ND_URGENCY_LOW));
}

/* ===== nd_any_today ===== */

void test_any_today_when_today_present(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 0;  /* today */
    nd_schedule_t s = nd_compute(&in);
    TEST_ASSERT_EQUAL_INT(1, nd_any_today(&s));
}

void test_any_today_when_none_today(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 5;  /* not today */
    nd_schedule_t s = nd_compute(&in);
    TEST_ASSERT_EQUAL_INT(0, nd_any_today(&s));
}

void test_any_today_null_schedule(void) {
    TEST_ASSERT_EQUAL_INT(0, nd_any_today(NULL));
}

void test_any_today_empty_schedule(void) {
    nd_schedule_t s;
    memset(&s, 0, sizeof(s));
    TEST_ASSERT_EQUAL_INT(0, nd_any_today(&s));
}

/* ===== Title/body population ===== */

void test_birthday_today_title_body(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_birthday = 0;
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_BIRTHDAY) {
            TEST_ASSERT_TRUE(strlen(s.items[i].title) > 0);
            TEST_ASSERT_TRUE(strlen(s.items[i].body) > 0);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_kin_return_today_title_body(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.days_until_galactic_return = 0;
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_GALACTIC_RETURN) {
            TEST_ASSERT_TRUE(strlen(s.items[i].title) > 0);
            TEST_ASSERT_TRUE(strlen(s.items[i].body) > 0);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_wavespell_title_includes_name(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.wavespell_days_remaining = 1;
    strncpy(in.wavespell_name, "Red Dragon", 63);
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_CYCLE_TRANSITION &&
            s.items[i].urgency == ND_URGENCY_MEDIUM) {
            TEST_ASSERT_TRUE(strlen(s.items[i].title) > 0);
            /* Body should mention the wavespell name */
            TEST_ASSERT_NOT_NULL(strstr(s.items[i].body, "Red Dragon"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

void test_resonance_body_includes_description(void) {
    nd_input_t in = nd_default_input();
    in.notification_level = 3;
    in.strongest_resonance = 4;
    strncpy(in.resonance_desc, "Cosmic alignment today", 127);
    nd_schedule_t s = nd_compute(&in);
    int found = 0;
    for (int i = 0; i < s.count; i++) {
        if (s.items[i].category == ND_CAT_RESONANCE) {
            TEST_ASSERT_NOT_NULL(strstr(s.items[i].body, "Cosmic alignment today"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* Default input (1 test) */
    RUN_TEST(test_default_input_values);

    /* Urgency names (5 tests) */
    RUN_TEST(test_urgency_name_none);
    RUN_TEST(test_urgency_name_low);
    RUN_TEST(test_urgency_name_medium);
    RUN_TEST(test_urgency_name_high);
    RUN_TEST(test_urgency_name_invalid);

    /* Category names (7 tests) */
    RUN_TEST(test_category_name_cycle);
    RUN_TEST(test_category_name_birthday);
    RUN_TEST(test_category_name_galactic_return);
    RUN_TEST(test_category_name_resonance);
    RUN_TEST(test_category_name_eclipse);
    RUN_TEST(test_category_name_milestone);
    RUN_TEST(test_category_name_invalid);

    /* Compute — empty/default (3 tests) */
    RUN_TEST(test_compute_default_input_no_notifications);
    RUN_TEST(test_compute_null_input_empty_schedule);
    RUN_TEST(test_compute_level_zero_returns_nothing);

    /* Compute — birthday (4 tests) */
    RUN_TEST(test_compute_birthday_today_high);
    RUN_TEST(test_compute_birthday_in_3_days_medium);
    RUN_TEST(test_compute_birthday_in_15_days_low);
    RUN_TEST(test_compute_birthday_in_40_days_no_notification);

    /* Compute — galactic return (3 tests) */
    RUN_TEST(test_compute_kin_return_today_high);
    RUN_TEST(test_compute_kin_return_tomorrow_medium);
    RUN_TEST(test_compute_kin_return_in_5_days_no_notification);

    /* Compute — wavespell (2 tests) */
    RUN_TEST(test_compute_wavespell_ending_tomorrow_medium);
    RUN_TEST(test_compute_wavespell_ending_today_low);

    /* Compute — castle (1 test) */
    RUN_TEST(test_compute_castle_ending_in_2_days_low);

    /* Compute — resonance (3 tests) */
    RUN_TEST(test_compute_resonance_4_medium);
    RUN_TEST(test_compute_resonance_5_high);
    RUN_TEST(test_compute_resonance_3_no_notification);

    /* Compute — notification level filtering (3 tests) */
    RUN_TEST(test_compute_level_1_high_only);
    RUN_TEST(test_compute_level_2_high_and_medium);
    RUN_TEST(test_compute_level_3_shows_all);

    /* Compute — multiple events (1 test) */
    RUN_TEST(test_compute_multiple_events_combined);

    /* Compute — sorting (2 tests) */
    RUN_TEST(test_compute_sorted_high_before_medium_before_low);
    RUN_TEST(test_compute_sorted_same_urgency_by_days);

    /* nd_count_above (3 tests) */
    RUN_TEST(test_count_above_empty_schedule);
    RUN_TEST(test_count_above_mixed_urgencies);
    RUN_TEST(test_count_above_null_schedule);

    /* nd_any_today (4 tests) */
    RUN_TEST(test_any_today_when_today_present);
    RUN_TEST(test_any_today_when_none_today);
    RUN_TEST(test_any_today_null_schedule);
    RUN_TEST(test_any_today_empty_schedule);

    /* Title/body population (4 tests) */
    RUN_TEST(test_birthday_today_title_body);
    RUN_TEST(test_kin_return_today_title_body);
    RUN_TEST(test_wavespell_title_includes_name);
    RUN_TEST(test_resonance_body_includes_description);

    return UNITY_END();
}
