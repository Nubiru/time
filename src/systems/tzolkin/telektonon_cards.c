#include "telektonon_cards.h"
#include <string.h>

/* Telektonon Cards — 4 Books of Wisdom and 7 Cronometro positions.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

/* --- Book data --- */

static const tk_book_info_t BOOKS[TK_BOOK_COUNT] = {
    {   /* Book 1 */
        .id            = TK_BOOK_SEVEN_GENERATIONS,
        .name          = "Lost Book of the Seven Generations",
        .card_count    = 7,
        .position_name = "Root"
    },
    {   /* Book 2 */
        .id            = TK_BOOK_GALACTIC_TIME,
        .name          = "Book of Galactic Time",
        .card_count    = 13,
        .position_name = "Galactic Wing"
    },
    {   /* Book 3 */
        .id            = TK_BOOK_COSMIC_FORM,
        .name          = "Book of Cosmic Form",
        .card_count    = 4,
        .position_name = "Solar Wing"
    },
    {   /* Book 4 */
        .id            = TK_BOOK_PLANETARY_REDEMPTION,
        .name          = "Telepathic Book of Planetary Redemption",
        .card_count    = 28,
        .position_name = "Crown / Tower"
    }
};

/* --- Cronometro position data --- */

static const tk_card_position_t POSITIONS[TK_POSITION_COUNT] = {
    {
        .position    = TK_POS_TOWER,
        .name        = "Tower",
        .name_es     = "Torre",
        .source_book = TK_BOOK_PLANETARY_REDEMPTION,
        .changes     = "Days 1,6,23,28 only"
    },
    {
        .position    = TK_POS_CROWN,
        .name        = "Crown",
        .name_es     = "Corona",
        .source_book = TK_BOOK_PLANETARY_REDEMPTION,
        .changes     = "Daily"
    },
    {
        .position    = TK_POS_HEART,
        .name        = "Heart",
        .name_es     = "Corazon",
        .source_book = TK_BOOK_GALACTIC_TIME,
        .changes     = "Monthly"
    },
    {
        .position    = TK_POS_GALACTIC_WING,
        .name        = "Galactic Wing",
        .name_es     = "Ala Galactica",
        .source_book = TK_BOOK_GALACTIC_TIME,
        .changes     = "Daily"
    },
    {
        .position    = TK_POS_SOLAR_WING_WEEKLY,
        .name        = "Solar Wing Weekly",
        .name_es     = "Ala Solar Semanal",
        .source_book = TK_BOOK_COSMIC_FORM,
        .changes     = "Weekly"
    },
    {
        .position    = TK_POS_SOLAR_WING_DAILY,
        .name        = "Solar Wing Daily",
        .name_es     = "Ala Solar Diaria",
        .source_book = TK_BOOK_COSMIC_FORM,
        .changes     = "Daily"
    },
    {
        .position    = TK_POS_ROOT,
        .name        = "Root",
        .name_es     = "Raiz",
        .source_book = TK_BOOK_SEVEN_GENERATIONS,
        .changes     = "Annually"
    }
};

/* --- Public functions --- */

int tk_book_count(void)
{
    return TK_BOOK_COUNT;
}

tk_book_info_t tk_book_get(tk_book_t book)
{
    if ((int)book < 0 || (int)book >= TK_BOOK_COUNT) {
        tk_book_info_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return BOOKS[(int)book];
}

const char *tk_book_name(tk_book_t book)
{
    if ((int)book < 0 || (int)book >= TK_BOOK_COUNT)
        return NULL;
    return BOOKS[(int)book].name;
}

int tk_book_card_count(tk_book_t book)
{
    if ((int)book < 0 || (int)book >= TK_BOOK_COUNT)
        return 0;
    return BOOKS[(int)book].card_count;
}

int tk_total_cards(void)
{
    return TK_TOTAL_CARD_COUNT;
}

int tk_position_count(void)
{
    return TK_POSITION_COUNT;
}

tk_card_position_t tk_position_get(int index)
{
    if (index < 0 || index >= TK_POSITION_COUNT) {
        tk_card_position_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return POSITIONS[index];
}

const char *tk_position_name(tk_position_t pos)
{
    if ((int)pos < 0 || (int)pos >= TK_POSITION_COUNT)
        return NULL;
    return POSITIONS[(int)pos].name;
}

int tk_is_tower_active(int day_of_moon)
{
    return (day_of_moon == 1  || day_of_moon == 6 ||
            day_of_moon == 23 || day_of_moon == 28) ? 1 : 0;
}
