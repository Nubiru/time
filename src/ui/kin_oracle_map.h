#ifndef TIME_KIN_ORACLE_MAP_H
#define TIME_KIN_ORACLE_MAP_H

/* Kin Oracle Map — oracle relationship visualization with formulas.
 *
 * Shows how the 5 oracle positions (Analog, Antipode, Occult, Guide,
 * Hidden) are mathematically derived from the destiny seal/kin.
 * Each relationship is annotated with its formula for display.
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_ORACLE_RULES 5

typedef struct {
    const char *position;   /* "Analog", "Antipode", "Occult", "Guide", "Hidden" */
    const char *formula;    /* human-readable formula */
    int source_kin;         /* destiny kin */
    int result_kin;         /* computed oracle kin */
    int result_seal;        /* seal of result kin */
    int result_tone;        /* tone of result kin */
    int result_color;       /* color of result kin (0-3) */
} kin_oracle_rule_t;

typedef struct {
    int destiny_kin;               /* the kin being analyzed (1-260) */
    int destiny_seal;              /* seal of destiny */
    int destiny_tone;              /* tone of destiny */
    int destiny_color;             /* color of destiny */
    kin_oracle_rule_t rules[KIN_ORACLE_RULES]; /* 5 oracle relationships */
} kin_oracle_map_t;

/* Compute the oracle relationship map for a kin (1-260).
 * Shows the 5 oracle relationships with formulas:
 *   0 "Analog":   "Seal 19-S" -> partner seal, same tone -> kin
 *   1 "Antipode": "Seal S+10" -> challenge seal, same tone -> kin
 *   2 "Occult":   "Kin 261-K" -> hidden power kin
 *   3 "Guide":    "Same color, tone" -> guide kin
 *   4 "Hidden":   "Kin 261-K" -> hidden teacher
 * Uses dreamspell_oracle() for actual computation.
 * Returns zeroed struct for invalid kin. */
kin_oracle_map_t kin_oracle_map_compute(int kin);

#endif /* TIME_KIN_ORACLE_MAP_H */
