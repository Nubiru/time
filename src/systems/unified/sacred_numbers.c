/* sacred_numbers.c — cross-system sacred number registry implementation.
 * Pure data + lookup. All data is static const. No malloc, no globals. */

#include "sacred_numbers.h"
#include <stddef.h>

/* Human-readable system names, indexed by knowledge_system_t */
static const char *const SYSTEM_NAMES[SYSTEM_COUNT] = {
    "Astronomy",
    "Astrology",
    "Tzolkin",
    "I Ching",
    "Chinese",
    "Human Design",
    "Hebrew",
    "Islamic",
    "Buddhist",
    "Hindu",
    "Kabbalah",
    "Geology",
    "Gregorian",
    "Music",
    "Biology",
    "Mathematics"
};

/* Registry of 20 sacred numbers, sorted ascending by value. */
static const sacred_number_t REGISTRY[] = {
    /* 3 */
    {
        .value = 3,
        .name = "Trinity",
        .significance = "Fundamental triadic division across traditions",
        .appearance_count = 5,
        .appearances = {
            { SYSTEM_ICHING,      "Trigram lines (yin/yang/changing)" },
            { SYSTEM_HINDU,       "Three Gunas (sattva, rajas, tamas)" },
            { SYSTEM_KABBALAH,    "Three Pillars of the Tree of Life" },
            { SYSTEM_HEBREW,      "Three Mother letters (Alef, Mem, Shin)" },
            { SYSTEM_MATHEMATICS, "Smallest odd prime, triangle" }
        }
    },
    /* 4 */
    {
        .value = 4,
        .name = "Quaternary",
        .significance = "Four-fold division of space and time",
        .appearance_count = 5,
        .appearances = {
            { SYSTEM_ASTROLOGY,   "Four elements (fire, earth, air, water)" },
            { SYSTEM_GREGORIAN,   "Four seasons" },
            { SYSTEM_KABBALAH,    "Four Worlds (Atziluth, Briah, Yetzirah, Assiah)" },
            { SYSTEM_HINDU,       "Four Yugas (Satya, Treta, Dvapara, Kali)" },
            { SYSTEM_BUDDHIST,    "Four Noble Truths" }
        }
    },
    /* 5 */
    {
        .value = 5,
        .name = "Pentad",
        .significance = "Five-fold elemental and sensory systems",
        .appearance_count = 4,
        .appearances = {
            { SYSTEM_HINDU,       "Five Panchanga limbs (tithi, vara, nakshatra, yoga, karana)" },
            { SYSTEM_CHINESE,     "Five elements / Wu Xing (wood, fire, earth, metal, water)" },
            { SYSTEM_ASTRONOMY,   "Five visible planets (Mercury-Saturn)" },
            { SYSTEM_TZOLKIN,     "Five castles of the Dreamspell" }
        }
    },
    /* 7 */
    {
        .value = 7,
        .name = "Heptad",
        .significance = "Week cycle, planetary rulers, chakra system",
        .appearance_count = 6,
        .appearances = {
            { SYSTEM_GREGORIAN,   "Seven days per week" },
            { SYSTEM_ASTRONOMY,   "Seven classical planets" },
            { SYSTEM_ASTROLOGY,   "Seven planetary rulers" },
            { SYSTEM_HINDU,       "Seven chakras" },
            { SYSTEM_HEBREW,      "Seven double letters; Shemitah (7-year) cycle" },
            { SYSTEM_MUSIC,       "Seven notes in diatonic scale" }
        }
    },
    /* 8 */
    {
        .value = 8,
        .name = "Octad",
        .significance = "Eight trigrams, eightfold paths",
        .appearance_count = 3,
        .appearances = {
            { SYSTEM_ICHING,      "Eight trigrams (Bagua)" },
            { SYSTEM_BUDDHIST,    "Noble Eightfold Path" },
            { SYSTEM_HINDU,       "Ashtanga (eight limbs of yoga)" }
        }
    },
    /* 9 */
    {
        .value = 9,
        .name = "Ennead",
        .significance = "Fu Xi pairing constant, completion, greatest single digit",
        .appearance_count = 4,
        .appearances = {
            { SYSTEM_ICHING,       "Fu Xi trigram opposite pairs all sum to 9" },
            { SYSTEM_CHINESE,      "Nine is Heaven's number; highest yang digit" },
            { SYSTEM_KABBALAH,     "Nine visible Sefirot (excluding Da'at)" },
            { SYSTEM_HUMAN_DESIGN, "Nine Centers of the Bodygraph" }
        }
    },
    /* 10 */
    {
        .value = 10,
        .name = "Decad",
        .significance = "Ten emanations, stems, digits",
        .appearance_count = 3,
        .appearances = {
            { SYSTEM_KABBALAH,    "Ten Sefirot on the Tree of Life" },
            { SYSTEM_CHINESE,     "Ten Heavenly Stems (Tiangan)" },
            { SYSTEM_MATHEMATICS, "Base-10 numeral system" }
        }
    },
    /* 12 */
    {
        .value = 12,
        .name = "Dodecad",
        .significance = "Zodiacal division, monthly cycle, tribal structure",
        .appearance_count = 6,
        .appearances = {
            { SYSTEM_ASTROLOGY,   "Twelve zodiac signs" },
            { SYSTEM_GREGORIAN,   "Twelve months per year" },
            { SYSTEM_HEBREW,      "Twelve tribes of Israel; twelve simple letters" },
            { SYSTEM_CHINESE,     "Twelve Earthly Branches (Dizhi)" },
            { SYSTEM_ASTRONOMY,   "Twelve hours of daylight (equinox)" },
            { SYSTEM_MUSIC,       "Twelve semitones in chromatic scale" }
        }
    },
    /* 13 */
    {
        .value = 13,
        .name = "Trecena",
        .significance = "Lunar months, Tzolkin tones, Baktun count",
        .appearance_count = 2,
        .appearances = {
            { SYSTEM_TZOLKIN,     "Thirteen tones (galactic tones of creation)" },
            { SYSTEM_ASTRONOMY,   "Approximately thirteen lunar months per year" }
        }
    },
    /* 19 */
    {
        .value = 19,
        .name = "Metonic",
        .significance = "Lunisolar synchronization cycle",
        .appearance_count = 2,
        .appearances = {
            { SYSTEM_ASTRONOMY,   "Metonic cycle: 19 years = 235 synodic months" },
            { SYSTEM_HEBREW,      "19-year cycle of the Hebrew calendar" }
        }
    },
    /* 20 */
    {
        .value = 20,
        .name = "Uinal",
        .significance = "Mayan vigesimal base, Tzolkin seals",
        .appearance_count = 2,
        .appearances = {
            { SYSTEM_TZOLKIN,     "Twenty solar seals (day signs)" },
            { SYSTEM_MATHEMATICS, "Mayan vigesimal (base-20) numeral system" }
        }
    },
    /* 22 */
    {
        .value = 22,
        .name = "Paths",
        .significance = "Hebrew letters, Kabbalistic paths, Tarot arcana",
        .appearance_count = 3,
        .appearances = {
            { SYSTEM_HEBREW,      "Twenty-two letters of the Hebrew alphabet" },
            { SYSTEM_KABBALAH,    "Twenty-two paths on the Tree of Life" },
            { SYSTEM_MATHEMATICS, "Twenty-two Major Arcana (Tarot)" }
        }
    },
    /* 27 */
    {
        .value = 27,
        .name = "Nakshatras",
        .significance = "Lunar mansions, sidereal month divisions",
        .appearance_count = 2,
        .appearances = {
            { SYSTEM_HINDU,       "Twenty-seven Nakshatras (lunar mansions)" },
            { SYSTEM_ASTRONOMY,   "Approximate sidereal month in days (27.3)" }
        }
    },
    /* 30 */
    {
        .value = 30,
        .name = "Lunar Month",
        .significance = "Approximate synodic month, tithis",
        .appearance_count = 3,
        .appearances = {
            { SYSTEM_HINDU,       "Thirty tithis per lunar month" },
            { SYSTEM_ASTRONOMY,   "Approximate synodic month length (29.5 days)" },
            { SYSTEM_ISLAMIC,     "Thirty-day months in Hijri calendar" }
        }
    },
    /* 36 */
    {
        .value = 36,
        .name = "Decans",
        .significance = "Decanal divisions, Human Design channels",
        .appearance_count = 2,
        .appearances = {
            { SYSTEM_HUMAN_DESIGN, "Thirty-six channels connecting centers" },
            { SYSTEM_ASTROLOGY,    "Thirty-six Egyptian decans (10-degree arcs)" }
        }
    },
    /* 64 */
    {
        .value = 64,
        .name = "Hexagrams",
        .significance = "Binary completeness: 2^6 across I Ching, HD, genetics",
        .appearance_count = 3,
        .appearances = {
            { SYSTEM_ICHING,       "Sixty-four hexagrams (King Wen sequence)" },
            { SYSTEM_HUMAN_DESIGN, "Sixty-four gates (mapped from hexagrams)" },
            { SYSTEM_BIOLOGY,      "Sixty-four DNA codons" }
        }
    },
    /* 72 */
    {
        .value = 72,
        .name = "Precession Degree",
        .significance = "Years per degree of axial precession",
        .appearance_count = 2,
        .appearances = {
            { SYSTEM_ASTRONOMY,   "72 years per degree of precession (25,920/360)" },
            { SYSTEM_KABBALAH,    "72 names of God (Shem HaMephorash)" }
        }
    },
    /* 108 */
    {
        .value = 108,
        .name = "Mala",
        .significance = "Sacred bead count, Nakshatra padas product (27x4)",
        .appearance_count = 3,
        .appearances = {
            { SYSTEM_HINDU,    "108 Nakshatra padas (27 x 4); mala bead count" },
            { SYSTEM_BUDDHIST, "108 mala beads; 108 defilements" },
            { SYSTEM_MATHEMATICS, "108 = 12 x 9 = 27 x 4" }
        }
    },
    /* 260 */
    {
        .value = 260,
        .name = "Tzolkin",
        .significance = "Sacred calendar cycle, near human gestation",
        .appearance_count = 2,
        .appearances = {
            { SYSTEM_TZOLKIN,  "260-day Tzolkin sacred calendar (13 x 20)" },
            { SYSTEM_BIOLOGY,  "Approximate human gestation period in days" }
        }
    },
    /* 360 */
    {
        .value = 360,
        .name = "Circle",
        .significance = "Degrees in circle, Tun days, approximate year",
        .appearance_count = 3,
        .appearances = {
            { SYSTEM_MATHEMATICS, "360 degrees in a circle" },
            { SYSTEM_TZOLKIN,     "360-day Tun (Mayan Long Count year)" },
            { SYSTEM_ASTRONOMY,   "Approximate days per solar year" }
        }
    },
    /* 432 */
    {
        .value = 432,
        .name = "Kali Yuga Unit",
        .significance = "Yuga base unit, precession harmonic",
        .appearance_count = 2,
        .appearances = {
            { SYSTEM_HINDU,    "Kali Yuga = 432,000 years (432 x 1000)" },
            { SYSTEM_ASTRONOMY, "Precession / 60 = 432 (25,920 / 60)" }
        }
    }
};

