#ifndef TIME_SEASONAL_GREETING_H
#define TIME_SEASONAL_GREETING_H

/*
 * seasonal_greeting.h — Contextual greetings based on time, calendar, astronomy.
 *
 * Pure module: no state, no GL, no side effects.
 * Returns static string pointers — caller must NOT free.
 */

/* Time-of-day greeting (based on hour 0-23) */
const char *sg_time_greeting(int hour);

/* Weekday index: 0=Sunday .. 6=Saturday */

/* Hebrew Sabbath greeting (Friday evening / Saturday) */
const char *sg_hebrew_greeting(int weekday, int hour);

/* Islamic greeting for Ramadan (caller determines if Ramadan is active) */
const char *sg_islamic_greeting(int is_ramadan);

/* Astronomical event greeting based on day of year (1-366)
 * Returns NULL if no event within +/- 1 day of a solstice/equinox. */
const char *sg_astro_greeting(int day_of_year, int is_southern);

/* Chinese New Year greeting (caller determines if it's CNY period) */
const char *sg_chinese_greeting(int is_new_year);

/* Buddhist greeting for Uposatha days */
const char *sg_buddhist_greeting(int is_uposatha);

/* Generic new year greeting for any system */
const char *sg_new_year_greeting(void);

/* Compose a full greeting: time greeting + optional system greeting.
 * Writes into buf (max buf_len), returns buf.
 * If no system greeting, just returns time greeting. */
const char *sg_compose(char *buf, int buf_len,
                       int hour, int weekday,
                       int day_of_year, int is_southern,
                       int hebrew_active, int islamic_ramadan,
                       int chinese_new_year, int buddhist_uposatha);

#endif /* TIME_SEASONAL_GREETING_H */
