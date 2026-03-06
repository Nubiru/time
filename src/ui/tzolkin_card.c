#include "tzolkin_card.h"
#include "../systems/tzolkin/tzolkin.h"
#include <stdio.h>
#include <string.h>

/*
 * Dreamspell color families: seals cycle Red, White, Blue, Yellow.
 * Seal 0 (Dragon) = Red, Seal 1 (Wind) = White, Seal 2 (Night) = Blue,
 * Seal 3 (Seed) = Yellow, Seal 4 (Serpent) = Red, ...
 */

static const char *COLOR_NAMES[4] = {
    "Red", "White", "Blue", "Yellow"
};

static const char *EARTH_FAMILY_NAMES[5] = {
    "Polar", "Cardinal", "Core", "Signal", "Gateway"
};

/*
 * Tone keywords (Arguelles/Dreamspell):
 * 1=Purpose, 2=Challenge, 3=Service, 4=Form, 5=Radiance,
 * 6=Equality, 7=Attunement, 8=Integrity, 9=Intention,
 * 10=Manifestation, 11=Liberation, 12=Cooperation, 13=Endurance
 */
static const char *TONE_KEYWORDS[13] = {
    "Purpose", "Challenge", "Service", "Form", "Radiance",
    "Equality", "Attunement", "Integrity", "Intention",
    "Manifestation", "Liberation", "Cooperation", "Endurance"
};

/*
 * Tone action verbs:
 * 1=Unify, 2=Polarize, 3=Activate, 4=Define, 5=Empower,
 * 6=Organize, 7=Channel, 8=Harmonize, 9=Pulse,
 * 10=Perfect, 11=Dissolve, 12=Dedicate, 13=Transcend
 */
static const char *TONE_ACTIONS[13] = {
    "Unify", "Polarize", "Activate", "Define", "Empower",
    "Organize", "Channel", "Harmonize", "Pulse",
    "Perfect", "Dissolve", "Dedicate", "Transcend"
};

/*
 * Wavespell initiating seals (Dreamspell order).
 * 20 wavespells of 13 kins each = 260 kins.
 * WS1 starts at Kin 1 (seal = (1-1)%20 = 0 = Dragon).
 * WS2 starts at Kin 14, WS3 at Kin 27, etc.
 * Kin N starts at seal (N-1)%20:
 *   WS1: Kin 1, seal 0 (Dragon)
 *   WS2: Kin 14, seal 13 (Wizard) — wait, (14-1)%20 = 13 = Wizard
 */

/*
 * Oracle relationships (Dreamspell):
 * - Analog: (seal + 19) % 20  — complement to sum 19
 * - Antipode: (seal + 10) % 20 — opposite
 * - Occult: (19 - seal) % 20  — hidden power (same as analog for Dragon/Sun)
 * - Guide: same color family seal, position depends on tone:
 *   Tone position within 5 same-color seals determines guide.
 *   Same-color seals for seal S: S, S+4, S+8, S+12, S+16 (all mod 20)
 *   Guide index within this group depends on tone:
 *     Tone 1,6,11 -> guide = self (position 0)
 *     Tone 2,7,12 -> guide = position 1 (seal+4)
 *     Tone 3,8,13 -> guide = position 2 (seal+8)
 *     Tone 4,9    -> guide = position 3 (seal+12)
 *     Tone 5,10   -> guide = position 4 (seal+16)
 */

static int guide_offset_for_tone(int tone)
{
    /* Map tone (1-13) to guide position (0-4) */
    switch (tone) {
    case 1: case 6: case 11: return 0;
    case 2: case 7: case 12: return 4;
    case 3: case 8: case 13: return 8;
    case 4: case 9:          return 12;
    case 5: case 10:         return 16;
    default:                 return 0;
    }
}

tzolkin_color_t tzolkin_seal_color(int seal)
{
    if (seal < 0 || seal > 19) return TZOLKIN_RED;
    return (tzolkin_color_t)(seal % 4);
}

const char *tzolkin_color_name(tzolkin_color_t color)
{
    if (color < 0 || color > 3) return "?";
    return COLOR_NAMES[color];
}

