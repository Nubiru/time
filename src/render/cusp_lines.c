#include "cusp_lines.h"

#include <math.h>
#include <stddef.h>

#define PI 3.14159265358979323846

int cusp_is_angular(int house_index)
{
    return (house_index == 0 || house_index == 3 ||
            house_index == 6 || house_index == 9) ? 1 : 0;
}

const char *cusp_axis_name(int house_index)
{
    switch (house_index) {
    case 0: return "ASC";
    case 3: return "IC";
    case 6: return "DSC";
    case 9: return "MC";
    default: return NULL;
    }
}

cusp_line_set_t cusp_lines_generate(const double *cusps,
                                     float inner_radius, float outer_radius)
{
    cusp_line_set_t set;
    set.count = 0;

    if (!cusps) return set;

    for (int i = 0; i < 12; i++) {
        float angle = (float)(cusps[i] * PI / 180.0);
        float c = cosf(angle);
        float s = sinf(angle);

        cusp_line_t *line = &set.lines[set.count];
        line->x1 = inner_radius * c;
        line->y1 = 0.0f;
        line->z1 = inner_radius * s;
        line->x2 = outer_radius * c;
        line->y2 = 0.0f;
        line->z2 = outer_radius * s;
        line->house_index = i;
        line->is_angular = cusp_is_angular(i);
        line->thickness = line->is_angular ? 2.0f : 1.0f;

        set.count++;
    }

    return set;
}
