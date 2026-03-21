/* tree_of_life_layout.c — Kabbalah Tree of Life card layout
 *
 * Wraps tree_geometry + sefirot data into a card-friendly layout with
 * names, colors, labels, and highlight state.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "tree_of_life_layout.h"
#include "../systems/kabbalah/sefirot.h"
#include "../systems/kabbalah/tree_geometry.h"
#include <string.h>

/* Card bounds: content area within the card */
#define CONTENT_X  0.10f
#define CONTENT_Y  0.05f
#define CONTENT_W  0.80f
#define CONTENT_H  0.85f

/* Title position */
#define TITLE_X    0.50f
#define TITLE_Y    0.02f

/* Overall card bounds */
#define BOUNDS_X   0.05f
#define BOUNDS_Y   0.00f
#define BOUNDS_W   0.90f
#define BOUNDS_H   0.95f

/* Scale a raw tree position (from tree_layout_generate(1,1)) into card space.
 * node_x comes in [0.0, 1.0], node_y comes in [0.0, 1.0].
 * Map to content area. */
static float scale_x(float raw_x)
{
    return CONTENT_X + raw_x * CONTENT_W;
}

static float scale_y(float raw_y)
{
    return CONTENT_Y + raw_y * CONTENT_H;
}

tree_of_life_layout_t tree_of_life_compute(int active_sefirah, int active_path)
{
    tree_of_life_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Generate raw geometry at unit scale (1x1) */
    tree_layout_t geo = tree_layout_generate(1.0f, 1.0f);

    layout.node_count = TOL_NODE_COUNT;
    layout.path_count = TOL_PATH_COUNT;
    layout.active_sefirah = active_sefirah;
    layout.active_path = active_path;

    /* Populate the 10 Sefirot nodes */
    for (int i = 0; i < 10; i++) {
        sefirah_t s = sefirot_get(i);
        layout.nodes[i].id = s.id;
        layout.nodes[i].name = s.name;
        layout.nodes[i].meaning = s.meaning;
        layout.nodes[i].pillar = sefirot_pillar_name(s.pillar);
        layout.nodes[i].x = scale_x(geo.node_x[i]);
        layout.nodes[i].y = scale_y(geo.node_y[i]);
        layout.nodes[i].r = s.color.r;
        layout.nodes[i].g = s.color.g;
        layout.nodes[i].b = s.color.b;
        layout.nodes[i].highlighted = (i == active_sefirah) ? 1 : 0;
    }

    /* Da'at as node[10] */
    sefirah_t daat = sefirot_get_daat();
    layout.nodes[10].id = daat.id;
    layout.nodes[10].name = daat.name;
    layout.nodes[10].meaning = daat.meaning;
    layout.nodes[10].pillar = sefirot_pillar_name(daat.pillar);
    layout.nodes[10].x = scale_x(geo.node_x[10]);
    layout.nodes[10].y = scale_y(geo.node_y[10]);
    layout.nodes[10].r = daat.color.r;
    layout.nodes[10].g = daat.color.g;
    layout.nodes[10].b = daat.color.b;
    layout.nodes[10].highlighted = (active_sefirah == 10) ? 1 : 0;

    /* Populate the 22 paths */
    for (int i = 0; i < TREE_PATH_COUNT; i++) {
        tree_path_t p = tree_path_get(i);
        layout.paths[i].path_id = p.id;
        layout.paths[i].from_node = p.from_sefirah;
        layout.paths[i].to_node = p.to_sefirah;
        layout.paths[i].hebrew_letter = p.hebrew_letter;
        layout.paths[i].tarot = p.tarot;
        layout.paths[i].x0 = scale_x(geo.lines[i].x0);
        layout.paths[i].y0 = scale_y(geo.lines[i].y0);
        layout.paths[i].x1 = scale_x(geo.lines[i].x1);
        layout.paths[i].y1 = scale_y(geo.lines[i].y1);
        layout.paths[i].highlighted = (i == active_path) ? 1 : 0;
    }

    /* Title */
    layout.title_x = TITLE_X;
    layout.title_y = TITLE_Y;

    /* Bounds */
    layout.bounds_x = BOUNDS_X;
    layout.bounds_y = BOUNDS_Y;
    layout.bounds_w = BOUNDS_W;
    layout.bounds_h = BOUNDS_H;

    return layout;
}

tol_node_t tree_of_life_node(const tree_of_life_layout_t *layout, int id)
{
    tol_node_t empty;
    memset(&empty, 0, sizeof(empty));
    empty.id = -1;

    if (!layout || id < 0 || id > 10) {
        return empty;
    }

    /* Find node by id — nodes are stored with 0-9 at indices 0-9,
     * Da'at (id=10) at index 10. */
    for (int i = 0; i < TOL_NODE_COUNT; i++) {
        if (layout->nodes[i].id == id) {
            return layout->nodes[i];
        }
    }

    return empty;
}

tol_path_t tree_of_life_path(const tree_of_life_layout_t *layout, int index)
{
    tol_path_t empty;
    memset(&empty, 0, sizeof(empty));
    empty.path_id = -1;

    if (!layout || index < 0 || index >= TOL_PATH_COUNT) {
        return empty;
    }

    return layout->paths[index];
}
