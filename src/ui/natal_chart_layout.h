#ifndef TIME_NATAL_CHART_LAYOUT_H
#define TIME_NATAL_CHART_LAYOUT_H

/* Natal chart wheel layout — 2D astrology chart with houses, planets, aspects.
 *
 * Computes a traditional natal chart wheel in normalized 0-1 coordinates:
 *   - 12 house cusp lines from inner to outer ring
 *   - 12 sign labels on the outer ring
 *   - Up to 8 planets placed on the planet ring
 *   - Aspect lines between planets inside the inner ring
 *   - Ascendant at the 9 o'clock (left) position
 *
 * Chart angle convention:
 *   angle = (longitude - ascendant + 180) mod 360
 *   Ascendant at left, degrees increase counter-clockwise.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define NATAL_CHART_PLANET_COUNT 8
#define NATAL_CHART_MAX_ASPECTS  28

typedef struct {
    float x, y;           /* position (normalized 0-1) */
    float angle_deg;      /* ecliptic longitude mapped to chart angle */
} chart_point_t;

typedef struct {
    int planet_index;     /* 0-7 */
    int sign;             /* 0-11 */
    float degree;         /* degree within sign (0-30) */
    chart_point_t pos;    /* position on the wheel */
    int dignity;          /* dignity score */
} chart_planet_t;

typedef struct {
    chart_point_t start;
    chart_point_t end;
    int type;             /* aspect_type_t (0-4) */
    float orb;            /* aspect orb in degrees */
} chart_aspect_line_t;

typedef struct {
    /* Wheel geometry */
    float center_x, center_y;    /* always 0.5, 0.5 */
    float outer_radius;          /* 0.45 */
    float inner_radius;          /* 0.30 */
    float planet_radius;         /* 0.37 — between inner and outer */

    /* House cusps — 12 lines from inner to outer */
    chart_point_t cusp_inner[12];   /* inner end of cusp line */
    chart_point_t cusp_outer[12];   /* outer end of cusp line */
    int cusp_signs[12];             /* sign for each house */

    /* Sign label positions — on the outer ring */
    chart_point_t sign_labels[12];  /* label center for each sign */

    /* Planets placed on the wheel */
    chart_planet_t planets[NATAL_CHART_PLANET_COUNT];
    int planet_count;

    /* Aspect lines between planets */
    chart_aspect_line_t aspects[NATAL_CHART_MAX_ASPECTS];
    int aspect_count;

    /* Ascendant position */
    float ascendant_deg;
    chart_point_t asc_point;
    int asc_sign;                   /* sign of the ascendant */
} natal_chart_layout_t;

/* Compute the natal chart wheel layout.
 * ascendant_lon: ecliptic longitude of ascendant (0-360 degrees).
 * planet_lons: array of 8 ecliptic longitudes (Mercury..Neptune).
 * max_orb: aspect orb tolerance in degrees.
 * Returns zeroed layout for NULL planet_lons. */
natal_chart_layout_t natal_chart_compute(
    double ascendant_lon,
    const double planet_lons[8],
    double max_orb
);

/* Get sign name for a planet in the layout.
 * Returns "?" for invalid planet_index. */
const char *natal_chart_planet_sign_name(
    const natal_chart_layout_t *layout,
    int planet_index
);

/* Convert a chart angle + radius to a point in normalized coordinates.
 * angle_deg: chart angle in degrees (0-360).
 * radius: distance from center.
 * cx, cy: center coordinates. */
chart_point_t natal_chart_angle_to_point(
    float angle_deg,
    float radius,
    float cx,
    float cy
);

#endif /* TIME_NATAL_CHART_LAYOUT_H */
