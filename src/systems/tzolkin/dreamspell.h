#ifndef TIME_DREAMSPELL_H
#define TIME_DREAMSPELL_H

/* Dreamspell Oracle Engine — Arguelles system.
 *
 * Computes oracle relationships, wavespell/castle membership,
 * earth families, color meanings, and tone keywords for any
 * kin in the 260-day Tzolkin.
 *
 * Seal indexing (0-19): Dragon, Wind, Night, Seed, Serpent,
 * World-Bridger, Hand, Star, Moon, Dog, Monkey, Human,
 * Skywalker, Wizard, Eagle, Warrior, Earth, Mirror, Storm, Sun.
 *
 * Color mapping: seal % 4 → 0=Red, 1=White, 2=Blue, 3=Yellow.
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 */

typedef struct {
    int destiny;    /* the kin itself (1-260) */
    int guide;      /* guide kin (same color, tone-dependent) */
    int analog;     /* analog partner */
    int antipode;   /* challenge/strengthening */
    int occult;     /* hidden power */
    int hidden;     /* kin 261 - destiny (hidden teacher) */
} dreamspell_oracle_t;

typedef struct {
    int number;             /* wavespell number 1-20 */
    int seal;               /* initiating seal (0-19) */
    const char *purpose;    /* wavespell purpose keyword (seal name) */
    const char *action;     /* wavespell action keyword (from color) */
    const char *power;      /* wavespell power keyword (from color) */
} dreamspell_wavespell_t;

typedef struct {
    int number;             /* castle number 1-5 */
    const char *name;       /* "Red Castle of Turning", etc. */
    const char *color;      /* "Red", "White", "Blue", "Yellow", "Green" */
    const char *theme;      /* "Birth", "Death", "Magic", "Intelligence", "Enchantment" */
    int start_kin;          /* first kin in this castle */
    int end_kin;            /* last kin in this castle */
} dreamspell_castle_t;

typedef struct {
    const char *name;       /* "Polar", "Cardinal", "Core", "Signal", "Gateway" */
    int members[4];         /* seal indices (0-19) */
    const char *direction;  /* "North", "East", "South", "West", "Center" */
} dreamspell_family_t;

typedef struct {
    const char *name;       /* "Red", "White", "Blue", "Yellow" */
    const char *action;     /* "Initiates", "Refines", "Transforms", "Ripens" */
    const char *quality;    /* "Birth", "Death", "Magic", "Intelligence" */
} dreamspell_color_t;

typedef struct {
    int number;             /* 1-13 */
    const char *name;       /* "Magnetic", "Lunar", etc. */
    const char *action;     /* "Unify", "Polarize", etc. */
    const char *power;      /* "Purpose", "Challenge", etc. */
} dreamspell_tone_t;

/* Compute the 5-position oracle for a kin (1-260).
 * Returns zeroed struct for invalid kin. */
dreamspell_oracle_t dreamspell_oracle(int kin);

/* Get wavespell info for a kin (1-260).
 * Returns zeroed struct for invalid kin. */
dreamspell_wavespell_t dreamspell_wavespell(int kin);

/* Get castle info for a kin (1-260).
 * Returns zeroed struct for invalid kin. */
dreamspell_castle_t dreamspell_castle(int kin);

/* Get earth family for a seal (0-19).
 * Returns struct with name="?" for invalid seal. */
dreamspell_family_t dreamspell_earth_family(int seal);

/* Get color info for a seal (0-19).
 * Returns struct with name="?" for invalid seal. */
dreamspell_color_t dreamspell_color(int seal);

/* Get tone info for a tone number (1-13).
 * Returns struct with number=0, name="?" for invalid tone. */
dreamspell_tone_t dreamspell_tone(int tone);

/* Get the initiating seal of the wavespell containing kin.
 * Returns -1 for invalid kin. */
int dreamspell_wavespell_seal(int kin);

/* Get position within wavespell (1-13).
 * Returns 0 for invalid kin. */
int dreamspell_wavespell_position(int kin);

/* Number of earth families (always 5). */
int dreamspell_family_count(void);

/* Number of castles (always 5). */
int dreamspell_castle_count(void);

#endif /* TIME_DREAMSPELL_H */
