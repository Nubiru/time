/* convergence.c -- Cross-Calendar Convergence Detector
 *
 * Standalone implementation with inline calendar approximations.
 * No external calendar module dependencies. Pure functions only. */

#include "convergence.h"
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

/* Synodic month (new moon to new moon) in days */
static const double SYNODIC_MONTH = 29.530589;

/* Reference new moon epoch (JD 2451550.1 ~ 2000-01-06) */
static const double NEW_MOON_EPOCH = 2451550.1;

/* Tzolkin anchor: JD 2452002.5 = kin index 0 (Red Magnetic Dragon) */
static const double TZOLKIN_EPOCH = 2452002.5;

/* Hijri epoch: JD 1948439.5 = 1 Muharram year 1 (622 CE July 16 Julian) */
static const double HIJRI_EPOCH = 1948439.5;

/* ===== Name tables ===== */

static const char *SYSTEM_NAMES[] = {
    "Gregorian", "Hebrew", "Islamic", "Chinese", "Tzolkin",
    "Haab", "I Ching", "Buddhist", "Hindu", "Lunar", "Zodiac"
};

static const char *SIGNAL_TYPE_NAMES[] = {
    "New Year", "New Month", "Holy Day", "Cycle Start",
    "Full Moon", "New Moon", "Solstice", "Sacred Number",
    "Tone One", "Sabbath"
};

const char *conv_system_name(conv_system_t sys)
{
    if (sys < 0 || sys >= CONV_SYS_COUNT) return "?";
    return SYSTEM_NAMES[sys];
}

const char *conv_signal_type_name(conv_signal_type_t type)
{
    if (type < 0 || type >= CONV_SIG_COUNT) return "?";
    return SIGNAL_TYPE_NAMES[type];
}

/* ===== Inline calendar helpers ===== */

/* JD to Gregorian (Meeus algorithm) */
static void jd_to_greg(double jd, int *year, int *month, int *day)
{
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

    double day_frac = (b - d) - (int)floor(30.6001 * e) + f;
    *day = (int)day_frac;

    if (e < 14) {
        *month = e - 1;
    } else {
        *month = e - 13;
    }

    if (*month > 2) {
        *year = c - 4716;
    } else {
        *year = c - 4715;
    }
}

/* Day of week from JD: 0=Mon, 1=Tue, ... 5=Sat, 6=Sun */
static int dow_from_jd(double jd)
{
    long day_num = (long)floor(jd + 0.5);
    int dow = (int)(day_num % 7);
    if (dow < 0) dow += 7;
    return dow;
}

/* Lunar phase fraction: 0.0 = new moon, 0.5 = full moon */
static double lunar_phase_frac(double jd)
{
    double phase = fmod(jd - NEW_MOON_EPOCH, SYNODIC_MONTH);
    if (phase < 0.0) phase += SYNODIC_MONTH;
    return phase / SYNODIC_MONTH;
}

/* Simplified solar longitude (degrees) from JD.
 * Rough approximation sufficient for zodiac sign detection. */
static double solar_longitude(double jd)
{
    /* J2000.0 = JD 2451545.0 */
    double d = jd - 2451545.0;
    /* Mean longitude of the sun */
    double l = fmod(280.460 + 0.9856474 * d, 360.0);
    if (l < 0.0) l += 360.0;
    /* Mean anomaly */
    double g = fmod(357.528 + 0.9856003 * d, 360.0);
    if (g < 0.0) g += 360.0;
    double g_rad = g * PI / 180.0;
    /* Ecliptic longitude */
    double lon = l + 1.915 * sin(g_rad) + 0.020 * sin(2.0 * g_rad);
    lon = fmod(lon, 360.0);
    if (lon < 0.0) lon += 360.0;
    return lon;
}

/* Tabular Hijri date from JD (Dershowitz & Reingold arithmetic calendar) */
static void hijri_from_jd_inline(double jd, int *year, int *month, int *day)
{
    /* Days from Hijri epoch */
    int j = (int)floor(jd - HIJRI_EPOCH + 0.5);
    if (j < 0) {
        *year = 1;
        *month = 1;
        *day = 1;
        return;
    }
    /* 30-year cycle: 10631 days per 30-year cycle */
    int cycle_num = j / 10631;
    int remainder = j % 10631;
    /* Year within cycle (approximate) */
    int y = (int)((30.0 * remainder + 15.0) / 10631.0);
    /* Days in years 1..y within cycle */
    int days_in_years = (int)((11.0 * y + 3.0) / 30.0) + 354 * y;
    if (remainder < days_in_years) {
        y--;
        days_in_years = (int)((11.0 * y + 3.0) / 30.0) + 354 * y;
    }
    int day_of_year = remainder - days_in_years;
    /* Month (odd months have 30 days, even have 29; month 12 has 30 in leap) */
    int m = (int)((11.0 * day_of_year + 10.0) / 325.0);
    if (m > 11) m = 11;
    int days_in_months = (int)((325.0 * m - 10.0) / 11.0);
    if (day_of_year < days_in_months) {
        m--;
        days_in_months = (int)((325.0 * m - 10.0) / 11.0);
    }

    *year = cycle_num * 30 + y + 1;
    *month = m + 1;
    *day = day_of_year - days_in_months + 1;
}

