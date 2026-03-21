#ifndef TIME_ZODIAC_WHEEL_LAYOUT_H
#define TIME_ZODIAC_WHEEL_LAYOUT_H

/* Zodiac wheel layout — 12-sign ring with Sun position.
 *
 * Computes a 2D card-friendly zodiac wheel: 12 signs arranged in a
 * circle with degree markers and the current Sun position highlighted.
 * Simpler than natal_chart_layout — no houses, no planets, just the
 * zodiac ring for card display.
 *
 * Positions are in normalized 0-1 coordinate space.
 *   Center: (0.50, 0.50), radius: 0.38
 *   Signs at 30-degree intervals starting from Aries at 0 degrees.
 *   Labels at midpoint of each arc, at radius*0.75 from center.
 *
 * Sun position: standard math convention (counterclockwise from right).
 *   x = center_x + radius * cos(angle_rad)
 *   y = center_y - radius * sin(angle_rad)
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

typedef struct {
    int sign;               /* 0-11 */
    const char *name;       /* "Aries", etc. */
    const char *symbol;     /* Unicode symbol */
    float label_x, label_y; /* label position on the wheel */
    float arc_start_deg;    /* start angle of this sign's arc */
    float arc_end_deg;      /* end angle */
} zw_sign_t;

typedef struct {
    zw_sign_t signs[12];
    float center_x, center_y;
    float radius;

    /* Current Sun position */
    int sun_sign;
    float sun_degree;        /* degree within sign (0-30) */
    float sun_x, sun_y;     /* position on wheel */
    float sun_angle_deg;    /* angle on wheel */
} zodiac_wheel_layout_t;

/* Compute zodiac wheel layout for a given sun ecliptic longitude (0-360).
 * Returns layout with center at (0.50, 0.50) and radius 0.38. */
zodiac_wheel_layout_t zodiac_wheel_compute(double sun_longitude);

/* Get the sign data at a given index (0-11).
 * Returns sign with sign=-1 for invalid index or NULL layout. */
zw_sign_t zodiac_wheel_sign_at(const zodiac_wheel_layout_t *layout, int sign);

#endif /* TIME_ZODIAC_WHEEL_LAYOUT_H */
