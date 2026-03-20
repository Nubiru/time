/* stored_convergences.c — Notable convergence date database
 *
 * Static list of historically significant dates with runtime
 * convergence computation via cd_scan.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "stored_convergences.h"
#include "../../math/julian.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

/* ===================================================================
 * Notable date definitions
 * =================================================================== */

typedef struct {
    int         year, month, day;
    const char *name;
    const char *category;
} sc_date_def_t;

static const sc_date_def_t DATE_LIST[] = {
    /* Historical milestones */
    { 1969,  7, 20, "Moon Landing",                     "historical" },
    { 1989, 11,  9, "Fall of the Berlin Wall",          "historical" },
    { 2001,  9, 11, "September 11",                     "historical" },
    { 2012, 12, 21, "Mayan 13.0.0.0.0",                "historical" },
    { 1945,  8,  6, "Hiroshima",                        "historical" },
    { 1945,  5,  8, "V-E Day",                          "historical" },
    { 1963, 11, 22, "JFK Assassination",                "historical" },
    { 1865,  4, 14, "Lincoln Assassination",            "historical" },
    { 1776,  7,  4, "US Declaration of Independence",   "historical" },
    { 1789,  7, 14, "French Revolution (Bastille Day)", "historical" },
    { 1492, 10, 12, "Columbus reaches Americas",        "historical" },
    { 1999, 12, 31, "Eve of the Millennium",            "historical" },
    { 2000,  1,  1, "Millennium Day",                   "historical" },

    /* 2026 equinoxes and solstices */
    { 2026,  3, 20, "Spring Equinox 2026",              "2026" },
    { 2026,  6, 21, "Summer Solstice 2026",             "2026" },
    { 2026,  9, 22, "Autumn Equinox 2026",              "2026" },
    { 2026, 12, 21, "Winter Solstice 2026",             "2026" },

    /* 2026 new moons (approximate) */
    { 2026,  1, 18, "New Moon Jan 2026",                "2026" },
    { 2026,  2, 17, "New Moon Feb 2026",                "2026" },
    { 2026,  3, 19, "New Moon Mar 2026",                "2026" },
    { 2026,  4, 17, "New Moon Apr 2026",                "2026" },
    { 2026,  5, 16, "New Moon May 2026",                "2026" },
    { 2026,  6, 15, "New Moon Jun 2026",                "2026" },
    { 2026,  7, 14, "New Moon Jul 2026",                "2026" },
    { 2026,  8, 12, "New Moon Aug 2026",                "2026" },
    { 2026,  9, 11, "New Moon Sep 2026",                "2026" },
    { 2026, 10, 10, "New Moon Oct 2026",                "2026" },
    { 2026, 11,  9, "New Moon Nov 2026",                "2026" },
    { 2026, 12,  9, "New Moon Dec 2026",                "2026" },

    /* 2026 full moons (approximate) */
    { 2026,  1,  3, "Full Moon Jan 2026",               "2026" },
    { 2026,  2,  1, "Full Moon Feb 2026",               "2026" },
    { 2026,  3,  3, "Full Moon Mar 2026",               "2026" },
    { 2026,  4,  1, "Full Moon Apr 2026",               "2026" },
    { 2026,  5,  1, "Full Moon May 2026",               "2026" },
    { 2026,  5, 31, "Full Moon May (2nd) 2026",         "2026" },
    { 2026,  6, 29, "Full Moon Jun 2026",               "2026" },
    { 2026,  7, 29, "Full Moon Jul 2026",               "2026" },
    { 2026,  8, 28, "Full Moon Aug 2026",               "2026" },
    { 2026,  9, 26, "Full Moon Sep 2026",               "2026" },
    { 2026, 10, 26, "Full Moon Oct 2026",               "2026" },
    { 2026, 11, 24, "Full Moon Nov 2026",               "2026" },
    { 2026, 12, 24, "Full Moon Dec 2026",               "2026" },

    /* Astronomical events */
    { 1066, 10, 14, "Battle of Hastings (Halley's Comet)", "astronomical" },
    { 1054,  7,  4, "Crab Nebula supernova recorded",       "astronomical" },
};