/* Approximate Hebrew date from JD.
 * Full Hebrew calendar algorithm is complex; we approximate for signal detection.
 * Returns month (7=Tishrei for Rosh Hashanah) and day. */
static void hebrew_from_jd_approx(double jd, int *month, int *day)
{
    /* Hebrew calendar epoch: 1 Tishrei 1 AM = JD 347995.5 */
    /* Average Hebrew year = 365.2468 days */
    /* For convergence detection, we approximate using the Gregorian date
     * and offset to Hebrew month/day. Tishrei 1 (Rosh Hashanah) typically
     * falls in September/October. */

    /* Simple approximation: use the Gregorian month/day to estimate Hebrew month/day */
    int gy, gm, gd;
    jd_to_greg(jd, &gy, &gm, &gd);

    /* Map Gregorian months to approximate Hebrew months.
     * This is intentionally rough — just for convergence detection.
     * Hebrew months don't align with Gregorian, but this gives us
     * approximate Rosh Chodesh (new month) and holiday detection. */

    /* Offset: Hebrew year starts ~Sep/Oct, month 7 = Tishrei
     * We compute days since an approximate Tishrei 1 epoch */

    /* Better approach: use a known anchor and the 19-year Metonic cycle */
    /* Anchor: 1 Tishrei 5784 = Sep 16, 2023 = JD 2460203.5 */
    static const double ANCHOR_JD = 2460203.5;
    static const double AVG_HEBREW_YEAR = 365.24682;
    static const double AVG_HEBREW_MONTH = 29.530594;

    double days_from_anchor = jd - ANCHOR_JD;
    /* Approximate day within Hebrew year */
    double day_in_year = fmod(days_from_anchor, AVG_HEBREW_YEAR);
    if (day_in_year < 0.0) day_in_year += AVG_HEBREW_YEAR;

    /* Approximate month (Hebrew months ~29.5 days, starting from Tishrei) */
    int approx_month_offset = (int)(day_in_year / AVG_HEBREW_MONTH);
    if (approx_month_offset > 12) approx_month_offset = 12;

    /* Hebrew month numbering: Tishrei=7 (civil new year), Nisan=1 */
    /* For detection, we use 7-based offset: month 7 + offset, wrapping at 13 */
    int hmonth = 7 + approx_month_offset;
    if (hmonth > 13) hmonth -= 13;

    /* Approximate day in month */
    double day_in_month = fmod(day_in_year - approx_month_offset * AVG_HEBREW_MONTH,
                               AVG_HEBREW_MONTH);
    if (day_in_month < 0.0) day_in_month += AVG_HEBREW_MONTH;
    int hday = (int)day_in_month + 1;
    if (hday > 30) hday = 30;

    *month = hmonth;
    *day = hday;
}

/* ===== Signal helper: add a signal if room ===== */

static int add_signal(conv_signal_t *out, int count, int max,
                      conv_system_t sys, conv_signal_type_t type,
                      const char *desc, int weight)
{
    if (count >= max) return count;
    out[count].system = sys;
    out[count].type = type;
    out[count].description = desc;
    out[count].weight = weight;
    return count + 1;
}

/* ===== Per-system check functions ===== */

static int check_gregorian(double jd, conv_signal_t *out, int max)
{
    int n = 0;
    int y, m, d;
    jd_to_greg(jd, &y, &m, &d);

    /* New Year: Jan 1 */
    if (m == 1 && d == 1) {
        n = add_signal(out, n, max, CONV_SYS_GREGORIAN,
                       CONV_SIG_NEW_YEAR, "Gregorian New Year", 3);
    }

    /* New Month: day 1 (but not if already counted as New Year) */
    if (d == 1 && !(m == 1)) {
        n = add_signal(out, n, max, CONV_SYS_GREGORIAN,
                       CONV_SIG_NEW_MONTH, "First of month", 1);
    }

    /* Solstice/Equinox dates (approximate) */
    if ((m == 3 && (d >= 19 && d <= 21)) ||
        (m == 6 && (d >= 20 && d <= 22)) ||
        (m == 9 && (d >= 21 && d <= 24)) ||
        (m == 12 && (d >= 20 && d <= 23))) {
        n = add_signal(out, n, max, CONV_SYS_GREGORIAN,
                       CONV_SIG_SOLSTICE, "Solstice/Equinox", 2);
    }

    return n;
}

