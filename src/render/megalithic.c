#include "megalithic.h"
#include "../systems/astronomy/solar_events.h"

#include <math.h>

#define DEG2RAD (PI / 180.0)
#define RAD2DEG (180.0 / PI)

/* Maximum lunar declination at major standstill */
static const double MAX_LUNAR_DEC = MEG_OBLIQUITY + MEG_LUNAR_INCLINATION;

/* ---- Site catalog ---- */

static const meg_site_t sites[] = {
    {
        .name = "Newgrange",
        .location = "Ireland",
        .lat = 53.6947,
        .lon = -6.4754,
        .alignment_az = 137.0,
        .event = MEG_EVENT_WINTER_SOLSTICE_SUNRISE,
        .epoch_bce = 3200,
        .tolerance_deg = 5.0
    },
    {
        .name = "Stonehenge",
        .location = "England",
        .lat = 51.1789,
        .lon = -1.8262,
        .alignment_az = 51.3,
        .event = MEG_EVENT_SUMMER_SOLSTICE_SUNRISE,
        .epoch_bce = 3000,
        .tolerance_deg = 2.0
    },
    {
        .name = "Callanish",
        .location = "Scotland",
        .lat = 58.1976,
        .lon = -6.7459,
        .alignment_az = 204.5,
        .event = MEG_EVENT_LUNAR_STANDSTILL_SOUTH,
        .epoch_bce = 2900,
        .tolerance_deg = 3.0
    },
    {
        .name = "Carnac",
        .location = "France",
        .lat = 47.5837,
        .lon = -3.0809,
        .alignment_az = 52.0,
        .event = MEG_EVENT_SUMMER_SOLSTICE_SUNRISE,
        .epoch_bce = 4500,
        .tolerance_deg = 2.5
    },
    {
        .name = "Mnajdra",
        .location = "Malta",
        .lat = 35.8267,
        .lon = 14.4364,
        .alignment_az = 90.0,
        .event = MEG_EVENT_EQUINOX_SUNRISE,
        .epoch_bce = 3600,
        .tolerance_deg = 1.5
    }
};

static const int SITE_COUNT = (int)(sizeof(sites) / sizeof(sites[0]));

/* ---- Event names ---- */

static const char *event_names[] = {
    "Summer Solstice Sunrise",
    "Summer Solstice Sunset",
    "Winter Solstice Sunrise",
    "Winter Solstice Sunset",
    "Equinox Sunrise",
    "Equinox Sunset",
    "Lunar Standstill North",
    "Lunar Standstill South"
};

/* ---- Public API ---- */

int meg_site_count(void)
{
    return SITE_COUNT;
}

meg_site_t meg_site_get(int index)
{
    if (index < 0 || index >= SITE_COUNT) {
        meg_site_t sentinel = {
            .name = (void *)0,
            .location = (void *)0,
            .lat = 0.0,
            .lon = 0.0,
            .alignment_az = 0.0,
            .event = MEG_EVENT_COUNT,
            .epoch_bce = 0,
            .tolerance_deg = 0.0
        };
        return sentinel;
    }
    return sites[index];
}

const char *meg_event_name(meg_event_t event)
{
    if (event < 0 || event >= MEG_EVENT_COUNT) {
        return "Unknown";
    }
    return event_names[event];
}

double meg_sunrise_azimuth(double lat_deg, double declination_deg)
{
    double lat_rad = lat_deg * DEG2RAD;
    double dec_rad = declination_deg * DEG2RAD;
    double cos_lat = cos(lat_rad);

    /* Avoid division by zero at poles */
    if (fabs(cos_lat) < 1.0e-10) {
        return (declination_deg >= 0.0) ? 0.0 : 180.0;
    }

    double ratio = sin(dec_rad) / cos_lat;

    /* Clamp for polar cases */
    if (ratio > 1.0) {
        return 0.0;   /* midnight sun — sun never sets */
    }
    if (ratio < -1.0) {
        return 180.0;  /* polar night — sun never rises */
    }

    return acos(ratio) * RAD2DEG;
}

double meg_sunset_azimuth(double lat_deg, double declination_deg)
{
    return 360.0 - meg_sunrise_azimuth(lat_deg, declination_deg);
}

double meg_lunar_standstill_azimuth(double lat_deg, int north)
{
    if (north) {
        /* Northern standstill: moonrise at positive max declination */
        return meg_sunrise_azimuth(lat_deg, MAX_LUNAR_DEC);
    } else {
        /* Southern standstill: moonset at negative max declination */
        return meg_sunset_azimuth(lat_deg, -MAX_LUNAR_DEC);
    }
}

double meg_solstice_declination(int summer)
{
    return summer ? MEG_OBLIQUITY : -MEG_OBLIQUITY;
}

double meg_equinox_declination(void)
{
    return 0.0;
}

