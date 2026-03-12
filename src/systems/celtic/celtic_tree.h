/* celtic_tree.h — Celtic Tree Calendar (Robert Graves system)
 * 13 months x 28 days + 1 intercalary day = 365 days.
 * Year starts Dec 24 (Birch) and ends Dec 23 (Yew intercalary day).
 * Each month corresponds to a tree in the Ogham alphabet.
 *
 * Sources: Robert Graves, "The White Goddess" (1948);
 * Ogham alphabet from the Book of Ballymote (1391). */

#ifndef TIME_CELTIC_TREE_H
#define TIME_CELTIC_TREE_H

#include <stdbool.h>

#define CELTIC_TREE_MONTHS     13
#define CELTIC_TREE_MONTH_DAYS 28
#define CELTIC_TREE_YEAR_DAYS  365

typedef struct {
    int month;     /* 1-13 (Birch=1 through Elder=13), 0 = intercalary (Yew day) */
    int day;       /* 1-28 for months 1-13, 1 for intercalary */
} celtic_tree_date_t;

typedef struct {
    int id;                 /* 0-13 (0=Yew intercalary, 1-13=months), -1 for invalid */
    const char *tree;       /* English tree name */
    const char *ogham;      /* Ogham name */
    const char *letter;     /* Ogham letter(s) */
    const char *meaning;    /* Symbolic meaning */
    int start_day_of_year;  /* Day of Gregorian year (1-365) when this month starts */
} celtic_tree_info_t;

/* Convert Julian Day to Celtic tree date.
 * The Celtic calendar is perpetual: same Gregorian dates every year. */
celtic_tree_date_t celtic_tree_from_jd(double jd);

/* Convert Celtic tree date to Julian Day.
 * gregorian_year is the year containing most of the Celtic year
 * (Jan-Dec portion). The Celtic year starting Dec 24 of year Y
 * uses gregorian_year = Y+1. */
double celtic_tree_to_jd(celtic_tree_date_t date, int gregorian_year);

/* Get full info struct for a month (0-13). Returns id=-1, tree="?" for invalid. */
celtic_tree_info_t celtic_tree_info(int month);

/* Tree name for a month (0-13). Returns "?" for invalid. */
const char *celtic_tree_name(int month);

/* Ogham name for a month (0-13). Returns "?" for invalid. */
const char *celtic_tree_ogham(int month);

/* Ogham letter for a month (0-13). Returns "?" for invalid. */
const char *celtic_tree_letter(int month);

/* Symbolic meaning for a month (0-13). Returns "?" for invalid. */
const char *celtic_tree_meaning(int month);

/* True if the date is the intercalary Yew day (month==0). */
bool celtic_tree_is_intercalary(celtic_tree_date_t date);

/* Celtic day of year (1-365). Birch 1 = 1, Yew intercalary = 365. */
int celtic_tree_day_of_year(celtic_tree_date_t date);

/* Number of days in a month. 28 for months 1-13, 1 for month 0 (intercalary).
 * Returns -1 for invalid. */
int celtic_tree_month_days(int month);

/* Which tree month contains a given Wheel of the Year festival?
 * Index 0-7 maps to WHEEL_SAMHAIN..WHEEL_MABON.
 * Returns month number (1-13), or -1 for invalid index. */
int celtic_tree_for_festival(int wheel_festival_index);

#endif /* TIME_CELTIC_TREE_H */
