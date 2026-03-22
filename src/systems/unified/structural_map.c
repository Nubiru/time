/* structural_map.c — Structural Pattern Map implementation
 *
 * Pure data module: all data as static const arrays.
 * No malloc, no globals, no side effects.
 *
 * Sources: Golden Dawn correspondences (public domain),
 * Sefer Yetzirah tradition (public domain),
 * traditional Kabbalistic, Hindu, Buddhist, Chinese systems. */

#include "structural_map.h"
#include <stddef.h>

/* ===== System Names ===== */

static const char *SYSTEM_NAMES[SYS_COUNT] = {
    "Kabbalah",
    "Human Design",
    "I Ching",
    "Zodiac",
    "Hebrew Letters",
    "Chinese Branches",
    "Classical Planets",
    "Chakras",
    "Weekdays",
    "DNA Codons",
    "Tarot Major Arcana",
    "Nakshatras"
};

/* ===== Sacred Number Concordances ===== */

#define CONCORDANCE_COUNT 13

static const concordance_t CONCORDANCES[CONCORDANCE_COUNT] = {
    {
        .number = 3,
        .description = "Trinity/tripartite structure",
        .system_count = 3,
        .systems = { "Kabbalah (3 pillars, 3 mother letters)",
                     "Hindu (3 gunas)",
                     "Buddhism (3 jewels)",
                     NULL, NULL, NULL }
    },
    {
        .number = 4,
        .description = "Quaternary cycles",
        .system_count = 4,
        .systems = { "Kabbalah (4 worlds)",
                     "Hindu (4 yugas)",
                     "Buddhism (4 noble truths)",
                     "Seasons (4 quarters)",
                     NULL, NULL }
    },
    {
        .number = 7,
        .description = "Septenary harmony",
        .system_count = 5,
        .systems = { "Classical Planets",
                     "Hebrew double letters",
                     "Weekdays",
                     "Chakras",
                     "Musical notes",
                     NULL }
    },
    {
        .number = 10,
        .description = "Decimal completeness",
        .system_count = 2,
        .systems = { "Kabbalah (Sefirot)",
                     "Chinese (Heavenly Stems)",
                     NULL, NULL, NULL, NULL }
    },
    {
        .number = 12,
        .description = "Dodecimal cycle",
        .system_count = 5,
        .systems = { "Zodiac signs",
                     "Hebrew simple letters",
                     "Chinese Earthly Branches",
                     "Months",
                     "Astrological Houses",
                     NULL }
    },
    {
        .number = 18,
        .description = "Sacred life number",
        .system_count = 3,
        .systems = { "Hindu (18 Gita chapters, 18 Mahabharata parvas)",
                     "Hebrew (chai = 18, gematria of life)",
                     "Buddhism (18 dhatus, elements of cognition)",
                     NULL, NULL, NULL }
    },
    {
        .number = 22,
        .description = "Alphabetic-symbolic bridge",
        .system_count = 3,
        .systems = { "Kabbalah paths",
                     "Hebrew letters",
                     "Tarot Major Arcana",
                     NULL, NULL, NULL }
    },
    {
        .number = 27,
        .description = "Lunar mansions",
        .system_count = 3,
        .systems = { "Nakshatras",
                     "I Ching trigram triples (3^3)",
                     "Zoroastrian lunar mansions (Indo-Iranian heritage)",
                     NULL, NULL, NULL }
    },
    {
        .number = 52,
        .description = "Calendar completeness",
        .system_count = 2,
        .systems = { "Qumran 364-day calendar (52 perfect weeks)",
                     "Maya Calendar Round (52 Haab years = 18,980 days)",
                     NULL, NULL, NULL, NULL }
    },
    {
        .number = 64,
        .description = "Binary combinatorics (2^6)",
        .system_count = 3,
        .systems = { "I Ching hexagrams",
                     "Human Design gates",
                     "DNA codons (4^3)",
                     NULL, NULL, NULL }
    },
    {
        .number = 108,
        .description = "Composite sacred number",
        .system_count = 3,
        .systems = { "Nakshatra padas (27x4)",
                     "Buddhist mala beads",
                     "Hindu sacred number",
                     NULL, NULL, NULL }
    },
    {
        .number = 231,
        .description = "Gates of creation",
        .system_count = 2,
        .systems = { "Kabbalah (231 gates = C(22,2) letter pairs, Sefer Yetzirah)",
                     "Tzolkin (Synchronotron sum 1..21 = 231)",
                     NULL, NULL, NULL, NULL }
    },
    {
        .number = 260,
        .description = "Galactic cycle",
        .system_count = 2,
        .systems = { "Tzolkin (13x20)",
                     "Human Design Rave mandala",
                     NULL, NULL, NULL, NULL }
    }
};

