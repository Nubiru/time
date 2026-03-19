/* brain_predict.h — Future event feed (Track 26, L3.2)
 *
 * Scans upcoming days for convergences, cycle milestones, and festivals.
 * Ranks by significance and returns the most interesting upcoming events.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_PREDICT_H
#define TIME_BRAIN_PREDICT_H

#include "brain_types.h"

/* Maximum events in a prediction result */
#define BR_PREDICT_MAX 16

/* Maximum lookahead (days) */
#define BR_PREDICT_MAX_DAYS 365

/* A predicted future event */
typedef struct {
    double          jd;                     /* when it occurs */
    int             days_from_now;          /* days until event */
    double          significance;           /* 0.0-1.0 */
    char            headline[BR_HEADLINE_MAX];
} br_predicted_event_t;

/* Prediction result */
typedef struct {
    br_predicted_event_t events[BR_PREDICT_MAX];
    int                  count;
    double               start_jd;          /* scan start date */
    int                  days_scanned;       /* how far ahead we looked */
} br_prediction_t;

/* Scan the next days_ahead days for significant events.
 * Events are ranked by significance (highest first).
 * days_ahead is clamped to BR_PREDICT_MAX_DAYS.
 * Returns number of events found. */
int br_predict(double jd, int days_ahead, br_prediction_t *out);

/* Find the next single significant event (significance >= min_sig).
 * Returns JD of the event, or 0.0 if none found within days_ahead. */
double br_predict_next(double jd, int days_ahead, double min_sig);

#endif /* TIME_BRAIN_PREDICT_H */
