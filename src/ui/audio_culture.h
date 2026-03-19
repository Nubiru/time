/* audio_culture.h -- Cultural audio timbres for calendar systems.
 *
 * Each calendar tradition gets a unique sonic identity defined by
 * harmonic partial amplitudes. These timbres layer briefly when
 * their calendar system has an active convergence event.
 *
 * Pure data module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_AUDIO_CULTURE_H
#define TIME_AUDIO_CULTURE_H

#include "../systems/unified/convergence_detect.h"

#define AC_MAX_PARTIALS 8

/* Cultural timbre profile */
typedef struct {
    cd_system_t system;              /* which calendar system */
    const char* name;                /* "Hebrew", "Buddhist", etc. */
    const char* timbre_desc;         /* "shofar brass", "singing bowl", etc. */
    float base_freq_hz;              /* fundamental frequency */
    int partial_count;               /* number of active partials */
    float partials[AC_MAX_PARTIALS]; /* harmonic amplitudes (normalized) */
    float base_amplitude;            /* default volume (0.0-1.0) */
} audio_culture_t;

/* Get cultural timbre for a calendar system.
 * Returns a profile with zero amplitude for unsupported systems. */
audio_culture_t audio_culture_get(cd_system_t system);

/* Get cultural timbre name. Returns "?" for unsupported. */
const char* audio_culture_name(cd_system_t system);

/* Get cultural timbre description. Returns "?" for unsupported. */
const char* audio_culture_desc(cd_system_t system);

/* Number of supported cultural timbres. */
int audio_culture_count(void);

/* Check if a system has a defined cultural timbre. */
int audio_culture_has_timbre(cd_system_t system);

#endif /* TIME_AUDIO_CULTURE_H */
