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

/* Castle type (5 castles of 52 kin each) */
typedef enum {
    CASTLE_RED_EASTERN     = 1,
    CASTLE_WHITE_NORTHERN  = 2,
    CASTLE_BLUE_WESTERN    = 3,
    CASTLE_YELLOW_SOUTHERN = 4,
    CASTLE_GREEN_CENTRAL   = 5
} tzolkin_castle_t;

/* Unified display data for Tzolkin Kin Card */
typedef struct {
    int kin;                    /* 1-260 */
    int seal;                   /* 0-19 */
    int tone;                   /* 1-13 */
    int castle;                 /* 1-5 */
    int wavespell;              /* 1-20 */
    int wavespell_position;     /* 1-13 (position within wavespell) */
    tzolkin_color_t color;
    char title[64];             /* "Kin 207 · Blue Crystal Hand" */
    char color_name[8];         /* "Blue" */
    char castle_name[48];       /* "Green Central Castle of Enchantment" */
    char tone_keyword[20];      /* "Cooperation" */
    char tone_action[16];       /* "Dedicate" */
    char seal_keyword[24];      /* "Accomplishment" */
    char oracle_guide[24];      /* seal name of guide */
    char oracle_analog[24];     /* seal name of analog */
    char oracle_antipode[24];   /* seal name of antipode */
    char oracle_occult[24];     /* seal name of occult */
    char summary[96];           /* "Blue Crystal Hand · WS 16 · Castle 4 Yellow Southern" */
} tzolkin_display_t;

/* Castle number (1-5) from kin (1-260). Returns 1 for invalid. */
tzolkin_castle_t tzolkin_castle(int kin);

/* Castle short name: "Red Eastern", "White Northern", etc. */
const char *tzolkin_castle_short_name(tzolkin_castle_t castle);

/* Castle full name: "Red Eastern Castle of Turning", etc. */
const char *tzolkin_castle_full_name(tzolkin_castle_t castle);

/* Castle name (alias for short name). */
const char *tzolkin_castle_name(tzolkin_castle_t castle);

/* Castle color: "Red", "White", "Blue", "Yellow", "Green" */
const char *tzolkin_castle_color_name(tzolkin_castle_t castle);

/* Wavespell position within wavespell (1-13). Returns 1 for invalid. */
int tzolkin_wavespell_position(int kin);

/* Seal keyword (Dreamspell). e.g. Dragon="Birth", Wind="Spirit" */
const char *tzolkin_seal_keyword(int seal);

/* Build complete display from kin number (1-260). */
tzolkin_display_t tzolkin_display_from_kin(int kin);

/* Build complete display from Julian Day. */
tzolkin_display_t tzolkin_display_from_jd(double jd);

#endif
