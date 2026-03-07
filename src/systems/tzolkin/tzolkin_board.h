#ifndef TIME_TZOLKIN_BOARD_H
#define TIME_TZOLKIN_BOARD_H

#include <stdbool.h>

/* Color families (4 colors, cycle through seals) */
typedef enum {
    TZOLKIN_BOARD_COLOR_RED = 0,    /* Initiates */
    TZOLKIN_BOARD_COLOR_WHITE,      /* Refines */
    TZOLKIN_BOARD_COLOR_BLUE,       /* Transforms */
    TZOLKIN_BOARD_COLOR_YELLOW,     /* Ripens */
    TZOLKIN_BOARD_COLOR_COUNT
} tzolkin_board_color_t;

/* 5 Castles (4 wavespells each = 52 kin each) */
typedef enum {
    TZOLKIN_BOARD_CASTLE_RED = 0,    /* Court of Birth (kin 1-52) */
    TZOLKIN_BOARD_CASTLE_WHITE,      /* Court of Death (kin 53-104) */
    TZOLKIN_BOARD_CASTLE_BLUE,       /* Court of Magic (kin 105-156) */
    TZOLKIN_BOARD_CASTLE_YELLOW,     /* Court of Intelligence (kin 157-208) */
    TZOLKIN_BOARD_CASTLE_GREEN,      /* Court of Enchantment (kin 209-260) */
    TZOLKIN_BOARD_CASTLE_COUNT
} tzolkin_board_castle_t;

/* Full kin data for the board */
typedef struct {
    int kin;                        /* 1-260 */
    int tone;                       /* 1-13 */
    int seal;                       /* 0-19 */
    tzolkin_board_color_t color;    /* Red/White/Blue/Yellow */
    int wavespell;                  /* 1-20 (which wavespell) */
    int wavespell_position;         /* 1-13 (position within wavespell) */
    tzolkin_board_castle_t castle;  /* Which of 5 castles */
    bool is_portal;                 /* Galactic Activation Portal day */
    int harmonic;                   /* Harmonic number (1-65, groups of 4 kin) */
} tzolkin_kin_t;

/* Get kin by number (1-260). Returns zeroed struct for invalid. */
tzolkin_kin_t tzolkin_board_kin(int kin_number);

/* Get kin by tone (1-13) and seal (0-19). */
tzolkin_kin_t tzolkin_board_by_tone_seal(int tone, int seal);

/* Get kin number from tone and seal. Returns 0 for invalid. */
int tzolkin_board_kin_number(int tone, int seal);

/* Is this kin a Galactic Activation Portal? */
bool tzolkin_board_is_portal(int kin_number);

/* Get color family for a seal index (0-19). */
tzolkin_board_color_t tzolkin_board_seal_color(int seal);

/* Get castle for a kin number (1-260). */
tzolkin_board_castle_t tzolkin_board_castle(int kin_number);

/* Get wavespell number for a kin (1-20). */
int tzolkin_board_wavespell(int kin_number);

/* Get harmonic number for a kin (1-65). */
int tzolkin_board_harmonic(int kin_number);

/* Color name string. */
const char *tzolkin_board_color_name(tzolkin_board_color_t color);

/* Castle name string. */
const char *tzolkin_board_castle_name(tzolkin_board_castle_t castle);

/* Total number of portal days. */
int tzolkin_board_portal_count(void);

#endif
