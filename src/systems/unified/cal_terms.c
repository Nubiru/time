/* cal_terms.c -- Calendar Term Database.
 *
 * 115 terms spanning 9 calendar/knowledge systems: Gregorian, Tzolkin,
 * Hebrew, Islamic, Chinese, Hindu, Astrology (months, seals, tones,
 * stems, branches, signs).
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#include "cal_terms.h"

#include <stddef.h>

/* ---------- term database ---------- */

static const cal_term_t TERMS[] = {

    /* ===== GREGORIAN MONTHS (12, indices 0-11) ===== */
    {  0, CAL_SYS_GREGORIAN, CAL_KIND_MONTH,  1, "January",   "January",   "First month",    "Latn" },
    {  1, CAL_SYS_GREGORIAN, CAL_KIND_MONTH,  2, "February",  "February",  "Second month",   "Latn" },
    {  2, CAL_SYS_GREGORIAN, CAL_KIND_MONTH,  3, "March",     "March",     "Third month",    "Latn" },
    {  3, CAL_SYS_GREGORIAN, CAL_KIND_MONTH,  4, "April",     "April",     "Fourth month",   "Latn" },
    {  4, CAL_SYS_GREGORIAN, CAL_KIND_MONTH,  5, "May",       "May",       "Fifth month",    "Latn" },
    {  5, CAL_SYS_GREGORIAN, CAL_KIND_MONTH,  6, "June",      "June",      "Sixth month",    "Latn" },
    {  6, CAL_SYS_GREGORIAN, CAL_KIND_MONTH,  7, "July",      "July",      "Seventh month",  "Latn" },
    {  7, CAL_SYS_GREGORIAN, CAL_KIND_MONTH,  8, "August",    "August",    "Eighth month",   "Latn" },
    {  8, CAL_SYS_GREGORIAN, CAL_KIND_MONTH,  9, "September", "September", "Ninth month",    "Latn" },
    {  9, CAL_SYS_GREGORIAN, CAL_KIND_MONTH, 10, "October",   "October",   "Tenth month",    "Latn" },
    { 10, CAL_SYS_GREGORIAN, CAL_KIND_MONTH, 11, "November",  "November",  "Eleventh month", "Latn" },
    { 11, CAL_SYS_GREGORIAN, CAL_KIND_MONTH, 12, "December",  "December",  "Twelfth month",  "Latn" },

    /* ===== TZOLKIN SEALS (20, indices 12-31) ===== */
    { 12, CAL_SYS_TZOLKIN, CAL_KIND_DAY,  1, "Imix",     "Imix",     "Dragon",       "Latn" },
    { 13, CAL_SYS_TZOLKIN, CAL_KIND_DAY,  2, "Ik",       "Ik",       "Wind",         "Latn" },
    { 14, CAL_SYS_TZOLKIN, CAL_KIND_DAY,  3, "Akbal",    "Akbal",    "Night",        "Latn" },
    { 15, CAL_SYS_TZOLKIN, CAL_KIND_DAY,  4, "Kan",      "Kan",      "Seed",         "Latn" },
    { 16, CAL_SYS_TZOLKIN, CAL_KIND_DAY,  5, "Chicchan", "Chicchan", "Serpent",       "Latn" },
    { 17, CAL_SYS_TZOLKIN, CAL_KIND_DAY,  6, "Cimi",     "Cimi",     "Worldbridger", "Latn" },
    { 18, CAL_SYS_TZOLKIN, CAL_KIND_DAY,  7, "Manik",    "Manik",    "Hand",         "Latn" },
    { 19, CAL_SYS_TZOLKIN, CAL_KIND_DAY,  8, "Lamat",    "Lamat",    "Star",         "Latn" },
    { 20, CAL_SYS_TZOLKIN, CAL_KIND_DAY,  9, "Muluc",    "Muluc",    "Moon",         "Latn" },
    { 21, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 10, "Oc",       "Oc",       "Dog",          "Latn" },
    { 22, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 11, "Chuen",    "Chuen",    "Monkey",       "Latn" },
    { 23, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 12, "Eb",       "Eb",       "Human",        "Latn" },
    { 24, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 13, "Ben",      "Ben",      "Skywalker",    "Latn" },
    { 25, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 14, "Ix",       "Ix",       "Wizard",       "Latn" },
    { 26, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 15, "Men",      "Men",      "Eagle",        "Latn" },
    { 27, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 16, "Cib",      "Cib",      "Warrior",      "Latn" },
    { 28, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 17, "Caban",    "Caban",    "Earth",        "Latn" },
    { 29, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 18, "Etznab",   "Etznab",   "Mirror",       "Latn" },
    { 30, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 19, "Cauac",    "Cauac",    "Storm",        "Latn" },
    { 31, CAL_SYS_TZOLKIN, CAL_KIND_DAY, 20, "Ahau",     "Ahau",     "Sun",          "Latn" },

    /* ===== TZOLKIN TONES (13, indices 32-44) ===== */
    { 32, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,  1, "Magnetic",      "Magnetic",      "Unity",          "Latn" },
    { 33, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,  2, "Lunar",         "Lunar",         "Challenge",      "Latn" },
    { 34, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,  3, "Electric",      "Electric",      "Service",        "Latn" },
    { 35, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,  4, "Self-Existing", "Self-Existing", "Form",           "Latn" },
    { 36, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,  5, "Overtone",      "Overtone",      "Radiance",       "Latn" },
    { 37, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,  6, "Rhythmic",      "Rhythmic",      "Equality",       "Latn" },
    { 38, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,  7, "Resonant",      "Resonant",      "Attunement",     "Latn" },
    { 39, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,  8, "Galactic",      "Galactic",      "Integrity",      "Latn" },
    { 40, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE,  9, "Solar",         "Solar",         "Intention",      "Latn" },
    { 41, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE, 10, "Planetary",     "Planetary",     "Manifestation",  "Latn" },
    { 42, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE, 11, "Spectral",      "Spectral",      "Liberation",     "Latn" },
    { 43, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE, 12, "Crystal",       "Crystal",       "Cooperation",    "Latn" },
    { 44, CAL_SYS_TZOLKIN, CAL_KIND_CYCLE, 13, "Cosmic",        "Cosmic",        "Presence",       "Latn" },

    /* ===== HEBREW MONTHS (12, indices 45-56) ===== */
    { 45, CAL_SYS_HEBREW, CAL_KIND_MONTH,  1, "Tishrei",  "Tishrei",  "Beginning",    "Hebr" },
    { 46, CAL_SYS_HEBREW, CAL_KIND_MONTH,  2, "Cheshvan", "Cheshvan", "Eighth month", "Hebr" },
    { 47, CAL_SYS_HEBREW, CAL_KIND_MONTH,  3, "Kislev",   "Kislev",   "Trust",        "Hebr" },
    { 48, CAL_SYS_HEBREW, CAL_KIND_MONTH,  4, "Tevet",    "Tevet",    "Goodness",     "Hebr" },
    { 49, CAL_SYS_HEBREW, CAL_KIND_MONTH,  5, "Shevat",   "Shevat",   "Staff/Rod",    "Hebr" },
    { 50, CAL_SYS_HEBREW, CAL_KIND_MONTH,  6, "Adar",     "Adar",     "Strength",     "Hebr" },
    { 51, CAL_SYS_HEBREW, CAL_KIND_MONTH,  7, "Nisan",    "Nisan",    "Miracles",     "Hebr" },
    { 52, CAL_SYS_HEBREW, CAL_KIND_MONTH,  8, "Iyar",     "Iyar",     "Light",        "Hebr" },
    { 53, CAL_SYS_HEBREW, CAL_KIND_MONTH,  9, "Sivan",    "Sivan",    "Season",       "Hebr" },
    { 54, CAL_SYS_HEBREW, CAL_KIND_MONTH, 10, "Tammuz",   "Tammuz",   "Warmth",       "Hebr" },
    { 55, CAL_SYS_HEBREW, CAL_KIND_MONTH, 11, "Av",       "Av",       "Father",       "Hebr" },
    { 56, CAL_SYS_HEBREW, CAL_KIND_MONTH, 12, "Elul",     "Elul",     "Search",       "Hebr" },

    /* ===== ISLAMIC MONTHS (12, indices 57-68) ===== */
    { 57, CAL_SYS_ISLAMIC, CAL_KIND_MONTH,  1, "Muharram",       "Muharram",       "Forbidden",       "Arab" },
    { 58, CAL_SYS_ISLAMIC, CAL_KIND_MONTH,  2, "Safar",          "Safar",          "Void",            "Arab" },
    { 59, CAL_SYS_ISLAMIC, CAL_KIND_MONTH,  3, "Rabi al-Awwal",  "Rabi al-Awwal",  "First Spring",    "Arab" },
    { 60, CAL_SYS_ISLAMIC, CAL_KIND_MONTH,  4, "Rabi al-Thani",  "Rabi al-Thani",  "Second Spring",   "Arab" },
    { 61, CAL_SYS_ISLAMIC, CAL_KIND_MONTH,  5, "Jumada al-Ula",  "Jumada al-Ula",  "First Freeze",    "Arab" },
    { 62, CAL_SYS_ISLAMIC, CAL_KIND_MONTH,  6, "Jumada al-Thani","Jumada al-Thani","Second Freeze",   "Arab" },
    { 63, CAL_SYS_ISLAMIC, CAL_KIND_MONTH,  7, "Rajab",          "Rajab",          "Respect",         "Arab" },
    { 64, CAL_SYS_ISLAMIC, CAL_KIND_MONTH,  8, "Shaban",         "Shaban",         "Scattered",       "Arab" },
    { 65, CAL_SYS_ISLAMIC, CAL_KIND_MONTH,  9, "Ramadan",        "Ramadan",        "Burning Heat",    "Arab" },
    { 66, CAL_SYS_ISLAMIC, CAL_KIND_MONTH, 10, "Shawwal",        "Shawwal",        "Raised",          "Arab" },
    { 67, CAL_SYS_ISLAMIC, CAL_KIND_MONTH, 11, "Dhu al-Qadah",   "Dhu al-Qadah",   "The Truce",       "Arab" },
    { 68, CAL_SYS_ISLAMIC, CAL_KIND_MONTH, 12, "Dhu al-Hijjah",  "Dhu al-Hijjah",  "The Pilgrimage",  "Arab" },

    /* ===== CHINESE HEAVENLY STEMS (10, indices 69-78) ===== */
    { 69, CAL_SYS_CHINESE, CAL_KIND_ELEMENT,  1, "Jia",  "Jia",  "Wood Yang",  "Hans" },
    { 70, CAL_SYS_CHINESE, CAL_KIND_ELEMENT,  2, "Yi",   "Yi",   "Wood Yin",   "Hans" },
    { 71, CAL_SYS_CHINESE, CAL_KIND_ELEMENT,  3, "Bing", "Bing", "Fire Yang",  "Hans" },
    { 72, CAL_SYS_CHINESE, CAL_KIND_ELEMENT,  4, "Ding", "Ding", "Fire Yin",   "Hans" },
    { 73, CAL_SYS_CHINESE, CAL_KIND_ELEMENT,  5, "Wu",   "Wu",   "Earth Yang", "Hans" },
    { 74, CAL_SYS_CHINESE, CAL_KIND_ELEMENT,  6, "Ji",   "Ji",   "Earth Yin",  "Hans" },
    { 75, CAL_SYS_CHINESE, CAL_KIND_ELEMENT,  7, "Geng", "Geng", "Metal Yang", "Hans" },
    { 76, CAL_SYS_CHINESE, CAL_KIND_ELEMENT,  8, "Xin",  "Xin",  "Metal Yin",  "Hans" },
    { 77, CAL_SYS_CHINESE, CAL_KIND_ELEMENT,  9, "Ren",  "Ren",  "Water Yang", "Hans" },
    { 78, CAL_SYS_CHINESE, CAL_KIND_ELEMENT, 10, "Gui",  "Gui",  "Water Yin",  "Hans" },

    /* ===== CHINESE EARTHLY BRANCHES (12, indices 79-90) ===== */
    { 79, CAL_SYS_CHINESE, CAL_KIND_SIGN,  1, "Zi",   "Zi",   "Rat",     "Hans" },
    { 80, CAL_SYS_CHINESE, CAL_KIND_SIGN,  2, "Chou", "Chou", "Ox",      "Hans" },
    { 81, CAL_SYS_CHINESE, CAL_KIND_SIGN,  3, "Yin",  "Yin",  "Tiger",   "Hans" },
    { 82, CAL_SYS_CHINESE, CAL_KIND_SIGN,  4, "Mao",  "Mao",  "Rabbit",  "Hans" },
    { 83, CAL_SYS_CHINESE, CAL_KIND_SIGN,  5, "Chen", "Chen", "Dragon",  "Hans" },
    { 84, CAL_SYS_CHINESE, CAL_KIND_SIGN,  6, "Si",   "Si",   "Snake",   "Hans" },
    { 85, CAL_SYS_CHINESE, CAL_KIND_SIGN,  7, "Wu",   "Wu",   "Horse",   "Hans" },
    { 86, CAL_SYS_CHINESE, CAL_KIND_SIGN,  8, "Wei",  "Wei",  "Goat",    "Hans" },
    { 87, CAL_SYS_CHINESE, CAL_KIND_SIGN,  9, "Shen", "Shen", "Monkey",  "Hans" },
    { 88, CAL_SYS_CHINESE, CAL_KIND_SIGN, 10, "You",  "You",  "Rooster", "Hans" },
    { 89, CAL_SYS_CHINESE, CAL_KIND_SIGN, 11, "Xu",   "Xu",   "Dog",     "Hans" },
    { 90, CAL_SYS_CHINESE, CAL_KIND_SIGN, 12, "Hai",  "Hai",  "Pig",     "Hans" },

    /* ===== HINDU MONTHS (12, indices 91-102) ===== */
    { 91,  CAL_SYS_HINDU, CAL_KIND_MONTH,  1, "Chaitra",      "Chaitra",      "Bright",          "Deva" },
    { 92,  CAL_SYS_HINDU, CAL_KIND_MONTH,  2, "Vaishakha",    "Vaishakha",    "Branched",        "Deva" },
    { 93,  CAL_SYS_HINDU, CAL_KIND_MONTH,  3, "Jyeshtha",     "Jyeshtha",     "Eldest",          "Deva" },
    { 94,  CAL_SYS_HINDU, CAL_KIND_MONTH,  4, "Ashadha",      "Ashadha",      "Achievement",     "Deva" },
    { 95,  CAL_SYS_HINDU, CAL_KIND_MONTH,  5, "Shravana",     "Shravana",     "Hearing",         "Deva" },
    { 96,  CAL_SYS_HINDU, CAL_KIND_MONTH,  6, "Bhadrapada",   "Bhadrapada",   "Auspicious Feet", "Deva" },
    { 97,  CAL_SYS_HINDU, CAL_KIND_MONTH,  7, "Ashvina",      "Ashvina",      "Horsemen",        "Deva" },
    { 98,  CAL_SYS_HINDU, CAL_KIND_MONTH,  8, "Kartika",      "Kartika",      "Pleiades",        "Deva" },
    { 99,  CAL_SYS_HINDU, CAL_KIND_MONTH,  9, "Margashirsha",  "Margashirsha",  "Deer Head",       "Deva" },
    { 100, CAL_SYS_HINDU, CAL_KIND_MONTH, 10, "Pausha",       "Pausha",       "Nourishment",     "Deva" },
    { 101, CAL_SYS_HINDU, CAL_KIND_MONTH, 11, "Magha",        "Magha",        "Gift",            "Deva" },
    { 102, CAL_SYS_HINDU, CAL_KIND_MONTH, 12, "Phalguna",     "Phalguna",     "Reddish",         "Deva" },

    /* ===== ASTROLOGY SIGNS (12, indices 103-114) ===== */
    { 103, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN,  1, "Aries",       "Aries",       "The Ram",          "Latn" },
    { 104, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN,  2, "Taurus",      "Taurus",      "The Bull",         "Latn" },
    { 105, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN,  3, "Gemini",      "Gemini",      "The Twins",        "Latn" },
    { 106, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN,  4, "Cancer",      "Cancer",      "The Crab",         "Latn" },
    { 107, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN,  5, "Leo",         "Leo",         "The Lion",         "Latn" },
    { 108, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN,  6, "Virgo",       "Virgo",       "The Maiden",       "Latn" },
    { 109, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN,  7, "Libra",       "Libra",       "The Scales",       "Latn" },
    { 110, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN,  8, "Scorpio",     "Scorpio",     "The Scorpion",     "Latn" },
    { 111, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN,  9, "Sagittarius", "Sagittarius", "The Archer",       "Latn" },
    { 112, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN, 10, "Capricorn",   "Capricorn",   "The Sea-Goat",     "Latn" },
    { 113, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN, 11, "Aquarius",    "Aquarius",    "The Water-Bearer", "Latn" },
    { 114, CAL_SYS_ASTROLOGY, CAL_KIND_SIGN, 12, "Pisces",      "Pisces",      "The Fish",         "Latn" },
};

