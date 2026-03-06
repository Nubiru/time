#include "tzolkin_card.h"

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
