#include "kin_wheel_layout.h"
#include "kin_cell.h"
#include "../systems/tzolkin/tzolkin.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SEAL_RADIUS  0.4f
#define TONE_RADIUS  0.25f
#define CENTER_X     0.5f
#define CENTER_Y     0.5f

kin_wheel_layout_t kin_wheel_compute(int kin)
{
    kin_wheel_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    if (kin < 1 || kin > 260)
        return layout;

    int today_seal = (kin - 1) % 20;
    int today_tone = (kin - 1) % 13 + 1;

    layout.today_seal = today_seal;
    layout.today_tone = today_tone;
    layout.today_kin  = kin;

    /* Outer ring: 20 seals */
    for (int i = 0; i < KIN_WHEEL_SEALS; i++) {
        float angle_deg = (float)i * 18.0f;
        float angle_rad = angle_deg * (float)(M_PI / 180.0);

        layout.seals[i].angle       = angle_deg;
        layout.seals[i].x           = CENTER_X + SEAL_RADIUS * sinf(angle_rad);
        layout.seals[i].y           = CENTER_Y - SEAL_RADIUS * cosf(angle_rad);
        layout.seals[i].index       = i;
        layout.seals[i].color       = kin_cell_color(i);
        layout.seals[i].highlighted = (i == today_seal) ? 1 : 0;
    }

    /* Inner ring: 13 tones */
    for (int i = 0; i < KIN_WHEEL_TONES; i++) {
        float angle_deg = (float)i * (360.0f / 13.0f);
        float angle_rad = angle_deg * (float)(M_PI / 180.0);

        layout.tones[i].angle       = angle_deg;
        layout.tones[i].x           = CENTER_X + TONE_RADIUS * sinf(angle_rad);
        layout.tones[i].y           = CENTER_Y - TONE_RADIUS * cosf(angle_rad);
        layout.tones[i].index       = i + 1;  /* tone number 1-13 */
        layout.tones[i].color       = -1;     /* tones have no directional color */
        layout.tones[i].highlighted = (i + 1 == today_tone) ? 1 : 0;
    }

    return layout;
}

kin_wheel_layout_t kin_wheel_from_jd(double jd)
{
    tzolkin_day_t day = tzolkin_from_jd(jd);
    return kin_wheel_compute(day.kin);
}
