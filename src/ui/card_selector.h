/* card_selector.h — Zoom-aware card system selection.
 *
 * Selects which 5 knowledge systems should display in the card
 * overlay based on current zoom depth. Maps card_depth's 20-system
 * analysis to the 5-slot card rendering system.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CARD_SELECTOR_H
#define TIME_CARD_SELECTOR_H

#include "card_depth.h"

#define CS_SLOT_COUNT 5

/* A card slot assignment */
typedef struct {
    int system_id;      /* ts_system_t value, or -1 if empty */
    float relevance;    /* 0.0-1.0: how relevant at current depth */
    float opacity;      /* 0.0-1.0: display opacity (from card_depth) */
    float scale;        /* 0.0-1.0: display scale (from card_depth) */
} cs_slot_t;

/* The full selection for one frame */
typedef struct {
    cs_slot_t slots[CS_SLOT_COUNT];
    int filled_count;   /* how many slots have system_id >= 0 */
    float zoom_level;   /* the zoom level this selection was computed for */
} cs_selection_t;

/* Select the top 5 systems for display at a given zoom level.
 * Uses card_depth internally (cd_default_planes + cd_compute).
 * aspect_ratio: viewport width/height */
cs_selection_t cs_select(float zoom_level, float aspect_ratio);

/* Select using a pre-computed cd_frame_t (avoids recomputing card_depth). */
cs_selection_t cs_select_from_frame(const cd_frame_t *frame);

/* Check if a specific system is in the selection.
 * Returns slot index (0-4), or -1 if absent. */
int cs_find_system(const cs_selection_t *sel, int system_id);

/* Get the system name for a slot (for display).
 * Returns NULL for empty slots (system_id == -1). */
const char *cs_slot_name(const cs_slot_t *slot);

#endif /* TIME_CARD_SELECTOR_H */
