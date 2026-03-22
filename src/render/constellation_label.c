/*
 * constellation_label.c — Constellation name label positions
 *
 * Computes centroids from constellation boundary vertices.
 * Averages all boundary RA/Dec points, then projects to Cartesian XYZ.
 *
 * Pure module: no GL, no malloc, no globals.
 */

#include "constellation_label.h"
#include "constellation_bounds.h"

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define DEG2RAD(d) ((d) * PI / 180.0)

/* Compute centroid of boundary vertices for one constellation.
 * Average RA in hours, average Dec in degrees, then project to XYZ. */
static cl_label_t compute_one(int index, float sphere_radius)
{
    cl_label_t label;
    cb_constellation_t meta = cb_get(index);

    if (meta.name == 0) {
        label.name = 0;
        label.abbr = 0;
        label.x = label.y = label.z = 0.0f;
        label.ra_hours = label.dec_deg = 0.0f;
        label.is_zodiac = 0;
        return label;
    }

    label.name = meta.name;
    label.abbr = meta.abbr;
    label.is_zodiac = meta.is_zodiac;

    /* Average boundary vertices for centroid.
     * Use sin/cos averaging for RA (circular quantity). */
    int n = cb_vertex_count(index);
    if (n < 1) {
        label.ra_hours = 0.0f;
        label.dec_deg = 0.0f;
        label.x = label.y = label.z = 0.0f;
        return label;
    }

    double sin_ra_sum = 0.0, cos_ra_sum = 0.0, dec_sum = 0.0;
    for (int i = 0; i < n; i++) {
        cb_point_t p = cb_vertex(index, i);
        double ra_rad = p.ra_hours * PI / 12.0;  /* hours to radians */
        sin_ra_sum += sin(ra_rad);
        cos_ra_sum += cos(ra_rad);
        dec_sum += (double)p.dec_deg;
    }

    double avg_ra_rad = atan2(sin_ra_sum, cos_ra_sum);
    if (avg_ra_rad < 0.0) avg_ra_rad += 2.0 * PI;
    double avg_dec_deg = dec_sum / (double)n;

    label.ra_hours = (float)(avg_ra_rad * 12.0 / PI);
    label.dec_deg  = (float)avg_dec_deg;

    /* Project RA/Dec to Cartesian XYZ on celestial sphere */
    double ra_rad = avg_ra_rad;
    double dec_rad = DEG2RAD(avg_dec_deg);
    label.x =  sphere_radius * (float)(cos(dec_rad) * cos(ra_rad));
    label.y =  sphere_radius * (float)(sin(dec_rad));
    label.z = -sphere_radius * (float)(cos(dec_rad) * sin(ra_rad));

    return label;
}

int cl_compute(float sphere_radius, cl_label_t *out)
{
    if (!out) return 0;

    int count = CB_CONSTELLATION_COUNT;
    if (count > CL_COUNT) count = CL_COUNT;

    for (int i = 0; i < count; i++) {
        out[i] = compute_one(i, sphere_radius);
    }

    return count;
}

cl_label_t cl_get(int index, float sphere_radius)
{
    if (index < 0 || index >= CB_CONSTELLATION_COUNT) {
        cl_label_t empty;
        empty.name = 0;
        empty.abbr = 0;
        empty.x = empty.y = empty.z = 0.0f;
        empty.ra_hours = empty.dec_deg = 0.0f;
        empty.is_zodiac = 0;
        return empty;
    }

    return compute_one(index, sphere_radius);
}
