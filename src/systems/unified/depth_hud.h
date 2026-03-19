/* depth_hud.h — HUD integration data for knowledge systems
 *
 * Defines where each system appears in the HUD, what toggle key
 * activates it, and format templates per depth tier. This is the
 * data spec that visuals read to render system-specific HUD elements.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Visual design spec, depth tier system, today_summary system IDs. */

#ifndef TIME_DEPTH_HUD_H
#define TIME_DEPTH_HUD_H

#include "today_summary.h"  /* ts_system_t, TS_SYS_COUNT */
#include "depth_tier.h"     /* depth_tier_t */

/* HUD position regions */
typedef enum {
    HUD_POS_TOP_LEFT = 0,
    HUD_POS_TOP_CENTER,
    HUD_POS_TOP_RIGHT,
    HUD_POS_BOTTOM_LEFT,
    HUD_POS_BOTTOM_CENTER,
    HUD_POS_BOTTOM_RIGHT,
    HUD_POS_LEFT_PANEL,
    HUD_POS_RIGHT_PANEL,
    HUD_POS_COUNT
} hud_position_t;

/* Per-system HUD spec */
typedef struct {
    int system_id;              /* ts_system_t value */
    const char *display_name;   /* Short name for HUD: "Hebrew", "Islamic", etc. */
    char toggle_key;            /* Keyboard shortcut: 'H', 'I', 'B', 'V', 'Q', 'U' */
    hud_position_t position;    /* Default HUD region */
    const char *format_t0;     /* Format template at T0 (glyph): "{glyph}" */
    const char *format_t1;     /* Format template at T1 (glance): "{year} {month} {day}" */
    const char *format_t2;     /* Format template at T2 (card): "{full_date} -- {meaning}" */
    int priority;               /* Display priority (lower = shown first, 0 = highest) */
} dh_system_spec_t;

/* HUD layout result -- all visible systems at current tier */
typedef struct {
    dh_system_spec_t specs[24];
    int count;
} dh_layout_t;

/* Get the HUD spec for a given system ID (0-19).
 * Invalid ID returns struct with system_id=-1, display_name="?". */
dh_system_spec_t dh_system_spec(int system_id);

/* Find system by toggle key. Returns ts_system_t or -1 if not found. */
int dh_system_by_key(char key);

/* Get system IDs assigned to a given HUD position.
 * Fills out_ids (up to max entries), returns count.
 * If out_ids is NULL, returns count only. */
int dh_systems_at_position(hud_position_t pos, int *out_ids, int max);

/* Human-readable position name.
 * Returns: "Top Left", "Top Center", ..., "?" for invalid. */
const char *dh_position_name(hud_position_t pos);

/* Total number of systems (TS_SYS_COUNT = 20). */
int dh_system_count(void);

/* Total number of HUD positions (HUD_POS_COUNT = 8). */
int dh_position_count(void);

#endif /* TIME_DEPTH_HUD_H */
