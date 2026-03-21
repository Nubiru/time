#include "kin_sync_matrix.h"
#include "../systems/tzolkin/matrix_441.h"
#include "../systems/tzolkin/synchronotron.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include <string.h>

/* Map a 1-441 HK value to row/col (both 1-indexed).
 * idx = value - 1, row = idx/21 + 1, col = idx%21 + 1.
 * Returns 0 if value is out of range. */
static int hk_to_row(int value)
{
    if (value < 1 || value > KIN_SM_TOTAL)
        return 0;
    return (value - 1) / KIN_SM_SIZE + 1;
}

static int hk_to_col(int value)
{
    if (value < 1 || value > KIN_SM_TOTAL)
        return 0;
    return (value - 1) % KIN_SM_SIZE + 1;
}

/* Add a marker if the value is valid (1-441) and not already present.
 * Also marks the corresponding cell as active. */
static void add_marker(kin_sync_matrix_t *m, int value, const char *label)
{
    if (value < 1 || value > KIN_SM_TOTAL)
        return;
    if (m->marker_count >= KIN_SM_MAX_ACTIVE)
        return;

    int row = hk_to_row(value);
    int col = hk_to_col(value);

    /* Deduplicate: skip if already have a marker at this position */
    for (int i = 0; i < m->marker_count; i++) {
        if (m->markers[i].row == row && m->markers[i].col == col)
            return;
    }

    kin_sm_marker_t *mk = &m->markers[m->marker_count];
    mk->number = value;
    mk->row = row;
    mk->col = col;
    mk->label = label;
    m->marker_count++;

    /* Mark cell active */
    int idx = (row - 1) * KIN_SM_SIZE + (col - 1);
    m->cells[idx].active = 1;
}

kin_sync_matrix_t kin_sync_matrix_compute(double jd)
{
    kin_sync_matrix_t result;
    memset(&result, 0, sizeof(result));

    if (jd < 0.0)
        return result;

    result.circuit_count = M441_CIRCUIT_COUNT;

    /* Step 1: Init 441 cells */
    for (int row = 1; row <= KIN_SM_SIZE; row++) {
        for (int col = 1; col <= KIN_SM_SIZE; col++) {
            int idx = (row - 1) * KIN_SM_SIZE + (col - 1);
            kin_sm_cell_t *cell = &result.cells[idx];

            cell->row = row;
            cell->col = col;
            cell->circuit = m441_circuit_for_position(row, col);
            cell->is_axis = (m441_is_tubo_mauri(row, col) ||
                            m441_is_equatorial(row, col)) ? 1 : 0;
            cell->is_center = m441_is_center(row, col);
            cell->active = 0;
            cell->x = (float)(col - 1) / 20.0f;
            cell->y = (float)(row - 1) / 20.0f;
        }
    }

    /* Step 2: Get today's Tzolkin and 13-Moon data */
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);

    /* seal_code for synchronotron is 1-20 (tz.seal is 0-19) */
    int seal_code = tz.seal + 1;

    /* Step 3: Compute and add markers */
    int destiny_hk = sync_seal_hk(seal_code);
    add_marker(&result, destiny_hk, "Destiny");

    int gate_hk = sync_heptad_gate_hk(tm.day_of_week);
    add_marker(&result, gate_hk, "Heptad Gate");

    int tonal = sync_tonal_value(tz.tone);
    add_marker(&result, tonal, "Tonal Value");

    /* Center marker (441) — always present */
    add_marker(&result, KIN_SM_TOTAL, "Center");

    return result;
}
