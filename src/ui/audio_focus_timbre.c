/* audio_focus_timbre.c -- Per-focus-mode sonic character data.
 *
 * Defines distinct sonic identity for each keyboard focus mode:
 *   A (1) Astrology  — crystalline glass harmonica, celestial, high harmonics
 *   K (2) Kin Maya   — warm wooden flute, earthy, few overtones
 *   I (3) I Ching    — deep bronze bell, resonant, contemplative
 *   C (4) Chinese    — guzheng pluck, balanced, rich mid harmonics
 *   D (5) Human Design — electronic pulse, modern, rhythmic
 *   T (6) Kabbalah   — sustained harmonic chord, mysterious, full series
 *
 * Brightness controls spectral emphasis:
 *   0.0 = dark (only fundamental+low partials)
 *   0.5 = neutral
 *   1.0 = bright (high partials emphasized)
 *
 * Attack time controls onset character:
 *   0.005-0.01s = percussive (bell, pluck)
 *   0.05-0.1s = moderate (flute, string)
 *   0.15-0.3s = sustained (pad, drone)
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "audio_focus_timbre.h"

#define TIMBRE_COUNT 6

/* FOCUS_MODE_OVERVIEW (0) has no timbre — it's the default state */
static const audio_focus_timbre_t TIMBRES[TIMBRE_COUNT] = {

    /* A (1): Astrology — glass harmonica, celestial
     * Bright, ethereal, high partial emphasis. Clear and ringing.
     * Inspired by: crystal singing bowls, glass armonica (Franklin). */
    {1, "Astrology", "crystalline glass",
     392.0f,   /* G4 — celestial, bright */
     0,        /* sine (pure foundation) */
     6,        /* 6 partials: strong even harmonics for bell-like quality */
     {1.0f, 0.15f, 0.05f, 0.35f, 0.02f, 0.25f, 0.0f, 0.0f},
     0.20f,    /* amplitude */
     0.85f,    /* brightness: very bright — crystalline highs */
     0.08f,    /* attack: moderate — emerges, doesn't strike */
     0.15f},   /* reverb: spacious, celestial reverb */

    /* K (2): Kin Maya — wooden flute, earthy
     * Warm, few overtones, fundamental-heavy. Like a quena or panpipe.
     * Inspired by: Mayan clay ocarinas, quena, panpipe. */
    {2, "Kin Maya", "warm wooden flute",
     330.0f,   /* E4 — warm, mid-range */
     0,        /* sine (flute-like purity) */
     3,        /* 3 partials: almost pure tone, woody */
     {1.0f, 0.15f, 0.05f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
     0.25f,    /* amplitude: slightly louder — earthy presence */
     0.25f,    /* brightness: dark — warm, no sparkle */
     0.10f,    /* attack: breath onset — soft entrance */
     0.08f},   /* reverb: intimate, grounded */

    /* I (3): I Ching — bronze bell, contemplative
     * Deep, resonant, prominent even harmonics (bell spectrum).
     * Inspired by: temple bells, Tibetan singing bowls, Chinese zhong. */
    {3, "I Ching", "deep bronze bell",
     165.0f,   /* E3 — deep, resonant */
     0,        /* sine (bell fundamental) */
     6,        /* 6 partials: rich bell spectrum with non-harmonic character */
     {1.0f, 0.6f, 0.8f, 0.3f, 0.5f, 0.2f, 0.0f, 0.0f},
     0.22f,    /* amplitude */
     0.35f,    /* brightness: warm-neutral — contemplative, not sparkling */
     0.005f,   /* attack: percussive strike — bell hit */
     0.20f},   /* reverb: long, temple-like resonance */

    /* C (4): Chinese — guzheng pluck, balanced
     * Plucked string with moderate sustain. Rich mid harmonics.
     * Inspired by: guzheng, pipa, erhu (bowed). */
    {4, "Chinese", "guzheng pluck",
     262.0f,   /* C4 — middle, balanced */
     0,        /* sine */
     6,        /* 6 partials: plucked string spectrum */
     {1.0f, 0.5f, 0.3f, 0.4f, 0.2f, 0.1f, 0.0f, 0.0f},
     0.22f,    /* amplitude */
     0.50f,    /* brightness: neutral — balanced spectrum */
     0.010f,   /* attack: quick pluck */
     0.05f},   /* reverb: modest — wood resonance, not cathedral */

    /* D (5): Human Design — electronic pulse, modern
     * Synthetic, rhythmic, sawtooth-based with filtered highs.
     * Inspired by: analog synths, pulse wave, electronic hum. */
    {5, "Human Design", "electronic pulse",
     220.0f,   /* A3 — foundation, grounded */
     2,        /* sawtooth — buzzy, electronic character */
     5,        /* 5 partials: sawtooth harmonics, gradually filtered */
     {1.0f, 0.5f, 0.33f, 0.25f, 0.20f, 0.0f, 0.0f, 0.0f},
     0.18f,    /* amplitude: restrained — doesn't overpower */
     0.70f,    /* brightness: fairly bright — electronic edge */
     0.005f,   /* attack: instant — electronic precision */
     0.02f},   /* reverb: dry — modern, precise, no wash */

    /* T (6): Kabbalah — sustained harmonic chord, mysterious
     * Full harmonic series, pad-like sustain. Sefirot resonance.
     * Inspired by: organ, harmonic series, overtone singing. */
    {6, "Kabbalah", "sustained harmonic chord",
     196.0f,   /* G3 — mystical, between earth and heaven */
     1,        /* triangle — smooth but richer than sine */
     8,        /* 8 partials: full harmonic series, all active */
     {1.0f, 0.7f, 0.5f, 0.4f, 0.35f, 0.25f, 0.18f, 0.12f},
     0.20f,    /* amplitude */
     0.40f,    /* brightness: warm-neutral — mysterious, not harsh */
     0.25f,    /* attack: very slow — pad emerges from silence */
     0.25f},   /* reverb: deep, cavernous — like a stone temple */
};

/* Null timbre for overview/invalid modes */
static const audio_focus_timbre_t NULL_TIMBRE = {
    0, "Overview", "none",
    0.0f, 0, 0,
    {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    0.0f, 0.0f, 0.0f, 0.0f
};

audio_focus_timbre_t audio_focus_timbre_get(int focus_mode)
{
    for (int i = 0; i < TIMBRE_COUNT; i++) {
        if (TIMBRES[i].focus_mode == focus_mode) {
            return TIMBRES[i];
        }
    }
    return NULL_TIMBRE;
}

int audio_focus_timbre_has(int focus_mode)
{
    for (int i = 0; i < TIMBRE_COUNT; i++) {
        if (TIMBRES[i].focus_mode == focus_mode) {
            return 1;
        }
    }
    return 0;
}

const char *audio_focus_timbre_name(int focus_mode)
{
    for (int i = 0; i < TIMBRE_COUNT; i++) {
        if (TIMBRES[i].focus_mode == focus_mode) {
            return TIMBRES[i].name;
        }
    }
    return "Overview";
}

int audio_focus_timbre_count(void)
{
    return TIMBRE_COUNT;
}

int audio_focus_timbre_apply(int focus_mode, int slot,
                             float *frequencies, float *amplitudes,
                             int *waveform_types, int *harmonic_counts,
                             float harmonic_amps[][AFT_MAX_PARTIALS],
                             float *pan_positions)
{
    if (!frequencies || !amplitudes || !waveform_types ||
        !harmonic_counts || !harmonic_amps || !pan_positions) {
        return 0;
    }

    audio_focus_timbre_t t = audio_focus_timbre_get(focus_mode);
    if (t.base_amplitude <= 0.0f) {
        return 0;
    }

    frequencies[slot]     = t.base_freq_hz;
    amplitudes[slot]      = t.base_amplitude * 0.5f;  /* 50% blend with planetary chord */
    waveform_types[slot]  = t.waveform;
    harmonic_counts[slot] = t.partial_count;
    for (int h = 0; h < AFT_MAX_PARTIALS; h++) {
        harmonic_amps[slot][h] = (h < t.partial_count) ? t.partials[h] : 0.0f;
    }
    pan_positions[slot]   = 0.0f;  /* center — focus is the spotlight */

    return 1;
}
