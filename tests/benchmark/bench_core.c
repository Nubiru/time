/* bench_core.c — Performance benchmark for core computation paths.
 *
 * Measures wall-clock time for key operations that run every frame
 * or every user interaction. Results printed as structured output
 * for CI parsing.
 *
 * Build: cmake --preset benchmark && cmake --build build-benchmark
 * Run:   ./build-benchmark/bench_core
 *
 * Output format (one line per benchmark):
 *   BENCH <name> <iterations> <total_us> <per_iter_ns>
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>

#include "math/julian.h"
#include "math/kepler.h"
#include "math/mat4.h"
#include "math/vec3.h"
#include "systems/astronomy/orbit.h"
#include "systems/astronomy/planets.h"
#include "systems/unified/calendar_convert.h"

/* --------------------------------------------------------------------------
 * Timing infrastructure
 * -------------------------------------------------------------------------- */

static double time_now_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1e6 + (double)ts.tv_nsec / 1e3;
}

typedef struct {
    const char *name;
    int iterations;
    double total_us;
    double per_iter_ns;
} bench_result_t;

static void bench_report(bench_result_t r) {
    printf("BENCH %-30s %8d iters  %10.1f us  %8.1f ns/iter\n",
           r.name, r.iterations, r.total_us, r.per_iter_ns);
}

/* --------------------------------------------------------------------------
 * Benchmarks
 * -------------------------------------------------------------------------- */

static bench_result_t bench_julian_day(void) {
    const int N = 1000000;
    double start = time_now_us();

    volatile double jd = 0;
    for (int i = 0; i < N; i++) {
        jd = gregorian_to_jd(2000 + (i % 100), 1 + (i % 12), 1.0 + (i % 28));
    }
    (void)jd;

    double elapsed = time_now_us() - start;
    return (bench_result_t){
        .name = "julian_day",
        .iterations = N,
        .total_us = elapsed,
        .per_iter_ns = (elapsed * 1000.0) / N
    };
}

static bench_result_t bench_kepler_solve(void) {
    const int N = 100000;
    double start = time_now_us();

    volatile double E = 0;
    for (int i = 0; i < N; i++) {
        double M = 0.01 + (i % 628) * 0.01; /* 0..2pi range */
        double e = 0.01 + (i % 90) * 0.01;  /* 0.01..0.90 eccentricity */
        E = kepler_solve(M, e);
    }
    (void)E;

    double elapsed = time_now_us() - start;
    return (bench_result_t){
        .name = "kepler_solve",
        .iterations = N,
        .total_us = elapsed,
        .per_iter_ns = (elapsed * 1000.0) / N
    };
}

static bench_result_t bench_mat4_multiply(void) {
    const int N = 1000000;
    double start = time_now_us();

    mat4_t a = mat4_perspective(1.0472f, 1.7778f, 0.1f, 1000.0f);
    mat4_t b = mat4_look_at(
        vec3_create(0, 0, 5), vec3_create(0, 0, 0), vec3_create(0, 1, 0));

    volatile float sink = 0;
    for (int i = 0; i < N; i++) {
        mat4_t c = mat4_multiply(a, b);
        sink = c.m[0];
        /* Rotate to prevent dead-code elimination */
        a.m[0] = b.m[0] + (float)i * 1e-10f;
    }
    (void)sink;

    double elapsed = time_now_us() - start;
    return (bench_result_t){
        .name = "mat4_multiply",
        .iterations = N,
        .total_us = elapsed,
        .per_iter_ns = (elapsed * 1000.0) / N
    };
}

static bench_result_t bench_planet_positions(void) {
    const int N = 10000;
    double start = time_now_us();

    volatile double sink = 0;
    for (int i = 0; i < N; i++) {
        double jd = 2451545.0 + i; /* J2000.0 + i days */
        solar_system_t ss = planets_at(jd);
        sink = ss.positions[0].longitude;
    }
    (void)sink;

    double elapsed = time_now_us() - start;
    return (bench_result_t){
        .name = "planets_at_8x",
        .iterations = N,
        .total_us = elapsed,
        .per_iter_ns = (elapsed * 1000.0) / N
    };
}

static bench_result_t bench_calendar_convert(void) {
    const int N = 10000;
    double start = time_now_us();

    volatile int sink = 0;
    for (int i = 0; i < N; i++) {
        double jd = 2451545.0 + i;
        /* Convert through all 21 calendar systems */
        for (int s = 0; s < CAL_SYSTEM_COUNT; s++) {
            cal_date_t d = cal_convert((cal_system_t)s, jd);
            sink = d.year;
        }
    }
    (void)sink;

    double elapsed = time_now_us() - start;
    return (bench_result_t){
        .name = "calendar_convert_21x",
        .iterations = N,
        .total_us = elapsed,
        .per_iter_ns = (elapsed * 1000.0) / N
    };
}

static bench_result_t bench_vec3_operations(void) {
    const int N = 1000000;
    double start = time_now_us();

    vec3_t a = vec3_create(1.0f, 2.0f, 3.0f);
    vec3_t b = vec3_create(4.0f, 5.0f, 6.0f);
    volatile float sink = 0;

    for (int i = 0; i < N; i++) {
        vec3_t c = vec3_add(a, b);
        vec3_t d = vec3_cross(a, c);
        float dot = vec3_dot(d, b);
        vec3_t n = vec3_normalize(d);
        sink = dot + n.x;
        a.x = c.x * 1e-10f;
    }
    (void)sink;

    double elapsed = time_now_us() - start;
    return (bench_result_t){
        .name = "vec3_ops_chain",
        .iterations = N,
        .total_us = elapsed,
        .per_iter_ns = (elapsed * 1000.0) / N
    };
}

/* --------------------------------------------------------------------------
 * Main
 * -------------------------------------------------------------------------- */

int main(void) {
    printf("=== Time Project — Performance Benchmarks ===\n");
    printf("%-30s %10s  %12s  %10s\n",
           "Benchmark", "Iters", "Total (us)", "Per-iter (ns)");
    printf("----------------------------------------------------------------------\n");

    bench_report(bench_julian_day());
    bench_report(bench_kepler_solve());
    bench_report(bench_mat4_multiply());
    bench_report(bench_vec3_operations());
    bench_report(bench_planet_positions());
    bench_report(bench_calendar_convert());

    printf("----------------------------------------------------------------------\n");
    printf("Done.\n");
    return 0;
}
