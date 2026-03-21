#include "kin_tubo_mauri.h"
#include "../systems/tzolkin/heptad_gate.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include <string.h>

/* Vertical column layout. */
#define TUBO_X       0.50f
#define TUBO_Y_START 0.05f
#define TUBO_Y_STEP  0.15f

kin_tubo_mauri_t kin_tubo_compute(double jd)
{
    kin_tubo_mauri_t tubo;
    memset(&tubo, 0, sizeof(tubo));
    tubo.today_gate = -1;

    if (jd < 0.0)
        return tubo;

    thirteen_moon_t tm = thirteen_moon_from_jd(jd);

    /* Day Out of Time has no moon day -> return zeroed */
    if (tm.is_day_out)
        return tubo;

    /* Invalid result (moon=0 and not day_out) -> zeroed */
    if (tm.moon == 0)
        return tubo;

    tubo.today_day_of_week = tm.day_of_week;
    tubo.today_gate = tm.day_of_week - 1; /* 0-indexed */

    /* Build all 7 gates */
    for (int i = 0; i < KIN_TUBO_GATES; i++) {
        heptad_gate_t gate_data = heptad_gate_for_day(i + 1);

        tubo.gates[i].day_of_week    = gate_data.day;
        tubo.gates[i].plasma         = gate_data.plasma;
        tubo.gates[i].chakra         = gate_data.chakra;
        tubo.gates[i].mantra         = gate_data.mantra;
        tubo.gates[i].consciousness  = gate_data.consciousness;
        tubo.gates[i].mental_sphere  = gate_data.mental_sphere;
        tubo.gates[i].hk_number      = gate_data.hk_number;
        tubo.gates[i].highlighted    = (i + 1 == tm.day_of_week) ? 1 : 0;
        tubo.gates[i].x              = TUBO_X;
        tubo.gates[i].y              = TUBO_Y_START + (float)i * TUBO_Y_STEP;
    }

    return tubo;
}
