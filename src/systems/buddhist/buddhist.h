/* buddhist.h — Buddhist calendar data module
 * Buddhist Era (Theravada convention: BE = CE + 543),
 * Uposatha lunar observance days, Vassa rain retreat,
 * 12 Theravada Pali lunar months, and Vesak computation.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BUDDHIST_H
#define TIME_BUDDHIST_H

#include <stdbool.h>

/* Uposatha day types */
typedef enum {
    UPOSATHA_NONE = 0,      /* Not an observance day */
    UPOSATHA_NEW_MOON,       /* Amavasya — new moon observance */
    UPOSATHA_FULL_MOON,      /* Purnima — full moon observance (most important) */
    UPOSATHA_FIRST_QUARTER,  /* Waxing half-moon */
    UPOSATHA_LAST_QUARTER    /* Waning half-moon */
} uposatha_type_t;

/* Uposatha observance info */
typedef struct {
    uposatha_type_t type;    /* Type of observance or NONE */
    const char *name;        /* "Purnima", "Amavasya", etc. */
    const char *description; /* Brief explanation */
    double moon_age_days;    /* Days since last new moon */
    double illumination;     /* 0.0-1.0 */
} uposatha_t;

/* Buddhist month info (lunar) */
typedef struct {
    int month;               /* 1-12 (Citta=1 to Phalguna=12, Theravada) */
    const char *name;        /* Pali month name */
    bool is_vassa;           /* Within the Rains Retreat (months 5-7 approx) */
} buddhist_month_t;

/* Convert CE year to Buddhist Era (Theravada: BE = CE + 543). */
int buddhist_era(int ce_year);

/* Convert Buddhist Era to CE year. */
int buddhist_to_ce(int be_year);

/* Get Uposatha status for a given Julian Day.
 * Uses lunar phase: observance if within tolerance of quarter phases. */
uposatha_t buddhist_uposatha(double jd);

/* Name of Uposatha type. Returns "None" for UPOSATHA_NONE. */
const char *buddhist_uposatha_name(uposatha_type_t type);

/* Find next Uposatha observance day from a given JD.
 * Returns the JD of the next observance. */
double buddhist_next_uposatha(double jd);

/* Get the Buddhist lunar month for a given JD (approximate).
 * Based on lunation count from the new moon nearest to the spring equinox. */
buddhist_month_t buddhist_month(double jd);

/* Buddhist month name by number (1-12). Returns "?" for invalid. */
const char *buddhist_month_name(int month);

/* Is the given JD within Vassa (Rains Retreat)?
 * Vassa: approximately sun longitude 100-200 degrees (~July to ~October). */
bool buddhist_is_vassa(double jd);

/* Count of Buddhist months (always 12). */
int buddhist_month_count(void);

/* Vesak full moon JD for a given CE year (approximate).
 * Vesak = first full moon when Sun is in Taurus (roughly May). */
double buddhist_vesak_jd(int ce_year);

#endif /* TIME_BUDDHIST_H */
