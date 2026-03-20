/* lao_calendar.c -- Lao Buddhist Calendar implementation
 * Self-contained Gregorian JD math (Meeus, "Astronomical Algorithms" Ch.7).
 * Buddhist Era offset: BE = CE + 543.
 * Nang Sangkhan maidens: seven celestial figures, one per weekday.
 * Lao zodiac: 12-animal cycle with Naga replacing Dragon. */

#include "lao_calendar.h"
#include <math.h>

/* ===== Buddhist Era offset ===== */

static const int BE_OFFSET = 543;

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

    int a;
    if (z < 2299161) {
        a = z;
    } else {
        int alpha = (int)floor((z - 1867216.25) / 36524.25);
        a = z + 1 + alpha - (int)floor((double)alpha / 4.0);
    }

    int b_val = a + 1524;
    int c = (int)floor((b_val - 122.1) / 365.25);
    int d = (int)floor(365.25 * c);
    int e = (int)floor((double)(b_val - d) / 30.6001);

    double day_frac = (b_val - d) - (int)floor(30.6001 * e) + f;

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

/* ===== Leap year (Gregorian rules on CE year) ===== */

static bool is_gregorian_leap(int ce_year)
{
    if (ce_year % 4 != 0) return false;
    if (ce_year % 100 != 0) return true;
    return (ce_year % 400 == 0);
}

/* ===== Lao month names (romanized from Lao script) ===== */

static const char *LAO_MONTH_NAMES[] = {
    "Mangkon",      /* 1  January   (from Makara/Capricorn) */
    "Kumphaa",      /* 2  February  (from Kumbha/Aquarius) */
    "Miinaa",       /* 3  March     (from Mina/Pisces) */
    "Meesaa",       /* 4  April     (from Mesha/Aries) */
    "Pheutsaphaa",  /* 5  May       (from Vrishabha/Taurus) */
    "Mithunaa",     /* 6  June      (from Mithuna/Gemini) */
    "Kawlakot",     /* 7  July      (from Karkata/Cancer) */
    "Singhaa",      /* 8  August    (from Simha/Leo) */
    "Kanyaa",       /* 9  September (from Kanya/Virgo) */
    "Tulaa",        /* 10 October   (from Tula/Libra) */
    "Phachik",      /* 11 November  (from Vrischika/Scorpio) */
    "Thanwaa"       /* 12 December  (from Dhanus/Sagittarius) */
};

static const char *LAO_MONTH_ABBRS[] = {
    "Man",  /* 1  */
    "Kum",  /* 2  */
    "Mii",  /* 3  */
    "Mee",  /* 4  */
    "Phe",  /* 5  */
    "Mit",  /* 6  */
    "Kaw",  /* 7  */
    "Sin",  /* 8  */
    "Kan",  /* 9  */
    "Tul",  /* 10 */
    "Pha",  /* 11 */
    "Thw"   /* 12 */
};

/* ===== Days per month (Gregorian) ===== */

