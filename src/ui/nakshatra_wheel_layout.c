/* nakshatra_wheel_layout.c — 27 Nakshatra wheel layout
 *
 * Places 27 Nakshatras on a circle, each spanning 360/27 = 13.333 degrees.
 * Ashwini at top (0 degrees), clockwise. Highlights the Moon's current
 * Nakshatra using tropical-to-sidereal conversion.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "nakshatra_wheel_layout.h"
#include "../systems/hindu/nakshatra.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Wheel geometry (normalized 0-1 coordinates). */
#define CENTER_X  0.50f
#define CENTER_Y  0.50f
#define RADIUS    0.40f

/* Each nakshatra spans 360/27 degrees. */
#define SPAN_DEG  (360.0 / 27.0)

static float deg_to_rad(float deg)
{
    return deg * (float)(M_PI / 180.0);
}

nakshatra_wheel_layout_t nakshatra_wheel_compute(double moon_tropical_lon,
                                                  double jd)
{
    nakshatra_wheel_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    layout.center_x = CENTER_X;
    layout.center_y = CENTER_Y;
    layout.radius   = RADIUS;

    /* Determine Moon's nakshatra if valid input */
    int moon_idx = -1;
    if (moon_tropical_lon >= 0.0 && jd > 0.0) {
        double ayanamsa = nakshatra_lahiri_ayanamsa(jd);
        double sidereal = nakshatra_tropical_to_sidereal(moon_tropical_lon,
                                                          ayanamsa);
        nakshatra_t moon_nak = nakshatra_from_longitude(sidereal);
        moon_idx = moon_nak.id;
    }
    layout.moon_nakshatra = moon_idx;

    /* Place 27 nakshatras on the wheel.
     * North=top convention: sin for x, -cos for y. */
    for (int i = 0; i < NW_NAKSHATRA_COUNT; i++) {
        nakshatra_t nak = nakshatra_get(i);

        layout.nakshatras[i].id      = nak.id;
        layout.nakshatras[i].name    = nak.name;
        layout.nakshatras[i].deity   = nak.deity;
        layout.nakshatras[i].quality = nak.quality;
        layout.nakshatras[i].symbol  = nak.symbol;

        /* Position at center of arc */
        float angle = (float)(i * SPAN_DEG + SPAN_DEG * 0.5);
        layout.nakshatras[i].angle_deg = (float)(i * SPAN_DEG);

        float rad = deg_to_rad(angle);
        layout.nakshatras[i].x = CENTER_X + RADIUS * sinf(rad);
        layout.nakshatras[i].y = CENTER_Y - RADIUS * cosf(rad);

        /* Arc boundaries */
        layout.nakshatras[i].arc_start_deg = (float)(i * SPAN_DEG);
        layout.nakshatras[i].arc_end_deg   = (float)((i + 1) * SPAN_DEG);

        /* Highlight if Moon is in this nakshatra */
        layout.nakshatras[i].highlighted = (i == moon_idx) ? 1 : 0;
    }

    return layout;
}

nw_nakshatra_t nakshatra_wheel_at(const nakshatra_wheel_layout_t *layout,
                                   int index)
{
    nw_nakshatra_t empty;
    memset(&empty, 0, sizeof(empty));
    empty.id      = -1;
    empty.name    = "?";
    empty.deity   = "?";
    empty.quality = "?";
    empty.symbol  = "?";

    if (!layout || index < 0 || index >= NW_NAKSHATRA_COUNT) {
        return empty;
    }

    return layout->nakshatras[index];
}
