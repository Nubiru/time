/* yuga.h -- Hindu Yuga cycle data and position calculations.
 * Covers the 4 Yugas (Satya, Treta, Dvapara, Kali) and larger
 * cycles (Maha-Yuga, Manvantara, Kalpa). All pure functions.
 * Data source: Surya Siddhanta Ch. 12. */

#ifndef TIME_YUGA_H
#define TIME_YUGA_H

#include <stdbool.h>

#define YUGA_COUNT 4

/* Yuga indices */
#define YUGA_SATYA   0
#define YUGA_TRETA   1
#define YUGA_DVAPARA 2
#define YUGA_KALI    3

/* Fundamental unit: 1 divine year = 360 human years */
#define DIVINE_YEAR 360

/* Cycle lengths in human years */
#define KALI_YUGA_YEARS      432000
#define DVAPARA_YUGA_YEARS   864000
#define TRETA_YUGA_YEARS    1296000
#define SATYA_YUGA_YEARS    1728000
#define MAHA_YUGA_YEARS     4320000    /* Sum of 4 Yugas */

/* Larger cycles */
#define MAHA_YUGAS_PER_KALPA  1000
#define KALPA_YEARS           4320000000LL  /* 1 Day of Brahma */
#define MANVANTARA_YUGAS      71            /* Maha-Yugas per Manvantara */
#define MANVANTARAS_PER_KALPA 14

/* Kali Yuga epoch: 3102 BCE (traditional) = JD 588465.5 */
#define KALI_YUGA_EPOCH_JD   588465.5
#define KALI_YUGA_EPOCH_CE   (-3101)  /* 3102 BCE in astronomical year numbering */

typedef struct {
    int id;                   /* 0-3 (Satya, Treta, Dvapara, Kali) */
    const char *name;         /* Sanskrit name */
    const char *meaning;      /* English meaning */
    const char *description;  /* Brief characterization */
    long long years;          /* Duration in human years */
    int ratio;                /* Ratio factor (4, 3, 2, 1) */
    double dharma_fraction;   /* Dharma present: 1.0, 0.75, 0.5, 0.25 */
} yuga_t;

typedef struct {
    int yuga_index;           /* Current Yuga (0-3) */
    long long years_elapsed;  /* Years elapsed in current Yuga */
    long long years_remaining;/* Years remaining in current Yuga */
    double fraction;          /* 0.0-1.0 position within current Yuga */
    int maha_yuga_number;     /* Which Maha-Yuga cycle (1-based) */
} yuga_position_t;

/* Get Yuga by index (0-3). Returns struct with id=-1 for invalid. */
yuga_t yuga_get(int index);

/* Yuga name by index. Returns "?" for invalid. */
const char *yuga_name(int index);

/* Yuga meaning by index. Returns "?" for invalid. */
const char *yuga_meaning(int index);

/* Get current Yuga position from years since Kali Yuga epoch.
 * years_since_epoch: elapsed years since 3102 BCE (positive = forward).
 * Handles cycling through Maha-Yugas. */
yuga_position_t yuga_position(long long years_since_epoch);

/* Get current Yuga position from Julian Day. */
yuga_position_t yuga_position_from_jd(double jd);

/* Get Yuga for a given year CE (astronomical year numbering).
 * 2026 CE -> Kali Yuga, 5127 years elapsed. */
yuga_position_t yuga_for_year(int ce_year);

/* Years in a Maha-Yuga (sum of 4 Yugas = 4,320,000). */
long long yuga_maha_yuga_years(void);

/* Years in a Kalpa (1000 Maha-Yugas = 4,320,000,000). */
long long yuga_kalpa_years(void);

/* Years in a Manvantara (71 Maha-Yugas = 306,720,000). */
long long yuga_manvantara_years(void);

/* Dharma fraction for a Yuga index (1.0 in Satya, 0.25 in Kali).
 * Returns 0.0 for invalid index. */
double yuga_dharma(int yuga_index);

/* Number of Yugas (always 4). */
int yuga_count(void);

#endif /* TIME_YUGA_H */
