/* tarot.c — Tarot-Calendar Mapping (Thoth deck / Golden Dawn)
 *
 * Implementation of the 78-card Thoth Tarot correspondence system.
 * See tarot.h for full documentation.
 *
 * Sources: Aleister Crowley "777 and Other Qabalistic Writings",
 *          Aleister Crowley "The Book of Thoth",
 *          Golden Dawn correspondence tables. */

#include "tarot.h"

#include <math.h>
#include <stddef.h>

/* ===== Internal helpers ===== */

/* Normalize ecliptic longitude to [0, 360) */
static double normalize_lon(double deg)
{
    double n = fmod(deg, 360.0);
    if (n < 0.0) {
        n += 360.0;
    }
    return n;
}

/* Zodiac sign (0=Aries..11=Pisces) from ecliptic longitude */
static int sign_from_lon(double deg)
{
    double n = normalize_lon(deg);
    return (int)(n / 30.0) % 12;
}

/* Decan index (0-35) from ecliptic longitude */
static int decan_from_lon(double deg)
{
    double n = normalize_lon(deg);
    return (int)(n / 10.0) % 36;
}

/* ===== Major Arcana data (Thoth deck) ===== */

/* 22 Major Arcana with Golden Dawn correspondences.
 * Thoth swap: card 8 = Adjustment (Libra/Lamed), card 11 = Lust (Leo/Teth).
 * Planet indices: 0=Mercury, 1=Moon, 2=Venus, 3=Jupiter, 4=Mars, 5=Sun, 6=Saturn */
static const tarot_major_t MAJOR_DATA[22] = {
    {  0, "The Fool",       "Aleph",   TAROT_ATTR_ELEMENT, TAROT_ELEM_AIR,    0 },
    {  1, "The Magus",      "Beth",    TAROT_ATTR_PLANET,  0, /* Mercury */    1 },
    {  2, "The Priestess",  "Gimel",   TAROT_ATTR_PLANET,  1, /* Moon */       2 },
    {  3, "The Empress",    "Daleth",  TAROT_ATTR_PLANET,  2, /* Venus */      3 },
    {  4, "The Emperor",    "Heh",     TAROT_ATTR_ZODIAC,  0, /* Aries */      4 },
    {  5, "The Hierophant", "Vav",     TAROT_ATTR_ZODIAC,  1, /* Taurus */     5 },
    {  6, "The Lovers",     "Zayin",   TAROT_ATTR_ZODIAC,  2, /* Gemini */     6 },
    {  7, "The Chariot",    "Cheth",   TAROT_ATTR_ZODIAC,  3, /* Cancer */     7 },
    {  8, "Adjustment",     "Lamed",   TAROT_ATTR_ZODIAC,  6, /* Libra */      8 },
    {  9, "The Hermit",     "Yod",     TAROT_ATTR_ZODIAC,  5, /* Virgo */      9 },
    { 10, "Fortune",        "Kaph",    TAROT_ATTR_PLANET,  3, /* Jupiter */   10 },
    { 11, "Lust",           "Teth",    TAROT_ATTR_ZODIAC,  4, /* Leo */       11 },
    { 12, "The Hanged Man", "Mem",     TAROT_ATTR_ELEMENT, TAROT_ELEM_WATER, 12 },
    { 13, "Death",          "Nun",     TAROT_ATTR_ZODIAC,  7, /* Scorpio */   13 },
    { 14, "Art",            "Samekh",  TAROT_ATTR_ZODIAC,  8, /* Sagittarius*/14 },
    { 15, "The Devil",      "Ayin",    TAROT_ATTR_ZODIAC,  9, /* Capricorn */ 15 },
    { 16, "The Tower",      "Peh",     TAROT_ATTR_PLANET,  4, /* Mars */      16 },
    { 17, "The Star",       "Tzaddi",  TAROT_ATTR_ZODIAC, 10, /* Aquarius */  17 },
    { 18, "The Moon",       "Qoph",    TAROT_ATTR_ZODIAC, 11, /* Pisces */    18 },
    { 19, "The Sun",        "Resh",    TAROT_ATTR_PLANET,  5, /* Sun */       19 },
    { 20, "The Aeon",       "Shin",    TAROT_ATTR_ELEMENT, TAROT_ELEM_FIRE,  20 },
    { 21, "The Universe",   "Tav",     TAROT_ATTR_PLANET,  6, /* Saturn */    21 }
};

