#include "moon_nodes.h"

#include <math.h>

#define PI 3.14159265358979323846
#define DEG2RAD (PI / 180.0)
#define J2000_JD 2451545.0
#define NODAL_PERIOD 6798.383 /* days — one full nodal regression cycle */

/* Normalize angle to 0-360 range */
static double normalize(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

/* Mean longitude of ascending node (Meeus, Ch. 47) */
static double mean_node_longitude(double jd)
{
    double T = (jd - J2000_JD) / 36525.0;
    double omega = 125.04452
                 - 1934.136261 * T
                 + 0.0020708 * T * T
                 + T * T * T / 450000.0;
    return normalize(omega);
}

/* Moon's mean longitude (simplified, for true node correction) */
static double moon_mean_longitude(double jd)
{
    double T = (jd - J2000_JD) / 36525.0;
    double L = 218.3165
             + 481267.8813 * T;
    return normalize(L);
}

/* Fill sign and degree fields from a longitude */
static void fill_sign_degree(double lon, int *sign, double *degree)
{
    double n = normalize(lon);
    *sign = (int)(n / 30.0);
    if (*sign > 11) *sign = 11;
    *degree = n - (*sign) * 30.0;
}

lunar_nodes_t lunar_nodes_mean(double jd)
{
    lunar_nodes_t n;
    n.north_node = mean_node_longitude(jd);
    n.south_node = normalize(n.north_node + 180.0);
    fill_sign_degree(n.north_node, &n.north_sign, &n.north_degree);
    fill_sign_degree(n.south_node, &n.south_sign, &n.south_degree);
    return n;
}

lunar_nodes_t lunar_nodes_true(double jd)
{
    double omega = mean_node_longitude(jd);
    double L = moon_mean_longitude(jd);

    /* Primary periodic correction (Meeus) */
    double delta = -1.4979 * sin(2.0 * omega * DEG2RAD)
                 - 0.15 * sin(2.0 * L * DEG2RAD);

    lunar_nodes_t n;
    n.north_node = normalize(omega + delta);
    n.south_node = normalize(n.north_node + 180.0);
    fill_sign_degree(n.north_node, &n.north_sign, &n.north_degree);
    fill_sign_degree(n.south_node, &n.south_sign, &n.south_degree);
    return n;
}

const char *lunar_node_north_name(void)
{
    return "North Node";
}

const char *lunar_node_south_name(void)
{
    return "South Node";
}

const char *lunar_node_north_symbol(void)
{
    return "\xe2\x98\x8a"; /* ☊ */
}

const char *lunar_node_south_symbol(void)
{
    return "\xe2\x98\x8b"; /* ☋ */
}

/* Angular distance between two longitudes (0-180) */
static double angular_dist(double a, double b)
{
    double d = fabs(normalize(a) - normalize(b));
    if (d > 180.0) d = 360.0 - d;
    return d;
}

int lunar_node_proximity(double longitude, double orb, double jd)
{
    lunar_nodes_t n = lunar_nodes_mean(jd);
    if (angular_dist(longitude, n.north_node) <= orb) return 1;
    if (angular_dist(longitude, n.south_node) <= orb) return 2;
    return 0;
}

double lunar_nodal_period(void)
{
    return NODAL_PERIOD;
}
