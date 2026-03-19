/* pass_schedule.h — Render pass scheduling and orchestration.
 *
 * Determines which render passes execute per frame based on:
 *   - View mode (Space, Earth, Galaxy, etc.)
 *   - Focus mode (Overview, Astrology, Kin, etc.)
 *   - LOD tier (Ultra -> Minimal)
 *
 * Each pass gets an enable flag, priority/order value, and opacity scale.
 * The render loop iterates passes in order, skipping disabled ones.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_PASS_SCHEDULE_H
#define TIME_PASS_SCHEDULE_H

/* Enumerate all 17 render passes */
typedef enum {
    PASS_STARS         = 0,
    PASS_MILKYWAY      = 1,
    PASS_CONSTELLATION = 2,
    PASS_PLANET        = 3,
    PASS_MOON          = 4,
    PASS_SATURN        = 5,
    PASS_ORBIT_TRAIL   = 6,
    PASS_ZODIAC        = 7,
    PASS_DIFFRACTION   = 8,
    PASS_DEEP_SKY      = 9,
    PASS_EARTH         = 10,
    PASS_BODYGRAPH     = 11,
    PASS_HEXAGRAM      = 12,
    PASS_TREE          = 13,
    PASS_CARD          = 14,
    PASS_TEXT          = 15,
    PASS_POST          = 16,
    PASS_COUNT         = 17
} pass_id_t;

/* Per-pass state for one frame */
typedef struct {
    int   enabled;       /* 1 = draw this pass, 0 = skip */
    int   draw_order;    /* lower = drawn first (0 = background) */
    float opacity_scale; /* multiplier on pass opacity (0.0-1.0) */
} pass_state_t;

/* Complete schedule for all passes */
typedef struct {
    pass_state_t passes[PASS_COUNT];
    int          active_count; /* how many passes are enabled */
} pass_schedule_t;

/* Compute pass schedule for Space View (default). */
pass_schedule_t ps_space_view(int lod_tier);

/* Compute pass schedule for Earth View (ground-level sky). */
pass_schedule_t ps_earth_view(int lod_tier);

/* Compute pass schedule for Galaxy View (cosmic scale). */
pass_schedule_t ps_galaxy_view(int lod_tier);

/* Compute pass schedule for any view_id (int).
 * Unknown view IDs fall back to Space View. */
pass_schedule_t ps_for_view(int view_id, int lod_tier);

/* Apply focus mode overrides to a schedule.
 * Focused system's passes get opacity 1.0, others get dimmed. */
pass_schedule_t ps_apply_focus(pass_schedule_t schedule, int focus_mode);

/* Get the name of a render pass. Returns "Unknown" for out-of-range. */
const char *ps_pass_name(pass_id_t pass);

/* Is a specific pass enabled in this schedule? */
int ps_is_enabled(const pass_schedule_t *schedule, pass_id_t pass);

/* Count enabled passes. */
int ps_active_count(const pass_schedule_t *schedule);

#endif /* TIME_PASS_SCHEDULE_H */
