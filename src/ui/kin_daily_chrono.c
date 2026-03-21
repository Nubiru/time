#include "kin_daily_chrono.h"
#include "kin_cell.h"
#include "../systems/tzolkin/dreamspell.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/tzolkin/thirteen_moon.h"
#include "../systems/tzolkin/heptad_gate.h"
#include <stdio.h>
#include <string.h>

/* Fill a kin_chrono_oracle_t from a kin number (1-260). */
static void fill_oracle_position(kin_chrono_oracle_t *pos, int kin)
{
    memset(pos, 0, sizeof(*pos));
    if (kin < 1 || kin > 260)
        return;

    pos->kin = kin;
    pos->seal = (kin - 1) % 20;
    pos->tone = (kin - 1) % 13 + 1;
    pos->color = kin_cell_color(pos->seal);

    dreamspell_color_t dc = dreamspell_color(pos->seal);
    const char *seal_name = tzolkin_seal_name(pos->seal);

    snprintf(pos->name, KIN_CHRONO_MAX_TEXT, "%s %s", dc.name, seal_name);
}

kin_daily_chrono_t kin_daily_chrono_compute(double jd)
{
    kin_daily_chrono_t c;
    memset(&c, 0, sizeof(c));

    if (jd < 0.0)
        return c;

    /* Tzolkin identity */
    tzolkin_day_t tz = tzolkin_from_jd(jd);
    c.kin = tz.kin;
    c.seal = tz.seal;
    c.tone = tz.tone;
    c.color = kin_cell_color(c.seal);

    /* Build full kin name: "Blue Magnetic Monkey" */
    dreamspell_color_t dc = dreamspell_color(c.seal);
    dreamspell_tone_t dt = dreamspell_tone(c.tone);
    const char *seal_name = tzolkin_seal_name(c.seal);

    snprintf(c.kin_name, KIN_CHRONO_MAX_TEXT, "%s %s %s",
             dc.name, dt.name, seal_name);

    /* Tone action and power */
    snprintf(c.tone_action, KIN_CHRONO_MAX_TEXT, "%s", dt.action);
    snprintf(c.tone_power, KIN_CHRONO_MAX_TEXT, "%s", dt.power);

    /* Color action */
    snprintf(c.color_action, KIN_CHRONO_MAX_TEXT, "%s", dc.action);

    /* Oracle: 5 positions */
    dreamspell_oracle_t oracle = dreamspell_oracle(c.kin);
    fill_oracle_position(&c.destiny, oracle.destiny);
    fill_oracle_position(&c.guide, oracle.guide);
    fill_oracle_position(&c.analog, oracle.analog);
    fill_oracle_position(&c.antipode, oracle.antipode);
    fill_oracle_position(&c.occult, oracle.occult);

    /* 13-Moon context */
    thirteen_moon_t tm = thirteen_moon_from_jd(jd);
    c.moon = tm.moon;
    c.moon_day = tm.day;
    c.is_day_out = tm.is_day_out;

    if (tm.moon >= 1 && tm.moon <= 13) {
        snprintf(c.moon_name, KIN_CHRONO_MAX_TEXT, "%s Moon",
                 thirteen_moon_name(tm.moon));
    }

    /* Plasma / Heptad Gate */
    c.plasma = tm.plasma;
    if (!tm.is_day_out && tm.day_of_week >= 1 && tm.day_of_week <= 7) {
        heptad_gate_t gate = heptad_gate_for_day(tm.day_of_week);
        snprintf(c.plasma_name, KIN_CHRONO_MAX_TEXT, "%s",
                 thirteen_moon_plasma_name(tm.plasma));
        snprintf(c.plasma_chakra, KIN_CHRONO_MAX_TEXT, "%s",
                 thirteen_moon_plasma_chakra(tm.plasma));
        snprintf(c.plasma_action, KIN_CHRONO_MAX_TEXT, "%s",
                 thirteen_moon_plasma_action(tm.plasma));
        snprintf(c.mantra, KIN_CHRONO_MAX_TEXT, "%s", gate.mantra);
    }

    /* Wavespell + Castle context */
    c.wavespell_position = dreamspell_wavespell_position(c.kin);

    dreamspell_wavespell_t ws = dreamspell_wavespell(c.kin);
    dreamspell_color_t ws_color = dreamspell_color(ws.seal);
    snprintf(c.wavespell_info, KIN_CHRONO_MAX_TEXT, "WS %d: %s %s",
             ws.number, ws_color.name, tzolkin_seal_name(ws.seal));

    dreamspell_castle_t castle = dreamspell_castle(c.kin);
    snprintf(c.castle_info, KIN_CHRONO_MAX_TEXT, "%s", castle.name);

    /* Flags */
    c.is_portal = 0;  /* caller checks separately */
    int pos = c.wavespell_position;
    c.is_power_day = (pos == 1 || pos == 5 || pos == 9 || pos == 13) ? 1 : 0;

    return c;
}
