/* audio_data.c -- Audio synthesis parameter data module.
 *
 * Extends frequency.h with harmonic series per planet, aspect
 * sonification mappings, moon phase envelopes, and interval tables.
 *
 * Harmonic amplitudes: h[n] = 1.0 / (n * (1 + (1-eccentricity)*n))
 * More eccentric orbit = richer harmonics (more overtones).
 * Circular orbit = purer tone (fewer harmonics).
 *
 * Moon envelope: amplitude = 0.2 + 0.8 * (0.5 + 0.5 * cos(2*PI*(phase - 0.5)))
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#include "audio_data.h"
#include "frequency.h"
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

/* --- Planet data tables --- */

/* Orbital eccentricities (same order as frequency.h planet table) */
static const double ECCENTRICITIES[AUDIO_MAX_PLANETS] = {
    0.0,      /* 0: Earth day (no orbit, pure tone) */
    0.2056,   /* 1: Mercury */
    0.0068,   /* 2: Venus */
    0.0167,   /* 3: Earth year */
    0.0934,   /* 4: Mars */
    0.0489,   /* 5: Jupiter */
    0.0565,   /* 6: Saturn */
    0.0457,   /* 7: Uranus */
    0.0113,   /* 8: Neptune */
    0.0549    /* 9: Moon (synodic) */
};

/* Number of harmonics per planet */
static const int HARMONIC_COUNTS[AUDIO_MAX_PLANETS] = {
    2,  /* Earth day: pure */
    6,  /* Mercury: rich */
    2,  /* Venus: pure */
    3,  /* Earth year */
    5,  /* Mars */
    4,  /* Jupiter */
    4,  /* Saturn */
    3,  /* Uranus */
    2,  /* Neptune: pure */
    4   /* Moon */
};

/* Waveform per planet */
static const audio_waveform_t WAVEFORMS[AUDIO_MAX_PLANETS] = {
    AUDIO_WAVE_SINE,      /* Earth day */
    AUDIO_WAVE_SAWTOOTH,  /* Mercury */
    AUDIO_WAVE_SINE,      /* Venus */
    AUDIO_WAVE_SINE,      /* Earth year */
    AUDIO_WAVE_TRIANGLE,  /* Mars */
    AUDIO_WAVE_TRIANGLE,  /* Jupiter */
    AUDIO_WAVE_TRIANGLE,  /* Saturn */
    AUDIO_WAVE_SINE,      /* Uranus */
    AUDIO_WAVE_SINE,      /* Neptune */
    AUDIO_WAVE_TRIANGLE   /* Moon */
};

/* Base amplitudes per planet */
static const double BASE_AMPLITUDES[AUDIO_MAX_PLANETS] = {
    0.30,  /* Sun (Earth day) */
    0.15,  /* Mercury */
    0.25,  /* Venus */
    0.20,  /* Earth year */
    0.20,  /* Mars */
    0.30,  /* Jupiter */
    0.25,  /* Saturn */
    0.10,  /* Uranus */
    0.08,  /* Neptune */
    0.35   /* Moon */
};

/* --- Aspect data table --- */

static const double ASPECT_ANGLES[AUDIO_ASPECT_COUNT] = {
    0.0, 60.0, 90.0, 120.0, 180.0
};

static const char *const ASPECT_NAMES[AUDIO_ASPECT_COUNT] = {
    "conjunction", "sextile", "square", "trine", "opposition"
};

static const char *const ASPECT_INTERVALS[AUDIO_ASPECT_COUNT] = {
    "unison", "minor third", "fourth", "fifth", "octave"
};

static const double ASPECT_CONSONANCES[AUDIO_ASPECT_COUNT] = {
    1.0, 0.7, 0.5, 0.9, 0.8
};

static const double ASPECT_VOLUMES[AUDIO_ASPECT_COUNT] = {
    1.5, 1.1, 0.8, 1.3, 1.2
};

/* --- Waveform names --- */

static const char *const WAVEFORM_NAMES[AUDIO_WAVE_COUNT] = {
    "sine", "triangle", "sawtooth"
};

/* --- Kepler voice assignments (Harmonices Mundi, Book V, 1619) ---
 * Mercury: soprano — widest range (octave + minor third)
 * Earth, Venus: alto — narrowest ranges (semitone, barely a comma)
 * Mars: tenor — range of a perfect fifth
 * Jupiter, Saturn: bass — deepest, slowest
 * Non-classical planets (Uranus, Neptune) and Moon: unassigned. */
