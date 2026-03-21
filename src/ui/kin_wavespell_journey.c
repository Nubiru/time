#include "kin_wavespell_journey.h"
#include "../systems/tzolkin/dreamspell.h"
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const char *PHASE_NAMES[4] = {
    "Initiate", "Extend", "Convert", "Complete"
};

kin_wavespell_journey_t kin_journey_compute(int today_tone)
{
    kin_wavespell_journey_t j;
    memset(&j, 0, sizeof(j));

    if (today_tone < 1 || today_tone > 13)
        return j;

    j.today_tone = today_tone;

    /* Phase: 1-4 = Initiate(0), 5-8 = Extend(1), 9-12 = Convert(2), 13 = Complete(3) */
    if (today_tone <= 4)
        j.phase = 0;
    else if (today_tone <= 8)
        j.phase = 1;
    else if (today_tone <= 12)
        j.phase = 2;
    else
        j.phase = 3;

    for (int i = 0; i < KIN_JOURNEY_STEPS; i++) {
        int tone = i + 1;
        dreamspell_tone_t dt = dreamspell_tone(tone);

        j.steps[i].tone = tone;
        j.steps[i].name = dt.name;
        j.steps[i].action = dt.action;
        j.steps[i].power = dt.power;

        /* Arc positions: left to right, with sine arc peaking at center */
        float t = (float)i / 12.0f;
        j.steps[i].x = 0.05f + t * 0.90f;
        j.steps[i].y = 0.5f - 0.3f * sinf(t * (float)M_PI);

        j.steps[i].highlighted = (tone == today_tone) ? 1 : 0;
        j.steps[i].is_gate = (tone == 1 || tone == 5 ||
                              tone == 9 || tone == 13) ? 1 : 0;
    }

    return j;
}

const char *kin_journey_phase_name(int phase)
{
    if (phase < 0 || phase > 3)
        return "?";
    return PHASE_NAMES[phase];
}