earth_family_t tzolkin_earth_family(int seal)
{
    if (seal < 0 || seal > 19) return EARTH_POLAR;
    /* Polar: 0,5,10,15 | Cardinal: 1,6,11,16 | Core: 2,7,12,17 |
     * Signal: 3,8,13,18 | Gateway: 4,9,14,19 */
    return (earth_family_t)(seal % 5);
}

const char *tzolkin_earth_family_name(earth_family_t family)
{
    if (family < 0 || family > 4) return "?";
    return EARTH_FAMILY_NAMES[family];
}

const char *tzolkin_tone_keyword(int tone)
{
    if (tone < 1 || tone > 13) return "?";
    return TONE_KEYWORDS[tone - 1];
}

const char *tzolkin_tone_action(int tone)
{
    if (tone < 1 || tone > 13) return "?";
    return TONE_ACTIONS[tone - 1];
}

tzolkin_oracle_t tzolkin_oracle(int seal, int tone)
{
    tzolkin_oracle_t o;

    if (seal < 0) seal = 0;
    if (seal > 19) seal = 19;
    if (tone < 1) tone = 1;
    if (tone > 13) tone = 13;

    o.destiny  = seal;
    o.analog   = (seal + 19) % 20;
    o.antipode = (seal + 10) % 20;
    o.occult   = (19 - seal + 20) % 20;
    o.guide    = (seal + guide_offset_for_tone(tone)) % 20;

    return o;
}

int tzolkin_wavespell(int kin)
{
    if (kin < 1) kin = 1;
    if (kin > 260) kin = 260;
    return ((kin - 1) / 13) + 1;
}

int tzolkin_wavespell_seal(int wavespell)
{
    if (wavespell < 1) wavespell = 1;
    if (wavespell > 20) wavespell = 20;
    /* Kin that starts this wavespell */
    int start_kin = (wavespell - 1) * 13 + 1;
    return (start_kin - 1) % 20;
}

/* --- Castle --- */

static const char *CASTLE_SHORT_NAMES[5] = {
    "Red Eastern", "White Northern", "Blue Western",
    "Yellow Southern", "Green Central"
};

static const char *CASTLE_FULL_NAMES[5] = {
    "Red Eastern Castle of Turning",
    "White Northern Castle of Crossing",
    "Blue Western Castle of Burning",
    "Yellow Southern Castle of Giving",
    "Green Central Castle of Enchantment"
};

static const char *CASTLE_COLOR_NAMES[5] = {
    "Red", "White", "Blue", "Yellow", "Green"
};

/* Seal keywords (Dreamspell "power" or action for each solar seal) */
static const char *SEAL_KEYWORDS[20] = {
    "Birth",            /* 0  Dragon */
    "Spirit",           /* 1  Wind */
    "Abundance",        /* 2  Night */
    "Flowering",        /* 3  Seed */
    "Life Force",       /* 4  Serpent */
    "Death",            /* 5  Worldbridger */
    "Accomplishment",   /* 6  Hand */
    "Elegance",         /* 7  Star */
    "Universal Water",  /* 8  Moon */
    "Heart",            /* 9  Dog */
    "Magic",            /* 10 Monkey */
    "Free Will",        /* 11 Human */
    "Space",            /* 12 Skywalker */
    "Timelessness",     /* 13 Wizard */
    "Vision",           /* 14 Eagle */
    "Intelligence",     /* 15 Warrior */
    "Navigation",       /* 16 Earth */
    "Endlessness",      /* 17 Mirror */
    "Self-Generation",  /* 18 Storm */
    "Universal Fire"    /* 19 Sun */
};

tzolkin_castle_t tzolkin_castle(int kin)
{
    if (kin < 1) kin = 1;
    if (kin > 260) kin = 260;
    return (tzolkin_castle_t)(((kin - 1) / 52) + 1);
}

const char *tzolkin_castle_short_name(tzolkin_castle_t castle)
{
    if (castle < 1 || castle > 5) return "?";
    return CASTLE_SHORT_NAMES[castle - 1];
}

const char *tzolkin_castle_full_name(tzolkin_castle_t castle)
{
    if (castle < 1 || castle > 5) return "?";
    return CASTLE_FULL_NAMES[castle - 1];
}

const char *tzolkin_castle_name(tzolkin_castle_t castle)
{
    return tzolkin_castle_short_name(castle);
}