static int check_hebrew(double jd, conv_signal_t *out, int max)
{
    int n = 0;
    int hmonth, hday;
    hebrew_from_jd_approx(jd, &hmonth, &hday);

    /* Rosh Hashanah: Tishrei 1 (month 7, day 1) */
    if (hmonth == 7 && hday == 1) {
        n = add_signal(out, n, max, CONV_SYS_HEBREW,
                       CONV_SIG_NEW_YEAR, "Rosh Hashanah", 3);
    }

    /* New Month (Rosh Chodesh): day 1 */
    if (hday == 1 && !(hmonth == 7)) {
        n = add_signal(out, n, max, CONV_SYS_HEBREW,
                       CONV_SIG_NEW_MONTH, "Rosh Chodesh", 1);
    }

    /* Yom Kippur: Tishrei 10 */
    if (hmonth == 7 && hday == 10) {
        n = add_signal(out, n, max, CONV_SYS_HEBREW,
                       CONV_SIG_HOLY_DAY, "Yom Kippur", 2);
    }

    /* Passover: Nisan 15 (month 1) */
    if (hmonth == 1 && hday == 15) {
        n = add_signal(out, n, max, CONV_SYS_HEBREW,
                       CONV_SIG_HOLY_DAY, "Passover", 2);
    }

    /* Sabbath: Saturday (dow 5 in our 0=Mon scheme) */
    int dow = dow_from_jd(jd);
    if (dow == 5) {
        n = add_signal(out, n, max, CONV_SYS_HEBREW,
                       CONV_SIG_SABBATH, "Shabbat", 1);
    }

    return n;
}

static int check_islamic(double jd, conv_signal_t *out, int max)
{
    int n = 0;
    int hy, hm, hd;
    hijri_from_jd_inline(jd, &hy, &hm, &hd);

    /* Islamic New Year: 1 Muharram */
    if (hm == 1 && hd == 1) {
        n = add_signal(out, n, max, CONV_SYS_ISLAMIC,
                       CONV_SIG_NEW_YEAR, "Islamic New Year", 3);
    }

    /* New Month: day 1 (not if already New Year) */
    if (hd == 1 && hm != 1) {
        n = add_signal(out, n, max, CONV_SYS_ISLAMIC,
                       CONV_SIG_NEW_MONTH, "New Islamic month", 1);
    }

    /* Ramadan start: month 9, day 1 */
    if (hm == 9 && hd == 1) {
        n = add_signal(out, n, max, CONV_SYS_ISLAMIC,
                       CONV_SIG_HOLY_DAY, "Ramadan begins", 2);
    }

    /* Eid ul-Fitr: month 10, day 1 */
    if (hm == 10 && hd == 1) {
        n = add_signal(out, n, max, CONV_SYS_ISLAMIC,
                       CONV_SIG_HOLY_DAY, "Eid ul-Fitr", 2);
    }

    /* Ashura: Muharram 10 */
    if (hm == 1 && hd == 10) {
        n = add_signal(out, n, max, CONV_SYS_ISLAMIC,
                       CONV_SIG_HOLY_DAY, "Ashura", 2);
    }

    return n;
}

static int check_chinese(double jd, conv_signal_t *out, int max)
{
    int n = 0;
    int y, m, d;
    jd_to_greg(jd, &y, &m, &d);

    /* Chinese New Year: approximate range Jan 21 - Feb 21 */
    /* More precisely, we check if this is a new moon in that window */
    if ((m == 1 && d >= 21) || (m == 2 && d <= 21)) {
        /* Check if near a new moon (phase fraction near 0) */
        double phase = lunar_phase_frac(jd);
        if (phase < 0.02 || phase > 0.98) {
            n = add_signal(out, n, max, CONV_SYS_CHINESE,
                           CONV_SIG_NEW_YEAR, "Chinese New Year (approx)", 3);
        }
    }

    /* Chinese new month: any new moon */
    {
        double phase = lunar_phase_frac(jd);
        if (phase < 0.02 || phase > 0.98) {
            /* Only add if not already added as New Year */
            if (n == 0) {
                n = add_signal(out, n, max, CONV_SYS_CHINESE,
                               CONV_SIG_NEW_MONTH, "Chinese new month (approx)", 1);
            }
        }
    }

    return n;
}

