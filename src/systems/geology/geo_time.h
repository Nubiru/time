/* geo_time.h — International Chronostratigraphic Chart data (ICS 2024).
 * Lookup functions for geological time: eons, eras, periods, epochs,
 * extinction events. All pure functions, no side effects. */

#ifndef TIME_GEO_TIME_H
#define TIME_GEO_TIME_H

/* ICS standard colors as RGB (0-255) */
typedef struct {
    int r, g, b;
} geo_color_t;

/* Geological time unit */
typedef struct {
    const char *name;
    double start_ma;   /* start in millions of years ago */
    double end_ma;     /* end in millions of years ago (0 = present) */
    geo_color_t color; /* ICS standard color */
} geo_unit_t;

/* Eon-level lookup (4 eons: Hadean, Archean, Proterozoic, Phanerozoic) */
int geo_eon_count(void);
geo_unit_t geo_eon_at(double ma);
geo_unit_t geo_eon_by_index(int index);

/* Era-level lookup (10 eras) */
int geo_era_count(void);
geo_unit_t geo_era_at(double ma);
geo_unit_t geo_era_by_index(int index);

/* Period-level lookup (22 periods) */
int geo_period_count(void);
geo_unit_t geo_period_at(double ma);
geo_unit_t geo_period_by_index(int index);

/* Epoch-level lookup (34 epochs) */
int geo_epoch_count(void);
geo_unit_t geo_epoch_at(double ma);
geo_unit_t geo_epoch_by_index(int index);

/* Convert Julian Day to millions of years ago (approximate).
 * Uses J2000.0 as reference: ma = (J2000_JD - jd) / 365.25 / 1e6 */
double geo_jd_to_ma(double jd);

/* Major extinction events (the Big Five) */
typedef struct {
    const char *name;
    double ma;          /* millions of years ago */
    const char *cause;  /* brief description */
} geo_extinction_t;

int geo_extinction_count(void);
geo_extinction_t geo_extinction_by_index(int index);
geo_extinction_t geo_extinction_nearest(double ma);

#endif /* TIME_GEO_TIME_H */
