/* depth_selector.h — Knowledge system visibility ranking
 *
 * Ranks all knowledge systems by visibility tier based on current
 * significance, convergence events, birth profile relevance, and
 * user toggles. Produces a sorted selection with rank and score.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: today_summary.h (ts_summary_t, ts_system_t),
 *          convergence_detect.h (cd_system_t, cd_strength_t). */

#ifndef TIME_DEPTH_SELECTOR_H
#define TIME_DEPTH_SELECTOR_H

#include "today_summary.h"  /* ts_summary_t, ts_entry_t, ts_system_t */

typedef enum {
    DS_VIS_HIDDEN      = 0,  /* user toggled off */
    DS_VIS_DORMANT     = 1,  /* available but not shown */
    DS_VIS_VISIBLE     = 2,  /* compact line in list */
    DS_VIS_PROMINENT   = 3,  /* standard card, visible without scrolling */
    DS_VIS_HIGHLIGHTED = 4,  /* glowing card, top position */
    DS_VIS_COUNT       = 5
} ds_visibility_t;

typedef struct {
    int system_id;           /* ts_system_t value */
    ds_visibility_t vis;     /* computed visibility */
    int rank;                /* 0 = most prominent, higher = less */
    int score;               /* raw relevance score for debugging */
} ds_entry_t;

#define DS_MAX_SYSTEMS 24

typedef struct {
    ds_entry_t entries[DS_MAX_SYSTEMS];
    int count;               /* number of valid entries */
    int highlighted_count;   /* entries with vis == HIGHLIGHTED */
    int prominent_count;     /* entries with vis >= PROMINENT */
} ds_selection_t;

/* Simplified convergence input (decoupled from cd_result_t). */
typedef struct {
    int converging_systems[DS_MAX_SYSTEMS]; /* ts_system_t IDs that are converging */
    int converging_count;                    /* how many are converging (0 if none) */
    int convergence_strength;                /* 0=none, 1=minor, 2=notable, 3=major, 4=rare */
} ds_convergence_t;

/* Simplified birth relevance input. */
typedef struct {
    int birth_systems[DS_MAX_SYSTEMS]; /* ts_system_t IDs relevant to user's birth */
    int birth_count;                    /* how many birth systems */
} ds_birth_t;

/* Compute visibility selection for all active systems.
 *
 * today:        summary with active system entries (NULL -> empty selection)
 * conv:         convergence data (NULL -> skip convergence promotion)
 * birth:        birth profile data (NULL -> skip birth promotion)
 * user_toggles: bitmask, bit N = ts_system_t N. 0xFFFFFFFF = no filtering.
 *               Bit clear -> HIDDEN. Bit set + DORMANT -> promote to VISIBLE.
 *
 * Returns: sorted selection ranked by visibility tier then significance. */
ds_selection_t ds_select(const ts_summary_t *today,
                         const ds_convergence_t *conv,
                         const ds_birth_t *birth,
                         unsigned int user_toggles);

/* Human-readable visibility name.
 * Returns: "Hidden", "Dormant", "Visible", "Prominent", "Highlighted",
 *          or "?" for invalid. */
const char *ds_visibility_name(ds_visibility_t vis);

/* Number of visibility levels (always DS_VIS_COUNT = 5). */
int ds_visibility_count(void);

#endif /* TIME_DEPTH_SELECTOR_H */
