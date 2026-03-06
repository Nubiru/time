/* bodygraph.c — Human Design bodygraph geometry implementation.
 * Pure data module. No GL, no malloc, no globals, no side effects. */

#include "bodygraph.h"

/* ===== CENTER DATA ===== */

static const bg_center_t CENTERS[BODYGRAPH_CENTER_COUNT] = {
    { BG_HEAD,          "Head",          {0.50f, 0.05f}, 0, 0, 1 },
    { BG_AJNA,          "Ajna",          {0.50f, 0.20f}, 0, 1, 0 },
    { BG_THROAT,        "Throat",        {0.50f, 0.35f}, 0, 0, 0 },
    { BG_G_CENTER,      "G Center",      {0.50f, 0.50f}, 0, 0, 0 },
    { BG_HEART,         "Heart",         {0.30f, 0.50f}, 1, 0, 0 },
    { BG_SPLEEN,        "Spleen",        {0.20f, 0.65f}, 0, 1, 0 },
    { BG_SACRAL,        "Sacral",        {0.50f, 0.75f}, 1, 0, 0 },
    { BG_SOLAR_PLEXUS,  "Solar Plexus",  {0.80f, 0.65f}, 1, 1, 0 },
    { BG_ROOT,          "Root",          {0.50f, 0.92f}, 1, 0, 1 },
};

/* ===== 36 CHANNELS ===== */

static const bg_channel_t CHANNELS[BODYGRAPH_CHANNEL_COUNT] = {
    /* Head-Ajna (3) */
    { 64, 47, BG_HEAD,          BG_AJNA          },
    { 61, 24, BG_HEAD,          BG_AJNA          },
    { 63,  4, BG_HEAD,          BG_AJNA          },

    /* Ajna-Throat (3) */
    { 17, 62, BG_AJNA,          BG_THROAT        },
    { 43, 23, BG_AJNA,          BG_THROAT        },
    { 11, 56, BG_AJNA,          BG_THROAT        },

    /* Throat-G Center (4) */
    { 31,  7, BG_THROAT,        BG_G_CENTER      },
    {  8,  1, BG_THROAT,        BG_G_CENTER      },
    { 33, 13, BG_THROAT,        BG_G_CENTER      },
    { 20, 10, BG_THROAT,        BG_G_CENTER      },

    /* Throat-Heart (1) */
    { 45, 21, BG_THROAT,        BG_HEART         },

    /* Throat-Spleen (2) */
    { 16, 48, BG_THROAT,        BG_SPLEEN        },
    { 20, 57, BG_THROAT,        BG_SPLEEN        },

    /* Throat-Solar Plexus (2) */
    { 35, 36, BG_THROAT,        BG_SOLAR_PLEXUS  },
    { 12, 22, BG_THROAT,        BG_SOLAR_PLEXUS  },

    /* Throat-Sacral (1) */
    { 20, 34, BG_THROAT,        BG_SACRAL        },

    /* G Center-Sacral (4) */
    { 15,  5, BG_G_CENTER,      BG_SACRAL        },
    { 46, 29, BG_G_CENTER,      BG_SACRAL        },
    {  2, 14, BG_G_CENTER,      BG_SACRAL        },
    { 10, 34, BG_G_CENTER,      BG_SACRAL        },

    /* G Center-Heart (1) */
    { 25, 51, BG_G_CENTER,      BG_HEART         },

    /* G Center-Spleen (1) */
    { 10, 57, BG_G_CENTER,      BG_SPLEEN        },

    /* Heart-Spleen (1) */
    { 26, 44, BG_HEART,         BG_SPLEEN        },

    /* Heart-Solar Plexus (1) */
    { 40, 37, BG_HEART,         BG_SOLAR_PLEXUS  },

    /* Spleen-Sacral (2) */
    { 50, 27, BG_SPLEEN,        BG_SACRAL        },
    { 34, 57, BG_SACRAL,        BG_SPLEEN        },

    /* Spleen-Root (3) */
    { 32, 54, BG_SPLEEN,        BG_ROOT          },
    { 28, 38, BG_SPLEEN,        BG_ROOT          },
    { 18, 58, BG_SPLEEN,        BG_ROOT          },

    /* Sacral-Solar Plexus (1) */
    { 59,  6, BG_SACRAL,        BG_SOLAR_PLEXUS  },

    /* Root-Sacral (3) */
    { 53, 42, BG_ROOT,          BG_SACRAL        },
    { 60,  3, BG_ROOT,          BG_SACRAL        },
    { 52,  9, BG_ROOT,          BG_SACRAL        },

    /* Root-Solar Plexus (3) */
    { 19, 49, BG_ROOT,          BG_SOLAR_PLEXUS  },
    { 39, 55, BG_ROOT,          BG_SOLAR_PLEXUS  },
    { 41, 30, BG_ROOT,          BG_SOLAR_PLEXUS  },
};

