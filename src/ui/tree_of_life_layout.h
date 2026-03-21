/* tree_of_life_layout.h — Kabbalah Tree of Life card layout
 *
 * Computes a card-friendly layout of the Tree of Life showing 10 Sefirot
 * (plus Da'at) as positioned nodes and 22 paths as connecting lines,
 * with labels, colors, and daily highlighting.
 *
 * Wraps tree_geometry.h (raw geometry) into a format suitable for card
 * rendering with names, meanings, pillar labels, and highlight state.
 *
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_TREE_OF_LIFE_LAYOUT_H
#define TIME_TREE_OF_LIFE_LAYOUT_H

#define TOL_NODE_COUNT 11     /* 10 Sefirot + Da'at */
#define TOL_PATH_COUNT 22

/* Single node (Sefirah) in the card layout */
typedef struct {
    int id;                   /* 0-9 (Keter=0, Malkuth=9), 10=Da'at */
    const char *name;         /* "Keter", "Chokmah", etc. */
    const char *meaning;      /* "Crown", "Wisdom", etc. */
    const char *pillar;       /* "Balance", "Mercy", "Severity" */
    float x, y;               /* position (normalized 0-1) */
    float r, g, b;            /* Queen Scale color */
    int highlighted;          /* 1 if this is the active Sefirah */
} tol_node_t;

/* Single path connecting two Sefirot */
typedef struct {
    int path_id;              /* 0-21 */
    int from_node;            /* sefirah index */
    int to_node;              /* sefirah index */
    const char *hebrew_letter;/* Hebrew letter name */
    const char *tarot;        /* Major Arcana name */
    float x0, y0;             /* start point */
    float x1, y1;             /* end point */
    int highlighted;          /* 1 if this path is active */
} tol_path_t;

typedef struct {
    tol_node_t nodes[TOL_NODE_COUNT];
    int node_count;           /* 11 (10 + Da'at) */

    tol_path_t paths[TOL_PATH_COUNT];
    int path_count;           /* 22 */

    int active_sefirah;       /* highlighted sefirah index, or -1 */
    int active_path;          /* highlighted path index, or -1 */

    /* Title position */
    float title_x, title_y;

    /* Overall bounds */
    float bounds_x, bounds_y;
    float bounds_w, bounds_h;
} tree_of_life_layout_t;

/* Compute the Tree of Life card layout.
 * active_sefirah: 0-9 to highlight a Sefirah, -1 for none.
 * active_path: 0-21 to highlight a path, -1 for none. */
tree_of_life_layout_t tree_of_life_compute(int active_sefirah, int active_path);

/* Get a node by id (0-10). Returns zeroed tol_node_t with id=-1 for invalid. */
tol_node_t tree_of_life_node(const tree_of_life_layout_t *layout, int id);

/* Get a path by index (0-21). Returns zeroed tol_path_t with path_id=-1 for invalid. */
tol_path_t tree_of_life_path(const tree_of_life_layout_t *layout, int index);

#endif /* TIME_TREE_OF_LIFE_LAYOUT_H */
