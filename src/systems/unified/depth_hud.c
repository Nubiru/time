/* depth_hud.c — HUD integration data for knowledge systems
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Visual design spec, depth tier system, today_summary system IDs. */

#include "depth_hud.h"

#include <stddef.h>  /* NULL */

/* Static table of all 20 system HUD specs, indexed by ts_system_t. */
static const dh_system_spec_t s_specs[TS_SYS_COUNT] = {
    /* TS_SYS_GREGORIAN (0) */
    { TS_SYS_GREGORIAN,  "Gregorian",    '0', HUD_POS_TOP_CENTER,
      "{date}", "{weekday} {date}",
      "{weekday} {date} {year}", 0 },

    /* TS_SYS_TZOLKIN (1) */
    { TS_SYS_TZOLKIN,    "Tzolkin",      'T', HUD_POS_TOP_RIGHT,
      "{kin}", "Kin {kin} {seal}",
      "Kin {kin} {seal} {tone} — {wavespell}", 1 },

    /* TS_SYS_HAAB (2) */
    { TS_SYS_HAAB,       "Haab",         '.', HUD_POS_TOP_RIGHT,
      "{month}", "{day} {month}",
      "{day} {month} — {meaning}", 12 },

    /* TS_SYS_CHINESE (3) */
    { TS_SYS_CHINESE,    "Chinese",      'C', HUD_POS_BOTTOM_LEFT,
      "{animal}", "{element} {animal}",
      "{stem}{branch} {element} {animal} — {year}", 5 },

    /* TS_SYS_HEBREW (4) */
    { TS_SYS_HEBREW,     "Hebrew",       'H', HUD_POS_LEFT_PANEL,
      "{letter}", "{year} {month} {day}",
      "{day} {month} {year} — {parasha}", 3 },

    /* TS_SYS_ISLAMIC (5) */
    { TS_SYS_ISLAMIC,    "Islamic",      'I', HUD_POS_LEFT_PANEL,
      "{month#}", "{year} {month} {day}",
      "{day} {month} {year} AH — {event}", 4 },

    /* TS_SYS_BUDDHIST (6) */
    { TS_SYS_BUDDHIST,   "Buddhist",     'B', HUD_POS_LEFT_PANEL,
      "BE", "BE {year} · {uposatha}",
      "BE {year} · {uposatha} — {observance}", 6 },

    /* TS_SYS_HINDU (7) */
    { TS_SYS_HINDU,      "Hindu/Vedic",  'V', HUD_POS_LEFT_PANEL,
      "{tithi}", "{tithi} · {nakshatra}",
      "{tithi} · {nakshatra} — {yoga}", 7 },

    /* TS_SYS_ICHING (8) */
    { TS_SYS_ICHING,     "I Ching",      'X', HUD_POS_TOP_RIGHT,
      "{hex#}", "Hex {n} {name}",
      "Hex {n} {name} — {judgment}", 2 },

    /* TS_SYS_ASTROLOGY (9) */
    { TS_SYS_ASTROLOGY,  "Astrology",    'A', HUD_POS_BOTTOM_LEFT,
      "{sun}", "{sun_sign} {moon_sign}",
      "{sun_sign} Sun · {moon_sign} Moon — {rising}", 1 },

    /* TS_SYS_HUMAN_DESIGN (10) */
    { TS_SYS_HUMAN_DESIGN, "Human Design", 'D', HUD_POS_RIGHT_PANEL,
      "{gate}", "Gate {n} · {name}",
      "Gate {n} · {name} — Line {line}", 8 },

    /* TS_SYS_KABBALAH (11) */
    { TS_SYS_KABBALAH,   "Kabbalah",     'Q', HUD_POS_RIGHT_PANEL,
      "{sef}", "{sefirah} · {world}",
      "{sefirah} · {world} — {path}", 9 },

    /* TS_SYS_COPTIC (12) */
    { TS_SYS_COPTIC,     "Coptic",       ',', HUD_POS_LEFT_PANEL,
      "{month}", "{year} {month} {day}",
      "{day} {month} {year} AM — {saint}", 13 },

    /* TS_SYS_ETHIOPIAN (13) */
    { TS_SYS_ETHIOPIAN,  "Ethiopian",    ';', HUD_POS_LEFT_PANEL,
      "{month}", "{year} {month} {day}",
      "{day} {month} {year} — {feast}", 14 },

    /* TS_SYS_PERSIAN (14) */
    { TS_SYS_PERSIAN,    "Persian",      'P', HUD_POS_LEFT_PANEL,
      "{month}", "{year} {month} {day}",
      "{day} {month} {year} SH — {festival}", 10 },

    /* TS_SYS_JAPANESE (15) */
    { TS_SYS_JAPANESE,   "Japanese",     'J', HUD_POS_BOTTOM_RIGHT,
      "{era}", "{era} {year}",
      "{era} {year} — {season}", 11 },

    /* TS_SYS_KOREAN (16) */
    { TS_SYS_KOREAN,     "Korean",       'K', HUD_POS_BOTTOM_RIGHT,
      "{year}", "Dangun {year}",
      "Dangun {year} — {holiday}", 15 },

    /* TS_SYS_THAI (17) */
    { TS_SYS_THAI,       "Thai",         'Y', HUD_POS_BOTTOM_RIGHT,
      "BE", "BE {year}",
      "BE {year} — {observance}", 16 },

    /* TS_SYS_GEOLOGICAL (18) */
    { TS_SYS_GEOLOGICAL, "Geological",   'G', HUD_POS_BOTTOM_CENTER,
      "{era}", "{eon} · {era}",
      "{eon} · {era} · {period} — {epoch}", 17 },

    /* TS_SYS_COSMIC (19) */
    { TS_SYS_COSMIC,     "Cosmic",       '/', HUD_POS_BOTTOM_CENTER,
      "{age}", "{age} Gy",
      "{age} Gy — {epoch}", 18 },

    /* TS_SYS_EARTH (20) */
    { TS_SYS_EARTH,      "Earth",        'e', HUD_POS_BOTTOM_CENTER,
      "{season}", "{season} ({progress}%)",
      "{season} — {daylight}h daylight", 19 },

    /* TS_SYS_ASTRONOMY (21) */
    { TS_SYS_ASTRONOMY,  "Astronomy",    '*', HUD_POS_TOP_RIGHT,
      "{phase}", "{phase} ({illum}%)",
      "{phase} — {age} days", 20 },

    /* TS_SYS_TAROT (22) */
    { TS_SYS_TAROT,      "Tarot",        'T', HUD_POS_RIGHT_PANEL,
      "{card}", "{name} ({n})",
      "{name} — {hebrew} — Path {path}", 21 },

    /* TS_SYS_NUMEROLOGY (23) */
    { TS_SYS_NUMEROLOGY, "Numerology",   'N', HUD_POS_RIGHT_PANEL,
      "{root}", "Day {root}",
      "Day {root} — {meaning}", 22 },

    /* TS_SYS_CHAKRA (24) */
    { TS_SYS_CHAKRA,     "Chakra",       'U', HUD_POS_RIGHT_PANEL,
      "{name}", "{sanskrit}",
      "{sanskrit} ({english}) — {theme}", 23 },

    /* TS_SYS_ZOROASTRIAN (25) */
    { TS_SYS_ZOROASTRIAN, "Zoroastrian", 'Z', HUD_POS_LEFT_PANEL,
      "{month}", "{year} {month} {day}",
      "{day} {month} {year} — {festival}", 24 },

    /* TS_SYS_BALINESE (26) */
    { TS_SYS_BALINESE,   "Balinese",     'W', HUD_POS_LEFT_PANEL,
      "{wuku}", "{wuku} day {d}",
      "{wuku} — day {d}/210", 25 },

    /* TS_SYS_FRENCH_REPUBLICAN (27) */
    { TS_SYS_FRENCH_REPUBLICAN, "French Republican", 'F', HUD_POS_BOTTOM_RIGHT,
      "{month}", "An {year} {month}",
      "{day} {month} An {year}", 26 },

    /* TS_SYS_AZTEC (28) */
    { TS_SYS_AZTEC,      "Aztec",        'z', HUD_POS_LEFT_PANEL,
      "{sign}", "{num} {sign}",
      "{num} {sign} — {meaning}", 27 },

    /* TS_SYS_EGYPTIAN (29) */
    { TS_SYS_EGYPTIAN,   "Egyptian",     'E', HUD_POS_BOTTOM_RIGHT,
      "{month}", "{year} {month} {day}",
      "{day} {month} Year {year}", 28 },

    /* TS_SYS_CELTIC (30) */
    { TS_SYS_CELTIC,     "Celtic",       'c', HUD_POS_BOTTOM_LEFT,
      "{tree}", "{tree} day {d}",
      "{tree} — {ogham}", 29 },

    /* TS_SYS_LAO (31) */
    { TS_SYS_LAO,        "Lao",          'l', HUD_POS_BOTTOM_RIGHT,
      "BE", "BE {year}",
      "BE {year} — {festival}", 30 },

    /* TS_SYS_MYANMAR (32) */
    { TS_SYS_MYANMAR,    "Myanmar",      'm', HUD_POS_BOTTOM_RIGHT,
      "{month}", "ME {year}",
      "ME {year} {month} {day}", 31 },

    /* TS_SYS_BAHAI (33) */
    { TS_SYS_BAHAI,      "Baha'i",       'b', HUD_POS_LEFT_PANEL,
      "{month}", "{year} {month} {day}",
      "{day} {month} {year} BE — {holy}", 32 },

    /* TS_SYS_TAMIL (34) */
    { TS_SYS_TAMIL,      "Tamil",        't', HUD_POS_BOTTOM_LEFT,
      "{month}", "{year} {month}",
      "{day} {month} {year}", 33 }
};

