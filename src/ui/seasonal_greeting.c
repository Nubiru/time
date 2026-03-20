#include "seasonal_greeting.h"

#include <stdio.h>
#include <string.h>

/* ---- Time-of-day greetings ---- */

const char *sg_time_greeting(int hour)
{
    /* Clamp to 0-23 */
    if (hour < 0) { hour = 0; }
    if (hour > 23) { hour = hour % 24; }

    if (hour >= 6 && hour < 12) {
        return "Good morning";
    }
    if (hour >= 12 && hour < 17) {
        return "Good afternoon";
    }
    if (hour >= 17 && hour < 21) {
        return "Good evening";
    }
    return "Good night";
}

/* ---- Hebrew Sabbath ---- */

const char *sg_hebrew_greeting(int weekday, int hour)
{
    /* Shabbat: Friday (5) after 18:00 through Saturday (6) before 19:00 */
    if (weekday == 5 && hour >= 18) {
        return "Shabbat Shalom";
    }
    if (weekday == 6 && hour < 19) {
        return "Shabbat Shalom";
    }
    return NULL;
}

/* ---- Islamic Ramadan ---- */

const char *sg_islamic_greeting(int is_ramadan)
{
    if (is_ramadan) {
        return "Ramadan Kareem";
    }
    return NULL;
}

/* ---- Astronomical events ---- */

/* Approximate day-of-year for solstices/equinoxes (Northern Hemisphere):
 *   Spring Equinox:  ~March 20  = day 79-80
 *   Summer Solstice: ~June 21   = day 172
 *   Autumn Equinox:  ~Sep 22-23 = day 265-266
 *   Winter Solstice: ~Dec 21    = day 355
 */

static const int s_event_days[] = { 80, 172, 266, 355 };

static const char *s_north_names[] = {
    "Happy Spring Equinox",
    "Happy Summer Solstice",
    "Happy Autumn Equinox",
    "Happy Winter Solstice"
};

static const char *s_south_names[] = {
    "Happy Autumn Equinox",
    "Happy Winter Solstice",
    "Happy Spring Equinox",
    "Happy Summer Solstice"
};

const char *sg_astro_greeting(int day_of_year, int is_southern)
{
    if (day_of_year < 1 || day_of_year > 366) {
        return NULL;
    }
    for (int i = 0; i < 4; i++) {
        int diff = day_of_year - s_event_days[i];
        if (diff < 0) { diff = -diff; }
        if (diff <= 1) {
            return is_southern ? s_south_names[i] : s_north_names[i];
        }
    }
    return NULL;
}

/* ---- Chinese New Year ---- */

const char *sg_chinese_greeting(int is_new_year)
{
    if (is_new_year) {
        return "Gong Xi Fa Cai";
    }
    return NULL;
}

/* ---- Buddhist Uposatha ---- */

const char *sg_buddhist_greeting(int is_uposatha)
{
    if (is_uposatha) {
        return "Blessed Uposatha";
    }
    return NULL;
}

/* ---- Generic new year ---- */

const char *sg_new_year_greeting(void)
{
    return "A new cycle begins";
}

/* ---- Compose ---- */

const char *sg_compose(char *buf, int buf_len,
                       int hour, int weekday,
                       int day_of_year, int is_southern,
                       int hebrew_active, int islamic_ramadan,
                       int chinese_new_year, int buddhist_uposatha)
{
    if (buf == NULL || buf_len <= 0) {
        return NULL;
    }

    const char *time_g = sg_time_greeting(hour);

    /* Find first applicable system greeting */
    const char *sys_g = NULL;
    if (hebrew_active) {
        sys_g = sg_hebrew_greeting(weekday, hour);
    }
    if (sys_g == NULL && islamic_ramadan) {
        sys_g = sg_islamic_greeting(1);
    }
    if (sys_g == NULL && chinese_new_year) {
        sys_g = sg_chinese_greeting(1);
    }
    if (sys_g == NULL && buddhist_uposatha) {
        sys_g = sg_buddhist_greeting(1);
    }

    /* Check astronomical event */
    const char *astro_g = sg_astro_greeting(day_of_year, is_southern);
    if (sys_g == NULL) {
        sys_g = astro_g;
    }

    if (sys_g != NULL) {
        snprintf(buf, (size_t)buf_len, "%s. %s", time_g, sys_g);
    } else {
        snprintf(buf, (size_t)buf_len, "%s", time_g);
    }

    return buf;
}
