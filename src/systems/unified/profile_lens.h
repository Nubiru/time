/* profile_lens.h — Same birth identity through 4 narrative frames.
 *
 * Takes a cosmic fingerprint and presents it through different lenses:
 * Pattern (behavioral), Chart (astronomical), Calendar (cultural),
 * Energy (spiritual). Each lens selects relevant systems and frames
 * the identity data with lens-appropriate vocabulary.
 *
 * L3.6 data backbone.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_PROFILE_LENS_H
#define TIME_PROFILE_LENS_H

#include "cosmic_fingerprint.h"

#define PL_MAX_SECTIONS  6
#define PL_TITLE_MAX     64
#define PL_BODY_MAX      256

/* Lens types */
typedef enum {
    PL_LENS_PATTERN = 0,   /* Behavioral: personality, tendencies, archetypes */
    PL_LENS_CHART,         /* Astronomical: positions, transits, cycles */
    PL_LENS_CALENDAR,      /* Cultural: dates, months, years across traditions */
    PL_LENS_ENERGY,        /* Spiritual: gates, channels, hexagrams, seals */
    PL_LENS_COUNT
} pl_lens_t;

/* A single section within a lens view */
typedef struct {
    char title[PL_TITLE_MAX];   /* "Sun Sign", "Kin Archetype", etc. */
    char body[PL_BODY_MAX];     /* Lens-appropriate description */
    cf_system_t system;         /* Which system this section comes from */
} pl_section_t;

/* Complete lens view */
typedef struct {
    char title[PL_TITLE_MAX];           /* "Your Pattern", "Your Chart", etc. */
    char subtitle[PL_BODY_MAX];         /* Brief lens description */
    pl_lens_t lens;
    pl_section_t sections[PL_MAX_SECTIONS];
    int section_count;
} pl_view_t;

/* Get lens display name. Returns "?" for invalid. */
const char *pl_lens_name(pl_lens_t lens);

/* Get lens subtitle/description. Returns "?" for invalid. */
const char *pl_lens_subtitle(pl_lens_t lens);

/* Number of available lenses. */
int pl_lens_count(void);

/* Compose a lens view from a cosmic fingerprint.
 * NULL fingerprint or invalid lens returns empty view. */
pl_view_t pl_compose(const cf_fingerprint_t *fp, pl_lens_t lens);

/* Format a composed view as multi-line text.
 * Returns bytes written (excluding NUL). 0 for NULL args. */
int pl_format_view(const pl_view_t *view, char *buf, int buf_size);

#endif /* TIME_PROFILE_LENS_H */
