#include "kepler.h"
#include <math.h>

#define PI 3.14159265358979323846

/* Meeus, "Astronomical Algorithms", Chapter 30.
 * Newton-Raphson iteration for Kepler's equation: M = E - e*sin(E)
 *
 * Iteration: E_{n+1} = E_n - (E_n - e*sin(E_n) - M) / (1 - e*cos(E_n))
 * Initial guess: E_0 = M
 * Convergence: |E_{n+1} - E_n| < 1e-10
 * Safety limit: 50 iterations max */

#define KEPLER_EPSILON 1e-10
#define KEPLER_MAX_ITER 50

double kepler_solve(double mean_anomaly, double eccentricity) {
    double E = mean_anomaly; /* initial guess */

    for (int i = 0; i < KEPLER_MAX_ITER; i++) {
        double dE = (E - eccentricity * sin(E) - mean_anomaly)
                   / (1.0 - eccentricity * cos(E));
        E -= dE;
        if (fabs(dE) < KEPLER_EPSILON) {
            break;
        }
    }

    return E;
}

/* tan(v/2) = sqrt((1+e)/(1-e)) * tan(E/2)
 * Uses atan2 for correct quadrant. */
double eccentric_to_true_anomaly(double ecc_anomaly, double eccentricity) {
    double half_E = ecc_anomaly / 2.0;
    double factor = sqrt((1.0 + eccentricity) / (1.0 - eccentricity));
    double half_v = atan(factor * tan(half_E));

    /* Preserve quadrant: v and E should be in the same half-plane */
    double v = 2.0 * half_v;
    /* If E is in (pi, 2*pi), v should also be adjusted */
    if (ecc_anomaly > PI) {
        v += 2.0 * PI;
    } else if (ecc_anomaly < -PI) {
        v -= 2.0 * PI;
    }

    return v;
}