static const int DAYS_IN_MONTH[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/* ===== Day names (romanized from Lao) ===== */

static const char *LAO_DAY_NAMES[] = {
    "Wan Aathit",    /* Sunday    (Aditya/Sun) */
    "Wan Chan",      /* Monday    (Chandra/Moon) */
    "Wan Angkhaan",  /* Tuesday   (Angaraka/Mars) */
    "Wan Phut",      /* Wednesday (Budha/Mercury) */
    "Wan Phahat",    /* Thursday  (Brihaspati/Jupiter) */
    "Wan Suk",       /* Friday    (Shukra/Venus) */
    "Wan Sao"        /* Saturday  (Shani/Saturn) */
};

/* ===== Festival names ===== */

static const char *FESTIVAL_NAMES[] = {
    "",                  /* LAO_FEST_NONE */
    "Pi Mai Lao",        /* LAO_FEST_PI_MAI */
    "National Day",      /* LAO_FEST_NATIONAL_DAY */
    "Liberation Day"     /* LAO_FEST_LIBERATION_DAY */
};

/* ===== Nang Sangkhan: seven celestial maidens ===== */

static const lao_sangkhan_t SANGKHAN_MAIDENS[] = {
    { 0, "Nang Thoungsa Thevi",  "Garuda",         "Discus and conch shell" },
    { 1, "Nang Kholaka Thevi",   "Tiger",          "Sword and walking cane" },
    { 2, "Nang Haksod Thevi",    "Pig",            "Trident and bow" },
    { 3, "Nang Montha Thevi",    "Donkey",         "Needle and walking staff" },
    { 4, "Nang Khilini Thevi",   "Elephant",       "Hook and lyre" },
    { 5, "Nang Khamitha Thevi",  "Water buffalo",  "Dagger and lyre" },
    { 6, "Nang Mahothon Thevi",  "Peacock",        "Throwing disk and trident" }
};

/* ===== Lao zodiac animals (12-year cycle, Naga replaces Dragon) ===== */

static const char *LAO_ZODIAC[] = {
    "Nuu (Rat)",
    "Ngua (Ox)",
    "Seua (Tiger)",
    "Katai (Rabbit)",
    "Nguu Nyai (Naga)",   /* Naga replaces Chinese Dragon */
    "Nguu Noi (Snake)",
    "Maa (Horse)",
    "Phae (Goat)",
    "Ling (Monkey)",
    "Kai (Rooster)",
    "Maa (Dog)",
    "Muu (Pig)"
};

/* ===== Public API ===== */

int lao_be_year(int ce_year)
{
    return ce_year + BE_OFFSET;
}

int lao_ce_year(int be_year)
{
    return be_year - BE_OFFSET;
}

lao_date_t lao_from_jd(double jd)
{
    int ce_year, month, day;
    jd_to_gregorian_internal(jd, &ce_year, &month, &day);

    lao_date_t result;
    result.be_year = ce_year + BE_OFFSET;
    result.month = month;
    result.day = day;
    return result;
}

double lao_to_jd(lao_date_t date)
{
    int ce_year = date.be_year - BE_OFFSET;
    return gregorian_to_jd_internal(ce_year, date.month, (double)date.day);
}

const char *lao_month_name(int month)
{
    if (month < 1 || month > 12) return "?";
    return LAO_MONTH_NAMES[month - 1];
}

const char *lao_month_abbr(int month)
{
    if (month < 1 || month > 12) return "?";
    return LAO_MONTH_ABBRS[month - 1];
}

bool lao_is_pi_mai(lao_date_t date)
{
    return (date.month == 4 && date.day >= 14 && date.day <= 16);
}

lao_festival_t lao_festival(lao_date_t date)
{
    /* Pi Mai: April 14-16 */
    if (date.month == 4 && date.day >= 14 && date.day <= 16) {
        return LAO_FEST_PI_MAI;
    }
    /* National Day: December 2 */
    if (date.month == 12 && date.day == 2) {
        return LAO_FEST_NATIONAL_DAY;
    }
    /* Liberation Day: October 12 */
    if (date.month == 10 && date.day == 12) {
        return LAO_FEST_LIBERATION_DAY;
    }
    return LAO_FEST_NONE;
}

const char *lao_festival_name(lao_festival_t fest)
{
    if (fest < 0 || fest >= LAO_FEST_COUNT) return "?";
    return FESTIVAL_NAMES[fest];
}

int lao_month_days(int be_year, int month)
{
    if (month < 1 || month > 12) return 0;
    int ce_year = be_year - BE_OFFSET;
    if (month == 2 && is_gregorian_leap(ce_year)) {
        return 29;
    }
    return DAYS_IN_MONTH[month - 1];
}

bool lao_is_leap_year(int be_year)
{
    int ce_year = be_year - BE_OFFSET;
    return is_gregorian_leap(ce_year);
}

int lao_day_of_week(lao_date_t date)
{
    double jd = lao_to_jd(date);
    /* JD 0.0 was Monday. (JD + 1.5) mod 7 gives 0=Sunday. */
    int dow = ((int)floor(jd + 1.5)) % 7;
    if (dow < 0) dow += 7;
    return dow;
}

const char *lao_day_name(int weekday)
{
    if (weekday < 0 || weekday > 6) return "?";
    return LAO_DAY_NAMES[weekday];
}

lao_sangkhan_t lao_sangkhan(int be_year)
{
    /* Maiden is determined by weekday of April 14 */
    lao_date_t pi_mai = { .be_year = be_year, .month = 4, .day = 14 };
    int dow = lao_day_of_week(pi_mai);
    return SANGKHAN_MAIDENS[dow];
}

const char *lao_zodiac_animal(int be_year)
{
    /* Same 12-year cycle as Chinese/Thai.
     * BE 2567 = 2024 CE = Dragon/Naga year.
     * 2024 % 12 = 8, which is Dragon position in Chinese cycle.
     * We use: (ce_year - 4) % 12 for standard alignment. */
    int ce_year = be_year - BE_OFFSET;
    int idx = ((ce_year - 4) % 12);
    if (idx < 0) idx += 12;
    return LAO_ZODIAC[idx];
}
