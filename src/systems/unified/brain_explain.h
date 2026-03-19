/* brain_explain.h — Explanation engine for cross-system correlations (Track 26, L1.1)
 *
 * Provides human-readable explanations for WHY two knowledge systems
 * correlate. Three explanation categories:
 *   SHARED_SOURCE  — both systems observe the same natural phenomenon
 *   TRANSMISSION   — one system influenced the other historically
 *   CONVERGENT     — independent traditions arrived at similar structures
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_EXPLAIN_H
#define TIME_BRAIN_EXPLAIN_H

#include "brain_types.h"

/* Maximum explanation text length */
#define BR_EXPLAIN_MAX 256

/* Why two systems correlate */
typedef enum {
    BR_EXPLAIN_SHARED_SOURCE = 0, /* same natural phenomenon observed */
    BR_EXPLAIN_TRANSMISSION,      /* historical knowledge transfer */
    BR_EXPLAIN_CONVERGENT,        /* independent parallel development */
    BR_EXPLAIN_UNKNOWN,           /* no known relationship */
    BR_EXPLAIN_TYPE_COUNT
} br_explain_type_t;

/* Full explanation entry */
typedef struct {
    int                system_a;
    int                system_b;
    br_explain_type_t  why;
    const char        *brief;     /* 1-line summary */
    const char        *detail;    /* fuller explanation */
} br_explanation_t;

/* Write explanation for a system pair into buf.
 * Returns number of characters written (excluding null terminator).
 * If pair is unknown, writes a generic fallback explanation.
 * Order-independent: (a,b) and (b,a) produce the same explanation. */
int br_explain(int system_a, int system_b, char *buf, int buf_size);

/* Look up full explanation entry for a system pair.
 * Returns pointer to static data, or NULL if pair has no entry. */
const br_explanation_t *br_explain_lookup(int system_a, int system_b);

/* Get explanation type name string. "?" for invalid. */
const char *br_explain_type_name(br_explain_type_t type);

/* Total number of explanation entries in the database. */
int br_explain_count(void);

#endif /* TIME_BRAIN_EXPLAIN_H */
