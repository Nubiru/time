/* audio_focus_timbre.h -- Per-focus-mode sonic character profiles.
 *
 * Each focus mode (A/K/I/C/D/T) gets a distinct sonic identity beyond
 * just harmonic partials: brightness (spectral tilt), attack time,
 * reverb character, and descriptive waveform type.
 *
 * Design informed by:
 *   - Digest 078 (Music of the Spheres): Kepler voice assignments
 *   - Digest 080 (Brain on Music): timbre segregation parameters
 *   - MEGA inbox 2026-03-22: per-system sonic character spec
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_AUDIO_FOCUS_TIMBRE_H
#define TIME_AUDIO_FOCUS_TIMBRE_H

#define AFT_MAX_PARTIALS 8

/* Sonic character profile for a focus mode */
typedef struct {
    int         focus_mode;                     /* focus_mode_e value (1-6) */
    const char *name;                           /* "Astrology", "Kin", etc. */
    const char *timbre_desc;                    /* "crystalline glass", etc. */
    float       base_freq_hz;                   /* fundamental frequency */
    int         waveform;                       /* audio_waveform_t */
    int         partial_count;                  /* active harmonics (1-8) */
    float       partials[AFT_MAX_PARTIALS];     /* harmonic amplitudes */
    float       base_amplitude;                 /* default volume (0-1) */
    float       brightness;                     /* spectral tilt: 0=dark, 1=bright */
    float       attack_s;                       /* attack time in seconds */
    float       reverb_boost;                   /* additional reverb when focused (0-0.3) */
} audio_focus_timbre_t;

/* Get sonic profile for a focus mode (1-6).
 * Returns a zero-amplitude profile for mode 0 (overview) or invalid. */
audio_focus_timbre_t audio_focus_timbre_get(int focus_mode);

/* Check if a focus mode has a defined sonic profile. */
int audio_focus_timbre_has(int focus_mode);

/* Get the focus timbre name. Returns "Overview" for mode 0. */
const char *audio_focus_timbre_name(int focus_mode);

/* Number of defined focus timbres (modes with sonic profiles). */
int audio_focus_timbre_count(void);

/* Apply focus timbre to oscillator parameters.
 * Writes frequency, amplitude, waveform, partials into the provided arrays
 * at the given slot index. Returns 1 if applied, 0 if no timbre for mode. */
int audio_focus_timbre_apply(int focus_mode, int slot,
                             float *frequencies, float *amplitudes,
                             int *waveform_types, int *harmonic_counts,
                             float harmonic_amps[][AFT_MAX_PARTIALS],
                             float *pan_positions);

#endif /* TIME_AUDIO_FOCUS_TIMBRE_H */
