/* lunar_harmonics.c -- Unified lunar harmonics (Track 54.2)
 *
 * Bridges astronomy/lunar.h and 5 lunar calendar systems.
 * Adds Chinese Xiu (28 lodges) and Arabic Manzil (28 mansions).
 * Pure functions, no malloc, no globals, no side effects. */

#include "lunar_harmonics.h"
#include <math.h>

#define PI 3.14159265358979323846

/* ===================================================================
 * Brown Lunation Number reference
 * Lunation 1 = first new moon of 1923
 * Reference new moon: Jan 17, 1923 ~JD 2423436.6
 * =================================================================== */

static const double BLN_EPOCH_JD = 2423436.6;

/* Reference new moon used by lunar.c (2000-01-06 18:14 UT) */
static const double REF_NEW_MOON_JD = 2451550.26;

/* ===================================================================
 * Chinese Xiu (28 Lodges)
 *
 * Traditional Chinese lunar lodges. The Horn (Jiao) is at ~202 deg
 * ecliptic (near Spica, ~0 deg Libra). Unequal widths based on
 * historical determinative star positions.
 * =================================================================== */

typedef struct {
    const char *name;
    const char *meaning;
    double start_deg;
    double span_deg;
} xiu_data_t;

static const xiu_data_t XIU_DATA[28] = {
    /* Eastern Azure Dragon (7 lodges) */
    {"Jiao",  "Horn",        202.0, 12.0},
    {"Kang",  "Neck",        214.0, 9.0},
    {"Di",    "Root",        223.0, 16.0},
    {"Fang",  "Room",        239.0, 5.0},
    {"Xin",   "Heart",       244.0, 5.0},
    {"Wei",   "Tail",        249.0, 18.0},
    {"Ji",    "Winnowing",   267.0, 11.0},
    /* Northern Black Tortoise (7 lodges) */
    {"Dou",   "Dipper",      278.0, 26.0},
    {"Niu",   "Ox",          304.0, 8.0},
    {"Nv",    "Girl",        312.0, 12.0},
    {"Xu",    "Emptiness",   324.0, 10.0},
    {"Wei2",  "Rooftop",     334.0, 17.0},
    {"Shi",   "Encampment",  351.0, 16.0},
    {"Bi",    "Wall",          7.0, 9.0},
    /* Western White Tiger (7 lodges) */
    {"Kui",   "Legs",         16.0, 16.0},
    {"Lou",   "Bond",         32.0, 12.0},
    {"Wei3",  "Stomach",      44.0, 14.0},
    {"Mao",   "Hairy Head",   58.0, 11.0},
    {"Bi2",   "Net",          69.0, 17.0},
    {"Zi",    "Turtle Beak",  86.0, 2.0},
    {"Shen",  "Three Stars",  88.0, 9.0},
    /* Southern Vermilion Bird (7 lodges) */
    {"Jing",  "Well",         97.0, 33.0},
    {"Gui",   "Ghost",       130.0, 4.0},
    {"Liu",   "Willow",      134.0, 15.0},
    {"Xing",  "Star",        149.0, 7.0},
    {"Zhang", "Extended Net", 156.0, 18.0},
    {"Yi",    "Wings",       174.0, 18.0},
    {"Zhen",  "Chariot",     192.0, 10.0}
};

/* ===================================================================
 * Arabic Manzil (28 Mansions)
 *
 * Traditional Arabian lunar mansions. Al-Sharatain starts near 0 deg
 * Aries. Equal-width divisions: 360/28 = 12.857... degrees each.
 * =================================================================== */

typedef struct {
    const char *name;
    const char *meaning;
} manzil_name_t;

