/* kalpa.h — Buddhist cosmic time cycle data module
 * Maha-kalpa hierarchy: formation, existence, destruction, emptiness.
 * Antarakalpa lifespan cycles within the existence phase.
 * Mappings from geological Ma and cosmic age to kalpa position.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_KALPA_H
#define TIME_KALPA_H

#include <stdbool.h>

/* Buddhist cosmic time hierarchy (Theravada Abhidhamma):
 * 1 antarakalpa = ~16.8 million years
 * 1 kalpa = 20 antarakalpas = ~336 million years
 * 1 maha-kalpa = 4 kalpas = ~1.344 billion years */

#define BUDDHIST_ANTARAKALPA_YEARS    16800000.0
#define BUDDHIST_KALPA_ANTARAKALPAS   20
#define BUDDHIST_KALPA_YEARS          (BUDDHIST_ANTARAKALPA_YEARS * BUDDHIST_KALPA_ANTARAKALPAS)
#define BUDDHIST_MAHA_KALPA_KALPAS    4
#define BUDDHIST_MAHA_KALPA_YEARS     (BUDDHIST_KALPA_YEARS * BUDDHIST_MAHA_KALPA_KALPAS)

typedef enum {
    KALPA_PHASE_FORMATION = 0,
    KALPA_PHASE_EXISTENCE,
    KALPA_PHASE_DESTRUCTION,
    KALPA_PHASE_EMPTINESS,
    KALPA_PHASE_COUNT
} kalpa_phase_t;

typedef struct {
    int maha_kalpa_number;
    kalpa_phase_t phase;
    int antarakalpa_number;
    double phase_fraction;
    double maha_kalpa_fraction;
    double years_in_phase;
} kalpa_position_t;

typedef struct {
    int antarakalpa_number;
    bool is_increasing;
    double max_lifespan_years;
    double min_lifespan_years;
    double current_lifespan;
} antarakalpa_info_t;

/* Compute kalpa position from a raw cosmic age in years. */
kalpa_position_t kalpa_position(double cosmic_age);

/* English name of a kalpa phase. Returns "?" for invalid. */
const char *kalpa_phase_name(kalpa_phase_t phase);

/* Pali name of a kalpa phase. Returns "?" for invalid. */
const char *kalpa_phase_meaning(kalpa_phase_t phase);

/* Antarakalpa lifespan cycle info.
 * antarakalpa_number: 0-19 within existence phase.
 * fraction_within: 0.0-1.0 position within the antarakalpa. */
antarakalpa_info_t kalpa_antarakalpa_info(int antarakalpa_number,
                                          double fraction_within);

/* Map geological Ma (millions of years ago) to kalpa position.
 * Assumes current maha-kalpa started ~4.54 billion years ago. */
kalpa_position_t kalpa_from_geological(double ma);

/* Map cosmic age (years since Big Bang) to kalpa position. */
kalpa_position_t kalpa_from_cosmic_age(double years_since_big_bang);

/* Number of phases in a maha-kalpa (always 4). */
int kalpa_phase_count(void);

/* Duration of one kalpa phase in years. */
double kalpa_phase_duration(void);

/* Duration of one maha-kalpa in years. */
double kalpa_maha_kalpa_duration(void);

/* Duration of one antarakalpa in years. */
double kalpa_antarakalpa_duration(void);

/* Ratio of maha-kalpa to estimated age of universe (~13.8 billion years). */
double kalpa_cosmic_ratio(void);

#endif /* TIME_KALPA_H */
