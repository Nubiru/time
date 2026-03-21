/* zodiac_wheel_layout.c — 12-sign zodiac ring with Sun position.
 *
 * Places 12 signs on a circle at 30-degree intervals and marks the
 * current Sun position. All coordinates normalized 0-1.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "zodiac_wheel_layout.h"
#include "../systems/astrology/zodiac.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Wheel geometry (normalized 0-1 coordinates). */
#define CENTER_X     0.50f
#define CENTER_Y     0.50f
#define RADIUS       0.38f
#define LABEL_RATIO  0.75f   /* labels at 75% of radius from center */

static float deg_to_rad(float deg)
{
    return deg * (float)(M_PI / 180.0);
}

/* Convert angle in degrees to a point on the wheel.
 * Standard math convention: 0 degrees = right, counterclockwise positive.
 * Screen y-axis is inverted (down = positive). */
static void angle_to_xy(float angle_deg, float r, float *out_x, float *out_y)
{
    float rad = deg_to_rad(angle_deg);
    *out_x = CENTER_X + r * cosf(rad);
    *out_y = CENTER_Y - r * sinf(rad);
}

zodiac_wheel_layout_t zodiac_wheel_compute(double sun_longitude)
{
    zodiac_wheel_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    layout.center_x = CENTER_X;
    layout.center_y = CENTER_Y;
    layout.radius   = RADIUS;

    /* Place 12 signs at 30-degree intervals. */
    for (int i = 0; i < 12; i++) {
        layout.signs[i].sign       = i;
        layout.signs[i].name       = zodiac_sign_name(i);
        layout.signs[i].symbol     = zodiac_sign_symbol(i);
        layout.signs[i].arc_start_deg = (float)(i * 30);
        layout.signs[i].arc_end_deg   = (float)((i + 1) * 30);

        /* Label at midpoint of arc, at LABEL_RATIO * radius from center. */
        float mid_deg = (float)(i * 30) + 15.0f;
        angle_to_xy(mid_deg, RADIUS * LABEL_RATIO,
                    &layout.signs[i].label_x,
                    &layout.signs[i].label_y);
    }

    /* Sun position. */
    double norm_lon = fmod(sun_longitude, 360.0);
    if (norm_lon < 0.0) norm_lon += 360.0;

    layout.sun_sign       = zodiac_sign(sun_longitude);
    layout.sun_degree     = (float)zodiac_degree(sun_longitude);
    layout.sun_angle_deg  = (float)norm_lon;
    angle_to_xy(layout.sun_angle_deg, RADIUS,
                &layout.sun_x, &layout.sun_y);

    return layout;
}

zw_sign_t zodiac_wheel_sign_at(const zodiac_wheel_layout_t *layout, int sign)
{
    zw_sign_t empty;
    memset(&empty, 0, sizeof(empty));
    empty.sign = -1;
    empty.name = "?";
    empty.symbol = "?";

    if (!layout || sign < 0 || sign > 11) {
        return empty;
    }

    return layout->signs[sign];
}
