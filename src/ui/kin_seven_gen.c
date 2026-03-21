#include "kin_seven_gen.h"
#include "kin_cell.h"
#include "../systems/tzolkin/dreamspell.h"
#include <string.h>

/* Level names. */
static const char *LEVEL_NAMES[KIN_SEVEN_GEN_LEVELS] = {
    "Crown", "Heart", "Root"
};

/* Center Y positions per level. */
static const float LEVEL_CENTER_Y[KIN_SEVEN_GEN_LEVELS] = {
    0.15f, 0.50f, 0.85f
};

/* Fill a cell from a kin number. */
static kin_seven_gen_cell_t fill_cell(int kin)
{
    kin_seven_gen_cell_t cell;
    memset(&cell, 0, sizeof(cell));

    if (kin < 1 || kin > 260)
        return cell;

    cell.kin = kin;
    cell.seal = (kin - 1) % 20;
    cell.tone = (kin - 1) % 13 + 1;
    cell.color = cell.seal % 4;
    return cell;
}

/* Fill a level from an oracle. */
static kin_seven_gen_level_t fill_level(int level_index, dreamspell_oracle_t oracle)
{
    kin_seven_gen_level_t level;
    memset(&level, 0, sizeof(level));

    level.level_name = LEVEL_NAMES[level_index];
    level.destiny  = fill_cell(oracle.destiny);
    level.guide    = fill_cell(oracle.guide);
    level.analog   = fill_cell(oracle.analog);
    level.antipode = fill_cell(oracle.antipode);
    level.occult   = fill_cell(oracle.occult);
    level.center_x = 0.5f;
    level.center_y = LEVEL_CENTER_Y[level_index];

    return level;
}

kin_seven_gen_t kin_seven_gen_compute(int kin)
{
    kin_seven_gen_t result;
    memset(&result, 0, sizeof(result));

    if (kin < 1 || kin > 260)
        return result;

    result.source_kin = kin;

    /* Level 0 (Crown): oracle of the kin itself */
    dreamspell_oracle_t oracle0 = dreamspell_oracle(kin);
    result.levels[0] = fill_level(0, oracle0);

    /* Level 1 (Heart): oracle of the kin's analog partner */
    dreamspell_oracle_t oracle1 = dreamspell_oracle(oracle0.analog);
    result.levels[1] = fill_level(1, oracle1);

    /* Level 2 (Root): oracle of the kin's guide */
    dreamspell_oracle_t oracle2 = dreamspell_oracle(oracle0.guide);
    result.levels[2] = fill_level(2, oracle2);

    return result;
}
