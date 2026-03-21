#include "kin_base_matrix.h"
#include "../systems/tzolkin/synchronotron.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include <string.h>

/* --- Internal helpers --- */

/* Add an active position if it is in range 0-440 and not a duplicate. */
static void add_active(kin_base_matrix_t *m, int number, const char *label)
{
    if (number < 0 || number > 440)
        return;
    if (m->active_count >= KIN_BM_MAX_ACTIVE)
        return;

    /* Check for duplicates. */
    for (int i = 0; i < m->active_count; i++) {
        if (m->active[i].number == number)
            return;
    }

    int row = number / KIN_BM_SIZE;
    int col = number % KIN_BM_SIZE;

    m->active[m->active_count].number = number;
    m->active[m->active_count].row    = row;
    m->active[m->active_count].col    = col;
    m->active[m->active_count].label  = label;
    m->active_count++;

    m->cells[number].active = 1;
}

/* --- Public API --- */

kin_base_matrix_t kin_base_matrix_compute(double jd)
{
    kin_base_matrix_t m;
    memset(&m, 0, sizeof(m));
    m.center_number = 220;

    /* Initialize all 441 cells. */
    for (int i = 0; i < KIN_BM_TOTAL; i++) {
        int row = i / KIN_BM_SIZE;
        int col = i % KIN_BM_SIZE;
        m.cells[i].number    = i;
        m.cells[i].x         = (float)col / (float)KIN_BM_SIZE;
        m.cells[i].y         = (float)row / (float)KIN_BM_SIZE;
        m.cells[i].active    = 0;
        m.cells[i].is_center = (i == 220) ? 1 : 0;
    }

    /* Invalid JD: return grid with no active positions. */
    if (jd < 0.0)
        return m;

    /* Get today's Tzolkin and 13-Moon data. */
    tzolkin_day_t tz  = tzolkin_from_jd(jd);
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);

    /* seal_code: Tzolkin seal 0-19 -> HK seal code 1-20 */
    int seal_code   = tz.seal + 1;
    int day_of_week = tm.day_of_week;

    /* Compute active positions from Synchronotron. */
    int destiny_hk = sync_seal_hk(seal_code);
    int gate_hk    = sync_heptad_gate_hk(day_of_week);
    int tonal      = sync_tonal_value(tz.tone);

    add_active(&m, destiny_hk, "Destiny");
    add_active(&m, gate_hk,    "Heptad Gate");
    add_active(&m, tonal,      "Tonal Value");

    return m;
}

void kin_bm_number_to_pos(int number, int *row, int *col)
{
    if (number < 0 || number > 440) {
        if (row) *row = -1;
        if (col) *col = -1;
        return;
    }
    if (row) *row = number / KIN_BM_SIZE;
    if (col) *col = number % KIN_BM_SIZE;
}

int kin_bm_pos_to_number(int row, int col)
{
    if (row < 0 || row >= KIN_BM_SIZE)
        return -1;
    if (col < 0 || col >= KIN_BM_SIZE)
        return -1;
    return row * KIN_BM_SIZE + col;
}
