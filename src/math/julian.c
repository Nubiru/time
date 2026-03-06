#include "julian.h"
#include <math.h>

/* Meeus, "Astronomical Algorithms", Chapter 7.
 * INT(x) = floor(x) — greatest integer <= x. */

static double jd_from_date(int year, int month, double day, int gregorian) {
    int y = year;
    int m = month;

    /* January and February are treated as months 13/14 of the previous year */
    if (m <= 2) {
        y = y - 1;
        m = m + 12;
    }

    double b = 0.0;
    if (gregorian) {
        int a = (int)floor((double)y / 100.0);
        b = 2.0 - a + (int)floor((double)a / 4.0);
    }

    double jd = floor(365.25 * (y + 4716))
              + floor(30.6001 * (m + 1))
              + day + b - 1524.5;

    return jd;
}

double gregorian_to_jd(int year, int month, double day) {
    return jd_from_date(year, month, day, 1);
}

double julian_cal_to_jd(int year, int month, double day) {
    return jd_from_date(year, month, day, 0);
}

double jd_to_gregorian(double jd, int *year, int *month) {
    double z_d = floor(jd + 0.5);
    double f = (jd + 0.5) - z_d;
    int z = (int)z_d;

    int a;
    if (z < 2299161) {
        a = z;
    } else {
        int alpha = (int)floor((z - 1867216.25) / 36524.25);
        a = z + 1 + alpha - (int)floor((double)alpha / 4.0);
    }

    int b = a + 1524;
    int c = (int)floor((b - 122.1) / 365.25);
    int d = (int)floor(365.25 * c);
    int e = (int)floor((double)(b - d) / 30.6001);

    double day = (b - d) - (int)floor(30.6001 * e) + f;

    int m;
    if (e < 14) {
        m = e - 1;
    } else {
        m = e - 13;
    }

    int y;
    if (m > 2) {
        y = c - 4716;
    } else {
        y = c - 4715;
    }

    *year = y;
    *month = m;
    return day;
}