static const manzil_name_t MANZIL_NAMES[28] = {
    {"Al-Sharatain",  "The Two Signs"},
    {"Al-Butain",     "The Little Belly"},
    {"Al-Thurayya",   "The Many Little Ones"},
    {"Al-Dabaran",    "The Follower"},
    {"Al-Haqa",       "The White Spot"},
    {"Al-Hana",       "The Mark"},
    {"Al-Dhira",      "The Forearm"},
    {"Al-Nathrah",    "The Gap"},
    {"Al-Tarf",       "The Glance"},
    {"Al-Jabhah",     "The Forehead"},
    {"Al-Zubrah",     "The Mane"},
    {"Al-Sarfah",     "The Changer"},
    {"Al-Awwa",       "The Barker"},
    {"Al-Simak",      "The Unarmed"},
    {"Al-Ghafr",      "The Cover"},
    {"Al-Zubana",     "The Claws"},
    {"Al-Iklil",      "The Crown"},
    {"Al-Qalb",       "The Heart"},
    {"Al-Shaulah",    "The Raised Tail"},
    {"Al-Naaim",      "The Ostriches"},
    {"Al-Baldah",     "The City"},
    {"Al-Sad-Dhabih", "Luck of the Slaughterer"},
    {"Al-Sad-Bula",   "Luck of the Swallower"},
    {"Al-Sad-Suud",   "Luck of Lucks"},
    {"Al-Sad-Akhbiyah","Luck of the Tents"},
    {"Al-Fargh-Awwal","The First Spout"},
    {"Al-Fargh-Thani","The Second Spout"},
    {"Al-Risha",      "The Rope"}
};

static const double MANZIL_SPAN = 360.0 / 28.0; /* ~12.857 degrees */

/* ===================================================================
 * Internal helpers
 * =================================================================== */

static double normalize_angle(double deg)
{
    double r = fmod(deg, 360.0);
    if (r < 0.0) r += 360.0;
    return r;
}

/* Target phase fraction for each of the 8 phases */
static double target_fraction(moon_phase_t phase)
{
    switch (phase) {
        case MOON_NEW:              return 0.0;
        case MOON_WAXING_CRESCENT:  return 0.125;
        case MOON_FIRST_QUARTER:    return 0.25;
        case MOON_WAXING_GIBBOUS:   return 0.375;
        case MOON_FULL:             return 0.5;
        case MOON_WANING_GIBBOUS:   return 0.625;
        case MOON_LAST_QUARTER:     return 0.75;
        case MOON_WANING_CRESCENT:  return 0.875;
    }
    return 0.0;
}

/* Find JD of previous new moon from given JD */
static double prev_new_moon_jd(double jd)
{
    double days_since = jd - REF_NEW_MOON_JD;
    double cycles = days_since / SYNODIC_MONTH_DAYS;
    double cycle_floor = floor(cycles);
    double prev_jd = REF_NEW_MOON_JD + cycle_floor * SYNODIC_MONTH_DAYS;
    if (prev_jd > jd) {
        prev_jd -= SYNODIC_MONTH_DAYS;
    }
    return prev_jd;
}

/* ===================================================================
 * Chinese Xiu
 * =================================================================== */

static lunar_mansion_t make_invalid_mansion(void)
{
    lunar_mansion_t m;
    m.id = -1;
    m.name = "?";
    m.meaning = "?";
    m.start_deg = 0.0;
    m.span_deg = 0.0;
    return m;
}

static lunar_mansion_t xiu_to_mansion(int i)
{
    lunar_mansion_t m;
    m.id = i;
    m.name = XIU_DATA[i].name;
    m.meaning = XIU_DATA[i].meaning;
    m.start_deg = XIU_DATA[i].start_deg;
    m.span_deg = XIU_DATA[i].span_deg;
    return m;
}

int lunar_xiu_count(void)
{
    return XIU_COUNT;
}

lunar_mansion_t lunar_xiu_get(int index)
{
    if (index < 0 || index >= XIU_COUNT) {
        return make_invalid_mansion();
    }
    return xiu_to_mansion(index);
}

const char *lunar_xiu_name(int index)
{
    if (index < 0 || index >= XIU_COUNT) return "?";
    return XIU_DATA[index].name;
}

lunar_mansion_t lunar_xiu_from_longitude(double longitude)
{
    double lon = normalize_angle(longitude);

    for (int i = 0; i < XIU_COUNT; i++) {
        double start = XIU_DATA[i].start_deg;
        double end = start + XIU_DATA[i].span_deg;

        if (end > 360.0) {
            /* Wraps around 0 */
            if (lon >= start || lon < (end - 360.0)) {
                return xiu_to_mansion(i);
            }
        } else {
            if (lon >= start && lon < end) {
                return xiu_to_mansion(i);
            }
        }
    }

    /* Fallback: should not happen if data covers 360 deg */
    return xiu_to_mansion(0);
}

/* ===================================================================
 * Arabic Manzil
 * =================================================================== */

