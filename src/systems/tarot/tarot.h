/* tarot.h — Tarot-Calendar Mapping (Thoth deck / Golden Dawn)
 *
 * Pure data module mapping the 78-card Thoth Tarot deck to zodiac,
 * planets, elements, and the Kabbalah Tree of Life paths.
 *
 * 22 Major Arcana: each maps to a Hebrew letter, a zodiac sign / planet /
 * element, and a path on the Tree of Life (0-21).
 *
 * 36 Decan (number) cards: each 10-degree decan maps to a number card
 * (2-10) in one of four suits (Wands=Fire, Cups=Water, Swords=Air,
 * Disks=Earth).
 *
 * 16 Court cards: Knight(11), Queen(12), Prince(13), Princess(14) in
 * each suit, ruling specific zodiac degree ranges (except Princess
 * which rules a quadrant).
 *
 * Self-contained: computes zodiac sign from longitude internally.
 * No dependencies on zodiac.h or decan_stars.h.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Aleister Crowley "777 and Other Qabalistic Writings",
 *          Aleister Crowley "The Book of Thoth",
 *          Golden Dawn correspondence tables. */

#ifndef TIME_TAROT_H
#define TIME_TAROT_H

/* Classical planet indices (7 planets, internal to this module) */
#define TAROT_PLANET_MERCURY  0
#define TAROT_PLANET_MOON     1
#define TAROT_PLANET_VENUS    2
#define TAROT_PLANET_JUPITER  3
#define TAROT_PLANET_MARS     4
#define TAROT_PLANET_SUN      5
#define TAROT_PLANET_SATURN   6
#define TAROT_PLANET_COUNT    7

typedef enum {
    TAROT_ELEM_FIRE = 0,
    TAROT_ELEM_WATER,
    TAROT_ELEM_AIR,
    TAROT_ELEM_EARTH,
    TAROT_ELEM_SPIRIT,
    TAROT_ELEM_COUNT
} tarot_element_t;

typedef enum {
    TAROT_SUIT_WANDS = 0,   /* Fire */
    TAROT_SUIT_CUPS,        /* Water */
    TAROT_SUIT_SWORDS,      /* Air */
    TAROT_SUIT_DISKS,       /* Earth */
    TAROT_SUIT_MAJOR,       /* Major Arcana (not a pip suit) */
    TAROT_SUIT_COUNT
} tarot_suit_t;

typedef enum {
    TAROT_ATTR_ZODIAC = 0,
    TAROT_ATTR_PLANET,
    TAROT_ATTR_ELEMENT,
    TAROT_ATTR_COUNT
} tarot_attr_type_t;

/* Major Arcanum data */
typedef struct {
    int number;               /* 0-21, or -1 for invalid */
    const char *name;         /* Thoth deck name */
    const char *hebrew_letter;/* Hebrew letter name (e.g., "Aleph") */
    tarot_attr_type_t attr_type; /* zodiac, planet, or element */
    int attr_index;           /* index into zodiac(0-11), planet(0-6), or element enum */
    int tree_path;            /* Tree of Life path index (0-21), -1 for invalid */
} tarot_major_t;

/* Minor Arcanum (decan/number card) data */
typedef struct {
    tarot_suit_t suit;        /* Wands, Cups, Swords, or Disks */
    int rank;                 /* 2-10 */
    int decan_index;          /* 0-35 (sequential decan index) */
    int zodiac_sign;          /* 0=Aries..11=Pisces */
    const char *name;         /* e.g., "2 of Wands" */
} tarot_minor_t;

/* Court card data */
typedef struct {
    tarot_suit_t suit;        /* Wands, Cups, Swords, or Disks */
    int rank;                 /* 11=Knight, 12=Queen, 13=Prince, 14=Princess, -1=invalid */
    const char *title;        /* e.g., "Knight of Wands" */
    int zodiac_start;         /* start degree (0-359), or -1 for Princess */
    int zodiac_end;           /* end degree (0-359), or -1 for Princess */
    tarot_element_t element;  /* suit element */
} tarot_court_t;

/* Get Major Arcanum by number (0-21).
 * Returns struct with number=-1 for invalid input. */
tarot_major_t tarot_major_get(int number);

/* Get Major Arcanum name by number (0-21).
 * Returns "Unknown" for invalid. */
const char *tarot_major_name(int number);

/* Get the Major Arcanum number for a zodiac sign (0=Aries..11=Pisces).
 * Returns -1 for invalid sign. */
int tarot_major_for_sign(int sign);

/* Get the Major Arcanum number for a classical planet (0-6).
 * Uses internal planet indexing: 0=Mercury, 1=Moon, 2=Venus,
 * 3=Jupiter, 4=Mars, 5=Sun, 6=Saturn.
 * Returns -1 for invalid planet. */
int tarot_major_for_planet(int planet);

/* Get the decan (number) card for an ecliptic longitude (degrees).
 * Handles wrapping for values outside [0,360). */
tarot_minor_t tarot_decan_card(double ecl_lon_deg);

/* Get a court card by suit (0-3) and rank (11-14).
 * Returns struct with rank=-1 for invalid input. */
tarot_court_t tarot_court_get(int suit, int rank);

/* Get both major and minor cards for an ecliptic longitude.
 * Either output pointer may be NULL.
 * major_out receives the Major Arcanum number for the zodiac sign.
 * minor_out receives the decan card data. */
void tarot_for_longitude(double ecl_lon_deg, int *major_out,
                         tarot_minor_t *minor_out);

/* Suit name string. Returns "Unknown" for invalid. */
const char *tarot_suit_name(tarot_suit_t suit);

/* Element name string. Returns "Unknown" for invalid. */
const char *tarot_element_name(tarot_element_t elem);

/* Tree of Life path index for a Major Arcanum (0-21).
 * Returns -1 for invalid number. */
int tarot_path_on_tree(int major_number);

/* Hebrew letter name for a Major Arcanum (0-21).
 * Returns "Unknown" for invalid number. */
const char *tarot_hebrew_letter(int major_number);

/* Total Major Arcana count (always 22). */
int tarot_major_count(void);

/* Total decan card count (always 36). */
int tarot_decan_card_count(void);

#endif /* TIME_TAROT_H */