static const int TERM_COUNT = sizeof(TERMS) / sizeof(TERMS[0]);

/* ---------- name tables ---------- */

static const char *SYSTEM_NAMES[] = {
    "Gregorian",
    "Tzolkin",
    "Hebrew",
    "Islamic",
    "Chinese",
    "Hindu",
    "Buddhist",
    "I Ching",
    "Astrology",
    "Human Design",
    "Kabbalah",
    "Geology",
    "Celtic",
    "Coptic",
    "Persian",
    "Japanese"
};

static const char *KIND_NAMES[] = {
    "Month",
    "Day",
    "Cycle",
    "Element",
    "Sign",
    "Festival"
};

/* ---------- sentinel ---------- */

static cal_term_t cal_term_invalid(void)
{
    cal_term_t invalid;
    invalid.id = -1;
    invalid.system = CAL_SYS_GREGORIAN;
    invalid.kind = CAL_KIND_MONTH;
    invalid.index = 0;
    invalid.native = NULL;
    invalid.transliterated = NULL;
    invalid.translated = NULL;
    invalid.script = NULL;
    return invalid;
}

/* ---------- public API ---------- */

int cal_term_count(void)
{
    return TERM_COUNT;
}

cal_term_t cal_term_get(int index)
{
    if (index < 0 || index >= TERM_COUNT) {
        return cal_term_invalid();
    }
    return TERMS[index];
}