static const kepler_voice_t KEPLER_VOICES[AUDIO_MAX_PLANETS] = {
    KEPLER_VOICE_COUNT,    /* 0: Earth day — not a planet voice */
    KEPLER_VOICE_SOPRANO,  /* 1: Mercury */
    KEPLER_VOICE_ALTO,     /* 2: Venus */
    KEPLER_VOICE_ALTO,     /* 3: Earth year */
    KEPLER_VOICE_TENOR,    /* 4: Mars */
    KEPLER_VOICE_BASS,     /* 5: Jupiter */
    KEPLER_VOICE_BASS,     /* 6: Saturn */
    KEPLER_VOICE_COUNT,    /* 7: Uranus — post-Kepler */
    KEPLER_VOICE_COUNT,    /* 8: Neptune — post-Kepler */
    KEPLER_VOICE_COUNT     /* 9: Moon — not in voice assignment */
};

static const char *const KEPLER_VOICE_NAMES[KEPLER_VOICE_COUNT] = {
    "bass", "tenor", "alto", "soprano"
};

/* --- Just intonation ratios (Pythagorean / Keplerian) ---
 * These are frequency ratios from natural harmonic relationships,
 * NOT the equal temperament ratios (2^(n/12)) used in modern tuning.
 * Pythagoras: 1:2, 2:3, 3:4. Kepler added: 4:5, 5:6, 9:10. */
static const double JUST_RATIOS[AUDIO_JI_COUNT] = {
    1.0,            /* unison */
    16.0 / 15.0,    /* minor second (semitone) */
    9.0 / 8.0,      /* major second (whole tone) */
    6.0 / 5.0,      /* minor third */
    5.0 / 4.0,      /* major third */
    4.0 / 3.0,      /* perfect fourth */
    45.0 / 32.0,    /* tritone (augmented fourth) */
    3.0 / 2.0,      /* perfect fifth */
    8.0 / 5.0,      /* minor sixth */
    5.0 / 3.0,      /* major sixth */
    9.0 / 5.0,      /* minor seventh */
    15.0 / 8.0,     /* major seventh */
    2.0 / 1.0       /* octave */
};

static const char *const JUST_INTERVAL_NAMES[AUDIO_JI_COUNT] = {
    "unison", "minor second", "major second", "minor third",
    "major third", "perfect fourth", "tritone", "perfect fifth",
    "minor sixth", "major sixth", "minor seventh", "major seventh",
    "octave"
};

/* --- Helper: compute harmonic amplitude using eccentricity formula --- */

static double harmonic_raw(int n, double eccentricity)
{
    /* h[n] = 1.0 / (n * (1 + (1-eccentricity)*n)) */
    double dn = (double)n;
    return 1.0 / (dn * (1.0 + (1.0 - eccentricity) * dn));
}

/* --- Public API --- */

double audio_planet_freq(int planet_index)
{
    if (planet_index < 0 || planet_index >= AUDIO_MAX_PLANETS) {
        return 0.0;
    }
    freq_planet_t p = freq_planet(planet_index);
    return p.audible_hz;
}

int audio_planet_harmonics(int planet_index, audio_harmonic_t *out,
                           int max_harmonics)
{
    if (planet_index < 0 || planet_index >= AUDIO_MAX_PLANETS) {
        return 0;
    }
    if (out == NULL || max_harmonics <= 0) {
        return 0;
    }

    double ecc = ECCENTRICITIES[planet_index];
    int count = HARMONIC_COUNTS[planet_index];
    if (count > max_harmonics) {
        count = max_harmonics;
    }
    if (count > AUDIO_MAX_HARMONICS) {
        count = AUDIO_MAX_HARMONICS;
    }

    /* Compute raw fundamental amplitude for normalization */
    double fundamental_raw = harmonic_raw(1, ecc);

    for (int i = 0; i < count; i++) {
        int n = i + 1;
        out[i].harmonic_number = n;
        out[i].freq_ratio = (double)n;
        out[i].amplitude = harmonic_raw(n, ecc) / fundamental_raw;
    }

    return count;
}

audio_planet_profile_t audio_planet_profile(int planet_index)
{
    audio_planet_profile_t result;
    memset(&result, 0, sizeof(result));

    if (planet_index < 0 || planet_index >= AUDIO_MAX_PLANETS) {
        result.planet_index = -1;
        return result;
    }

    result.planet_index = planet_index;
    result.fundamental_hz = audio_planet_freq(planet_index);
    result.waveform = WAVEFORMS[planet_index];
    result.base_amplitude = BASE_AMPLITUDES[planet_index];
    result.harmonic_count = audio_planet_harmonics(
        planet_index, result.harmonics, AUDIO_MAX_HARMONICS);

    return result;
}

