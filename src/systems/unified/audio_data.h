/* audio_data.h -- Pure data module for audio synthesis parameters.
 *
 * Extends frequency.h with harmonic series per planet (overtones
 * based on orbital eccentricity), aspect sonification mappings,
 * moon phase amplitude envelopes, and musical interval tables.
 *
 * Provides all data needed for the WebAudio engine without
 * touching any audio APIs.
 *
 * Planet table (10 entries):
 *   0=Earth day, 1=Mercury, 2=Venus, 3=Earth year, 4=Mars,
 *   5=Jupiter, 6=Saturn, 7=Uranus, 8=Neptune, 9=Moon (synodic)
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_AUDIO_DATA_H
#define TIME_AUDIO_DATA_H

#include <stdbool.h>

#define AUDIO_MAX_HARMONICS 8
#define AUDIO_MAX_PLANETS   10

/* Aspect types for sonification */
typedef enum {
    AUDIO_ASPECT_CONJUNCTION = 0,  /* 0 degrees */
    AUDIO_ASPECT_SEXTILE,          /* 60 degrees */
    AUDIO_ASPECT_SQUARE,           /* 90 degrees */
    AUDIO_ASPECT_TRINE,            /* 120 degrees */
    AUDIO_ASPECT_OPPOSITION,       /* 180 degrees */
    AUDIO_ASPECT_COUNT
} audio_aspect_t;

/* Waveform type */
typedef enum {
    AUDIO_WAVE_SINE = 0,
    AUDIO_WAVE_TRIANGLE,
    AUDIO_WAVE_SAWTOOTH,
    AUDIO_WAVE_COUNT
} audio_waveform_t;

/* Harmonic overtone */
typedef struct {
    int harmonic_number;    /* 1 = fundamental, 2 = first overtone, etc. */
    double amplitude;       /* relative amplitude (0.0 - 1.0) */
    double freq_ratio;      /* frequency ratio relative to fundamental */
} audio_harmonic_t;

/* Planet audio profile */
typedef struct {
    int planet_index;              /* index into freq_planet() */
    double fundamental_hz;         /* audible fundamental frequency */
    int harmonic_count;            /* number of harmonics (1-AUDIO_MAX_HARMONICS) */
    audio_harmonic_t harmonics[AUDIO_MAX_HARMONICS];
    audio_waveform_t waveform;     /* suggested waveform */
    double base_amplitude;         /* default amplitude (0.0 - 1.0) */
} audio_planet_profile_t;

/* Aspect sonification mapping */
typedef struct {
    audio_aspect_t aspect;
    const char *name;              /* "conjunction", "sextile", etc. */
    double angle_deg;              /* 0, 60, 90, 120, 180 */
    const char *musical_interval;  /* "unison", "minor third", "fourth", "fifth", "octave" */
    double consonance;             /* 0.0 (dissonant) to 1.0 (consonant) */
    double volume_factor;          /* amplitude multiplier when aspect is active */
} audio_aspect_map_t;

/* Moon phase envelope point */
typedef struct {
    double phase;          /* 0.0 = new moon, 0.5 = full moon, 1.0 = next new */
    double amplitude;      /* amplitude at this phase (0.0 - 1.0) */
} audio_moon_point_t;

/* Get planet audio profile by index (0-9). */
audio_planet_profile_t audio_planet_profile(int planet_index);

/* Get the audible frequency for a planet. */
double audio_planet_freq(int planet_index);

/* Get the harmonic series for a planet. Returns harmonic count. */
int audio_planet_harmonics(int planet_index, audio_harmonic_t *out, int max_harmonics);

/* Get aspect sonification mapping. */
audio_aspect_map_t audio_aspect_map(audio_aspect_t aspect);

/* Get the musical harmony description for an aspect. */
const char *audio_aspect_harmony(audio_aspect_t aspect);

/* Get aspect consonance value (0.0 = dissonant, 1.0 = consonant). */
double audio_aspect_consonance(audio_aspect_t aspect);

/* Compute moon amplitude envelope at a given phase (0.0-1.0). */
double audio_moon_envelope(double moon_phase);

/* Get the interval relationship between two planets. Returns interval name. */
const char *audio_interval_name(int planet_a, int planet_b);

/* Compute suggested base amplitude for a planet (based on distance/size). */
double audio_planet_amplitude(int planet_index);

/* Get aspect type from angle (with orb tolerance). Returns -1 if no aspect. */
int audio_aspect_from_angle(double angle_deg, double orb_deg);

/* Number of aspect types. */
int audio_aspect_count(void);

/* Waveform name string. */
const char *audio_waveform_name(audio_waveform_t waveform);

#endif /* TIME_AUDIO_DATA_H */
