#include "lenses_of_time.h"
#include <string.h>

/* Lenses of Time data — 4 template frequencies of the Synchronotron.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 */
static const lens_of_time_t LENSES[LENS_OF_TIME_COUNT] = {
    {   /* Lens 1 — tones 1, 5, 9, 13 */
        .number        = 1,
        .value         = 108,
        .name          = "Cosmic Standard",
        .template_name = "Stellar Mind",
        .pulsar        = "Time Pulsar",
        .dimension     = 4
    },
    {   /* Lens 2 — tones 2, 6, 10 */
        .number        = 2,
        .value         = 144,
        .name          = "Cosmic Harmonic",
        .template_name = "Foundation of Tollan",
        .pulsar        = "Life Pulsar",
        .dimension     = 1
    },
    {   /* Lens 3 — tones 3, 7, 11 */
        .number        = 3,
        .value         = 216,
        .name          = "Cosmic Cube",
        .template_name = "Primary Cube",
        .pulsar        = "Sense Pulsar",
        .dimension     = 2
    },
    {   /* Lens 4 — tones 4, 8, 12 */
        .number        = 4,
        .value         = 288,
        .name          = "Ultimate Sphere",
        .template_name = "Polar Light Harmonic",
        .pulsar        = "Mind Pulsar",
        .dimension     = 3
    }
};

/* Tone-to-lens index mapping (0-based lens index for tones 1-13).
 * Tone 1 -> lens 0, tone 2 -> lens 1, ... */
static const int TONE_TO_LENS[13] = {
    0,  /* tone  1 -> Lens 1 (108) */
    1,  /* tone  2 -> Lens 2 (144) */
    2,  /* tone  3 -> Lens 3 (216) */
    3,  /* tone  4 -> Lens 4 (288) */
    0,  /* tone  5 -> Lens 1 (108) */
    1,  /* tone  6 -> Lens 2 (144) */
    2,  /* tone  7 -> Lens 3 (216) */
    3,  /* tone  8 -> Lens 4 (288) */
    0,  /* tone  9 -> Lens 1 (108) */
    1,  /* tone 10 -> Lens 2 (144) */
    2,  /* tone 11 -> Lens 3 (216) */
    3,  /* tone 12 -> Lens 4 (288) */
    0   /* tone 13 -> Lens 1 (108) */
};

lens_of_time_t lens_for_tone(int tone)
{
    if (tone < 1 || tone > 13) {
        lens_of_time_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return LENSES[TONE_TO_LENS[tone - 1]];
}

int lens_count(void)
{
    return LENS_OF_TIME_COUNT;
}

lens_of_time_t lens_by_number(int lens_num)
{
    if (lens_num < 1 || lens_num > LENS_OF_TIME_COUNT) {
        lens_of_time_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return LENSES[lens_num - 1];
}

int lens_tonal_value(int tone)
{
    if (tone < 1 || tone > 13)
        return 0;
    return tone * LENSES[TONE_TO_LENS[tone - 1]].value;
}

int lens_value_for_tone(int tone)
{
    if (tone < 1 || tone > 13)
        return 0;
    return LENSES[TONE_TO_LENS[tone - 1]].value;
}
