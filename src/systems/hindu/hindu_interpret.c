/* hindu_interpret.c -- Hindu tithi interpretation data
 *
 * All functions are pure (no globals, no malloc, no side effects).
 * Tithi data from traditional Hindu Panchanga sources. */

#include "hindu_interpret.h"
#include <stdio.h>
#include <string.h>

#define TITHI_COUNT 30

/* Full tithi names (paksha prefix + base name) for display */
static const char *const TITHI_FULL_NAMES[TITHI_COUNT] = {
    "Shukla Pratipada",    "Shukla Dvitiya",      "Shukla Tritiya",
    "Shukla Chaturthi",    "Shukla Panchami",     "Shukla Shashthi",
    "Shukla Saptami",      "Shukla Ashtami",      "Shukla Navami",
    "Shukla Dashami",      "Shukla Ekadashi",     "Shukla Dvadashi",
    "Shukla Trayodashi",   "Shukla Chaturdashi",  "Purnima",
    "Krishna Pratipada",   "Krishna Dvitiya",     "Krishna Tritiya",
    "Krishna Chaturthi",   "Krishna Panchami",    "Krishna Shashthi",
    "Krishna Saptami",     "Krishna Ashtami",     "Krishna Navami",
    "Krishna Dashami",     "Krishna Ekadashi",    "Krishna Dvadashi",
    "Krishna Trayodashi",  "Krishna Chaturdashi", "Amavasya"
};

static const hdi_tithi_t TITHIS[TITHI_COUNT] = {
    /*  1 */ {  1, "Agni",      "Auspicious",       "Beginning new ventures",
               "First day of the bright fortnight -- new beginnings" },
    /*  2 */ {  2, "Brahma",    "Auspicious",       "Travel, marriage",
               "Day of creativity and union" },
    /*  3 */ {  3, "Gauri",     "Very Auspicious",  "Rituals, celebrations",
               "Akshaya Tritiya -- prosperity that never diminishes" },
    /*  4 */ {  4, "Ganesha",   "Neutral",          "Warfare, aggression",
               "Day of obstacles and their removal" },
    /*  5 */ {  5, "Nagas",     "Auspicious",       "Learning, education",
               "Day of knowledge -- Vasant Panchami" },
    /*  6 */ {  6, "Kartikeya", "Auspicious",       "Healing, vitality",
               "Day of divine youth and energy" },
    /*  7 */ {  7, "Surya",     "Auspicious",       "Worship, travel",
               "Day of the Sun -- illumination" },
    /*  8 */ {  8, "Shiva",     "Neutral",          "Fasting, contemplation",
               "Day of transformation -- neither good nor ill" },
    /*  9 */ {  9, "Durga",     "Auspicious",       "Courage, determination",
               "Day of the fierce goddess" },
    /* 10 */ { 10, "Yama",      "Neutral",          "Justice, duty",
               "Day of dharmic action" },
    /* 11 */ { 11, "Vishnu",    "Very Auspicious",  "Fasting, devotion",
               "Day of spiritual discipline -- Ekadashi fast" },
    /* 12 */ { 12, "Vishnu",    "Auspicious",       "Charity, pilgrimage",
               "Day of generosity" },
    /* 13 */ { 13, "Kamadeva",  "Auspicious",       "Love, arts, devotion",
               "Day of desire and beauty" },
    /* 14 */ { 14, "Shiva",     "Inauspicious",     "Remedial rituals",
               "Day before fullness -- tension and preparation" },
    /* 15 */ { 15, "Soma",      "Very Auspicious",  "Worship, celebration",
               "Full Moon -- complete illumination" },
    /* 16 */ { 16, "Agni",      "Neutral",          "Rest after fullness",
               "First day of waning -- the turn inward begins" },
    /* 17 */ { 17, "Brahma",    "Neutral",          "Domestic work",
               "Day of gentle decrease" },
    /* 18 */ { 18, "Gauri",     "Auspicious",       "Rituals, remembrance",
               "Day of enduring grace" },
    /* 19 */ { 19, "Ganesha",   "Inauspicious",     "Avoid new ventures",
               "Day of hidden obstacles" },
    /* 20 */ { 20, "Nagas",     "Neutral",          "Study, reflection",
               "Day of inner knowledge" },
    /* 21 */ { 21, "Kartikeya", "Neutral",          "Healing practices",
               "Day of quiet strength" },
    /* 22 */ { 22, "Surya",     "Neutral",          "Routine work",
               "Day of fading light" },
    /* 23 */ { 23, "Shiva",     "Inauspicious",     "Fasting, austerity",
               "Krishna Ashtami -- Janmashtami (Krishna's birthday)" },
    /* 24 */ { 24, "Durga",     "Neutral",          "Protective rituals",
               "Day of fierce protection" },
    /* 25 */ { 25, "Yama",      "Neutral",          "Justice, settling accounts",
               "Day of reckoning" },
    /* 26 */ { 26, "Vishnu",    "Very Auspicious",  "Fasting, devotion",
               "Day of spiritual discipline -- Ekadashi fast" },
    /* 27 */ { 27, "Vishnu",    "Auspicious",       "Charity",
               "Day of release and giving" },
    /* 28 */ { 28, "Kamadeva",  "Neutral",          "Arts, creativity",
               "Day of subtle beauty" },
    /* 29 */ { 29, "Shiva",     "Inauspicious",     "Maha Shivaratri (great night of Shiva)",
               "Night before darkness -- dissolution" },
    /* 30 */ { 30, "Pitris",    "Neutral",          "Ancestor worship, new beginnings",
               "New Moon -- the still point before rebirth" }
};

