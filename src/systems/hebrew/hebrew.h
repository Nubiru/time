/* hebrew.h — Hebrew (Jewish) lunisolar calendar
 *
 * Pure functions for bidirectional conversion between Julian Day
 * and the Hebrew calendar. Algorithm based on Dershowitz & Reingold
 * "Calendrical Calculations" formulation.
 *
 * Month numbering: 1=Nisan through 12=Adar (common) or 13=Adar II (leap).
 * Civil year begins with Tishrei (month 7). */

#ifndef TIME_HEBREW_H
#define TIME_HEBREW_H

#include <stdbool.h>

typedef struct {
    int year;    /* Hebrew year (e.g., 5786) */
    int month;   /* 1-13 (1=Nisan, 7=Tishrei, 13=Adar II in leap years) */
    int day;     /* 1-30 */
} hebrew_date_t;

typedef enum {
    HEBREW_DEFICIENT = 353,
    HEBREW_REGULAR   = 354,
    HEBREW_ABUNDANT  = 355
} hebrew_year_type_t;

/* Convert Julian Day to Hebrew date. */
hebrew_date_t hebrew_from_jd(double jd);

/* Convert Hebrew date to Julian Day (midnight UT). */
double hebrew_to_jd(hebrew_date_t date);

/* Month name string. Returns "Adar" for common, "Adar I" for leap month 12. */
const char *hebrew_month_name(int month, bool is_leap_year);

/* True if year is a leap year in the 19-year Metonic cycle. */
bool hebrew_is_leap(int year);

/* Number of days in a given month of a given year. */
int hebrew_month_days(int year, int month);

/* Total days in the Hebrew year. */
int hebrew_year_days(int year);

/* Year type: DEFICIENT (353), REGULAR (354), or ABUNDANT (355). */
hebrew_year_type_t hebrew_year_type(int year);

/* Number of months: 12 (common) or 13 (leap). */
int hebrew_months_in_year(int year);

/* Molad of Tishrei as fractional days from Hebrew epoch. */
double hebrew_molad_tishrei(int year);

/* Rata Die (fixed day) of 1 Tishrei for the given Hebrew year. */
int hebrew_new_year(int year);

#endif /* TIME_HEBREW_H */
