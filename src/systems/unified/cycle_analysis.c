/* cycle_analysis — pure number theory for cycle relationships across time systems.
 * Algorithms: Euclidean GCD, LCM via GCD with overflow check. */

#include "cycle_analysis.h"

#include <limits.h>
#include <math.h>
#include <string.h>

/* ===== GCD / LCM ===== */

long long cycle_gcd(long long a, long long b)
{
    /* Euclidean algorithm */
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long cycle_lcm(long long a, long long b)
{
    if (a == 0 || b == 0) return 0;
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    long long g = cycle_gcd(a, b);
    long long div = a / g;

    /* Overflow check: div * b must not exceed LLONG_MAX */
    if (div > LLONG_MAX / b) return 0;

    return div * b;
}

long long cycle_gcd_array(const long long *values, int count)
{
    if (count <= 0) return 0;

    long long result = values[0];
    for (int i = 1; i < count; i++) {
        result = cycle_gcd(result, values[i]);
    }
    return result;
}

long long cycle_lcm_array(const long long *values, int count)
{
    if (count <= 0) return 0;

    long long result = values[0];
    for (int i = 1; i < count; i++) {
        result = cycle_lcm(result, values[i]);
        if (result == 0) return 0; /* overflow propagation */
    }
    return result;
}

/* ===== Ratio and resonance ===== */

double cycle_ratio(long long length_a, long long length_b)
{
    if (length_b == 0) return 0.0;
    return (double)length_a / (double)length_b;
}

int cycle_is_harmonic(long long length_a, long long length_b, double tolerance)
{
    if (length_b == 0) return 0;
    double r = (double)length_a / (double)length_b;
    double nearest = round(r);
    if (nearest < 1.0) nearest = 1.0;
    double diff = fabs(r - nearest) / nearest;
    return diff <= tolerance ? 1 : 0;
}

cycle_resonance_t cycle_resonance(long long length_a, long long length_b)
{
    cycle_resonance_t res;
    memset(&res, 0, sizeof(res));

    res.lcm = cycle_lcm(length_a, length_b);
    res.ratio = cycle_ratio(length_a, length_b);

    /* Nearest integer ratio */
    double nearest = round(res.ratio);
    if (nearest < 1.0) nearest = 1.0;
    res.harmonic = (int)nearest;

    /* Near-integer: within 1% of nearest integer */
    double diff = fabs(res.ratio - nearest) / nearest;
    res.near_integer = (diff <= 0.01) ? 1 : 0;

    return res;
}

/* ===== Position and alignment ===== */

long long cycle_position(long long day_count, long long cycle_length)
{
    if (cycle_length <= 0) return 0;
    long long pos = day_count % cycle_length;
    if (pos < 0) pos += cycle_length;
    return pos;
}

long long cycle_next_alignment(long long current_day, long long cycle_a, long long cycle_b)
{
    long long period = cycle_lcm(cycle_a, cycle_b);
    if (period == 0) return 0; /* overflow or invalid */

    long long pos = cycle_position(current_day, period);
    if (pos == 0) return period; /* already aligned, next one is period away */
    return period - pos;
}

int cycle_alignment_count(long long start_day, long long end_day,
                          long long cycle_a, long long cycle_b)
{
    long long period = cycle_lcm(cycle_a, cycle_b);
    if (period == 0) return 0;
    if (start_day > end_day) return 0;

    /* First alignment >= start_day */
    long long pos = cycle_position(start_day, period);
    long long first;
    if (pos == 0) {
        first = start_day;
    } else {
        first = start_day + (period - pos);
    }

    if (first > end_day) return 0;

    /* Count: (last - first) / period + 1, where last <= end_day */
    return (int)((end_day - first) / period) + 1;
}

/* ===== Known cycle lengths ===== */

/* Pre-defined cycle table (static const, no mutable globals) */
static const struct {
    const char *name;
    long long length;
} known_cycles[] = {
    {"Tzolkin",         260},
    {"Haab",            365},
    {"Calendar Round",  18980},
    {"Metonic",         6940},
    {"Saros",           6585},
    {"Hijri 30-year",   10631},
    {"Sexagenary",      21915},
    {"Sabbatical",      2557},
    {"Jubilee",         18263},
    {"Week",            7},
    {"Lunar synodic",   30},
    {"Solar year",      365},
};

static const int KNOWN_CYCLE_COUNT = (int)(sizeof(known_cycles) / sizeof(known_cycles[0]));

long long cycle_known_length(const char *name)
{
    if (!name) return 0;
    for (int i = 0; i < KNOWN_CYCLE_COUNT; i++) {
        if (strcmp(name, known_cycles[i].name) == 0) {
            return known_cycles[i].length;
        }
    }
    return 0;
}
