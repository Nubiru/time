/* grand_cycle.c — Grand Cycle period calculator implementation.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "grand_cycle.h"

/* Built-in cycle definitions */
static const gc_cycle_t BUILT_IN_CYCLES[] = {
    { "Tzolkin",            260,   1 },
    { "Haab",               365,   1 },
    { "Calendar Round",   18980,   1 },
    { "I Ching (64 hex)",    64,   1 },
    { "Sexagenary Cycle", 21915,   1 },  /* 60 * 365.25 */
    { "Metonic Cycle",     6940,   1 },  /* 19 * 365.25 ~ 6939.75 */
    { "Saros Cycle",       6585,   1 },
    { "Rokuyo",               6,   1 },
    { "French Decade",       10,   1 },
};

#define BUILT_IN_COUNT (int)(sizeof(BUILT_IN_CYCLES) / sizeof(BUILT_IN_CYCLES[0]))

int64_t gc_gcd(int64_t a, int64_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int64_t gc_lcm(int64_t a, int64_t b)
{
    if (a == 0 || b == 0) return 0;
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    /* a / gcd * b to avoid overflow */
    return (a / gc_gcd(a, b)) * b;
}

int64_t gc_lcm_array(const int64_t *periods, int count)
{
    if (!periods || count <= 0) return 0;

    int64_t result = periods[0];
    if (result < 0) result = -result;

    for (int i = 1; i < count; i++) {
        result = gc_lcm(result, periods[i]);
        if (result == 0) return 0;
    }
    return result;
}

int gc_get_cycles(gc_cycle_t *out, int max)
{
    if (!out || max <= 0) return 0;

    int n = (max < BUILT_IN_COUNT) ? max : BUILT_IN_COUNT;
    for (int i = 0; i < n; i++) {
        out[i] = BUILT_IN_CYCLES[i];
    }
    return n;
}

int gc_compute(gc_result_t *out)
{
    if (!out) return 0;
    return gc_compute_subset(BUILT_IN_CYCLES, BUILT_IN_COUNT, out);
}

int gc_compute_subset(const gc_cycle_t *cycles, int count, gc_result_t *out)
{
    if (!out) return 0;
    if (!cycles || count <= 0) {
        out->lcm_days = 0;
        out->lcm_years = 0.0;
        out->cycle_count = 0;
        out->description = "No cycles provided";
        return 0;
    }

    /* Normalize periods: if scale == 100, keep hundredths;
     * otherwise treat as direct day values. For mixed scales,
     * multiply day-scale values by 100 so all are in hundredths,
     * then divide result by 100. */
    int has_scaled = 0;
    for (int i = 0; i < count; i++) {
        if (cycles[i].scale == 100) {
            has_scaled = 1;
            break;
        }
    }

    int64_t periods[GC_MAX_CYCLES];
    int n = (count > GC_MAX_CYCLES) ? GC_MAX_CYCLES : count;

    for (int i = 0; i < n; i++) {
        int64_t p = cycles[i].period_days;
        if (p <= 0) p = 1;  /* guard against zero/negative */
        if (has_scaled && cycles[i].scale == 1) {
            p *= 100;  /* promote to hundredths */
        }
        periods[i] = p;
    }

    int64_t lcm = gc_lcm_array(periods, n);

    /* If we promoted to hundredths, divide back */
    if (has_scaled) {
        lcm /= 100;
    }

    out->lcm_days = lcm;
    out->lcm_years = (double)lcm / 365.25;
    out->cycle_count = n;
    out->description = "Grand Cycle — time until exact recurrence";

    return 1;
}
