/* birth_sky.h — Birth moment visual state.
 *
 * Bridges birth_profile_t to the visual layer: ring highlights,
 * featured system selection, card content for birth display.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_BIRTH_SKY_H
#define TIME_BIRTH_SKY_H

#include "../systems/unified/birth_profile.h"
#include "../systems/astrology/aspects.h"
#include "ring_today.h"

#include <stdbool.h>

#define BS_MAX_FEATURED 6

/* Birth sky visual state */
typedef struct {
    double jd;                          /* birth Julian Day */
    ring_today_t ring_indices;          /* ring segments for birth moment */
    int featured[BS_MAX_FEATURED];      /* top systems to feature (ts_system_t values) */
    int featured_count;                 /* how many featured (0-6) */
    aspect_list_t natal_aspects;        /* natal chart aspects (planet pairs) */
    double geo_longitudes[8];           /* geocentric planet longitudes at birth */
    bool active;                        /* true when viewing birth sky */
} birth_sky_t;

/* Birth card content (richer than daily card — shows full birth profile) */
typedef struct {
    char title[64];
    char line1[128];
    char line2[128];
    char line3[128];
    char detail[256];
} birth_card_t;

/* Compute birth sky state from a birth profile.
 * Computes ring indices at birth JD and selects featured systems. */
birth_sky_t birth_sky_from_profile(const birth_profile_t *profile);

/* Create an empty/inactive birth sky (no birth data). */
birth_sky_t birth_sky_empty(void);

/* Check if birth sky has valid data. */
bool birth_sky_is_valid(const birth_sky_t *bs);

/* Format birth card for Tzolkin (most detailed — full oracle). */
birth_card_t birth_card_tzolkin(const birth_profile_t *profile);

/* Format birth card for Western astrology. */
birth_card_t birth_card_astrology(const birth_profile_t *profile);

/* Format birth card for Chinese zodiac. */
birth_card_t birth_card_chinese(const birth_profile_t *profile);

/* Format birth card for I Ching. */
birth_card_t birth_card_iching(const birth_profile_t *profile);

/* Format birth card for a system by ts_system_t ID.
 * Returns empty card for unsupported systems. */
birth_card_t birth_card_for_system(const birth_profile_t *profile, int system_id);

/* Format a one-line summary of the most notable natal aspect.
 * buf: output buffer, buf_size: max bytes.
 * Returns buf, or "" if no aspects found. */
const char *birth_sky_top_aspect(const birth_sky_t *bs, char *buf, int buf_size);

/* Count aspects of a given type in the natal chart. */
int birth_sky_aspect_count(const birth_sky_t *bs, aspect_type_t type);

#endif /* TIME_BIRTH_SKY_H */