static const int REGISTRY_COUNT = (int)(sizeof(REGISTRY) / sizeof(REGISTRY[0]));

/* ===== Public API ===== */

int sacred_number_count(void)
{
    return REGISTRY_COUNT;
}

sacred_number_t sacred_number_get(int value)
{
    for (int i = 0; i < REGISTRY_COUNT; i++) {
        if (REGISTRY[i].value == value) {
            return REGISTRY[i];
        }
    }
    sacred_number_t empty = { .value = 0, .name = NULL, .significance = NULL,
                              .appearance_count = 0, .appearances = {{0}} };
    return empty;
}

sacred_number_t sacred_number_by_index(int index)
{
    if (index < 0 || index >= REGISTRY_COUNT) {
        sacred_number_t empty = { .value = 0, .name = NULL, .significance = NULL,
                                  .appearance_count = 0, .appearances = {{0}} };
        return empty;
    }
    return REGISTRY[index];
}

int sacred_number_system_count(int value)
{
    sacred_number_t n = sacred_number_get(value);
    return n.appearance_count;
}

bool sacred_number_in_system(int value, knowledge_system_t system)
{
    sacred_number_t n = sacred_number_get(value);
    if (n.value == 0 && value != 0) {
        return false;
    }
    if (n.name == NULL) {
        return false;
    }
    for (int i = 0; i < n.appearance_count; i++) {
        if (n.appearances[i].system == system) {
            return true;
        }
    }
    return false;
}

