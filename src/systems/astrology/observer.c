#include "observer.h"
#include <math.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)
#define RAD_TO_DEG (180.0 / PI)

double ascendant_longitude(double lst_deg, double obliquity_deg, double latitude_deg) {
    double lst = lst_deg * DEG_TO_RAD;
    double obl = obliquity_deg * DEG_TO_RAD;
    double lat = latitude_deg * DEG_TO_RAD;

    double y = -cos(lst);
    double x = sin(obl) * tan(lat) + cos(obl) * sin(lst);
    double asc = atan2(y, x) * RAD_TO_DEG;

    asc = fmod(asc, 360.0);
    if (asc < 0.0) asc += 360.0;
    if (asc >= 360.0) asc -= 360.0;
    return asc;
}
