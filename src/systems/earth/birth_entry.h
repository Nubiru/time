/* birth_entry.h — Birth date entry state machine.
 *
 * Pure data layer for the birth date entry flow.
 * Three steps: Date -> Location -> Ready for Reveal.
 * Manages input field state: day/month/year spinning wheels,
 * validation, location coordinates, and reveal readiness.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * All functions take state by value and return new state by value. */

#ifndef TIME_BIRTH_ENTRY_H
#define TIME_BIRTH_ENTRY_H

/* Year range for valid birth dates */
#define BE_YEAR_MIN 1900
#define BE_YEAR_MAX 2100

/* --- Steps in the entry flow --- */

typedef enum {
    BE_STEP_DATE = 0,     /* User is entering date */
    BE_STEP_LOCATION,     /* User is entering location */
    BE_STEP_READY,        /* All data entered, ready for reveal */
    BE_STEP_COUNT
} be_step_t;

/* --- Which date field has focus --- */

typedef enum {
    BE_FIELD_DAY = 0,
    BE_FIELD_MONTH,
    BE_FIELD_YEAR,
    BE_FIELD_COUNT
} be_field_t;

/* --- Date entry state --- */

typedef struct {
    int day;              /* 1-31 */
    int month;            /* 1-12 */
    int year;             /* 1900-2100 */
    be_field_t focus;     /* which field is active for spinning wheel */
    int valid;            /* 1 if current date is valid, 0 if not */
} be_date_t;

/* --- Location entry state --- */

typedef struct {
    double latitude;      /* -90 to 90 */
    double longitude;     /* -180 to 180 */
    int set;              /* 1 if user selected a location */
    int skipped;          /* 1 if user chose to skip */
} be_location_t;

/* --- Complete birth entry state --- */

typedef struct {
    be_step_t step;
    be_date_t date;
    be_location_t location;
} be_entry_t;

/* --- Creation --- */

/* Create initial entry state with today's date as starting point. */
be_entry_t be_create(int today_year, int today_month, int today_day);

/* --- Date manipulation --- */

/* Increment/decrement the focused field by delta (+1 or -1).
 * Day wraps 1<->max, month wraps 1<->12, year clamps to range.
 * Day is clamped if month/year change makes it out of range. */
be_entry_t be_spin(be_entry_t entry, int delta);

/* Set focus to a specific date field. */
be_entry_t be_focus(be_entry_t entry, be_field_t field);

/* Set date directly. Updates valid flag. */
be_entry_t be_set_date(be_entry_t entry, int year, int month, int day);

/* Validate a date (checks month lengths, leap years, year range). */
int be_date_valid(const be_date_t *date);

/* Days in month (accounts for leap years). Returns 0 for invalid month. */
int be_days_in_month(int year, int month);

/* Check if year is a leap year. */
int be_is_leap_year(int year);

/* --- Step transitions --- */

/* Confirm date and advance to location step.
 * Returns unchanged if date invalid or not on DATE step. */
be_entry_t be_confirm_date(be_entry_t entry);

/* Set location and advance to ready.
 * Coordinates are clamped to valid ranges.
 * Only works from LOCATION step. */
be_entry_t be_set_location(be_entry_t entry, double lat, double lon);

/* Skip location and advance to ready.
 * Only works from LOCATION step. */
be_entry_t be_skip_location(be_entry_t entry);

/* Go back one step. DATE stays on DATE. READY->LOCATION. LOCATION->DATE.
 * Going back from READY resets location set/skipped flags. */
be_entry_t be_back(be_entry_t entry);

/* --- Queries --- */

/* Get human-readable step name. */
const char *be_step_name(be_step_t step);

/* Is the entry ready for reveal? (step == READY) */
int be_is_ready(const be_entry_t *entry);

/* Format date as "DD Month YYYY" into buffer.
 * Returns number of characters written, or 0 if buffer too small. */
int be_format_date(const be_entry_t *entry, char *buf, int buf_size);

#endif /* TIME_BIRTH_ENTRY_H */
