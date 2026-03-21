#include "kin_wavespell_layout.h"
#include "../systems/tzolkin/dreamspell.h"
#include <stddef.h>
#include <string.h>

kin_wavespell_layout_t kin_wavespell_compute(int kin)
{
    kin_wavespell_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    if (kin < 1 || kin > 260)
        return layout;

    dreamspell_wavespell_t ws = dreamspell_wavespell(kin);
    int position = dreamspell_wavespell_position(kin);
    int start = kin - (position - 1);

    layout.wavespell_number = ws.number;
    layout.today_position = position;
    layout.start_kin = start;
    layout.purpose = ws.purpose;

    for (int i = 0; i < KIN_WS_CELLS; i++) {
        float x = 0.04f + (float)i * (0.92f / 12.0f);
        layout.cells[i] = kin_cell_create(start + i, x, 0.5f, 0.065f, 0.25f);
    }

    layout.cells[position - 1].highlighted = 1;

    return layout;
}

const char *kin_wavespell_tone_name(int position)
{
    dreamspell_tone_t t = dreamspell_tone(position);
    return t.name;
}

int kin_wavespell_is_power_day(int position)
{
    return (position == 1 || position == 5 ||
            position == 9 || position == 13) ? 1 : 0;
}
