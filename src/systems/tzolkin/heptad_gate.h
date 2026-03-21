#ifndef TIME_HEPTAD_GATE_H
#define TIME_HEPTAD_GATE_H

/* Heptad Gates — Synchronotron/Hunab Ku 21 system.
 *
 * Each of the 7 days of the heptad (week) carries a specific archetype
 * position: a plasma, chakra, seal, mantra, Hunab Ku number, circuit,
 * mental sphere, and consciousness state.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 *         (Boletin Intergalactico No 7).
 */

#define HEPTAD_GATE_COUNT 7

typedef struct {
    int day;                    /* 1-7 (day of heptad) */
    const char *plasma;         /* "DALI", "SELI", etc. */
    int seal_code;              /* solar seal code (1, 7, 2, 13, 20, 18, 21) */
    const char *seal_name;      /* "Dragon", "Hand", etc. */
    int hk_number;              /* Hunab Ku 21 number (108, 291, ...) */
    const char *circuit;        /* "Opens Circuit 8", etc. */
    const char *mental_sphere;  /* "1st Sphere", "2nd Sphere", etc. */
    const char *consciousness;  /* "Pre-conscious", "Sub-conscious", etc. */
    const char *mantra;         /* "OM", "HRAM", etc. */
    const char *chakra;         /* "Crown", "Root", etc. */
} heptad_gate_t;

/* Get full heptad gate data for a day (1-7).
 * Returns zeroed struct for invalid day. */
heptad_gate_t heptad_gate_for_day(int day_of_week);

/* Plasma name for a day (1-7). Returns NULL for invalid. */
const char *heptad_gate_plasma_name(int day_of_week);

/* Mantra for a day (1-7). Returns NULL for invalid. */
const char *heptad_gate_mantra(int day_of_week);

/* Chakra for a day (1-7). Returns NULL for invalid. */
const char *heptad_gate_chakra(int day_of_week);

/* Seal name for a day (1-7). Returns NULL for invalid. */
const char *heptad_gate_seal_name(int day_of_week);

/* Hunab Ku 21 number for a day (1-7). Returns 0 for invalid. */
int heptad_gate_hk_number(int day_of_week);

/* Number of heptad gates (always 7). */
int heptad_gate_count(void);

#endif /* TIME_HEPTAD_GATE_H */
