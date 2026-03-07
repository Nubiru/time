/* japanese.c -- Japanese Calendar System implementation
 * Imperial era dating, rokuyo, 24 sekki, Japanese zodiac.
 * Pure functions, no malloc, no globals, no side effects.
 *
 * Attribution: Imperial era system codified by the Meiji government (1868).
 * Rokuyo from Chinese fortune-telling via Muromachi period Japan.
 * 24 Sekki solar terms from Chinese agricultural calendar.
 * JD/Gregorian algorithms from Meeus, "Astronomical Algorithms". */

#include "japanese.h"
#include <math.h>

/* ===== Internal JD / Gregorian helpers ===== */

static double gregorian_to_jd(int year, int month, int day)
{
    int y = year;
    int m = month;
    if (m <= 2) {
        y -= 1;
        m += 12;
    }
    int a = y / 100;
    int b = 2 - a + a / 4;
    return floor(365.25 * (y + 4716)) + floor(30.6001 * (m + 1))
           + day + b - 1524.5;
}

static void jd_to_gregorian(double jd, int *year, int *month, int *day)
{
    double z_d = floor(jd + 0.5);
    int z = (int)z_d;
    int a;
    if (z < 2299161) {
        a = z;
    } else {
        int alpha = (int)floor((z_d - 1867216.25) / 36524.25);
        a = z + 1 + alpha - alpha / 4;
    }
    int bb = a + 1524;
    int c = (int)floor((bb - 122.1) / 365.25);
    int d = (int)floor(365.25 * c);
    int e = (int)floor((bb - d) / 30.6001);

    *day = bb - d - (int)floor(30.6001 * e);
    *month = (e < 14) ? e - 1 : e - 13;
    *year = (*month > 2) ? c - 4716 : c - 4715;
}

/* ===== Era boundary JDs (computed at midnight UT) ===== */

/* Pre-computed JD values for era start dates (midnight UT):
 * Meiji:  1868-01-25 = JD 2403356.5
 * Taisho: 1912-07-30 = JD 2419613.5
 * Showa:  1926-12-25 = JD 2424874.5
 * Heisei: 1989-01-08 = JD 2447534.5
 * Reiwa:  2019-05-01 = JD 2458604.5 */

typedef struct {
    int start_year;
    int start_month;
    int start_day;
    double start_jd;
} era_info_t;

static const era_info_t ERA_INFO[JP_ERA_COUNT] = {
    { 1868,  1, 25, 2403356.5 },  /* Meiji */
    { 1912,  7, 30, 2419613.5 },  /* Taisho */
    { 1926, 12, 25, 2424874.5 },  /* Showa */
    { 1989,  1,  8, 2447534.5 },  /* Heisei */
    { 2019,  5,  1, 2458604.5 },  /* Reiwa */
};

static const char *ERA_NAMES[JP_ERA_COUNT] = {
    "Meiji", "Taisho", "Showa", "Heisei", "Reiwa"
};

static const char *ERA_KANJI[JP_ERA_COUNT] = {
    "\xe6\x98\x8e\xe6\xb2\xbb",  /* 明治 */
    "\xe5\xa4\xa7\xe6\xad\xa3",  /* 大正 */
    "\xe6\x98\xad\xe5\x92\x8c",  /* 昭和 */
    "\xe5\xb9\xb3\xe6\x88\x90",  /* 平成 */
    "\xe4\xbb\xa4\xe5\x92\x8c",  /* 令和 */
};

/* ===== Rokuyo data ===== */

static const char *ROKUYO_NAMES[JP_ROKUYO_COUNT] = {
    "Sensho", "Tomobiki", "Senbu", "Butsumetsu", "Taian", "Shakko"
};

static const char *ROKUYO_MEANINGS[JP_ROKUYO_COUNT] = {
    "Win first",       /* Sensho */
    "Draw friend",     /* Tomobiki */
    "Lose first",      /* Senbu */
    "Buddha's death",  /* Butsumetsu */
    "Great peace",     /* Taian */
    "Red mouth",       /* Shakko */
};

/* Synodic month (mean lunation period) */
#define SYNODIC_MONTH 29.53059

/* Reference new moon: 2000-01-06 = JD 2451550.1 */
#define REF_NEW_MOON_JD 2451550.1

/* ===== 24 Sekki data ===== */

