#include "convergence_visual.h"

#include <math.h>
#include <stddef.h>
#include <string.h>

#define CV_PI 3.14159265358979323846
#define CV_DEG_TO_RAD (CV_PI / 180.0)

/* Brand colors (normalized float RGBA) */
static const float CV_TEAL[3] = { 0.2f, 0.75f, 0.8f };   /* celestial teal */
static const float CV_GOLD[3] = { 1.0f, 0.85f, 0.55f };   /* solar gold */

static float cv_clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

cv_frame_t cv_frame_init(void)
{
    cv_frame_t frame;
    memset(&frame, 0, sizeof(frame));
    return frame;
}

int cv_frame_add_line(cv_frame_t *frame, const cv_line_t *line)
{
    if (frame == NULL || line == NULL) return 0;
    if (frame->line_count >= CV_MAX_LINES) return 0;
    frame->lines[frame->line_count] = *line;
    frame->line_count++;
    return 1;
}

int cv_frame_add_glow(cv_frame_t *frame, const cv_glow_t *glow)
{
    if (frame == NULL || glow == NULL) return 0;
    if (frame->glow_count >= CV_MAX_GLOWS) return 0;
    frame->glows[frame->glow_count] = *glow;
    frame->glow_count++;
    return 1;
}

float cv_ring_midpoint(const cr_layout_t *layout, int ring_index)
{
    if (layout == NULL) return 0.0f;
    float inner = cr_ring_inner(layout, ring_index);
    float outer = cr_ring_outer(layout, ring_index);
    return (inner + outer) / 2.0f;
}

void cv_warmth_to_color(float warmth, float rgba[4])
{
    float t = cv_clampf(warmth, 0.0f, 1.0f);
    rgba[0] = CV_TEAL[0] + t * (CV_GOLD[0] - CV_TEAL[0]);
    rgba[1] = CV_TEAL[1] + t * (CV_GOLD[1] - CV_TEAL[1]);
    rgba[2] = CV_TEAL[2] + t * (CV_GOLD[2] - CV_TEAL[2]);
    rgba[3] = 1.0f;
}

cv_line_t cv_line_between_rings(const cr_layout_t *layout,
                                int ring_a_idx, int ring_b_idx,
                                float angle_a_deg, float angle_b_deg,
                                float opacity, float thickness, float warmth)
{
    cv_line_t line;
    memset(&line, 0, sizeof(line));

    if (layout == NULL) return line;

    float mid_a = cv_ring_midpoint(layout, ring_a_idx);
    float mid_b = cv_ring_midpoint(layout, ring_b_idx);

    float rad_a = (float)(angle_a_deg * CV_DEG_TO_RAD);
    float rad_b = (float)(angle_b_deg * CV_DEG_TO_RAD);

    line.x1 = mid_a * cosf(rad_a);
    line.z1 = mid_a * sinf(rad_a);
    line.x2 = mid_b * cosf(rad_b);
    line.z2 = mid_b * sinf(rad_b);

    line.opacity = opacity;
    line.thickness = thickness;

    cv_warmth_to_color(warmth, line.color);

    return line;
}
