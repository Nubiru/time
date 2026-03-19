/* islamic_interpret.h -- Islamic month & prayer interpretation data
 *
 * Static data for 12 Hijri months (4 sacred) and 5 daily prayers.
 * Composition function producing tiered interpretation text.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_ISLAMIC_INTERPRET_H
#define TIME_ISLAMIC_INTERPRET_H

#include "hijri.h"

/* Per-month interpretation data */
typedef struct {
    int month;                  /* 1-12 */
    const char *name;           /* Month name */
    int is_sacred;              /* 1 if sacred month (haram), 0 otherwise */
    const char *significance;   /* Key significance of this month */
    const char *event;          /* Major event/observance or "" if none */
    const char *brief;          /* One-sentence meaning */
} isi_month_t;

/* Per-prayer interpretation data */
typedef struct {
    int index;                  /* 0-4 (Fajr, Dhuhr, Asr, Maghrib, Isha) */
    const char *name;           /* Prayer name */
    const char *arabic;         /* Arabic transliteration */
    const char *time_desc;      /* When it occurs */
    const char *meaning;        /* Spiritual significance */
} isi_prayer_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[8];              /* T0: month number as string */
    char glance[128];           /* T1: "{year} {month} {day} -- {significance}" */
    char detail[512];           /* T3: Full narrative */
} islamic_interp_t;

/* Return interpretation data for a Hijri month (1-12).
 * Invalid month: returns struct with month=-1, all strings "?". */
isi_month_t isi_month_data(int month);

/* Return interpretation data for a daily prayer (0-4).
 * Invalid index: returns struct with index=-1, all strings "?". */
isi_prayer_t isi_prayer_data(int index);

/* Compose tiered interpretation from a Hijri date.
 * Glyph: month number. Glance: year + month + day + significance.
 * Detail: brief + sacred status + event if present. */
islamic_interp_t isi_interpret(hijri_date_t date);

/* Returns 12 (total number of Hijri months). */
int isi_month_count(void);

/* Returns 5 (total number of daily prayers). */
int isi_prayer_count(void);

#endif /* TIME_ISLAMIC_INTERPRET_H */