static const char *SEKKI_NAMES[24] = {
    "Shunbun",   /* 0:   0 deg - Vernal Equinox */
    "Seimei",    /* 1:  15 deg - Clear and Bright */
    "Kokuu",     /* 2:  30 deg - Grain Rain */
    "Rikka",     /* 3:  45 deg - Start of Summer */
    "Shoman",    /* 4:  60 deg - Grain Buds */
    "Boshu",     /* 5:  75 deg - Grain in Ear */
    "Geshi",     /* 6:  90 deg - Summer Solstice */
    "Shosho",    /* 7: 105 deg - Minor Heat */
    "Taisho",    /* 8: 120 deg - Major Heat */
    "Risshu",    /* 9: 135 deg - Start of Autumn */
    "Shosho",    /* 10: 150 deg - End of Heat */
    "Hakuro",    /* 11: 165 deg - White Dew */
    "Shubun",    /* 12: 180 deg - Autumnal Equinox */
    "Kanro",     /* 13: 195 deg - Cold Dew */
    "Soko",      /* 14: 210 deg - Frost's Descent */
    "Ritto",     /* 15: 225 deg - Start of Winter */
    "Shosetsu",  /* 16: 240 deg - Minor Snow */
    "Taisetsu",  /* 17: 255 deg - Major Snow */
    "Toji",      /* 18: 270 deg - Winter Solstice */
    "Shokan",    /* 19: 285 deg - Minor Cold */
    "Daikan",    /* 20: 300 deg - Major Cold */
    "Risshun",   /* 21: 315 deg - Start of Spring */
    "Usui",      /* 22: 330 deg - Rain Water */
    "Keichitsu", /* 23: 345 deg - Awakening of Insects */
};

static const char *SEKKI_KANJI[24] = {
    "\xe6\x98\xa5\xe5\x88\x86",          /* 春分 */
    "\xe6\xb8\x85\xe6\x98\x8e",          /* 清明 */
    "\xe7\xa9\x80\xe9\x9b\xa8",          /* 穀雨 */
    "\xe7\xab\x8b\xe5\xa4\x8f",          /* 立夏 */
    "\xe5\xb0\x8f\xe6\xba\x80",          /* 小満 */
    "\xe8\x8a\x92\xe7\xa8\xae",          /* 芒種 */
    "\xe5\xa4\x8f\xe8\x87\xb3",          /* 夏至 */
    "\xe5\xb0\x8f\xe6\x9a\x91",          /* 小暑 */
    "\xe5\xa4\xa7\xe6\x9a\x91",          /* 大暑 */
    "\xe7\xab\x8b\xe7\xa7\x8b",          /* 立秋 */
    "\xe5\x87\xa6\xe6\x9a\x91",          /* 処暑 */
    "\xe7\x99\xbd\xe9\x9c\xb2",          /* 白露 */
    "\xe7\xa7\x8b\xe5\x88\x86",          /* 秋分 */
    "\xe5\xaf\x92\xe9\x9c\xb2",          /* 寒露 */
    "\xe9\x9c\x9c\xe9\x99\x8d",          /* 霜降 */
    "\xe7\xab\x8b\xe5\x86\xac",          /* 立冬 */
    "\xe5\xb0\x8f\xe9\x9b\xaa",          /* 小雪 */
    "\xe5\xa4\xa7\xe9\x9b\xaa",          /* 大雪 */
    "\xe5\x86\xac\xe8\x87\xb3",          /* 冬至 */
    "\xe5\xb0\x8f\xe5\xaf\x92",          /* 小寒 */
    "\xe5\xa4\xa7\xe5\xaf\x92",          /* 大寒 */
    "\xe7\xab\x8b\xe6\x98\xa5",          /* 立春 */
    "\xe9\x9b\xa8\xe6\xb0\xb4",          /* 雨水 */
    "\xe5\x95\x93\xe8\x9f\x84",          /* 啓蟄 */
};

/* ===== Japanese Zodiac data ===== */

static const char *ZODIAC_NAMES[12] = {
    "Ne", "Ushi", "Tora", "U", "Tatsu", "Mi",
    "Uma", "Hitsuji", "Saru", "Tori", "Inu", "I"
};

static const char *ZODIAC_KANJI[12] = {
    "\xe5\xad\x90",  /* 子 Rat */
    "\xe4\xb8\x91",  /* 丑 Ox */
    "\xe5\xaf\x85",  /* 寅 Tiger */
    "\xe5\x8d\xaf",  /* 卯 Rabbit */
    "\xe8\xbe\xb0",  /* 辰 Dragon */
    "\xe5\xb7\xb3",  /* 巳 Snake */
    "\xe5\x8d\x88",  /* 午 Horse */
    "\xe6\x9c\xaa",  /* 未 Sheep */
    "\xe7\x94\xb3",  /* 申 Monkey */
    "\xe9\x85\x89",  /* 酉 Rooster */
    "\xe6\x88\x8c",  /* 戌 Dog */
    "\xe4\xba\xa5",  /* 亥 Boar */
};

/* ===== Imperial Era Functions ===== */