/* Zodiac sign -> Major Arcana number lookup.
 * Index: 0=Aries..11=Pisces */
static const int SIGN_TO_MAJOR[12] = {
     4, /* Aries       -> Emperor */
     5, /* Taurus      -> Hierophant */
     6, /* Gemini      -> Lovers */
     7, /* Cancer      -> Chariot */
    11, /* Leo         -> Lust (Thoth) */
     9, /* Virgo       -> Hermit */
     8, /* Libra       -> Adjustment (Thoth) */
    13, /* Scorpio     -> Death */
    14, /* Sagittarius -> Art */
    15, /* Capricorn   -> Devil */
    17, /* Aquarius    -> Star */
    18  /* Pisces      -> Moon */
};

/* Classical planet -> Major Arcana number lookup.
 * Index: 0=Mercury, 1=Moon, 2=Venus, 3=Jupiter, 4=Mars, 5=Sun, 6=Saturn */
static const int PLANET_TO_MAJOR[7] = {
     1, /* Mercury -> Magus */
     2, /* Moon    -> Priestess */
     3, /* Venus   -> Empress */
    10, /* Jupiter -> Fortune */
    16, /* Mars    -> Tower */
    19, /* Sun     -> Sun */
    21  /* Saturn  -> Universe */
};

/* ===== Decan card data ===== */

/* Suit for each zodiac sign (fire/earth/air/water triplicity).
 * Aries(fire)=Wands, Taurus(earth)=Disks, Gemini(air)=Swords, etc. */
static const tarot_suit_t SIGN_SUIT[12] = {
    TAROT_SUIT_WANDS,   /* Aries (fire) */
    TAROT_SUIT_DISKS,   /* Taurus (earth) */
    TAROT_SUIT_SWORDS,  /* Gemini (air) */
    TAROT_SUIT_CUPS,    /* Cancer (water) */
    TAROT_SUIT_WANDS,   /* Leo (fire) */
    TAROT_SUIT_DISKS,   /* Virgo (earth) */
    TAROT_SUIT_SWORDS,  /* Libra (air) */
    TAROT_SUIT_CUPS,    /* Scorpio (water) */
    TAROT_SUIT_WANDS,   /* Sagittarius (fire) */
    TAROT_SUIT_DISKS,   /* Capricorn (earth) */
    TAROT_SUIT_SWORDS,  /* Aquarius (air) */
    TAROT_SUIT_CUPS     /* Pisces (water) */
};

/* Rank offset for the first decan card of each sign's triplicity group.
 * Fire signs (Aries=0, Leo=4, Sag=8): card ranks start at 2, 5, 8
 * Water signs (Cancer=3, Scorpio=7, Pisces=11): card ranks start at 2, 5, 8
 * Air signs (Gemini=2, Libra=6, Aquarius=10): card ranks start at 8, 2, 5
 * Earth signs (Taurus=1, Virgo=5, Capricorn=9): card ranks start at 5, 8, 2
 *
 * The pattern per element is that the three signs of each element get ranks
 * (2,3,4), (5,6,7), (8,9,10) in the order the signs appear around the zodiac.
 *
 * For each sign, store the rank of decan 1 (decan 2 = rank+1, decan 3 = rank+2). */
static const int SIGN_BASE_RANK[12] = {
    2,  /* Aries       -> Wands 2,3,4 */
    5,  /* Taurus      -> Disks 5,6,7 */
    8,  /* Gemini      -> Swords 8,9,10 */
    2,  /* Cancer      -> Cups 2,3,4 */
    5,  /* Leo         -> Wands 5,6,7 */
    8,  /* Virgo       -> Disks 8,9,10 */
    2,  /* Libra       -> Swords 2,3,4 */
    5,  /* Scorpio     -> Cups 5,6,7 */
    8,  /* Sagittarius -> Wands 8,9,10 */
    2,  /* Capricorn   -> Disks 2,3,4 */
    5,  /* Aquarius    -> Swords 5,6,7 */
    8   /* Pisces      -> Cups 8,9,10 */
};

