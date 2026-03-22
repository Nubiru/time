/* daily_earth_layout.c — Daily Earth surface card layout
 *
 * Calls daylight and seasons modules to fill Earth-surface card data,
 * then computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_earth_layout.h"
#include "../systems/earth/daylight.h"
#include "../systems/earth/seasons.h"
#include "../math/julian.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DAYLIGHT_Y   0.18f
#define SUN_Y        0.32f
#define SEASON_Y     0.46f
#define POLAR_Y      0.60f
#define LENGTH_Y     0.74f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

/* Compute day of year from JD using Gregorian calendar conversion. */
static int day_of_year_from_jd(double jd)
{
    int year, month;
    double day_frac = jd_to_gregorian(jd, &year, &month);
    int day = (int)day_frac;

    /* Compute Jan 1 JD for this year, then difference */
    double jan1_jd = gregorian_to_jd(year, 1, 1.0);
    double current_jd = gregorian_to_jd(year, month, (double)day);

    int doy = (int)(current_jd - jan1_jd) + 1;

    /* Clamp to valid range */
    if (doy < 1) doy = 1;
    if (doy > 365) doy = 365;

    return doy;
}

daily_earth_layout_t daily_earth_compute(double jd, double lat, double lon)
{
    daily_earth_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Daylight data */
    daylight_t dl = daylight_at(jd, lat, lon);
    layout.is_day = dl.is_day;
    layout.sun_elevation_deg = dl.sun_elevation_deg;
    layout.day_length_hours = dl.day_length_hours;
    layout.polar_day = dl.polar_day;
    layout.polar_night = dl.polar_night;

    /* Season data */
    int doy = day_of_year_from_jd(jd);
    season_info_t si = season_at(lat, doy);
    layout.season = (int)si.season;
    layout.season_name = si.name;
    layout.season_progress = si.progress;

    /* Interpretation (composed — no separate interpret module) */
    snprintf(layout.glance, sizeof(layout.glance), "%s · %.1f hours daylight",
             si.name ? si.name : "?", dl.day_length_hours);
    if (dl.polar_day) {
        snprintf(layout.detail, sizeof(layout.detail),
                 "Polar day · Sun %.1f° above horizon · %s %.0f%% through",
                 dl.sun_elevation_deg, si.name ? si.name : "?",
                 si.progress * 100.0);
    } else if (dl.polar_night) {
        snprintf(layout.detail, sizeof(layout.detail),
                 "Polar night · Sun %.1f° below horizon · %s %.0f%% through",
                 fabs(dl.sun_elevation_deg), si.name ? si.name : "?",
                 si.progress * 100.0);
    } else {
        snprintf(layout.detail, sizeof(layout.detail),
                 "Sun %.1f° %s horizon · %s %.0f%% through · %.1fh daylight",
                 fabs(dl.sun_elevation_deg),
                 dl.sun_elevation_deg >= 0.0 ? "above" : "below",
                 si.name ? si.name : "?", si.progress * 100.0,
                 dl.day_length_hours);
    }

    /* Title slot */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = TITLE_Y;
    layout.title_slot.w = SLOT_W;
    layout.title_slot.h = SLOT_H;

    /* Daylight slot */
    layout.daylight_slot.x = SLOT_X;
    layout.daylight_slot.y = DAYLIGHT_Y;
    layout.daylight_slot.w = SLOT_W;
    layout.daylight_slot.h = SLOT_H;

    /* Sun slot */
    layout.sun_slot.x = SLOT_X;
    layout.sun_slot.y = SUN_Y;
    layout.sun_slot.w = SLOT_W;
    layout.sun_slot.h = SLOT_H;

    /* Season slot */
    layout.season_slot.x = SLOT_X;
    layout.season_slot.y = SEASON_Y;
    layout.season_slot.w = SLOT_W;
    layout.season_slot.h = SLOT_H;

    /* Polar slot */
    layout.polar_slot.x = SLOT_X;
    layout.polar_slot.y = POLAR_Y;
    layout.polar_slot.w = SLOT_W;
    layout.polar_slot.h = SLOT_H;

    /* Length slot */
    layout.length_slot.x = SLOT_X;
    layout.length_slot.y = LENGTH_Y;
    layout.length_slot.w = SLOT_W;
    layout.length_slot.h = SLOT_H;

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

int daily_earth_is_day(const daily_earth_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    return layout->is_day;
}
