/* brain_audio_bridge.h — Bridge: brain intelligence → audio event pipeline
 *
 * Converts brain scan results (correlations, insights, convergence strength)
 * into audio-consumable event parameters. Unblocks AUDIO stream L2.4.
 *
 * Mapping:
 *   correlation types  → consonance (structural=consonant, coincidental=dissonant)
 *   convergence_strength → density (0.0=silence, 1.0=full activation)
 *   insight count/type → event_intensity + dominant_insight
 *   system involvement → system_count for spatial mixing
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BRAIN_AUDIO_BRIDGE_H
#define TIME_BRAIN_AUDIO_BRIDGE_H

#include "brain_types.h"

/* Audio event derived from brain analysis */
typedef struct {
    float  consonance;          /* 0.0=dissonant, 1.0=consonant */
    float  convergence_density; /* 0.0=no convergences, 1.0=rare alignment */
    int    convergence_count;   /* number of active insights */
    float  event_intensity;     /* 0.0-1.0 combined intensity */
    float  surprise_factor;     /* 0.0=expected, 1.0=highly anomalous */
    int    dominant_insight;    /* br_insight_type_t of top insight */
    int    system_count;        /* total systems involved across insights */
} br_audio_event_t;

/* Convert brain scan result into audio event parameters.
 * out is zeroed internally. If result is NULL, out is zeroed.
 * If out is NULL, does nothing. */
void br_audio_from_result(const br_result_t *result, br_audio_event_t *out);

/* Map insight type to suggested audio response intensity.
 * Returns 0.0 (subtle) to 1.0 (dramatic). 0.0 for invalid. */
float br_audio_insight_intensity(br_insight_type_t type);

/* Map correlation type to consonance value.
 * STRUCTURAL=1.0, HARMONIC=0.75, APPROXIMATE=0.5, COINCIDENTAL=0.25.
 * Returns 0.0 for invalid types. */
float br_audio_correlation_consonance(br_correlation_type_t type);

/* Map convergence strength (0.0-1.0) to audio density.
 * Clamped to [0.0, 1.0]. */
float br_audio_convergence_density(double convergence_strength);

#endif /* TIME_BRAIN_AUDIO_BRIDGE_H */
