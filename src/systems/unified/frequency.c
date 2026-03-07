/* frequency.c -- Frequency mapper implementation.
 *
 * Converts time cycles to frequencies, transposes to audible range,
 * maps to musical notes (12-TET, A4=440Hz), and detects musical intervals.
 *
 * Planet table (10 entries):
 *   0=Earth day, 1=Mercury, 2=Venus, 3=Earth year, 4=Mars,
 *   5=Jupiter, 6=Saturn, 7=Uranus, 8=Neptune, 9=Moon (synodic)
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#include "frequency.h"
#include <math.h>
#include <string.h>

/* Audible range boundaries */
static const double AUDIBLE_MIN = 20.0;
static const double AUDIBLE_MAX = 20000.0;

/* Note name table (12-TET chromatic scale, C=0 through B=11) */
static const char *const NOTE_NAMES[FREQ_SEMITONES] = {
    "C", "C#", "D", "D#", "E", "F",
    "F#", "G", "G#", "A", "A#", "B"
};

/* Interval name table (0-12 semitones) */
static const char *const INTERVAL_NAMES[13] = {
    "unison",           /* 0 */
    "minor second",     /* 1 */
    "major second",     /* 2 */
    "minor third",      /* 3 */
    "major third",      /* 4 */
    "perfect fourth",   /* 5 */
    "tritone",          /* 6 */
    "perfect fifth",    /* 7 */
    "minor sixth",      /* 8 */
    "major sixth",      /* 9 */
    "minor seventh",    /* 10 */
    "major seventh",    /* 11 */
    "octave"            /* 12 */
};

/* Planetary orbital periods in seconds */
static const double PLANET_PERIODS[FREQ_MAX_PLANETS] = {
    86400.0,        /* 0: Earth day */
    7600537.0,      /* 1: Mercury (87.969 days) */
    19414166.0,     /* 2: Venus (224.701 days) */
    31558150.0,     /* 3: Earth year (365.256 days) */
    59354294.0,     /* 4: Mars (686.980 days) */
    374335690.0,    /* 5: Jupiter (4332.59 days) */
    929596608.0,    /* 6: Saturn (10759.22 days) */
    2651370019.0,   /* 7: Uranus (30688.5 days) */
    5200418560.0,   /* 8: Neptune (60182.0 days) */
    2551443.0       /* 9: Moon synodic (29.530589 days) */
};

static const char *const PLANET_NAMES[FREQ_MAX_PLANETS] = {
    "Earth day", "Mercury", "Venus", "Earth year", "Mars",
    "Jupiter", "Saturn", "Uranus", "Neptune", "Moon"
};

/* --- Helper: copy note name into fixed buffer --- */

