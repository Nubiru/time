#ifndef TIME_LUNAR_HARMONICS_H
#define TIME_LUNAR_HARMONICS_H

#include "../../systems/astronomy/lunar.h"

#define SYNODIC_MONTH_DAYS 29.530588853
#define XIU_COUNT 28
#define MANZIL_COUNT 28
#define LUNAR_SYSTEM_COUNT 5

/* Lunar calendar systems that use the Moon */
typedef enum {
    LUNAR_SYS_HEBREW = 0,
    LUNAR_SYS_ISLAMIC,
    LUNAR_SYS_CHINESE,
    LUNAR_SYS_BUDDHIST,
    LUNAR_SYS_HINDU
} lunar_system_id_t;

/* Lunar mansion (Chinese Xiu or Arabic Manzil) */
typedef struct {
    int id;                /* 0-27, or -1 for invalid */
    const char *name;      /* Original name */
    const char *meaning;   /* English meaning */
    double start_deg;      /* ecliptic longitude start */
    double span_deg;       /* angular width (degrees) */
} lunar_mansion_t;

/* Cross-calendar lunar month status */
typedef struct {
    lunar_system_id_t system;
    const char *system_name;       /* "Hebrew", "Islamic", etc. */
    int approx_day_in_month;       /* 1-30, approximate day */
    int month_length;              /* typical month length for this system */
    double phase_at_month_start;   /* phase fraction when this system's month begins */
} lunar_month_status_t;

/* Lunation info */
typedef struct {
    int lunation_number;           /* Brown Lunation Number */
    double new_moon_jd;            /* JD of this lunation's new moon */
    double full_moon_jd;           /* JD of this lunation's full moon */
    double next_new_moon_jd;       /* JD of next lunation's new moon */
    double progress;               /* 0.0 to 1.0 within this lunation */
} lunation_t;

/* Next occurrence of ANY of the 8 phases */
double lunar_next_phase_of(double jd, moon_phase_t target);

/* Previous occurrence of a specific phase */
double lunar_prev_phase_of(double jd, moon_phase_t target);

/* Brown Lunation Number for a given JD */
int lunar_lunation_number(double jd);

/* Full lunation context */
lunation_t lunar_lunation(double jd);

/* Chinese Xiu (28 Lodges) from ecliptic longitude */
lunar_mansion_t lunar_xiu_from_longitude(double longitude);
lunar_mansion_t lunar_xiu_get(int index);
const char *lunar_xiu_name(int index);
int lunar_xiu_count(void);

/* Arabic Manzil (28 Mansions) from ecliptic longitude */
lunar_mansion_t lunar_manzil_from_longitude(double longitude);
lunar_mansion_t lunar_manzil_get(int index);
const char *lunar_manzil_name(int index);
int lunar_manzil_count(void);

/* Cross-calendar lunar month status for one system */
lunar_month_status_t lunar_month_status(double jd, lunar_system_id_t system);

/* All 5 systems at once */
void lunar_month_status_all(double jd, lunar_month_status_t out[LUNAR_SYSTEM_COUNT]);

/* System name */
const char *lunar_system_name(lunar_system_id_t system);

/* How many of the 5 systems start a new month within tolerance_days of this JD? */
int lunar_systems_aligned(double jd, double tolerance_days);

#endif /* TIME_LUNAR_HARMONICS_H */
