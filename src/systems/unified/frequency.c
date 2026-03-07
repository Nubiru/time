/* frequency.c — Frequency mapper implementation.
 *
 * Converts time cycles to frequencies, transposes to audible range,
 * maps to musical notes (12-TET), and detects musical intervals.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#include "frequency.h"
#include <math.h>

/* Static note name table (12-TET chromatic scale) */
static const char *const NOTE_NAMES[FREQUENCY_NOTE_COUNT] = {
    "C", "C#", "D", "D#", "E", "F",
    "F#", "G", "G#", "A", "A#", "B"
};

/* Static interval name table */
static const char *const INTERVAL_NAMES[FREQUENCY_INTERVAL_COUNT] = {
    "Unison",      /* 0 semitones */
    "Minor 2nd",   /* 1 */
    "Major 2nd",   /* 2 */
    "Minor 3rd",   /* 3 */
    "Major 3rd",   /* 4 */
    "Perfect 4th", /* 5 */
    "Tritone",     /* 6 */
    "Perfect 5th", /* 7 */
    "Minor 6th",   /* 8 */
    "Major 6th",   /* 9 */
    "Minor 7th",   /* 10 */
    "Major 7th",   /* 11 */
    "Octave"       /* 12 */
};

/* Planetary orbital periods in days */
static const double PLANET_PERIODS[FREQUENCY_PLANET_COUNT] = {
    87.969,    /* Mercury */
    224.701,   /* Venus */
    365.256,   /* Earth */
    686.980,   /* Mars */
    4332.589,  /* Jupiter */
    10759.22,  /* Saturn */
    30688.5,   /* Uranus */
    60182.0    /* Neptune */
};

static const char *const PLANET_NAMES[FREQUENCY_PLANET_COUNT] = {
    "Mercury", "Venus", "Earth", "Mars",
    "Jupiter", "Saturn", "Uranus", "Neptune"
};

/* --- Core conversions --- */

double frequency_from_period(double period_seconds)
{
    if (period_seconds <= 0.0) {
        return 0.0;
    }
    return 1.0 / period_seconds;
}

double frequency_to_period(double hz)
{
    if (hz <= 0.0) {
        return 0.0;
    }
    return 1.0 / hz;
}

double frequency_from_days(double days)
{
    if (days <= 0.0) {
        return 0.0;
    }
    return 1.0 / (days * SECONDS_PER_DAY);
}

double frequency_from_years(double years)
{
    if (years <= 0.0) {
        return 0.0;
    }
    return 1.0 / (years * SECONDS_PER_YEAR);
}

/* --- Audible transposition --- */

double frequency_to_audible(double hz)
{
    if (hz <= 0.0) {
        return 0.0;
    }
    while (hz < FREQUENCY_AUDIBLE_MIN) {
        hz *= 2.0;
    }
    while (hz > FREQUENCY_AUDIBLE_MAX) {
        hz /= 2.0;
    }
    return hz;
}

int frequency_octaves_to_audible(double hz)
{
    if (hz <= 0.0) {
        return 0;
    }
    int octaves = 0;
    while (hz < FREQUENCY_AUDIBLE_MIN) {
        hz *= 2.0;
        octaves++;
    }
    while (hz > FREQUENCY_AUDIBLE_MAX) {
        hz /= 2.0;
        octaves--;
    }
    return octaves;
}

/* --- Musical note mapping --- */

