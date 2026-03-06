#ifndef TIME_NAKSHATRA_H
#define TIME_NAKSHATRA_H

#define NAKSHATRA_COUNT 27
#define NAKSHATRA_PADA_COUNT 4
#define NAKSHATRA_TOTAL_PADAS 108

/* Nakshatra data */
typedef struct {
    int id;                     /* 0-26 */
    const char *name;           /* Sanskrit name */
    const char *meaning;        /* English meaning */
    double start_deg;           /* sidereal longitude start (degrees) */
    double end_deg;             /* sidereal longitude end (degrees) */
    int ruling_planet;          /* 0=Ketu,1=Venus,2=Sun,3=Moon,4=Mars,5=Rahu,6=Jupiter,7=Saturn,8=Mercury */
    const char *deity;          /* presiding deity */
    const char *symbol;         /* traditional symbol */
    const char *quality;        /* Deva/Manushya/Rakshasa (divine/human/demon) */
    int yogatara_catalog_index; /* index in star_catalog (-1 if not in catalog) */
} nakshatra_t;

/* Pada (quarter) data */
typedef struct {
    int nakshatra_id;           /* which nakshatra (0-26) */
    int pada_number;            /* 1-4 */
    int navamsa_sign;           /* zodiac sign (0-11: Aries-Pisces) */
    double start_deg;           /* sidereal longitude start */
    double end_deg;             /* sidereal longitude end */
} nakshatra_pada_t;

/* Get Nakshatra from sidereal longitude (0-360). */
nakshatra_t nakshatra_from_longitude(double sidereal_longitude);

/* Get Nakshatra by index (0-26). */
nakshatra_t nakshatra_get(int index);

/* Get Nakshatra name. */
const char *nakshatra_name(int index);

/* Get pada from sidereal longitude. */
nakshatra_pada_t nakshatra_pada_from_longitude(double sidereal_longitude);

/* Get specific pada. nakshatra_id: 0-26, pada: 1-4. */
nakshatra_pada_t nakshatra_pada_get(int nakshatra_id, int pada);

/* Get Navamsa sign (0-11) from sidereal longitude. */
int nakshatra_navamsa(double sidereal_longitude);

/* Convert tropical longitude to sidereal using Lahiri ayanamsa.
 * ayanamsa_deg: current ayanamsa (approximately 24.0 for modern dates).
 * Returns sidereal longitude (0-360). */
double nakshatra_tropical_to_sidereal(double tropical_longitude,
                                       double ayanamsa_deg);

/* Compute Lahiri ayanamsa for a given Julian Day.
 * Approximate formula: ayanamsa = 23.85 + 0.01396 * (jd - 2451545.0) / 365.25
 * Returns ayanamsa in degrees. */
double nakshatra_lahiri_ayanamsa(double jd);

/* Ruling planet name for a Nakshatra ruling planet index. */
const char *nakshatra_ruler_name(int ruler_index);

/* Total number of Nakshatras. */
int nakshatra_count(void);

#endif /* TIME_NAKSHATRA_H */
