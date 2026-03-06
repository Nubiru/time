#include "dignity.h"

/* Traditional rulers: RULERS[sign] = planet index that rules that sign. */
static const int RULERS[12] = {
    3,  /* 0  Aries     -> Mars    */
    1,  /* 1  Taurus    -> Venus   */
    0,  /* 2  Gemini    -> Mercury */
    9,  /* 3  Cancer    -> Moon    */
    8,  /* 4  Leo       -> Sun     */
    0,  /* 5  Virgo     -> Mercury */
    1,  /* 6  Libra     -> Venus   */
    3,  /* 7  Scorpio   -> Mars    */
    4,  /* 8  Sagittarius -> Jupiter */
    5,  /* 9  Capricorn -> Saturn  */
    5,  /* 10 Aquarius  -> Saturn  */
    4   /* 11 Pisces    -> Jupiter */
};

/* Exaltation rulers: EXALTATIONS[sign] = planet index, or -1 if none. */
static const int EXALTATIONS[12] = {
    8,  /* 0  Aries     -> Sun     */
    9,  /* 1  Taurus    -> Moon    */
   -1,  /* 2  Gemini    -> none    */
    4,  /* 3  Cancer    -> Jupiter */
   -1,  /* 4  Leo       -> none    */
    0,  /* 5  Virgo     -> Mercury */
    5,  /* 6  Libra     -> Saturn  */
   -1,  /* 7  Scorpio   -> none    */
   -1,  /* 8  Sagittarius -> none  */
    3,  /* 9  Capricorn -> Mars    */
   -1,  /* 10 Aquarius  -> none    */
    1   /* 11 Pisces    -> Venus   */
};

/* Primary domicile for each planet. -1 for Earth/Uranus/Neptune. */
static const int PRIMARY_DOMICILE[10] = {
    2,  /* 0 Mercury -> Gemini  */
    1,  /* 1 Venus   -> Taurus  */
   -1,  /* 2 Earth   -> none    */
    0,  /* 3 Mars    -> Aries   */
    8,  /* 4 Jupiter -> Sagittarius */
    9,  /* 5 Saturn  -> Capricorn */
   -1,  /* 6 Uranus  -> none    */
   -1,  /* 7 Neptune -> none    */
    4,  /* 8 Sun     -> Leo     */
    3   /* 9 Moon    -> Cancer  */
};

static int is_ruler(int planet, int sign) {
    return RULERS[sign] == planet;
}

static int is_exalted(int planet, int sign) {
    return EXALTATIONS[sign] == planet;
}

static int is_in_detriment(int planet, int sign) {
    int opposite = (sign + 6) % 12;
    return RULERS[opposite] == planet;
}

static int is_in_fall(int planet, int sign) {
    int opposite = (sign + 6) % 12;
    return EXALTATIONS[opposite] == planet;
}

dignity_result_t planet_dignity(int planet, int sign) {
    dignity_result_t r;
    r.planet = planet;
    r.sign = sign;
    r.dignity = DIGNITY_PEREGRINE;
    r.score = 0;

    if (planet < 0 || planet > 9 || sign < 0 || sign > 11) {
        return r;
    }

    /* Earth has no astrological dignities */
    if (planet == DIGNITY_EARTH) {
        return r;
    }

    /* Check in order of strength: domicile > exaltation > detriment > fall */
    if (is_ruler(planet, sign)) {
        r.dignity = DIGNITY_DOMICILE;
        r.score = DIGNITY_DOMICILE;
    } else if (is_exalted(planet, sign)) {
        r.dignity = DIGNITY_EXALTATION;
        r.score = DIGNITY_EXALTATION;
    } else if (is_in_detriment(planet, sign)) {
        r.dignity = DIGNITY_DETRIMENT;
        r.score = DIGNITY_DETRIMENT;
    } else if (is_in_fall(planet, sign)) {
        r.dignity = DIGNITY_FALL;
        r.score = DIGNITY_FALL;
    }

    return r;
}

int sign_ruler(int sign) {
    if (sign < 0 || sign > 11) return -1;
    return RULERS[sign];
}

int sign_exaltation_ruler(int sign) {
    if (sign < 0 || sign > 11) return -1;
    return EXALTATIONS[sign];
}

int planet_domicile_sign(int planet) {
    if (planet < 0 || planet > 9) return -1;
    return PRIMARY_DOMICILE[planet];
}

const char *dignity_name(dignity_type_t dignity) {
    switch (dignity) {
        case DIGNITY_DOMICILE:   return "Domicile";
        case DIGNITY_EXALTATION: return "Exaltation";
        case DIGNITY_PEREGRINE:  return "Peregrine";
        case DIGNITY_FALL:       return "Fall";
        case DIGNITY_DETRIMENT:  return "Detriment";
    }
    return "?";
}

int dignity_score_total(const int planet_signs[10]) {
    int total = 0;
    for (int i = 0; i < 10; i++) {
        dignity_result_t r = planet_dignity(i, planet_signs[i]);
        total += r.score;
    }
    return total;
}
