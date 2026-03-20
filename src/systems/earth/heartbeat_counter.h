/* heartbeat_counter.h — Global heartbeat counter since first humans.
 * Estimates total heartbeats of all humans since first Homo sapiens
 * (~300,000 years ago). Uses population curve x average heartrate
 * (72 bpm) x time. Pure data + computation.
 * No GL, no malloc, no globals, no side effects.
 *
 * Sources: Population Reference Bureau, biorhythm.h (72 bpm average). */

#ifndef TIME_HEARTBEAT_COUNTER_H
#define TIME_HEARTBEAT_COUNTER_H

#define HB_TEXT_MAX 256

/* Heartbeat counter snapshot for a given year */
typedef struct {
    double total_exabeats;        /* cumulative heartbeats in units of 1e18 */
    double beats_per_second;      /* current rate (pop_alive * 72 / 60) */
    double beats_per_year;        /* current annual rate */
    char total_text[HB_TEXT_MAX]; /* "~3,500 exabeats" */
    char rate_text[HB_TEXT_MAX];  /* "~9.7 billion heartbeats per second" */
    char poetic_text[HB_TEXT_MAX]; /* "Every second, 9.7 billion hearts beat as one" */
} hb_counter_t;

/* Compute heartbeat statistics for a given CE year.
 * Negative years for BCE (e.g., -10000 for 10000 BCE). */
hb_counter_t hb_compute(int ce_year);

/* Estimate cumulative heartbeats (exabeats, 1e18) from first humans
 * to given year. Uses pre-computed anchor points with interpolation. */
double hb_total_exabeats(int ce_year);

/* Current heartbeats per second worldwide at given year.
 * = pop_alive_at_year(year) * 1e6 * 72.0 / 60.0 */
double hb_beats_per_second(int ce_year);

/* Format counter as single-line display text. Returns chars written.
 * Returns 0 if c, buf, or buf_size invalid. */
int hb_format(const hb_counter_t *c, char *buf, int buf_size);

#endif /* TIME_HEARTBEAT_COUNTER_H */
