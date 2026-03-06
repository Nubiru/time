/* cycle_analysis — pure number theory for cycle relationships across time systems.
 * Computes GCD, LCM, resonance detection between sacred/astronomical cycles. */

#ifndef TIME_CYCLE_ANALYSIS_H
#define TIME_CYCLE_ANALYSIS_H

#define CYCLE_MAX_SYSTEMS 16

/* Cycle identity */
typedef struct {
    const char *name;       /* cycle name */
    long long length;       /* cycle length in days (or smallest unit) */
    const char *system;     /* originating system name */
} cycle_def_t;

/* Resonance between two cycles */
typedef struct {
    int system_a;           /* index into cycle array */
    int system_b;           /* index into cycle array */
    long long lcm;          /* least common multiple */
    double ratio;           /* length_a / length_b */
    int near_integer;       /* 1 if ratio is within 1% of an integer */
    int harmonic;           /* nearest integer ratio (e.g., 3 if ratio ~3.0) */
} cycle_resonance_t;

/* GCD of two positive integers. */
long long cycle_gcd(long long a, long long b);

/* LCM of two positive integers. Returns 0 if overflow. */
long long cycle_lcm(long long a, long long b);

/* GCD of an array of positive integers. */
long long cycle_gcd_array(const long long *values, int count);

/* LCM of an array of positive integers. Returns 0 if overflow. */
long long cycle_lcm_array(const long long *values, int count);

/* Compute resonance between two cycle lengths. */
cycle_resonance_t cycle_resonance(long long length_a, long long length_b);

/* How many cycles of B fit in one cycle of A? (fractional) */
double cycle_ratio(long long length_a, long long length_b);

/* Check if two cycles are harmonically related (ratio within tolerance of integer). */
int cycle_is_harmonic(long long length_a, long long length_b, double tolerance);

/* Position within a cycle at a given day count.
 * Returns position 0 to (cycle_length-1). Handles negative day_count. */
long long cycle_position(long long day_count, long long cycle_length);

/* Days until next alignment of two cycles from a given day.
 * Both cycles start at day 0. Returns days until both are at position 0. */
long long cycle_next_alignment(long long current_day, long long cycle_a, long long cycle_b);

/* Count alignments of two cycles within a range of days.
 * Both aligned when day % lcm(a,b) == 0. */
int cycle_alignment_count(long long start_day, long long end_day,
                          long long cycle_a, long long cycle_b);

/* Pre-defined sacred/astronomical cycle lengths (in days) */
long long cycle_known_length(const char *name);

#endif /* TIME_CYCLE_ANALYSIS_H */
