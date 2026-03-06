#ifndef TIME_TZOLKIN_CARD_H
#define TIME_TZOLKIN_CARD_H

/* Seal color families (Dreamspell). Each seal belongs to one of 4 color families. */
typedef enum {
    TZOLKIN_RED    = 0,  /* Initiate */
    TZOLKIN_WHITE  = 1,  /* Refine */
    TZOLKIN_BLUE   = 2,  /* Transform */
    TZOLKIN_YELLOW = 3   /* Ripen */
} tzolkin_color_t;

/* Earth family groups (4 seals per family, 5 families). */
typedef enum {
    EARTH_POLAR    = 0,
    EARTH_CARDINAL = 1,
    EARTH_CORE     = 2,
    EARTH_SIGNAL   = 3,
    EARTH_GATEWAY  = 4
} earth_family_t;

/* Oracle cross for a given kin. */
typedef struct {
    int destiny;   /* seal index 0-19: the kin's own seal */
    int analog;    /* seal index 0-19: support/similar energy */
    int antipode;  /* seal index 0-19: challenge/opposite */
    int occult;    /* seal index 0-19: hidden power */
    int guide;     /* seal index 0-19: guide (depends on tone) */
} tzolkin_oracle_t;

/* Get the color family of a seal (0-19). */
tzolkin_color_t tzolkin_seal_color(int seal);

/* Get the color family name. */
const char *tzolkin_color_name(tzolkin_color_t color);

/* Get the Earth family of a seal (0-19). */
earth_family_t tzolkin_earth_family(int seal);

/* Get the Earth family name. */
const char *tzolkin_earth_family_name(earth_family_t family);

/* Get the tone keyword (1-13). e.g. "Purpose", "Challenge", ... "Endurance" */
const char *tzolkin_tone_keyword(int tone);

/* Get the tone action verb (1-13). e.g. "Unify", "Polarize", ... "Transcend" */
const char *tzolkin_tone_action(int tone);

/* Compute the oracle cross for a given seal (0-19) and tone (1-13). */
tzolkin_oracle_t tzolkin_oracle(int seal, int tone);

/* Get the wavespell number for a kin (1-260). Returns 1-20. */
int tzolkin_wavespell(int kin);

/* Get the seal that initiates a given wavespell (1-20). Returns seal index 0-19. */
int tzolkin_wavespell_seal(int wavespell);

#endif