static int determine_era(int year, int month, int day)
{
    /* Check from newest to oldest */
    for (int i = JP_ERA_COUNT - 1; i >= 0; i--) {
        int sy = ERA_INFO[i].start_year;
        int sm = ERA_INFO[i].start_month;
        int sd = ERA_INFO[i].start_day;
        if (year > sy) return i;
        if (year == sy) {
            if (month > sm) return i;
            if (month == sm && day >= sd) return i;
        }
    }
    return -1; /* pre-Meiji */
}

japanese_date_t japanese_from_jd(double jd)
{
    japanese_date_t result;
    int y, m, d;
    jd_to_gregorian(jd, &y, &m, &d);

    result.greg_year = y;
    result.month = m;
    result.day = d;
    result.era_index = determine_era(y, m, d);

    if (result.era_index >= 0) {
        result.era_year = y - ERA_INFO[result.era_index].start_year + 1;
    } else {
        result.era_year = 0;
    }

    return result;
}

double japanese_to_jd(japanese_date_t date)
{
    return gregorian_to_jd(date.greg_year, date.month, date.day);
}

const char *japanese_era_name(int era_index)
{
    if (era_index < 0 || era_index >= JP_ERA_COUNT) return "?";
    return ERA_NAMES[era_index];
}

const char *japanese_era_kanji(int era_index)
{
    if (era_index < 0 || era_index >= JP_ERA_COUNT) return "?";
    return ERA_KANJI[era_index];
}

int japanese_era_start_year(int era_index)
{
    if (era_index < 0 || era_index >= JP_ERA_COUNT) return -1;
    return ERA_INFO[era_index].start_year;
}

bool japanese_is_meiji_or_later(double jd)
{
    return jd >= ERA_INFO[JP_ERA_MEIJI].start_jd;
}

/* ===== Rokuyo Functions ===== */

jp_rokuyo_t japanese_rokuyo(double jd)
{
    /* Approximate lunar month and day from JD using synodic month.
     * Reference: new moon on 2000-01-06 = JD 2451550.1
     * From any JD, compute days since reference new moon,
     * derive approximate lunar month and day. */
    double days_since_ref = jd - REF_NEW_MOON_JD;
    double lunations = days_since_ref / SYNODIC_MONTH;

    /* Normalize lunations to positive */
    double lunar_month_frac = lunations - floor(lunations);
    int approx_lunar_day = (int)(lunar_month_frac * SYNODIC_MONTH) + 1;

    /* Approximate lunar month number (use integer part of lunations).
     * We only need (lunar_month + lunar_day) % 6, so the absolute
     * month number doesn't matter as long as it's consistent. */
    int approx_lunar_month = (int)floor(lunations);
    /* Make sure month is positive for modulo */
    if (approx_lunar_month < 0) {
        /* Shift to positive: find equivalent positive value mod 6 */
        approx_lunar_month = approx_lunar_month % 6;
        if (approx_lunar_month < 0) approx_lunar_month += 6;
    }

    int r = (approx_lunar_month + approx_lunar_day) % 6;
    if (r < 0) r += 6;
    return (jp_rokuyo_t)r;
}

const char *japanese_rokuyo_name(jp_rokuyo_t r)
{
    if (r < 0 || r >= JP_ROKUYO_COUNT) return "?";
    return ROKUYO_NAMES[r];
}

const char *japanese_rokuyo_meaning(jp_rokuyo_t r)
{
    if (r < 0 || r >= JP_ROKUYO_COUNT) return "?";
    return ROKUYO_MEANINGS[r];
}

/* ===== 24 Sekki Functions ===== */

int japanese_sekki(double solar_longitude_deg)
{
    /* Normalize to [0, 360) */
    double lon = fmod(solar_longitude_deg, 360.0);
    if (lon < 0.0) lon += 360.0;
    return (int)(lon / 15.0) % 24;
}

const char *japanese_sekki_name(int index)
{
    if (index < 0 || index >= 24) return "?";
    return SEKKI_NAMES[index];
}

const char *japanese_sekki_kanji(int index)
{
    if (index < 0 || index >= 24) return "?";
    return SEKKI_KANJI[index];
}

double japanese_sekki_longitude(int index)
{
    if (index < 0 || index >= 24) return -1.0;
    return (double)(index * 15);
}

/* ===== Japanese Zodiac Functions ===== */

int japanese_zodiac_animal(int year)
{
    int r = (year - 4) % 12;
    if (r < 0) r += 12;
    return r;
}

const char *japanese_zodiac_name(int animal)
{
    if (animal < 0 || animal >= 12) return "?";
    return ZODIAC_NAMES[animal];
}

const char *japanese_zodiac_kanji(int animal)
{
    if (animal < 0 || animal >= 12) return "?";
    return ZODIAC_KANJI[animal];
}
