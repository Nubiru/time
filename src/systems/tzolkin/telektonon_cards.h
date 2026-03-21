#ifndef TIME_TELEKTONON_CARDS_H
#define TIME_TELEKTONON_CARDS_H

/* Telektonon Cards — 52 cards in 4 Books of Wisdom.
 *
 * The Telektonon game uses 52 cards divided into 4 Books of Wisdom.
 * Each day, cards are placed in a 7-position layout called the
 * Cronometro Diario Universal (Daily Universal Chronometer).
 *
 * Books:
 *   1. Lost Book of the Seven Generations   (7 cards)
 *   2. Book of Galactic Time                (13 cards)
 *   3. Book of Cosmic Form                  (4 cards)
 *   4. Telepathic Book of Planetary Redemption (28 cards)
 *
 * Cronometro positions (7):
 *   Tower, Crown, Heart, Galactic Wing,
 *   Solar Wing Weekly, Solar Wing Daily, Root
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

#define TK_BOOK_COUNT     4
#define TK_TOTAL_CARD_COUNT 52
#define TK_POSITION_COUNT 7

/* --- Enums --- */

typedef enum {
    TK_BOOK_SEVEN_GENERATIONS     = 0,
    TK_BOOK_GALACTIC_TIME         = 1,
    TK_BOOK_COSMIC_FORM           = 2,
    TK_BOOK_PLANETARY_REDEMPTION  = 3
} tk_book_t;

typedef enum {
    TK_POS_TOWER              = 0,
    TK_POS_CROWN              = 1,
    TK_POS_HEART              = 2,
    TK_POS_GALACTIC_WING      = 3,
    TK_POS_SOLAR_WING_WEEKLY  = 4,
    TK_POS_SOLAR_WING_DAILY   = 5,
    TK_POS_ROOT               = 6
} tk_position_t;

/* --- Structs --- */

typedef struct {
    tk_book_t    id;
    const char  *name;
    int          card_count;
    const char  *position_name;
} tk_book_info_t;

typedef struct {
    tk_position_t  position;
    const char    *name;
    const char    *name_es;
    tk_book_t      source_book;
    const char    *changes;
} tk_card_position_t;

/* --- Functions --- */

/* Number of books (always 4). */
int tk_book_count(void);

/* Get book info by book enum. Returns zeroed struct for invalid. */
tk_book_info_t tk_book_get(tk_book_t book);

/* Book name string. Returns NULL for invalid. */
const char *tk_book_name(tk_book_t book);

/* Card count for a book. Returns 0 for invalid. */
int tk_book_card_count(tk_book_t book);

/* Total cards across all books (always 52). */
int tk_total_cards(void);

/* Number of cronometro positions (always 7). */
int tk_position_count(void);

/* Get position data by index (0-6). Returns zeroed struct for invalid. */
tk_card_position_t tk_position_get(int index);

/* Position name (English). Returns NULL for invalid. */
const char *tk_position_name(tk_position_t pos);

/* Is the Tower position active on this day of the moon?
 * Active on days 1, 6, 23, 28. Returns 1 or 0. */
int tk_is_tower_active(int day_of_moon);

#endif /* TIME_TELEKTONON_CARDS_H */
