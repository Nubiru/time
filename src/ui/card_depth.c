/* card_depth.c — Card positioning in zoom-depth space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "card_depth.h"
#include <math.h>

/* System IDs from ts_system_t (today_summary.h) — copied as constants
 * to keep card_depth self-contained (no header dependency). */
enum {
    CD_SYS_GREGORIAN    =  0,
    CD_SYS_TZOLKIN      =  1,
    CD_SYS_HAAB         =  2,
    CD_SYS_CHINESE      =  3,
    CD_SYS_HEBREW       =  4,
    CD_SYS_ISLAMIC      =  5,
    CD_SYS_BUDDHIST     =  6,
    CD_SYS_HINDU        =  7,
    CD_SYS_ICHING       =  8,
    CD_SYS_ASTROLOGY    =  9,
    CD_SYS_HUMAN_DESIGN = 10,
    CD_SYS_KABBALAH     = 11,
    CD_SYS_COPTIC       = 12,
    CD_SYS_ETHIOPIAN    = 13,
    CD_SYS_PERSIAN      = 14,
    CD_SYS_JAPANESE     = 15,
    CD_SYS_KOREAN       = 16,
    CD_SYS_THAI         = 17,
    CD_SYS_GEOLOGICAL   = 18,
    CD_SYS_COSMIC       = 19
};

/* Visibility threshold for the 'visible' flag */
#define CD_VISIBLE_THRESHOLD 0.01f

/* Default plane table — static, never changes */
static const cd_plane_t s_default_planes[] = {
    /* Ring 0 — Cosmic (depth 0.5) */
    { CD_SYS_COSMIC,       0.5f, 0.5f },
    { CD_SYS_GEOLOGICAL,   0.5f, 0.5f },

    /* Ring 1 — Age (depth 1.5) */
    { CD_SYS_BUDDHIST,     1.5f, 0.5f },
    { CD_SYS_HINDU,        1.5f, 0.5f },

    /* Ring 2 — Era (depth 2.5) */
    { CD_SYS_KABBALAH,     2.5f, 0.5f },
    { CD_SYS_HUMAN_DESIGN, 2.5f, 0.6f },

    /* Ring 3 — Year (depth 3.5) */
    { CD_SYS_ASTROLOGY,    3.5f, 0.9f },
    { CD_SYS_CHINESE,      3.5f, 0.8f },
    { CD_SYS_HEBREW,       3.5f, 0.5f },
    { CD_SYS_ISLAMIC,      3.5f, 0.5f },
    { CD_SYS_COPTIC,       3.5f, 0.5f },
    { CD_SYS_ETHIOPIAN,    3.5f, 0.5f },
    { CD_SYS_PERSIAN,      3.5f, 0.5f },
    { CD_SYS_THAI,         3.5f, 0.5f },

    /* Ring 4 — Cycle (depth 4.5) */
    { CD_SYS_ICHING,       4.5f, 0.7f },
    { CD_SYS_KOREAN,       4.5f, 0.5f },
    { CD_SYS_JAPANESE,     4.5f, 0.5f },

    /* Ring 5 — Today (depth 5.5) */
    { CD_SYS_GREGORIAN,    5.5f, 0.5f },
    { CD_SYS_TZOLKIN,      5.5f, 1.0f },
    { CD_SYS_HAAB,         5.5f, 0.5f }
};

#define CD_DEFAULT_COUNT \
    ((int)(sizeof(s_default_planes) / sizeof(s_default_planes[0])))

int cd_default_planes(cd_plane_t *out, int max_count)
{
    if (!out || max_count <= 0) {
        return 0;
    }

    int count = CD_DEFAULT_COUNT;
    if (count > max_count) {
        count = max_count;
    }

    for (int i = 0; i < count; i++) {
        out[i] = s_default_planes[i];
    }

    return count;
}

float cd_depth_distance(float zoom_level, float plane_depth)
{
    return plane_depth - zoom_level;
}

