/* personal_space.h -- Personal Space Data for the introvert layer.
 *
 * Manages user's private relationship with time: favorite systems,
 * tracked cycles, bookmarked dates, explored knowledge paths, and
 * a "cosmic fingerprint" archetype computed from birth data.
 *
 * Track 49.1 — introvert layer foundation.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_PERSONAL_SPACE_H
#define TIME_PERSONAL_SPACE_H

#include <stdio.h>

#define PS_MAX_FAVORITES 16
#define PS_MAX_CYCLES 16
#define PS_MAX_BOOKMARKS 32
#define PS_MAX_EXPLORED 32
#define PS_NOTE_MAX 128
#define PS_CYCLE_NAME_MAX 64

/* A tracked cycle the user watches */
typedef struct {
    char name[PS_CYCLE_NAME_MAX];    /* "My Saturn Return", "Next Lunar Eclipse" */
    int system_id;                    /* which knowledge system (-1 if cross-system) */
    double start_jd;                  /* cycle start (0 = unknown) */
    double end_jd;                    /* cycle end (0 = ongoing) */
    double next_event_jd;             /* next significant date in this cycle */
    int active;                       /* 1 = tracking, 0 = paused */
} ps_cycle_t;

/* A bookmarked moment in time */
typedef struct {
    double jd;                        /* the date */
    char note[PS_NOTE_MAX];           /* user's personal note */
    int system_id;                    /* associated system (-1 if general) */
} ps_bookmark_t;

/* Cosmic fingerprint — archetype from birth data */
typedef struct {
    int kin_seal;                     /* Dreamspell seal (0-19), -1 if no birth date */
    int kin_tone;                     /* Dreamspell tone (1-13), -1 if no birth date */
    int zodiac_sign;                  /* Sun sign (0-11), -1 if no birth date */
    int chinese_animal;               /* Chinese animal (0-11), -1 if no birth date */
    int hexagram;                     /* I Ching hexagram (1-64), 0 if no birth date */
    int archetype_score;              /* composite personality metric (0-100) */
} ps_archetype_t;

/* Complete personal space */
typedef struct {
    int favorite_systems[PS_MAX_FAVORITES]; /* system IDs, -1 terminated */
    int favorite_count;
    ps_cycle_t cycles[PS_MAX_CYCLES];
    int cycle_count;
    ps_bookmark_t bookmarks[PS_MAX_BOOKMARKS];
    int bookmark_count;
    int explored_systems[PS_MAX_EXPLORED];  /* system IDs visited */
    int explored_count;
    ps_archetype_t archetype;
} ps_space_t;

/* Create empty personal space. */
ps_space_t ps_create(void);

/* Add a favorite system. Returns modified space. */
ps_space_t ps_add_favorite(ps_space_t space, int system_id);

/* Remove a favorite system. Returns modified space. */
ps_space_t ps_remove_favorite(ps_space_t space, int system_id);

/* Check if a system is favorited. */
int ps_is_favorite(const ps_space_t *space, int system_id);

/* Add a tracked cycle. Returns modified space. */
ps_space_t ps_add_cycle(ps_space_t space, const char *name, int system_id,
                         double start_jd, double next_event_jd);

/* Remove a tracked cycle by index. Returns modified space. */
ps_space_t ps_remove_cycle(ps_space_t space, int index);

/* Pause/resume a tracked cycle. Returns modified space. */
ps_space_t ps_toggle_cycle(ps_space_t space, int index);

/* Bookmark a date with a note. Returns modified space. */
ps_space_t ps_bookmark(ps_space_t space, double jd, const char *note, int system_id);

/* Remove a bookmark by index. Returns modified space. */
ps_space_t ps_remove_bookmark(ps_space_t space, int index);

/* Record that a system was explored (for learning path). Returns modified space. */
ps_space_t ps_mark_explored(ps_space_t space, int system_id);

/* Check if a system has been explored. */
int ps_is_explored(const ps_space_t *space, int system_id);

/* Count of explored systems. */
int ps_explored_count(const ps_space_t *space);

/* Exploration progress (0.0-1.0, explored / total available systems). */
double ps_exploration_progress(const ps_space_t *space, int total_systems);

/* Compute archetype from birth data. */
ps_archetype_t ps_compute_archetype(int kin_seal, int kin_tone,
                                     int zodiac_sign, int chinese_animal,
                                     int hexagram);

/* Get active cycles (non-paused). Fills out array, returns count. */
int ps_active_cycles(const ps_space_t *space, ps_cycle_t *out, int max);

/* Get upcoming bookmarks within a JD range. Fills out, returns count. */
int ps_upcoming_bookmarks(const ps_space_t *space, double jd_start,
                           double jd_end, ps_bookmark_t *out, int max);

/* Summary: count of favorites, active cycles, bookmarks, explored. */
int ps_summary(const ps_space_t *space, char *buf, int buf_size);

#endif /* TIME_PERSONAL_SPACE_H */