/* Pre-computed decan card names (36 entries) */
static const char *DECAN_NAMES[36] = {
    "2 of Wands",   "3 of Wands",   "4 of Wands",    /* Aries */
    "5 of Disks",   "6 of Disks",   "7 of Disks",    /* Taurus */
    "8 of Swords",  "9 of Swords",  "10 of Swords",  /* Gemini */
    "2 of Cups",    "3 of Cups",    "4 of Cups",     /* Cancer */
    "5 of Wands",   "6 of Wands",   "7 of Wands",    /* Leo */
    "8 of Disks",   "9 of Disks",   "10 of Disks",   /* Virgo */
    "2 of Swords",  "3 of Swords",  "4 of Swords",   /* Libra */
    "5 of Cups",    "6 of Cups",    "7 of Cups",     /* Scorpio */
    "8 of Wands",   "9 of Wands",   "10 of Wands",   /* Sagittarius */
    "2 of Disks",   "3 of Disks",   "4 of Disks",    /* Capricorn */
    "5 of Swords",  "6 of Swords",  "7 of Swords",   /* Aquarius */
    "8 of Cups",    "9 of Cups",    "10 of Cups"     /* Pisces */
};

/* ===== Court card data ===== */

/* Court card titles (4 suits x 4 ranks) */
static const char *COURT_TITLES[4][4] = {
    { "Knight of Wands",  "Queen of Wands",  "Prince of Wands",  "Princess of Wands"  },
    { "Knight of Cups",   "Queen of Cups",   "Prince of Cups",   "Princess of Cups"   },
    { "Knight of Swords", "Queen of Swords", "Prince of Swords", "Princess of Swords" },
    { "Knight of Disks",  "Queen of Disks",  "Prince of Disks",  "Princess of Disks"  }
};

/* Suit element mapping */
static const tarot_element_t SUIT_ELEMENT[4] = {
    TAROT_ELEM_FIRE,   /* Wands */
    TAROT_ELEM_WATER,  /* Cups */
    TAROT_ELEM_AIR,    /* Swords */
    TAROT_ELEM_EARTH   /* Disks */
};

/* Court card zodiac ranges (degrees).
 * Knight: straddles sign boundary (20 deg of prev sign + 20 deg of next).
 * Queen: next 20 deg.
 * Prince: next 20 deg.
 * Princess: rules quadrant, not degrees (-1, -1).
 *
 * Thoth court card assignments (Golden Dawn):
 * Wands (Fire):
 *   Knight: Scorpio 20 (230) - Sagittarius 20 (260)
 *   Queen:  Pisces 20 (350) - Aries 20 (20)
 *   Prince: Cancer 20 (110) - Leo 20 (140)
 *   Princess: quadrant
 * Cups (Water):
 *   Knight: Aquarius 20 (320) - Pisces 20 (350)
 *   Queen:  Gemini 20 (80) - Cancer 20 (110)
 *   Prince: Libra 20 (200) - Scorpio 20 (230)
 *   Princess: quadrant
 * Swords (Air):
 *   Knight: Taurus 20 (50) - Gemini 20 (80)
 *   Queen:  Virgo 20 (170) - Libra 20 (200)
 *   Prince: Capricorn 20 (290) - Aquarius 20 (320)
 *   Princess: quadrant
 * Disks (Earth):
 *   Knight: Leo 20 (140) - Virgo 20 (170)
 *   Queen:  Sagittarius 20 (260) - Capricorn 20 (290)
 *   Prince: Aries 20 (20) - Taurus 20 (50)
 *   Princess: quadrant
 */
