/* korean_calendar.c -- Korean Traditional Calendar implementation
 * Self-contained Gregorian JD math (Meeus, "Astronomical Algorithms" Ch.7).
 * New moon approximation (Meeus, "Astronomical Algorithms" Ch.49).
 * Dangun epoch: Korean year = Gregorian year + 2333.
 * Sexagenary cycle: same formula as Chinese (stem=(year-4)%10, branch=(year-4)%12). */

#include <math.h>
#include <stddef.h>

#include "korean_calendar.h"

#define PI 3.14159265358979323846

/* ===== Sexagenary cycle epoch ===== */

static const int CYCLE_EPOCH = 4; /* 4 CE = Gap-Ja (stem 0, branch 0, cycle year 1) */

/* ===== Reference new moon (Meeus Ch.49) ===== */

static const double NEW_MOON_REF_JD = 2451550.09766; /* Jan 6, 2000 ~18:14 UT */
static const double SYNODIC_MONTH   = 29.530588861;

/* ===== Name tables ===== */

static const char *STEM_NAMES[10] = {
    "Gap", "Eul", "Byeong", "Jeong", "Mu",
    "Gi", "Gyeong", "Sin", "Im", "Gye"
};

static const char *BRANCH_NAMES[12] = {
    "Ja", "Chuk", "In", "Myo", "Jin", "Sa",
    "O", "Mi", "Sin", "Yu", "Sul", "Hae"
};

static const char *ANIMAL_NAMES[12] = {
    "Jwi", "So", "Horangi", "Tokki", "Yong", "Baem",
    "Mal", "Yang", "Wonsungi", "Dak", "Gae", "Dwaeji"
};

static const char *ELEMENT_NAMES[5] = {
    "Mok", "Hwa", "To", "Geum", "Su"
};

/* All 60 sexagenary combination names: "Stem-Branch" for cycle years 1-60 */
static const char *SEXAGENARY_NAMES[60] = {
    "Gap-Ja",       "Eul-Chuk",     "Byeong-In",    "Jeong-Myo",    "Mu-Jin",
    "Gi-Sa",        "Gyeong-O",     "Sin-Mi",        "Im-Sin",       "Gye-Yu",
    "Gap-Sul",      "Eul-Hae",      "Byeong-Ja",    "Jeong-Chuk",   "Mu-In",
    "Gi-Myo",       "Gyeong-Jin",   "Sin-Sa",        "Im-O",         "Gye-Mi",
    "Gap-Sin",      "Eul-Yu",       "Byeong-Sul",   "Jeong-Hae",    "Mu-Ja",
    "Gi-Chuk",      "Gyeong-In",    "Sin-Myo",       "Im-Jin",       "Gye-Sa",
    "Gap-O",        "Eul-Mi",       "Byeong-Sin",   "Jeong-Yu",     "Mu-Sul",
    "Gi-Hae",       "Gyeong-Ja",    "Sin-Chuk",      "Im-In",        "Gye-Myo",
    "Gap-Jin",      "Eul-Sa",       "Byeong-O",     "Jeong-Mi",     "Mu-Sin",
    "Gi-Yu",        "Gyeong-Sul",   "Sin-Hae",       "Im-Ja",        "Gye-Chuk",
    "Gap-In",       "Eul-Myo",      "Byeong-Jin",   "Jeong-Sa",     "Mu-O",
    "Gi-Mi",        "Gyeong-Sin",   "Sin-Yu",        "Im-Sul",       "Gye-Hae"
};

/* ===== Festival table ===== */

static const korean_festival_t FESTIVALS[7] = {
    { 0, "Seollal",       "\xec\x84\xa4\xeb\x82\xa0",
      "Lunar New Year", 1, 1 },
    { 1, "Daeboreum",     "\xeb\x8c\x80\xeb\xb3\xb4\xeb\xa6\x84",
      "Great Full Moon Festival", 1, 15 },
    { 2, "Samjinnal",     "\xec\x82\xbc\xec\xa7\x87\xeb\x82\xa0",
      "Spring Festival", 3, 3 },
    { 3, "Dano",          "\xeb\x8b\xa8\xec\x98\xa4",
      "Summer Festival", 5, 5 },
    { 4, "Chilseok",      "\xec\xb9\xa0\xec\x84\x9d",
      "Star Festival", 7, 7 },
    { 5, "Chuseok",       "\xec\xb6\x94\xec\x84\x9d",
      "Harvest Moon Festival", 8, 15 },
    { 6, "Jungyangjeol",  "\xec\xa4\x91\xec\x96\x91\xec\xa0\x88",
      "Double Ninth Festival", 9, 9 }
};

/* ===== Internal Gregorian <-> JD (Meeus Ch.7) ===== */