/* Sacred numbers in ascending order */
static const int SACRED_NUMBERS[CONCORDANCE_COUNT] = {
    3, 4, 7, 10, 12, 18, 22, 27, 52, 64, 108, 231, 260
};

/* ===== 7 Classical Planet Correspondences ===== */
/* Order: Sun, Moon, Mars, Mercury, Jupiter, Venus, Saturn
 * Hebrew letters: Sefer Yetzirah / Golden Dawn tradition
 * Chakra associations: traditional Hindu-Western synthesis */

#define PLANET_ROW_COUNT 7

static const planet_row_t PLANET_ROWS[PLANET_ROW_COUNT] = {
    { 0, "Sun",     "Resh",  "Sunday",    "Solar Plexus", 5 }, /* Tiferet */
    { 1, "Moon",    "Gimel", "Monday",    "Third Eye",    8 }, /* Yesod */
    { 2, "Mars",    "Pe",    "Tuesday",   "Root",         4 }, /* Gevurah */
    { 3, "Mercury", "Bet",   "Wednesday", "Throat",       7 }, /* Hod */
    { 4, "Jupiter", "Kaf",   "Thursday",  "Crown",        3 }, /* Chesed */
    { 5, "Venus",   "Dalet", "Friday",    "Heart",        6 }, /* Netzach */
    { 6, "Saturn",  "Tav",   "Saturday",  "Sacral",       2 }  /* Binah */
};

/* ===== 12 Zodiac Correspondences ===== */
/* Hebrew simple letters: Golden Dawn / Sefer Yetzirah tradition
 * Chinese Branches: approximate 12-fold mapping (both are 12-fold
 * divisions but from different traditions)
 * Tarot: Golden Dawn Major Arcana assignments */

#define ZODIAC_ROW_COUNT 12

static const zodiac_row_t ZODIAC_ROWS[ZODIAC_ROW_COUNT] = {
    {  0, "Aries",       "He",     "Rat (Zi)",      "Emperor"     },
    {  1, "Taurus",      "Vav",    "Ox (Chou)",     "Hierophant"  },
    {  2, "Gemini",      "Zayin",  "Tiger (Yin)",   "Lovers"      },
    {  3, "Cancer",      "Chet",   "Rabbit (Mao)",  "Chariot"     },
    {  4, "Leo",         "Tet",    "Dragon (Chen)",  "Strength"    },
    {  5, "Virgo",       "Yod",    "Snake (Si)",    "Hermit"      },
    {  6, "Libra",       "Lamed",  "Horse (Wu)",    "Justice"     },
    {  7, "Scorpio",     "Nun",    "Goat (Wei)",    "Death"       },
    {  8, "Sagittarius", "Samekh", "Monkey (Shen)", "Temperance"  },
    {  9, "Capricorn",   "Ayin",   "Rooster (You)", "Devil"       },
    { 10, "Aquarius",    "Tsade",  "Dog (Xu)",      "Star"        },
    { 11, "Pisces",      "Qof",    "Pig (Hai)",     "Moon"        }
};

/* ===== Sefirot <-> Human Design Centers ===== */
/* 10 Sefirot map to 9 HD Centers (some Sefirot share a center)
 * Order: Keter(0), Chokmah(1), Binah(2), Chesed(3), Gevurah(4),
 *        Tiferet(5), Netzach(6), Hod(7), Yesod(8), Malkuth(9) */

#define SEFIROT_HD_COUNT 10

static const char *SEFIRAH_TO_HD[SEFIROT_HD_COUNT] = {
    "Head",         /* 0 Keter    -> Head */
    "Ajna",         /* 1 Chokmah  -> Ajna */
    "Ajna",         /* 2 Binah    -> Ajna */
    "G Center",     /* 3 Chesed   -> G Center */
    "Heart/Will",   /* 4 Gevurah  -> Heart/Will */
    "G Center",     /* 5 Tiferet  -> G Center */
    "Sacral",       /* 6 Netzach  -> Sacral */
    "Spleen",       /* 7 Hod      -> Spleen */
    "Solar Plexus", /* 8 Yesod    -> Solar Plexus */
    "Root"          /* 9 Malkuth  -> Root */
};

