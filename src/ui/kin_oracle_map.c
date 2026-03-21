#include "kin_oracle_map.h"
#include "../systems/tzolkin/dreamspell.h"
#include <string.h>

/*
 * Kin Oracle Map — oracle relationship visualization with formulas.
 *
 * Decomposes the Dreamspell oracle into 5 named rules, each with
 * a human-readable formula and the computed result kin/seal/tone/color.
 *
 * Pure: no GL, no malloc, no globals, no side effects.
 */

/* Position names for the 5 oracle relationships */
static const char *RULE_POSITIONS[KIN_ORACLE_RULES] = {
    "Analog", "Antipode", "Occult", "Guide", "Hidden"
};

/* Human-readable formulas */
static const char *RULE_FORMULAS[KIN_ORACLE_RULES] = {
    "Seal 19-S",
    "Seal S+10",
    "Kin 261-K",
    "Same color, tone",
    "Kin 261-K"
};

/* Extract seal, tone, color from a kin number */
static void decompose_kin(int kin, int *seal, int *tone, int *color)
{
    *seal = (kin - 1) % 20;
    *tone = (kin - 1) % 13 + 1;
    *color = *seal % 4;
}

kin_oracle_map_t kin_oracle_map_compute(int kin)
{
    kin_oracle_map_t map;
    memset(&map, 0, sizeof(map));

    if (kin < 1 || kin > 260)
        return map;

    /* Destiny decomposition */
    map.destiny_kin = kin;
    decompose_kin(kin, &map.destiny_seal, &map.destiny_tone, &map.destiny_color);

    /* Get oracle from dreamspell engine */
    dreamspell_oracle_t oracle = dreamspell_oracle(kin);

    /* Map oracle positions to result kins */
    int result_kins[KIN_ORACLE_RULES] = {
        oracle.analog,
        oracle.antipode,
        oracle.occult,
        oracle.guide,
        oracle.hidden
    };

    /* Fill each rule */
    for (int i = 0; i < KIN_ORACLE_RULES; i++) {
        map.rules[i].position = RULE_POSITIONS[i];
        map.rules[i].formula = RULE_FORMULAS[i];
        map.rules[i].source_kin = kin;
        map.rules[i].result_kin = result_kins[i];
        decompose_kin(result_kins[i],
                      &map.rules[i].result_seal,
                      &map.rules[i].result_tone,
                      &map.rules[i].result_color);
    }

    return map;
}
