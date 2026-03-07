/* number_scanner — cross-system number scanner.
 * Pure lookup module: scans a target number against registered time cycles
 * and sacred numbers. Finds factor, multiple, and scale relationships.
 * No malloc, no globals, no side effects. */

#ifndef TIME_NUMBER_SCANNER_H
#define TIME_NUMBER_SCANNER_H

/* Maximum matches per scan */
#define NS_MAX_MATCHES 32

/* Relationship types */
#define NS_REL_FACTOR   0  /* target divides cycle evenly */
#define NS_REL_MULTIPLE 1  /* cycle divides target evenly */
#define NS_REL_SCALE    2  /* target * 10^n approximates cycle */

/* A single match: where a number was found */
typedef struct {
    const char *system_name;    /* "Tzolkin", "Precession", etc. */
    const char *cycle_name;     /* "Calendar Round", "Great Year", etc. */
    double cycle_length;        /* Cycle length */
    int relationship;           /* NS_REL_FACTOR, NS_REL_MULTIPLE, NS_REL_SCALE */
    double ratio;               /* Exact ratio */
    char description[128];      /* Human-readable (embedded, no static buffers) */
} ns_match_t;

/* Scan result */
typedef struct {
    ns_match_t matches[NS_MAX_MATCHES];
    int match_count;
    int scale_appearances;      /* How many scales (x1, x10, x100, ...) */
    int coincidence_score;      /* Number of independent systems */
} ns_scan_result_t;

/* A registered cycle */
typedef struct {
    const char *system;
    const char *name;
    double length;
    const char *unit;
} ns_cycle_t;

/* Sacred number entry */
typedef struct {
    int number;
    const char *name;
    const char *description;
} ns_sacred_t;

/* Scan a target number against all registered cycles. */
ns_scan_result_t number_scan(int target);

/* Which sacred numbers divide this cycle length? Fills out_factors. Returns count. */
int factor_scan(long cycle_length, int *out_factors, int out_max);

/* How many independent systems reference this number? */
int coincidence_score(int number);

/* How many cycles are registered? */
int ns_cycle_count(void);

/* Get cycle by index. Returns entry with system=NULL if invalid. */
ns_cycle_t ns_cycle_get(int index);

/* How many sacred numbers registered? */
int ns_sacred_count(void);

/* Get sacred number by index. Returns entry with number=-1 if invalid. */
ns_sacred_t ns_sacred_get(int index);

/* Is this number a factor of any registered cycle? */
int ns_is_factor_of_cycle(int number);

/* Is this number a multiple of any sacred number? */
int ns_is_sacred_multiple(int number);

/* Check if number appears at scale (x10^n matches a cycle within 1%). Returns count. */
int ns_scale_check(int number);

#endif /* TIME_NUMBER_SCANNER_H */