/* HD Center -> primary Sefirah index
 * 9 HD Centers: Head(0), Ajna(1), Throat(2), G Center(3),
 *               Heart/Will(4), Sacral(5), Spleen(6), Solar Plexus(7), Root(8) */

#define HD_CENTER_COUNT 9

static const int HD_TO_SEFIRAH[HD_CENTER_COUNT] = {
    0,  /* Head         -> Keter (0) */
    1,  /* Ajna         -> Chokmah (1) (primary; Binah also maps here) */
    -1, /* Throat       -> no direct Sefirah (Da'at is hidden) */
    3,  /* G Center     -> Chesed (3) (primary; Tiferet also maps here) */
    4,  /* Heart/Will   -> Gevurah (4) */
    6,  /* Sacral       -> Netzach (6) */
    7,  /* Spleen       -> Hod (7) */
    8,  /* Solar Plexus -> Yesod (8) */
    9   /* Root         -> Malkuth (9) */
};

/* ===== Function Implementations ===== */

int structural_system_count(void)
{
    return SYS_COUNT;
}

const char *structural_system_name(system_id_t sys)
{
    if (sys < 0 || sys >= SYS_COUNT) {
        return "?";
    }
    return SYSTEM_NAMES[sys];
}

int structural_concordance_count(void)
{
    return CONCORDANCE_COUNT;
}

concordance_t structural_concordance_get(int index)
{
    if (index < 0 || index >= CONCORDANCE_COUNT) {
        concordance_t invalid = {
            .number = -1,
            .description = NULL,
            .system_count = 0,
            .systems = { NULL, NULL, NULL, NULL, NULL, NULL }
        };
        return invalid;
    }
    return CONCORDANCES[index];
}

concordance_t structural_concordance_for_number(int number)
{
    for (int i = 0; i < CONCORDANCE_COUNT; i++) {
        if (CONCORDANCES[i].number == number) {
            return CONCORDANCES[i];
        }
    }
    concordance_t invalid = {
        .number = -1,
        .description = NULL,
        .system_count = 0,
        .systems = { NULL, NULL, NULL, NULL, NULL, NULL }
    };
    return invalid;
}

planet_row_t structural_planet_row(int planet_index)
{
    if (planet_index < 0 || planet_index >= PLANET_ROW_COUNT) {
        planet_row_t invalid = {
            .planet_index = -1,
            .planet = "?",
            .hebrew_letter = "?",
            .weekday = "?",
            .chakra = "?",
            .sefirah_index = -1
        };
        return invalid;
    }
    return PLANET_ROWS[planet_index];
}

zodiac_row_t structural_zodiac_row(int sign_index)
{
    if (sign_index < 0 || sign_index >= ZODIAC_ROW_COUNT) {
        zodiac_row_t invalid = {
            .sign_index = -1,
            .zodiac = "?",
            .hebrew_letter = "?",
            .chinese_branch = "?",
            .tarot = "?"
        };
        return invalid;
    }
    return ZODIAC_ROWS[sign_index];
}

const char *structural_sefirah_to_hd_center(int sefirah_index)
{
    if (sefirah_index < 0 || sefirah_index >= SEFIROT_HD_COUNT) {
        return "?";
    }
    return SEFIRAH_TO_HD[sefirah_index];
}

int structural_hd_center_to_sefirah(int hd_center_index)
{
    if (hd_center_index < 0 || hd_center_index >= HD_CENTER_COUNT) {
        return -1;
    }
    return HD_TO_SEFIRAH[hd_center_index];
}

int structural_sacred_number_count(void)
{
    return CONCORDANCE_COUNT;
}

int structural_sacred_number(int index)
{
    if (index < 0 || index >= CONCORDANCE_COUNT) {
        return -1;
    }
    return SACRED_NUMBERS[index];
}

bool structural_appears_in_systems(int number, int min_systems)
{
    for (int i = 0; i < CONCORDANCE_COUNT; i++) {
        if (CONCORDANCES[i].number == number) {
            return CONCORDANCES[i].system_count >= min_systems;
        }
    }
    return false;
}
