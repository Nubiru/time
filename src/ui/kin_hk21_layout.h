#ifndef TIME_KIN_HK21_LAYOUT_H
#define TIME_KIN_HK21_LAYOUT_H

/* Hunab Ku 21 Galactic Tree layout — 21 archetype positions.
 *
 * The Galactic Tree of Life: 20 solar seals + Hunab Ku center arranged
 * in 5 concentric rings across 4 domains of wisdom.
 *
 *   Ring 1 (Gates of Light, seals 1-4):      y=0.1
 *   Ring 2 (Gates of Power, seals 5,8,11,14): y=0.3
 *   Ring 3 (Sources of Power, seals 6,9,12,15): y=0.5
 *   Ring 4 (Seats of Power, seals 7,10,13,16): y=0.7
 *   Ring 5 (Portal Matrix, seals 17-20):     y=0.9
 *   Center (Hunab Ku, seal 21):              (0.5, 0.5)
 *
 * Within each ring: 4 nodes at x = 0.15, 0.38, 0.62, 0.85
 *
 * Source: Arguelles, "Hunab Ku 21 Playing Board"
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_HK21_NODES 21

typedef struct {
    int seal_code;                /* 1-21 */
    const char *seal_name;        /* "Dragon", "Wind", ... "Hunab Ku" */
    int hk_number;                /* 90-441 */
    int role;                     /* hk21_role_t value */
    int domain;                   /* hk21_domain_t value */
    const char *statement;        /* "Gate of Being", etc. */
    const char *planet;           /* "Neptune", etc. */
    float x, y;                   /* position in layout */
    int highlighted;              /* 1 if today's seal maps here */
} kin_hk21_node_t;

typedef struct {
    kin_hk21_node_t nodes[KIN_HK21_NODES]; /* 21 positions */
    int today_seal;               /* 0-19 (Tzolkin seal) */
    int today_node;               /* 0-19: which node corresponds to today (-1 if none) */
} kin_hk21_layout_t;

/* Compute the Hunab Ku 21 tree layout for a seal (0-19).
 * 21 nodes arranged in a diamond/mandala pattern.
 * Today's Tzolkin seal (0-19) maps to node seal_code (1-20).
 * Returns zeroed struct for invalid seal (-1, 20+). */
kin_hk21_layout_t kin_hk21_compute(int today_seal);

/* Map Tzolkin seal (0-19) to Hunab Ku seal code (1-20).
 * Always seal+1. Returns -1 for invalid seal. */
int kin_hk21_seal_to_code(int tzolkin_seal);

#endif /* TIME_KIN_HK21_LAYOUT_H */
