#include "precession.h"

#include <math.h>

/* IAU 2006 general precession coefficients (arcseconds) */
static const double PA_LINEAR    = 5028.796195;  /* per century */
static const double PA_QUADRATIC = 1.1054348;    /* per century^2 */
static const double PA_CUBIC     = -0.00007964;  /* per century^3 */

/* Lahiri ayanamsa at J2000.0 (degrees) */
static const double AYANAMSA_J2000 = 23.853;

static const char *AGE_NAMES[12] = {
    "Pisces",      /* 0 — current */
    "Aquarius",    /* 1 — upcoming */
    "Capricorn",   /* 2 */
    "Sagittarius", /* 3 */
    "Scorpio",     /* 4 */
    "Libra",       /* 5 */
    "Virgo",       /* 6 */
    "Leo",         /* 7 */
    "Cancer",      /* 8 */
    "Gemini",      /* 9 */
    "Taurus",      /* 10 */
    "Aries"        /* 11 */
};

static double normalize_angle(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    return deg;
}

double prec_centuries_from_j2000(double jd)
{
    return (jd - PREC_J2000_JD) / 36525.0;
}

double prec_accumulated(double jd)
{
    double t = prec_centuries_from_j2000(jd);
    double pa_arcsec = PA_LINEAR * t
                     + PA_QUADRATIC * t * t
                     + PA_CUBIC * t * t * t;
    return pa_arcsec / 3600.0;
}

double prec_rate(double jd)
{
    double t = prec_centuries_from_j2000(jd);
    /* Derivative of precession polynomial (arcsec/century) */
    double rate_century = PA_LINEAR
                        + 2.0 * PA_QUADRATIC * t
                        + 3.0 * PA_CUBIC * t * t;
    /* Convert to arcsec/year */
    return rate_century / 100.0;
}

double prec_ayanamsa_lahiri(double jd)
{
    return AYANAMSA_J2000 + prec_accumulated(jd);
}

double prec_tropical_to_sidereal(double tropical_lon, double jd)
{
    double ayan = prec_ayanamsa_lahiri(jd);
    return normalize_angle(tropical_lon - ayan);
}

double prec_sidereal_to_tropical(double sidereal_lon, double jd)
{
    double ayan = prec_ayanamsa_lahiri(jd);
    return normalize_angle(sidereal_lon + ayan);
}

double prec_cycle_period(void)
{
    return PREC_PERIOD;
}

int prec_current_age(double jd)
{
    double ayan = prec_ayanamsa_lahiri(jd);
    double norm = normalize_angle(ayan);
    int age = (int)(norm / 30.0);
    if (age > 11) age = 11;
    return age;
}

const char *prec_age_name(int age_index)
{
    if (age_index < 0 || age_index > 11)
        return "?";
    return AGE_NAMES[age_index];
}
