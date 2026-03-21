#ifndef TIME_KIN_WHEEL_LAYOUT_H
#define TIME_KIN_WHEEL_LAYOUT_H

/* Concentric seal/tone wheel layout.
 *
 * 20 seals in an outer ring, 13 tones in an inner ring.
 * Today's seal and tone are highlighted.
 *
 * Angles: 0 = top, clockwise.
 * Positions: normalized unit square, center at (0.5, 0.5).
 * Outer ring radius: 0.4, inner ring radius: 0.25.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_WHEEL_SEALS 20
#define KIN_WHEEL_TONES 13

/* A positioned element on the wheel. */
typedef struct {
    float angle;      /* degrees, 0=top, CW */
    float x, y;       /* normalized XY position (center = 0.5, 0.5) */
    int index;        /* seal index (0-19) or tone number (1-13) */
    int color;        /* directional color for seals (0-3), -1 for tones */
    int highlighted;  /* 1 = today's */
} kin_wheel_node_t;

typedef struct {
    kin_wheel_node_t seals[KIN_WHEEL_SEALS];   /* outer ring */
    kin_wheel_node_t tones[KIN_WHEEL_TONES];   /* inner ring */
    int today_seal;                             /* 0-19 */
    int today_tone;                             /* 1-13 */
    int today_kin;                              /* 1-260 */
} kin_wheel_layout_t;

/* Compute the wheel layout for a kin (1-260).
 * Outer ring: 20 nodes at radius 0.4 from center (0.5, 0.5).
 * Inner ring: 13 nodes at radius 0.25 from center.
 * Angles: seal_angle = seal * 18 degrees, tone_angle = (tone-1) * (360/13).
 * XY: x = 0.5 + radius * sin(angle), y = 0.5 - radius * cos(angle).
 * Today's seal and tone highlighted.
 * Returns zeroed struct for invalid kin. */
kin_wheel_layout_t kin_wheel_compute(int kin);

/* Compute wheel layout from Julian Day. Convenience wrapper. */
kin_wheel_layout_t kin_wheel_from_jd(double jd);

#endif /* TIME_KIN_WHEEL_LAYOUT_H */
