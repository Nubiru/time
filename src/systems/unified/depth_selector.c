/* depth_selector.c — Knowledge system visibility ranking
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: today_summary.h types, convergence_detect.h concepts. */

#include "depth_selector.h"

/* Check if system_id appears in the convergence list. */
static int is_converging(const ds_convergence_t *conv, int system_id)
{
    if (!conv) return 0;
    for (int i = 0; i < conv->converging_count && i < DS_MAX_SYSTEMS; i++) {
        if (conv->converging_systems[i] == system_id) return 1;
    }
    return 0;
}

/* Check if system_id appears in the birth systems list. */
static int is_birth_system(const ds_birth_t *birth, int system_id)
{
    if (!birth) return 0;
    for (int i = 0; i < birth->birth_count && i < DS_MAX_SYSTEMS; i++) {
        if (birth->birth_systems[i] == system_id) return 1;
    }
    return 0;
}

/* Clamp visibility to valid range. */
static ds_visibility_t clamp_vis(int v)
{
    if (v < DS_VIS_HIDDEN) return DS_VIS_HIDDEN;
    if (v > DS_VIS_HIGHLIGHTED) return DS_VIS_HIGHLIGHTED;
    return (ds_visibility_t)v;
}

/* Promote visibility to at least the given tier (never demote). */
static ds_visibility_t promote_to_at_least(ds_visibility_t current,
                                           ds_visibility_t minimum)
{
    return (current >= minimum) ? current : minimum;
}

/* Compare two ds_entry_t for sorting: higher vis first, then higher score. */
static void sort_entries(ds_entry_t *entries, int count)
{
    /* Simple insertion sort — count is small (max 24). */
    for (int i = 1; i < count; i++) {
        ds_entry_t key = entries[i];
        int j = i - 1;
        while (j >= 0 &&
               (entries[j].vis < key.vis ||
                (entries[j].vis == key.vis && entries[j].score < key.score))) {
            entries[j + 1] = entries[j];
            j--;
        }
        entries[j + 1] = key;
    }
}

ds_selection_t ds_select(const ts_summary_t *today,
                         const ds_convergence_t *conv,
                         const ds_birth_t *birth,
                         unsigned int user_toggles)
{
    ds_selection_t sel;
    sel.count = 0;
    sel.highlighted_count = 0;
    sel.prominent_count = 0;

    if (!today) return sel;

    /* Pass 1: compute visibility for each active entry */
    for (int i = 0; i < today->entry_count && i < TS_MAX_SYSTEMS; i++) {
        const ts_entry_t *e = &today->entries[i];
        if (!e->active) continue;

        int sys_id = (int)e->system;
        int sig = e->significance;

        /* a. Start at DORMANT */
        ds_visibility_t vis = DS_VIS_DORMANT;

        /* b. Gregorian anchor */
        if (sys_id == TS_SYS_GREGORIAN) {
            vis = promote_to_at_least(vis, DS_VIS_VISIBLE);
        }

        /* c. Significance >= 2 -> PROMINENT */
        if (sig >= 2) {
            vis = promote_to_at_least(vis, DS_VIS_PROMINENT);
        }

        /* d. Significance >= 3 -> HIGHLIGHTED */
        if (sig >= 3) {
            vis = promote_to_at_least(vis, DS_VIS_HIGHLIGHTED);
        }

        /* e. Birth systems -> at least VISIBLE */
        if (is_birth_system(birth, sys_id)) {
            vis = promote_to_at_least(vis, DS_VIS_VISIBLE);
        }

        /* f. Converging systems: +1 tier */
        if (is_converging(conv, sys_id)) {
            vis = clamp_vis((int)vis + 1);
        }

        /* g. Strong convergence (>=3) -> at least HIGHLIGHTED */
        if (conv && conv->convergence_strength >= 3 &&
            is_converging(conv, sys_id)) {
            vis = promote_to_at_least(vis, DS_VIS_HIGHLIGHTED);
        }

        /* h. User toggles */
        if (user_toggles != 0xFFFFFFFF) {
            unsigned int bit = (1u << (unsigned int)sys_id);
            if (!(user_toggles & bit)) {
                /* Bit clear -> HIDDEN */
                vis = DS_VIS_HIDDEN;
            } else if (vis == DS_VIS_DORMANT) {
                /* Bit set + DORMANT -> promote to VISIBLE */
                vis = DS_VIS_VISIBLE;
            }
        }

        /* Build entry */
        ds_entry_t *out = &sel.entries[sel.count];
        out->system_id = sys_id;
        out->vis = vis;
        out->rank = 0; /* assigned after sort */
        out->score = sig;
        sel.count++;
    }

    /* i. Sort: HIGHLIGHTED first, then PROMINENT, VISIBLE, DORMANT.
     *    Within same tier, higher significance first. */
    sort_entries(sel.entries, sel.count);

    /* Assign ranks and compute counts */
    for (int i = 0; i < sel.count; i++) {
        sel.entries[i].rank = i;
        if (sel.entries[i].vis == DS_VIS_HIGHLIGHTED) {
            sel.highlighted_count++;
        }
        if (sel.entries[i].vis >= DS_VIS_PROMINENT) {
            sel.prominent_count++;
        }
    }

    return sel;
}

const char *ds_visibility_name(ds_visibility_t vis)
{
    switch (vis) {
    case DS_VIS_HIDDEN:      return "Hidden";
    case DS_VIS_DORMANT:     return "Dormant";
    case DS_VIS_VISIBLE:     return "Visible";
    case DS_VIS_PROMINENT:   return "Prominent";
    case DS_VIS_HIGHLIGHTED: return "Highlighted";
    default:                 return "?";
    }
}

int ds_visibility_count(void)
{
    return DS_VIS_COUNT;
}
