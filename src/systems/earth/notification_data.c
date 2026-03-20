/* notification_data.c — Pure notification scheduling data for the PERSONA stream.
 * Determines which events merit notification and assigns urgency/timing.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "notification_data.h"

#include <string.h>
#include <stdio.h>

/* --- String tables --- */

static const char *URGENCY_NAMES[ND_URGENCY_COUNT] = {
    "None",
    "Low",
    "Medium",
    "High"
};

static const char *CATEGORY_NAMES[ND_CAT_COUNT] = {
    "Cycle Transition",
    "Birthday",
    "Galactic Return",
    "Resonance",
    "Eclipse",
    "Milestone"
};

/* --- Helpers --- */

/* Map notification_level to minimum urgency threshold.
 * 0=off (nothing), 1=HIGH only, 2=MEDIUM+, 3=LOW+ (all). */
static nd_urgency_t min_urgency_for_level(int level)
{
    switch (level) {
    case 1:  return ND_URGENCY_HIGH;
    case 2:  return ND_URGENCY_MEDIUM;
    case 3:  return ND_URGENCY_LOW;
    default: return ND_URGENCY_COUNT; /* nothing passes */
    }
}

/* Append a notification if it passes the urgency threshold and there is room. */
static void schedule_add(nd_schedule_t *s, nd_urgency_t min_urgency,
                         nd_category_t cat, nd_urgency_t urgency,
                         const char *title, const char *body, int days_until)
{
    if (urgency < min_urgency) {
        return;
    }
    if (s->count >= ND_MAX_NOTIFICATIONS) {
        return;
    }

    nd_notification_t *n = &s->items[s->count];
    n->category = cat;
    n->urgency = urgency;
    n->days_until = days_until;

    strncpy(n->title, title, ND_TITLE_MAX - 1);
    n->title[ND_TITLE_MAX - 1] = '\0';

    strncpy(n->body, body, ND_BODY_MAX - 1);
    n->body[ND_BODY_MAX - 1] = '\0';

    s->count++;
}

/* Sort schedule: descending urgency, then ascending days_until.
 * Simple insertion sort — max 8 items. */
static void schedule_sort(nd_schedule_t *s)
{
    for (int i = 1; i < s->count; i++) {
        nd_notification_t tmp = s->items[i];
        int j = i - 1;
        while (j >= 0) {
            int swap = 0;
            if (s->items[j].urgency < tmp.urgency) {
                swap = 1;
            } else if (s->items[j].urgency == tmp.urgency &&
                       s->items[j].days_until > tmp.days_until) {
                swap = 1;
            }
            if (!swap) break;
            s->items[j + 1] = s->items[j];
            j--;
        }
        s->items[j + 1] = tmp;
    }
}

/* --- Public API --- */

nd_input_t nd_default_input(void)
{
    nd_input_t in;
    memset(&in, 0, sizeof(in));
    in.wavespell_days_remaining = -1;
    in.castle_days_remaining = -1;
    in.zodiac_days_remaining = -1;
    in.days_until_birthday = -1;
    in.days_until_galactic_return = -1;
    in.strongest_resonance = 0;
    in.notification_level = 1;
    return in;
}

