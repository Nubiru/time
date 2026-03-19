/* audio_event.h -- Audio event detection from aspects and convergences.
 *
 * Scans a Julian Day for musically-relevant events:
 * - Planetary aspects (conjunction, trine, etc.) -> consonance score
 * - Calendar convergences (cross-system alignments) -> harmonic density
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_AUDIO_EVENT_H
#define TIME_AUDIO_EVENT_H

#define AE_MAX_ACTIVE_ASPECTS 28 /* 8 choose 2 */

/* Summary of audio-relevant events at a given time */
typedef struct {
    int   aspect_count;        /* number of active aspects found */
    float consonance;          /* 0.0=dissonant, 1.0=consonant (weighted avg) */
    float tightest_orb;        /* smallest orb among active aspects (degrees) */
    float convergence_density; /* 0.0=no convergences, 1.0=rare alignment */
    int   convergence_count;   /* number of convergence events */
    float event_intensity;     /* 0.0-1.0 combined intensity */
} audio_event_summary_t;

/* Scan a Julian Day for audio-relevant events.
 * Combines aspect detection and convergence scanning.
 * max_orb: maximum orb for aspect detection (degrees, typically 8.0). */
audio_event_summary_t audio_event_scan(double jd, double max_orb);

/* Get overall consonance from current planetary aspects only.
 * Returns weighted average of all active aspect consonance values.
 * Weight = (1 - orb/max_orb) so tighter aspects count more.
 * If no aspects found, returns 0.5 (neutral). */
float audio_event_consonance(double jd, double max_orb);

/* Get convergence density (0.0-1.0).
 * Simple wrapper around cd_significance. */
float audio_event_density(double jd);

/* Combined event intensity (0.0-1.0).
 * Blends aspect consonance and convergence density:
 * intensity = 0.6 * consonance + 0.4 * density */
float audio_event_intensity(double jd, double max_orb);

#endif /* TIME_AUDIO_EVENT_H */
