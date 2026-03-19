/* brain_types.h — Shared type definitions for the brain intelligence layer.
 *
 * Foundation types for the brain pipeline (Tracks 26-28):
 * correlation discovery, insight generation, narrative composition.
 *
 * All brain modules depend on these types. No GL, no malloc,
 * no globals, no side effects.
 *
 * The brain is a pipeline:
 *   DETECT → ANALYZE → SCORE → NARRATE → PERSONALIZE → OUTPUT */

#ifndef TIME_BRAIN_TYPES_H
#define TIME_BRAIN_TYPES_H

/* ===================================================================
 * Constants
 * =================================================================== */

#define BR_MAX_CORRELATIONS 32
#define BR_MAX_INSIGHTS     16
#define BR_MAX_SYSTEMS       8
#define BR_HEADLINE_MAX    128
#define BR_DETAIL_MAX      256
#define BR_SUMMARY_MAX     512
#define BR_THREAD_MAX       32

/* ===================================================================
 * Correlation type — how two systems relate
 * =================================================================== */

typedef enum {
    BR_CORR_STRUCTURAL = 0, /* exact integer match (64 = 64, 2^6) */
    BR_CORR_HARMONIC,       /* ratio within 0.1% of simple fraction */
    BR_CORR_APPROXIMATE,    /* ratio within 1% of known constant */
    BR_CORR_COINCIDENTAL,   /* no clear mathematical relationship */
    BR_CORR_COUNT
} br_correlation_type_t;

/* ===================================================================
 * Insight type — what kind of finding
 * =================================================================== */

typedef enum {
    BR_INSIGHT_CONVERGENCE = 0, /* multiple systems align */
    BR_INSIGHT_NUMBER,          /* sacred number is active */
    BR_INSIGHT_CYCLE,           /* cycle boundary or milestone */
    BR_INSIGHT_FESTIVAL,        /* cultural/religious festival */
    BR_INSIGHT_ASTRONOMICAL,    /* celestial event */
    BR_INSIGHT_QUIET,           /* nothing notable — ordinary day */
    BR_INSIGHT_COUNT
} br_insight_type_t;

/* ===================================================================
 * Correlation — a discovered relationship between two systems
 * =================================================================== */

typedef struct {
    int                    system_a;        /* cd_system_t or ts_system_t */
    int                    system_b;
    double                 alignment_score; /* 0.0 (none) to 1.0 (perfect) */
    br_correlation_type_t  type;
    double                 confidence;      /* 0.0 to 1.0 */
} br_correlation_t;

/* ===================================================================
 * Insight — a single finding about the current moment
 * =================================================================== */

typedef struct {
    br_insight_type_t  type;
    char               headline[BR_HEADLINE_MAX];
    char               detail[BR_DETAIL_MAX];
    int                systems[BR_MAX_SYSTEMS];
    int                system_count;
    double             score;               /* 0.0 to 1.0 */
} br_insight_t;

/* ===================================================================
 * Result — complete brain scan output for a Julian Day
 * =================================================================== */

typedef struct {
    double            jd;
    br_correlation_t  correlations[BR_MAX_CORRELATIONS];
    int               correlation_count;
    br_insight_t      insights[BR_MAX_INSIGHTS];
    int               insight_count;
    int               top_insight_index;
    double            convergence_strength; /* 0.0 to 1.0 */
} br_result_t;

/* ===================================================================
 * Narrative — daily story composed from brain findings
 * =================================================================== */

typedef struct {
    char    headline[BR_HEADLINE_MAX];
    char    summary[BR_SUMMARY_MAX];
    char    thread_type[BR_THREAD_MAX];
    int     systems_featured[BR_MAX_SYSTEMS];
    int     system_count;
    int     wisdom_id;          /* index into wisdom.h quote database */
    double  confidence;         /* 0.0 (forced) to 1.0 (natural fit) */
} br_narrative_t;

/* ===================================================================
 * Name lookup utilities
 * =================================================================== */

/* Returns name string for a correlation type. "?" if invalid. */
const char *br_correlation_type_name(int type);

/* Returns name string for an insight type. "?" if invalid. */
const char *br_insight_type_name(int type);

/* Number of correlation types (always BR_CORR_COUNT). */
int br_correlation_type_count(void);

/* Number of insight types (always BR_INSIGHT_COUNT). */
int br_insight_type_count(void);

#endif /* TIME_BRAIN_TYPES_H */
