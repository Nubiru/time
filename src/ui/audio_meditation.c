/* audio_meditation.c -- Meditation mode computation.
 *
 * Phi-timed breathing cycle with cosine-smoothed amplitude curves.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "audio_meditation.h"
#include "golden_layout.h"
#include <math.h>

#define MED_PI 3.14159265358979323846

/* Phi breathing cycle:
 * Inhale: PHI_RATIO seconds (~1.618s)
 * Exhale: PHI_SQ seconds (~2.618s)
 * Total cycle: PHI_RATIO + PHI_SQ = PHI_CU (~4.236s) */

static const float INHALE_DUR = (float)PHI_RATIO;   /* ~1.618s */
static const float EXHALE_DUR = (float)PHI_SQ;      /* ~2.618s */
static const float CYCLE_DUR  = (float)(PHI_RATIO + PHI_SQ); /* ~4.236s */

/* Breath amplitude range */
static const float BREATH_MIN = 0.3f; /* exhale trough */
static const float BREATH_MAX = 1.0f; /* inhale peak */

float med_cycle_duration(void)
{
    return CYCLE_DUR;
}

float med_inhale_duration(void)
{
    return INHALE_DUR;
}

float med_exhale_duration(void)
{
    return EXHALE_DUR;
}

/* Get position within the breathing cycle (0.0 to CYCLE_DUR) */
static float cycle_position(double real_time_sec)
{
    double pos = fmod(real_time_sec, (double)CYCLE_DUR);
    if (pos < 0.0)
        pos += (double)CYCLE_DUR;
    return (float)pos;
}

med_phase_t med_breath_phase(double real_time_sec)
{
    float pos = cycle_position(real_time_sec);
    return (pos < INHALE_DUR) ? MED_PHASE_INHALE : MED_PHASE_EXHALE;
}

float med_breath_progress(double real_time_sec)
{
    float pos = cycle_position(real_time_sec);
    if (pos < INHALE_DUR) {
        return pos / INHALE_DUR;
    }
    return (pos - INHALE_DUR) / EXHALE_DUR;
}

float med_breath_factor(double real_time_sec)
{
    float pos = cycle_position(real_time_sec);

    /* Smooth cosine curves for natural breathing feel */
    float factor;
    if (pos < INHALE_DUR) {
        /* Inhale: rise from BREATH_MIN to BREATH_MAX */
        float t = pos / INHALE_DUR; /* 0 to 1 */
        /* Cosine rise: starts slow, accelerates, ends slow */
        factor = BREATH_MIN
                 + (BREATH_MAX - BREATH_MIN) * 0.5f
                       * (1.0f - cosf((float)MED_PI * t));
    } else {
        /* Exhale: fall from BREATH_MAX to BREATH_MIN */
        float t = (pos - INHALE_DUR) / EXHALE_DUR; /* 0 to 1 */
        /* Cosine fall: starts slow, accelerates, ends slow */
        factor = BREATH_MAX
                 - (BREATH_MAX - BREATH_MIN) * 0.5f
                       * (1.0f - cosf((float)MED_PI * t));
    }

    return factor;
}

med_state_t med_compute(double real_time_sec, float binaural_hz)
{
    med_state_t state;
    state.active = 1;
    state.breath_factor = med_breath_factor(real_time_sec);
    state.breath_phase = med_breath_phase(real_time_sec);
    state.breath_progress = med_breath_progress(real_time_sec);
    state.binaural_offset_hz = (binaural_hz > 0.0f) ? binaural_hz : 6.0f;
    state.reverb_boost = 0.25f; /* meditation gets more reverb */
    state.dimming = 0.7f;       /* dim visuals to 30% brightness */
    return state;
}
