#ifndef TIME_WHEEL_LAYOUT_H
#define TIME_WHEEL_LAYOUT_H

typedef struct {
    float x;
    float y;
} wheel_point_t;

typedef struct {
    float start_deg;
    float end_deg;
} wheel_segment_t;

/* Convert degrees (0=Aries cusp, counterclockwise) to (x,y) on a circle. */
wheel_point_t wheel_position(float degrees, float radius);

/* Position for a zodiac sign center (sign 0=Aries at 15deg, etc.) */
wheel_point_t wheel_sign_center(int sign, float radius);

/* Position for a specific ecliptic longitude on the wheel. */
wheel_point_t wheel_longitude(float ecl_lon_deg, float radius);

/* Segment arc: start/end angles for a zodiac sign's 30-degree segment. */
wheel_segment_t wheel_sign_segment(int sign);

/* Check if a longitude falls within a segment. */
int wheel_in_segment(float lon_deg, wheel_segment_t seg);

/* Angular distance between two points (always positive, 0-180). */
float wheel_angular_distance(float deg_a, float deg_b);

/* Generate N evenly-spaced points on a circle. Caller provides array. */
void wheel_ring_points(float radius, int n, wheel_point_t *out);

/* Billboard offset: given camera azimuth (radians), return angle to face camera. */
float wheel_billboard_angle(float camera_azimuth_rad);

#endif
