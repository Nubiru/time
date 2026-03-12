/* location_alerts.c — Location-based alert evaluation.
 * Evaluates surf, snow, golden hour, prayer, tide, and daylight
 * conditions using existing Earth/astronomy modules. Pure functions. */

#include "location_alerts.h"
#include "tide_predict.h"
#include "daylight.h"
#include "snow_season.h"
#include "surf_spots.h"
#include "ski_resorts.h"
#include "../islamic/prayer_times.h"
#include "../astronomy/solar_events.h"
#include "../../math/julian.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* ===== Helpers ===== */

/* Extract month (1-12) from Julian Day */
static int jd_to_month(double jd) {
    int year, month;
    jd_to_gregorian(jd, &year, &month);
    (void)year;
    return month;
}

/* ===== alert_type_name / alert_urgency_name ===== */

const char *alert_type_name(alert_type_t type) {
    switch (type) {
        case ALERT_SURF:        return "Surf";
        case ALERT_SNOW:        return "Snow";
        case ALERT_GOLDEN_HOUR: return "Golden Hour";
        case ALERT_BLUE_HOUR:   return "Blue Hour";
        case ALERT_PRAYER:      return "Prayer";
        case ALERT_DAYLIGHT:    return "Daylight";
        case ALERT_TIDE:        return "Tide";
        default:                return "Unknown";
    }
}

const char *alert_urgency_name(alert_urgency_t urgency) {
    switch (urgency) {
        case ALERT_INFO:       return "Info";
        case ALERT_SUGGESTION: return "Suggestion";
        case ALERT_NOTABLE:    return "Notable";
        default:               return "Unknown";
    }
}

/* ===== alert_sort ===== */

void alert_sort(alert_t *alerts, int count) {
    if (!alerts || count <= 1) return;

    /* Insertion sort — max 16 items, no malloc needed */
    for (int i = 1; i < count; i++) {
        alert_t key = alerts[i];
        int j = i - 1;

        /* Sort by urgency DESC, then hours_until ASC */
        while (j >= 0 &&
               (alerts[j].urgency < key.urgency ||
                (alerts[j].urgency == key.urgency &&
                 alerts[j].hours_until > key.hours_until))) {
            alerts[j + 1] = alerts[j];
            j--;
        }
        alerts[j + 1] = key;
    }
}

/* ===== alert_check_surf ===== */

int alert_check_surf(double jd, double lat, double lon,
                     alert_t *out, int max) {
    if (max <= 0) return 0;

    int count = 0;
    int spot_idx = surf_spot_nearest(lat, lon);
    const char *spot_name = surf_spot_name(spot_idx);
    if (!spot_name) spot_name = "nearby break";

    /* Check for king tide approaching */
    tide_state_t tide = tide_predict_state(jd, lat, lon);
    if (tide.is_king && count < max) {
        out[count].type = ALERT_SURF;
        out[count].urgency = ALERT_NOTABLE;
        snprintf(out[count].message, sizeof(out[count].message),
                 "King tide at %s", spot_name);
        out[count].hours_until = 0.0;
        out[count].quality = 0.9;
        count++;
    }

    /* Check surf quality today */
    double quality = tide_predict_surf_quality(jd, spot_idx);
    if (quality > 0.7 && count < max) {
        out[count].type = ALERT_SURF;
        out[count].urgency = ALERT_SUGGESTION;
        snprintf(out[count].message, sizeof(out[count].message),
                 "Good surf at %s (%.0f%%)", spot_name, quality * 100.0);
        out[count].hours_until = 0.0;
        out[count].quality = quality;
        count++;
    } else if (count < max) {
        /* Always produce at least an INFO with current quality */
        out[count].type = ALERT_SURF;
        out[count].urgency = ALERT_INFO;
        snprintf(out[count].message, sizeof(out[count].message),
                 "Surf at %s: %.0f%% quality", spot_name, quality * 100.0);
        out[count].hours_until = 0.0;
        out[count].quality = quality;
        count++;
    }

    return count;
}

/* ===== alert_check_snow ===== */