static double gregorian_to_jd_internal(int year, int month, double day)
{
    int y = year;
    int m = month;

    if (m <= 2) {
        y = y - 1;
        m = m + 12;
    }

    int a = (int)floor((double)y / 100.0);
    double b = 2.0 - a + (int)floor((double)a / 4.0);

    return floor(365.25 * (y + 4716))
         + floor(30.6001 * (m + 1))
         + day + b - 1524.5;
}

static void jd_to_gregorian_internal(double jd, int *year, int *month, int *day)
{
    double z_d = floor(jd + 0.5);
    double f = (jd + 0.5) - z_d;
    int z = (int)z_d;
    (void)f; /* fractional day not needed for date */

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

    double day_frac = (b - d) - (int)floor(30.6001 * e) + f;

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
    *day = (int)day_frac;
}

/* ===== Positive modulo (C modulo can be negative) ===== */

static int pos_mod(int val, int mod)
{
    int r = val % mod;
    if (r < 0) r += mod;
    return r;
}

/* ===== Approximate new moon (Meeus Ch.49, truncated) ===== */

/* Return approximate JD of the new moon nearest to a given lunation number k.
 * k = 0 corresponds to the reference new moon (Jan 6, 2000).
 * Positive k = future new moons, negative = past.
 * Source: Jean Meeus, "Astronomical Algorithms", 2nd ed., Ch.49 */
static double approx_new_moon(double k)
{
    double T = k / 1236.85;
    double T2 = T * T;
    double T3 = T2 * T;
    double T4 = T3 * T;

    /* Mean JDE of the new moon phase */
    double jde = 2451550.09766
               + 29.530588861 * k
               + 0.00015437 * T2
               - 0.000000150 * T3
               + 0.00000000073 * T4;

    /* Sun's mean anomaly */
    double M = 2.5534 + 29.10535670 * k
             - 0.0000014 * T2
             - 0.00000011 * T3;
    M = M * PI / 180.0;

    /* Moon's mean anomaly */
    double Mp = 201.5643 + 385.81693528 * k
              + 0.0107582 * T2
              + 0.00001238 * T3
              - 0.000000058 * T4;
    Mp = Mp * PI / 180.0;

    /* Moon's argument of latitude */
    double F = 160.7108 + 390.67050284 * k
             - 0.0016118 * T2
             - 0.00000227 * T3
             + 0.000000011 * T4;
    F = F * PI / 180.0;

    /* Longitude of ascending node */
    double omega = 124.7746 - 1.56375588 * k
                 + 0.0020672 * T2
                 + 0.00000215 * T3;
    omega = omega * PI / 180.0;

    /* Corrections (primary terms) */
    double correction = -0.40720 * sin(Mp)
                      +  0.17241 * sin(M)
                      +  0.01608 * sin(2.0 * Mp)
                      +  0.01039 * sin(2.0 * F)
                      +  0.00739 * sin(Mp - M)
                      -  0.00514 * sin(Mp + M)
                      +  0.00208 * sin(2.0 * M)
                      -  0.00111 * sin(Mp - 2.0 * F)
                      -  0.00057 * sin(Mp + 2.0 * F)
                      +  0.00056 * sin(2.0 * Mp + M)
                      -  0.00042 * sin(3.0 * Mp)
                      +  0.00042 * sin(M + 2.0 * F)
                      +  0.00038 * sin(M - 2.0 * F)
                      -  0.00024 * sin(2.0 * Mp - M)
                      -  0.00017 * sin(omega);

    return jde + correction;
}

/* Find the new moon JD on or before a given JD */
static double new_moon_before(double jd)
{
    /* Estimate lunation number from reference */
    double k_est = (jd - NEW_MOON_REF_JD) / SYNODIC_MONTH;
    int k = (int)floor(k_est);

    double nm = approx_new_moon((double)k);
    if (nm > jd + 0.5) {
        nm = approx_new_moon((double)(k - 1));
    }
    /* If we overshot backward, step forward */
    double nm_next = approx_new_moon((double)(k + 1));
    if (nm_next <= jd + 0.5) {
        nm = nm_next;
    }

    return nm;
}

/* ===== Seollal: first new moon on or after Jan 21 of a Gregorian year ===== */

double korean_seollal_jd(int gregorian_year)
{
    /* Jan 21 of the given year */
    double jan21_jd = gregorian_to_jd_internal(gregorian_year, 1, 21.0);

    /* Find new moon on or after Jan 21 */
    double k_est = (jan21_jd - NEW_MOON_REF_JD) / SYNODIC_MONTH;
    int k = (int)floor(k_est);

    double nm = approx_new_moon((double)k);
    while (nm < jan21_jd - 0.5) {
        k++;
        nm = approx_new_moon((double)k);
    }

    return floor(nm) + 0.5; /* Snap to midnight UT */
}