static lunar_mansion_t manzil_to_mansion(int i)
{
    lunar_mansion_t m;
    m.id = i;
    m.name = MANZIL_NAMES[i].name;
    m.meaning = MANZIL_NAMES[i].meaning;
    m.start_deg = i * MANZIL_SPAN;
    m.span_deg = MANZIL_SPAN;
    return m;
}

int lunar_manzil_count(void)
{
    return MANZIL_COUNT;
}

lunar_mansion_t lunar_manzil_get(int index)
{
    if (index < 0 || index >= MANZIL_COUNT) {
        return make_invalid_mansion();
    }
    return manzil_to_mansion(index);
}

const char *lunar_manzil_name(int index)
{
    if (index < 0 || index >= MANZIL_COUNT) return "?";
    return MANZIL_NAMES[index].name;
}

lunar_mansion_t lunar_manzil_from_longitude(double longitude)
{
    double lon = normalize_angle(longitude);
    int index = (int)(lon / MANZIL_SPAN);
    if (index >= MANZIL_COUNT) index = MANZIL_COUNT - 1;
    return manzil_to_mansion(index);
}

/* ===================================================================
 * Brown Lunation Number
 * =================================================================== */

int lunar_lunation_number(double jd)
{
    double days_since_epoch = jd - BLN_EPOCH_JD;
    double cycles = days_since_epoch / SYNODIC_MONTH_DAYS;

    /* Lunation 1 starts at epoch. Before epoch is <= 0 */
    int n = (int)floor(cycles) + 1;
    return n;
}

/* ===================================================================
 * Lunation context
 * =================================================================== */

lunation_t lunar_lunation(double jd)
{
    lunation_t lun;

    double new_jd = prev_new_moon_jd(jd);
    double next_jd = new_jd + SYNODIC_MONTH_DAYS;
    double full_jd = new_jd + SYNODIC_MONTH_DAYS * 0.5;

    lun.new_moon_jd = new_jd;
    lun.next_new_moon_jd = next_jd;
    lun.full_moon_jd = full_jd;
    lun.lunation_number = lunar_lunation_number(jd);
    lun.progress = (jd - new_jd) / (next_jd - new_jd);
    if (lun.progress < 0.0) lun.progress = 0.0;
    if (lun.progress > 1.0) lun.progress = 1.0;

    return lun;
}

/* ===================================================================
 * Next/previous phase of any type
 * =================================================================== */

double lunar_next_phase_of(double jd, moon_phase_t target)
{
    double tf = target_fraction(target);

    /* Get current phase fraction */
    lunar_info_t info = lunar_phase(jd);
    double current = info.phase_fraction;

    /* How far forward (in fraction) to the target? */
    double delta = tf - current;
    if (delta <= 0.0) {
        delta += 1.0; /* wrap to next cycle */
    }

    /* Convert fraction to days */
    double days_forward = delta * SYNODIC_MONTH_DAYS;
    double estimate = jd + days_forward;

    /* Refine: iterate a few times to get closer */
    for (int iter = 0; iter < 5; iter++) {
        lunar_info_t check = lunar_phase(estimate);
        double err = tf - check.phase_fraction;

        /* Handle wrapping near 0/1 boundary */
        if (err > 0.5) err -= 1.0;
        if (err < -0.5) err += 1.0;

        double correction = err * SYNODIC_MONTH_DAYS;
        estimate += correction;

        if (fabs(correction) < 0.001) break;
    }

    /* Ensure result is in the future */
    if (estimate <= jd) {
        estimate += SYNODIC_MONTH_DAYS;
    }

    return estimate;
}

double lunar_prev_phase_of(double jd, moon_phase_t target)
{
    double tf = target_fraction(target);

    lunar_info_t info = lunar_phase(jd);
    double current = info.phase_fraction;

    /* How far backward (in fraction) to the target? */
    double delta = current - tf;
    if (delta <= 0.0) {
        delta += 1.0;
    }

    double days_backward = delta * SYNODIC_MONTH_DAYS;
    double estimate = jd - days_backward;

    /* Refine */
    for (int iter = 0; iter < 5; iter++) {
        lunar_info_t check = lunar_phase(estimate);
        double err = tf - check.phase_fraction;

        if (err > 0.5) err -= 1.0;
        if (err < -0.5) err += 1.0;

        double correction = err * SYNODIC_MONTH_DAYS;
        estimate += correction;

        if (fabs(correction) < 0.001) break;
    }

    /* Ensure result is in the past */
    if (estimate >= jd) {
        estimate -= SYNODIC_MONTH_DAYS;
    }

    return estimate;
}