/* Invalid/sentinel spec */
static const dh_system_spec_t s_invalid = {
    -1, "?", '\0', HUD_POS_TOP_LEFT,
    "", "", "", -1
};

dh_system_spec_t dh_system_spec(int system_id)
{
    if (system_id < 0 || system_id >= TS_SYS_COUNT) {
        return s_invalid;
    }
    return s_specs[system_id];
}

int dh_system_by_key(char key)
{
    if (key == '\0') {
        return -1;
    }
    for (int i = 0; i < TS_SYS_COUNT; i++) {
        if (s_specs[i].toggle_key == key) {
            return i;
        }
    }
    return -1;
}

int dh_systems_at_position(hud_position_t pos, int *out_ids, int max)
{
    if ((int)pos < 0 || (int)pos >= HUD_POS_COUNT) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < TS_SYS_COUNT; i++) {
        if (s_specs[i].position == pos) {
            if (out_ids != NULL && count < max) {
                out_ids[count] = i;
            }
            count++;
        }
    }
    /* If out_ids is NULL, return total count; otherwise return filled count */
    if (out_ids == NULL) {
        return count;
    }
    return (count < max) ? count : max;
}

const char *dh_position_name(hud_position_t pos)
{
    switch (pos) {
    case HUD_POS_TOP_LEFT:       return "Top Left";
    case HUD_POS_TOP_CENTER:     return "Top Center";
    case HUD_POS_TOP_RIGHT:      return "Top Right";
    case HUD_POS_BOTTOM_LEFT:    return "Bottom Left";
    case HUD_POS_BOTTOM_CENTER:  return "Bottom Center";
    case HUD_POS_BOTTOM_RIGHT:   return "Bottom Right";
    case HUD_POS_LEFT_PANEL:     return "Left Panel";
    case HUD_POS_RIGHT_PANEL:    return "Right Panel";
    default:                     return "?";
    }
}

int dh_system_count(void)
{
    return TS_SYS_COUNT;
}

int dh_position_count(void)
{
    return HUD_POS_COUNT;
}
