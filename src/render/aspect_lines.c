#include "aspect_lines.h"
#include "color_palette.h"

#include <math.h>

/* Aspect angles in degrees. Index matches type field. */
static const double ASPECT_ANGLES[5] = {
    0.0,    /* 0 = conjunction */
    180.0,  /* 1 = opposition */
    120.0,  /* 2 = trine */
    90.0,   /* 3 = square */
    60.0    /* 4 = sextile */
};

static double angular_sep(double lon_a, double lon_b)
{
    double d = fmod(lon_a - lon_b + 360.0, 360.0);
    if (d > 180.0) d = 360.0 - d;
    return d;
}

static int detect_aspect(double separation, double max_orb, double *out_orb)
{
    for (int i = 0; i < 5; i++) {
        double dev = fabs(separation - ASPECT_ANGLES[i]);
        if (dev <= max_orb) {
            *out_orb = dev;
            return i;
        }
    }
    return -1;
}

void aspect_line_color(int type, float *r, float *g, float *b)
{
    if (type < 0 || type > 4) {
        *r = 1.0f;
        *g = 1.0f;
        *b = 1.0f;
        return;
    }
    color_rgb_t c = color_aspect(type);
    *r = c.r;
    *g = c.g;
    *b = c.b;
}

float aspect_line_opacity(double orb_degrees, double max_orb)
{
    if (max_orb <= 0.0) return 1.0f;
    double ratio = orb_degrees / max_orb;
    if (ratio < 0.0) ratio = 0.0;
    if (ratio > 1.0) ratio = 1.0;
    return (float)(1.0 - 0.8 * ratio);
}

aspect_line_set_t aspect_lines_generate(const float *positions,
                                        const double *longitudes,
                                        int num_planets,
                                        double orb)
{
    aspect_line_set_t set;
    set.count = 0;

    if (num_planets < 2) return set;
    if (num_planets > 8) num_planets = 8;

    for (int i = 0; i < num_planets - 1; i++) {
        for (int j = i + 1; j < num_planets; j++) {
            if (set.count >= MAX_ASPECT_LINES) return set;

            double sep = angular_sep(longitudes[i], longitudes[j]);
            double dev = 0.0;
            int type = detect_aspect(sep, orb, &dev);

            if (type < 0) continue;

            aspect_line_t *line = &set.lines[set.count];
            line->x1 = positions[i * 3 + 0];
            line->y1 = positions[i * 3 + 1];
            line->z1 = positions[i * 3 + 2];
            line->x2 = positions[j * 3 + 0];
            line->y2 = positions[j * 3 + 1];
            line->z2 = positions[j * 3 + 2];

            aspect_line_color(type, &line->r, &line->g, &line->b);
            line->opacity = aspect_line_opacity(dev, orb);
            line->type = type;

            set.count++;
        }
    }

    return set;
}
