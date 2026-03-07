/* phase_space — N-dimensional cycle phase space mapper.
 * Represents the current moment as a point in N-dimensional space where
 * each dimension is a time system's position in its cycle (0.0-1.0).
 * Computes toroidal/euclidean distance, finds nearest return to a
 * target configuration across time. Pure functions only. */

#ifndef TIME_PHASE_SPACE_H
#define TIME_PHASE_SPACE_H

#define PI 3.14159265358979323846

/* Maximum dimensions (time systems) */
#define PHASE_MAX_DIMS 16

/* A cycle definition for phase space */
typedef struct {
    const char *name;      /* "Tzolkin", "Lunar", "Zodiac", etc. */
    double period_days;    /* cycle length in days */
    double epoch_offset;   /* offset from JD epoch (days into first cycle at JD 0) */
} phase_cycle_t;

/* A point in phase space (N-dimensional, each coord 0.0-1.0) */
typedef struct {
    double coords[PHASE_MAX_DIMS];
    int dims;              /* number of active dimensions */
} phase_point_t;

/* Distance result between two phase points */
typedef struct {
    double euclidean;      /* straight-line distance */
    double toroidal;       /* distance on torus (wrapping at 0/1 boundaries) */
    double max_component;  /* largest single-dimension distance */
    int furthest_dim;      /* dimension with largest distance */
} phase_distance_t;

/* Alignment result */
typedef struct {
    double jd;             /* Julian Date of alignment */
    double distance;       /* toroidal distance from target at that time */
    int is_exact;          /* 1 if distance < tolerance */
} phase_alignment_t;

/* Compute phase position for a single cycle at a given JD.
 * Returns value in [0.0, 1.0). */
double phase_position(double jd, double period_days, double epoch_offset);

/* Compute full phase point for multiple cycles at a given JD.
 * cycles: array of cycle definitions.
 * count: number of cycles (1 to PHASE_MAX_DIMS).
 * Returns phase point. */
phase_point_t phase_point_at(double jd, const phase_cycle_t *cycles, int count);

/* Distance between two phase points. */
phase_distance_t phase_distance(phase_point_t a, phase_point_t b);

/* Toroidal distance for a single dimension (shortest path on circle). */
double phase_toroidal_1d(double a, double b);

/* Euclidean distance (non-wrapping). */
double phase_euclidean(phase_point_t a, phase_point_t b);

/* Toroidal distance (wrapping at 0/1 boundaries). */
double phase_toroidal(phase_point_t a, phase_point_t b);

/* Search forward from start_jd for nearest return to target phase.
 * step_days: search step size.
 * max_days: maximum search range.
 * tolerance: distance threshold for "match".
 * Returns best alignment found. */
phase_alignment_t phase_nearest_return(double start_jd,
                                        phase_point_t target,
                                        const phase_cycle_t *cycles, int count,
                                        double step_days, double max_days,
                                        double tolerance);

/* Search backward for nearest past occurrence. */
phase_alignment_t phase_nearest_past(double start_jd,
                                      phase_point_t target,
                                      const phase_cycle_t *cycles, int count,
                                      double step_days, double max_days,
                                      double tolerance);

/* Get pre-defined cycle by name.
 * Known cycles: "tzolkin", "lunar", "zodiac", "metonic", "saros",
 *               "sexagenary", "hijri", "sabbatical", "week".
 * Returns cycle with period_days=0 if not found. */
phase_cycle_t phase_known_cycle(const char *name);

/* Count of pre-defined cycles. */
int phase_known_cycle_count(void);

/* Get pre-defined cycle by index. */
phase_cycle_t phase_known_cycle_get(int index);

/* Create a zero phase point (all coords = 0.0). */
phase_point_t phase_point_zero(int dims);

/* Create a phase point from an array of coordinates. */
phase_point_t phase_point_from_array(const double *coords, int dims);

#endif /* TIME_PHASE_SPACE_H */
