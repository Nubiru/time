/* stored_convergences.h — Static database of notable convergence dates
 *
 * Pre-defined list of historically and culturally significant dates
 * with their convergence data computed on demand via cd_scan.
 * Provides a curated "convergence timeline" for the user to explore.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_STORED_CONVERGENCES_H
#define TIME_STORED_CONVERGENCES_H

#include "convergence_detect.h"

/* Maximum stored entries */
#define SC_MAX_ENTRIES 128

/* A stored convergence entry */
typedef struct {
    double          jd;
    const char     *name;           /* human-readable name */
    const char     *category;       /* "historical", "astronomical", "2026" */
    int             system_count;   /* filled by sc_compute */
    cd_strength_t   strength;       /* filled by sc_compute */
    double          significance;   /* filled by sc_compute */
} sc_entry_t;

/* Result of a stored convergence query */
typedef struct {
    sc_entry_t  entries[SC_MAX_ENTRIES];
    int         count;
} sc_result_t;

/* Get the full list of notable dates (static, no computation).
 * Returns number of entries. out must point to valid sc_result_t. */
int sc_list(sc_result_t *out);

/* Compute convergence data for all stored dates.
 * Calls cd_scan for each entry, filling system_count/strength/significance.
 * out must have been populated by sc_list first.
 * Returns number of entries computed. */
int sc_compute(sc_result_t *out);

/* Find the entry nearest to a given JD.
 * Returns pointer to the nearest entry, or NULL if list is empty.
 * Must call sc_list first. */
const sc_entry_t *sc_nearest(const sc_result_t *result, double jd);

/* Count entries with system_count >= threshold.
 * Must call sc_compute first. */
int sc_count_above(const sc_result_t *result, int min_systems);

/* Get entries filtered by category (e.g., "2026", "historical").
 * Copies matching entries to out, up to max_out.
 * Returns number of matches. */
int sc_filter_category(const sc_result_t *result, const char *category,
                       sc_entry_t *out, int max_out);

#endif /* TIME_STORED_CONVERGENCES_H */
