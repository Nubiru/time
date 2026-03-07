# Task: Frequency Mapper

**Agent**: GAMMA
**Roadmap Reference**: Track 27.1 — "Agent: Frequency Mapper (Agent A)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure module converting any time cycle to a frequency and transposing it to audible range via octave shifts. Computes planetary orbital frequencies, maps frequencies to musical notes (A440 tuning), and detects musical intervals between planet pairs. The "Music of the Spheres" — computable.

## READ FIRST

- `src/systems/astronomy/orbit.h` — orbital periods
- `src/systems/earth/biorhythm.h` — biological rhythm frequencies (already delivered)

## Files to Create

- `src/systems/unified/frequency.h`
- `src/systems/unified/frequency.c`
- `tests/systems/unified/test_frequency.c`

## API

```c
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

/* Get planetary orbital frequency by index (0=Sun/day, 1=Mercury..8=Neptune, 9=Moon). */
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

/* Get frequency of a specific note (e.g., "A", 4 → 440 Hz). */
double freq_note_hz(const char *note_name, int octave);

#endif /* TIME_FREQUENCY_H */
```

## Planetary Orbital Periods (seconds)

| Body | Period | Source |
|------|--------|--------|
| Earth day | 86,400 s | definition |
| Moon (synodic) | 2,551,443 s | 29.530589 days |
| Mercury | 7,600,537 s | 87.969 days |
| Venus | 19,414,166 s | 224.701 days |
| Earth (year) | 31,558,150 s | 365.256 days |
| Mars | 59,354,294 s | 686.980 days |
| Jupiter | 374,335,690 s | 4,332.59 days |
| Saturn | 929,596,608 s | 10,759.22 days |
| Uranus | 2,651,370,019 s | 30,688.5 days |
| Neptune | 5,200,418,560 s | 60,182.0 days |

## Known Results (for test validation)

- Earth year → ~31.69 nHz → octave-transposed ≈ 136.1 Hz (C#3, the "Om" frequency)
- Synodic month → ~210.42 Hz (G#3)
- Earth day → ~194.18 Hz (G3)

## DONE WHEN

- [ ] cycle_to_frequency converts period to Hz
- [ ] frequency_to_audible transposes any Hz to 20-20000 range
- [ ] frequency_to_note identifies nearest A440 note with cents deviation
- [ ] 10 planetary frequencies with orbital data
- [ ] Musical interval detection (unison through octave)
- [ ] Planet-pair interval computation
- [ ] Known frequencies verified (Earth year ≈ 136.1 Hz, day ≈ 194.18 Hz)
- [ ] >= 35 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- `#include <math.h>` for log2, pow, fabs, round, fmod
- `#include <string.h>` for strcmp, snprintf
- Standalone module (no compile-time deps on orbit.h or biorhythm.h)
- A440 tuning standard (equal temperament, 12-TET)