cd_frame_t cd_compute(float zoom_level, const cd_plane_t *planes,
                      int plane_count, float aspect_ratio)
{
    cd_frame_t frame;
    frame.focal_depth = zoom_level;
    frame.focal_index = -1;
    frame.count = 0;

    if (!planes || plane_count <= 0) {
        return frame;
    }

    int count = plane_count;
    if (count > CD_MAX_CARDS) {
        count = CD_MAX_CARDS;
    }

    /* First pass: count cards at each unique depth for layout grouping.
     * We track up to 6 depth groups (rings 0-5). */
    float group_depths[6];
    int group_counts[6];
    int group_total = 0;

    for (int i = 0; i < count; i++) {
        float d = planes[i].depth;
        int found = 0;
        for (int g = 0; g < group_total; g++) {
            if (fabsf(group_depths[g] - d) < 0.01f) {
                group_counts[g]++;
                found = 1;
                break;
            }
        }
        if (!found && group_total < 6) {
            group_depths[group_total] = d;
            group_counts[group_total] = 1;
            group_total++;
        }
    }

    float max_opacity = -1.0f;
    (void)aspect_ratio; /* reserved for future layout refinement */

    for (int i = 0; i < count; i++) {
        cd_card_state_t *card = &frame.cards[i];
        card->system_id = planes[i].system_id;

        float distance = cd_depth_distance(zoom_level, planes[i].depth);

        /* Phi-based scale falloff */
        float abs_dist = fabsf(distance);
        float scale = 1.0f / (1.0f + abs_dist * CD_PHI);

        /* Quadratic opacity falloff (more natural visual fading) */
        float opacity = scale * scale;

        /* For non-focal depth groups, fade lower-priority cards faster */
        if (abs_dist > 0.01f) {
            float priority_fade = planes[i].priority;
            opacity *= priority_fade;
        }

        /* Clamp opacity to [0, 1] */
        if (opacity < 0.0f) { opacity = 0.0f; }
        if (opacity > 1.0f) { opacity = 1.0f; }

        card->scale = scale;
        card->opacity = opacity;
        card->visible = (opacity > CD_VISIBLE_THRESHOLD) ? 1 : 0;
        card->z = distance;

        /* Y offset: slight variation by depth group */
        card->y = 0.0f;

        /* X offset: spread cards within same depth group */
        int group_idx = -1;
        int card_index_in_group = 0;
        {
            /* Find which group this card belongs to */
            int running = 0;
            for (int g = 0; g < group_total; g++) {
                if (fabsf(group_depths[g] - planes[i].depth) < 0.01f) {
                    group_idx = g;
                    break;
                }
            }
            /* Count how many cards before this one share the same depth */
            for (int j = 0; j < i; j++) {
                if (fabsf(planes[j].depth - planes[i].depth) < 0.01f) {
                    running++;
                }
            }
            card_index_in_group = running;
        }

        int group_size = 1;
        if (group_idx >= 0 && group_idx < group_total) {
            group_size = group_counts[group_idx];
        }

        float group_offset = ((float)card_index_in_group
                              - (float)(group_size - 1) / 2.0f) * 0.35f;
        card->x = group_offset * scale;

        /* Track focal card (highest opacity) */
        if (opacity > max_opacity) {
            max_opacity = opacity;
            frame.focal_index = i;
        }
    }

    frame.count = count;
    return frame;
}

int cd_visible_count(const cd_frame_t *frame)
{
    if (!frame) {
        return 0;
    }

    int vis = 0;
    for (int i = 0; i < frame->count; i++) {
        if (frame->cards[i].visible) {
            vis++;
        }
    }
    return vis;
}

const cd_card_state_t *cd_find_system(const cd_frame_t *frame, int system_id)
{
    if (!frame) {
        return (void *)0;
    }

    for (int i = 0; i < frame->count; i++) {
        if (frame->cards[i].system_id == system_id) {
            return &frame->cards[i];
        }
    }
    return (void *)0;
}
