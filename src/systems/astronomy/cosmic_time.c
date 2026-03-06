#include "cosmic_time.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* Cosmological constants (Planck 2018 + IAU) */
#define UNIVERSE_AGE    13.787e9    /* years — Planck 2018 */
#define SUN_AGE         4.603e9     /* years — radiometric dating */
#define EARTH_AGE       4.543e9     /* years — radiometric dating */
#define GALACTIC_YEAR   225.0e6     /* years — Sun's orbital period around Milky Way */
#define J2000_JD        2451545.0   /* Julian Day of J2000.0 epoch */
#define J2000_YEAR      2000.0      /* Calendar year at J2000.0 */
#define DAYS_PER_YEAR   365.25      /* Julian year */
#define SECONDS_PER_DAY 86400.0

cosmic_context_t cosmic_context(double jd)
{
    cosmic_context_t ctx;
    double years_from_j2000 = (jd - J2000_JD) / DAYS_PER_YEAR;
    double current_year = J2000_YEAR + years_from_j2000;

    ctx.universe_age_years = UNIVERSE_AGE + (current_year - 2000.0);
    ctx.sun_age_years = SUN_AGE + (current_year - 2000.0);
    ctx.earth_age_years = EARTH_AGE + (current_year - 2000.0);
    ctx.galactic_year_years = GALACTIC_YEAR;
    ctx.sun_galactic_orbits = ctx.sun_age_years / GALACTIC_YEAR;
    ctx.sun_galactic_phase = fmod(ctx.sun_galactic_orbits, 1.0);

    return ctx;
}

void cosmic_format_age(double years, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    double abs_years = fabs(years);

    if (abs_years >= 1.0e9) {
        snprintf(buf, sz, "%.3f Gyr", years / 1.0e9);
    } else if (abs_years >= 1.0e6) {
        snprintf(buf, sz, "%.3f Myr", years / 1.0e6);
    } else if (abs_years >= 1.0e3) {
        snprintf(buf, sz, "%.1f kyr", years / 1.0e3);
    } else {
        snprintf(buf, sz, "%.0f yr", years);
    }
}

/* Geological eras — major boundaries in years before present */
typedef struct {
    double start_bp;    /* years before present */
    const char *name;
} geo_era_t;

static const geo_era_t GEO_ERAS[] = {
    {0.0,         "Holocene"},         /* 0 - 11,700 */
    {11700.0,     "Pleistocene"},      /* 11,700 - 2.58 Myr */
    {2.58e6,      "Pliocene"},         /* 2.58 - 5.33 Myr */
    {5.33e6,      "Miocene"},          /* 5.33 - 23.03 Myr */
    {23.03e6,     "Oligocene"},        /* 23.03 - 33.9 Myr */
    {33.9e6,      "Eocene"},           /* 33.9 - 56.0 Myr */
    {56.0e6,      "Paleocene"},        /* 56.0 - 66.0 Myr */
    {66.0e6,      "Cretaceous"},       /* 66 - 145 Myr */
    {145.0e6,     "Jurassic"},         /* 145 - 201.4 Myr */
    {201.4e6,     "Triassic"},         /* 201.4 - 251.9 Myr */
    {251.9e6,     "Permian"},          /* 251.9 - 298.9 Myr */
    {298.9e6,     "Carboniferous"},    /* 298.9 - 358.9 Myr */
    {358.9e6,     "Devonian"},         /* 358.9 - 419.2 Myr */
    {419.2e6,     "Silurian"},         /* 419.2 - 443.8 Myr */
    {443.8e6,     "Ordovician"},       /* 443.8 - 485.4 Myr */
    {485.4e6,     "Cambrian"},         /* 485.4 - 538.8 Myr */
    {538.8e6,     "Ediacaran"},        /* 538.8 - 635 Myr */
    {635.0e6,     "Cryogenian"},       /* 635 Myr - 720 Myr */
    {720.0e6,     "Tonian"},           /* 720 Myr - 1 Gyr */
    {1.0e9,       "Mesoproterozoic"},  /* 1.0 - 1.6 Gyr */
    {1.6e9,       "Paleoproterozoic"}, /* 1.6 - 2.5 Gyr */
    {2.5e9,       "Archean"},          /* 2.5 - 4.0 Gyr */
    {4.0e9,       "Hadean"},           /* 4.0 - 4.6 Gyr */
};

#define GEO_ERA_COUNT (sizeof(GEO_ERAS) / sizeof(GEO_ERAS[0]))

const char *cosmic_geological_era(double years_bp)
{
    if (years_bp < 0.0) return "Future";

    /* Walk backward through eras to find the latest one that starts before years_bp */
    for (int i = (int)GEO_ERA_COUNT - 1; i >= 0; i--) {
        if (years_bp >= GEO_ERAS[i].start_bp) {
            return GEO_ERAS[i].name;
        }
    }

    return "Holocene";
}

double cosmic_universe_fraction(double jd)
{
    cosmic_context_t ctx = cosmic_context(jd);
    return ctx.universe_age_years / UNIVERSE_AGE;
}

void cosmic_distance_label(float distance, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;

    /* Scene units mapping (approximate, tied to project's sqrt-distance scale):
     * <1     = inner solar system   -> AU
     * 1-50   = outer solar system   -> AU
     * 50-500 = stellar neighborhood -> light-years
     * >500   = galactic scale       -> kly
     */
    if (distance < 1.0f) {
        snprintf(buf, sz, "%.1f AU", (double)distance * 5.0);
    } else if (distance < 50.0f) {
        snprintf(buf, sz, "%.0f AU", (double)distance * 5.0);
    } else if (distance < 500.0f) {
        double ly = (double)distance * 2.0;
        snprintf(buf, sz, "%.0f ly", ly);
    } else {
        double kly = (double)distance * 0.05;
        snprintf(buf, sz, "%.1f kly", kly);
    }
}

double cosmic_seconds_per_day(void)
{
    return SECONDS_PER_DAY;
}

double cosmic_seconds_per_year(void)
{
    return DAYS_PER_YEAR * SECONDS_PER_DAY;
}

double cosmic_days_per_year(void)
{
    return DAYS_PER_YEAR;
}
