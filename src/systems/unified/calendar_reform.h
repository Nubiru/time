/* calendar_reform — historical calendar systems and reform proposals.
 * Pure lookup module: static const data + seasonal hour computation.
 * No malloc, no globals, no side effects. */

#ifndef TIME_CALENDAR_REFORM_H
#define TIME_CALENDAR_REFORM_H

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* Calendar system types */
typedef enum {
    CAL_TYPE_LUNAR = 0,       /* Pure lunar */
    CAL_TYPE_SOLAR,           /* Pure solar */
    CAL_TYPE_LUNISOLAR,       /* Lunisolar hybrid */
    CAL_TYPE_PROPOSED,        /* Reform proposal (never adopted) */
    CAL_TYPE_ADOPTED,         /* Reform that was implemented */
    CAL_TYPE_COUNT
} calendar_type_t;

/* A historical calendar system or reform proposal */
typedef struct {
    int id;
    const char *name;              /* "Gregorian", "13 Moon", "World Calendar", etc. */
    int year;                      /* Year proposed/adopted (negative=BCE) */
    const char *originator;        /* Person/body who proposed it */
    const char *culture;           /* Culture/civilization */
    calendar_type_t type;
    int days_per_year;             /* Base days (360, 364, 365, etc.) */
    int months;                    /* Number of months */
    int intercalary_days;          /* Extra days (epagomenal, Day Out of Time, etc.) */
    const char *month_structure;   /* "12x30", "13x28+1", "12 unequal", etc. */
    const char *description;       /* Full description */
    const char *problem_solved;    /* What calendar problem this addresses */
    const char *weakness;          /* Known flaw or criticism */
} calendar_system_t;

/* Get total calendar systems cataloged. */
int calendar_system_count(void);

/* Get system by index. Returns entry with id=-1 if invalid. */
calendar_system_t calendar_system_get(int index);

/* Get systems by type. Fills out_indices. Returns count found. */
int calendar_systems_by_type(calendar_type_t type, int *out_indices, int out_max);

/* Seasonal hour length in minutes.
 * In seasonal/temporal hours, day and night each have 12 hours.
 * Day hours expand in summer, shrink in winter.
 * lat_deg: latitude (-90 to 90).
 * day_of_year: 1-365.
 * is_day_hour: 1 for day hour, 0 for night hour.
 * Returns length in minutes (60.0 at equinox everywhere). */
double seasonal_hour_length(double lat_deg, int day_of_year, int is_day_hour);

/* Day length in hours at given latitude and day of year.
 * Uses solar declination approximation.
 * Returns 0-24 (handles polar day/night). */
double calendar_day_length(double lat_deg, int day_of_year);

/* Night length in hours. Complement of day length. */
double calendar_night_length(double lat_deg, int day_of_year);

/* Calendar type name string. */
const char *calendar_type_name(calendar_type_t type);

/* How many distinct cultures are represented? */
int calendar_culture_count(void);

/* Get distinct culture by index. */
const char *calendar_culture_get(int index);

/* Get the Gregorian month lengths (static data). Returns days for month 1-12. */
int gregorian_month_days(int month, int is_leap);

/* Year length comparison: return exact year length for a system index (in days, as double). */
double calendar_year_length(int index);

#endif /* TIME_CALENDAR_REFORM_H */
