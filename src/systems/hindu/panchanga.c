/* panchanga.c -- Panchanga (five-limbed Hindu calendar) calculator
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "panchanga.h"
#include "nakshatra.h"

#include <math.h>
#include <stddef.h>

/* ===== Tithi data: 30 tithis (15 Shukla + 15 Krishna) ===== */

static const char *const TITHI_NAMES[30] = {
    "Pratipada", "Dvitiya",    "Tritiya",    "Chaturthi",  "Panchami",
    "Shashthi",  "Saptami",    "Ashtami",    "Navami",     "Dashami",
    "Ekadashi",  "Dvadashi",   "Trayodashi", "Chaturdashi", "Purnima",
    "Pratipada", "Dvitiya",    "Tritiya",    "Chaturthi",  "Panchami",
    "Shashthi",  "Saptami",    "Ashtami",    "Navami",     "Dashami",
    "Ekadashi",  "Dvadashi",   "Trayodashi", "Chaturdashi", "Amavasya"
};

/* ===== Vara data ===== */

static const char *const VARA_NAMES[7] = {
    "Ravivara", "Somavara", "Mangalavara", "Budhavara",
    "Guruvara", "Shukravara", "Shanivara"
};

static const char *const VARA_DEITIES[7] = {
    "Surya", "Chandra", "Mangala", "Budha",
    "Brihaspati", "Shukra", "Shani"
};

static const char *const VARA_PLANETS[7] = {
    "Sun", "Moon", "Mars", "Mercury",
    "Jupiter", "Venus", "Saturn"
};

/* ===== Yoga data: 27 yogas ===== */

static const char *const YOGA_NAMES[27] = {
    "Vishkambha", "Priti",     "Ayushman",   "Saubhagya",
    "Shobhana",   "Atiganda",  "Sukarma",    "Dhriti",
    "Shula",       "Ganda",     "Vriddhi",    "Dhruva",
    "Vyaghata",   "Harshana",  "Vajra",      "Siddhi",
    "Vyatipata",  "Variyan",   "Parigha",    "Shiva",
    "Siddha",     "Sadhya",    "Shubha",     "Shukla",
    "Brahma",     "Indra",     "Vaidhriti"
};

/* Quality: 0=Auspicious, 1=Inauspicious, 2=Neutral */
static const int YOGA_QUALITY[27] = {
    1, 0, 0, 0, 0, 1, 0, 0,  /* Vishkambha(I), Priti(A), Ayushman(A), Saubhagya(A), Shobhana(A), Atiganda(I), Sukarma(A), Dhriti(A) */
    1, 1, 0, 0, 1, 0, 1, 0,  /* Shula(I), Ganda(I), Vriddhi(A), Dhruva(A), Vyaghata(I), Harshana(A), Vajra(I), Siddhi(A) */
    1, 2, 1, 0, 0, 0, 0, 0,  /* Vyatipata(I), Variyan(N), Parigha(I), Shiva(A), Siddha(A), Sadhya(A), Shubha(A), Shukla(A) */
    0, 0, 1                   /* Brahma(A), Indra(A), Vaidhriti(I) */
};

static const char *const QUALITY_LABELS[3] = {
    "Auspicious", "Inauspicious", "Neutral"
};

/* ===== Karana data: 11 karanas (4 fixed + 7 moving) ===== */

static const char *const KARANA_NAMES[11] = {
    "Kimstughna",   /* 0: fixed */
    "Bava",         /* 1: moving */
    "Balava",       /* 2: moving */
    "Kaulava",      /* 3: moving */
    "Taitila",      /* 4: moving */
    "Gara",         /* 5: moving */
    "Vanija",       /* 6: moving */
    "Vishti",       /* 7: moving */
    "Shakuni",      /* 8: fixed */
    "Chatushpada",  /* 9: fixed */
    "Naga"          /* 10: fixed */
};

static const bool KARANA_FIXED[11] = {
    true,                                        /* Kimstughna */
    false, false, false, false, false, false, false, /* 7 moving */
    true, true, true                              /* Shakuni, Chatushpada, Naga */
};

/* ===== Tithi ===== */

tithi_t panchanga_tithi(double elongation)
{
    /* Normalize elongation to [0, 360) */
    while (elongation < 0.0) elongation += 360.0;
    while (elongation >= 360.0) elongation -= 360.0;

    int number = (int)floor(elongation / 12.0) + 1;
    if (number > 30) number = 30;
    if (number < 1) number = 1;

    tithi_t t;
    t.number = number;
    t.name = TITHI_NAMES[number - 1];
    t.paksha = (number <= 15) ? "Shukla" : "Krishna";
    t.paksha_day = (number <= 15) ? number : number - 15;
    return t;
}

