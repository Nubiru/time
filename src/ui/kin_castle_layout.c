#include "kin_castle_layout.h"
#include "../systems/tzolkin/dreamspell.h"
#include <string.h>

/*
 * Kin Castle Layout — castle frame with 4 wavespell sub-sections.
 *
 * Castle grouping: 5 castles of 52 kin each (4 wavespells x 13 kin).
 *   Castle 1: kin 1-52   (Red Castle of Turning)
 *   Castle 2: kin 53-104 (White Castle of Crossing)
 *   Castle 3: kin 105-156 (Blue Castle of Burning)
 *   Castle 4: kin 157-208 (Yellow Castle of Giving)
 *   Castle 5: kin 209-260 (Green Castle of Enchantment)
 *
 * Wavespell layout: 4 horizontal strips stacked vertically.
 *
 * Pure: no GL, no malloc, no globals, no side effects.
 */

/* Wavespell Y positions: evenly spaced from top to bottom */
static const float WS_Y[KIN_CASTLE_WAVESPELLS] = {
    0.05f, 0.275f, 0.50f, 0.725f
};

static const float WS_X = 0.05f;
static const float WS_W = 0.90f;
static const float WS_H = 0.20f;

kin_castle_layout_t kin_castle_compute(int kin)
{
    kin_castle_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    if (kin < 1 || kin > 260)
        return layout;

    /* Get castle info from dreamspell */
    dreamspell_castle_t castle = dreamspell_castle(kin);

    layout.castle_number = castle.number;
    layout.name = castle.name;
    layout.color = castle.color;
    layout.theme = castle.theme;
    layout.start_kin = castle.start_kin;
    layout.end_kin = castle.end_kin;
    layout.today_kin = kin;

    /* Position within castle (1-52) */
    layout.today_position = kin - castle.start_kin + 1;

    /* Which wavespell within the castle (0-3) */
    layout.today_wavespell = (kin - castle.start_kin) / 13;

    /* Populate 4 wavespells */
    for (int i = 0; i < KIN_CASTLE_WAVESPELLS; i++) {
        int ws_start = castle.start_kin + i * 13;

        /* Get wavespell info */
        dreamspell_wavespell_t ws_info = dreamspell_wavespell(ws_start);

        layout.wavespells[i].wavespell_number = ws_info.number;
        layout.wavespells[i].start_kin = ws_start;
        layout.wavespells[i].seal = (ws_start - 1) % 20;
        layout.wavespells[i].purpose = ws_info.purpose;

        /* Layout geometry */
        layout.wavespells[i].x = WS_X;
        layout.wavespells[i].y = WS_Y[i];
        layout.wavespells[i].w = WS_W;
        layout.wavespells[i].h = WS_H;

        /* Mark which wavespell contains today's kin */
        layout.wavespells[i].contains_today =
            (kin >= ws_start && kin < ws_start + 13) ? 1 : 0;
    }

    return layout;
}

int kin_castle_number(int kin)
{
    if (kin < 1 || kin > 260)
        return 0;
    return (kin - 1) / 52 + 1;
}