/* ===== GATE-TO-CENTER LOOKUP (index 0 unused, 1-64 valid) ===== */

static const bg_center_id_t GATE_CENTERS[65] = {
    BG_CENTER_COUNT,    /* 0: unused */
    BG_G_CENTER,        /* 1 */
    BG_G_CENTER,        /* 2 */
    BG_SACRAL,          /* 3 */
    BG_AJNA,            /* 4 */
    BG_SACRAL,          /* 5 */
    BG_SOLAR_PLEXUS,    /* 6 */
    BG_G_CENTER,        /* 7 */
    BG_THROAT,          /* 8 */
    BG_SACRAL,          /* 9 */
    BG_G_CENTER,        /* 10 */
    BG_AJNA,            /* 11 */
    BG_THROAT,          /* 12 */
    BG_G_CENTER,        /* 13 */
    BG_SACRAL,          /* 14 */
    BG_G_CENTER,        /* 15 */
    BG_THROAT,          /* 16 */
    BG_AJNA,            /* 17 */
    BG_SPLEEN,          /* 18 */
    BG_ROOT,            /* 19 */
    BG_THROAT,          /* 20 */
    BG_HEART,           /* 21 */
    BG_SOLAR_PLEXUS,    /* 22 */
    BG_THROAT,          /* 23 */
    BG_AJNA,            /* 24 */
    BG_G_CENTER,        /* 25 */
    BG_HEART,           /* 26 */
    BG_SACRAL,          /* 27 */
    BG_SPLEEN,          /* 28 */
    BG_SACRAL,          /* 29 */
    BG_SOLAR_PLEXUS,    /* 30 */
    BG_THROAT,          /* 31 */
    BG_SPLEEN,          /* 32 */
    BG_THROAT,          /* 33 */
    BG_SACRAL,          /* 34 */
    BG_THROAT,          /* 35 */
    BG_SOLAR_PLEXUS,    /* 36 */
    BG_SOLAR_PLEXUS,    /* 37 */
    BG_ROOT,            /* 38 */
    BG_ROOT,            /* 39 */
    BG_HEART,           /* 40 */
    BG_ROOT,            /* 41 */
    BG_SACRAL,          /* 42 */
    BG_AJNA,            /* 43 */
    BG_SPLEEN,          /* 44 */
    BG_THROAT,          /* 45 */
    BG_G_CENTER,        /* 46 */
    BG_AJNA,            /* 47 */
    BG_SPLEEN,          /* 48 */
    BG_SOLAR_PLEXUS,    /* 49 */
    BG_SPLEEN,          /* 50 */
    BG_HEART,           /* 51 */
    BG_ROOT,            /* 52 */
    BG_ROOT,            /* 53 */
    BG_ROOT,            /* 54 */
    BG_SOLAR_PLEXUS,    /* 55 */
    BG_THROAT,          /* 56 */
    BG_SPLEEN,          /* 57 */
    BG_ROOT,            /* 58 */
    BG_SACRAL,          /* 59 */
    BG_ROOT,            /* 60 */
    BG_HEAD,            /* 61 */
    BG_THROAT,          /* 62 */
    BG_HEAD,            /* 63 */
    BG_HEAD,            /* 64 */
};

