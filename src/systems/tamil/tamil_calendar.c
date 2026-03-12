/* tamil_calendar.c — Tamil sidereal solar calendar implementation
 *
 * Pure arithmetic solar calendar aligned to sidereal zodiac.
 * 12 months matching sidereal signs. 60-year Jovian cycle.
 * Thiruvalluvar era: year 1 = 31 BCE (CE offset = 31).
 *
 * Sidereal longitude: mean tropical Sun minus Lahiri ayanamsa.
 * Puthandu approximation: ~JD 2451649.0 for CE 2000, sidereal year 365.25636d.
 *
 * Sources: Meeus "Astronomical Algorithms" (mean solar longitude),
 *          Lahiri ayanamsa formula, traditional Tamil panchangam. */

#include "tamil_calendar.h"
#include <math.h>

#define PI 3.14159265358979323846

/* J2000.0 epoch */
static const double J2000 = 2451545.0;

/* Approximate JD of Puthandu (Sun enters sidereal Aries) for CE 2000 */
static const double PUTHANDU_JD_2000 = 2451649.0;

/* Sidereal year in days */
static const double SIDEREAL_YEAR = 365.25636;

/* Thiruvalluvar era offset: TV = CE + 31 */
static const int TV_OFFSET = 31;

/* Jovian cycle reference: TV 2018 (1987 CE) = Prabhava (year 1) */
static const int JOVIAN_REF_TV = 2018;

static const char *MONTH_NAMES[] = {
    "Chittirai", "Vaigasi", "Aani", "Aadi", "Avani", "Purattasi",
    "Aippasi", "Karthigai", "Margazhi", "Thai", "Maasi", "Panguni"
};

static const char *MONTH_MEANINGS[] = {
    "Beautiful/Brilliant", "Brightness", "Twins", "Beginning/First",
    "Earth", "Dew/Cloud", "Deepening", "Pleiades",
    "Path", "Auspicious", "Measure", "Flowering"
};

/* Average month lengths (solar, approximate).
 * Total = 365. Panguni gets the extra day for common year. */
static const int MONTH_DAYS[] = {
    31, 31, 32, 31, 31, 30, 30, 29, 30, 29, 30, 31
};

/* Cumulative days at start of each month (0-indexed by month-1).
 * CUMULATIVE[0] = 0 (Chittirai starts at day 0 from Puthandu).
 * CUMULATIVE[i] = sum of MONTH_DAYS[0..i-1]. */
static const int CUMULATIVE[] = {
    0, 31, 62, 94, 125, 156, 186, 216, 245, 275, 304, 334
};

static const char *FESTIVAL_NAMES[] = {
    "",                     /* TAMIL_FEST_NONE */
    "Puthandu",             /* TAMIL_FEST_PUTHANDU */
    "Thai Pongal",          /* TAMIL_FEST_PONGAL */
    "Chithirai Thiruvizha", /* TAMIL_FEST_CHITHIRAI_FEST */
    "Deepavali"             /* TAMIL_FEST_DEEPAVALI */
};

static const char *JOVIAN_NAMES[] = {
    "Prabhava", "Vibhava", "Shukla", "Pramodoota", "Prajothpatti",
    "Angirasa", "Srimukha", "Bhava", "Yuva", "Dhatu",
    "Eeshvara", "Bahudhanya", "Pramathi", "Vikrama", "Vishu",
    "Chitrabhanu", "Subhanu", "Tharana", "Parthiva", "Vyaya",
    "Sarvajith", "Sarvadhari", "Virodhi", "Vikrithi", "Khara",
    "Nandana", "Vijaya", "Jaya", "Manmatha", "Durmukhi",
    "Hevilambi", "Vilambi", "Vikari", "Sharvari", "Plava",
    "Shubhakrithu", "Sobhakrithu", "Krodhi", "Vishvavasu", "Parabhava",
    "Plavanga", "Keelaka", "Saumya", "Sadharana", "Virodhikrithu",
    "Parithaapi", "Pramadeecha", "Ananda", "Rakshasa", "Nala",
    "Pingala", "Kalayukthi", "Siddharthi", "Raudri", "Durmathi",
    "Dundubhi", "Rudhirodgari", "Raktakshi", "Krodhana", "Akshaya"
};

/* Normalize angle to [0, 360) */
static double normalize_deg(double deg)
{
    double result = fmod(deg, 360.0);
    if (result < 0.0) {
        result += 360.0;
    }
    return result;
}

double tamil_sidereal_sun(double jd)
{
    /* Julian centuries from J2000.0 */
    double T = (jd - J2000) / 36525.0;

    /* Mean tropical solar longitude (Meeus, simplified) */
    double L = 280.46646 + 36000.76983 * T;

    /* Equation of center (first-order approximation) */
    double M = 357.52911 + 35999.05029 * T; /* mean anomaly */
    double M_rad = normalize_deg(M) * PI / 180.0;
    double C = 1.9146 * sin(M_rad) + 0.02 * sin(2.0 * M_rad);
    double sun_lon = L + C;

    /* Lahiri ayanamsa (approximate) */
    double years_from_j2000 = (jd - J2000) / 365.25;
    double ayanamsa = 23.85 + 0.01396 * years_from_j2000;

    /* Sidereal longitude */
    double sidereal = sun_lon - ayanamsa;
    return normalize_deg(sidereal);
}

