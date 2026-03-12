#include "date_parse.h"
#include "../math/julian.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* Parse an integer from str starting at *pos, advancing *pos past digits.
 * Returns the parsed integer. Sets *ok to 0 on failure. */
static int parse_int(const char *str, int *pos, int *ok)
{
    int val = 0;
    int digits = 0;
    int neg = 0;

    if (str[*pos] == '-') {
        neg = 1;
        (*pos)++;
    }

    while (str[*pos] >= '0' && str[*pos] <= '9') {
        val = val * 10 + (str[*pos] - '0');
        (*pos)++;
        digits++;
    }

    if (digits == 0) {
        *ok = 0;
        return 0;
    }

    return neg ? -val : val;
}

/* Expect a specific character at position, advance past it */
static int expect_char(const char *str, int *pos, char c)
{
    if (str[*pos] == c) {
        (*pos)++;
        return 1;
    }
    return 0;
}

int date_is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int date_days_in_month(int year, int month)
{
    static const int DAYS[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && date_is_leap_year(year)) return 29;
    return DAYS[month - 1];
}

int date_validate(int year, int month, int day, int hour, int minute, int second)
{
    (void)year; /* Any year is valid for astronomical dates */
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > date_days_in_month(year, month)) return 0;
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    if (second < 0 || second > 59) return 0;
    return 1;
}

parsed_date_t date_parse(const char *str)
{
    parsed_date_t d;
    memset(&d, 0, sizeof(d));

    if (!str || str[0] == '\0') return d;

    int pos = 0;
    int ok = 1;

    /* Parse year (may be negative) */
    d.year = parse_int(str, &pos, &ok);
    if (!ok) return d;

    /* Expect '-' */
    if (!expect_char(str, &pos, '-')) return d;

    /* Parse month */
    d.month = parse_int(str, &pos, &ok);
    if (!ok) return d;

    /* Expect '-' */
    if (!expect_char(str, &pos, '-')) return d;

    /* Parse day */
    d.day = parse_int(str, &pos, &ok);
    if (!ok) return d;

    /* Optional time part */
    if (str[pos] == ' ' || str[pos] == 'T') {
        pos++;

        d.hour = parse_int(str, &pos, &ok);
        if (!ok) return d;

        if (!expect_char(str, &pos, ':')) return d;

        d.minute = parse_int(str, &pos, &ok);
        if (!ok) return d;

        /* Optional seconds */
        if (str[pos] == ':') {
            pos++;
            d.second = parse_int(str, &pos, &ok);
            if (!ok) return d;
        }
    }

    /* Validate */
    if (!date_validate(d.year, d.month, d.day, d.hour, d.minute, d.second))
        return d;

    d.valid = 1;
    return d;
}

static double date_to_jd(parsed_date_t d)
{
    if (!d.valid) return -1.0;

    double day_frac = (double)d.day
                    + (double)d.hour / 24.0
                    + (double)d.minute / 1440.0
                    + (double)d.second / 86400.0;

    return gregorian_to_jd(d.year, d.month, day_frac);
}

double date_parse_to_jd(const char *str)
{
    parsed_date_t d = date_parse(str);
    return date_to_jd(d);
}

void date_format_iso(double jd, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    int iday = (int)day;

    snprintf(buf, sz, "%04d-%02d-%02d", year, month, iday);
}

void date_format_full(double jd, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    int year, month;
    double day = jd_to_gregorian(jd, &year, &month);
    int iday = (int)day;

    double frac = day - (double)iday;
    double hours_f = frac * 24.0;
    int hour = (int)hours_f;
    double mins_f = (hours_f - (double)hour) * 60.0;
    int minute = (int)mins_f;
    double secs_f = (mins_f - (double)minute) * 60.0;
    int second = (int)(secs_f + 0.5); /* round */

    /* Handle carry from rounding */
    if (second >= 60) { second = 0; minute++; }
    if (minute >= 60) { minute = 0; hour++; }
    if (hour >= 24) { hour = 0; }

    snprintf(buf, sz, "%04d-%02d-%02d %02d:%02d:%02d",
             year, month, iday, hour, minute, second);
}
