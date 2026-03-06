#ifndef TIME_KEPLER_H
#define TIME_KEPLER_H

/* Solve Kepler's equation: M = E - e*sin(E)
 * Given mean anomaly M (radians) and eccentricity e,
 * returns eccentric anomaly E (radians).
 * Uses Newton-Raphson iteration. */
double kepler_solve(double mean_anomaly, double eccentricity);

/* Convert eccentric anomaly E to true anomaly v (radians). */
double eccentric_to_true_anomaly(double ecc_anomaly, double eccentricity);

#endif
