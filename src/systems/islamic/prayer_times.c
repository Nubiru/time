/* prayer_times.c — Islamic prayer times from solar position.
 * Pure functions. Uses se_solar_events() and se_solar_declination()
 * from astronomy/solar_events.h for sunrise/sunset/noon and declination. */

#include "prayer_times.h"
#include "../astronomy/solar_events.h"

#include <math.h>

#define PI 3.14159265358979323846
#define DEG2RAD (PI / 180.0)
#define RAD2DEG (180.0 / PI)

/* Method parameter tables */

static const char *METHOD_NAMES[] = {
    "Muslim World League",
    "ISNA",
    "Egyptian General Authority",
    "Umm al-Qura",
    "University of Islamic Sciences, Karachi"
};

static const double FAJR_ANGLES[] = {
    18.0,   /* MWL */
    15.0,   /* ISNA */
    19.5,   /* Egypt */
    18.5,   /* Makkah */
    18.0    /* Karachi */
};

static const double ISHA_ANGLES[] = {
    17.0,   /* MWL */
    15.0,   /* ISNA */
    17.5,   /* Egypt */
     0.0,   /* Makkah — uses fixed 90 minutes */
    18.0    /* Karachi */
};

const char *prayer_method_name(prayer_method_t method)
{
    if (method < 0 || method >= PRAYER_METHOD_COUNT) {
        return "Unknown";
    }
    return METHOD_NAMES[method];
}

double prayer_method_fajr_angle(prayer_method_t method)
{
    if (method < 0 || method >= PRAYER_METHOD_COUNT) {
        return 0.0;
    }
    return FAJR_ANGLES[method];
}

double prayer_method_isha_angle(prayer_method_t method)
{
    if (method < 0 || method >= PRAYER_METHOD_COUNT) {
        return 0.0;
    }
    return ISHA_ANGLES[method];
}

/* Compute hour angle (in days, as fraction of full rotation) for a given
 * sun altitude at a given latitude and declination.
 * Returns the hour angle as a fraction of a day (divide by 2*PI already done).
 * Sign: positive = after noon, negative = before noon.
 * Returns NAN if the sun never reaches that altitude. */
static double hour_angle_days(double altitude_deg, double lat_deg, double dec_deg)
{
    double lat_r = lat_deg * DEG2RAD;
    double dec_r = dec_deg * DEG2RAD;
    double alt_r = altitude_deg * DEG2RAD;

    double cos_omega = (sin(alt_r) - sin(lat_r) * sin(dec_r))
                     / (cos(lat_r) * cos(dec_r));

    if (cos_omega > 1.0 || cos_omega < -1.0) {
        return NAN;
    }

    /* acos gives radians [0, PI]. Convert to fraction of day. */
    return acos(cos_omega) / (2.0 * PI);
}

prayer_times_t prayer_times_compute(double jd_noon_input, double lat, double lon,
                                     prayer_method_t method, asr_jurisprudence_t asr)
{
    prayer_times_t result;

    /* Get sunrise, sunset, solar noon from solar_events */
    solar_day_t day = se_solar_events(jd_noon_input, lat, lon);
    double solar_noon = day.solar_noon_jd;
    double dec = se_solar_declination(solar_noon);

    /* === Sunrise === */
    result.sunrise = day.sunrise_jd;

    /* === Maghrib === sunset */
    result.maghrib = day.sunset_jd;

    /* === Dhuhr === solar noon + 65 seconds safety margin */
    result.dhuhr = solar_noon + 65.0 / 86400.0;

    /* === Fajr === sun at -fajr_angle before sunrise */
    double fajr_angle = prayer_method_fajr_angle(method);
    double fajr_ha = hour_angle_days(-fajr_angle, lat, dec);
    if (isnan(fajr_ha)) {
        result.fajr = NAN;
    } else {
        result.fajr = solar_noon - fajr_ha;
    }

    /* === Asr === */
    {
        double factor = (asr == ASR_HANAFI) ? 2.0 : 1.0;
        double lat_r = lat * DEG2RAD;
        double dec_r = dec * DEG2RAD;

        /* Zenith angle at noon = |lat - dec| */
        double zenith_noon = fabs(lat_r - dec_r);

        /* Asr altitude: when shadow = factor * object_height + noon_shadow
         * tan(zenith_at_noon) = noon shadow / object height
         * At Asr: shadow / height = factor + tan(zenith_at_noon)
         * So altitude = atan(1 / (factor + tan(zenith_at_noon))) */
        double asr_alt_rad = atan(1.0 / (factor + tan(zenith_noon)));
        double asr_alt_deg = asr_alt_rad * RAD2DEG;

        double asr_ha = hour_angle_days(asr_alt_deg, lat, dec);
        if (isnan(asr_ha)) {
            result.asr = NAN;
        } else {
            result.asr = solar_noon + asr_ha;
        }
    }

    /* === Isha === */
    if (method == PRAYER_METHOD_MAKKAH) {
        /* Umm al-Qura: Isha = Maghrib + 90 minutes */
        result.isha = result.maghrib + 90.0 / 1440.0;
    } else {
        double isha_angle = prayer_method_isha_angle(method);
        double isha_ha = hour_angle_days(-isha_angle, lat, dec);
        if (isnan(isha_ha)) {
            result.isha = NAN;
        } else {
            result.isha = solar_noon + isha_ha;
        }
    }

    return result;
}

next_prayer_t prayer_next(prayer_times_t times, double current_jd)
{
    next_prayer_t np;

    if (current_jd < times.fajr) {
        np.name = "Fajr";
        np.jd = times.fajr;
    } else if (current_jd < times.sunrise) {
        np.name = "Sunrise";
        np.jd = times.sunrise;
    } else if (current_jd < times.dhuhr) {
        np.name = "Dhuhr";
        np.jd = times.dhuhr;
    } else if (current_jd < times.asr) {
        np.name = "Asr";
        np.jd = times.asr;
    } else if (current_jd < times.maghrib) {
        np.name = "Maghrib";
        np.jd = times.maghrib;
    } else if (current_jd < times.isha) {
        np.name = "Isha";
        np.jd = times.isha;
    } else {
        /* After Isha — next prayer is Fajr (next day) */
        np.name = "Fajr";
        np.jd = 0.0; /* No next-day info in single-day struct */
    }

    return np;
}
