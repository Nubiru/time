#include "kin_hk21_layout.h"
#include "../systems/tzolkin/hunab_ku_21.h"
#include <string.h>

/* Ring y-positions. */
#define RING1_Y  0.10f   /* Gates of Light */
#define RING2_Y  0.30f   /* Gates of Power */
#define RING3_Y  0.50f   /* Sources of Power */
#define RING4_Y  0.70f   /* Seats of Power */
#define RING5_Y  0.90f   /* Portal Matrix */
#define CENTER_X 0.50f
#define CENTER_Y 0.50f

/* 4 x-positions within each ring (one per domain quadrant). */
static const float RING_X[4] = { 0.15f, 0.38f, 0.62f, 0.85f };

/* Map seal code (1-20) to ring index.
 * Uses the role enum from hunab_ku_21.h:
 *   GATE_OF_LIGHT(0)=ring1, GATE_OF_POWER(1)=ring2,
 *   SOURCE_OF_POWER(2)=ring3, SEAT_OF_POWER(3)=ring4,
 *   PORTAL_MATRIX(4)=ring5 */
static float y_for_role(int role)
{
    switch (role) {
    case 0: return RING1_Y;   /* HK21_GATE_OF_LIGHT */
    case 1: return RING2_Y;   /* HK21_GATE_OF_POWER */
    case 2: return RING3_Y;   /* HK21_SOURCE_OF_POWER */
    case 3: return RING4_Y;   /* HK21_SEAT_OF_POWER */
    case 4: return RING5_Y;   /* HK21_PORTAL_MATRIX */
    default: return CENTER_Y; /* HK21_CENTER */
    }
}

kin_hk21_layout_t kin_hk21_compute(int today_seal)
{
    kin_hk21_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    if (today_seal < 0 || today_seal > 19)
        return layout;

    layout.today_seal = today_seal;
    layout.today_node = today_seal; /* seal 0 -> node 0, seal 19 -> node 19 */

    /* Track how many nodes placed per ring for x-position assignment. */
    int ring_counters[6] = {0, 0, 0, 0, 0, 0};

    for (int i = 0; i < KIN_HK21_NODES; i++) {
        int seal_code = i + 1;
        hk21_archetype_t arch = hk21_archetype_for_seal(seal_code);

        layout.nodes[i].seal_code  = arch.seal_code;
        layout.nodes[i].seal_name  = arch.seal_name;
        layout.nodes[i].hk_number  = arch.hk_number;
        layout.nodes[i].role       = (int)arch.role;
        layout.nodes[i].domain     = (int)arch.domain;
        layout.nodes[i].statement  = arch.statement;
        layout.nodes[i].planet     = arch.planet;

        layout.nodes[i].y = y_for_role((int)arch.role);

        /* x-position: center for Hunab Ku, ring slot for others */
        if ((int)arch.role == 5) {
            layout.nodes[i].x = CENTER_X;
        } else {
            int slot = ring_counters[(int)arch.role];
            if (slot < 0 || slot >= 4)
                slot = 0;
            layout.nodes[i].x = RING_X[slot];
            ring_counters[(int)arch.role]++;
        }

        /* Highlight the node matching today's Tzolkin seal */
        layout.nodes[i].highlighted = (seal_code == today_seal + 1) ? 1 : 0;
    }

    return layout;
}

int kin_hk21_seal_to_code(int tzolkin_seal)
{
    if (tzolkin_seal < 0 || tzolkin_seal > 19)
        return -1;
    return tzolkin_seal + 1;
}