/* ===================================================================
 * System names
 * =================================================================== */

const char *lunar_system_name(lunar_system_id_t system)
{
    switch (system) {
        case LUNAR_SYS_HEBREW:   return "Hebrew";
        case LUNAR_SYS_ISLAMIC:  return "Islamic";
        case LUNAR_SYS_CHINESE:  return "Chinese";
        case LUNAR_SYS_BUDDHIST: return "Buddhist";
        case LUNAR_SYS_HINDU:    return "Hindu";
    }
    return "?";
}

/* ===================================================================
 * Cross-calendar lunar month status
 *
 * Uses lunar phase fraction to approximate day-in-month.
 * - Hebrew/Chinese/Buddhist: month starts near new moon (phase ~0.0)
 * - Islamic: month starts near first visibility (phase ~0.04)
 * - Hindu (Amant): month starts near new moon (phase ~0.0)
 * =================================================================== */

/* Phase fraction at which each system's month begins */
static double month_start_phase(lunar_system_id_t system)
{
    switch (system) {
        case LUNAR_SYS_HEBREW:   return 0.0;    /* New moon */
        case LUNAR_SYS_ISLAMIC:  return 0.04;   /* First visibility */
        case LUNAR_SYS_CHINESE:  return 0.0;    /* New moon */
        case LUNAR_SYS_BUDDHIST: return 0.0;    /* New moon */
        case LUNAR_SYS_HINDU:    return 0.0;    /* Amant (new moon) */
    }
    return 0.0;
}

/* Typical month length for each system */
static int typical_month_length(lunar_system_id_t system)
{
    switch (system) {
        case LUNAR_SYS_HEBREW:   return 30; /* alternates 29/30 */
        case LUNAR_SYS_ISLAMIC:  return 30; /* alternates 29/30 */
        case LUNAR_SYS_CHINESE:  return 29; /* alternates 29/30 */
        case LUNAR_SYS_BUDDHIST: return 30; /* varies */
        case LUNAR_SYS_HINDU:    return 30; /* varies */
    }
    return 30;
}

lunar_month_status_t lunar_month_status(double jd, lunar_system_id_t system)
{
    lunar_month_status_t status;
    status.system = system;
    status.system_name = lunar_system_name(system);
    status.phase_at_month_start = month_start_phase(system);
    status.month_length = typical_month_length(system);

    /* Get current phase fraction */
    lunar_info_t info = lunar_phase(jd);
    double current_phase = info.phase_fraction;

    /* How far past month start are we? */
    double phase_since_start = current_phase - status.phase_at_month_start;
    if (phase_since_start < 0.0) {
        phase_since_start += 1.0;
    }

    /* Convert phase progress to day in month */
    double day_frac = phase_since_start * SYNODIC_MONTH_DAYS;
    int day = (int)floor(day_frac) + 1;
    if (day < 1) day = 1;
    if (day > status.month_length) day = status.month_length;

    status.approx_day_in_month = day;

    return status;
}

void lunar_month_status_all(double jd, lunar_month_status_t out[LUNAR_SYSTEM_COUNT])
{
    for (int i = 0; i < LUNAR_SYSTEM_COUNT; i++) {
        out[i] = lunar_month_status(jd, (lunar_system_id_t)i);
    }
}

/* ===================================================================
 * Alignment detection
 * =================================================================== */

int lunar_systems_aligned(double jd, double tolerance_days)
{
    int count = 0;
    double tolerance_phase = tolerance_days / SYNODIC_MONTH_DAYS;

    for (int i = 0; i < LUNAR_SYSTEM_COUNT; i++) {
        lunar_system_id_t sys = (lunar_system_id_t)i;
        double start_phase = month_start_phase(sys);

        /* Get current phase */
        lunar_info_t info = lunar_phase(jd);
        double current = info.phase_fraction;

        /* Distance from month-start phase */
        double dist = fabs(current - start_phase);
        if (dist > 0.5) dist = 1.0 - dist;

        if (dist <= tolerance_phase) {
            count++;
        }
    }

    return count;
}