static void copy_name(char *dst, int dst_size, const char *src)
{
    int i = 0;
    while (i < dst_size - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

/* --- Helper: look up note index by name string --- */

static int note_index_from_name(const char *name)
{
    if (name == NULL) {
        return -1;
    }
    for (int i = 0; i < FREQ_SEMITONES; i++) {
        if (strcmp(name, NOTE_NAMES[i]) == 0) {
            return i;
        }
    }
    return -1;
}

/* --- Core conversions --- */

double freq_from_period(double period_seconds)
{
    if (period_seconds <= 0.0) {
        return 0.0;
    }
    return 1.0 / period_seconds;
}

double freq_to_audible(double hz)
{
    if (hz <= 0.0) {
        return 0.0;
    }
    while (hz < AUDIBLE_MIN) {
        hz *= 2.0;
    }
    while (hz > AUDIBLE_MAX) {
        hz /= 2.0;
    }
    return hz;
}

double freq_octave_shift(double hz, int octaves)
{
    if (hz <= 0.0) {
        return 0.0;
    }
    return hz * pow(2.0, (double)octaves);
}

double freq_semitone_distance(double hz_a, double hz_b)
{
    if (hz_a <= 0.0 || hz_b <= 0.0) {
        return 0.0;
    }
    return 12.0 * log2(hz_b / hz_a);
}

/* --- Musical note mapping --- */

freq_note_t freq_to_note(double hz)
{
    freq_note_t result;
    memset(&result, 0, sizeof(result));
    result.hz = hz;

    if (hz <= 0.0) {
        copy_name(result.name, sizeof(result.name), "?");
        return result;
    }

    /* Semitones from A4: n = 12 * log2(hz / 440) */
    double n = 12.0 * log2(hz / FREQ_A4_HZ);
    int n_rounded = (int)round(n);

    /* Note index within octave: A=9 in our C-based indexing
     * MIDI note number = 69 + n_rounded
     * note_index = (midi % 12 + 12) % 12 */
    int midi = 69 + n_rounded;
    int note_idx = ((midi % 12) + 12) % 12;

    copy_name(result.name, sizeof(result.name), NOTE_NAMES[note_idx]);

    /* Octave: MIDI 12 = C0, MIDI 24 = C1, ..., MIDI 60 = C4
     * octave = (midi / 12) - 1 for positive MIDI */
    if (midi >= 0) {
        result.octave = (midi / 12) - 1;
    } else {
        result.octave = ((midi - 11) / 12) - 1;
    }

    /* Cents deviation from nearest ET note */
    double nearest_hz = FREQ_A4_HZ * pow(2.0, (double)n_rounded / 12.0);
    result.cents_off = 1200.0 * log2(hz / nearest_hz);

    return result;
}

double freq_note_hz(const char *note_name, int octave)
{
    int idx = note_index_from_name(note_name);
    if (idx < 0) {
        return 0.0;
    }
    /* MIDI = (octave + 1) * 12 + note_index */
    int midi = (octave + 1) * 12 + idx;
    return FREQ_A4_HZ * pow(2.0, (midi - 69.0) / 12.0);
}

/* --- Interval detection --- */

freq_interval_t freq_interval(double hz_a, double hz_b)
{
    freq_interval_t result;
    memset(&result, 0, sizeof(result));

    if (hz_a <= 0.0 || hz_b <= 0.0) {
        copy_name(result.name, sizeof(result.name), "?");
        return result;
    }

    result.ratio = hz_b / hz_a;
    double cents = 1200.0 * log2(hz_b / hz_a);

    /* Semitones: round cents / 100, wrap to 0-12 */
    int semitones = (int)round(cents / 100.0);
    semitones = ((semitones % 12) + 12) % 12;

    /* Detect full octave (or near it) */
    if (fabs(cents - 1200.0) < 50.0) {
        semitones = 12;
    }

    result.semitones = semitones;

    /* Cents deviation from the ET interval */
    result.cents_off = cents - (double)semitones * 100.0;

    if (semitones >= 0 && semitones <= 12) {
        copy_name(result.name, sizeof(result.name), INTERVAL_NAMES[semitones]);
    } else {
        copy_name(result.name, sizeof(result.name), "?");
    }

    return result;
}

/* --- Planetary frequencies --- */

static int count_octaves_to_audible(double hz)
{
    if (hz <= 0.0) {
        return 0;
    }
    int octaves = 0;
    while (hz < AUDIBLE_MIN) {
        hz *= 2.0;
        octaves++;
    }
    while (hz > AUDIBLE_MAX) {
        hz /= 2.0;
        octaves--;
    }
    return octaves;
}

freq_planet_t freq_planet(int planet_index)
{
    freq_planet_t result;
    memset(&result, 0, sizeof(result));

    if (planet_index < 0 || planet_index >= FREQ_MAX_PLANETS) {
        result.name = "?";
        result.note.name[0] = '?';
        result.note.name[1] = '\0';
        return result;
    }

    result.name = PLANET_NAMES[planet_index];
    result.orbital_period_s = PLANET_PERIODS[planet_index];
    result.base_hz = freq_from_period(result.orbital_period_s);
    result.audible_hz = freq_to_audible(result.base_hz);
    result.note = freq_to_note(result.audible_hz);
    result.octaves_shifted = count_octaves_to_audible(result.base_hz);

    return result;
}

int freq_planet_count(void)
{
    return FREQ_MAX_PLANETS;
}

freq_interval_t freq_planet_interval(int planet_a, int planet_b)
{
    freq_planet_t pa = freq_planet(planet_a);
    freq_planet_t pb = freq_planet(planet_b);

    if (pa.audible_hz <= 0.0 || pb.audible_hz <= 0.0) {
        freq_interval_t invalid;
        memset(&invalid, 0, sizeof(invalid));
        copy_name(invalid.name, sizeof(invalid.name), "?");
        return invalid;
    }

    return freq_interval(pa.audible_hz, pb.audible_hz);
}