hdi_tithi_t hdi_tithi_data(int tithi)
{
    if (tithi < 1 || tithi > TITHI_COUNT) {
        hdi_tithi_t invalid = { -1, "?", "?", "?", "?" };
        return invalid;
    }
    return TITHIS[tithi - 1];
}

int hdi_tithi_count(void)
{
    return TITHI_COUNT;
}

hindu_interp_t hdi_interpret(int tithi, const char *nakshatra_name,
                             const char *yoga_name)
{
    hindu_interp_t result;
    memset(&result, 0, sizeof(result));

    hdi_tithi_t t = hdi_tithi_data(tithi);

    if (t.number < 0) {
        /* Invalid tithi */
        snprintf(result.glyph, sizeof(result.glyph), "?");
        snprintf(result.glance, sizeof(result.glance), "? -- ? . ? . ?");
        snprintf(result.detail, sizeof(result.detail), "?");
        return result;
    }

    /* T0: glyph — "S{n}" for Shukla (1-15), "K{n}" for Krishna (16-30) */
    if (tithi <= 15) {
        snprintf(result.glyph, sizeof(result.glyph), "S%d", tithi);
    } else {
        snprintf(result.glyph, sizeof(result.glyph), "K%d", tithi - 15);
    }

    /* Get full tithi name */
    const char *full_name = TITHI_FULL_NAMES[tithi - 1];

    /* T1: glance — "{tithi_name} -- {deity} . {quality} . {activity}" */
    snprintf(result.glance, sizeof(result.glance),
             "%s -- %s . %s . %s",
             full_name, t.deity, t.quality, t.activity);

    /* T3: detail — full narrative */
    int written = snprintf(result.detail, sizeof(result.detail),
             "%s: %s. Deity: %s. %s day for %s.",
             full_name, t.brief, t.deity, t.quality, t.activity);

    /* Append nakshatra if provided */
    if (nakshatra_name != NULL &&
        written > 0 && (size_t)written < sizeof(result.detail)) {
        int added = snprintf(result.detail + written,
                 sizeof(result.detail) - (size_t)written,
                 " Nakshatra: %s.", nakshatra_name);
        if (added > 0) {
            written += added;
        }
    }

    /* Append yoga if provided */
    if (yoga_name != NULL &&
        written > 0 && (size_t)written < sizeof(result.detail)) {
        snprintf(result.detail + written,
                 sizeof(result.detail) - (size_t)written,
                 " Yoga: %s.", yoga_name);
    }

    return result;
}