audio_aspect_map_t audio_aspect_map(audio_aspect_t aspect)
{
    audio_aspect_map_t result;
    memset(&result, 0, sizeof(result));

    if (aspect < 0 || aspect >= AUDIO_ASPECT_COUNT) {
        result.aspect = AUDIO_ASPECT_COUNT;
        result.name = "unknown";
        result.musical_interval = "unknown";
        return result;
    }

    result.aspect = aspect;
    result.name = ASPECT_NAMES[aspect];
    result.angle_deg = ASPECT_ANGLES[aspect];
    result.musical_interval = ASPECT_INTERVALS[aspect];
    result.consonance = ASPECT_CONSONANCES[aspect];
    result.volume_factor = ASPECT_VOLUMES[aspect];

    return result;
}

const char *audio_aspect_harmony(audio_aspect_t aspect)
{
    if (aspect < 0 || aspect >= AUDIO_ASPECT_COUNT) {
        return "unknown";
    }
    return ASPECT_INTERVALS[aspect];
}

double audio_aspect_consonance(audio_aspect_t aspect)
{
    if (aspect < 0 || aspect >= AUDIO_ASPECT_COUNT) {
        return 0.0;
    }
    return ASPECT_CONSONANCES[aspect];
}

double audio_moon_envelope(double moon_phase)
{
    /* Clamp/wrap phase to [0.0, 1.0] */
    if (moon_phase < 0.0) {
        moon_phase = moon_phase - floor(moon_phase);
    } else if (moon_phase > 1.0) {
        moon_phase = moon_phase - floor(moon_phase);
    }

    /* amplitude = 0.2 + 0.8 * (0.5 + 0.5 * cos(2*PI*(phase - 0.5))) */
    return 0.2 + 0.8 * (0.5 + 0.5 * cos(2.0 * PI * (moon_phase - 0.5)));
}

const char *audio_interval_name(int planet_a, int planet_b)
{
    /* Interval name literals (indexed by semitone count 0-12) */
    static const char *const INTERVAL_LABELS[13] = {
        "unison", "minor second", "major second", "minor third",
        "major third", "perfect fourth", "tritone", "perfect fifth",
        "minor sixth", "major sixth", "minor seventh", "major seventh",
        "octave"
    };

    if (planet_a < 0 || planet_a >= AUDIO_MAX_PLANETS ||
        planet_b < 0 || planet_b >= AUDIO_MAX_PLANETS) {
        return "unknown";
    }

    freq_interval_t interval = freq_planet_interval(planet_a, planet_b);
    int st = interval.semitones;
    if (st >= 0 && st <= 12) {
        return INTERVAL_LABELS[st];
    }
    return "unknown";
}

double audio_planet_amplitude(int planet_index)
{
    if (planet_index < 0 || planet_index >= AUDIO_MAX_PLANETS) {
        return 0.0;
    }
    return BASE_AMPLITUDES[planet_index];
}

int audio_aspect_from_angle(double angle_deg, double orb_deg)
{
    /* Normalize angle to [0, 360) */
    while (angle_deg < 0.0) {
        angle_deg += 360.0;
    }
    while (angle_deg >= 360.0) {
        angle_deg -= 360.0;
    }

    for (int i = 0; i < AUDIO_ASPECT_COUNT; i++) {
        double diff = fabs(angle_deg - ASPECT_ANGLES[i]);
        /* Also check wrap-around for conjunction at 0/360 */
        if (diff > 180.0) {
            diff = 360.0 - diff;
        }
        if (diff <= orb_deg) {
            return i;
        }
    }

    return -1;
}

int audio_aspect_count(void)
{
    return AUDIO_ASPECT_COUNT;
}

const char *audio_waveform_name(audio_waveform_t waveform)
{
    if (waveform < 0 || waveform >= AUDIO_WAVE_COUNT) {
        return "unknown";
    }
    return WAVEFORM_NAMES[waveform];
}

kepler_voice_t audio_kepler_voice(int planet_index)
{
    if (planet_index < 0 || planet_index >= AUDIO_MAX_PLANETS) {
        return KEPLER_VOICE_COUNT;
    }
    return KEPLER_VOICES[planet_index];
}

const char *audio_kepler_voice_name(kepler_voice_t voice)
{
    if (voice < 0 || voice >= KEPLER_VOICE_COUNT) {
        return "unassigned";
    }
    return KEPLER_VOICE_NAMES[voice];
}

double audio_just_ratio(audio_just_interval_t interval)
{
    if (interval < 0 || interval >= AUDIO_JI_COUNT) {
        return 0.0;
    }
    return JUST_RATIOS[interval];
}

const char *audio_just_interval_name(audio_just_interval_t interval)
{
    if (interval < 0 || interval >= AUDIO_JI_COUNT) {
        return "unknown";
    }
    return JUST_INTERVAL_NAMES[interval];
}

int audio_just_interval_count(void)
{
    return AUDIO_JI_COUNT;
}
