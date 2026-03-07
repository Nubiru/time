#include "aztec.h"

#include <math.h>

/* GMT correlation constant — Mayan Long Count 0.0.0.0.0 = JDN 584283.
 * On this date: Tonalpohualli = 4 Xochitl, Xiuhpohualli = Izcalli day 8. */
static const long GMT_EPOCH = 584283L;

/* Xiuhpohualli year day (1-based) on the GMT epoch.
 * Month 18 (Izcalli) day 8 = 17*20 + 8 = 348. */
static const int EPOCH_YEAR_DAY = 348;

/* Calendar Round length: LCM(260, 365) = 18,980 days = 52 years. */
static const int CALENDAR_ROUND = 18980;

/* The 4 year bearer day-sign indices (within the 20 day signs). */
static const int BEARER_SIGNS[4] = { 7, 12, 17, 2 };
/* Tochtli=7, Acatl=12, Tecpatl=17, Calli=2 */

static const char *DAY_SIGN_NAMES[20] = {
    "Cipactli",      "Ehecatl",       "Calli",         "Cuetzpalin",
    "Coatl",         "Miquiztli",     "Mazatl",        "Tochtli",
    "Atl",           "Itzcuintli",    "Ozomatli",      "Malinalli",
    "Acatl",         "Ocelotl",       "Cuauhtli",      "Cozcacuauhtli",
    "Ollin",         "Tecpatl",       "Quiahuitl",     "Xochitl"
};

static const char *DAY_SIGN_MEANINGS[20] = {
    "Crocodile",     "Wind",          "House",         "Lizard",
    "Serpent",       "Death",         "Deer",          "Rabbit",
    "Water",         "Dog",           "Monkey",        "Grass",
    "Reed",          "Jaguar",        "Eagle",         "Vulture",
    "Movement",      "Flint",         "Rain",          "Flower"
};

/* Month names: index 0 = Nemontemi, 1-18 = regular months. */
static const char *MONTH_NAMES[19] = {
    "Nemontemi",
    "Atlcahualo",       "Tlacaxipehualiztli", "Tozoztontli",
    "Hueytozoztli",     "Toxcatl",            "Etzalcualiztli",
    "Tecuilhuitontli",  "Hueytecuilhuitl",    "Tlaxochimaco",
    "Xocotlhuetzi",     "Ochpaniztli",        "Teotleco",
    "Tepeilhuitl",      "Quecholli",          "Panquetzaliztli",
    "Atemoztli",        "Tititl",             "Izcalli"
};

static const char *MONTH_MEANINGS[19] = {
    "Useless Days",
    "Cessation of Water",    "Flaying of Men",       "Short Vigil",
    "Great Vigil",           "Drought",              "Eating of Maize and Beans",
    "Small Feast of Lords",  "Great Feast of Lords",  "Gift of Flowers",
    "Fall of Fruits",        "Sweeping",              "Arrival of Gods",
    "Feast of Mountains",    "Precious Feather",      "Raising of Banners",
    "Descent of Water",      "Stretching",            "Growth"
};

static const char *YEAR_BEARER_NAMES[4] = {
    "Tochtli", "Acatl", "Tecpatl", "Calli"
};

/* Safe modulo that always returns non-negative for positive divisor. */
static int mod_pos(long a, int m)
{
    int r = (int)(a % m);
    if (r < 0) r += m;
    return r;
}

/* Convert JD to Julian Day Number (integer day number). */
static long jd_to_day_num(double jd)
{
    return (long)floor(jd + 0.5);
}

/* ======================================================================== */

aztec_tonalpohualli_t aztec_tonalpohualli(double jd)
{
    aztec_tonalpohualli_t result;
    long day_num = jd_to_day_num(jd);
    long offset = day_num - GMT_EPOCH;

    /* On the epoch (offset=0): sign=19 (Xochitl), number=4.
     * sign = (offset + 19) % 20
     * number = ((offset + 3) % 13) + 1 */
    result.day_sign = mod_pos(offset + 19, 20);
    result.day_number = mod_pos(offset + 3, 13) + 1;

    /* day_index: position 1-260 in the Tonalpohualli cycle.
     * 1 Cipactli = day_index 1. On epoch: 4 Xochitl = day_index 160.
     * offset 0 → day_index 160, so day_index = ((offset + 159) % 260) + 1 */
    result.day_index = mod_pos(offset + 159, 260) + 1;

    return result;
}

const char *aztec_day_sign_name(int sign)
{
    if (sign < 0 || sign > 19) return "?";
    return DAY_SIGN_NAMES[sign];
}

const char *aztec_day_sign_meaning(int sign)
{
    if (sign < 0 || sign > 19) return "?";
    return DAY_SIGN_MEANINGS[sign];
}

/* ======================================================================== */

