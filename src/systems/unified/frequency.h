/* frequency.h -- Pure module converting time cycles to frequencies.
 *
 * Maps orbital periods and other cycles to frequencies in Hz,
 * transposes to audible range via octave doubling/halving,
 * identifies nearest musical note (12-TET, A4=440Hz),
 * and detects musical intervals between frequency pairs.
 *
 * Implements the "Music of the Spheres" computationally:
 * every cycle is a frequency, every pair of cycles has a musical interval.
 *
 * Planet table (10 entries):
 *   0=Earth day, 1=Mercury, 2=Venus, 3=Earth year, 4=Mars,
 *   5=Jupiter, 6=Saturn, 7=Uranus, 8=Neptune, 9=Moon (synodic)
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_FREQUENCY_H
#define TIME_FREQUENCY_H

#define FREQ_MAX_PLANETS 10
#define FREQ_A4_HZ 440.0
#define FREQ_SEMITONES 12

/* Musical note */
typedef struct {
    char name[4];       /* "C", "C#", "D", ..., "B" */
    int octave;         /* octave number (4 = middle C octave) */
    double cents_off;   /* deviation from equal temperament (-50 to +50) */
    double hz;          /* actual frequency */
} freq_note_t;

/* Musical interval between two frequencies */
typedef struct {
    char name[24];      /* "unison", "minor third", "perfect fifth", etc. */
    double ratio;       /* frequency ratio (e.g., 1.5 for perfect fifth) */
    int semitones;      /* nearest equal-temperament semitone count */
    double cents_off;   /* deviation from ET interval */
} freq_interval_t;

/* Planetary frequency data */
typedef struct {
    const char *name;       /* planet name */
    double orbital_period_s; /* orbital period in seconds */
    double base_hz;         /* raw frequency = 1/period */
    double audible_hz;      /* octave-transposed to audible range */
    freq_note_t note;       /* nearest musical note */
    int octaves_shifted;    /* how many octaves up to reach audible */
} freq_planet_t;

/* Convert a time period (seconds) to frequency (Hz). */
double freq_from_period(double period_seconds);

/* Convert a frequency to audible range by octave doubling/halving. */
double freq_to_audible(double hz);

/* Find the nearest musical note for a frequency (A440 tuning). */
freq_note_t freq_to_note(double hz);

/* Get planetary orbital frequency by index (0=Earth day, 1=Mercury..8=Neptune, 9=Moon). */
freq_planet_t freq_planet(int planet_index);

/* Get number of planets in the frequency table. */
int freq_planet_count(void);

/* Compute musical interval between two frequencies. */
freq_interval_t freq_interval(double hz_a, double hz_b);

/* Compute interval between two planets by index. */
freq_interval_t freq_planet_interval(int planet_a, int planet_b);

/* Octave-transpose a frequency by n octaves (positive=up, negative=down). */
double freq_octave_shift(double hz, int octaves);

/* Get the number of semitones between two frequencies. */
double freq_semitone_distance(double hz_a, double hz_b);

/* Get frequency of a specific note (e.g., "A", 4 -> 440 Hz).
 * Returns 0.0 for invalid note name or NULL. */
double freq_note_hz(const char *note_name, int octave);

#endif /* TIME_FREQUENCY_H */
