/* earth_story.h — Cinematic Earth History: 20 dramatic moments across 4.5 Ga.
 * Individual cinematic timestamps with drama levels, visual descriptions,
 * and narrative impact. The "highlight reel" of Earth's story.
 * Pure data, no side effects. */

#ifndef TIME_EARTH_STORY_H
#define TIME_EARTH_STORY_H

#define ES_MOMENT_COUNT 20
#define ES_DRAMA_MAX 5

/* Drama level: how cinematically dramatic this moment is */
typedef enum {
    ES_DRAMA_QUIET = 1,        /* Slow change, subtle */
    ES_DRAMA_NOTABLE = 2,      /* Significant but gradual */
    ES_DRAMA_DRAMATIC = 3,     /* Major transformation */
    ES_DRAMA_CATASTROPHIC = 4, /* Sudden, violent change */
    ES_DRAMA_APOCALYPTIC = 5   /* World-altering in an instant */
} es_drama_t;

/* A single cinematic moment in Earth's story */
typedef struct {
    int index;
    const char *title;          /* "The Great Dying" */
    double ma;                  /* 252.0 (millions of years ago) */
    es_drama_t drama;           /* drama level 1-5 */
    const char *visual;         /* "Volcanic chains stretch across Siberia..." */
    const char *narrative;      /* Rich story paragraph: what happened, why it matters */
    const char *aftermath;      /* What came next */
    const char *sound_hint;     /* Suggested audio: "deep_rumble", "silence", "ocean_waves" */
} es_moment_t;

/* Get moment by index (0 to ES_MOMENT_COUNT-1).
 * Invalid: index=-1, title=NULL */
es_moment_t es_moment_get(int index);

/* Total moment count. */
int es_moment_count(void);

/* Find the moment closest to a given time (Ma). */
es_moment_t es_moment_nearest(double ma);

/* Get moments at or above a drama threshold.
 * Writes indices to out_indices (max out_max).
 * Returns count written. */
int es_moments_by_drama(es_drama_t min_drama, int *out_indices, int out_max);

/* Title by index. Returns "?" for invalid. */
const char *es_moment_title(int index);

/* Drama level name string. */
const char *es_drama_name(es_drama_t drama);

/* Format moment as multi-line display text.
 * Returns chars written (excluding NUL), 0 on error. */
int es_moment_format(int index, char *buf, int buf_size);

#endif /* TIME_EARTH_STORY_H */