/* ===== Sexagenary cycle from Gregorian year ===== */

static void compute_sexagenary(int year, int *stem, int *branch, int *element,
                               int *polarity, int *cycle_year)
{
    int offset = year - CYCLE_EPOCH;
    *stem       = pos_mod(offset, 10);
    *branch     = pos_mod(offset, 12);
    *element    = *stem / 2;
    *polarity   = *stem % 2;
    *cycle_year = pos_mod(offset, 60) + 1;
}

/* ===== Public API ===== */

int korean_dangun_year(int gregorian_year)
{
    return gregorian_year + KOREAN_DANGUN_OFFSET;
}

korean_date_t korean_from_jd(double jd)
{
    korean_date_t d;
    int gy, gm, gd;
    jd_to_gregorian_internal(jd, &gy, &gm, &gd);

    /* Determine Korean/lunar year: if JD is before this year's Seollal,
     * the Korean year is the previous Gregorian year. */
    double seollal = korean_seollal_jd(gy);
    int korean_gy = gy;
    if (jd < seollal - 0.5) {
        korean_gy = gy - 1;
        seollal = korean_seollal_jd(korean_gy);
    }

    d.gregorian_year = korean_gy;
    d.dangun_year = korean_gy + KOREAN_DANGUN_OFFSET;

    /* Sexagenary cycle based on Korean year */
    compute_sexagenary(korean_gy, &d.stem, &d.branch, &d.element,
                       &d.polarity, &d.cycle_year);
    d.animal = d.branch;

    /* Compute approximate lunar month and day */
    double prev_nm = new_moon_before(jd);
    double lunar_day_frac = jd - floor(prev_nm);
    d.lunar_day = (int)floor(lunar_day_frac) + 1;
    if (d.lunar_day < 1) d.lunar_day = 1;
    if (d.lunar_day > 30) d.lunar_day = 30;

    /* Lunar month: count new moons from Seollal */
    double k_seollal = (seollal - NEW_MOON_REF_JD) / SYNODIC_MONTH;
    double k_now = (prev_nm - NEW_MOON_REF_JD) / SYNODIC_MONTH;
    int month_offset = (int)round(k_now - k_seollal);
    d.lunar_month = month_offset + 1;
    if (d.lunar_month < 1) d.lunar_month = 1;
    if (d.lunar_month > 12) d.lunar_month = 12;

    return d;
}

double korean_to_jd(int gregorian_year, int lunar_month, int lunar_day)
{
    /* Start from Seollal (lunar month 1, day 1) of the given year */
    double seollal = korean_seollal_jd(gregorian_year);

    /* Advance by (lunar_month - 1) synodic months */
    double k_seollal = (seollal - NEW_MOON_REF_JD) / SYNODIC_MONTH;
    int k_base = (int)round(k_seollal);
    int target_k = k_base + (lunar_month - 1);

    double nm = approx_new_moon((double)target_k);

    /* Add (lunar_day - 1) days from the new moon */
    return floor(nm) + 0.5 + (lunar_day - 1);
}

const korean_festival_t *korean_festival(int lunar_month, int lunar_day)
{
    for (int i = 0; i < KOREAN_FESTIVAL_COUNT; i++) {
        if (FESTIVALS[i].lunar_month == lunar_month &&
            FESTIVALS[i].lunar_day == lunar_day) {
            return &FESTIVALS[i];
        }
    }
    return NULL;
}

const char *korean_stem_name(int stem)
{
    if (stem < 0 || stem > 9) return "?";
    return STEM_NAMES[stem];
}

const char *korean_branch_name(int branch)
{
    if (branch < 0 || branch > 11) return "?";
    return BRANCH_NAMES[branch];
}

const char *korean_animal_name(int animal)
{
    if (animal < 0 || animal > 11) return "?";
    return ANIMAL_NAMES[animal];
}

const char *korean_element_name(int element)
{
    if (element < 0 || element > 4) return "?";
    return ELEMENT_NAMES[element];
}

const char *korean_polarity_name(int polarity)
{
    if (polarity < 0 || polarity > 1) return "?";
    return polarity == 0 ? "Yang" : "Yin";
}

const char *korean_sexagenary_name(int cycle_year)
{
    if (cycle_year < 1 || cycle_year > 60) return "?";
    return SEXAGENARY_NAMES[cycle_year - 1];
}

int korean_festival_count(void)
{
    return KOREAN_FESTIVAL_COUNT;
}

korean_festival_t korean_festival_by_index(int index)
{
    if (index < 0 || index >= KOREAN_FESTIVAL_COUNT) {
        korean_festival_t invalid = { -1, "?", "?", "?", 0, 0 };
        return invalid;
    }
    return FESTIVALS[index];
}
