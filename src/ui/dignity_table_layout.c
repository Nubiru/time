/* dignity_table_layout.c — 7x5 planetary dignity grid.
 *
 * Builds a grid showing domicile, exaltation, detriment, and fall
 * for the 7 traditional planets. All coordinates normalized 0-1.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "dignity_table_layout.h"
#include "../systems/astrology/dignity.h"
#include "../systems/astrology/zodiac.h"
#include <string.h>

/* Table geometry (normalized 0-1 coordinates). */
#define TABLE_X     0.05f
#define TABLE_Y     0.10f
#define TABLE_W     0.90f
#define TABLE_H     0.85f

#define COL_COUNT   5
#define ROW_COUNT   8   /* 1 header + 7 planets */

/* Planet order: Sun, Moon, Mercury, Venus, Mars, Jupiter, Saturn.
 * These are dignity.h planet indices. */
static const int PLANET_ORDER[DT_PLANET_COUNT] = {
    DIGNITY_SUN,     /* 8  */
    DIGNITY_MOON,    /* 9  */
    DIGNITY_MERCURY, /* 0  */
    DIGNITY_VENUS,   /* 1  */
    DIGNITY_MARS,    /* 3  */
    DIGNITY_JUPITER, /* 4  */
    DIGNITY_SATURN   /* 5  */
};

static const char *PLANET_NAMES[DT_PLANET_COUNT] = {
    "Sun", "Moon", "Mercury", "Venus", "Mars", "Jupiter", "Saturn"
};

/* Find the exaltation sign for a given planet by scanning all 12 signs. */
static int find_exaltation_sign(int planet)
{
    for (int s = 0; s < 12; s++) {
        if (sign_exaltation_ruler(s) == planet) {
            return s;
        }
    }
    return -1;
}

/* Build a cell at grid position (col, row) within the table. */
static dt_cell_t make_cell(int col, int row)
{
    dt_cell_t cell;
    float col_w = TABLE_W / (float)COL_COUNT;
    float row_h = TABLE_H / (float)ROW_COUNT;
    cell.x = TABLE_X + (float)col * col_w;
    cell.y = TABLE_Y + (float)row * row_h;
    cell.w = col_w;
    cell.h = row_h;
    return cell;
}

dignity_table_layout_t dignity_table_compute(void)
{
    dignity_table_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    layout.table_x = TABLE_X;
    layout.table_y = TABLE_Y;
    layout.table_w = TABLE_W;
    layout.table_h = TABLE_H;

    /* Column headers (row 0). */
    layout.header_name        = make_cell(0, 0);
    layout.header_domicile    = make_cell(1, 0);
    layout.header_exaltation  = make_cell(2, 0);
    layout.header_detriment   = make_cell(3, 0);
    layout.header_fall        = make_cell(4, 0);

    /* Planet rows (rows 1-7). */
    for (int i = 0; i < DT_PLANET_COUNT; i++) {
        int planet = PLANET_ORDER[i];
        int row = i + 1;  /* row 0 is header */

        layout.rows[i].planet      = planet;
        layout.rows[i].planet_name = PLANET_NAMES[i];

        /* Domicile: primary sign this planet rules. */
        int dom = planet_domicile_sign(planet);
        layout.rows[i].domicile_sign = dom;

        /* Exaltation: scan all signs for this planet. */
        int exalt = find_exaltation_sign(planet);
        layout.rows[i].exaltation_sign = exalt;

        /* Detriment: opposite of domicile. */
        if (dom >= 0) {
            layout.rows[i].detriment_sign = (dom + 6) % 12;
        } else {
            layout.rows[i].detriment_sign = -1;
        }

        /* Fall: opposite of exaltation. */
        if (exalt >= 0) {
            layout.rows[i].fall_sign = (exalt + 6) % 12;
        } else {
            layout.rows[i].fall_sign = -1;
        }

        /* Cell positions. */
        layout.rows[i].name_cell        = make_cell(0, row);
        layout.rows[i].domicile_cell    = make_cell(1, row);
        layout.rows[i].exaltation_cell  = make_cell(2, row);
        layout.rows[i].detriment_cell   = make_cell(3, row);
        layout.rows[i].fall_cell        = make_cell(4, row);
    }

    return layout;
}

dt_planet_row_t dignity_table_row(const dignity_table_layout_t *layout, int index)
{
    dt_planet_row_t empty;
    memset(&empty, 0, sizeof(empty));
    empty.planet = -1;
    empty.planet_name = "?";
    empty.domicile_sign = -1;
    empty.exaltation_sign = -1;
    empty.detriment_sign = -1;
    empty.fall_sign = -1;

    if (!layout || index < 0 || index >= DT_PLANET_COUNT) {
        return empty;
    }

    return layout->rows[index];
}