static int check_tzolkin(double jd, conv_signal_t *out, int max)
{
    int n = 0;

    /* Kin index 0-259 */
    int kin_idx = ((int)floor(jd - TZOLKIN_EPOCH) % 260 + 260) % 260;
    int tone = kin_idx % 13 + 1;  /* 1-13 */

    /* Cycle start: kin index 0 = Kin 1 (Red Magnetic Dragon) */
    if (kin_idx == 0) {
        n = add_signal(out, n, max, CONV_SYS_TZOLKIN,
                       CONV_SIG_CYCLE_START, "Tzolkin cycle start (Kin 1)", 3);
    }

    /* Tone 1 (wavespell start) */
    if (tone == 1) {
        n = add_signal(out, n, max, CONV_SYS_TZOLKIN,
                       CONV_SIG_TONE_ONE, "Wavespell start (Tone 1)", 1);
    }

    return n;
}

static int check_haab(double jd, conv_signal_t *out, int max)
{
    int n = 0;

    /* Day in Haab cycle (0-364). Offset 65 aligns with Dreamspell anchor. */
    int day_in_haab = ((int)floor(jd - TZOLKIN_EPOCH + 65) % 365 + 365) % 365;
    int haab_month = day_in_haab / 20;  /* 0-18 */
    int haab_day = day_in_haab % 20;

    /* New Year: 0 Pop (month 0, day 0) */
    if (haab_month == 0 && haab_day == 0) {
        n = add_signal(out, n, max, CONV_SYS_HAAB,
                       CONV_SIG_NEW_YEAR, "Haab New Year (0 Pop)", 3);
    }

    /* Wayeb: month 18 (the 5-day unlucky period, days 360-364) */
    if (haab_month == 18) {
        n = add_signal(out, n, max, CONV_SYS_HAAB,
                       CONV_SIG_HOLY_DAY, "Wayeb (5 nameless days)", 2);
    }

    return n;
}

static int check_iching(double jd, conv_signal_t *out, int max)
{
    int n = 0;

    /* I Ching hexagram cycles through 64.
     * hexagram index 0 = King Wen 1 (Creative) */
    int hex_idx = ((int)floor(jd) % 64 + 64) % 64;
    int king_wen = hex_idx + 1;

    /* Hexagram 1 (Creative) or 2 (Receptive) */
    if (king_wen == 1) {
        n = add_signal(out, n, max, CONV_SYS_ICHING,
                       CONV_SIG_CYCLE_START, "Hexagram 1: The Creative", 2);
    } else if (king_wen == 2) {
        n = add_signal(out, n, max, CONV_SYS_ICHING,
                       CONV_SIG_SACRED_NUMBER, "Hexagram 2: The Receptive", 2);
    }

    return n;
}

static int check_buddhist(double jd, conv_signal_t *out, int max)
{
    int n = 0;

    /* Buddhist Uposatha: observance on full moon and new moon days */
    double phase = lunar_phase_frac(jd);

    /* Full moon: phase ~ 0.5, tolerance ~0.02 (~0.6 days) */
    if (fabs(phase - 0.5) < 0.02) {
        n = add_signal(out, n, max, CONV_SYS_BUDDHIST,
                       CONV_SIG_FULL_MOON, "Uposatha (Purnima)", 2);
    }

    /* New moon: phase ~ 0.0 or ~1.0 */
    if (phase < 0.02 || phase > 0.98) {
        n = add_signal(out, n, max, CONV_SYS_BUDDHIST,
                       CONV_SIG_NEW_MOON, "Uposatha (Amavasya)", 2);
    }

    return n;
}

static int check_hindu(double jd, conv_signal_t *out, int max)
{
    int n = 0;

    double phase = lunar_phase_frac(jd);

    /* Approximate tithi from lunar phase: 30 tithis per synodic month */
    int tithi = (int)(phase * 30.0) + 1;
    if (tithi > 30) tithi = 30;

    /* Purnima: full moon (tithi 15 in Shukla, i.e. phase ~0.5) */
    if (fabs(phase - 0.5) < 0.02) {
        n = add_signal(out, n, max, CONV_SYS_HINDU,
                       CONV_SIG_FULL_MOON, "Purnima (full moon)", 2);
    }

    /* Amavasya: new moon (tithi 30/new) */
    if (phase < 0.02 || phase > 0.98) {
        n = add_signal(out, n, max, CONV_SYS_HINDU,
                       CONV_SIG_NEW_MOON, "Amavasya (new moon)", 1);
    }

    /* Pratipada: tithi 1 (first tithi after new moon) */
    if (tithi == 1) {
        n = add_signal(out, n, max, CONV_SYS_HINDU,
                       CONV_SIG_SACRED_NUMBER, "Pratipada (tithi 1)", 1);
    }

    return n;
}

