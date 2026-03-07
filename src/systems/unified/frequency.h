/* frequency.h — Pure module converting time cycles to frequencies.
 *
 * Maps orbital periods and other cycles to frequencies in Hz,
 * transposes to audible range via octave doubling/halving,
 * identifies nearest musical note (12-TET, A4=440Hz),
 * and detects musical intervals between frequency pairs.
 *
 * Implements the "Music of the Spheres" computationally:
 * every cycle is a frequency, every pair of cycles has a musical interval.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_FREQUENCY_H
#define TIME_FREQUENCY_H

#define PI 3.14159265358979323846

/* Reference pitch: A4 = 440 Hz (ISO 16) */
#define FREQUENCY_A4_HZ 440.0

/* Audible range */
#define FREQUENCY_AUDIBLE_MIN 20.0
#define FREQUENCY_AUDIBLE_MAX 20000.0

/* 12-TET semitone ratio */
#define FREQUENCY_SEMITONE_RATIO 1.05946309435929526456  /* 2^(1/12) */

/* Seconds in common time units */
#define SECONDS_PER_DAY 86400.0
#define SECONDS_PER_YEAR 31557600.0  /* Julian year = 365.25 days */

/* Maximum planet count */
#define FREQUENCY_PLANET_COUNT 8

/* Musical note names (12-TET chromatic) */
#define FREQUENCY_NOTE_COUNT 12

/* Musical interval names */
#define FREQUENCY_INTERVAL_COUNT 13

/* Musical note result */
typedef struct {
    const char *name;      /* "C", "C#", "D", etc. */
    int octave;            /* octave number (4 = middle octave) */
    double cents_off;      /* deviation from nearest note in cents (-50 to +50) */
    double exact_hz;       /* the exact frequency */
    int midi_number;       /* MIDI note number (A4 = 69) */
} frequency_note_t;

/* Musical interval result */
typedef struct {
    const char *name;      /* "Unison", "Minor 2nd", "Major 3rd", "Perfect 5th", etc. */
    int semitones;         /* interval in semitones (0-12) */
    double cents;          /* exact interval in cents */
    double ratio;          /* frequency ratio */
} frequency_interval_t;

/* Planetary frequency data */
typedef struct {
    const char *name;      /* "Mercury", "Venus", etc. */
    double orbital_period_days;
    double orbital_hz;     /* 1 / period_seconds */
    double audible_hz;     /* octave-transposed to audible range */
    frequency_note_t note; /* nearest musical note */
} planet_frequency_t;

/* Convert a time period to frequency.
 * period_seconds: cycle length in seconds.
 * Returns frequency in Hz. Returns 0 for period <= 0. */
double frequency_from_period(double period_seconds);

/* Convert frequency to period in seconds.
 * hz: frequency in Hz.
 * Returns period in seconds. Returns 0 for hz <= 0. */
double frequency_to_period(double hz);

/* Transpose frequency to audible range via octave doubling/halving.
 * hz: any positive frequency.
 * Returns equivalent frequency in 20-20000 Hz range. */
double frequency_to_audible(double hz);

/* How many octaves to transpose to reach audible range?
 * Returns signed integer (positive = up, negative = down). */
int frequency_octaves_to_audible(double hz);

/* Map frequency to nearest musical note (12-TET, A4=440Hz). */
frequency_note_t frequency_to_note(double hz);

/* Get frequency of a specific note.
 * note_index: 0=C, 1=C#, 2=D, ..., 11=B.
 * octave: octave number (4 = middle). */
double frequency_note_hz(int note_index, int octave);

/* Musical interval between two frequencies. */
frequency_interval_t frequency_interval(double hz_a, double hz_b);

/* Interval in cents between two frequencies. */
double frequency_cents(double hz_a, double hz_b);

/* Get planetary orbital frequency data for planet index (0=Mercury..7=Neptune). */
planet_frequency_t frequency_planet(int planet_index);

/* Planet count (always 8). */
int frequency_planet_count(void);

/* Convert days to frequency in Hz. */
double frequency_from_days(double days);

/* Convert years to frequency in Hz. */
double frequency_from_years(double years);

/* Note name by index (0="C", 1="C#", ..., 11="B"). Returns "?" for invalid. */
const char *frequency_note_name(int index);

/* Interval name by semitones (0="Unison", 1="Minor 2nd", ..., 12="Octave"). */
const char *frequency_interval_name(int semitones);

#endif /* TIME_FREQUENCY_H */