/* ===== Vara ===== */

vara_t panchanga_vara(double jd)
{
    /* Day of week from Julian Day:
     * floor(jd + 1.5) mod 7: 0=Sunday..6=Saturday */
    int day = ((int)floor(jd + 1.5)) % 7;
    if (day < 0) day += 7;

    vara_t v;
    v.number = day;
    v.name = VARA_NAMES[day];
    v.deity = VARA_DEITIES[day];
    v.planet = VARA_PLANETS[day];
    return v;
}

/* ===== Yoga ===== */

yoga_t panchanga_yoga(double sun_sidereal, double moon_sidereal)
{
    double sum = sun_sidereal + moon_sidereal;
    /* Normalize to [0, 360) */
    while (sum < 0.0) sum += 360.0;
    while (sum >= 360.0) sum -= 360.0;

    int index = (int)floor(sum / (360.0 / 27.0));
    if (index >= 27) index = 26;
    if (index < 0) index = 0;

    yoga_t y;
    y.number = index;
    y.name = YOGA_NAMES[index];
    y.quality = QUALITY_LABELS[YOGA_QUALITY[index]];
    return y;
}

/* ===== Karana ===== */

karana_t panchanga_karana(double elongation)
{
    /* Normalize elongation to [0, 360) */
    while (elongation < 0.0) elongation += 360.0;
    while (elongation >= 360.0) elongation -= 360.0;

    int position = (int)floor(elongation / 6.0);
    if (position >= 60) position = 59;
    if (position < 0) position = 0;

    karana_t k;

    if (position == 0) {
        /* Fixed: Kimstughna */
        k.number = 0;
    } else if (position <= 56) {
        /* Moving: cycle through 7 karanas (indices 1-7) */
        k.number = ((position - 1) % 7) + 1;
    } else {
        /* Fixed: 57=Shakuni(8), 58=Chatushpada(9), 59=Naga(10) */
        k.number = position - 57 + 8;
    }

    k.name = KARANA_NAMES[k.number];
    k.is_fixed = KARANA_FIXED[k.number];
    return k;
}

/* ===== panchanga_compute ===== */

panchanga_t panchanga_compute(double jd, double sun_longitude,
                              double moon_longitude)
{
    double ayanamsa = nakshatra_lahiri_ayanamsa(jd);
    double sun_sid = nakshatra_tropical_to_sidereal(sun_longitude, ayanamsa);
    double moon_sid = nakshatra_tropical_to_sidereal(moon_longitude, ayanamsa);

    double elongation = moon_longitude - sun_longitude;
    /* Normalize to [0, 360) */
    while (elongation < 0.0) elongation += 360.0;
    while (elongation >= 360.0) elongation -= 360.0;

    panchanga_t p;
    p.tithi = panchanga_tithi(elongation);
    p.vara = panchanga_vara(jd);
    p.nakshatra_index = (int)(moon_sid / (360.0 / 27.0));
    if (p.nakshatra_index >= 27) p.nakshatra_index = 26;
    if (p.nakshatra_index < 0) p.nakshatra_index = 0;
    p.yoga = panchanga_yoga(sun_sid, moon_sid);
    p.karana = panchanga_karana(elongation);
    return p;
}

/* ===== Name lookups ===== */

const char *panchanga_tithi_name(int tithi_number)
{
    if (tithi_number < 1 || tithi_number > 30) return NULL;
    return TITHI_NAMES[tithi_number - 1];
}

const char *panchanga_vara_name(int vara_number)
{
    if (vara_number < 0 || vara_number > 6) return NULL;
    return VARA_NAMES[vara_number];
}

const char *panchanga_yoga_name(int yoga_number)
{
    if (yoga_number < 0 || yoga_number > 26) return NULL;
    return YOGA_NAMES[yoga_number];
}

const char *panchanga_karana_name(int karana_number)
{
    if (karana_number < 0 || karana_number > 10) return NULL;
    return KARANA_NAMES[karana_number];
}

/* ===== Count accessors ===== */

int panchanga_tithi_count(void) { return PANCHANGA_TITHI_COUNT; }
int panchanga_vara_count(void) { return PANCHANGA_VARA_COUNT; }
int panchanga_yoga_count(void) { return PANCHANGA_YOGA_COUNT; }
int panchanga_karana_count(void) { return PANCHANGA_KARANA_COUNT; }