static int check_lunar(double jd, conv_signal_t *out, int max)
{
    int n = 0;

    double phase = lunar_phase_frac(jd);

    /* Full Moon: phase ~ 0.5 */
    if (fabs(phase - 0.5) < 0.02) {
        n = add_signal(out, n, max, CONV_SYS_LUNAR,
                       CONV_SIG_FULL_MOON, "Full Moon", 2);
    }

    /* New Moon: phase ~ 0.0 */
    if (phase < 0.02 || phase > 0.98) {
        n = add_signal(out, n, max, CONV_SYS_LUNAR,
                       CONV_SIG_NEW_MOON, "New Moon", 2);
    }

    return n;
}

static int check_zodiac(double jd, conv_signal_t *out, int max)
{
    int n = 0;

    double lon = solar_longitude(jd);
    /* Degree within current sign (0-30) */
    double deg_in_sign = fmod(lon, 30.0);

    /* Sign ingress: near 0 degrees of a sign (within ~1 degree) */
    if (deg_in_sign < 1.0 || deg_in_sign > 29.0) {
        n = add_signal(out, n, max, CONV_SYS_ZODIAC,
                       CONV_SIG_CYCLE_START, "Zodiac sign ingress", 2);
    }

    return n;
}

/* ===== System dispatch ===== */

typedef int (*check_fn_t)(double jd, conv_signal_t *out, int max);

static const check_fn_t CHECK_FUNCTIONS[] = {
    check_gregorian,
    check_hebrew,
    check_islamic,
    check_chinese,
    check_tzolkin,
    check_haab,
    check_iching,
    check_buddhist,
    check_hindu,
    check_lunar,
    check_zodiac
};

int conv_check_system(conv_system_t sys, double jd,
                      conv_signal_t *out, int max_out)
{
    if (sys < 0 || sys >= CONV_SYS_COUNT) return 0;
    if (max_out <= 0) return 0;
    return CHECK_FUNCTIONS[sys](jd, out, max_out);
}

/* ===== Scan date ===== */

conv_scan_t conv_scan_date(double jd)
{
    conv_scan_t scan;
    memset(&scan, 0, sizeof(scan));
    scan.jd = jd;

    int total = 0;
    int sys_flags[CONV_SYS_COUNT] = {0};

    for (int s = 0; s < CONV_SYS_COUNT; s++) {
        int remaining = CONV_MAX_SIGNALS - total;
        if (remaining <= 0) break;
        int n = CHECK_FUNCTIONS[s](jd, &scan.signals[total], remaining);
        if (n > 0) {
            sys_flags[s] = 1;
            total += n;
        }
    }

    scan.signal_count = total;

    /* Count active systems */
    int active = 0;
    for (int s = 0; s < CONV_SYS_COUNT; s++) {
        if (sys_flags[s]) active++;
    }
    scan.systems_active = active;

    /* Sum weights */
    int weight = 0;
    for (int i = 0; i < total; i++) {
        weight += scan.signals[i].weight;
    }
    scan.total_weight = weight;

    /* Convergence: 3+ systems active */
    scan.is_convergence = (active >= 3) ? 1 : 0;

    return scan;
}

/* ===== Range functions ===== */

double conv_best_in_range(double jd_start, double jd_end)
{
    double best_jd = jd_start;
    int best_weight = -1;

    for (double jd = jd_start; jd <= jd_end + 0.1; jd += 1.0) {
        conv_scan_t scan = conv_scan_date(jd);
        if (scan.total_weight > best_weight) {
            best_weight = scan.total_weight;
            best_jd = jd;
        }
    }

    return best_jd;
}

int conv_count_in_range(double jd_start, double jd_end)
{
    int count = 0;
    for (double jd = jd_start; jd <= jd_end + 0.1; jd += 1.0) {
        conv_scan_t scan = conv_scan_date(jd);
        if (scan.is_convergence) count++;
    }
    return count;
}

double conv_next_convergence(double jd, int max_days)
{
    for (int d = 1; d <= max_days; d++) {
        double test_jd = jd + d;
        conv_scan_t scan = conv_scan_date(test_jd);
        if (scan.is_convergence) return test_jd;
    }
    return 0.0;
}

double conv_prev_convergence(double jd, int max_days)
{
    for (int d = 1; d <= max_days; d++) {
        double test_jd = jd - d;
        conv_scan_t scan = conv_scan_date(test_jd);
        if (scan.is_convergence) return test_jd;
    }
    return 0.0;
}
