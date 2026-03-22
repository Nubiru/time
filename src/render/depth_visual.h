/*
 * depth_visual.h — Depth tier visual properties for concentric navigation
 *
 * Maps log_zoom to a depth tier and provides per-tier visual properties:
 * tier label, background tint color, and tint opacity.
 *
 * Makes depth navigation FEEL different at each layer — not just
 * card swapping, but actual visual depth cues.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * Header-only — all functions are static inline.
 */

#ifndef TIME_DEPTH_VISUAL_H
#define TIME_DEPTH_VISUAL_H

/* Depth tier (derived from log_zoom, simplified from zoom_depth.h) */
typedef enum {
    DV_TIER_GLYPH   = 0,  /* Far zoom: one symbol only */
    DV_TIER_GLANCE  = 1,  /* Medium: one-line summary */
    DV_TIER_CARD    = 2,  /* Close: full card */
    DV_TIER_CONTEXT = 3,  /* Closer: context + cycles */
    DV_TIER_BOARD   = 4,  /* Closest: full board view */
} dv_tier_t;

/* Per-tier visual tint (RGBA) */
typedef struct {
    float r, g, b, a;
} dv_tint_t;

/* Map log_zoom to depth tier */
static inline dv_tier_t dv_tier_from_zoom(float log_zoom)
{
    if (log_zoom > 2.0f)  return DV_TIER_GLYPH;
    if (log_zoom > 0.7f)  return DV_TIER_GLANCE;
    if (log_zoom > -0.7f) return DV_TIER_CARD;
    if (log_zoom > -2.5f) return DV_TIER_CONTEXT;
    return DV_TIER_BOARD;
}

/* Get tier display name */
static inline const char *dv_tier_name(dv_tier_t tier)
{
    switch (tier) {
    case DV_TIER_GLYPH:   return "GLYPH";
    case DV_TIER_GLANCE:  return "GLANCE";
    case DV_TIER_CARD:    return "CARD";
    case DV_TIER_CONTEXT: return "CONTEXT";
    case DV_TIER_BOARD:   return "BOARD";
    default:              return "";
    }
}

/* Get background tint for depth tier.
 * Subtle color shift makes each tier FEEL different.
 *   Glyph: no tint — pure dark cosmos
 *   Glance: warm gold (nearby star warmth)
 *   Card: no tint (default view)
 *   Context: teal (deeper, cooler)
 *   Board: deep purple (cosmic depth) */
static inline dv_tint_t dv_tier_tint(dv_tier_t tier)
{
    dv_tint_t t;
    switch (tier) {
    case DV_TIER_GLYPH:
        t.r = 0.0f; t.g = 0.0f; t.b = 0.0f; t.a = 0.0f;
        break;
    case DV_TIER_GLANCE:
        t.r = 1.0f; t.g = 0.85f; t.b = 0.55f; t.a = 0.03f; /* solar gold */
        break;
    case DV_TIER_CARD:
        t.r = 0.0f; t.g = 0.0f; t.b = 0.0f; t.a = 0.0f; /* no tint */
        break;
    case DV_TIER_CONTEXT:
        t.r = 0.2f; t.g = 0.75f; t.b = 0.8f; t.a = 0.05f; /* celestial teal */
        break;
    case DV_TIER_BOARD:
        t.r = 0.4f; t.g = 0.2f; t.b = 0.7f; t.a = 0.03f; /* cosmic purple */
        break;
    default:
        t.r = 0.0f; t.g = 0.0f; t.b = 0.0f; t.a = 0.0f;
    }
    return t;
}

/* Smooth blend factor between two adjacent tiers (0.0-1.0).
 * Prevents hard transitions when zooming between tiers. */
static inline float dv_tier_blend(float log_zoom, dv_tier_t tier)
{
    float mid;
    float width = 0.5f; /* blend zone half-width */
    switch (tier) {
    case DV_TIER_GLYPH:   mid = 2.5f;  break;
    case DV_TIER_GLANCE:  mid = 1.35f; break;
    case DV_TIER_CARD:    mid = 0.0f;  break;
    case DV_TIER_CONTEXT: mid = -1.6f; break;
    case DV_TIER_BOARD:   mid = -3.5f; break;
    default:              return 0.0f;
    }
    float dist = log_zoom - mid;
    if (dist < 0.0f) dist = -dist;
    float t = 1.0f - dist / width;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return t;
}

#endif /* TIME_DEPTH_VISUAL_H */
