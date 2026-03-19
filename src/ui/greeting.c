/* greeting.c — Seasonal & Contextual Greetings
 *
 * Pure functions producing context-aware greeting text.
 * No globals, no malloc, no side effects. */

#include "greeting.h"

#include <stddef.h>

/* ---- Cardinal date constants (approximate day of year) ---- */

#define DOY_VERNAL_EQUINOX   80   /* ~Mar 20 */
#define DOY_SUMMER_SOLSTICE  172  /* ~Jun 21 */
#define DOY_AUTUMNAL_EQUINOX 265  /* ~Sep 22 */
#define DOY_WINTER_SOLSTICE  355  /* ~Dec 21 */
#define CARDINAL_TOLERANCE   1    /* match within +/- 1 day */

/* ---- Helpers ---- */

static int abs_diff(int a, int b)
{
    int d = a - b;
    return d < 0 ? -d : d;
}

static int near_cardinal(int doy, int cardinal)
{
    return abs_diff(doy, cardinal) <= CARDINAL_TOLERANCE;
}

/* ---- Time of day ---- */

const char *greeting_time_text(int hour)
{
    if (hour >= 6 && hour <= 11) return "Good morning";
    if (hour >= 12 && hour <= 16) return "Good afternoon";
    if (hour >= 17 && hour <= 20) return "Good evening";
    return "Good night";
}

/* ---- Cardinal date (solstice/equinox) ---- */

const char *greeting_cardinal_text(int day_of_year, double latitude)
{
    int southern = (latitude < 0.0);

    if (near_cardinal(day_of_year, DOY_VERNAL_EQUINOX)) {
        return southern
            ? "Happy Autumnal Equinox"
            : "Happy Vernal Equinox";
    }
    if (near_cardinal(day_of_year, DOY_SUMMER_SOLSTICE)) {
        return southern
            ? "Happy Winter Solstice"
            : "Happy Summer Solstice";
    }
    if (near_cardinal(day_of_year, DOY_AUTUMNAL_EQUINOX)) {
        return southern
            ? "Happy Vernal Equinox"
            : "Happy Autumnal Equinox";
    }
    if (near_cardinal(day_of_year, DOY_WINTER_SOLSTICE)) {
        return southern
            ? "Happy Summer Solstice"
            : "Happy Winter Solstice";
    }

    return NULL;
}

/* ---- Sabbath ---- */

const char *greeting_sabbath_text(int weekday, int hour)
{
    /* Shabbat: Friday after sundown (~18:00) through Saturday before sundown.
     * Simplified: Friday 17+ and Saturday 0-19. */
    if (weekday == 5 && hour >= 17) return "Shabbat Shalom";
    if (weekday == 6 && hour < 18) return "Shabbat Shalom";
    return NULL;
}

/* ---- Aggregate generator ---- */

static void add_greeting(greeting_result_t *r, greeting_type_t type,
                         const char *text, const char *source)
{
    if (r->count >= GREETING_MAX) return;
    r->items[r->count].type = type;
    r->items[r->count].text = text;
    r->items[r->count].source = source;
    r->count++;
}

greeting_result_t greeting_generate(greeting_ctx_t ctx)
{
    greeting_result_t r = { .count = 0 };

    /* Always include time-of-day */
    add_greeting(&r, GREET_TIME_OF_DAY,
                 greeting_time_text(ctx.hour), "universal");

    /* Check cardinal dates */
    const char *cardinal = greeting_cardinal_text(ctx.day_of_year, ctx.latitude);
    if (cardinal != NULL) {
        int is_solstice =
            near_cardinal(ctx.day_of_year, DOY_SUMMER_SOLSTICE) ||
            near_cardinal(ctx.day_of_year, DOY_WINTER_SOLSTICE);
        add_greeting(&r,
                     is_solstice ? GREET_SOLSTICE : GREET_EQUINOX,
                     cardinal, "astronomy");
    }

    /* Check Sabbath */
    const char *sabbath = greeting_sabbath_text(ctx.weekday, ctx.hour);
    if (sabbath != NULL) {
        add_greeting(&r, GREET_SABBATH, sabbath, "Hebrew");
    }

    return r;
}

int greeting_count(const greeting_result_t *result)
{
    if (result == NULL) return 0;
    return result->count;
}
