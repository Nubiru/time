#ifndef TIME_DIGNITY_H
#define TIME_DIGNITY_H

/* Essential dignity types with traditional scoring. */
typedef enum {
    DIGNITY_DETRIMENT  = -5,
    DIGNITY_FALL       = -4,
    DIGNITY_PEREGRINE  =  0,
    DIGNITY_EXALTATION =  4,
    DIGNITY_DOMICILE   =  5
} dignity_type_t;

/* Result of checking a planet's dignity in a sign. */
typedef struct {
    dignity_type_t dignity;  /* strongest dignity in this sign */
    int planet;              /* planet index (0-9) */
    int sign;                /* zodiac sign (0-11) */
    int score;               /* numerical score */
} dignity_result_t;

/* Planet indices (0-7 match planets.h, 8-9 are luminaries for astrology). */
enum {
    DIGNITY_MERCURY = 0,
    DIGNITY_VENUS   = 1,
    DIGNITY_EARTH   = 2,
    DIGNITY_MARS    = 3,
    DIGNITY_JUPITER = 4,
    DIGNITY_SATURN  = 5,
    DIGNITY_URANUS  = 6,
    DIGNITY_NEPTUNE = 7,
    DIGNITY_SUN     = 8,
    DIGNITY_MOON    = 9
};

/* Core: determine the strongest essential dignity for a planet in a sign. */
dignity_result_t planet_dignity(int planet, int sign);

/* Lookup: traditional ruler of a sign (planet index). */
int sign_ruler(int sign);

/* Lookup: exaltation ruler of a sign (planet index, or -1 if none). */
int sign_exaltation_ruler(int sign);

/* Lookup: primary domicile sign for a planet (-1 if none). */
int planet_domicile_sign(int planet);

/* Human-readable dignity name. */
const char *dignity_name(dignity_type_t dignity);

/* Batch: sum dignity scores for 10 planets given their sign placements.
 * planet_signs[i] = zodiac sign (0-11) that planet i occupies. */
int dignity_score_total(const int planet_signs[10]);

#endif
