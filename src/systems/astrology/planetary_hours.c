#include "planetary_hours.h"

#include <math.h>

/* Chaldean order: Saturn(0), Jupiter(1), Mars(2), Sun(3), Venus(4), Mercury(5), Moon(6) */

static const int DAY_RULERS[7] = {
    3,  /* Sunday    = Sun     */
    6,  /* Monday    = Moon    */
    2,  /* Tuesday   = Mars    */
    5,  /* Wednesday = Mercury */
    1,  /* Thursday  = Jupiter */
    4,  /* Friday    = Venus   */
    0   /* Saturday  = Saturn  */
};

static const char *PLANET_NAMES[7] = {
    "Saturn", "Jupiter", "Mars", "Sun", "Venus", "Mercury", "Moon"
};

static const char *PLANET_SYMBOLS[7] = {
    "\xe2\x99\x84",  /* Saturn  */
    "\xe2\x99\x83",  /* Jupiter */
    "\xe2\x99\x82",  /* Mars    */
    "\xe2\x98\x89",  /* Sun     */
    "\xe2\x99\x80",  /* Venus   */
    "\xe2\x98\xbf",  /* Mercury */
    "\xe2\x98\xbd"   /* Moon    */
};

static const char *DAY_NAMES[7] = {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
};

int planetary_day_ruler(int day_of_week)
{
    if (day_of_week < 0 || day_of_week > 6)
        return -1;
    return DAY_RULERS[day_of_week];
}

planetary_hour_t planetary_hour(double jd)
{
    planetary_hour_t h;

    int raw = (int)floor(jd + 1.5);
    h.day_of_week = ((raw % 7) + 7) % 7;
    h.day_ruler = DAY_RULERS[h.day_of_week];

    double frac = jd - floor(jd) + 0.5;
    double solar_hour = fmod(frac * 24.0 + 18.0, 24.0);
    h.hour_index = (int)solar_hour;
    if (h.hour_index > 23) h.hour_index = 23;

    h.planet = (h.day_ruler + h.hour_index) % 7;

    return h;
}

const char *planet_chaldean_name(int planet)
{
    if (planet < 0 || planet > 6)
        return "?";
    return PLANET_NAMES[planet];
}

const char *planet_chaldean_symbol(int planet)
{
    if (planet < 0 || planet > 6)
        return "?";
    return PLANET_SYMBOLS[planet];
}

const char *planetary_day_name(int day_of_week)
{
    if (day_of_week < 0 || day_of_week > 6)
        return "?";
    return DAY_NAMES[day_of_week];
}