int alert_check_snow(double jd, double lat, double lon,
                     alert_t *out, int max) {
    if (max <= 0) return 0;

    int count = 0;
    int resort_idx = ski_resort_nearest(lat, lon);
    const char *resort_name = ski_resort_name(resort_idx);
    if (!resort_name) return 0;

    ski_resort_t resort = ski_resort_get(resort_idx);
    if (resort.id < 0) return 0;

    int month = jd_to_month(jd);
    int in_season = ski_resort_in_season(resort_idx, month);

    if (in_season) {
        /* Check powder likelihood */
        double powder = snow_powder_likelihood(lat, resort.summit_elev_m, month);
        if (powder > 0.6 && count < max) {
            out[count].type = ALERT_SNOW;
            out[count].urgency = ALERT_SUGGESTION;
            snprintf(out[count].message, sizeof(out[count].message),
                     "Peak powder at %s", resort_name);
            out[count].hours_until = 0.0;
            out[count].quality = powder;
            count++;
        } else if (count < max) {
            double snow_q = ski_resort_snow_quality(resort_idx, month);
            out[count].type = ALERT_SNOW;
            out[count].urgency = ALERT_INFO;
            snprintf(out[count].message, sizeof(out[count].message),
                     "%s in season (%.0f%% snow)", resort_name,
                     snow_q * 100.0);
            out[count].hours_until = 0.0;
            out[count].quality = snow_q;
            count++;
        }
    } else {
        /* Check if season is approaching (within ~30 days) */
        int season_start = resort.season_start;
        int months_until = season_start - month;
        if (months_until < 0) months_until += 12;
        if (months_until <= 1 && months_until > 0 && count < max) {
            out[count].type = ALERT_SNOW;
            out[count].urgency = ALERT_INFO;
            snprintf(out[count].message, sizeof(out[count].message),
                     "%s season opens soon", resort_name);
            out[count].hours_until = months_until * 30.0 * 24.0;
            out[count].quality = 0.0;
            count++;
        }
    }

    return count;
}

/* ===== alert_check_golden_hour ===== */

int alert_check_golden_hour(double jd, double lat, double lon,
                            alert_t *out, int max) {
    if (max <= 0) return 0;

    int count = 0;

    /* Check morning golden hour */
    daylight_window_t gm = daylight_golden_hour_morning(jd, lat, lon);
    if (gm.valid) {
        double hours_to_start = (gm.start_jd - jd) * 24.0;
        double hours_to_end = (gm.end_jd - jd) * 24.0;

        if (hours_to_start > 0.0 && hours_to_start <= 2.0 && count < max) {
            int mins = (int)(hours_to_start * 60.0);
            out[count].type = ALERT_GOLDEN_HOUR;
            out[count].urgency = ALERT_SUGGESTION;
            snprintf(out[count].message, sizeof(out[count].message),
                     "Morning golden hour in %d minutes", mins);
            out[count].hours_until = hours_to_start;
            out[count].quality = 0.8;
            count++;
        } else if (hours_to_start <= 0.0 && hours_to_end > 0.0
                   && count < max) {
            int mins_left = (int)(hours_to_end * 60.0);
            out[count].type = ALERT_GOLDEN_HOUR;
            out[count].urgency = ALERT_NOTABLE;
            snprintf(out[count].message, sizeof(out[count].message),
                     "Golden hour now (%d min left)", mins_left);
            out[count].hours_until = 0.0;
            out[count].quality = 1.0;
            count++;
        }
    }

    /* Check evening golden hour */
    daylight_window_t ge = daylight_golden_hour_evening(jd, lat, lon);
    if (ge.valid) {
        double hours_to_start = (ge.start_jd - jd) * 24.0;
        double hours_to_end = (ge.end_jd - jd) * 24.0;

        if (hours_to_start > 0.0 && hours_to_start <= 2.0 && count < max) {
            int mins = (int)(hours_to_start * 60.0);
            out[count].type = ALERT_GOLDEN_HOUR;
            out[count].urgency = ALERT_SUGGESTION;
            snprintf(out[count].message, sizeof(out[count].message),
                     "Evening golden hour in %d minutes", mins);
            out[count].hours_until = hours_to_start;
            out[count].quality = 0.8;
            count++;
        } else if (hours_to_start <= 0.0 && hours_to_end > 0.0
                   && count < max) {
            int mins_left = (int)(hours_to_end * 60.0);
            out[count].type = ALERT_GOLDEN_HOUR;
            out[count].urgency = ALERT_NOTABLE;
            snprintf(out[count].message, sizeof(out[count].message),
                     "Golden hour now (%d min left)", mins_left);
            out[count].hours_until = 0.0;
            out[count].quality = 1.0;
            count++;
        }
    }

    /* Check morning blue hour */
    daylight_window_t bm = daylight_blue_hour_morning(jd, lat, lon);
    if (bm.valid) {
        double hours_to_start = (bm.start_jd - jd) * 24.0;
        double hours_to_end = (bm.end_jd - jd) * 24.0;

        if (hours_to_start > 0.0 && hours_to_start <= 2.0 && count < max) {
            int mins = (int)(hours_to_start * 60.0);
            out[count].type = ALERT_BLUE_HOUR;
            out[count].urgency = ALERT_SUGGESTION;
            snprintf(out[count].message, sizeof(out[count].message),
                     "Morning blue hour in %d minutes", mins);
            out[count].hours_until = hours_to_start;
            out[count].quality = 0.8;
            count++;
        } else if (hours_to_start <= 0.0 && hours_to_end > 0.0
                   && count < max) {
            int mins_left = (int)(hours_to_end * 60.0);
            out[count].type = ALERT_BLUE_HOUR;
            out[count].urgency = ALERT_NOTABLE;
            snprintf(out[count].message, sizeof(out[count].message),
                     "Blue hour now (%d min left)", mins_left);
            out[count].hours_until = 0.0;
            out[count].quality = 1.0;
            count++;
        }
    }

    /* Check evening blue hour */
    daylight_window_t be = daylight_blue_hour_evening(jd, lat, lon);
    if (be.valid) {
        double hours_to_start = (be.start_jd - jd) * 24.0;
        double hours_to_end = (be.end_jd - jd) * 24.0;

        if (hours_to_start > 0.0 && hours_to_start <= 2.0 && count < max) {
            int mins = (int)(hours_to_start * 60.0);
            out[count].type = ALERT_BLUE_HOUR;
            out[count].urgency = ALERT_SUGGESTION;
            snprintf(out[count].message, sizeof(out[count].message),
                     "Evening blue hour in %d minutes", mins);
            out[count].hours_until = hours_to_start;
            out[count].quality = 0.8;
            count++;
        } else if (hours_to_start <= 0.0 && hours_to_end > 0.0
                   && count < max) {
            int mins_left = (int)(hours_to_end * 60.0);
            out[count].type = ALERT_BLUE_HOUR;
            out[count].urgency = ALERT_NOTABLE;
            snprintf(out[count].message, sizeof(out[count].message),
                     "Blue hour now (%d min left)", mins_left);
            out[count].hours_until = 0.0;
            out[count].quality = 1.0;
            count++;
        }
    }

    return count;
}

