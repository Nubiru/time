#include "kin_oracle_layout.h"
#include "../systems/tzolkin/dreamspell.h"

/* Oracle cross positions (normalized 0-1 coordinates). */
#define DESTINY_X   0.50f
#define DESTINY_Y   0.50f
#define GUIDE_X     0.50f
#define GUIDE_Y     0.15f
#define ANALOG_X    0.85f
#define ANALOG_Y    0.50f
#define ANTIPODE_X  0.15f
#define ANTIPODE_Y  0.50f
#define OCCULT_X    0.50f
#define OCCULT_Y    0.85f

/* Cell dimensions. */
#define CELL_W      0.25f
#define CELL_H      0.20f

/* Position names. */
static const char *POSITION_NAMES[KIN_ORACLE_POSITIONS] = {
    "Destiny", "Guide", "Analog", "Antipode", "Occult"
};

kin_oracle_layout_t kin_oracle_compute(int kin)
{
    kin_oracle_layout_t layout;
    kin_cell_t zero = {0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0};

    if (kin < 1 || kin > 260) {
        layout.destiny  = zero;
        layout.guide    = zero;
        layout.analog   = zero;
        layout.antipode = zero;
        layout.occult   = zero;
        layout.hidden_kin = 0;
        return layout;
    }

    dreamspell_oracle_t oracle = dreamspell_oracle(kin);

    layout.destiny  = kin_cell_create(oracle.destiny,  DESTINY_X,  DESTINY_Y,  CELL_W, CELL_H);
    layout.guide    = kin_cell_create(oracle.guide,     GUIDE_X,    GUIDE_Y,    CELL_W, CELL_H);
    layout.analog   = kin_cell_create(oracle.analog,    ANALOG_X,   ANALOG_Y,   CELL_W, CELL_H);
    layout.antipode = kin_cell_create(oracle.antipode,  ANTIPODE_X, ANTIPODE_Y, CELL_W, CELL_H);
    layout.occult   = kin_cell_create(oracle.occult,    OCCULT_X,   OCCULT_Y,   CELL_W, CELL_H);

    /* Destiny is the active/highlighted kin. */
    layout.destiny.highlighted = 1;

    layout.hidden_kin = 261 - kin;

    return layout;
}

const char *kin_oracle_position_name(int index)
{
    if (index < 0 || index >= KIN_ORACLE_POSITIONS)
        return "?";
    return POSITION_NAMES[index];
}

kin_cell_t kin_oracle_cell_at(const kin_oracle_layout_t *layout, int index)
{
    kin_cell_t zero = {0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0};
    if (!layout || index < 0 || index >= KIN_ORACLE_POSITIONS)
        return zero;

    switch (index) {
    case 0: return layout->destiny;
    case 1: return layout->guide;
    case 2: return layout->analog;
    case 3: return layout->antipode;
    case 4: return layout->occult;
    default: return zero;
    }
}
