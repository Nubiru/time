#include "natal_chart_layout.h"
#include "../systems/astrology/zodiac.h"
#include "../systems/astrology/houses.h"
#include "../systems/astrology/aspects.h"
#include "../systems/astrology/dignity.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Wheel geometry constants. */
#define CENTER_X       0.5f
#define CENTER_Y       0.5f
#define OUTER_RADIUS   0.45f
#define INNER_RADIUS   0.30f
#define PLANET_RADIUS  0.37f
#define LABEL_RADIUS   ((OUTER_RADIUS + INNER_RADIUS) / 2.0f)

/* Convert ecliptic longitude to chart angle.
 * Ascendant at 180 degrees (9 o'clock / left). */
static float lon_to_chart_angle(double longitude, double ascendant)
{
    double angle = fmod(longitude - ascendant + 180.0, 360.0);
    if (angle < 0.0) angle += 360.0;
    return (float)angle;
}

chart_point_t natal_chart_angle_to_point(float angle_deg, float radius,
                                         float cx, float cy)
{
    chart_point_t p;
    double rad = (double)angle_deg * M_PI / 180.0;
    p.x = cx + radius * (float)cos(rad);
    p.y = cy - radius * (float)sin(rad);  /* y flipped for screen space */
    p.angle_deg = angle_deg;
    return p;
}

natal_chart_layout_t natal_chart_compute(double ascendant_lon,
                                         const double planet_lons[8],
                                         double max_orb)
{
    natal_chart_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    layout.center_x = CENTER_X;
    layout.center_y = CENTER_Y;
    layout.outer_radius = OUTER_RADIUS;
    layout.inner_radius = INNER_RADIUS;
    layout.planet_radius = PLANET_RADIUS;

    /* Normalize ascendant. */
    double asc = fmod(ascendant_lon, 360.0);
    if (asc < 0.0) asc += 360.0;
    layout.ascendant_deg = (float)asc;
    layout.asc_sign = zodiac_sign(asc);

    /* Ascendant point on outer ring at 180 degrees (left). */
    layout.asc_point = natal_chart_angle_to_point(
        180.0f, OUTER_RADIUS, CENTER_X, CENTER_Y);

    if (!planet_lons) {
        return layout;
    }

    /* House cusps via Whole Sign. */
    house_system_t houses = houses_whole_sign(asc);
    for (int i = 0; i < 12; i++) {
        layout.cusp_signs[i] = houses.signs[i];
        float chart_angle = lon_to_chart_angle(houses.cusps[i], asc);
        layout.cusp_inner[i] = natal_chart_angle_to_point(
            chart_angle, INNER_RADIUS, CENTER_X, CENTER_Y);
        layout.cusp_outer[i] = natal_chart_angle_to_point(
            chart_angle, OUTER_RADIUS, CENTER_X, CENTER_Y);
    }

    /* Sign labels at midpoint of each 30-degree arc. */
    for (int i = 0; i < 12; i++) {
        double sign_mid_lon = houses.cusps[i] + 15.0;
        float chart_angle = lon_to_chart_angle(sign_mid_lon, asc);
        layout.sign_labels[i] = natal_chart_angle_to_point(
            chart_angle, LABEL_RADIUS, CENTER_X, CENTER_Y);
    }

    /* Place planets on the wheel. */
    layout.planet_count = NATAL_CHART_PLANET_COUNT;
    for (int i = 0; i < NATAL_CHART_PLANET_COUNT; i++) {
        double lon = planet_lons[i];
        int sign = zodiac_sign(lon);
        float deg = (float)zodiac_degree(lon);
        float chart_angle = lon_to_chart_angle(lon, asc);

        layout.planets[i].planet_index = i;
        layout.planets[i].sign = sign;
        layout.planets[i].degree = deg;
        layout.planets[i].pos = natal_chart_angle_to_point(
            chart_angle, PLANET_RADIUS, CENTER_X, CENTER_Y);

        /* Dignity: skip Earth (index 2) — it has no astrological dignity. */
        dignity_result_t dr = planet_dignity(i, sign);
        layout.planets[i].dignity = dr.score;
    }

    /* Find aspects between all 8 planets. */
    aspect_list_t alist = aspects_find_all(planet_lons, max_orb);
    layout.aspect_count = alist.count;
    for (int i = 0; i < alist.count; i++) {
        int pa = alist.aspects[i].planet_a;
        int pb = alist.aspects[i].planet_b;

        /* Aspect lines connect planet positions inside the inner ring. */
        layout.aspects[i].start = layout.planets[pa].pos;
        layout.aspects[i].end = layout.planets[pb].pos;
        layout.aspects[i].type = (int)alist.aspects[i].type;
        layout.aspects[i].orb = (float)alist.aspects[i].orb;
    }

    return layout;
}

const char *natal_chart_planet_sign_name(const natal_chart_layout_t *layout,
                                         int planet_index)
{
    if (!layout || planet_index < 0 ||
        planet_index >= NATAL_CHART_PLANET_COUNT) {
        return "?";
    }
    return zodiac_sign_name(layout->planets[planet_index].sign);
}
