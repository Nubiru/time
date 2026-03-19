/* equatorial_horizontal.c — Equatorial <-> Horizontal coordinate transform.
 *
 * Meeus, "Astronomical Algorithms", Chapter 13.
 * Bennett refraction formula, Meeus Chapter 16. */

#include "equatorial_horizontal.h"
#include "sidereal.h"
#include <math.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
#define RAD_TO_DEG (180.0 / PI)

static double normalize_deg(double deg) {
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

double eh_hour_angle(double lst_deg, double ra_deg) {
    return normalize_deg(lst_deg - ra_deg);
}

horizontal_t eh_to_horizontal(equatorial_t eq, double observer_lat_deg,
                              double lst_deg) {
    double h_deg = eh_hour_angle(lst_deg, eq.ra);
    double h   = h_deg * DEG_TO_RAD;
    double dec = eq.dec * DEG_TO_RAD;
    double lat = observer_lat_deg * DEG_TO_RAD;

    /* Altitude: sin(alt) = sin(dec)*sin(lat) + cos(dec)*cos(lat)*cos(H) */
    double sin_alt = sin(dec) * sin(lat) + cos(dec) * cos(lat) * cos(h);
    double alt = asin(sin_alt);

    /* Azimuth (Meeus measures from South; we convert to compass North=0).
     * A_south = atan2(sin(H), cos(H)*sin(lat) - tan(dec)*cos(lat))
     * A_compass = A_south + 180 */
    double az_south = atan2(sin(h),
                            cos(h) * sin(lat) - tan(dec) * cos(lat));

    horizontal_t result;
    result.altitude = alt * RAD_TO_DEG;
    result.azimuth  = normalize_deg(az_south * RAD_TO_DEG + 180.0);
    return result;
}

equatorial_t eh_to_equatorial(horizontal_t hz, double observer_lat_deg,
                              double lst_deg) {
    double alt = hz.altitude * DEG_TO_RAD;
    double lat = observer_lat_deg * DEG_TO_RAD;
    double az  = hz.azimuth * DEG_TO_RAD; /* compass: North=0 */

    /* Declination (spherical cosine rule, angle at zenith = compass azimuth):
     * sin(dec) = sin(alt)*sin(lat) + cos(alt)*cos(lat)*cos(A_north) */
    double sin_dec = sin(alt) * sin(lat) + cos(alt) * cos(lat) * cos(az);
    double dec = asin(sin_dec);

    /* Hour angle (spherical sine/cosine rule):
     * H = atan2(-sin(A_north), tan(alt)*cos(lat) - cos(A_north)*sin(lat)) */
    double h = atan2(-sin(az),
                     tan(alt) * cos(lat) - cos(az) * sin(lat));

    /* RA = LST - H */
    double ra = normalize_deg(lst_deg - h * RAD_TO_DEG);

    equatorial_t result;
    result.ra  = ra;
    result.dec = dec * RAD_TO_DEG;
    return result;
}

horizontal_t eh_to_horizontal_jd(equatorial_t eq, double observer_lat_deg,
                                 double observer_lon_deg, double jd) {
    double gast = gast_degrees(jd);
    double lst  = lst_degrees(gast, observer_lon_deg);
    return eh_to_horizontal(eq, observer_lat_deg, lst);
}

int eh_is_visible(horizontal_t hz) {
    return hz.altitude >= 0.0 ? 1 : 0;
}

double eh_refraction(double altitude_deg) {
    /* Bennett formula (Meeus Ch. 16):
     * R = 1.02 / tan(h + 10.3/(h + 5.11)) / 60   (arcminutes -> degrees)
     * where h is apparent altitude in degrees.
     * For very low/negative altitudes, clamp to avoid extreme values. */
    if (altitude_deg < -1.0) {
        altitude_deg = -1.0;
    }
    double h_arg = altitude_deg + 10.3 / (altitude_deg + 5.11);
    double tan_val = tan(h_arg * DEG_TO_RAD);

    /* Guard against division by zero (should not happen for valid range) */
    if (fabs(tan_val) < 1.0e-10) {
        return 0.59; /* Standard horizon refraction */
    }

    double r = 1.02 / tan_val / 60.0; /* Convert arcminutes to degrees */

    /* Refraction is always positive (it lifts objects) */
    if (r < 0.0) r = 0.0;

    return r;
}