/* Approximate JD of Puthandu for a given CE year */
static double puthandu_jd(int ce_year)
{
    return PUTHANDU_JD_2000 + (ce_year - 2000) * SIDEREAL_YEAR;
}

tamil_date_t tamil_from_jd(double jd)
{
    tamil_date_t result;

    /* Estimate CE year from JD.
     * J2000 = Jan 1.5, 2000. Approximate CE year. */
    int ce_approx = 2000 + (int)floor((jd - J2000) / 365.25);

    /* Find the Puthandu that starts the current Tamil year */
    double py = puthandu_jd(ce_approx);

    /* If jd is before this year's Puthandu, go back one year */
    if (jd < py) {
        ce_approx--;
        py = puthandu_jd(ce_approx);
    }

    /* If jd is past next year's Puthandu, go forward */
    double py_next = puthandu_jd(ce_approx + 1);
    if (jd >= py_next) {
        ce_approx++;
        py = py_next;
    }

    result.year = ce_approx + TV_OFFSET;

    /* Day offset from Puthandu */
    int day_offset = (int)floor(jd - py);

    /* Find month from cumulative days */
    result.month = 12; /* default to last month */
    for (int m = 11; m >= 0; m--) {
        if (day_offset >= CUMULATIVE[m]) {
            result.month = m + 1;
            break;
        }
    }

    result.day = day_offset - CUMULATIVE[result.month - 1] + 1;

    /* Clamp day to valid range */
    if (result.day < 1) {
        result.day = 1;
    }

    result.jovian = tamil_jovian_year(result.year);

    return result;
}

double tamil_to_jd(tamil_date_t date)
{
    int ce_year = date.year - TV_OFFSET;
    double py = puthandu_jd(ce_year);

    /* Add cumulative days for completed months + day offset */
    int month_idx = date.month - 1;
    if (month_idx < 0) month_idx = 0;
    if (month_idx > 11) month_idx = 11;

    int days = CUMULATIVE[month_idx] + (date.day - 1);
    return py + days;
}

const char *tamil_month_name(int month)
{
    if (month < 1 || month > TAMIL_MONTHS) {
        return "?";
    }
    return MONTH_NAMES[month - 1];
}

const char *tamil_month_meaning(int month)
{
    if (month < 1 || month > TAMIL_MONTHS) {
        return "?";
    }
    return MONTH_MEANINGS[month - 1];
}

int tamil_month_days(int month)
{
    if (month < 1 || month > TAMIL_MONTHS) {
        return 0;
    }
    return MONTH_DAYS[month - 1];
}

const char *tamil_jovian_name(int jovian)
{
    if (jovian < 1 || jovian > TAMIL_JOVIAN_CYCLE) {
        return "?";
    }
    return JOVIAN_NAMES[jovian - 1];
}

int tamil_jovian_year(int thiruvalluvar_year)
{
    int offset = thiruvalluvar_year - JOVIAN_REF_TV;
    /* Proper modulo for negative values */
    int result = ((offset % TAMIL_JOVIAN_CYCLE) + TAMIL_JOVIAN_CYCLE) % TAMIL_JOVIAN_CYCLE + 1;
    return result;
}

int tamil_thiruvalluvar_from_ce(int ce_year)
{
    return ce_year + TV_OFFSET;
}

int tamil_ce_from_thiruvalluvar(int tv_year)
{
    return tv_year - TV_OFFSET;
}

tamil_festival_t tamil_festival(tamil_date_t date)
{
    int m = date.month;
    int d = date.day;

    /* Puthandu: Tamil New Year (Chittirai 1) */
    if (m == 1 && d == 1) {
        return TAMIL_FEST_PUTHANDU;
    }

    /* Chithirai Thiruvizha: full moon proxy in Chittirai (day 15) */
    if (m == 1 && d == 15) {
        return TAMIL_FEST_CHITHIRAI_FEST;
    }

    /* Deepavali: new moon proxy in Aippasi (day 15) */
    if (m == 7 && d == 15) {
        return TAMIL_FEST_DEEPAVALI;
    }

    /* Thai Pongal: Sun enters sidereal Capricorn (Thai 1) */
    if (m == 10 && d == 1) {
        return TAMIL_FEST_PONGAL;
    }

    return TAMIL_FEST_NONE;
}

const char *tamil_festival_name(tamil_festival_t fest)
{
    if (fest < 0 || fest >= TAMIL_FEST_COUNT) {
        return "?";
    }
    return FESTIVAL_NAMES[fest];
}
