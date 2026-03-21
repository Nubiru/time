/* bodygraph_chart_layout.h — Visual layout of the Human Design bodygraph.
 *
 * Computes positioned elements for rendering the bodygraph: 9 centers
 * with defined/undefined state, 36 channels with active/inactive state,
 * and gate positions.  Takes an array of active gates and returns a
 * layout ready for rendering in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BODYGRAPH_CHART_LAYOUT_H
#define TIME_BODYGRAPH_CHART_LAYOUT_H

#define BG_CHART_CENTER_COUNT  9
#define BG_CHART_MAX_CHANNELS  36

/* Single center in the bodygraph display. */
typedef struct {
    int id;               /* bg_center_id_t (0-8) */
    const char *name;     /* "Head", "Sacral", etc. */
    float x, y;           /* position (normalized 0-1) */
    float w, h;           /* size of center shape */
    int is_defined;       /* 1 if this center has at least one complete channel */
    int is_motor;         /* 1 if motor center */
    int is_awareness;     /* 1 if awareness center */
} chart_center_t;

/* Channel connecting two centers. */
typedef struct {
    int channel_index;    /* 0-35 */
    int gate_a;           /* gate number */
    int gate_b;           /* gate number */
    int center_a;         /* center id */
    int center_b;         /* center id */
    float x0, y0;         /* start point */
    float x1, y1;         /* end point */
    int is_active;        /* 1 if both gates are active */
} chart_channel_t;

typedef struct {
    /* Centers */
    chart_center_t centers[BG_CHART_CENTER_COUNT];
    int defined_count;          /* number of defined centers */

    /* Channels */
    chart_channel_t channels[BG_CHART_MAX_CHANNELS];
    int channel_count;          /* total channels (always 36) */
    int active_channel_count;   /* channels where both gates active */

    /* HD type determination (simplified) */
    int type;                   /* -1 = unknown, 4 = Reflector (no defined) */
    const char *type_name;      /* "Unknown", "Reflector", "Generator", etc. */

    /* Overall bounds */
    float bounds_x, bounds_y;
    float bounds_w, bounds_h;
} bodygraph_chart_layout_t;

/* Compute the bodygraph chart layout.
 * active_gates: array of 65 ints (indices 1-64), 1=active, 0=inactive.
 * If NULL, all gates are inactive (blank bodygraph). */
bodygraph_chart_layout_t bodygraph_chart_compute(const int *active_gates);

/* Get a center from the layout by center id (0-8).
 * Returns center with id=-1 for invalid center_id. */
chart_center_t bodygraph_chart_center(const bodygraph_chart_layout_t *layout,
                                      int center_id);

/* Check if a center is defined in the layout.
 * Returns 0 for invalid center_id or NULL layout. */
int bodygraph_chart_is_defined(const bodygraph_chart_layout_t *layout,
                               int center_id);

#endif /* TIME_BODYGRAPH_CHART_LAYOUT_H */
