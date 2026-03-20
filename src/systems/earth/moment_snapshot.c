/* moment_snapshot.c — Portable summary of all system states at a given Julian Day.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "moment_snapshot.h"

#include <stdio.h>
#include <string.h>

/* --- Helpers --- */

static void safe_copy(char *dst, const char *src, int max_len) {
    int i = 0;
    while (i < max_len - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

static const char *month_name(int month) {
    static const char *names[] = {
        "", "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    if (month < 1 || month > 12) return "";
    return names[month];
}

/* Add a system to the snapshot. Returns pointer to the new entry, or NULL if full. */
static ms_system_state_t *add_system(ms_snapshot_t *snap, int system_id,
                                     const char *name) {
    if (snap->system_count >= MS_MAX_SYSTEMS) return NULL;
    ms_system_state_t *sys = &snap->systems[snap->system_count];
    memset(sys, 0, sizeof(*sys));
    sys->system_id = system_id;
    safe_copy(sys->system_name, name, MS_NAME_MAX);
    sys->available = 1;
    snap->system_count++;
    snap->available_count++;
    return sys;
}

/* --- snprintf wrapper that returns bytes consumed (clamped to buf_size-1) --- */

static int fmt_append(char *buf, int buf_size, int offset, const char *fmt, ...) {
    if (offset >= buf_size) return offset;
    int remaining = buf_size - offset;

    /* va_list dance */
    __builtin_va_list ap;
    __builtin_va_start(ap, fmt);
    int written = vsnprintf(buf + offset, (size_t)remaining, fmt, ap);
    __builtin_va_end(ap);

    if (written < 0) return offset;
    if (written >= remaining) return buf_size - 1;
    return offset + written;
}

/* --- Public API --- */

ms_input_t ms_default_input(void) {
    ms_input_t in;
    memset(&in, 0, sizeof(in));
    in.seal = -1;
    in.sun_sign = -1;
    in.animal = -1;
    in.element = -1;
    return in;
}

ms_snapshot_t ms_capture(const ms_input_t *input) {
    ms_snapshot_t snap;
    memset(&snap, 0, sizeof(snap));

    if (!input) return snap;

    snap.jd = input->jd;
    snap.year = input->year;
    snap.month = input->month;
    snap.day = input->day;
    snap.hour = input->hour;
    snap.minute = input->minute;

    if (input->note) {
        safe_copy(snap.note, input->note, MS_NOTE_MAX);
    }

    /* 0: Gregorian — requires weekday and valid date */
    if (input->weekday && input->year > 0) {
        ms_system_state_t *sys = add_system(&snap, 0, "Gregorian");
        if (sys) {
            snprintf(sys->primary, MS_VALUE_MAX, "%s, %s %d, %d",
                     input->weekday, month_name(input->month),
                     input->day, input->year);
            snprintf(sys->secondary, MS_DETAIL_MAX, "Day %d of 365",
                     input->day_of_year);
        }
    }

    /* 1: Tzolkin — requires kin > 0 */
    if (input->kin > 0 && input->seal_name && input->tone_name) {
        ms_system_state_t *sys = add_system(&snap, 1, "Tzolkin");
        if (sys) {
            snprintf(sys->primary, MS_VALUE_MAX, "Kin %d %s %s",
                     input->kin, input->tone_name, input->seal_name);
            snprintf(sys->secondary, MS_DETAIL_MAX, "Wavespell %d, Tone %d",
                     input->wavespell, input->tone);
        }
    }

    /* 2: Astrology — requires sun_sign_name */
    if (input->sun_sign_name) {
        ms_system_state_t *sys = add_system(&snap, 2, "Astrology");
        if (sys) {
            safe_copy(sys->primary, input->sun_sign_name, MS_VALUE_MAX);
        }
    }

    /* 3: Chinese — requires element_name and animal_name */
    if (input->element_name && input->animal_name) {
        ms_system_state_t *sys = add_system(&snap, 3, "Chinese");
        if (sys) {
            snprintf(sys->primary, MS_VALUE_MAX, "%s %s",
                     input->element_name, input->animal_name);
        }
    }

    /* 4: I Ching — requires hexagram > 0 and name */
    if (input->hexagram > 0 && input->hexagram_name) {
        ms_system_state_t *sys = add_system(&snap, 4, "I Ching");
        if (sys) {
            snprintf(sys->primary, MS_VALUE_MAX, "Hexagram %d: %s",
                     input->hexagram, input->hexagram_name);
        }
    }

    /* 5: Hebrew — requires hebrew_year > 0 and month_name */
    if (input->hebrew_year > 0 && input->hebrew_month_name) {
        ms_system_state_t *sys = add_system(&snap, 5, "Hebrew");
        if (sys) {
            snprintf(sys->primary, MS_VALUE_MAX, "%s %d, %d",
                     input->hebrew_month_name, input->hebrew_day,
                     input->hebrew_year);
        }
    }

    /* 6: Islamic — requires islamic_year > 0 and month_name */
    if (input->islamic_year > 0 && input->islamic_month_name) {
        ms_system_state_t *sys = add_system(&snap, 6, "Islamic");
        if (sys) {
            snprintf(sys->primary, MS_VALUE_MAX, "%s %d, %d",
                     input->islamic_month_name, input->islamic_day,
                     input->islamic_year);
        }
    }

    /* 7: Buddhist — requires buddhist_year > 0 */
    if (input->buddhist_year > 0) {
        ms_system_state_t *sys = add_system(&snap, 7, "Buddhist");
        if (sys) {
            snprintf(sys->primary, MS_VALUE_MAX, "BE %d",
                     input->buddhist_year);
        }
    }

    /* 8: Moon — requires moon_phase */
    if (input->moon_phase) {
        ms_system_state_t *sys = add_system(&snap, 8, "Moon");
        if (sys) {
            safe_copy(sys->primary, input->moon_phase, MS_VALUE_MAX);
        }
    }

    return snap;
}

const ms_system_state_t *ms_system(const ms_snapshot_t *snap, int index) {
    if (index < 0 || index >= snap->system_count) return NULL;
    return &snap->systems[index];
}

const ms_system_state_t *ms_find_system(const ms_snapshot_t *snap, int system_id) {
    for (int i = 0; i < snap->system_count; i++) {
        if (snap->systems[i].system_id == system_id) {
            return &snap->systems[i];
        }
    }
    return NULL;
}

int ms_available_count(const ms_snapshot_t *snap) {
    return snap->available_count;
}

int ms_format(const ms_snapshot_t *snap, char *buf, int buf_size) {
    if (buf_size <= 0) return 0;

    int pos = 0;

    /* Date header */
    pos = fmt_append(buf, buf_size, pos, "%04d-%02d-%02d",
                     snap->year, snap->month, snap->day);
    if (snap->hour > 0 || snap->minute > 0) {
        pos = fmt_append(buf, buf_size, pos, " %02d:%02d",
                         snap->hour, snap->minute);
    }
    pos = fmt_append(buf, buf_size, pos, "\n");

    /* Systems */
    for (int i = 0; i < snap->system_count; i++) {
        const ms_system_state_t *sys = &snap->systems[i];
        pos = fmt_append(buf, buf_size, pos, "%s: %s",
                         sys->system_name, sys->primary);
        if (sys->secondary[0] != '\0') {
            pos = fmt_append(buf, buf_size, pos, " (%s)", sys->secondary);
        }
        pos = fmt_append(buf, buf_size, pos, "\n");
    }

    /* Note */
    if (snap->note[0] != '\0') {
        pos = fmt_append(buf, buf_size, pos, "\nNote: %s\n", snap->note);
    }

    return pos;
}

int ms_format_compact(const ms_snapshot_t *snap, char *buf, int buf_size) {
    if (buf_size <= 0) return 0;

    int pos = 0;

    /* Date */
    pos = fmt_append(buf, buf_size, pos, "%04d-%02d-%02d",
                     snap->year, snap->month, snap->day);

    /* Key systems separated by middle dot */
    for (int i = 0; i < snap->system_count; i++) {
        const ms_system_state_t *sys = &snap->systems[i];
        pos = fmt_append(buf, buf_size, pos, " \xc2\xb7 %s", sys->primary);
    }

    return pos;
}

int ms_diff_count(const ms_snapshot_t *a, const ms_snapshot_t *b) {
    int diffs = 0;

    /* Check each system_id that appears in either snapshot */
    for (int id = 0; id < MS_MAX_SYSTEMS; id++) {
        const ms_system_state_t *sa = ms_find_system(a, id);
        const ms_system_state_t *sb = ms_find_system(b, id);

        /* Both absent: no diff */
        if (!sa && !sb) continue;

        /* One present, one absent: diff */
        if (!sa || !sb) {
            diffs++;
            continue;
        }

        /* Both present: compare primary text */
        if (strcmp(sa->primary, sb->primary) != 0) {
            diffs++;
        }
    }

    return diffs;
}
