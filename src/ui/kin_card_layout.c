#include "kin_card_layout.h"
#include "kin_cell.h"
#include "../systems/tzolkin/dreamspell.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/tzolkin/tzolkin_board.h"
#include <string.h>
#include <stdio.h>

const char *kin_card_full_name(int seal, char *buf, int buf_size)
{
    if (seal < 0 || seal > 19 || buf == NULL || buf_size < 1)
        return "";

    /* tzolkin_seal_name already returns "Red Dragon" etc. — use directly. */
    const char *full = tzolkin_seal_name(seal);
    snprintf(buf, (size_t)buf_size, "%s", full);
    return buf;
}

kin_card_layout_t kin_card_compute(int kin)
{
    kin_card_layout_t card;
    memset(&card, 0, sizeof(card));

    if (kin < 1 || kin > 260)
        return card;

    /* Identity */
    card.kin  = kin;
    card.seal = (kin - 1) % 20;
    card.tone = ((kin - 1) % 13) + 1;
    card.color = card.seal % 4;

    /* Title: "Kin 131" */
    snprintf(card.title, KIN_CARD_MAX_TEXT, "Kin %d", kin);

    /* Seal name: "Blue Monkey" (from tzolkin_seal_name which includes color) */
    kin_card_full_name(card.seal, card.seal_name, KIN_CARD_MAX_TEXT);

    /* Tone name: "Magnetic", "Lunar", etc. */
    dreamspell_tone_t dt = dreamspell_tone(card.tone);
    snprintf(card.tone_name, KIN_CARD_MAX_TEXT, "%s", dt.name);

    /* Tone dots = tone value */
    card.tone_dots = card.tone;

    /* Oracle */
    dreamspell_oracle_t oracle = dreamspell_oracle(kin);
    card.guide_kin    = oracle.guide;
    card.analog_kin   = oracle.analog;
    card.antipode_kin = oracle.antipode;
    card.occult_kin   = oracle.occult;

    /* Oracle summary: "Guide: [color] [seal]" of guide kin */
    {
        int guide_seal = (oracle.guide - 1) % 20;
        char guide_name[48];
        kin_card_full_name(guide_seal, guide_name, (int)sizeof(guide_name));
        snprintf(card.oracle_summary, KIN_CARD_MAX_TEXT,
                 "Guide: %s", guide_name);
    }

    /* Wavespell info: "WS 11 / Pos 1" */
    {
        dreamspell_wavespell_t ws = dreamspell_wavespell(kin);
        int pos = dreamspell_wavespell_position(kin);
        snprintf(card.wavespell_info, KIN_CARD_MAX_TEXT,
                 "WS %d / Pos %d", ws.number, pos);
    }

    /* Castle name */
    {
        dreamspell_castle_t castle = dreamspell_castle(kin);
        snprintf(card.castle_name, KIN_CARD_MAX_TEXT, "%s", castle.name);
    }

    /* Portal flag */
    card.is_portal = tzolkin_board_is_portal(kin) ? 1 : 0;

    /* Power day: wavespell positions 1, 5, 9, 13 */
    {
        int pos = dreamspell_wavespell_position(kin);
        card.is_power_day = (pos == 1 || pos == 5 ||
                             pos == 9 || pos == 13) ? 1 : 0;
    }

    return card;
}
