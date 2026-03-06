/* fossil_milestones.h — Key milestones in Earth's biological history.
 * Static const data from stromatolites (3.5 Ga) through Homo sapiens (0.3 Ma).
 * Each milestone records what appeared, when, and why it mattered.
 * All pure functions, no side effects. */

#ifndef TIME_FOSSIL_MILESTONES_H
#define TIME_FOSSIL_MILESTONES_H

#include <stdbool.h>

/* Milestone category */
typedef enum {
    FOSSIL_CAT_ORIGIN = 0,       /* First appearance of life form */
    FOSSIL_CAT_INNOVATION,        /* Key evolutionary innovation */
    FOSSIL_CAT_EXTINCTION,        /* Mass extinction event */
    FOSSIL_CAT_COLONIZATION,      /* New habitat conquered */
    FOSSIL_CAT_DIVERSIFICATION    /* Rapid radiation/explosion */
} fossil_category_t;

typedef struct {
    int id;                       /* Unique index */
    const char *name;             /* Milestone name */
    double ma;                    /* Millions of years ago */
    const char *what_appeared;    /* What emerged or happened */
    const char *what_changed;     /* Why it mattered */
    const char *significance;     /* One-line impact statement */
    fossil_category_t category;   /* Event category */
} fossil_milestone_t;

/* Total number of milestones. */
int fossil_milestone_count(void);

/* Get milestone by index. Returns struct with id=-1 for invalid. */
fossil_milestone_t fossil_milestone_get(int index);

/* Get milestone name by index. Returns "?" for invalid. */
const char *fossil_milestone_name(int index);

/* Find milestone closest to a given time (Ma). */
fossil_milestone_t fossil_milestone_at(double ma);

/* Find milestones within a time range (Ma).
 * Writes matching indices to out_indices (up to max_results).
 * Returns number of matches found. */
int fossil_milestones_in_range(double start_ma, double end_ma,
                                int *out_indices, int max_results);

/* Count milestones of a given category. */
int fossil_milestones_of_category(fossil_category_t category);

/* Get category name string. */
const char *fossil_category_name(fossil_category_t category);

/* Is there a mass extinction at this time (within tolerance)? */
bool fossil_is_extinction(double ma, double tolerance_ma);

#endif /* TIME_FOSSIL_MILESTONES_H */