const char *sacred_number_system_name(knowledge_system_t system)
{
    if ((int)system < 0 || (int)system >= SYSTEM_COUNT) {
        return "?";
    }
    return SYSTEM_NAMES[(int)system];
}

int sacred_number_find_by_system(knowledge_system_t system, int *out_indices, int max_results)
{
    if (out_indices == NULL || max_results <= 0) {
        return 0;
    }
    int found = 0;
    for (int i = 0; i < REGISTRY_COUNT && found < max_results; i++) {
        for (int j = 0; j < REGISTRY[i].appearance_count; j++) {
            if (REGISTRY[i].appearances[j].system == system) {
                out_indices[found] = i;
                found++;
                break;
            }
        }
    }
    return found;
}

int sacred_number_factor_of(int value)
{
    if (value <= 0) {
        return 0;
    }

    /* Pass 1: Check if value IS a sacred number (exact match) */
    for (int i = 0; i < REGISTRY_COUNT; i++) {
        if (REGISTRY[i].value == value) {
            return value;
        }
    }

    /* Pass 2: Check if a sacred number is an exact factor of value
     * (i.e., value is a multiple of a sacred number).
     * Smallest sacred numbers checked first (registry is sorted ascending). */
    for (int i = 0; i < REGISTRY_COUNT; i++) {
        if (REGISTRY[i].value > 0 && value % REGISTRY[i].value == 0) {
            return REGISTRY[i].value;
        }
    }

    /* Pass 3: Check if value is an exact factor of a sacred number
     * (i.e., a sacred number is a multiple of value).
     * Return the smallest such sacred number. */
    for (int i = 0; i < REGISTRY_COUNT; i++) {
        if (value > 0 && REGISTRY[i].value % value == 0) {
            return REGISTRY[i].value;
        }
    }

    return 0;
}
