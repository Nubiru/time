#include "kin_round_layout.h"
#include "kin_cell.h"
#include "../systems/tzolkin/cr_cycle.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define PETAL_RADIUS 0.4f
#define CENTER_X     0.5f
#define CENTER_Y     0.5f

/* Dreamspell year bearer seals: Seed, Moon, Wizard, Storm. */
static const int YEAR_BEARERS[4] = {3, 8, 13, 18};

int kin_round_year_bearer(int year_in_round)
{
    if (year_in_round < 0 || year_in_round >= KIN_ROUND_PETALS)
        return -1;
    return YEAR_BEARERS[year_in_round % 4];
}

kin_round_layout_t kin_round_compute(double jd)
{
    kin_round_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    if (jd < 0.0)
        return layout;

    cr_cycle_t cr = cr_cycle_from_jd(jd);
    int remaining = cr_cycle_days_remaining(jd);

    layout.current_year   = cr.year_in_round;
    layout.days_remaining = remaining;
    layout.progress       = 1.0f - (float)remaining / (float)CR_CYCLE_DAYS;

    for (int i = 0; i < KIN_ROUND_PETALS; i++) {
        float angle_deg = (float)i * (360.0f / 52.0f);
        float angle_rad = angle_deg * (float)(M_PI / 180.0);

        int seal = kin_round_year_bearer(i);

        layout.petals[i].angle         = angle_deg;
        layout.petals[i].x             = CENTER_X + PETAL_RADIUS * sinf(angle_rad);
        layout.petals[i].y             = CENTER_Y - PETAL_RADIUS * cosf(angle_rad);
        layout.petals[i].year_in_round = i;
        layout.petals[i].bearer_seal   = seal;
        layout.petals[i].bearer_tone   = (i % 13) + 1;
        layout.petals[i].color         = kin_cell_color(seal);
        layout.petals[i].highlighted   = (i == cr.year_in_round) ? 1 : 0;
    }

    return layout;
}