/* Compute the event azimuth for a given site and declination */
static double compute_event_az(meg_event_t event, double lat, double dec)
{
    switch (event) {
        case MEG_EVENT_SUMMER_SOLSTICE_SUNRISE:
        case MEG_EVENT_WINTER_SOLSTICE_SUNRISE:
        case MEG_EVENT_EQUINOX_SUNRISE:
            return meg_sunrise_azimuth(lat, dec);

        case MEG_EVENT_SUMMER_SOLSTICE_SUNSET:
        case MEG_EVENT_WINTER_SOLSTICE_SUNSET:
        case MEG_EVENT_EQUINOX_SUNSET:
            return meg_sunset_azimuth(lat, dec);

        case MEG_EVENT_LUNAR_STANDSTILL_NORTH:
            return meg_lunar_standstill_azimuth(lat, 1);

        case MEG_EVENT_LUNAR_STANDSTILL_SOUTH:
            return meg_lunar_standstill_azimuth(lat, 0);

        default:
            return -1.0;
    }
}

/* Get declination for a solar event using actual JD, or fixed for lunar */
static double get_event_declination(meg_event_t event, double jd)
{
    switch (event) {
        case MEG_EVENT_SUMMER_SOLSTICE_SUNRISE:
        case MEG_EVENT_SUMMER_SOLSTICE_SUNSET:
        case MEG_EVENT_WINTER_SOLSTICE_SUNRISE:
        case MEG_EVENT_WINTER_SOLSTICE_SUNSET:
        case MEG_EVENT_EQUINOX_SUNRISE:
        case MEG_EVENT_EQUINOX_SUNSET:
            return se_solar_declination(jd);

        case MEG_EVENT_LUNAR_STANDSTILL_NORTH:
            return MAX_LUNAR_DEC;

        case MEG_EVENT_LUNAR_STANDSTILL_SOUTH:
            return -MAX_LUNAR_DEC;

        default:
            return 0.0;
    }
}

meg_alignment_t meg_check_alignment(int site_index, double jd)
{
    meg_alignment_t result = { .aligned = 0, .event_az = -1.0, .site_az = 0.0, .delta_deg = 999.0 };

    if (site_index < 0 || site_index >= SITE_COUNT) {
        return result;
    }

    meg_site_t site = sites[site_index];
    double dec = get_event_declination(site.event, jd);
    double event_az = compute_event_az(site.event, site.lat, dec);

    result.event_az = event_az;
    result.site_az = site.alignment_az;
    result.delta_deg = fabs(event_az - site.alignment_az);

    /* Handle wraparound (e.g., 359 vs 1 degree) */
    if (result.delta_deg > 180.0) {
        result.delta_deg = 360.0 - result.delta_deg;
    }

    result.aligned = (result.delta_deg <= site.tolerance_deg) ? 1 : 0;

    return result;
}

int meg_aligned_now(double jd, int *out, int max_out)
{
    int count = 0;
    for (int i = 0; i < SITE_COUNT && count < max_out; i++) {
        meg_alignment_t a = meg_check_alignment(i, jd);
        if (a.aligned) {
            out[count++] = i;
        }
    }
    return count;
}

int meg_days_to_alignment(int site_index, double jd)
{
    if (site_index < 0 || site_index >= SITE_COUNT) {
        return -1;
    }

    meg_site_t site = sites[site_index];

    /* Lunar standstill: 18.6-year cycle, too complex for simple day scan */
    if (site.event == MEG_EVENT_LUNAR_STANDSTILL_NORTH ||
        site.event == MEG_EVENT_LUNAR_STANDSTILL_SOUTH) {
        return -1;
    }

    /* For solstice events, find when |declination| is maximized near target.
     * For equinox events, find when |declination| is near zero.
     * Scan forward day-by-day up to 366 days. */

    int is_equinox = (site.event == MEG_EVENT_EQUINOX_SUNRISE ||
                      site.event == MEG_EVENT_EQUINOX_SUNSET);

    double best_score = 1.0e10;
    int best_day = 0;

    for (int d = 0; d <= 366; d++) {
        double test_jd = jd + (double)d;
        double dec = se_solar_declination(test_jd);
        double score;

        if (is_equinox) {
            /* Score = closeness to zero declination */
            score = fabs(dec);
        } else {
            /* For solstice: score = distance from peak declination.
             * Summer solstice: dec should be near +23.44
             * Winter solstice: dec should be near -23.44 */
            int is_summer = (site.event == MEG_EVENT_SUMMER_SOLSTICE_SUNRISE ||
                             site.event == MEG_EVENT_SUMMER_SOLSTICE_SUNSET);
            double target_dec = is_summer ? MEG_OBLIQUITY : -MEG_OBLIQUITY;
            score = fabs(dec - target_dec);
        }

        if (score < best_score) {
            best_score = score;
            best_day = d;
        }
    }

    return best_day;
}
