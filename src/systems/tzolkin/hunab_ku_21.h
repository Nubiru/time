#ifndef TIME_HUNAB_KU_21_H
#define TIME_HUNAB_KU_21_H

/* Hunab Ku 21 — Galactic Tree of Life and Knowledge.
 *
 * 21 archetype positions (20 solar seals + Hunab Ku center) arranged
 * in 5 concentric rings across 4 domains of wisdom. Each position
 * carries a seal, Hunab Ku number, role type, domain, planet, flow
 * direction (Galactic-Karmic or Solar-Prophetic), and statement.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Hunab Ku 21 Playing Board"
 *         Arguelles, "Como Entrar a Hunab Ku 21"
 */

#define HK21_ARCHETYPE_COUNT 21
#define HK21_LENS_COUNT       4

/* --- Role types (5 rings + center) --- */

typedef enum {
    HK21_GATE_OF_LIGHT   = 0,   /* Ring 1: seals 1-4 */
    HK21_GATE_OF_POWER   = 1,   /* Ring 2: seals 5, 8, 11, 14 */
    HK21_SOURCE_OF_POWER = 2,   /* Ring 3: seals 6, 9, 12, 15 */
    HK21_SEAT_OF_POWER   = 3,   /* Ring 4: seals 7, 10, 13, 16 */
    HK21_PORTAL_MATRIX   = 4,   /* Ring 5: seals 17-20 */
    HK21_CENTER           = 5    /* Hunab Ku: seal 21 */
} hk21_role_t;

/* --- Domains of wisdom --- */

typedef enum {
    HK21_KNOWLEDGE    = 0,
    HK21_LOVE         = 1,
    HK21_PROPHECY     = 2,
    HK21_INTELLIGENCE = 3,
    HK21_UNITY        = 4
} hk21_domain_t;

/* --- Flow direction --- */

typedef enum {
    HK21_GALACTIC_KARMIC   = 0,   /* GK — inward */
    HK21_SOLAR_PROPHETIC   = 1,   /* SP — outward */
    HK21_FLOW_NONE         = 2    /* Hunab Ku center */
} hk21_flow_t;

/* --- Archetype data --- */

typedef struct {
    int              seal_code;    /* 1-21 */
    const char      *seal_name;    /* "Dragon", "Wind", ... "Hunab Ku" */
    int              hk_number;    /* 90-441 */
    hk21_role_t      role;
    hk21_domain_t    domain;
    const char      *planet;       /* "Neptune", "Uranus", ... */
    hk21_flow_t      flow;
    const char      *statement;    /* "Gate of Being", etc. */
} hk21_archetype_t;

/* Get full archetype data for a seal code (1-21).
 * Returns zeroed struct for invalid seal code. */
hk21_archetype_t hk21_archetype_for_seal(int seal_code);

/* Number of archetypes (always 21). */
int hk21_archetype_count(void);

/* Role name string. Returns "Unknown" for invalid. */
const char *hk21_role_name(hk21_role_t role);

/* Domain name string. Returns "Unknown" for invalid. */
const char *hk21_domain_name(hk21_domain_t domain);

/* Flow name string. Returns "Unknown" for invalid. */
const char *hk21_flow_name(hk21_flow_t flow);

/* Lens number (lens 1-4 -> 108, 144, 216, 288). Returns 0 for invalid. */
int hk21_lens_number(int lens);

/* Lens name (lens 1-4). Returns NULL for invalid. */
const char *hk21_lens_name(int lens);

/* Domain for a seal code (1-21). Returns HK21_UNITY for invalid. */
hk21_domain_t hk21_domain_for_seal(int seal_code);

/* Hunab Ku number for a seal code (1-21). Returns 0 for invalid. */
int hk21_hk_number_for_seal(int seal_code);

#endif /* TIME_HUNAB_KU_21_H */
