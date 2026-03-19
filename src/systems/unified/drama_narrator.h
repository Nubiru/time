/* drama_narrator.h — Dramatization narrative for historical data modules.
 * Provides pre-authored dramatic voice for achievement, calendar_reform,
 * and earth_drama data. Each entry has a hook (opening line) and body
 * (full dramatic narration) that brings raw data to life.
 *
 * Pure lookup module: static const data, no malloc, no globals, no side effects.
 *
 * Sources: achievement.h (17), calendar_reform.h (10), earth_drama.h (12). */

#ifndef TIME_DRAMA_NARRATOR_H
#define TIME_DRAMA_NARRATOR_H

/* Source data module types */
typedef enum {
    DRAMA_SRC_ACHIEVEMENT = 0,  /* Historical astronomical achievements */
    DRAMA_SRC_CALENDAR,         /* Calendar systems and reforms */
    DRAMA_SRC_EARTH,            /* Earth drama geological periods */
    DRAMA_SRC_COUNT
} drama_source_t;

/* A single dramatized text entry */
typedef struct {
    int id;                  /* Global index (0 to count-1) */
    drama_source_t source;   /* Which data module */
    int source_index;        /* Index into source data module */
    const char *hook;        /* Opening dramatic line (1 sentence) */
    const char *body;        /* Full dramatic narration (paragraph) */
} drama_entry_t;

/* Total dramatization count (all sources). */
int drama_narrator_count(void);

/* Get by global index. Returns entry with id=-1 if invalid. */
drama_entry_t drama_narrator_get(int index);

/* Get by source type and source index. Returns entry with id=-1 if not found. */
drama_entry_t drama_narrator_for(drama_source_t source, int source_index);

/* Count entries for a given source type. Returns 0 for invalid type. */
int drama_narrator_count_by_source(drama_source_t source);

/* Get entries by source type. Fills out_indices (max out_max). Returns count. */
int drama_narrator_by_source(drama_source_t source, int *out_indices, int out_max);

/* Source type name string. Returns "?" for invalid. */
const char *drama_source_name(drama_source_t source);

#endif /* TIME_DRAMA_NARRATOR_H */
