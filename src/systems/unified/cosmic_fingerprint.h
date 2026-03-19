/* cosmic_fingerprint.h — Display-ready multi-system identity from birth data.
 *
 * Takes a birth_profile_t and produces card-ready entries for each system:
 * headline, subtitle, glyph ID, and reveal order for the L1.4 identity
 * ceremony and L2.6 fingerprint card.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_COSMIC_FINGERPRINT_H
#define TIME_COSMIC_FINGERPRINT_H

#include "birth_profile.h"

#define CF_MAX_SYSTEMS  16
#define CF_HEADLINE_MAX 64
#define CF_SUBTITLE_MAX 96
#define CF_SUMMARY_MAX  2048

/* System identifiers for fingerprint entries. */
typedef enum {
    CF_SYS_GREGORIAN = 0,
    CF_SYS_ASTROLOGY,
    CF_SYS_CHINESE,
    CF_SYS_TZOLKIN,
    CF_SYS_ICHING,
    CF_SYS_HUMAN_DESIGN,
    CF_SYS_HEBREW,
    CF_SYS_ISLAMIC,
    CF_SYS_BUDDHIST,
    CF_SYS_COUNT
} cf_system_t;

/* A single system's identity card data. */
typedef struct {
    cf_system_t system;
    const char *system_name;            /* "Tzolkin", "Astrology", etc. */
    char headline[CF_HEADLINE_MAX];     /* "Yellow Rhythmic Warrior" */
    char subtitle[CF_SUBTITLE_MAX];     /* "Kin 196, Wavespell 15" */
    int glyph_id;                       /* System-specific icon index */
    int reveal_order;                   /* 1-based order in reveal sequence */
    int available;                      /* 1 if this system has data */
} cf_entry_t;

/* Complete cosmic fingerprint. */
typedef struct {
    cf_entry_t entries[CF_SYS_COUNT];
    int count;                          /* Number of available entries */
    int birth_year;
    int birth_month;
    int birth_day;
} cf_fingerprint_t;

/* Compute fingerprint from a computed birth profile. */
cf_fingerprint_t cf_compute(const birth_profile_t *profile);

/* Get entry by system ID. Returns NULL if system unavailable or out of range. */
const cf_entry_t *cf_entry(const cf_fingerprint_t *fp, cf_system_t system);

/* Get headline text for a system. Returns "" if unavailable. */
const char *cf_headline(const cf_fingerprint_t *fp, cf_system_t system);

/* Get system display name for an ID. */
const char *cf_system_name(cf_system_t system);

/* Fill out_systems with system IDs in reveal order. Returns count written. */
int cf_reveal_order(const cf_fingerprint_t *fp, cf_system_t *out_systems,
                    int max);

/* Format entire fingerprint as shareable multi-line text.
 * Returns bytes written (excluding NUL). */
int cf_format_summary(const cf_fingerprint_t *fp, char *buf, int buf_size);

/* Count of available (populated) entries. */
int cf_available_count(const cf_fingerprint_t *fp);

#endif /* TIME_COSMIC_FINGERPRINT_H */