bg_center_t bodygraph_center(bg_center_id_t id)
{
    if (id < 0 || id >= BG_CENTER_COUNT) {
        bg_center_t invalid = { BG_CENTER_COUNT, "Unknown", {0.0f, 0.0f}, 0, 0, 0 };
        return invalid;
    }
    return CENTERS[id];
}

const char *bodygraph_center_name(bg_center_id_t id)
{
    if (id < 0 || id >= BG_CENTER_COUNT) {
        return "Unknown";
    }
    return CENTERS[id].name;
}

bg_point_t bodygraph_center_position(bg_center_id_t id)
{
    if (id < 0 || id >= BG_CENTER_COUNT) {
        bg_point_t zero = {0.0f, 0.0f};
        return zero;
    }
    return CENTERS[id].position;
}

bg_channel_t bodygraph_channel(int index)
{
    if (index < 0 || index >= BODYGRAPH_CHANNEL_COUNT) {
        bg_channel_t invalid = { 0, 0, BG_CENTER_COUNT, BG_CENTER_COUNT };
        return invalid;
    }
    return CHANNELS[index];
}

int bodygraph_channel_count(void)
{
    return BODYGRAPH_CHANNEL_COUNT;
}

bg_center_id_t bodygraph_gate_center(int gate_number)
{
    if (gate_number < 1 || gate_number > 64) {
        return BG_CENTER_COUNT;
    }
    return GATE_CENTERS[gate_number];
}

bg_gate_pos_t bodygraph_gate_position(int gate_number)
{
    if (gate_number < 1 || gate_number > 64) {
        bg_gate_pos_t invalid = { 0, BG_CENTER_COUNT, {0.0f, 0.0f} };
        return invalid;
    }

    bg_center_id_t center = GATE_CENTERS[gate_number];
    bg_point_t center_pos = CENTERS[center].position;

    /* Place gate near its center with a small offset based on gate number.
     * This produces unique positions per gate within the center area. */
    float radius = 0.04f;

    bg_gate_pos_t gp;
    gp.gate_number = gate_number;
    gp.center = center;
    gp.position.x = center_pos.x + radius * ((float)(gate_number % 7) - 3.0f) / 3.0f * 0.05f;
    gp.position.y = center_pos.y + radius * ((float)(gate_number % 5) - 2.0f) / 2.0f * 0.05f;

    return gp;
}

int bodygraph_channel_for_gates(int gate_a, int gate_b)
{
    for (int i = 0; i < BODYGRAPH_CHANNEL_COUNT; i++) {
        if ((CHANNELS[i].gate_a == gate_a && CHANNELS[i].gate_b == gate_b) ||
            (CHANNELS[i].gate_a == gate_b && CHANNELS[i].gate_b == gate_a)) {
            return i;
        }
    }
    return -1;
}

void bodygraph_defined_centers(const int *active_gates, int *defined_centers)
{
    for (int i = 0; i < BODYGRAPH_CENTER_COUNT; i++) {
        defined_centers[i] = 0;
    }

    for (int i = 0; i < BODYGRAPH_CHANNEL_COUNT; i++) {
        if (active_gates[CHANNELS[i].gate_a] && active_gates[CHANNELS[i].gate_b]) {
            defined_centers[CHANNELS[i].center_a] = 1;
            defined_centers[CHANNELS[i].center_b] = 1;
        }
    }
}

int bodygraph_active_channel_count(const int *active_gates)
{
    int count = 0;
    for (int i = 0; i < BODYGRAPH_CHANNEL_COUNT; i++) {
        if (active_gates[CHANNELS[i].gate_a] && active_gates[CHANNELS[i].gate_b]) {
            count++;
        }
    }
    return count;
}