static const int DATE_COUNT = (int)(sizeof(DATE_LIST) / sizeof(DATE_LIST[0]));

/* ===================================================================
 * Public API
 * =================================================================== */

int sc_list(sc_result_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));

    int count = DATE_COUNT;
    if (count > SC_MAX_ENTRIES) count = SC_MAX_ENTRIES;

    for (int i = 0; i < count; i++) {
        const sc_date_def_t *d = &DATE_LIST[i];
        out->entries[i].jd = gregorian_to_jd(d->year, d->month, (double)d->day);
        out->entries[i].name = d->name;
        out->entries[i].category = d->category;
        out->entries[i].system_count = 0;
        out->entries[i].strength = 0;
        out->entries[i].significance = 0.0;
    }

    out->count = count;
    return count;
}

int sc_compute(sc_result_t *out) {
    if (!out) return 0;

    for (int i = 0; i < out->count; i++) {
        cd_result_t cd = cd_scan(out->entries[i].jd);
        if (cd.count > 0) {
            const cd_event_t *ev = &cd.events[0];
            out->entries[i].system_count = ev->system_count;
            out->entries[i].strength = ev->strength;
            out->entries[i].significance = ev->significance;
            /* Store system IDs */
            int sc = ev->system_count;
            if (sc > SC_MAX_SYSTEMS) sc = SC_MAX_SYSTEMS;
            for (int s = 0; s < sc; s++) {
                out->entries[i].systems[s] = (int)ev->systems[s];
            }
        }
    }
    return out->count;
}

const sc_entry_t *sc_nearest(const sc_result_t *result, double jd) {
    if (!result || result->count == 0) return NULL;

    int best_idx = 0;
    double best_dist = fabs(result->entries[0].jd - jd);

    for (int i = 1; i < result->count; i++) {
        double dist = fabs(result->entries[i].jd - jd);
        if (dist < best_dist) {
            best_dist = dist;
            best_idx = i;
        }
    }
    return &result->entries[best_idx];
}

int sc_count_above(const sc_result_t *result, int min_systems) {
    if (!result) return 0;
    int count = 0;
    for (int i = 0; i < result->count; i++) {
        if (result->entries[i].system_count >= min_systems) count++;
    }
    return count;
}

int sc_filter_category(const sc_result_t *result, const char *category,
                       sc_entry_t *out, int max_out) {
    if (!result || !category || !out || max_out <= 0) return 0;

    int found = 0;
    for (int i = 0; i < result->count && found < max_out; i++) {
        if (result->entries[i].category &&
            strcmp(result->entries[i].category, category) == 0) {
            out[found++] = result->entries[i];
        }
    }
    return found;
}

int sc_narrative(const sc_entry_t *entry, char *buf, int buf_size) {
    if (!entry || !buf || buf_size <= 0) return 0;
    buf[0] = '\0';

    if (entry->system_count == 0) {
        return snprintf(buf, (size_t)buf_size, "%s: a quiet day", entry->name);
    }

    /* Build: "Moon Landing: Astronomy, Islamic, and Buddhist converge" */
    int pos = 0;
    int remaining = buf_size;

    int n = snprintf(buf, (size_t)remaining, "%s: ", entry->name);
    if (n > 0) { int u = n < remaining ? n : remaining - 1; pos += u; remaining -= u; }

    int sc = entry->system_count;
    if (sc > SC_MAX_SYSTEMS) sc = SC_MAX_SYSTEMS;

    for (int i = 0; i < sc && remaining > 1; i++) {
        const char *sname = cd_system_name((cd_system_t)entry->systems[i]);
        if (i == 0) {
            n = snprintf(buf + pos, (size_t)remaining, "%s", sname);
        } else if (i == sc - 1) {
            n = snprintf(buf + pos, (size_t)remaining,
                         sc == 2 ? " and %s" : ", and %s", sname);
        } else {
            n = snprintf(buf + pos, (size_t)remaining, ", %s", sname);
        }
        if (n > 0) { int u = n < remaining ? n : remaining - 1; pos += u; remaining -= u; }
    }

    n = snprintf(buf + pos, (size_t)remaining, " converge");
    if (n > 0) { int u = n < remaining ? n : remaining - 1; pos += u; }

    return pos;
}