static const int COURT_ZODIAC[4][4][2] = {
    /* Wands */
    { {230, 260}, {350,  20}, {110, 140}, {-1, -1} },
    /* Cups */
    { {320, 350}, { 80, 110}, {200, 230}, {-1, -1} },
    /* Swords */
    { { 50,  80}, {170, 200}, {290, 320}, {-1, -1} },
    /* Disks */
    { {140, 170}, {260, 290}, { 20,  50}, {-1, -1} }
};

/* ===== String tables ===== */

static const char *SUIT_NAMES[6] = {
    "Wands", "Cups", "Swords", "Disks", "Major Arcana", "Unknown"
};

static const char *ELEMENT_NAMES[6] = {
    "Fire", "Water", "Air", "Earth", "Spirit", "Unknown"
};

/* ===== Public API ===== */

tarot_major_t tarot_major_get(int number)
{
    if (number < 0 || number > 21) {
        tarot_major_t invalid = { -1, "Unknown", "Unknown",
                                  TAROT_ATTR_COUNT, -1, -1 };
        return invalid;
    }
    return MAJOR_DATA[number];
}

const char *tarot_major_name(int number)
{
    if (number < 0 || number > 21) {
        return "Unknown";
    }
    return MAJOR_DATA[number].name;
}

int tarot_major_for_sign(int sign)
{
    if (sign < 0 || sign > 11) {
        return -1;
    }
    return SIGN_TO_MAJOR[sign];
}

int tarot_major_for_planet(int planet)
{
    if (planet < 0 || planet > 6) {
        return -1;
    }
    return PLANET_TO_MAJOR[planet];
}

tarot_minor_t tarot_decan_card(double ecl_lon_deg)
{
    int di = decan_from_lon(ecl_lon_deg);
    int sign = di / 3;
    int decan_in_sign = di % 3; /* 0, 1, or 2 */

    tarot_minor_t result;
    result.suit = SIGN_SUIT[sign];
    result.rank = SIGN_BASE_RANK[sign] + decan_in_sign;
    result.decan_index = di;
    result.zodiac_sign = sign;
    result.name = DECAN_NAMES[di];
    return result;
}

tarot_court_t tarot_court_get(int suit, int rank)
{
    if (suit < 0 || suit > 3 || rank < 11 || rank > 14) {
        tarot_court_t invalid = { TAROT_SUIT_MAJOR, -1, "Unknown",
                                  -1, -1, TAROT_ELEM_COUNT };
        return invalid;
    }
    int ri = rank - 11; /* 0=Knight, 1=Queen, 2=Prince, 3=Princess */

    tarot_court_t result;
    result.suit = (tarot_suit_t)suit;
    result.rank = rank;
    result.title = COURT_TITLES[suit][ri];
    result.zodiac_start = COURT_ZODIAC[suit][ri][0];
    result.zodiac_end = COURT_ZODIAC[suit][ri][1];
    result.element = SUIT_ELEMENT[suit];
    return result;
}

void tarot_for_longitude(double ecl_lon_deg, int *major_out,
                         tarot_minor_t *minor_out)
{
    int sign = sign_from_lon(ecl_lon_deg);

    if (major_out != NULL) {
        *major_out = SIGN_TO_MAJOR[sign];
    }
    if (minor_out != NULL) {
        *minor_out = tarot_decan_card(ecl_lon_deg);
    }
}

const char *tarot_suit_name(tarot_suit_t suit)
{
    if (suit < 0 || suit >= TAROT_SUIT_COUNT) {
        return "Unknown";
    }
    return SUIT_NAMES[suit];
}

const char *tarot_element_name(tarot_element_t elem)
{
    if (elem < 0 || elem >= TAROT_ELEM_COUNT) {
        return "Unknown";
    }
    return ELEMENT_NAMES[elem];
}

int tarot_path_on_tree(int major_number)
{
    if (major_number < 0 || major_number > 21) {
        return -1;
    }
    return MAJOR_DATA[major_number].tree_path;
}

const char *tarot_hebrew_letter(int major_number)
{
    if (major_number < 0 || major_number > 21) {
        return "Unknown";
    }
    return MAJOR_DATA[major_number].hebrew_letter;
}

int tarot_major_count(void)
{
    return 22;
}

int tarot_decan_card_count(void)
{
    return 36;
}
