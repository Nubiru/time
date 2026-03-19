/* audio_score.h -- Pure audio score computation module.
 *
 * Computes WHAT should be heard from the current time state.
 * No WebAudio, no sound output -- only pure parameters.
 *
 * Uses planetary frequencies from audio_data.h, view warmth from
 * view_registry.h, and a simplified lunar-phase tension model.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_AUDIO_SCORE_H
#define TIME_AUDIO_SCORE_H

#define AS_MAX_PLANETS 9
#define AS_MAX_HARMONICS 8

typedef struct {
    float frequencies[AS_MAX_PLANETS];  /* per-planet Hz (transposed to audible) */
    float amplitudes[AS_MAX_PLANETS];   /* per-planet volume (0-1) */
    int   planet_count;                 /* how many planets in chord */
    float master_volume;                /* overall level (0-1) */
    float tension;                      /* 0=consonant, 1=maximum dissonance */
    float warmth;                       /* -1=cold (galaxy), 0=neutral (space), +1=warm (earth) */
    float tempo_bpm;                    /* rhythm rate from time speed */
    int   waveform_types[AS_MAX_PLANETS];              /* audio_waveform_t per planet */
    int   harmonic_counts[AS_MAX_PLANETS];             /* number of partials per planet */
    float harmonic_amps[AS_MAX_PLANETS][AS_MAX_HARMONICS]; /* partial amplitudes (normalized) */
    float reverb_wet;                                  /* 0.0=fully dry, 1.0=fully wet */
    float reverb_decay_s;                              /* reverb impulse response duration */
    float moon_factor;                                 /* 0.0-1.0 current moon envelope value */
    float pan_positions[AS_MAX_PLANETS];               /* -1.0=left, 0.0=center, 1.0=right */
} audio_params_t;

/* Compute full audio parameters from current state.
 * jd: Julian Day (for planet positions)
 * view_id: current view (VIEW_SPACE, VIEW_EARTH, etc.)
 * log_zoom: current camera zoom level
 * time_speed: simulation speed multiplier (1.0 = real-time) */
audio_params_t audio_score_compute(double jd, int view_id, float log_zoom,
                                   double time_speed);

/* Get the planetary chord -- all audible planet frequencies + amplitudes.
 * Returns number of planets written (up to max). */
int audio_score_chord(double jd, float *frequencies, float *amplitudes,
                      int max);

/* Get convergence tension level (0.0=no convergence, 1.0=rare alignment).
 * Uses simplified lunar-phase approximation. */
float audio_score_tension(double jd);

/* Get viewpoint warmth (-1=galaxy/cold, 0=space/neutral, +1=earth/warm).
 * Maps view_id and zoom level to a warmth value. */
float audio_score_warmth(int view_id, float log_zoom);

/* Map time simulation speed to tempo BPM.
 * Real-time (1.0x) = 60 BPM, faster = higher BPM, paused = 0. */
float audio_score_tempo(double time_speed);

/* Get a descriptive name for current audio mood. */
const char *audio_score_mood(float warmth, float tension);

#endif /* TIME_AUDIO_SCORE_H */
