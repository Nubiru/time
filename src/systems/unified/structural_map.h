/* structural_map.h — Structural Pattern Map: cross-system concordances
 *
 * Pure data module providing structural parallels between time/knowledge
 * systems as static mapping tables. Documents mathematical concordances:
 * Kabbalah <-> Human Design, I Ching <-> HD Gates <-> DNA Codons,
 * Zodiac <-> Hebrew Letters <-> Chinese Branches, Classical Planets <->
 * Hebrew Double Letters <-> Chakras <-> Days.
 *
 * Sources: Golden Dawn correspondences, Sefer Yetzirah, traditional
 * Kabbalistic/Hindu/Chinese systems (all public domain).
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_STRUCTURAL_MAP_H
#define TIME_STRUCTURAL_MAP_H

#include <stdbool.h>

/* System identifiers */
typedef enum {
    SYS_KABBALAH = 0,
    SYS_HUMAN_DESIGN,
    SYS_ICHING,
    SYS_ZODIAC,
    SYS_HEBREW_LETTERS,
    SYS_CHINESE_BRANCHES,
    SYS_PLANETS_CLASSICAL,
    SYS_CHAKRAS,
    SYS_WEEKDAYS,
    SYS_DNA_CODONS,
    SYS_TAROT_MAJOR,
    SYS_NAKSHATRAS,
    SYS_COUNT
} system_id_t;

/* A single concordance entry: one number appearing across systems */
typedef struct {
    int number;              /* The shared number (e.g., 7, 12, 22, 64) */
    const char *description; /* What this number represents */
    int system_count;        /* How many systems share this number */
    const char *systems[6];  /* System names that share this number */
} concordance_t;

/* Planetary correspondence row (7 classical planets) */
typedef struct {
    int planet_index;        /* 0-6 */
    const char *planet;      /* Planet name */
    const char *hebrew_letter; /* Hebrew double letter */
    const char *weekday;     /* Day of week */
    const char *chakra;      /* Chakra association */
    int sefirah_index;       /* Sefirot index (Tiferet=5, Yesod=8, etc.) */
} planet_row_t;

/* Zodiac correspondence row (12 signs) */
typedef struct {
    int sign_index;          /* 0-11 */
    const char *zodiac;      /* Zodiac sign name */
    const char *hebrew_letter; /* Hebrew simple letter */
    const char *chinese_branch; /* Earthly Branch animal */
    const char *tarot;       /* Major Arcana card */
} zodiac_row_t;

/* Get number of concordances. */
int structural_concordance_count(void);

/* Get concordance by index. Returns entry with number=-1 if invalid. */
concordance_t structural_concordance_get(int index);

/* Find concordance for a specific number. Returns entry with number=-1 if not found. */
concordance_t structural_concordance_for_number(int number);

/* Get planetary correspondence row. Returns row with planet="?" if invalid. */
planet_row_t structural_planet_row(int planet_index);

/* Get zodiac correspondence row. Returns row with zodiac="?" if invalid. */
zodiac_row_t structural_zodiac_row(int sign_index);

/* Get system name by ID. Returns "?" for invalid. */
const char *structural_system_name(system_id_t sys);

/* Number of systems tracked. */
int structural_system_count(void);

/* Get Sefirot-to-HD-Center mapping (10 Sefirot -> 9 Centers).
 * sefirah_index: 0-9. Returns HD center name or "?" for invalid. */
const char *structural_sefirah_to_hd_center(int sefirah_index);

/* Get HD Center to Sefirot reverse mapping.
 * hd_center_index: 0-8. Returns primary Sefirah index or -1 for invalid. */
int structural_hd_center_to_sefirah(int hd_center_index);

/* How many sacred numbers are tracked? */
int structural_sacred_number_count(void);

/* Get a sacred number by index. Returns -1 for invalid. */
int structural_sacred_number(int index);

/* Verify: does number N appear in M or more systems? */
bool structural_appears_in_systems(int number, int min_systems);

#endif /* TIME_STRUCTURAL_MAP_H */
