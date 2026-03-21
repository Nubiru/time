#include "kin_pulsar_layout.h"
#include <string.h>

/*
 * Kin Pulsar Layout — 4 pulsar groups connecting wavespell tones.
 *
 * Pulsar mapping: tone position mod 4 determines pulsar membership.
 *   (tone - 1) % 4 == 0 → Pulsar 0 "Time"          (1st dimension)
 *   (tone - 1) % 4 == 1 → Pulsar 1 "Sense"         (2nd dimension)
 *   (tone - 1) % 4 == 2 → Pulsar 2 "Mind"          (3rd dimension)
 *   (tone - 1) % 4 == 3 → Pulsar 3 "Self-Existing" (4th dimension)
 *
 * Pure: no GL, no malloc, no globals, no side effects.
 */

/* Static pulsar data */

static const char *PULSAR_NAMES[KIN_PULSAR_COUNT] = {
    "Time", "Sense", "Mind", "Self-Existing"
};

static const char *PULSAR_DIMENSIONS[KIN_PULSAR_COUNT] = {
    "1st", "2nd", "3rd", "4th"
};

static const int PULSAR_TONES[KIN_PULSAR_COUNT][KIN_PULSAR_MAX_TONES] = {
    { 1,  5,  9, 13},   /* Time: 4 tones */
    { 2,  6, 10,  0},   /* Sense: 3 tones */
    { 3,  7, 11,  0},   /* Mind: 3 tones */
    { 4,  8, 12,  0}    /* Self-Existing: 3 tones */
};

static const int PULSAR_TONE_COUNTS[KIN_PULSAR_COUNT] = { 4, 3, 3, 3 };

static const float PULSAR_COLORS[KIN_PULSAR_COUNT][4] = {
    { 1.0f, 0.85f, 0.55f, 1.0f },   /* Time: gold */
    { 0.85f, 0.20f, 0.20f, 1.0f },  /* Sense: red */
    { 0.20f, 0.40f, 0.80f, 1.0f },  /* Mind: blue */
    { 0.30f, 0.70f, 0.40f, 1.0f }   /* Self-Existing: green */
};

int kin_pulsar_for_tone(int tone)
{
    if (tone < 1 || tone > 13)
        return -1;
    return (tone - 1) % 4;
}

kin_pulsar_layout_t kin_pulsar_compute(int today_tone)
{
    kin_pulsar_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    if (today_tone < 1 || today_tone > 13)
        return layout;

    layout.today_tone = today_tone;
    layout.today_pulsar = kin_pulsar_for_tone(today_tone);

    for (int i = 0; i < KIN_PULSAR_COUNT; i++) {
        layout.pulsars[i].name = PULSAR_NAMES[i];
        layout.pulsars[i].dimension = PULSAR_DIMENSIONS[i];
        layout.pulsars[i].tone_count = PULSAR_TONE_COUNTS[i];

        for (int t = 0; t < KIN_PULSAR_MAX_TONES; t++) {
            layout.pulsars[i].tones[t] = PULSAR_TONES[i][t];
        }

        for (int c = 0; c < 4; c++) {
            layout.pulsars[i].color[c] = PULSAR_COLORS[i][c];
        }

        /* Mark if today's tone is in this pulsar */
        layout.pulsars[i].contains_today = (i == layout.today_pulsar) ? 1 : 0;
    }

    return layout;
}
