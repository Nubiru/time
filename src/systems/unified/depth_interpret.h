/* depth_interpret.h — Unified interpretation dispatcher
 *
 * Single API for obtaining tiered interpretations from any knowledge system.
 * Routes to the appropriate system-specific interpret module and returns
 * a standardized depth_interp_t result with glyph, glance, and detail text.
 *
 * Callers pre-compute calendar values and pass them via di_params_t.
 * This module handles routing and format normalization only.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: hebrew_interpret, islamic_interpret, buddhist_interpret,
 *          hindu_interpret, iching_interpret, kabbalah_interpret. */

#ifndef TIME_DEPTH_INTERPRET_H
#define TIME_DEPTH_INTERPRET_H

#include "today_summary.h"  /* ts_system_t */

/* Standardized tiered interpretation — the universal output format */
typedef struct {
    char glyph[16];             /* T0: symbol/abbreviation */
    char glance[128];           /* T1: one-line summary */
    char detail[512];           /* T3: full narrative paragraph */
    int system_id;              /* which ts_system_t this is for */
    int has_data;               /* 1 if interpretation was produced, 0 if unsupported */
} depth_interp_t;

/* Input parameters for interpretation (pre-computed by caller) */
typedef struct {
    int hebrew_month;           /* 1-13, 0 = not available */
    int hebrew_day;
    int hebrew_year;
    int hebrew_sabbatical;      /* 1-7, 0 = unknown */
    int hebrew_is_leap;

    int islamic_month;          /* 1-12, 0 = not available */
    int islamic_day;
    int islamic_year;

    int buddhist_year;          /* BE year, 0 = not available */
    int buddhist_month;         /* 1-12 */
    int buddhist_uposatha;      /* uposatha_type_t value */
    int buddhist_kalpa_phase;   /* kalpa_phase_t value */

    int hindu_tithi;            /* 1-30, 0 = not available */
    const char *hindu_nakshatra;
    const char *hindu_yoga;

    int iching_king_wen;        /* 1-64, 0 = not available */
    const char *iching_upper_tri;
    const char *iching_lower_tri;

    int kabbalah_sefirah;       /* 0-9, -1 = not available */
    const char *kabbalah_planet;
} di_params_t;

/* Route to appropriate interpret module based on system_id.
 * Extracts relevant fields from params.
 * If params is NULL or required fields are zero/missing, returns has_data=0.
 * Invalid system_id: has_data=0, glyph="?", glance="Unknown system". */
depth_interp_t depth_interpret(int system_id, const di_params_t *params);

/* Return a zeroed-out params struct (all int fields 0, pointers NULL,
 * kabbalah_sefirah = -1). */
di_params_t depth_interpret_default_params(void);

/* Returns 1 if the system has an interpret module, 0 if not.
 * Currently supported: HEBREW, ISLAMIC, BUDDHIST, HINDU, ICHING, KABBALAH. */
int depth_interpret_supported(int system_id);

/* Returns count of supported systems (currently 6). */
int depth_interpret_supported_count(void);

#endif /* TIME_DEPTH_INTERPRET_H */
