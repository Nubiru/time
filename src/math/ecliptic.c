#include "ecliptic.h"
#include <math.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
#define RAD_TO_DEG (180.0 / PI)

static double normalize_deg(double deg) {
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

equatorial_t ecliptic_to_equatorial(double longitude, double latitude, double obliquity) {
    double lon = longitude * DEG_TO_RAD;
    double lat = latitude * DEG_TO_RAD;
    double obl = obliquity * DEG_TO_RAD;

    /* Declination: sin(dec) = sin(lat)*cos(obl) + cos(lat)*sin(obl)*sin(lon) */
    double sin_dec = sin(lat) * cos(obl) + cos(lat) * sin(obl) * sin(lon);
    double dec = asin(sin_dec);

    /* Right ascension: atan2 for correct quadrant */
    double y = sin(lon) * cos(obl) - tan(lat) * sin(obl);
    double x = cos(lon);
    double ra = atan2(y, x) * RAD_TO_DEG;

    equatorial_t result;
    result.dec = dec * RAD_TO_DEG;
    result.ra = normalize_deg(ra);
    return result;
}

/* Meeus Eq. 22.2: mean obliquity of the ecliptic.
 * eps0 = 23d 26' 21".448 - 46".8150*T - 0".00059*T^2 + 0".001813*T^3 */
double mean_obliquity(double jd) {
    double T = (jd - 2451545.0) / 36525.0;
    return 23.439291111
         - (46.8150 / 3600.0) * T
         - (0.00059 / 3600.0) * T * T
         + (0.001813 / 3600.0) * T * T * T;
}
