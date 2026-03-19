/* greeting.h — Seasonal & Contextual Greetings
 * Generates context-aware greeting text based on time of day,
 * season, astronomical events, and calendar occasions.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * All returned strings are pointers to static const data. */

#ifndef TIME_GREETING_H
#define TIME_GREETING_H

#define GREETING_MAX 4

typedef enum {
    GREET_TIME_OF_DAY = 0,  /* "Good morning" */
    GREET_SOLSTICE,         /* "Happy Summer Solstice!" */
    GREET_EQUINOX,          /* "Happy Vernal Equinox!" */
    GREET_SABBATH,          /* "Shabbat Shalom" */
    GREET_TYPE_COUNT
} greeting_type_t;

typedef struct {
    greeting_type_t type;
    const char *text;       /* greeting string */
    const char *source;     /* cultural attribution */
} greeting_t;

typedef struct {
    greeting_t items[GREETING_MAX];
    int count;
} greeting_result_t;

/* Context for greeting generation */
typedef struct {
    int hour;           /* 0-23 local time */
    int day_of_year;    /* 1-365 */
    int weekday;        /* 0=Sunday, 1=Monday ... 5=Friday, 6=Saturday */
    double latitude;    /* -90 to 90 degrees */
} greeting_ctx_t;

/* Time-of-day greeting for the given hour.
 * 6-11: "Good morning", 12-16: "Good afternoon",
 * 17-20: "Good evening", else: "Good night". */
const char *greeting_time_text(int hour);

/* Solstice/equinox greeting if within 1 day of a cardinal date.
 * Returns NULL if not near any cardinal date.
 * Hemisphere-aware: Jun solstice is "Summer" in north, "Winter" in south. */
const char *greeting_cardinal_text(int day_of_year, double latitude);

/* Sabbath greeting for Friday evening through Saturday afternoon.
 * weekday: 0=Sunday ... 5=Friday, 6=Saturday.
 * Returns "Shabbat Shalom" during Shabbat, NULL otherwise. */
const char *greeting_sabbath_text(int weekday, int hour);

/* Generate all applicable greetings for the given context.
 * Always includes time-of-day. Adds cardinal/sabbath when applicable. */
greeting_result_t greeting_generate(greeting_ctx_t ctx);

/* Count greetings in a result. Returns 0 for NULL. */
int greeting_count(const greeting_result_t *result);

#endif /* TIME_GREETING_H */
