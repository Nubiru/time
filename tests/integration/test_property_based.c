/*
 * test_property_based.c — Property-based tests with large input sets
 *
 * Uses a deterministic LCG PRNG for reproducible "random" inputs.
 * Tests mathematical properties (range, idempotence, symmetry, identity)
 * across thousands of values.
 *
 * Pure function tests only. No GL calls.
 */

#include "unity.h"
#include "math/easing.h"
#include "math/vec3.h"
#include "math/mat4.h"
#include <math.h>

#define PI 3.14159265358979323846

void setUp(void) { }
void tearDown(void) { }

/* ===== Deterministic PRNG (Linear Congruential Generator) ===== */

static unsigned int prng_state = 42;

static void prng_seed(unsigned int seed) {
    prng_state = seed;
}

/* Returns value in [0, 32767] */
static int prng_next(void) {
    prng_state = prng_state * 1103515245u + 12345u;
    return (int)((prng_state >> 16) & 0x7FFF);
}

/* Returns double in [0.0, 1.0) */
static double prng_double(void) {
    return (double)prng_next() / 32768.0;
}

/* Returns double in [lo, hi) */
static double prng_range(double lo, double hi) {
    return lo + prng_double() * (hi - lo);
}

/* ===== Property 1: angle_normalize range ===== */
/* For 10,000 values in [-1e6, 1e6], result always in [0, 360) */

void test_angle_normalize_range(void) {
    prng_seed(1001);
    int failures = 0;
    for (int i = 0; i < 10000; i++) {
        double deg = prng_range(-1e6, 1e6);
        double norm = angle_normalize(deg);
        if (norm < 0.0 || norm >= 360.0) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "angle_normalize produced value outside [0, 360)");
}

/* Edge cases for angle_normalize */
void test_angle_normalize_edge_cases(void) {
    /* Zero */
    TEST_ASSERT_TRUE(fabs(angle_normalize(0.0) - 0.0) < 1e-9);

    /* Exactly 360 should wrap to ~0 */
    TEST_ASSERT_TRUE(fabs(angle_normalize(360.0) - 0.0) < 1e-9);

    /* Negative */
    TEST_ASSERT_TRUE(fabs(angle_normalize(-90.0) - 270.0) < 1e-9);

    /* Large positive */
    TEST_ASSERT_TRUE(fabs(angle_normalize(720.0) - 0.0) < 1e-9);

    /* Large negative */
    TEST_ASSERT_TRUE(fabs(angle_normalize(-720.0) - 0.0) < 1e-9);
}

/* ===== Property 2: angle_normalize idempotent ===== */
/* normalize(normalize(x)) == normalize(x) for 1000 values */

void test_angle_normalize_idempotent(void) {
    prng_seed(2002);
    int failures = 0;
    for (int i = 0; i < 1000; i++) {
        double deg = prng_range(-1e6, 1e6);
        double once = angle_normalize(deg);
        double twice = angle_normalize(once);
        if (fabs(once - twice) > 1e-9) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "angle_normalize not idempotent");
}

/* ===== Property 3: angle_distance anti-symmetric ===== */
/* angle_distance returns signed distance in (-180, 180].
 * dist(a,b) == -dist(b,a) for 1000 pairs; result always in (-180, 180]. */

