/* phase_space.c — N-dimensional cycle phase space mapper implementation.
 * Pure functions: no malloc, no globals, no side effects. */

#include "phase_space.h"

#include <math.h>
#include <string.h>

/* Pre-defined known cycles */
static const phase_cycle_t KNOWN_CYCLES[] = {
    {"tzolkin",     260.0,      63.0},      /* GMT correlation: 584283 mod 260 = 63 */
    {"lunar",       29.530589,  0.0},       /* synodic month */
    {"zodiac",      365.24219,  0.0},       /* tropical year */
    {"metonic",     6939.688,   0.0},       /* 19 tropical years */
    {"saros",       6585.321,   0.0},       /* 223 synodic months */
    {"sexagenary",  21914.55,   0.0},       /* 60 * 365.2425 */
    {"hijri",       10631.016,  0.0},       /* 30 * 354.3672 */
    {"sabbatical",  2556.75,    0.0},       /* 7 * 365.25 */
    {"week",        7.0,        0.0}        /* 7-day week */
};

static const int KNOWN_CYCLE_COUNT =
    (int)(sizeof(KNOWN_CYCLES) / sizeof(KNOWN_CYCLES[0]));

double phase_position(double jd, double period_days, double epoch_offset)
{
    double shifted = jd - epoch_offset;
    double mod = fmod(shifted, period_days);
    if (mod < 0.0) {
        mod += period_days;
    }
    return mod / period_days;
}

phase_point_t phase_point_at(double jd, const phase_cycle_t *cycles, int count)
{
    phase_point_t p;
    int n = count;
    if (n > PHASE_MAX_DIMS) {
        n = PHASE_MAX_DIMS;
    }
    if (n < 0) {
        n = 0;
    }
    p.dims = n;
    for (int i = 0; i < n; i++) {
        p.coords[i] = phase_position(jd, cycles[i].period_days,
                                      cycles[i].epoch_offset);
    }
    /* Zero out unused coords */
    for (int i = n; i < PHASE_MAX_DIMS; i++) {
        p.coords[i] = 0.0;
    }
    return p;
}

double phase_toroidal_1d(double a, double b)
{
    double diff = fabs(a - b);
    double wrapped = 1.0 - diff;
    if (wrapped < diff) {
        return wrapped;
    }
    return diff;
}

double phase_euclidean(phase_point_t a, phase_point_t b)
{
    int n = a.dims;
    if (b.dims < n) {
        n = b.dims;
    }
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double d = a.coords[i] - b.coords[i];
        sum += d * d;
    }
    return sqrt(sum);
}

double phase_toroidal(phase_point_t a, phase_point_t b)
{
    int n = a.dims;
    if (b.dims < n) {
        n = b.dims;
    }
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double d = phase_toroidal_1d(a.coords[i], b.coords[i]);
        sum += d * d;
    }
    return sqrt(sum);
}

phase_distance_t phase_distance(phase_point_t a, phase_point_t b)
{
    phase_distance_t result;
    result.euclidean = phase_euclidean(a, b);

    int n = a.dims;
    if (b.dims < n) {
        n = b.dims;
    }

    double toroidal_sum = 0.0;
    double max_comp = 0.0;
    int max_dim = 0;

    for (int i = 0; i < n; i++) {
        double d = phase_toroidal_1d(a.coords[i], b.coords[i]);
        toroidal_sum += d * d;
        if (d > max_comp) {
            max_comp = d;
            max_dim = i;
        }
    }

    result.toroidal = sqrt(toroidal_sum);
    result.max_component = max_comp;
    result.furthest_dim = max_dim;

    return result;
}

/* Internal: search in a direction (forward or backward) for nearest phase match.
 * direction: +1 for forward, -1 for backward. */
static phase_alignment_t phase_search(double start_jd,
                                       phase_point_t target,
                                       const phase_cycle_t *cycles, int count,
                                       double step_days, double max_days,
                                       double tolerance, int direction)
{
    phase_alignment_t best;
    best.jd = start_jd;
    best.distance = 1e30;
    best.is_exact = 0;

    double dir = (direction >= 0) ? 1.0 : -1.0;
    double end_jd = start_jd + dir * max_days;

    /* Coarse search */
    double best_coarse_jd = start_jd;
    double best_coarse_dist = 1e30;

    double jd = start_jd + dir * step_days;
    while ((dir > 0.0 && jd <= end_jd) || (dir < 0.0 && jd >= end_jd)) {
        phase_point_t p = phase_point_at(jd, cycles, count);
        double d = phase_toroidal(p, target);
        if (d < best_coarse_dist) {
            best_coarse_dist = d;
            best_coarse_jd = jd;
        }
        jd += dir * step_days;
    }

    /* Refinement: search around best coarse point with smaller steps */
    double refine_start = best_coarse_jd - step_days;
    double refine_end = best_coarse_jd + step_days;
    double refine_step = step_days / 100.0;
    if (refine_step < 0.001) {
        refine_step = 0.001;
    }

    double rjd = refine_start;
    while (rjd <= refine_end) {
        phase_point_t p = phase_point_at(rjd, cycles, count);
        double d = phase_toroidal(p, target);
        if (d < best.distance) {
            best.distance = d;
            best.jd = rjd;
        }
        rjd += refine_step;
    }

    if (best.distance < tolerance) {
        best.is_exact = 1;
    }

    return best;
}

phase_alignment_t phase_nearest_return(double start_jd,
                                        phase_point_t target,
                                        const phase_cycle_t *cycles, int count,
                                        double step_days, double max_days,
                                        double tolerance)
{
    return phase_search(start_jd, target, cycles, count,
                        step_days, max_days, tolerance, 1);
}

phase_alignment_t phase_nearest_past(double start_jd,
                                      phase_point_t target,
                                      const phase_cycle_t *cycles, int count,
                                      double step_days, double max_days,
                                      double tolerance)
{
    return phase_search(start_jd, target, cycles, count,
                        step_days, max_days, tolerance, -1);
}

phase_cycle_t phase_known_cycle(const char *name)
{
    for (int i = 0; i < KNOWN_CYCLE_COUNT; i++) {
        if (strcmp(name, KNOWN_CYCLES[i].name) == 0) {
            return KNOWN_CYCLES[i];
        }
    }
    phase_cycle_t empty = {name, 0.0, 0.0};
    return empty;
}

int phase_known_cycle_count(void)
{
    return KNOWN_CYCLE_COUNT;
}

phase_cycle_t phase_known_cycle_get(int index)
{
    if (index < 0 || index >= KNOWN_CYCLE_COUNT) {
        phase_cycle_t empty = {"", 0.0, 0.0};
        return empty;
    }
    return KNOWN_CYCLES[index];
}

phase_point_t phase_point_zero(int dims)
{
    phase_point_t p;
    int n = dims;
    if (n > PHASE_MAX_DIMS) {
        n = PHASE_MAX_DIMS;
    }
    if (n < 0) {
        n = 0;
    }
    p.dims = n;
    for (int i = 0; i < PHASE_MAX_DIMS; i++) {
        p.coords[i] = 0.0;
    }
    return p;
}

phase_point_t phase_point_from_array(const double *coords, int dims)
{
    phase_point_t p;
    int n = dims;
    if (n > PHASE_MAX_DIMS) {
        n = PHASE_MAX_DIMS;
    }
    if (n < 0) {
        n = 0;
    }
    p.dims = n;
    for (int i = 0; i < n; i++) {
        p.coords[i] = coords[i];
    }
    for (int i = n; i < PHASE_MAX_DIMS; i++) {
        p.coords[i] = 0.0;
    }
    return p;
}
