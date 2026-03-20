/* notification_data.h — Pure notification scheduling data for the PERSONA stream.
 * Given cycle and event data, determines which events are worth notifying
 * the user about and assigns urgency/timing. Gentle, never pushy.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_NOTIFICATION_DATA_H
#define TIME_NOTIFICATION_DATA_H

#define ND_MAX_NOTIFICATIONS 8
#define ND_TITLE_MAX 64
#define ND_BODY_MAX 128

/* Notification urgency levels */
typedef enum {
    ND_URGENCY_NONE = 0,    /* Don't notify */
    ND_URGENCY_LOW,          /* Can wait, show when user opens Time */
    ND_URGENCY_MEDIUM,       /* Show within 24 hours */
    ND_URGENCY_HIGH,         /* Show soon — rare event happening */
    ND_URGENCY_COUNT
} nd_urgency_t;

/* Notification categories */
typedef enum {
    ND_CAT_CYCLE_TRANSITION = 0,  /* Wavespell/castle ending */
    ND_CAT_BIRTHDAY,               /* Birthday approaching/today */
    ND_CAT_GALACTIC_RETURN,        /* Kin return (every 260 days) */
    ND_CAT_RESONANCE,              /* Strong personal resonance today */
    ND_CAT_ECLIPSE,                /* Astronomical event */
    ND_CAT_MILESTONE,              /* Age milestone, streak */
    ND_CAT_COUNT
} nd_category_t;

/* A single notification */
typedef struct {
    nd_category_t category;
    nd_urgency_t urgency;
    char title[ND_TITLE_MAX];     /* "Wavespell Ending" */
    char body[ND_BODY_MAX];       /* "Red Dragon wavespell ends tomorrow" */
    int days_until;                /* 0 = today, 1 = tomorrow, etc. */
} nd_notification_t;

/* Input: pre-computed event data */
typedef struct {
    /* Cycle transitions (days remaining for each) */
    int wavespell_days_remaining;    /* -1 if unknown */
    int castle_days_remaining;       /* -1 if unknown */
    int zodiac_days_remaining;       /* -1 if unknown */
    char wavespell_name[64];         /* Current wavespell name */
    char castle_name[64];            /* Current castle name */
    char zodiac_name[64];            /* Current zodiac sign */

    /* Personal events */
    int days_until_birthday;         /* 0 = today, -1 = unknown */
    int days_until_galactic_return;  /* 0-259, 0 = today, -1 = unknown */

    /* Resonance */
    int strongest_resonance;         /* 0-5, 0 = none */
    char resonance_desc[128];        /* Description if any */

    /* User preference */
    int notification_level;          /* 0=off, 1=low, 2=medium, 3=high */
} nd_input_t;

/* Output: scheduled notifications */
typedef struct {
    nd_notification_t items[ND_MAX_NOTIFICATIONS];
    int count;
} nd_schedule_t;

/* Create default input (all unknown/-1, notification_level=1). */
nd_input_t nd_default_input(void);

/* Compute notification schedule from input.
 * Respects notification_level: 0=nothing, 1=only HIGH, 2=HIGH+MEDIUM, 3=all. */
nd_schedule_t nd_compute(const nd_input_t *input);

/* Get urgency name. "?" for invalid. */
const char *nd_urgency_name(nd_urgency_t urgency);

/* Get category name. "?" for invalid. */
const char *nd_category_name(nd_category_t category);

/* Count notifications at or above a given urgency level. */
int nd_count_above(const nd_schedule_t *schedule, nd_urgency_t min_urgency);

/* Check if any notification is for today (days_until == 0). */
int nd_any_today(const nd_schedule_t *schedule);

#endif /* TIME_NOTIFICATION_DATA_H */
