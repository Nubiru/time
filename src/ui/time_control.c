#include <stdio.h>
#include <string.h>

#include "time_control.h"

/* --- Speed Presets --- */

static const tc_speed_t s_presets[] = {
    { "Paused",      "0", 0.0,     0 },
    { "Real-time",   "1", 1.0,     1 },
    { "Fast",        "2", 60.0,    2 },
    { "Faster",      "3", 3600.0,  3 },
    { "Day/sec",     "4", 86400.0, 4 },
    { "10 Days/sec", "5", 864000.0, 5 }
};

static const int s_preset_count = (int)(sizeof(s_presets) / sizeof(s_presets[0]));

/* --- Internal: JD to date (Meeus algorithm, self-contained) --- */

static void jd_to_ymd(double jd, int *y, int *m, int *d)
{
    int z = (int)(jd + 0.5);
    int a = z;
    if (z >= 2299161) {
        int alpha = (int)((z - 1867216.25) / 36524.25);
        a = z + 1 + alpha - alpha / 4;
    }
    int b = a + 1524;
    int c = (int)((b - 122.1) / 365.25);
    int dd = (int)(365.25 * c);
    int e = (int)((b - dd) / 30.6001);
    *d = b - dd - (int)(30.6001 * e);
    *m = (e < 14) ? e - 1 : e - 13;
    *y = (*m > 2) ? c - 4716 : c - 4715;
}

static void jd_to_hms(double jd, int *h, int *min, int *sec)
{
    double frac = jd + 0.5;
    frac = frac - (int)frac;
    int total_seconds = (int)(frac * 86400.0 + 0.5);
    *h = total_seconds / 3600;
    *min = (total_seconds % 3600) / 60;
    *sec = total_seconds % 60;
}

static const char *day_of_week_name(double jd)
{
    /* JD 0.0 (noon Jan 1, 4713 BC) was a Monday.
     * Julian Day Number = (int)(jd + 0.5).
     * JDN mod 7: 0=Mon, 1=Tue, ..., 5=Sat, 6=Sun. */
    static const char *names[] = {
        "Monday", "Tuesday", "Wednesday", "Thursday",
        "Friday", "Saturday", "Sunday"
    };
    int jdn = (int)(jd + 0.5);
    int dow = jdn % 7;
    if (dow < 0) dow += 7;
    return names[dow];
}

/* --- Speed Presets API --- */

int tc_speed_count(void)
{
    return s_preset_count;
}

tc_speed_t tc_speed_get(int index)
{
    if (index < 0 || index >= s_preset_count) {
        tc_speed_t empty = { "Unknown", "", 0.0, -1 };
        return empty;
    }
    return s_presets[index];
}

int tc_speed_find_by_key(int key_index)
{
    for (int i = 0; i < s_preset_count; i++) {
        if (s_presets[i].key_index == key_index) {
            return i;
        }
    }
    return -1;
}

const char *tc_speed_name(double multiplier)
{
    for (int i = 0; i < s_preset_count; i++) {
        double diff = s_presets[i].multiplier - multiplier;
        if (diff < 0.0001 && diff > -0.0001) {
            return s_presets[i].name;
        }
    }
    return "Custom";
}

/* --- State Management --- */

tc_state_t tc_state_init(double current_jd)
{
    tc_state_t s;
    s.jd = current_jd;
    s.speed = 1.0;
    s.direction = TC_DIR_FORWARD;
    s.speed_index = 1;  /* Real-time */
    s.is_live = 1;
    s.saved_speed = 1.0;
    s.saved_direction = TC_DIR_FORWARD;
    return s;
}

tc_state_t tc_set_speed(tc_state_t state, int preset_index)
{
    if (preset_index < 0 || preset_index >= s_preset_count) {
        return state;
    }

    state.speed = s_presets[preset_index].multiplier;
    state.speed_index = preset_index;

    /* Preset 0 = pause */
    if (preset_index == 0) {
        state.direction = TC_DIR_PAUSED;
    } else if (state.direction == TC_DIR_PAUSED) {
        /* Resuming from pause via preset: go forward */
        state.direction = TC_DIR_FORWARD;
    }

    /* Changing speed away from real-time or pausing means not live */
    if (preset_index != 1) {
        state.is_live = 0;
    }

    return state;
}

tc_state_t tc_set_speed_by_key(tc_state_t state, int key_index)
{
    int idx = tc_speed_find_by_key(key_index);
    if (idx < 0) {
        return state;
    }
    return tc_set_speed(state, idx);
}

