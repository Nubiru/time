#include "zodiac.h"
#include <math.h>

static const char *SIGN_NAMES[] = {
    "Aries", "Taurus", "Gemini", "Cancer",
    "Leo", "Virgo", "Libra", "Scorpio",
    "Sagittarius", "Capricorn", "Aquarius", "Pisces"
};

static const char *SIGN_SYMBOLS[] = {
    "\u2648", "\u2649", "\u264A", "\u264B",
    "\u264C", "\u264D", "\u264E", "\u264F",
    "\u2650", "\u2651", "\u2652", "\u2653"
};

static double normalize_lon(double lon) {
    lon = fmod(lon, 360.0);
    if (lon < 0.0) lon += 360.0;
    return lon;
}

int zodiac_sign(double longitude) {
    return (int)floor(normalize_lon(longitude) / 30.0) % 12;
}

const char *zodiac_sign_name(int sign) {
    if (sign < 0 || sign > 11) return "?";
    return SIGN_NAMES[sign];
}

double zodiac_degree(double longitude) {
    return fmod(normalize_lon(longitude), 30.0);
}

const char *zodiac_sign_symbol(int sign) {
    if (sign < 0 || sign > 11) return "?";
    return SIGN_SYMBOLS[sign];
}

int zodiac_decan(double longitude) {
    double deg = zodiac_degree(longitude);
    return (int)floor(deg / 10.0) + 1;
}
