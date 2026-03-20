/* moment_snapshot.h — Portable summary of all system states at a given Julian Day.
 *
 * A "time photograph": captures what every system says about a single
 * instant, for bookmarks and moment comparison.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_MOMENT_SNAPSHOT_H
#define TIME_MOMENT_SNAPSHOT_H

#define MS_MAX_SYSTEMS   16
#define MS_NAME_MAX      32
#define MS_VALUE_MAX     64
#define MS_DETAIL_MAX    128
#define MS_NOTE_MAX      128
#define MS_SUMMARY_MAX   1024

/* A single system's state at the snapshot moment */
typedef struct {
    int system_id;                    /* 0-15 */
    char system_name[MS_NAME_MAX];    /* "Tzolkin", "Astrology", etc. */
    char primary[MS_VALUE_MAX];       /* "Kin 196 Yellow Rhythmic Warrior" */
    char secondary[MS_DETAIL_MAX];    /* "Wavespell 15, Castle 4, Tone 6" */
    int available;                    /* 1 if this system has data */
} ms_system_state_t;

/* Complete moment snapshot */
typedef struct {
    double jd;                        /* Julian Day of the snapshot */
    int year, month, day;             /* Gregorian date */
    int hour, minute;                 /* Time (0 if date-only) */
    char note[MS_NOTE_MAX];           /* User's personal note */
    ms_system_state_t systems[MS_MAX_SYSTEMS];
    int system_count;                 /* Number of populated systems */
    int available_count;              /* Number with available=1 */
} ms_snapshot_t;

/* Pre-computed system values for the snapshot moment */
typedef struct {
    double jd;
    int year, month, day;
    int hour, minute;

    /* Gregorian */
    const char *weekday;              /* "Wednesday", NULL if unknown */
    int day_of_year;                  /* 1-366 */

    /* Tzolkin */
    int kin;                          /* 1-260, 0 = unknown */
    int seal;                         /* 0-19, -1 = unknown */
    int tone;                         /* 1-13, 0 = unknown */
    const char *seal_name;            /* NULL if unknown */
    const char *tone_name;            /* NULL if unknown */
    int wavespell;                    /* 1-20, 0 = unknown */

    /* Astrology */
    int sun_sign;                     /* 0-11, -1 = unknown */
    const char *sun_sign_name;        /* NULL if unknown */

    /* Chinese */
    int animal;                       /* 0-11, -1 = unknown */
    int element;                      /* 0-4, -1 = unknown */
    const char *animal_name;          /* NULL if unknown */
    const char *element_name;         /* NULL if unknown */

    /* I Ching */
    int hexagram;                     /* 1-64, 0 = unknown */
    const char *hexagram_name;        /* NULL if unknown */

    /* Hebrew */
    int hebrew_year;                  /* 0 = unknown */
    int hebrew_month;                 /* 0 = unknown */
    int hebrew_day;                   /* 0 = unknown */
    const char *hebrew_month_name;    /* NULL if unknown */

    /* Islamic */
    int islamic_year;                 /* 0 = unknown */
    int islamic_month;                /* 0 = unknown */
    int islamic_day;                  /* 0 = unknown */
    const char *islamic_month_name;   /* NULL if unknown */

    /* Buddhist */
    int buddhist_year;                /* 0 = unknown */

    /* Moon */
    const char *moon_phase;           /* "Full Moon", etc., NULL if unknown */

    /* User note */
    const char *note;                 /* NULL for empty note */
} ms_input_t;

/* Create default (zeroed/unknown) input. */
ms_input_t ms_default_input(void);

/* Capture a snapshot from pre-computed input.
 * Populates systems that have data, skips unknown ones.
 * Returns empty snapshot if input is NULL. */
ms_snapshot_t ms_capture(const ms_input_t *input);

/* Get a system state by index. Returns NULL if out of range. */
const ms_system_state_t *ms_system(const ms_snapshot_t *snap, int index);

/* Get a system state by system_id. Returns NULL if not found. */
const ms_system_state_t *ms_find_system(const ms_snapshot_t *snap, int system_id);

/* Count available (populated) systems in the snapshot. */
int ms_available_count(const ms_snapshot_t *snap);

/* Format snapshot as multi-line text.
 * Returns bytes written (excluding NUL). */
int ms_format(const ms_snapshot_t *snap, char *buf, int buf_size);

/* Format a compact one-line summary: "2026-03-20 Kin 196 · Pisces · Water Rabbit"
 * Returns bytes written (excluding NUL). */
int ms_format_compact(const ms_snapshot_t *snap, char *buf, int buf_size);

/* Compare two snapshots. Returns number of systems that differ. */
int ms_diff_count(const ms_snapshot_t *a, const ms_snapshot_t *b);

#endif /* TIME_MOMENT_SNAPSHOT_H */