tc_state_t tc_toggle_pause(tc_state_t state)
{
    if (state.direction == TC_DIR_PAUSED) {
        /* Resume: restore saved speed and direction */
        state.speed = state.saved_speed;
        state.direction = state.saved_direction;
        /* Recalculate speed_index */
        state.speed_index = -1;
        for (int i = 0; i < s_preset_count; i++) {
            double diff = s_presets[i].multiplier - state.speed;
            if (diff < 0.0001 && diff > -0.0001) {
                state.speed_index = i;
                break;
            }
        }
    } else {
        /* Pause: save current speed and direction */
        state.saved_speed = state.speed;
        state.saved_direction = state.direction;
        state.speed = 0.0;
        state.direction = TC_DIR_PAUSED;
        state.speed_index = 0;
    }
    return state;
}

tc_state_t tc_reverse(tc_state_t state)
{
    if (state.direction == TC_DIR_FORWARD) {
        state.direction = TC_DIR_REVERSE;
    } else if (state.direction == TC_DIR_REVERSE) {
        state.direction = TC_DIR_FORWARD;
    } else {
        /* Paused: set saved direction to reverse, stay paused */
        state.saved_direction = (state.saved_direction == TC_DIR_FORWARD)
                                ? TC_DIR_REVERSE : TC_DIR_FORWARD;
    }
    state.is_live = 0;
    return state;
}

tc_state_t tc_go_live(tc_state_t state, double current_jd)
{
    state.jd = current_jd;
    state.speed = 1.0;
    state.direction = TC_DIR_FORWARD;
    state.speed_index = 1;
    state.is_live = 1;
    state.saved_speed = 1.0;
    state.saved_direction = TC_DIR_FORWARD;
    return state;
}

tc_state_t tc_step(tc_state_t state, double days)
{
    state.jd += days;
    state.is_live = 0;
    return state;
}

tc_state_t tc_advance(tc_state_t state, double dt_seconds)
{
    if (state.is_live) {
        return state;
    }
    if (state.direction == TC_DIR_PAUSED) {
        return state;
    }
    state.jd += (dt_seconds * state.speed * (double)state.direction) / 86400.0;
    return state;
}

/* --- Display Formatting --- */

tc_display_t tc_format(tc_state_t state)
{
    tc_display_t d;
    memset(&d, 0, sizeof(d));

    /* Date and time from JD */
    int y, m, day;
    jd_to_ymd(state.jd, &y, &m, &day);

    int h, min, sec;
    jd_to_hms(state.jd, &h, &min, &sec);

    snprintf(d.date, TC_DATE_BUF_SIZE, "%04d-%02d-%02d", y, m, day);
    snprintf(d.time, TC_DATE_BUF_SIZE, "%02d:%02d:%02d", h, min, sec);

    /* Day name */
    const char *dn = day_of_week_name(state.jd);
    snprintf(d.day_name, sizeof(d.day_name), "%s", dn);

    /* Speed display */
    tc_format_speed(state, d.speed_display, (int)sizeof(d.speed_display));

    /* Direction icon */
    const char *icon = tc_direction_icon(state.direction);
    snprintf(d.direction_icon, sizeof(d.direction_icon), "%s", icon);

    return d;
}

int tc_format_speed(tc_state_t state, char *buf, int buf_size)
{
    if (buf_size <= 0) {
        return 0;
    }

    if (state.direction == TC_DIR_PAUSED) {
        return snprintf(buf, (unsigned long)buf_size, "PAUSED");
    }

    if (state.is_live && state.speed > 0.9999 && state.speed < 1.0001) {
        return snprintf(buf, (unsigned long)buf_size, "Real-time");
    }

    double effective = state.speed * (double)state.direction;

    /* Show as integer if close to whole number */
    if (effective > -0.0001 && effective < 0.0001) {
        return snprintf(buf, (unsigned long)buf_size, "0x");
    }

    int ieff = (int)effective;
    double diff = effective - (double)ieff;
    if (diff < 0.001 && diff > -0.001) {
        return snprintf(buf, (unsigned long)buf_size, "%dx", ieff);
    }

    return snprintf(buf, (unsigned long)buf_size, "%.1fx", effective);
}

const char *tc_direction_icon(tc_direction_t dir)
{
    switch (dir) {
    case TC_DIR_FORWARD: return ">";
    case TC_DIR_PAUSED:  return "||";
    case TC_DIR_REVERSE: return "<";
    }
    return "?";
}

/* --- Query --- */

int tc_is_paused(tc_state_t state)
{
    return state.direction == TC_DIR_PAUSED ? 1 : 0;
}

int tc_is_reverse(tc_state_t state)
{
    return state.direction == TC_DIR_REVERSE ? 1 : 0;
}

int tc_is_live(tc_state_t state)
{
    return state.is_live;
}
