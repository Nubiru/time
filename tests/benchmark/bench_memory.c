/* bench_memory.c — Memory budget verification.
 *
 * Runs the same computation workload as bench_core and verifies that
 * total process memory stays within budget. Pure functions should use
 * only stack — no heap growth expected.
 *
 * Measurement: /proc/self/status VmRSS (resident set size) and VmPeak.
 * Heap verification is also done by valgrind in CI (memory job).
 *
 * Build: cmake --preset benchmark && cmake --build build-benchmark
 * Run:   ./build-benchmark/bench_memory
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math/julian.h"
#include "math/kepler.h"
#include "math/mat4.h"
#include "math/vec3.h"
#include "systems/astronomy/planets.h"
#include "systems/unified/calendar_convert.h"

/* --------------------------------------------------------------------------
 * Memory measurement via /proc/self/status
 * -------------------------------------------------------------------------- */

typedef struct {
    long vm_peak_kb;  /* peak virtual memory */
    long vm_rss_kb;   /* current resident set */
    long vm_data_kb;  /* data + stack segment */
} mem_stats_t;

static mem_stats_t get_mem_stats(void) {
    mem_stats_t stats = {0, 0, 0};
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return stats;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "VmPeak:", 7) == 0)
            stats.vm_peak_kb = atol(line + 7);
        else if (strncmp(line, "VmRSS:", 6) == 0)
            stats.vm_rss_kb = atol(line + 6);
        else if (strncmp(line, "VmData:", 7) == 0)
            stats.vm_data_kb = atol(line + 7);
    }
    fclose(f);
    return stats;
}

/* --------------------------------------------------------------------------
 * Workload: exercise all pure computation paths
 * -------------------------------------------------------------------------- */

static void run_workload(void) {
    volatile double sink_d = 0;
    volatile float sink_f = 0;

    /* Julian Day: 1M conversions */
    for (int i = 0; i < 1000000; i++) {
        sink_d = gregorian_to_jd(2000 + (i % 100), 1 + (i % 12), 1.0 + (i % 28));
    }

    /* Kepler: 100K solves */
    for (int i = 0; i < 100000; i++) {
        sink_d = kepler_solve(0.01 + (i % 628) * 0.01, 0.01 + (i % 90) * 0.01);
    }

    /* mat4: 1M multiplies */
    mat4_t a = mat4_perspective(1.0472f, 1.7778f, 0.1f, 1000.0f);
    mat4_t b = mat4_look_at(
        vec3_create(0, 0, 5), vec3_create(0, 0, 0), vec3_create(0, 1, 0));
    for (int i = 0; i < 1000000; i++) {
        mat4_t c = mat4_multiply(a, b);
        sink_f = c.m[0];
        a.m[0] = b.m[0] + (float)i * 1e-10f;
    }

    /* vec3: 1M chains */
    vec3_t va = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t vb = vec3_create(4.0f, 5.0f, 6.0f);
    for (int i = 0; i < 1000000; i++) {
        vec3_t c = vec3_add(va, vb);
        vec3_t d = vec3_cross(va, c);
        sink_f = vec3_dot(d, vb) + vec3_normalize(d).x;
        va.x = c.x * 1e-10f;
    }

    /* Planets: 10K full solar system */
    for (int i = 0; i < 10000; i++) {
        solar_system_t ss = planets_at(2451545.0 + i);
        sink_d = ss.positions[0].longitude;
    }

    /* Calendars: 10K x 21 systems */
    for (int i = 0; i < 10000; i++) {
        for (int s = 0; s < CAL_SYSTEM_COUNT; s++) {
            cal_date_t d = cal_convert((cal_system_t)s, 2451545.0 + i);
            sink_d = d.year;
        }
    }

    (void)sink_d;
    (void)sink_f;
}

/* --------------------------------------------------------------------------
 * Main — measure before/after workload, verify budget
 * -------------------------------------------------------------------------- */

/* Total RSS budget for the entire benchmark process.
 * This includes the executable, shared libs, stack, and data.
 * Pure math functions should NOT grow this. */
#define RSS_BUDGET_KB 8192  /* 8 MB — generous for a pure-math workload */

int main(void) {
    printf("=== Time Project — Memory Budget Tests ===\n\n");

    mem_stats_t before = get_mem_stats();
    printf("Before workload:\n");
    printf("  VmRSS:  %ld KB\n", before.vm_rss_kb);
    printf("  VmData: %ld KB\n", before.vm_data_kb);

    run_workload();

    mem_stats_t after = get_mem_stats();
    printf("\nAfter workload (1M julian + 100K kepler + 1M mat4 + 1M vec3 + 10K planets + 210K calendars):\n");
    printf("  VmPeak: %ld KB\n", after.vm_peak_kb);
    printf("  VmRSS:  %ld KB (delta: %+ld KB)\n",
           after.vm_rss_kb, after.vm_rss_kb - before.vm_rss_kb);
    printf("  VmData: %ld KB (delta: %+ld KB)\n",
           after.vm_data_kb, after.vm_data_kb - before.vm_data_kb);

    /* Budget check: total peak RSS */
    printf("\nBudget: VmPeak <= %d KB\n", RSS_BUDGET_KB);

    int pass = 1;

    if (after.vm_peak_kb > RSS_BUDGET_KB) {
        printf("FAIL: VmPeak %ld KB exceeds budget %d KB\n",
               after.vm_peak_kb, RSS_BUDGET_KB);
        pass = 0;
    }

    /* Data segment growth check: pure functions shouldn't grow heap.
     * Allow 256 KB for demand paging / lazy initialization. */
    long data_delta = after.vm_data_kb - before.vm_data_kb;
    if (data_delta > 256) {
        printf("FAIL: VmData grew by %ld KB (budget: 256 KB) — possible heap allocation\n",
               data_delta);
        pass = 0;
    }

    if (pass) {
        printf("PASS: memory within budget (peak: %ld KB, data delta: %+ld KB)\n",
               after.vm_peak_kb, data_delta);
        return 0;
    }
    return 1;
}