/* ===== alert_check_prayer ===== */

int alert_check_prayer(double jd, double lat, double lon,
                       alert_t *out, int max) {
    if (max <= 0) return 0;

    int count = 0;

    /* Compute prayer times for noon of the current day */
    double jd_noon = floor(jd) + 0.5;
    if (jd_noon > jd + 0.5) jd_noon -= 1.0;

    prayer_times_t times = prayer_times_compute(jd_noon, lat, lon,
                                                PRAYER_METHOD_MWL,
                                                ASR_SHAFII);
    next_prayer_t next = prayer_next(times, jd);

    if (next.name) {
        double hours_until = (next.jd - jd) * 24.0;
        if (hours_until < 0.0) hours_until = 0.0;

        /* If within 30 minutes, SUGGESTION */
        if (hours_until <= 0.5 && count < max) {
            int mins = (int)(hours_until * 60.0);
            if (mins < 1) mins = 1;
            out[count].type = ALERT_PRAYER;
            out[count].urgency = ALERT_SUGGESTION;
            snprintf(out[count].message, sizeof(out[count].message),
                     "%s in %d minutes", next.name, mins);
            out[count].hours_until = hours_until;
            out[count].quality = 1.0;
            count++;
        } else if (count < max) {
            /* Always produce INFO for next prayer */
            int mins = (int)(hours_until * 60.0);
            int hrs = mins / 60;
            int rem_mins = mins % 60;
            out[count].type = ALERT_PRAYER;
            out[count].urgency = ALERT_INFO;
            if (hrs > 0) {
                snprintf(out[count].message, sizeof(out[count].message),
                         "Next: %s in %dh %dm", next.name, hrs, rem_mins);
            } else {
                snprintf(out[count].message, sizeof(out[count].message),
                         "Next: %s in %d minutes", next.name, rem_mins);
            }
            out[count].hours_until = hours_until;
            out[count].quality = 0.5;
            count++;
        }
    }

    return count;
}

/* ===== alert_check_tide ===== */

