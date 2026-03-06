#include "location_presets.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static const location_preset_t PRESETS[] = {
    {"Greenwich",     "UK",          51.4769,    -0.0005,    0.0},
    {"New York",      "USA",         40.7128,   -74.0060,   -5.0},
    {"Los Angeles",   "USA",         34.0522,  -118.2437,   -8.0},
    {"Chicago",       "USA",         41.8781,   -87.6298,   -6.0},
    {"London",        "UK",          51.5074,    -0.1278,    0.0},
    {"Paris",         "France",      48.8566,     2.3522,    1.0},
    {"Berlin",        "Germany",     52.5200,    13.4050,    1.0},
    {"Rome",          "Italy",       41.9028,    12.4964,    1.0},
    {"Moscow",        "Russia",      55.7558,    37.6173,    3.0},
    {"Cairo",         "Egypt",       30.0444,    31.2357,    2.0},
    {"Istanbul",      "Turkey",      41.0082,    28.9784,    3.0},
    {"Dubai",         "UAE",         25.2048,    55.2708,    4.0},
    {"Mumbai",        "India",       19.0760,    72.8777,    5.5},
    {"Delhi",         "India",       28.7041,    77.1025,    5.5},
    {"Bangkok",       "Thailand",    13.7563,   100.5018,    7.0},
    {"Beijing",       "China",       39.9042,   116.4074,    8.0},
    {"Shanghai",      "China",       31.2304,   121.4737,    8.0},
    {"Tokyo",         "Japan",       35.6762,   139.6503,    9.0},
    {"Seoul",         "South Korea", 37.5665,   126.9780,    9.0},
    {"Sydney",        "Australia",  -33.8688,   151.2093,   10.0},
    {"Auckland",      "New Zealand",-36.8485,   174.7633,   12.0},
    {"Mexico City",   "Mexico",      19.4326,   -99.1332,   -6.0},
    {"Sao Paulo",     "Brazil",     -23.5505,   -46.6333,   -3.0},
    {"Buenos Aires",  "Argentina",  -34.6037,   -58.3816,   -3.0},
    {"Cape Town",     "South Africa",-33.9249,   18.4241,    2.0},
    {"Jerusalem",     "Israel",      31.7683,    35.2137,    2.0},
    {"Mecca",         "Saudi Arabia",21.3891,    39.8579,    3.0},
    {"Cusco",         "Peru",       -13.5320,   -71.9675,   -5.0},
    {"Kathmandu",     "Nepal",       27.7172,    85.3240,    5.75},
    {"Reykjavik",     "Iceland",     64.1466,   -21.9426,    0.0},
};

#define PRESET_COUNT (int)(sizeof(PRESETS) / sizeof(PRESETS[0]))

/* Case-insensitive character comparison */
static char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z') return (char)(c + ('a' - 'A'));
    return c;
}

/* Case-insensitive substring search */
static int ci_strstr(const char *haystack, const char *needle)
{
    if (!haystack || !needle) return 0;
    size_t nlen = strlen(needle);
    size_t hlen = strlen(haystack);
    if (nlen > hlen) return 0;

    for (size_t i = 0; i <= hlen - nlen; i++) {
        size_t j;
        for (j = 0; j < nlen; j++) {
            if (to_lower(haystack[i + j]) != to_lower(needle[j])) break;
        }
        if (j == nlen) return 1;
    }
    return 0;
}

int location_preset_count(void)
{
    return PRESET_COUNT;
}

location_preset_t location_preset_get(int index)
{
    if (index < 0 || index >= PRESET_COUNT) {
        location_preset_t empty = {NULL, NULL, 0.0, 0.0, 0.0};
        return empty;
    }
    return PRESETS[index];
}

int location_preset_find(const char *query)
{
    if (!query || query[0] == '\0') return -1;

    for (int i = 0; i < PRESET_COUNT; i++) {
        if (ci_strstr(PRESETS[i].name, query)) return i;
    }
    /* Also search country names */
    for (int i = 0; i < PRESET_COUNT; i++) {
        if (ci_strstr(PRESETS[i].country, query)) return i;
    }
    return -1;
}

int location_preset_default(void)
{
    return 0; /* Greenwich */
}

void location_format_coords(double latitude, double longitude, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    char ns = (latitude >= 0.0) ? 'N' : 'S';
    char ew = (longitude >= 0.0) ? 'E' : 'W';

    snprintf(buf, sz, "%.2f%c%c, %.2f%c%c",
             fabs(latitude), 0xC2, ns == 'N' ? 0xB0 : 0xB0, /* degree sign */
             fabs(longitude), 0xC2, ew == 'E' ? 0xB0 : 0xB0);

    /* Simpler approach: use degree symbol as ° */
    snprintf(buf, sz, "%.2f\xc2\xb0%c, %.2f\xc2\xb0%c",
             fabs(latitude), ns, fabs(longitude), ew);
}

void location_format_full(location_preset_t loc, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    const char *name = loc.name ? loc.name : "Unknown";
    const char *country = loc.country ? loc.country : "?";

    char coords[48];
    location_format_coords(loc.latitude, loc.longitude, coords, sizeof(coords));

    snprintf(buf, sz, "%s, %s (%s)", name, country, coords);
}
