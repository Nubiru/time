/* structural_insight.h — Cross-system structural insight database
 *
 * Given two knowledge systems the user is viewing, surfaces the structural
 * parallel between them. Pre-authored insights describing mathematical,
 * symbolic, or conceptual connections between system pairs.
 *
 * Uses ts_system_t values from today_summary.h as system identifiers,
 * plus SI_SYS_* constants for systems not yet in that enum.
 *
 * All functions are pure: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Sefer Yetzirah, Ra Uru Hu (Human Design), Jose Arguelles
 * (Dreamspell), Reingold/Dershowitz (Calendrical Calculations),
 * Hindu Panchanga, Buddhist Abhidharma, geological time literature. */

#ifndef TIME_STRUCTURAL_INSIGHT_H
#define TIME_STRUCTURAL_INSIGHT_H

#include "today_summary.h"  /* ts_system_t, TS_SYS_COUNT */

#define SI_MAX_INSIGHTS 4

/* Extended system IDs for systems not in ts_system_t.
 * These start after TS_SYS_COUNT to avoid collision. */
#define SI_SYS_EGYPTIAN     (TS_SYS_COUNT + 0)
#define SI_SYS_CELTIC       (TS_SYS_COUNT + 1)
#define SI_SYS_ZOROASTRIAN  (TS_SYS_COUNT + 2)
#define SI_SYS_EXT_COUNT    (TS_SYS_COUNT + 3)

/* A single structural insight between two systems */
typedef struct {
    int system_a;               /* ts_system_t or SI_SYS_* */
    int system_b;               /* ts_system_t or SI_SYS_* */
    const char *title;          /* "The 64-fold Mirror" */
    const char *insight;        /* The actual insight text */
    int depth;                  /* 1=surface, 2=intermediate, 3=deep */
} si_entry_t;

/* Result from querying a system pair */
typedef struct {
    si_entry_t entries[SI_MAX_INSIGHTS];
    int count;
} si_result_t;

/* Query all insights connecting two systems.
 * Order does not matter: si_query(A, B) == si_query(B, A).
 * No insights or invalid IDs -> count=0. */
si_result_t si_query(int system_a, int system_b);

/* Total number of insights in the database. */
int si_all_count(void);

/* Get insight by global index (0 to si_all_count()-1).
 * Invalid index -> entry with system_a=-1, title="?". */
si_entry_t si_get(int index);

/* List system IDs that have insights paired with the given system.
 * Writes partner IDs to out_partners (up to max).
 * If out_partners is NULL, returns count only.
 * Invalid system -> 0. */
int si_pairs_with(int system_id, int *out_partners, int max);

/* Human-readable depth name.
 * 1->"Surface", 2->"Intermediate", 3->"Deep", else "?". */
const char *si_depth_name(int depth);

#endif /* TIME_STRUCTURAL_INSIGHT_H */
