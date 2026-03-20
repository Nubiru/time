/* fossil_narrative.h — Display-ready narrative arcs for life on Earth.
 * Organizes biological history into 8 ages with rich narrative text,
 * key species, and rise/fall stories. The dinosaur timeline gets special
 * treatment with 10 individual milestone events.
 * All pure functions, no side effects. */

#ifndef TIME_FOSSIL_NARRATIVE_H
#define TIME_FOSSIL_NARRATIVE_H

#define FN_ERA_COUNT       8
#define FN_DINO_EVENT_COUNT 10

typedef struct {
    int index;
    const char *name;            /* "Age of Reptiles" */
    double start_ma;             /* 252.0 */
    double end_ma;               /* 66.0 */
    const char *narrative;       /* Rich story paragraph */
    const char *key_species;     /* "T. rex, Triceratops, Stegosaurus" */
    const char *rise_story;      /* How this group rose to dominance */
    const char *fall_story;      /* How it ended (or "ongoing") */
} fn_era_t;

typedef struct {
    int index;
    const char *title;           /* "T. rex: The Tyrant King" */
    double ma;                   /* 68.0 */
    const char *story;           /* Rich narrative about this moment */
    const char *significance;    /* Why this matters in Earth's story */
} fn_dino_event_t;

/* Get biological era by index (0 to FN_ERA_COUNT-1). Invalid: index=-1, name=NULL */
fn_era_t fn_era_get(int index);

/* Total era count */
int fn_era_count(void);

/* Which era covers a given time in Ma. Returns index=-1 if outside range */
fn_era_t fn_era_at_time(double ma);

/* Era name by index. Returns "?" for invalid */
const char *fn_era_name(int index);

/* Get dinosaur timeline event by index (0 to FN_DINO_EVENT_COUNT-1). Invalid: index=-1 */
fn_dino_event_t fn_dino_event_get(int index);

/* Dinosaur event count */
int fn_dino_event_count(void);

/* Format era as display text. Returns chars written. */
int fn_era_format(int index, char *buf, int buf_size);

/* Format dinosaur event as display text. Returns chars written. */
int fn_dino_format(int index, char *buf, int buf_size);

#endif /* TIME_FOSSIL_NARRATIVE_H */
