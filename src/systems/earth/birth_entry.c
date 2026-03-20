/* birth_entry.c — Birth date entry state machine.
 * Pure functions. No GL, no malloc, no globals, no side effects. */

#include "birth_entry.h"

#include <stdio.h>

/* --- Static data --- */

static const char *s_step_names[BE_STEP_COUNT] = {
    "Date",
    "Location",
    "Ready"
};

static const char *s_month_names[12] = {
    "January", "February", "March", "April",
    "May", "June", "July", "August",
    "September", "October", "November", "December"
};

static const int s_days_per_month[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/* --- Helpers --- */

static double clamp_d(double val, double lo, double hi) {
    if (val < lo) { return lo; }
    if (val > hi) { return hi; }
    return val;
}

static int clamp_day(int day, int year, int month) {
    int max = be_days_in_month(year, month);
    if (max == 0) { return day; }
    if (day > max) { return max; }
    if (day < 1) { return 1; }
    return day;
}

/* --- Public functions --- */

int be_is_leap_year(int year) {
    if (year % 400 == 0) { return 1; }
    if (year % 100 == 0) { return 0; }
    if (year % 4 == 0) { return 1; }
    return 0;
}

int be_days_in_month(int year, int month) {
    if (month < 1 || month > 12) { return 0; }
    if (month == 2 && be_is_leap_year(year)) { return 29; }
    return s_days_per_month[month - 1];
}

int be_date_valid(const be_date_t *date) {
    if (date->year < BE_YEAR_MIN || date->year > BE_YEAR_MAX) { return 0; }
    if (date->month < 1 || date->month > 12) { return 0; }
    if (date->day < 1) { return 0; }
    int max = be_days_in_month(date->year, date->month);
    if (date->day > max) { return 0; }
    return 1;
}

be_entry_t be_create(int today_year, int today_month, int today_day) {
    be_entry_t e;
    e.step = BE_STEP_DATE;
    e.date.year = today_year;
    e.date.month = today_month;
    e.date.day = today_day;
    e.date.focus = BE_FIELD_DAY;
    e.date.valid = be_date_valid(&e.date);
    e.location.latitude = 0.0;
    e.location.longitude = 0.0;
    e.location.set = 0;
    e.location.skipped = 0;
    return e;
}

be_entry_t be_focus(be_entry_t entry, be_field_t field) {
    if (field >= 0 && field < BE_FIELD_COUNT) {
        entry.date.focus = field;
    }
    return entry;
}

be_entry_t be_spin(be_entry_t entry, int delta) {
    switch (entry.date.focus) {
    case BE_FIELD_DAY: {
        int max = be_days_in_month(entry.date.year, entry.date.month);
        if (max == 0) { break; }
        int d = entry.date.day + delta;
        if (d > max) { d = 1; }
        if (d < 1) { d = max; }
        entry.date.day = d;
        break;
    }
    case BE_FIELD_MONTH: {
        int m = entry.date.month + delta;
        if (m > 12) { m = 1; }
        if (m < 1) { m = 12; }
        entry.date.month = m;
        entry.date.day = clamp_day(entry.date.day, entry.date.year, m);
        break;
    }
    case BE_FIELD_YEAR: {
        int y = entry.date.year + delta;
        if (y > BE_YEAR_MAX) { y = BE_YEAR_MAX; }
        if (y < BE_YEAR_MIN) { y = BE_YEAR_MIN; }
        entry.date.year = y;
        entry.date.day = clamp_day(entry.date.day, y, entry.date.month);
        break;
    }
    default:
        break;
    }
    entry.date.valid = be_date_valid(&entry.date);
    return entry;
}

be_entry_t be_set_date(be_entry_t entry, int year, int month, int day) {
    entry.date.year = year;
    entry.date.month = month;
    entry.date.day = day;
    entry.date.valid = be_date_valid(&entry.date);
    return entry;
}

be_entry_t be_confirm_date(be_entry_t entry) {
    if (entry.step != BE_STEP_DATE) { return entry; }
    if (!entry.date.valid) { return entry; }
    entry.step = BE_STEP_LOCATION;
    return entry;
}

be_entry_t be_set_location(be_entry_t entry, double lat, double lon) {
    if (entry.step != BE_STEP_LOCATION) { return entry; }
    entry.location.latitude = clamp_d(lat, -90.0, 90.0);
    entry.location.longitude = clamp_d(lon, -180.0, 180.0);
    entry.location.set = 1;
    entry.step = BE_STEP_READY;
    return entry;
}

be_entry_t be_skip_location(be_entry_t entry) {
    if (entry.step != BE_STEP_LOCATION) { return entry; }
    entry.location.skipped = 1;
    entry.step = BE_STEP_READY;
    return entry;
}

be_entry_t be_back(be_entry_t entry) {
    switch (entry.step) {
    case BE_STEP_LOCATION:
        entry.step = BE_STEP_DATE;
        break;
    case BE_STEP_READY:
        entry.location.set = 0;
        entry.location.skipped = 0;
        entry.step = BE_STEP_LOCATION;
        break;
    default:
        break;
    }
    return entry;
}

const char *be_step_name(be_step_t step) {
    if (step < 0 || step >= BE_STEP_COUNT) {
        return "Unknown";
    }
    return s_step_names[step];
}

int be_is_ready(const be_entry_t *entry) {
    return entry->step == BE_STEP_READY ? 1 : 0;
}

int be_format_date(const be_entry_t *entry, char *buf, int buf_size) {
    if (entry->date.month < 1 || entry->date.month > 12) { return 0; }
    const char *month_name = s_month_names[entry->date.month - 1];
    int needed = snprintf(buf, (unsigned)buf_size, "%02d %s %d",
                          entry->date.day, month_name, entry->date.year);
    if (needed < 0 || needed >= buf_size) {
        if (buf_size > 0) { buf[0] = '\0'; }
        return 0;
    }
    return needed;
}
