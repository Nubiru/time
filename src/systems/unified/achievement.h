/* achievement — historical astronomical achievement data module.
 * Pure lookup module: static const data, no malloc, no globals, no side effects.
 * Catalogs ancient astronomers' computational precision vs modern values. */

#ifndef TIME_ACHIEVEMENT_H
#define TIME_ACHIEVEMENT_H

/* Achievement categories */
typedef enum {
    ACH_CAT_ORBITAL = 0,     /* Orbital period, year length */
    ACH_CAT_GEOMETRIC,       /* Earth size, distances */
    ACH_CAT_LUNAR,           /* Lunar cycle, eclipse prediction */
    ACH_CAT_PLANETARY,       /* Planet positions, cycles */
    ACH_CAT_CALENDAR,        /* Calendar accuracy, intercalation */
    ACH_CAT_MATHEMATICAL,    /* Mathematical methods, equations */
    ACH_CAT_COUNT
} achievement_category_t;

/* A single historical achievement */
typedef struct {
    int id;                        /* Unique index */
    const char *person;            /* "Aryabhata", "Hillel II", etc. */
    int year;                      /* Negative for BCE (e.g., -240 for 240 BCE) */
    const char *culture;           /* "Indian", "Jewish", "Maya", etc. */
    const char *description;       /* What was computed/discovered */
    double value_computed;         /* Their measured/computed value */
    double modern_value;           /* Modern accepted value */
    const char *unit;              /* "km", "days", "degrees", etc. */
    double absolute_error;         /* |computed - modern| */
    double relative_error_pct;     /* |(computed - modern) / modern| * 100 */
    const char *tools_available;   /* "naked eye", "gnomon", "armillary sphere", etc. */
    const char *tools_NOT_available; /* "telescope", "clock", etc. */
    achievement_category_t category;
    const char *significance;      /* Why this matters */
} achievement_t;

/* Total number of achievements. */
int achievement_count(void);

/* Get achievement by index. Returns entry with id=-1 if invalid. */
achievement_t achievement_get(int index);

/* Get achievements by culture. Fills out_indices (max out_max entries).
 * Returns count found. */
int achievement_by_culture(const char *culture, int *out_indices, int out_max);

/* Get achievements by person. Fills out_indices. Returns count found. */
int achievement_by_person(const char *person, int *out_indices, int out_max);

/* Get achievements by category. Fills out_indices. Returns count found. */
int achievement_by_category(achievement_category_t cat, int *out_indices, int out_max);

/* Error ratio: modern_value / absolute_error (how many times more precise than the error).
 * Higher = more impressive. Returns 0.0 if invalid or zero error. */
double achievement_error_ratio(int index);

/* Accuracy percentage: 100.0 - relative_error_pct. Returns 0.0 if invalid. */
double achievement_accuracy_pct(int index);

/* Category name string. Returns "?" for invalid. */
const char *achievement_category_name(achievement_category_t cat);

/* How many distinct cultures are represented? */
int achievement_culture_count(void);

/* Get distinct culture name by index. Returns "?" for invalid. */
const char *achievement_culture_get(int index);

/* How many distinct persons are represented? */
int achievement_person_count(void);

#endif /* TIME_ACHIEVEMENT_H */