nd_schedule_t nd_compute(const nd_input_t *input)
{
    nd_schedule_t s;
    memset(&s, 0, sizeof(s));

    if (input == NULL) {
        return s;
    }

    if (input->notification_level <= 0) {
        return s;
    }

    nd_urgency_t min_urg = min_urgency_for_level(input->notification_level);

    /* --- Galactic return (kin) --- */
    if (input->days_until_galactic_return == 0) {
        schedule_add(&s, min_urg, ND_CAT_GALACTIC_RETURN, ND_URGENCY_HIGH,
                     "Galactic Return",
                     "Your Galactic Return is today", 0);
    } else if (input->days_until_galactic_return == 1) {
        schedule_add(&s, min_urg, ND_CAT_GALACTIC_RETURN, ND_URGENCY_MEDIUM,
                     "Galactic Return Tomorrow",
                     "Your Galactic Return is tomorrow", 1);
    }

    /* --- Birthday --- */
    if (input->days_until_birthday == 0) {
        schedule_add(&s, min_urg, ND_CAT_BIRTHDAY, ND_URGENCY_HIGH,
                     "Birthday Today",
                     "Happy birthday!", 0);
    } else if (input->days_until_birthday >= 1 && input->days_until_birthday <= 7) {
        char body[ND_BODY_MAX];
        snprintf(body, ND_BODY_MAX, "Your birthday is in %d day%s",
                 input->days_until_birthday,
                 input->days_until_birthday == 1 ? "" : "s");
        schedule_add(&s, min_urg, ND_CAT_BIRTHDAY, ND_URGENCY_MEDIUM,
                     "Birthday Approaching",
                     body, input->days_until_birthday);
    } else if (input->days_until_birthday >= 8 && input->days_until_birthday <= 30) {
        char body[ND_BODY_MAX];
        snprintf(body, ND_BODY_MAX, "Your birthday is in %d days",
                 input->days_until_birthday);
        schedule_add(&s, min_urg, ND_CAT_BIRTHDAY, ND_URGENCY_LOW,
                     "Birthday This Month",
                     body, input->days_until_birthday);
    }

    /* --- Resonance --- */
    if (input->strongest_resonance >= 5) {
        char body[ND_BODY_MAX];
        if (input->resonance_desc[0] != '\0') {
            snprintf(body, ND_BODY_MAX, "%s", input->resonance_desc);
        } else {
            snprintf(body, ND_BODY_MAX, "Extraordinary resonance detected");
        }
        schedule_add(&s, min_urg, ND_CAT_RESONANCE, ND_URGENCY_HIGH,
                     "Extraordinary Resonance", body, 0);
    } else if (input->strongest_resonance >= 4) {
        char body[ND_BODY_MAX];
        if (input->resonance_desc[0] != '\0') {
            snprintf(body, ND_BODY_MAX, "%s", input->resonance_desc);
        } else {
            snprintf(body, ND_BODY_MAX, "Strong resonance detected");
        }
        schedule_add(&s, min_urg, ND_CAT_RESONANCE, ND_URGENCY_MEDIUM,
                     "Strong Resonance", body, 0);
    }

    /* --- Wavespell ending --- */
    if (input->wavespell_days_remaining == 1) {
        char body[ND_BODY_MAX];
        if (input->wavespell_name[0] != '\0') {
            snprintf(body, ND_BODY_MAX, "%s wavespell ends tomorrow",
                     input->wavespell_name);
        } else {
            snprintf(body, ND_BODY_MAX, "Current wavespell ends tomorrow");
        }
        schedule_add(&s, min_urg, ND_CAT_CYCLE_TRANSITION, ND_URGENCY_MEDIUM,
                     "Wavespell Ending", body, 1);
    } else if (input->wavespell_days_remaining == 0) {
        char body[ND_BODY_MAX];
        if (input->wavespell_name[0] != '\0') {
            snprintf(body, ND_BODY_MAX, "%s wavespell ends today",
                     input->wavespell_name);
        } else {
            snprintf(body, ND_BODY_MAX, "Current wavespell ends today");
        }
        schedule_add(&s, min_urg, ND_CAT_CYCLE_TRANSITION, ND_URGENCY_LOW,
                     "Wavespell Ending Today", body, 0);
    }

    /* --- Castle ending --- */
    if (input->castle_days_remaining >= 0 && input->castle_days_remaining <= 3) {
        char body[ND_BODY_MAX];
        if (input->castle_name[0] != '\0') {
            snprintf(body, ND_BODY_MAX, "%s castle ends in %d day%s",
                     input->castle_name, input->castle_days_remaining,
                     input->castle_days_remaining == 1 ? "" : "s");
        } else {
            snprintf(body, ND_BODY_MAX, "Current castle ends in %d day%s",
                     input->castle_days_remaining,
                     input->castle_days_remaining == 1 ? "" : "s");
        }
        schedule_add(&s, min_urg, ND_CAT_CYCLE_TRANSITION, ND_URGENCY_LOW,
                     "Castle Ending", body, input->castle_days_remaining);
    }

    /* Sort: HIGH first, then MEDIUM, then LOW; within same urgency, sooner first */
    schedule_sort(&s);

    return s;
}

const char *nd_urgency_name(nd_urgency_t urgency)
{
    if (urgency < 0 || urgency >= ND_URGENCY_COUNT) {
        return "?";
    }
    return URGENCY_NAMES[urgency];
}

const char *nd_category_name(nd_category_t category)
{
    if (category < 0 || category >= ND_CAT_COUNT) {
        return "?";
    }
    return CATEGORY_NAMES[category];
}

int nd_count_above(const nd_schedule_t *schedule, nd_urgency_t min_urgency)
{
    if (schedule == NULL) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < schedule->count; i++) {
        if (schedule->items[i].urgency >= min_urgency) {
            count++;
        }
    }
    return count;
}

int nd_any_today(const nd_schedule_t *schedule)
{
    if (schedule == NULL) {
        return 0;
    }
    for (int i = 0; i < schedule->count; i++) {
        if (schedule->items[i].days_until == 0) {
            return 1;
        }
    }
    return 0;
}
