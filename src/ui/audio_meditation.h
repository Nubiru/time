/* audio_meditation.h -- Meditation mode computation for audio.
 *
 * Computes phi-timed breathing amplitude and binaural beat frequency
 * offsets for meditation mode. The breathing cycle uses the golden ratio:
 *   Inhale duration:  phi   seconds (~1.618s)
 *   Exhale duration:  phi^2 seconds (~2.618s)
 *   Full cycle:       phi + phi^2 = phi^3 seconds (~4.236s)
 *
 * Binaural beats: slight frequency offset between left/right channels
 * to entrain theta brainwaves (4-8Hz range for meditation).
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_AUDIO_MEDITATION_H
#define TIME_AUDIO_MEDITATION_H

/* Meditation breathing phase */
typedef enum {
    MED_PHASE_INHALE = 0,
    MED_PHASE_EXHALE = 1
} med_phase_t;

/* Meditation state snapshot */
typedef struct {
    int active;               /* 1 if meditation mode is on */
    float breath_factor;      /* 0.3 (exhale trough) to 1.0 (inhale peak) */
    med_phase_t breath_phase; /* current breathing phase */
    float breath_progress;    /* 0.0-1.0 progress within current phase */
    float binaural_offset_hz; /* frequency offset for binaural beats (e.g., 6.0) */
    float reverb_boost;       /* extra reverb wet for meditation (0.0-0.3) */
    float dimming;            /* visual dimming factor 0.0=normal, 1.0=full dim */
    int   chakra_active;      /* 1 if a chakra is selected, 0 if not */
    int   chakra_id;          /* 0-6 (CHAKRA_MULADHARA to CHAKRA_SAHASRARA) */
    float chakra_freq_hz;     /* Solfeggio frequency for selected chakra */
} med_state_t;

/* Compute meditation breathing factor from real-time clock.
 * Returns amplitude factor: peaks during inhale (1.0), troughs during
 * exhale (0.3). Uses golden ratio timing: inhale=phi sec, exhale=phi^2 sec.
 * If not in meditation (active=0), returns 1.0. */
float med_breath_factor(double real_time_sec);

/* Get the current breathing phase (inhale or exhale). */
med_phase_t med_breath_phase(double real_time_sec);

/* Get progress within current breathing phase (0.0 to 1.0). */
float med_breath_progress(double real_time_sec);

/* Get the full meditation state for a given time.
 * binaural_hz: desired binaural beat frequency (typically 4-8 Hz for theta).
 * Returns complete state snapshot. */
med_state_t med_compute(double real_time_sec, float binaural_hz);

/* Compute meditation state with optional chakra focus.
 * chakra: -1 for no chakra, 0-6 for specific chakra.
 * binaural_hz: binaural beat frequency (typically 4-8 Hz). */
med_state_t med_compute_chakra(double real_time_sec, float binaural_hz,
                               int chakra);

/* Get the phi breathing cycle duration (phi + phi^2 = phi^3 seconds). */
float med_cycle_duration(void);

/* Get inhale duration (phi seconds). */
float med_inhale_duration(void);

/* Get exhale duration (phi^2 seconds). */
float med_exhale_duration(void);

#endif /* TIME_AUDIO_MEDITATION_H */
