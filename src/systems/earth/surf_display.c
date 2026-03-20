/* surf_display.c — Display-ready surf forecast formatter.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "surf_display.h"
#include "surf_spots.h"
#include "tide_predict.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* --- Helper: safe string copy with null termination --- */

static void safe_copy(char *dst, const char *src, int max_len)
{
    int i = 0;
    while (i < max_len - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

/* --- Helper: snprintf wrapper returning clamped length --- */

static int safe_snprintf_len(int written, int buf_size)
{
    if (written < 0) return 0;
    if (written >= buf_size) return buf_size - 1;
    return written;
}

/* --- sd_quality_label --- */

const char *sd_quality_label(double quality)
{
    if (quality < 0.2) return "Poor";
    if (quality < 0.4) return "Fair";
    if (quality < 0.6) return "Good";
    if (quality < 0.8) return "Excellent";
    return "Epic";
}

/* --- sd_format_tide --- */

int sd_format_tide(double height, int rising, int is_spring, int is_neap,
                   int is_king, double hours_to_high, double hours_to_low,
                   char *buf, int buf_size)
{
    if (buf_size <= 0) return 0;

    const char *direction = rising ? "Rising" : "Falling";
    const char *nearest;
    double nearest_hours;

    if (rising) {
        nearest = "high";
        nearest_hours = hours_to_high;
    } else {
        nearest = "low";
        nearest_hours = hours_to_low;
    }

    const char *tide_type;
    if (is_king)       tide_type = "King";
    else if (is_spring) tide_type = "Spring";
    else if (is_neap)   tide_type = "Neap";
    else                tide_type = "Normal";

    (void)height;

    int written = snprintf(buf, (size_t)buf_size,
        "%s tide | %.1fh to %s | %s tide",
        direction, nearest_hours, nearest, tide_type);

    return safe_snprintf_len(written, buf_size);
}

/* --- Helper: generate contextual surf advice --- */

static void build_advice(wave_type_t wave_type, int rising,
                         int is_spring, int is_king,
                         double quality, tide_pref_t best_tide,
                         char *buf, int buf_size)
{
    if (buf_size <= 0) return;

    const char *wave_name;
    switch (wave_type) {
        case WAVE_REEF:       wave_name = "reef";        break;
        case WAVE_BEACH:      wave_name = "beach break";  break;
        case WAVE_POINT:      wave_name = "point break";  break;
        case WAVE_RIVER_MOUTH: wave_name = "river mouth"; break;
        default:              wave_name = "break";        break;
    }

    /* King tide takes priority */
    if (is_king) {
        snprintf(buf, (size_t)buf_size,
            "King tide at %s \xe2\x80\x94 exceptional power, experienced surfers only",
            wave_name);
        return;
    }

    /* Spring tide mention */
    if (is_spring && quality >= 0.6) {
        snprintf(buf, (size_t)buf_size,
            "Spring tide amplifies swell at %s \xe2\x80\x94 powerful session ahead",
            wave_name);
        return;
    }

    /* Wave-type + tide-state specific advice */
    switch (wave_type) {
        case WAVE_REEF:
            if (rising && (best_tide == TIDE_MID || best_tide == TIDE_LOW)) {
                snprintf(buf, (size_t)buf_size,
                    "Mid-tide rising on reef \xe2\x80\x94 prime conditions");
            } else if (!rising && best_tide == TIDE_LOW) {
                snprintf(buf, (size_t)buf_size,
                    "Falling tide on reef \xe2\x80\x94 watch the shallows");
            } else {
                snprintf(buf, (size_t)buf_size,
                    "Reef break active \xe2\x80\x94 respect the coral");
            }
            break;

        case WAVE_BEACH:
            if (best_tide == TIDE_LOW || !rising) {
                snprintf(buf, (size_t)buf_size,
                    "Low tide exposes sandbars \xe2\x80\x94 watch for rips");
            } else {
                snprintf(buf, (size_t)buf_size,
                    "Beach break shifting with tide \xe2\x80\x94 read the banks");
            }
            break;

        case WAVE_POINT:
            if (best_tide == TIDE_HIGH || (!rising && best_tide == TIDE_ALL)) {
                snprintf(buf, (size_t)buf_size,
                    "High tide at point break \xe2\x80\x94 long rides possible");
            } else if (rising) {
                snprintf(buf, (size_t)buf_size,
                    "Rising tide at point break \xe2\x80\x94 building momentum");
            } else {
                snprintf(buf, (size_t)buf_size,
                    "Point break wrapping nicely \xe2\x80\x94 find the lineup");
            }
            break;

        case WAVE_RIVER_MOUTH:
            if (is_spring) {
                snprintf(buf, (size_t)buf_size,
                    "Spring tide at river mouth \xe2\x80\x94 strong currents, powerful walls");
            } else {
                snprintf(buf, (size_t)buf_size,
                    "River mouth break forming \xe2\x80\x94 watch the channel");
            }
            break;
    }
}

/* --- Helper: convert JD fractional day to HH:MM string --- */

static void jd_to_hhmm(double jd, int *hours, int *minutes)
{
    /* JD 0.0 = noon, 0.5 = midnight, so fractional part gives time */
    double frac = jd - floor(jd);
    /* JD day starts at noon: frac 0.0 = 12:00, 0.5 = 00:00 */
    double hours_f = (frac + 0.5) * 24.0;
    if (hours_f >= 24.0) hours_f -= 24.0;

    *hours = (int)hours_f;
    *minutes = (int)((hours_f - *hours) * 60.0);
    if (*hours < 0) *hours = 0;
    if (*hours > 23) *hours = 23;
    if (*minutes < 0) *minutes = 0;
    if (*minutes > 59) *minutes = 59;
}

/* --- sd_forecast --- */

sd_forecast_t sd_forecast(int spot_index, double jd)
{
    sd_forecast_t f;
    memset(&f, 0, sizeof(f));

    /* Validate spot index */
    surf_spot_t spot = surf_spot_get(spot_index);
    if (spot.id < 0) {
        return f;
    }

    /* Spot identity */
    safe_copy(f.spot_name, spot.name, 64);
    safe_copy(f.country, spot.country, 32);

    /* Get tidal state */
    tide_state_t tide = tide_predict_state(jd, spot.lat, spot.lon);

    /* Tide line */
    sd_format_tide(tide.height_relative, tide.rising,
                   tide.is_spring, tide.is_neap, tide.is_king,
                   tide.hours_to_next_high, tide.hours_to_next_low,
                   f.tide_line, SD_LINE_MAX);

    /* Quality */
    f.quality = tide_predict_surf_quality(jd, spot_index);

    /* Quality line: "Good conditions (72%)" */
    snprintf(f.quality_line, SD_LINE_MAX,
        "%s conditions (%d%%)",
        sd_quality_label(f.quality),
        (int)(f.quality * 100.0 + 0.5));

    /* Best time today */
    double best_jd = tide_predict_best_today(jd, spot_index);
    int best_h = 0;
    int best_m = 0;
    jd_to_hhmm(best_jd, &best_h, &best_m);
    snprintf(f.best_time_line, SD_LINE_MAX,
        "Best window: ~%02d:%02d today", best_h, best_m);

    /* Surf windows in next 24h */
    surf_forecast_t windows = tide_predict_surf_windows(jd, jd + 1.0,
                                                         spot_index);
    f.window_count = windows.count;

    /* Advice */
    build_advice(spot.wave_type, tide.rising,
                 tide.is_spring, tide.is_king,
                 f.quality, spot.best_tide,
                 f.advice, SD_LINE_MAX);

    return f;
}

/* --- sd_spot_card --- */

sd_spot_card_t sd_spot_card(int spot_index)
{
    sd_spot_card_t card;
    memset(&card, 0, sizeof(card));

    surf_spot_t spot = surf_spot_get(spot_index);
    if (spot.id < 0) {
        return card;
    }

    safe_copy(card.name, spot.name, 64);
    safe_copy(card.country, spot.country, 32);

    /* Wave type: "Reef break", "Beach break", "Point break", "River mouth" */
    const char *wt = surf_wave_type_name(spot.wave_type);
    if (wt) {
        if (spot.wave_type == WAVE_RIVER_MOUTH) {
            safe_copy(card.wave_type, "River mouth", 32);
        } else {
            snprintf(card.wave_type, 32, "%s break", wt);
        }
    }

    /* Best tide: "Low tide", "Mid tide", "High tide", "All tides" */
    const char *bt = surf_tide_name(spot.best_tide);
    if (bt) {
        if (spot.best_tide == TIDE_ALL) {
            safe_copy(card.best_tide, "All tides", 32);
        } else {
            snprintf(card.best_tide, 32, "%s tide", bt);
        }
    }

    /* Best season name (raw names already good: "Winter", "Spring", etc.) */
    const char *bs = surf_season_name(spot.best_season);
    if (bs) safe_copy(card.best_season, bs, 32);

    /* Description */
    if (spot.description) {
        safe_copy(card.description, spot.description, SD_LINE_MAX);
    }

    /* Coordinates: "{abs(lat)}°{N/S}, {abs(lon)}°{E/W}" */
    double alat = fabs(spot.lat);
    double alon = fabs(spot.lon);
    char ns = spot.lat >= 0.0 ? 'N' : 'S';
    char ew = spot.lon >= 0.0 ? 'E' : 'W';
    snprintf(card.coordinates, 64,
        "%.2f\xc2\xb0%c, %.2f\xc2\xb0%c",
        alat, ns, alon, ew);

    return card;
}

/* --- sd_format_forecast --- */

int sd_format_forecast(const sd_forecast_t *f, char *buf, int buf_size)
{
    if (buf_size <= 0) return 0;

    int written = snprintf(buf, (size_t)buf_size,
        "%s, %s\n"
        "%s\n"
        "%s\n"
        "%s\n"
        "%s",
        f->spot_name, f->country,
        f->tide_line,
        f->quality_line,
        f->best_time_line,
        f->advice);

    return safe_snprintf_len(written, buf_size);
}

/* --- sd_format_card --- */

int sd_format_card(const sd_spot_card_t *card, char *buf, int buf_size)
{
    if (buf_size <= 0) return 0;

    int written = snprintf(buf, (size_t)buf_size,
        "%s, %s\n"
        "%s\n"
        "Best tide: %s | Best season: %s\n"
        "%s\n"
        "%s",
        card->name, card->country,
        card->wave_type,
        card->best_tide, card->best_season,
        card->coordinates,
        card->description);

    return safe_snprintf_len(written, buf_size);
}
