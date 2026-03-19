/* brain_query.h — Compound question engine (Track 26, L3.3)
 *
 * Answers multi-system queries: "When did/will Tzolkin Kin 1 AND
 * Hebrew Rosh Hashanah AND full moon all occur on the same day?"
 *
 * Scans a date range, checking each day against query conditions.
 * Uses convergence_detect to check system activity at each date.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_QUERY_H
#define TIME_BRAIN_QUERY_H

#include "brain_types.h"
#include "convergence_detect.h"

/* Maximum systems in a query */
#define BR_QUERY_MAX_SYSTEMS 6

/* Maximum results per query */
#define BR_QUERY_MAX_RESULTS 16

/* Maximum scan range (days) */
#define BR_QUERY_MAX_RANGE 36525  /* ~100 years */

/* Query operator */
typedef enum {
    BR_QUERY_AND = 0,   /* all specified systems must be active */
    BR_QUERY_OR         /* any specified system may be active */
} br_query_op_t;

/* A compound query */
typedef struct {
    cd_system_t    systems[BR_QUERY_MAX_SYSTEMS];
    int            system_count;
    br_query_op_t  op;
    double         min_significance;  /* per-system minimum (0.0-1.0) */
} br_query_t;

/* A query match */
typedef struct {
    double  jd;
    double  significance;             /* combined significance */
    int     matched_count;            /* how many query systems matched */
    char    headline[BR_HEADLINE_MAX];
} br_query_match_t;

/* Query result */
typedef struct {
    br_query_match_t  matches[BR_QUERY_MAX_RESULTS];
    int               count;
    int               days_scanned;
} br_query_result_t;

/* Search for dates matching a compound query.
 * Scans from start_jd forward (positive range_days) or backward (negative).
 * Returns number of matching dates found. */
int br_query_search(const br_query_t *query, double start_jd,
                    int range_days, br_query_result_t *out);

/* Build a simple AND query for N systems. Convenience constructor. */
br_query_t br_query_and(const cd_system_t *systems, int count);

/* Build a simple OR query for N systems. */
br_query_t br_query_or(const cd_system_t *systems, int count);

#endif /* TIME_BRAIN_QUERY_H */