aztec_xiuhpohualli_t aztec_xiuhpohualli(double jd)
{
    aztec_xiuhpohualli_t result;
    long day_num = jd_to_day_num(jd);
    long offset = day_num - GMT_EPOCH;

    /* year_day: 1-based position in the 365-day cycle.
     * On epoch (offset=0), year_day = EPOCH_YEAR_DAY (348).
     * year_day = ((offset + EPOCH_YEAR_DAY - 1) % 365) + 1 */
    int yd = mod_pos(offset + EPOCH_YEAR_DAY - 1, 365) + 1;
    result.year_day = yd;

    if (yd <= 360) {
        /* Regular months 1-18, each 20 days */
        result.month = ((yd - 1) / 20) + 1;
        result.day = ((yd - 1) % 20) + 1;
    } else {
        /* Nemontemi: days 361-365 */
        result.month = 0;
        result.day = yd - 360;
    }

    return result;
}

const char *aztec_month_name(int month)
{
    if (month < 0 || month > 18) return "?";
    return MONTH_NAMES[month];
}

const char *aztec_month_meaning(int month)
{
    if (month < 0 || month > 18) return "?";
    return MONTH_MEANINGS[month];
}

int aztec_month_days(int month)
{
    if (month < 0 || month > 18) return 0;
    if (month == 0) return 5;
    return 20;
}

/* ======================================================================== */

/* Find the offset (from GMT_EPOCH) of the new year for the Aztec year
 * containing the given offset. New year = year_day 1 = Atlcahualo day 1. */
static long new_year_offset(long offset)
{
    int yd_0based = mod_pos(offset + EPOCH_YEAR_DAY - 1, 365);
    return offset - yd_0based;
}

/* Map a day sign index to year bearer index (0-3), or -1 if not a bearer. */
static int sign_to_bearer(int sign)
{
    for (int i = 0; i < 4; i++) {
        if (BEARER_SIGNS[i] == sign) return i;
    }
    return -1;
}

aztec_calendar_round_t aztec_calendar_round(double jd)
{
    aztec_calendar_round_t result;
    long day_num = jd_to_day_num(jd);
    long offset = day_num - GMT_EPOCH;

    /* Find the new year of the current Aztec year */
    long ny_off = new_year_offset(offset);

    /* Tonalpohualli sign and number on new year's day */
    int ny_sign = mod_pos(ny_off + 19, 20);
    int ny_number = mod_pos(ny_off + 3, 13) + 1;

    /* Map sign to year bearer index */
    result.year_bearer = sign_to_bearer(ny_sign);
    result.year_number = ny_number;

    /* round_year: position 1-52 in the Calendar Round.
     * The 52-year cycle has 4 bearers x 13 numbers.
     * Bearer cycles: 0,1,2,3,0,1,2,3,...
     * Numbers cycle 1-13 for each complete set of 4 bearers.
     * round_year = bearer * 13 + (number - 1) + 1 */
    result.round_year = result.year_bearer * 13 + (result.year_number - 1) + 1;

    return result;
}

const char *aztec_year_bearer_name(int bearer)
{
    if (bearer < 0 || bearer > 3) return "?";
    return YEAR_BEARER_NAMES[bearer];
}

/* ======================================================================== */

bool aztec_is_new_fire(double jd)
{
    long day_num = jd_to_day_num(jd);
    long offset = day_num - GMT_EPOCH;

    /* Must be a new year day (year_day == 1) */
    int yd_0based = mod_pos(offset + EPOCH_YEAR_DAY - 1, 365);
    if (yd_0based != 0) return false;

    /* Must be on a Calendar Round boundary (every 18980 days from a
     * reference new year). The first new year after the epoch is at
     * offset = 365 - EPOCH_YEAR_DAY + 1 = 18.
     * New Fire at offset 18 + n*18980. */
    long ny_off = new_year_offset(offset);
    long ref = 365L - EPOCH_YEAR_DAY + 1;  /* = 18 */
    long diff = ny_off - ref;
    return mod_pos(diff, CALENDAR_ROUND) == 0;
}

int aztec_days_to_new_fire(double jd)
{
    long day_num = jd_to_day_num(jd);
    long offset = day_num - GMT_EPOCH;

    /* Reference: first New Fire after epoch at offset 18 */
    long ref = 365L - EPOCH_YEAR_DAY + 1;  /* = 18 */

    /* Position within the Calendar Round cycle (from a New Fire reference) */
    int pos = mod_pos(offset - ref, CALENDAR_ROUND);

    /* Days remaining = (CALENDAR_ROUND - pos) % CALENDAR_ROUND */
    if (pos == 0) return 0;
    return CALENDAR_ROUND - pos;
}

int aztec_calendar_round_days(void)
{
    return CALENDAR_ROUND;
}