const char *tzolkin_castle_color_name(tzolkin_castle_t castle)
{
    if (castle < 1 || castle > 5) return "?";
    return CASTLE_COLOR_NAMES[castle - 1];
}

int tzolkin_wavespell_position(int kin)
{
    if (kin < 1) kin = 1;
    if (kin > 260) kin = 260;
    return ((kin - 1) % 13) + 1;
}

const char *tzolkin_seal_keyword(int seal)
{
    if (seal < 0 || seal > 19) return "?";
    return SEAL_KEYWORDS[seal];
}

/* --- Display formatter --- */

/* Extract base seal name (skip color prefix).
 * "Blue Hand" -> "Hand", "White Worldbridger" -> "Worldbridger" */
static const char *base_seal_name(const char *full_name)
{
    const char *sp = strchr(full_name, ' ');
    if (sp) return sp + 1;
    return full_name;
}

static void safe_copy(char *dst, size_t n, const char *src)
{
    strncpy(dst, src, n - 1);
    dst[n - 1] = '\0';
}

tzolkin_display_t tzolkin_display_from_kin(int kin)
{
    tzolkin_display_t d;
    memset(&d, 0, sizeof(d));

    if (kin < 1) kin = 1;
    if (kin > 260) kin = 260;

    d.kin  = kin;
    d.seal = (kin - 1) % 20;
    d.tone = ((kin - 1) % 13) + 1;
    d.wavespell = tzolkin_wavespell(kin);
    d.wavespell_position = tzolkin_wavespell_position(kin);
    d.castle = (int)tzolkin_castle(kin);
    d.color  = tzolkin_seal_color(d.seal);

    /* String lookups */
    const char *seal_full  = tzolkin_seal_name(d.seal);
    const char *seal_base  = base_seal_name(seal_full);
    const char *tone_nm    = tzolkin_tone_name(d.tone);
    const char *col_nm     = tzolkin_color_name(d.color);
    const char *castle_fnm = tzolkin_castle_full_name((tzolkin_castle_t)d.castle);
    const char *castle_snm = tzolkin_castle_short_name((tzolkin_castle_t)d.castle);
    const char *kw         = tzolkin_tone_keyword(d.tone);
    const char *act        = tzolkin_tone_action(d.tone);
    const char *seal_kw    = tzolkin_seal_keyword(d.seal);

    /* Oracle */
    tzolkin_oracle_t orc = tzolkin_oracle(d.seal, d.tone);
    safe_copy(d.oracle_guide,    sizeof(d.oracle_guide),    tzolkin_seal_name(orc.guide));
    safe_copy(d.oracle_analog,   sizeof(d.oracle_analog),   tzolkin_seal_name(orc.analog));
    safe_copy(d.oracle_antipode, sizeof(d.oracle_antipode), tzolkin_seal_name(orc.antipode));
    safe_copy(d.oracle_occult,   sizeof(d.oracle_occult),   tzolkin_seal_name(orc.occult));

    /* Copy short strings */
    safe_copy(d.color_name,   sizeof(d.color_name),   col_nm);
    safe_copy(d.castle_name,  sizeof(d.castle_name),  castle_fnm);
    safe_copy(d.tone_keyword, sizeof(d.tone_keyword), kw);
    safe_copy(d.tone_action,  sizeof(d.tone_action),  act);
    safe_copy(d.seal_keyword, sizeof(d.seal_keyword), seal_kw);

    /* Title: "Kin N · Color Tone Seal" */
    snprintf(d.title, sizeof(d.title),
             "Kin %d \xc2\xb7 %s %s %s",
             d.kin, col_nm, tone_nm, seal_base);

    /* Summary: "Color Tone Seal · WS N · Castle N CastleShortName" */
    snprintf(d.summary, sizeof(d.summary),
             "%s %s %s \xc2\xb7 WS %d \xc2\xb7 Castle %d %s",
             col_nm, tone_nm, seal_base,
             d.wavespell, d.castle, castle_snm);

    return d;
}

tzolkin_display_t tzolkin_display_from_jd(double jd)
{
    tzolkin_day_t day = tzolkin_from_jd(jd);
    return tzolkin_display_from_kin(day.kin);
}
