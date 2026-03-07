#include "tzolkin_board.h"

/*
 * Galactic Activation Portal detection.
 *
 * The 52 GAP days form the "Loom of the Maya" diamond pattern on the
 * 13x20 harmonic grid (13 columns of 20 kins each).
 *
 * Pattern by column distance d from edge (d = min(col, 12-col)):
 *   d=0,1: outer pair at rows {d, 19-d}              (2 portals)
 *   d=2,3,4: outer+inner pairs {d, 19-d, 11-d, 8+d}  (4 portals)
 *   d=5: full band rows 5-14                          (10 portals)
 *   d=6 (center column): none                         (0 portals)
 *
 * Total: 2*2 + 2*2 + 3*2*4 + 2*10 = 52
 *
 * Source: Arguelles, Dreamspell (1990).
 */
bool tzolkin_board_is_portal(int kin_number)
{
    if (kin_number < 1 || kin_number > 260)
        return false;

    int col = (kin_number - 1) / 20;   /* 0-12 */
    int row = (kin_number - 1) % 20;   /* 0-19 */
    int d = col <= 6 ? col : 12 - col; /* distance from edge, 0-6 */

    if (d == 6)
        return false;

    if (d <= 1)
        return row == d || row == 19 - d;

    if (d <= 4)
        return row == d || row == 19 - d ||
               row == 11 - d || row == 8 + d;

    /* d == 5: full band rows 5-14 */
    return row >= 5 && row <= 14;
}

int tzolkin_board_portal_count(void)
{
    return 52;
}

tzolkin_board_color_t tzolkin_board_seal_color(int seal)
{
    if (seal < 0 || seal > 19)
        return TZOLKIN_BOARD_COLOR_RED;
    return (tzolkin_board_color_t)(seal % 4);
}

tzolkin_board_castle_t tzolkin_board_castle(int kin_number)
{
    if (kin_number < 1 || kin_number > 260)
        return TZOLKIN_BOARD_CASTLE_RED;
    return (tzolkin_board_castle_t)((kin_number - 1) / 52);
}

int tzolkin_board_wavespell(int kin_number)
{
    if (kin_number < 1 || kin_number > 260)
        return 0;
    return (kin_number - 1) / 13 + 1;
}

int tzolkin_board_harmonic(int kin_number)
{
    if (kin_number < 1 || kin_number > 260)
        return 0;
    return (kin_number - 1) / 4 + 1;
}

/*
 * Tone-seal to kin conversion using Chinese Remainder Theorem.
 *
 * kin-1 ≡ (tone-1) mod 13  AND  kin-1 ≡ seal mod 20
 * Since gcd(13,20) = 1:
 *   inv(20, 13) = 2   (20*2 = 40 ≡ 1 mod 13)
 *   inv(13, 20) = 17  (13*17 = 221 ≡ 1 mod 20)
 *   k = ((tone-1)*40 + seal*221) % 260
 *   kin = k + 1
 */
int tzolkin_board_kin_number(int tone, int seal)
{
    if (tone < 1 || tone > 13 || seal < 0 || seal > 19)
        return 0;
    int k = ((tone - 1) * 40 + seal * 221) % 260;
    return k + 1;
}

tzolkin_kin_t tzolkin_board_kin(int kin_number)
{
    tzolkin_kin_t kin = {0, 0, 0, TZOLKIN_BOARD_COLOR_RED, 0, 0,
                         TZOLKIN_BOARD_CASTLE_RED, false, 0};

    if (kin_number < 1 || kin_number > 260)
        return kin;

    kin.kin = kin_number;
    kin.tone = ((kin_number - 1) % 13) + 1;
    kin.seal = (kin_number - 1) % 20;
    kin.color = tzolkin_board_seal_color(kin.seal);
    kin.wavespell = tzolkin_board_wavespell(kin_number);
    kin.wavespell_position = (kin_number - 1) % 13 + 1;
    kin.castle = tzolkin_board_castle(kin_number);
    kin.is_portal = tzolkin_board_is_portal(kin_number);
    kin.harmonic = tzolkin_board_harmonic(kin_number);

    return kin;
}

tzolkin_kin_t tzolkin_board_by_tone_seal(int tone, int seal)
{
    int kn = tzolkin_board_kin_number(tone, seal);
    return tzolkin_board_kin(kn);
}

const char *tzolkin_board_color_name(tzolkin_board_color_t color)
{
    switch (color) {
    case TZOLKIN_BOARD_COLOR_RED:    return "Red";
    case TZOLKIN_BOARD_COLOR_WHITE:  return "White";
    case TZOLKIN_BOARD_COLOR_BLUE:   return "Blue";
    case TZOLKIN_BOARD_COLOR_YELLOW: return "Yellow";
    default:                         return "?";
    }
}

const char *tzolkin_board_castle_name(tzolkin_board_castle_t castle)
{
    switch (castle) {
    case TZOLKIN_BOARD_CASTLE_RED:    return "Red Castle of Turning";
    case TZOLKIN_BOARD_CASTLE_WHITE:  return "White Castle of Crossing";
    case TZOLKIN_BOARD_CASTLE_BLUE:   return "Blue Castle of Burning";
    case TZOLKIN_BOARD_CASTLE_YELLOW: return "Yellow Castle of Giving";
    case TZOLKIN_BOARD_CASTLE_GREEN:  return "Green Castle of Enchantment";
    default:                          return "?";
    }
}