int cal_terms_by_system(cal_system_t system, int *results, int max_results)
{
    if (results == NULL || max_results <= 0) return 0;
    if ((int)system < 0 || system >= CAL_SYS_COUNT) return 0;
    int count = 0;
    for (int i = 0; i < TERM_COUNT && count < max_results; i++) {
        if (TERMS[i].system == system) {
            results[count++] = i;
        }
    }
    return count;
}

int cal_terms_by_kind(cal_kind_t kind, int *results, int max_results)
{
    if (results == NULL || max_results <= 0) return 0;
    if ((int)kind < 0 || kind >= CAL_KIND_COUNT) return 0;
    int count = 0;
    for (int i = 0; i < TERM_COUNT && count < max_results; i++) {
        if (TERMS[i].kind == kind) {
            results[count++] = i;
        }
    }
    return count;
}

int cal_terms_by_system_kind(cal_system_t system, cal_kind_t kind,
                              int *results, int max_results)
{
    if (results == NULL || max_results <= 0) return 0;
    if ((int)system < 0 || system >= CAL_SYS_COUNT) return 0;
    if ((int)kind < 0 || kind >= CAL_KIND_COUNT) return 0;
    int count = 0;
    for (int i = 0; i < TERM_COUNT && count < max_results; i++) {
        if (TERMS[i].system == system && TERMS[i].kind == kind) {
            results[count++] = i;
        }
    }
    return count;
}

cal_term_t cal_term_lookup(cal_system_t system, cal_kind_t kind, int position)
{
    if ((int)system < 0 || system >= CAL_SYS_COUNT) return cal_term_invalid();
    if ((int)kind < 0 || kind >= CAL_KIND_COUNT) return cal_term_invalid();
    for (int i = 0; i < TERM_COUNT; i++) {
        if (TERMS[i].system == system &&
            TERMS[i].kind == kind &&
            TERMS[i].index == position) {
            return TERMS[i];
        }
    }
    return cal_term_invalid();
}

const char *cal_term_system_name(cal_system_t system)
{
    if ((int)system < 0 || system >= CAL_SYS_COUNT) return "Unknown";
    return SYSTEM_NAMES[system];
}

const char *cal_kind_name(cal_kind_t kind)
{
    if ((int)kind < 0 || kind >= CAL_KIND_COUNT) return "Unknown";
    return KIND_NAMES[kind];
}
