#include "clear_signs.h"
#include <string.h>

/* Clear Signs data — 13 Kins from Pacal Votan's sarcophagus lid.
 *
 * Sorted by kin number. Each entry records the seal, tone, group
 * assignment, and sarcophagus border position.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */
static const clear_sign_t SIGNS[CLEAR_SIGN_COUNT] = {
    {   /* #1 — Kin 20: Yellow Resonant Sun */
        .kin_number = 20,
        .seal_code  = 0,
        .seal_name  = "Sun",
        .tone       = 7,
        .group      = 2,
        .border     = "East"
    },
    {   /* #2 — Kin 26: White Cosmic Worldbridger */
        .kin_number = 26,
        .seal_code  = 6,
        .seal_name  = "Worldbridger",
        .tone       = 13,
        .group      = 6,
        .border     = ""
    },
    {   /* #3 — Kin 30: White Self-Existing Dog */
        .kin_number = 30,
        .seal_code  = 10,
        .seal_name  = "Dog",
        .tone       = 4,
        .group      = 5,
        .border     = "West"
    },
    {   /* #4 — Kin 40: Yellow Magnetic Sun */
        .kin_number = 40,
        .seal_code  = 0,
        .seal_name  = "Sun",
        .tone       = 1,
        .group      = 6,
        .border     = ""
    },
    {   /* #5 — Kin 57: Red Overtone Earth */
        .kin_number = 57,
        .seal_code  = 17,
        .seal_name  = "Earth",
        .tone       = 5,
        .group      = 2,
        .border     = "East"
    },
    {   /* #6 — Kin 58: White Rhythmic Mirror */
        .kin_number = 58,
        .seal_code  = 18,
        .seal_name  = "Mirror",
        .tone       = 6,
        .group      = 1,
        .border     = "South"
    },
    {   /* #7 — Kin 60: Yellow Galactic Sun (Pacal Votan) */
        .kin_number = 60,
        .seal_code  = 0,
        .seal_name  = "Sun",
        .tone       = 8,
        .group      = 1,
        .border     = "South"
    },
    {   /* #8 — Kin 87: Blue Solar Hand */
        .kin_number = 87,
        .seal_code  = 7,
        .seal_name  = "Hand",
        .tone       = 9,
        .group      = 2,
        .border     = "East"
    },
    {   /* #9 — Kin 106: White Lunar Worldbridger */
        .kin_number = 106,
        .seal_code  = 6,
        .seal_name  = "Worldbridger",
        .tone       = 2,
        .group      = 4,
        .border     = "North"
    },
    {   /* #10 — Kin 132: Yellow Lunar Human */
        .kin_number = 132,
        .seal_code  = 12,
        .seal_name  = "Human",
        .tone       = 2,
        .group      = 3,
        .border     = ""
    },
    {   /* #11 — Kin 176: Yellow Resonant Warrior */
        .kin_number = 176,
        .seal_code  = 16,
        .seal_name  = "Warrior",
        .tone       = 7,
        .group      = 2,
        .border     = "East"
    },
    {   /* #12 — Kin 211: Blue Electric Monkey */
        .kin_number = 211,
        .seal_code  = 11,
        .seal_name  = "Monkey",
        .tone       = 3,
        .group      = 5,
        .border     = "West"
    },
    {   /* #13 — Kin 245: Red Spectral Serpent */
        .kin_number = 245,
        .seal_code  = 5,
        .seal_name  = "Serpent",
        .tone       = 11,
        .group      = 3,
        .border     = ""
    }
};

/* 7 pairs: each row is {smaller_kin, larger_kin}.
 * Kin 60 is unpaired (Pacal Votan's own signature). */
static const int PAIRS[6][2] = {
    { 20, 87},
    { 26, 106},
    { 30, 132},
    { 40, 176},
    { 57, 211},
    { 58, 245}
};

#define PAIR_COUNT 6

int clear_sign_count(void)
{
    return CLEAR_SIGN_COUNT;
}

clear_sign_t clear_sign_get(int index)
{
    if (index < 0 || index >= CLEAR_SIGN_COUNT) {
        clear_sign_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return SIGNS[index];
}

int clear_sign_is_clear_sign(int kin_number)
{
    for (int i = 0; i < CLEAR_SIGN_COUNT; i++) {
        if (SIGNS[i].kin_number == kin_number)
            return 1;
    }
    return 0;
}

int clear_sign_total_tone_sum(void)
{
    int sum = 0;
    for (int i = 0; i < CLEAR_SIGN_COUNT; i++) {
        sum += SIGNS[i].tone;
    }
    return sum;
}

int clear_sign_group_tone_sum(int group)
{
    if (group < 1 || group > 6)
        return 0;
    int sum = 0;
    for (int i = 0; i < CLEAR_SIGN_COUNT; i++) {
        if (SIGNS[i].group == group)
            sum += SIGNS[i].tone;
    }
    return sum;
}

int clear_sign_pair(int kin_number)
{
    for (int i = 0; i < PAIR_COUNT; i++) {
        if (PAIRS[i][0] == kin_number)
            return PAIRS[i][1];
        if (PAIRS[i][1] == kin_number)
            return PAIRS[i][0];
    }
    /* Kin 60 is a clear sign but unpaired; all other non-matches return 0. */
    return 0;
}

int clear_sign_pacal_votan_kin(void)
{
    return 60;
}
