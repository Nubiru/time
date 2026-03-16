/* Timeline data — pure module providing tick data for the visual time scrubber.
 * Given a JD range and zoom level, produces labeled tick marks with relative
 * positions (0.0-1.0). Supports multiple time scales (hours to gigayears).
 * No GL, no malloc, no globals, no side effects. */

#ifndef TIME_TIMELINE_DATA_H
#define TIME_TIMELINE_DATA_H

#define TL_MAX_TICKS 128
#define TL_LABEL_MAX 48

/* Timeline granularity levels */
typedef enum {
    TL_GRAN_HOUR = 0,      /* 1-hour ticks */
    TL_GRAN_DAY,            /* 1-day ticks */
    TL_GRAN_WEEK,           /* 7-day ticks */
    TL_GRAN_MONTH,          /* month ticks */
    TL_GRAN_YEAR,           /* year ticks */
    TL_GRAN_DECADE,         /* 10-year ticks */
    TL_GRAN_CENTURY,        /* 100-year ticks */
    TL_GRAN_MILLENNIUM,     /* 1000-year ticks */
    TL_GRAN_MEGAYEAR,       /* million-year ticks */
    TL_GRAN_GIGAYEAR,       /* billion-year ticks */
    TL_GRAN_COUNT
} tl_granularity_t;

/* Tick importance for visual weight */
typedef enum {
    TL_TICK_MINOR = 0,      /* small mark, no label */
    TL_TICK_MAJOR,           /* medium mark, label */
    TL_TICK_ANCHOR,          /* large mark, bold label (now, epoch, era boundary) */
    TL_TICK_SPECIAL          /* user bookmark or festival */
} tl_tick_type_t;

/* A single tick on the timeline */
typedef struct {
    double jd;               /* Julian Day of this tick */
    double position;         /* 0.0-1.0 relative position in view range */
    tl_tick_type_t type;     /* visual importance */
    char label[TL_LABEL_MAX]; /* "2024", "Jan", "12:00", "3.5 Ga" */
} tl_tick_t;

/* Timeline view range + tick output */
typedef struct {
    double jd_start;         /* left edge of view */
    double jd_end;           /* right edge of view */
    double jd_center;        /* current time (cursor position) */
    tl_granularity_t granularity; /* auto-selected or manual */
    tl_tick_t ticks[TL_MAX_TICKS];
    int tick_count;
} tl_timeline_t;

/* Auto-select appropriate granularity for a JD range span. */
tl_granularity_t tl_auto_granularity(double jd_span);

/* Generate timeline ticks for a JD range. */
tl_timeline_t tl_generate(double jd_start, double jd_end, double jd_center);

/* Generate with explicit granularity override. */
tl_timeline_t tl_generate_at(double jd_start, double jd_end, double jd_center,
                              tl_granularity_t granularity);

/* Zoom: expand/contract range around center by a factor. */
tl_timeline_t tl_zoom(tl_timeline_t tl, double factor);

/* Pan: shift range by a fraction of the current span. */
tl_timeline_t tl_pan(tl_timeline_t tl, double fraction);

/* Find the tick nearest to a given position (0.0-1.0). Returns tick index, -1 if empty. */
int tl_nearest_tick(const tl_timeline_t *tl, double position);

/* Get human-readable range description. "Mar 15 - Mar 22, 2026" or "Cenozoic Era".
 * Returns number of characters written (excluding null). */
int tl_range_label(const tl_timeline_t *tl, char *buf, int buf_size);

/* Granularity step size in days. */
double tl_granularity_days(tl_granularity_t gran);

/* Granularity name string. */
const char *tl_granularity_name(tl_granularity_t gran);

/* Count ticks by type. */
int tl_count_by_type(const tl_timeline_t *tl, tl_tick_type_t type);

#endif /* TIME_TIMELINE_DATA_H */
