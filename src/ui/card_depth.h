/* card_depth.h — Card positioning in zoom-depth space.
 *
 * Computes where each knowledge system's card appears as
 * the user zooms through information layers. Cards near
 * the focal depth plane are large and visible; distant
 * cards fade and shrink.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CARD_DEPTH_H
#define TIME_CARD_DEPTH_H

#define CD_MAX_CARDS 20
#define CD_PHI 1.618033988749895f

/* Depth plane assignment — which zoom level a system "lives" at */
typedef struct {
    int system_id;      /* ts_system_t value */
    float depth;        /* zoom depth where this card is centered (0.0-6.0) */
    float priority;     /* 0.0-1.0, higher = more important at this depth */
} cd_plane_t;

/* Computed card visual state for one system */
typedef struct {
    int system_id;
    float x;            /* screen-space X position (-1 to 1, 0 = center) */
    float y;            /* screen-space Y position (-1 to 1, 0 = center) */
    float z;            /* depth (0 = focal plane, negative = behind camera) */
    float scale;        /* 0.0-1.0 size multiplier (1.0 at focal plane) */
    float opacity;      /* 0.0-1.0 (1.0 at focal plane, fading with distance) */
    int visible;        /* 1 if opacity > 0.01, 0 otherwise */
} cd_card_state_t;

/* All card states for one frame */
typedef struct {
    cd_card_state_t cards[CD_MAX_CARDS];
    int count;              /* number of valid entries */
    int focal_index;        /* index of the card nearest to focal plane (-1 if none) */
    float focal_depth;      /* current zoom depth (the "camera" position) */
} cd_frame_t;

/* Get the default depth plane assignments for all knowledge systems.
 * Returns count (number of planes written to out, max CD_MAX_CARDS).
 * Systems are grouped by timescale:
 *   depth 0.5: Cosmic, Geological (deep time)
 *   depth 1.5: Buddhist, Hindu (age/era scale)
 *   depth 2.5: Kabbalah, Human Design (structural)
 *   depth 3.5: Astrology, Chinese, Hebrew, Islamic, Coptic, Ethiopian,
 *              Persian, Thai (year scale)
 *   depth 4.5: I Ching, Korean, Japanese (cycle scale)
 *   depth 5.5: Gregorian, Tzolkin, Haab (today scale) */
int cd_default_planes(cd_plane_t *out, int max_count);

/* Compute card states for all systems at a given zoom depth.
 * zoom_level: current camera zoom (0.0-6.0, from scroll_to_zoom)
 * planes: depth plane assignments (from cd_default_planes)
 * plane_count: number of planes
 * aspect_ratio: viewport width/height (for layout) */
cd_frame_t cd_compute(float zoom_level, const cd_plane_t *planes,
                      int plane_count, float aspect_ratio);

/* Get the visible card count (cards with opacity > threshold). */
int cd_visible_count(const cd_frame_t *frame);

/* Find the card state for a specific system ID.
 * Returns pointer to card_state within frame, or NULL if not found. */
const cd_card_state_t *cd_find_system(const cd_frame_t *frame, int system_id);

/* Compute depth distance between camera and a depth plane.
 * Positive = plane is ahead (further), negative = plane is behind. */
float cd_depth_distance(float zoom_level, float plane_depth);

#endif /* TIME_CARD_DEPTH_H */
