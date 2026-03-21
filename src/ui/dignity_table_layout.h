#ifndef TIME_DIGNITY_TABLE_LAYOUT_H
#define TIME_DIGNITY_TABLE_LAYOUT_H

/* Planetary dignity table layout — 7x5 grid of essential dignities.
 *
 * Computes a card-friendly grid showing planetary dignities (domicile,
 * exaltation, detriment, fall) for the 7 traditional planets: Sun, Moon,
 * Mercury, Venus, Mars, Jupiter, Saturn.
 *
 * Grid: 1 header row + 7 planet rows, 5 columns (name, domicile,
 * exaltation, detriment, fall). Positions in normalized 0-1 space.
 *
 * Table bounds: x=0.05, y=0.10, w=0.90, h=0.85
 * 5 equal columns at 20% each, 8 equal rows.
 *
 * Sources: Ptolemy, "Tetrabiblos" (traditional dignities).
 * Pure module: no GL, no malloc, no globals, no side effects. */

typedef struct {
    float x, y, w, h;
} dt_cell_t;

/* One row = one planet */
typedef struct {
    int planet;                  /* dignity index: 8=Sun, 9=Moon, 0=Mercury, 1=Venus, 3=Mars, 4=Jupiter, 5=Saturn */
    const char *planet_name;
    int domicile_sign;           /* sign where planet rules (-1 if none) */
    int exaltation_sign;         /* sign where planet is exalted (-1 if none) */
    int detriment_sign;          /* opposite of domicile (-1 if none) */
    int fall_sign;               /* opposite of exaltation (-1 if none) */
    dt_cell_t name_cell;         /* planet name cell */
    dt_cell_t domicile_cell;
    dt_cell_t exaltation_cell;
    dt_cell_t detriment_cell;
    dt_cell_t fall_cell;
} dt_planet_row_t;

#define DT_PLANET_COUNT 7  /* Sun, Moon, Mercury, Venus, Mars, Jupiter, Saturn */

typedef struct {
    dt_planet_row_t rows[DT_PLANET_COUNT];

    /* Column headers */
    dt_cell_t header_name;
    dt_cell_t header_domicile;
    dt_cell_t header_exaltation;
    dt_cell_t header_detriment;
    dt_cell_t header_fall;

    float table_x, table_y, table_w, table_h;
} dignity_table_layout_t;

/* Compute the full dignity table layout.
 * Returns a table with 7 rows (Sun..Saturn) and header cells. */
dignity_table_layout_t dignity_table_compute(void);

/* Get a planet row by index (0-6).
 * Returns row with planet=-1 for invalid index or NULL layout. */
dt_planet_row_t dignity_table_row(const dignity_table_layout_t *layout, int index);

#endif /* TIME_DIGNITY_TABLE_LAYOUT_H */
