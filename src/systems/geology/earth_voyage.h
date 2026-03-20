/* earth_voyage.h — Earth History Voyage: chapter-based narrative of 4.54 Ga.
 * 12 chapters covering Earth's full story from formation to humanity,
 * structured for fly-through rendering. Pure data, no side effects. */

#ifndef TIME_EARTH_VOYAGE_H
#define TIME_EARTH_VOYAGE_H

#define EV_CHAPTER_COUNT 12
#define EV_EVENTS_PER_CHAPTER 6

typedef struct {
    int index;
    const char *title;           /* "Birth of Earth" */
    double start_ma;             /* 4540.0 (older, larger) */
    double end_ma;               /* 4000.0 (younger, smaller) */
    const char *narrative;       /* Multi-sentence story paragraph */
    const char *visual_hint;     /* "molten_surface", "ocean_world", etc. */
    int event_count;
    const char *events[EV_EVENTS_PER_CHAPTER];  /* Key events in chapter */
} ev_chapter_t;

/* Get chapter by index (0 to EV_CHAPTER_COUNT-1).
 * Returns struct with index=-1, title=NULL for invalid. */
ev_chapter_t ev_chapter_get(int index);

/* Total chapter count (always EV_CHAPTER_COUNT). */
int ev_chapter_count(void);

/* Which chapter covers a given time in Ma.
 * Returns struct with index=-1, title=NULL if outside range.
 * Boundary: exact start_ma belongs to the next (younger) chapter. */
ev_chapter_t ev_chapter_at_time(double ma);

/* Chapter title by index. Returns "?" for invalid. */
const char *ev_chapter_title(int index);

/* Format chapter as multi-line text into buf (max buf_size).
 * Returns chars written (excluding NUL), 0 on error. */
int ev_chapter_format(int index, char *buf, int buf_size);

#endif /* TIME_EARTH_VOYAGE_H */