frequency_note_t frequency_to_note(double hz)
{
    frequency_note_t result;
    result.exact_hz = hz;

    if (hz <= 0.0) {
        result.name = "?";
        result.octave = 0;
        result.cents_off = 0.0;
        result.midi_number = 0;
        return result;
    }

    /* MIDI note number: 69 + 12 * log2(hz / 440) */
    double midi_exact = 69.0 + 12.0 * log2(hz / FREQUENCY_A4_HZ);
    int midi = (int)round(midi_exact);

    result.midi_number = midi;

    /* Note index: C=0 through B=11
     * MIDI 0 = C-1, MIDI 12 = C0, MIDI 60 = C4, MIDI 69 = A4
     * note_index = midi % 12, but handle negatives */
    int note_index = ((midi % 12) + 12) % 12;
    result.name = NOTE_NAMES[note_index];

    /* Octave: MIDI 12 = C0, MIDI 24 = C1, ...
     * octave = (midi / 12) - 1, but floor division for negatives */
    int octave;
    if (midi >= 0) {
        octave = (midi / 12) - 1;
    } else {
        /* For negative MIDI (very low frequencies), floor division */
        octave = ((midi - 11) / 12) - 1;
    }
    result.octave = octave;

    /* Cents deviation from nearest note */
    double note_hz = FREQUENCY_A4_HZ * pow(2.0, (midi - 69.0) / 12.0);
    result.cents_off = 1200.0 * log2(hz / note_hz);

    return result;
}

double frequency_note_hz(int note_index, int octave)
{
    /* MIDI = (octave + 1) * 12 + note_index */
    int midi = (octave + 1) * 12 + note_index;
    return FREQUENCY_A4_HZ * pow(2.0, (midi - 69.0) / 12.0);
}

/* --- Interval detection --- */

double frequency_cents(double hz_a, double hz_b)
{
    if (hz_a <= 0.0 || hz_b <= 0.0) {
        return 0.0;
    }
    return 1200.0 * log2(hz_b / hz_a);
}

frequency_interval_t frequency_interval(double hz_a, double hz_b)
{
    frequency_interval_t result;

    if (hz_a <= 0.0 || hz_b <= 0.0) {
        result.name = "?";
        result.semitones = 0;
        result.cents = 0.0;
        result.ratio = 0.0;
        return result;
    }

    result.ratio = hz_b / hz_a;
    result.cents = 1200.0 * log2(hz_b / hz_a);

    /* Semitones: round cents / 100, clamp to 0-12 */
    int semitones = (int)round(result.cents / 100.0);

    /* Wrap to 0-12 range */
    semitones = ((semitones % 12) + 12) % 12;

    /* Check if it's a full octave (or multiple) */
    if (fabs(result.cents - 1200.0) < 50.0) {
        semitones = 12;
    }

    result.semitones = semitones;

    if (semitones >= 0 && semitones < FREQUENCY_INTERVAL_COUNT) {
        result.name = INTERVAL_NAMES[semitones];
    } else {
        result.name = "?";
    }

    return result;
}

/* --- Name lookups --- */

const char *frequency_note_name(int index)
{
    if (index < 0 || index >= FREQUENCY_NOTE_COUNT) {
        return "?";
    }
    return NOTE_NAMES[index];
}

const char *frequency_interval_name(int semitones)
{
    if (semitones < 0 || semitones >= FREQUENCY_INTERVAL_COUNT) {
        return "?";
    }
    return INTERVAL_NAMES[semitones];
}

/* --- Planetary frequencies --- */

planet_frequency_t frequency_planet(int planet_index)
{
    planet_frequency_t result;

    if (planet_index < 0 || planet_index >= FREQUENCY_PLANET_COUNT) {
        result.name = "?";
        result.orbital_period_days = 0.0;
        result.orbital_hz = 0.0;
        result.audible_hz = 0.0;
        result.note.name = "?";
        result.note.octave = 0;
        result.note.cents_off = 0.0;
        result.note.exact_hz = 0.0;
        result.note.midi_number = 0;
        return result;
    }

    result.name = PLANET_NAMES[planet_index];
    result.orbital_period_days = PLANET_PERIODS[planet_index];
    result.orbital_hz = frequency_from_days(result.orbital_period_days);
    result.audible_hz = frequency_to_audible(result.orbital_hz);
    result.note = frequency_to_note(result.audible_hz);

    return result;
}

int frequency_planet_count(void)
{
    return FREQUENCY_PLANET_COUNT;
}
