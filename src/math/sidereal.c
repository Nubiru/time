#include "sidereal.h"
#include <math.h>

/* Meeus, "Astronomical Algorithms", Chapter 12.
 * All formulas output degrees. */

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)

static double normalize_degrees(double deg) {
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

/* IAU 1982 formula for GMST at 0h UT (degrees).
 * Meeus Eq. 12.4 (degree form):
 * theta = 100.46061837 + 36000.770053608*T + 0.000387933*T^2 - T^3/38710000
 * where T = (JD - 2451545.0) / 36525 */
double gmst_degrees(double jd_0h_ut) {
    double T = (jd_0h_ut - 2451545.0) / 36525.0;
    double theta = 100.46061837
                 + 36000.770053608 * T
                 + 0.000387933 * T * T
                 - T * T * T / 38710000.0;
    return normalize_degrees(theta);
}

/* General GMST for any instant (Meeus Eq. 12.3, degree form):
 * theta = 280.46061837 + 360.98564736629*(JD-2451545.0)
 *       + 0.000387933*T^2 - T^3/38710000 */
static double gmst_general(double jd) {
    double T = (jd - 2451545.0) / 36525.0;
    double theta = 280.46061837
                 + 360.98564736629 * (jd - 2451545.0)
                 + 0.000387933 * T * T
                 - T * T * T / 38710000.0;
    return normalize_degrees(theta);
}

/* Simplified nutation in longitude (arcseconds).
 * Meeus Chapter 22, low-accuracy expressions (~0.5" accuracy). */
static double nutation_longitude(double T) {
    double omega = (125.04452 - 1934.136261 * T) * DEG_TO_RAD;
    double L_sun = (280.4665 + 36000.7698 * T) * DEG_TO_RAD;
    double L_moon = (218.3165 + 481267.8813 * T) * DEG_TO_RAD;

    return -17.20 * sin(omega)
           - 1.32 * sin(2.0 * L_sun)
           - 0.23 * sin(2.0 * L_moon)
           + 0.21 * sin(2.0 * omega);
}

/* Simplified nutation in obliquity (arcseconds). */
static double nutation_obliquity(double T) {
    double omega = (125.04452 - 1934.136261 * T) * DEG_TO_RAD;
    double L_sun = (280.4665 + 36000.7698 * T) * DEG_TO_RAD;
    double L_moon = (218.3165 + 481267.8813 * T) * DEG_TO_RAD;

    return  9.20 * cos(omega)
          + 0.57 * cos(2.0 * L_sun)
          + 0.10 * cos(2.0 * L_moon)
          - 0.09 * cos(2.0 * omega);
}

/* Mean obliquity of the ecliptic (degrees).
 * Meeus Eq. 22.2:
 * eps0 = 23°26'21".448 - 46".8150*T - 0".00059*T^2 + 0".001813*T^3 */
static double mean_obliquity(double T) {
    return 23.439291111
         - (46.8150 / 3600.0) * T
         - (0.00059 / 3600.0) * T * T
         + (0.001813 / 3600.0) * T * T * T;
}

/* Greenwich Apparent Sidereal Time = GMST + equation of the equinoxes.
 * Equation of equinoxes = delta_psi * cos(epsilon) (arcseconds). */
double gast_degrees(double jd) {
    double T = (jd - 2451545.0) / 36525.0;
    double gmst = gmst_general(jd);

    double delta_psi = nutation_longitude(T);
    double delta_eps = nutation_obliquity(T);
    double eps0 = mean_obliquity(T);
    double eps = eps0 + delta_eps / 3600.0;

    double eq_equinoxes = delta_psi * cos(eps * DEG_TO_RAD); /* arcseconds */

    return normalize_degrees(gmst + eq_equinoxes / 3600.0);
}

/* Local Sidereal Time = GST + longitude.
 * longitude_deg: positive east, negative west (modern convention). */
double lst_degrees(double gst_deg, double longitude_deg) {
    return normalize_degrees(gst_deg + longitude_deg);
}
