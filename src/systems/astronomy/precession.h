#ifndef TIME_PRECESSION_H
#define TIME_PRECESSION_H

#define PREC_J2000_JD 2451545.0
#define PREC_PERIOD   25772.0

/* Julian centuries from J2000.0 epoch. */
double prec_centuries_from_j2000(double jd);

/* Accumulated general precession in longitude since J2000.0, in degrees.
 * IAU 2006 polynomial (Capitaine et al. 2003). */
double prec_accumulated(double jd);

/* Precession rate at a given epoch, in arcseconds per Julian year. */
double prec_rate(double jd);

/* Lahiri ayanamsa in degrees (tropical-to-sidereal offset). */
double prec_ayanamsa_lahiri(double jd);

/* Convert tropical ecliptic longitude to sidereal. */
double prec_tropical_to_sidereal(double tropical_lon, double jd);

/* Convert sidereal ecliptic longitude to tropical. */
double prec_sidereal_to_tropical(double sidereal_lon, double jd);

/* Full precession cycle period in Julian years. */
double prec_cycle_period(void);

/* Current astrological age index (0=Pisces, 1=Aquarius, ..., 11=Aries). */
int prec_current_age(double jd);

/* Name of astrological age from index. Returns "?" for invalid. */
const char *prec_age_name(int age_index);

#endif