void test_angle_distance_antisymmetric(void) {
    prng_seed(3003);
    int antisym_failures = 0;
    int range_failures = 0;
    for (int i = 0; i < 1000; i++) {
        double a = prng_range(0.0, 720.0);
        double b = prng_range(0.0, 720.0);
        double d_ab = angle_distance(a, b);
        double d_ba = angle_distance(b, a);
        /* Anti-symmetric: d(a,b) = -d(b,a) */
        if (fabs(d_ab + d_ba) > 1e-9) {
            antisym_failures++;
        }
        /* Range: result in [-180, 180] */
        if (d_ab < -180.0 - 1e-9 || d_ab > 180.0 + 1e-9) {
            range_failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, antisym_failures,
        "angle_distance not anti-symmetric");
    TEST_ASSERT_EQUAL_MESSAGE(0, range_failures,
        "angle_distance outside [-180, 180]");
}

/* angle_distance specific values (signed, shortest path) */
void test_angle_distance_known_values(void) {
    TEST_ASSERT_TRUE(fabs(angle_distance(0.0, 0.0) - 0.0) < 1e-9);
    TEST_ASSERT_TRUE(fabs(angle_distance(0.0, 90.0) - 90.0) < 1e-9);
    TEST_ASSERT_TRUE(fabs(angle_distance(0.0, 180.0) - 180.0) < 1e-9);
    /* 350 -> 80: shortest path is +90 (forward through 0) */
    TEST_ASSERT_TRUE(fabs(angle_distance(350.0, 80.0) - 90.0) < 1e-9);
    /* 355 -> 5: shortest path is +10 (forward through 0) */
    TEST_ASSERT_TRUE(fabs(angle_distance(355.0, 5.0) - 10.0) < 1e-9);
    /* Reverse: 80 -> 350 should be -90 */
    TEST_ASSERT_TRUE(fabs(angle_distance(80.0, 350.0) - (-90.0)) < 1e-9);
}

/* ===== Property 4: vec3_normalize unit length ===== */
/* For 1000 random non-zero vectors, |normalize(v)| approx 1.0 */

void test_vec3_normalize_unit_length(void) {
    prng_seed(4004);
    int failures = 0;
    for (int i = 0; i < 1000; i++) {
        float x = (float)prng_range(-100.0, 100.0);
        float y = (float)prng_range(-100.0, 100.0);
        float z = (float)prng_range(-100.0, 100.0);

        /* Skip near-zero vectors */
        vec3_t v = vec3_create(x, y, z);
        float len = vec3_length(v);
        if (len < 1e-6f) continue;

        vec3_t n = vec3_normalize(v);
        float nlen = vec3_length(n);
        if (fabsf(nlen - 1.0f) > 1e-5f) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "vec3_normalize did not produce unit length");
}

/* ===== Property 5: vec3_normalize idempotent ===== */
/* normalize(normalize(v)) approx normalize(v) for 1000 vectors */

void test_vec3_normalize_idempotent(void) {
    prng_seed(5005);
    int failures = 0;
    for (int i = 0; i < 1000; i++) {
        float x = (float)prng_range(-100.0, 100.0);
        float y = (float)prng_range(-100.0, 100.0);
        float z = (float)prng_range(-100.0, 100.0);

        vec3_t v = vec3_create(x, y, z);
        if (vec3_length(v) < 1e-6f) continue;

        vec3_t once = vec3_normalize(v);
        vec3_t twice = vec3_normalize(once);
        float dx = fabsf(once.x - twice.x);
        float dy = fabsf(once.y - twice.y);
        float dz = fabsf(once.z - twice.z);
        if (dx > 1e-5f || dy > 1e-5f || dz > 1e-5f) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "vec3_normalize not idempotent");
}

/* ===== vec3 additional property tests ===== */

/* cross product is anti-commutative: a x b = -(b x a) */
void test_vec3_cross_anticommutative(void) {
    prng_seed(5050);
    int failures = 0;
    for (int i = 0; i < 500; i++) {
        vec3_t a = vec3_create(
            (float)prng_range(-50.0, 50.0),
            (float)prng_range(-50.0, 50.0),
            (float)prng_range(-50.0, 50.0));
        vec3_t b = vec3_create(
            (float)prng_range(-50.0, 50.0),
            (float)prng_range(-50.0, 50.0),
            (float)prng_range(-50.0, 50.0));

        vec3_t ab = vec3_cross(a, b);
        vec3_t ba = vec3_cross(b, a);
        if (fabsf(ab.x + ba.x) > 1e-3f ||
            fabsf(ab.y + ba.y) > 1e-3f ||
            fabsf(ab.z + ba.z) > 1e-3f) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "vec3_cross not anti-commutative");
}

/* dot product is commutative: a . b = b . a */
void test_vec3_dot_commutative(void) {
    prng_seed(5060);
    int failures = 0;
    for (int i = 0; i < 500; i++) {
        vec3_t a = vec3_create(
            (float)prng_range(-50.0, 50.0),
            (float)prng_range(-50.0, 50.0),
            (float)prng_range(-50.0, 50.0));
        vec3_t b = vec3_create(
            (float)prng_range(-50.0, 50.0),
            (float)prng_range(-50.0, 50.0),
            (float)prng_range(-50.0, 50.0));

        float d_ab = vec3_dot(a, b);
        float d_ba = vec3_dot(b, a);
        if (fabsf(d_ab - d_ba) > 1e-3f) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "vec3_dot not commutative");
}

/* ===== Property 6: mat4 identity and rotation properties ===== */
/* mat4_inverse does not exist in this codebase. Instead, test:
 * a) I * M = M (identity is multiplicative identity)
 * b) Rotation matrices are orthogonal: R_x(a) * R_x(-a) approx I */

void test_mat4_identity_multiply(void) {
    prng_seed(6006);
    mat4_t I = mat4_identity();
    int failures = 0;

    for (int i = 0; i < 20; i++) {
        /* Create a random transformation matrix via composition */
        float tx = (float)prng_range(-100.0, 100.0);
        float ty = (float)prng_range(-100.0, 100.0);
        float tz = (float)prng_range(-100.0, 100.0);
        mat4_t M = mat4_translate(tx, ty, tz);

        /* I * M should equal M */
        mat4_t result = mat4_multiply(I, M);
        for (int j = 0; j < 16; j++) {
            if (fabsf(result.m[j] - M.m[j]) > 1e-6f) {
                failures++;
                break;
            }
        }

        /* M * I should equal M */
        result = mat4_multiply(M, I);
        for (int j = 0; j < 16; j++) {
            if (fabsf(result.m[j] - M.m[j]) > 1e-6f) {
                failures++;
                break;
            }
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "mat4 identity multiply property failed");
}

void test_mat4_rotation_inverse(void) {
    /* For rotation matrices, R(angle) * R(-angle) = I */
    prng_seed(6007);
    mat4_t I = mat4_identity();
    int failures = 0;

    for (int i = 0; i < 20; i++) {
        float angle = (float)prng_range(-PI, PI);

        /* Test X rotation */
        mat4_t rx_pos = mat4_rotate_x(angle);
        mat4_t rx_neg = mat4_rotate_x(-angle);
        mat4_t rx_prod = mat4_multiply(rx_pos, rx_neg);
        for (int j = 0; j < 16; j++) {
            if (fabsf(rx_prod.m[j] - I.m[j]) > 1e-5f) {
                failures++;
                break;
            }
        }

        /* Test Y rotation */
        mat4_t ry_pos = mat4_rotate_y(angle);
        mat4_t ry_neg = mat4_rotate_y(-angle);
        mat4_t ry_prod = mat4_multiply(ry_pos, ry_neg);
        for (int j = 0; j < 16; j++) {
            if (fabsf(ry_prod.m[j] - I.m[j]) > 1e-5f) {
                failures++;
                break;
            }
        }

        /* Test Z rotation */
        mat4_t rz_pos = mat4_rotate_z(angle);
        mat4_t rz_neg = mat4_rotate_z(-angle);
        mat4_t rz_prod = mat4_multiply(rz_pos, rz_neg);
        for (int j = 0; j < 16; j++) {
            if (fabsf(rz_prod.m[j] - I.m[j]) > 1e-5f) {
                failures++;
                break;
            }
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "mat4 rotation inverse property failed");
}

/* mat4_multiply is associative: (A * B) * C = A * (B * C) */
void test_mat4_multiply_associative(void) {
    prng_seed(6008);
    int failures = 0;

    for (int i = 0; i < 20; i++) {
        float a1 = (float)prng_range(-PI, PI);
        float a2 = (float)prng_range(-PI, PI);
        float a3 = (float)prng_range(-PI, PI);

        mat4_t A = mat4_rotate_x(a1);
        mat4_t B = mat4_rotate_y(a2);
        mat4_t C = mat4_rotate_z(a3);

        mat4_t ab_c = mat4_multiply(mat4_multiply(A, B), C);
        mat4_t a_bc = mat4_multiply(A, mat4_multiply(B, C));

        for (int j = 0; j < 16; j++) {
            if (fabsf(ab_c.m[j] - a_bc.m[j]) > 1e-4f) {
                failures++;
                break;
            }
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "mat4 multiply not associative");
}

/* ===== Easing function range properties ===== */

/* All easing functions should map [0,1] -> [0,1] endpoints:
 * f(0) = 0 and f(1) = 1 (for standard easing functions) */
void test_easing_endpoints(void) {
    ease_fn_t fns[] = {
        ease_linear,
        ease_in_quad, ease_out_quad, ease_in_out_quad,
        ease_in_cubic, ease_out_cubic, ease_in_out_cubic,
        ease_in_sine, ease_out_sine, ease_in_out_sine,
        ease_in_expo, ease_out_expo
    };
    int count = (int)(sizeof(fns) / sizeof(fns[0]));

    for (int i = 0; i < count; i++) {
        TEST_ASSERT_TRUE(fabs(fns[i](0.0) - 0.0) < 1e-6);
        TEST_ASSERT_TRUE(fabs(fns[i](1.0) - 1.0) < 1e-6);
    }
}

/* Linear easing should be identity: f(t) = t */
void test_easing_linear_identity(void) {
    prng_seed(7007);
    int failures = 0;
    for (int i = 0; i < 1000; i++) {
        double t = prng_double();
        double result = ease_linear(t);
        if (fabs(result - t) > 1e-12) {
            failures++;
        }
    }
    TEST_ASSERT_EQUAL_MESSAGE(0, failures,
        "ease_linear is not identity");
}

/* Easing functions should be monotonically non-decreasing on [0,1]
 * for the standard (non-elastic, non-back) functions */
void test_easing_monotonic(void) {
    ease_fn_t fns[] = {
        ease_linear,
        ease_in_quad, ease_out_quad, ease_in_out_quad,
        ease_in_cubic, ease_out_cubic, ease_in_out_cubic,
        ease_in_sine, ease_out_sine, ease_in_out_sine
    };
    int count = (int)(sizeof(fns) / sizeof(fns[0]));

    for (int f = 0; f < count; f++) {
        double prev = fns[f](0.0);
        int failures = 0;
        for (int i = 1; i <= 1000; i++) {
            double t = (double)i / 1000.0;
            double cur = fns[f](t);
            if (cur < prev - 1e-12) {
                failures++;
            }
            prev = cur;
        }
        TEST_ASSERT_EQUAL_MESSAGE(0, failures,
            "easing function not monotonic on [0,1]");
    }
}

int main(void) {
    UNITY_BEGIN();

    /* angle_normalize properties */
    RUN_TEST(test_angle_normalize_range);
    RUN_TEST(test_angle_normalize_edge_cases);
    RUN_TEST(test_angle_normalize_idempotent);

    /* angle_distance properties */
    RUN_TEST(test_angle_distance_antisymmetric);
    RUN_TEST(test_angle_distance_known_values);

    /* vec3_normalize properties */
    RUN_TEST(test_vec3_normalize_unit_length);
    RUN_TEST(test_vec3_normalize_idempotent);

    /* vec3 additional properties */
    RUN_TEST(test_vec3_cross_anticommutative);
    RUN_TEST(test_vec3_dot_commutative);

    /* mat4 properties */
    RUN_TEST(test_mat4_identity_multiply);
    RUN_TEST(test_mat4_rotation_inverse);
    RUN_TEST(test_mat4_multiply_associative);

    /* easing properties */
    RUN_TEST(test_easing_endpoints);
    RUN_TEST(test_easing_linear_identity);
    RUN_TEST(test_easing_monotonic);

    return UNITY_END();
}
