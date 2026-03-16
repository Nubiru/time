/* test_helpers.h — Common test constants and macros for Time project tests
 *
 * Shared Julian Day reference points, tolerances, and convenience macros
 * for use across all test files. */

#ifndef TIME_TEST_HELPERS_H
#define TIME_TEST_HELPERS_H

/* Common Julian Day reference points */
#define JD_J2000        2451545.0     /* 2000-01-01 12:00 TT */
#define JD_UNIX_EPOCH   2440587.5     /* 1970-01-01 00:00 UT */
#define JD_Y2K          2451544.5     /* 2000-01-01 00:00 UT */
#define JD_GREGORIAN_REFORM 2299160.5 /* 1582-10-15 */
#define JD_MOON_LANDING 2440422.5     /* 1969-07-20 */

/* Common tolerances */
#define JD_EPSILON      0.0001        /* ~8.6 seconds */
#define ANGLE_EPSILON   0.01          /* degrees */
#define FLOAT_EPSILON   1e-6

/* Convenience: float comparison with tolerance */
#define ASSERT_JD_NEAR(expected, actual) \
    TEST_ASSERT_FLOAT_WITHIN(JD_EPSILON, (expected), (actual))

#define ASSERT_ANGLE_NEAR(expected, actual) \
    TEST_ASSERT_FLOAT_WITHIN(ANGLE_EPSILON, (expected), (actual))

#endif /* TIME_TEST_HELPERS_H */