int alert_check_tide(double jd, double lat, double lon,
                     alert_t *out, int max) {
    if (max <= 0) return 0;

    int count = 0;
    tide_state_t tide = tide_predict_state(jd, lat, lon);

    /* King tide — NOTABLE */
    if (tide.is_king && count < max) {
        out[count].type = ALERT_TIDE;
        out[count].urgency = ALERT_NOTABLE;
        snprintf(out[count].message, sizeof(out[count].message),
                 "King tide active");
        out[count].hours_until = 0.0;
        out[count].quality = 1.0;
        count++;
    }

    /* Spring tide within 24h — SUGGESTION */
    if (tide.is_spring && !tide.is_king && count < max) {
        out[count].type = ALERT_TIDE;
        out[count].urgency = ALERT_SUGGESTION;
        snprintf(out[count].message, sizeof(out[count].message),
                 "Spring tide — higher highs, lower lows");
        out[count].hours_until = 0.0;
        out[count].quality = tide.spring_neap_factor;
        count++;
    }

    /* General tide info */
    if (count < max) {
        const char *direction = tide.rising ? "rising" : "falling";
        double next_h = tide.hours_to_next_high;
        out[count].type = ALERT_TIDE;
        out[count].urgency = ALERT_INFO;
        snprintf(out[count].message, sizeof(out[count].message),
                 "Tide %s, high in %.1f hours", direction, next_h);
        out[count].hours_until = next_h;
        out[count].quality = tide.spring_neap_factor;
        count++;
    }

    return count;
}

/* ===== alert_check_daylight ===== */

int alert_check_daylight(double jd, double lat, double lon,
                         alert_t *out, int max) {
    if (max <= 0) return 0;

    int count = 0;
    daylight_t dl = daylight_at(jd, lat, lon);

    /* Polar day */
    if (dl.polar_day && count < max) {
        out[count].type = ALERT_DAYLIGHT;
        out[count].urgency = ALERT_INFO;
        snprintf(out[count].message, sizeof(out[count].message),
                 "Midnight sun — 24 hours of daylight");
        out[count].hours_until = 0.0;
        out[count].quality = 1.0;
        count++;
        return count;
    }

    /* Polar night */
    if (dl.polar_night && count < max) {
        out[count].type = ALERT_DAYLIGHT;
        out[count].urgency = ALERT_INFO;
        snprintf(out[count].message, sizeof(out[count].message),
                 "Polar night — no sunrise today");
        out[count].hours_until = 0.0;
        out[count].quality = 0.0;
        count++;
        return count;
    }

    /* Sunrise within 1 hour */
    if (dl.minutes_to_sunrise >= 0.0 && dl.minutes_to_sunrise <= 60.0
        && count < max) {
        int mins = (int)dl.minutes_to_sunrise;
        out[count].type = ALERT_DAYLIGHT;
        out[count].urgency = ALERT_SUGGESTION;
        snprintf(out[count].message, sizeof(out[count].message),
                 "Sunrise in %d minutes", mins);
        out[count].hours_until = dl.minutes_to_sunrise / 60.0;
        out[count].quality = 0.9;
        count++;
    }

    /* Sunset within 1 hour */
    if (dl.minutes_to_sunset >= 0.0 && dl.minutes_to_sunset <= 60.0
        && count < max) {
        int mins = (int)dl.minutes_to_sunset;
        out[count].type = ALERT_DAYLIGHT;
        out[count].urgency = ALERT_SUGGESTION;
        snprintf(out[count].message, sizeof(out[count].message),
                 "Sunset in %d minutes", mins);
        out[count].hours_until = dl.minutes_to_sunset / 60.0;
        out[count].quality = 0.9;
        count++;
    }

    /* General daylight info if no special alerts */
    if (count == 0 && count < max) {
        out[count].type = ALERT_DAYLIGHT;
        out[count].urgency = ALERT_INFO;
        snprintf(out[count].message, sizeof(out[count].message),
                 "%.1f hours of daylight today", dl.day_length_hours);
        out[count].hours_until = 0.0;
        out[count].quality = dl.day_length_hours / 24.0;
        count++;
    }

    return count;
}

/* ===== location_alerts_evaluate ===== */

alert_result_t location_alerts_evaluate(double jd, double lat, double lon) {
    alert_result_t result;
    memset(&result, 0, sizeof(result));

    int total = 0;
    int remaining = ALERT_MAX;

    /* Collect alerts from all checkers */
    int n;

    n = alert_check_surf(jd, lat, lon, &result.alerts[total], remaining);
    total += n;
    remaining -= n;

    if (remaining > 0) {
        n = alert_check_snow(jd, lat, lon, &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    if (remaining > 0) {
        n = alert_check_golden_hour(jd, lat, lon,
                                    &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    if (remaining > 0) {
        n = alert_check_prayer(jd, lat, lon,
                               &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    if (remaining > 0) {
        n = alert_check_tide(jd, lat, lon,
                             &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    if (remaining > 0) {
        n = alert_check_daylight(jd, lat, lon,
                                 &result.alerts[total], remaining);
        total += n;
        remaining -= n;
    }

    result.count = total;

    /* Sort: NOTABLE first, then soonest */
    alert_sort(result.alerts, result.count);

    return result;
}
