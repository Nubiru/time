/* convergence_interpret.c -- Interpretive narratives for convergence events
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "convergence_interpret.h"
#include <string.h>
#include <stdio.h>

/* ===================================================================
 * Static data tables
 * =================================================================== */

static const ci_strength_t s_strengths[4] = {
    {
        CD_STRENGTH_MINOR,
        "Minor Alignment",
        "Two systems share a significant moment",
        "A whisper between traditions — coincidence or resonance"
    },
    {
        CD_STRENGTH_NOTABLE,
        "Notable Convergence",
        "Three systems align simultaneously",
        "Multiple traditions marking the same moment — the pattern becomes visible"
    },
    {
        CD_STRENGTH_MAJOR,
        "Major Convergence",
        "Four or more systems converge",
        "A rare chorus — when this many traditions agree, the moment demands attention"
    },
    {
        CD_STRENGTH_RARE,
        "Rare Convergence",
        "Five or more systems in alignment",
        "Exceedingly rare — the fabric of time reveals its hidden unity"
    }
};

static const ci_system_t s_systems[21] = {
    { CD_SYS_ASTRONOMY,    "Astronomy",            "Scientific observation",  "Solstice, equinox, or lunar phase" },
    { CD_SYS_ASTROLOGY,    "Astrology",            "Western esoteric",        "Sign ingress or major planetary aspect" },
    { CD_SYS_TZOLKIN,      "Tzolkin",              "Mayan Dreamspell",        "Kin 1 (Red Magnetic Dragon) or galactic portal day" },
    { CD_SYS_ICHING,       "I Ching",              "Chinese wisdom",          "Hexagram 1 (Creative) or 2 (Receptive)" },
    { CD_SYS_CHINESE,      "Chinese Calendar",     "East Asian",              "New Year or first of month" },
    { CD_SYS_HEBREW,       "Hebrew Calendar",      "Jewish",                  "New month (Rosh Chodesh) or High Holiday" },
    { CD_SYS_ISLAMIC,      "Islamic Calendar",     "Muslim",                  "New month (hilal) or sacred month boundary" },
    { CD_SYS_HINDU,        "Hindu Calendar",       "Vedic",                   "Sacred tithi (Ekadashi, Purnima, Amavasya)" },
    { CD_SYS_BUDDHIST,     "Buddhist Calendar",    "Theravada",               "Uposatha (quarter moon) or Vesak" },
    { CD_SYS_PERSIAN,      "Persian Calendar",     "Iranian",                 "Nowruz or month boundary" },
    { CD_SYS_COPTIC,       "Coptic Calendar",      "Egyptian Christian",      "New year or month boundary" },
    { CD_SYS_CELTIC,       "Celtic Wheel",         "European pagan",          "Sabbat (solstice, equinox, or cross-quarter)" },
    { CD_SYS_BAHAI,        "Baha'i Calendar",      "Baha'i Faith",            "Naw-Ruz or 19-day Feast" },
    { CD_SYS_JAPANESE,     "Japanese Calendar",    "Shinto/Buddhist",         "Taian (auspicious rokuyo) or sekki transition" },
    { CD_SYS_EGYPTIAN,     "Egyptian Calendar",    "Ancient Egyptian",        "Month boundary or epagomenal days" },
    { CD_SYS_FRENCH,       "French Republican",    "Revolutionary",           "Month boundary or complementary days" },
    { CD_SYS_KOREAN,       "Korean Calendar",      "Korean traditional",      "Festival or sexagenary cycle year 1" },
    { CD_SYS_THAI,         "Thai Calendar",        "Thai Buddhist",           "Songkran or festival" },
    { CD_SYS_TAMIL,        "Tamil Calendar",       "South Indian",            "Festival or Jovian cycle year 1" },
    { CD_SYS_MYANMAR,      "Myanmar Calendar",     "Burmese",                 "Month boundary or Thingyan" },
    { CD_SYS_ZOROASTRIAN,  "Zoroastrian Calendar", "Mazdayasna",              "Gatha days or seasonal gahanbar" }
};

static const ci_pattern_t s_patterns[8] = {
    {
        "Lunar Triple",
        "Three lunar calendars mark new month together",
        { CD_SYS_HEBREW, CD_SYS_ISLAMIC, CD_SYS_CHINESE, -1, -1, -1 }
    },
    {
        "Solar Cross",
        "Solstice/equinox aligns with calendar boundaries",
        { CD_SYS_ASTRONOMY, CD_SYS_CELTIC, CD_SYS_PERSIAN, -1, -1, -1 }
    },
    {
        "Sacred Day",
        "Multiple sacred day observances coincide",
        { CD_SYS_HINDU, CD_SYS_BUDDHIST, CD_SYS_HEBREW, -1, -1, -1 }
    },
    {
        "New Year Cluster",
        "Multiple new year celebrations align",
        { CD_SYS_PERSIAN, CD_SYS_BAHAI, CD_SYS_CHINESE, -1, -1, -1 }
    },
    {
        "East-West Bridge",
        "Eastern and Western systems resonate",
        { CD_SYS_ASTROLOGY, CD_SYS_ICHING, CD_SYS_HINDU, -1, -1, -1 }
    },
    {
        "Dreamspell Gateway",
        "Mayan portal day coincides with other markers",
        { CD_SYS_TZOLKIN, CD_SYS_ASTRONOMY, CD_SYS_ASTROLOGY, -1, -1, -1 }
    },
    {
        "Deep Time Echo",
        "Cosmic cycle and human calendar align",
        { CD_SYS_ASTRONOMY, CD_SYS_BUDDHIST, CD_SYS_HINDU, -1, -1, -1 }
    },
    {
        "Universal Renewal",
        "Month/year boundaries across 4+ traditions",
        { CD_SYS_HEBREW, CD_SYS_ISLAMIC, CD_SYS_CHINESE, CD_SYS_PERSIAN, -1, -1 }
    }
};

static const ci_strength_t s_invalid_strength = {
    0, "?", "?", "?"
};

static const ci_system_t s_invalid_system = {
    0, "?", "?", "?"
};

static const ci_pattern_t s_invalid_pattern = {
    "?", "?", { -1, -1, -1, -1, -1, -1 }
};

/* ===================================================================
 * Public API
 * =================================================================== */

ci_strength_t ci_strength_data(cd_strength_t strength)
{
    if (strength < CD_STRENGTH_MINOR || strength > CD_STRENGTH_RARE) {
        return s_invalid_strength;
    }
    return s_strengths[strength - 1];
}

ci_system_t ci_system_data(cd_system_t system)
{
    if ((int)system < 0 || (int)system >= CD_SYS_COUNT) {
        return s_invalid_system;
    }
    return s_systems[system];
}

ci_pattern_t ci_pattern_data(int index)
{
    if (index < 0 || index >= 8) {
        return s_invalid_pattern;
    }
    return s_patterns[index];
}

convergence_interp_t ci_interpret(cd_strength_t strength,
                                  const cd_system_t *systems,
                                  int system_count)
{
    convergence_interp_t result;
    memset(&result, 0, sizeof(result));

    ci_strength_t st = ci_strength_data(strength);

    /* Glyph: one star per strength level */
    int stars = (int)strength;
    if (stars < 1) stars = 1;
    if (stars > 4) stars = 4;
    for (int i = 0; i < stars; i++) {
        result.glyph[i] = '*';
    }
    result.glyph[stars] = '\0';

    /* Handle NULL/empty systems */
    if (systems == NULL || system_count <= 0) {
        snprintf(result.glance, sizeof(result.glance),
                 "%s", st.name);
        snprintf(result.detail, sizeof(result.detail),
                 "%s.", st.description);
        return result;
    }

    /* Glance: "{strength_name} -- {count} traditions align" */
    snprintf(result.glance, sizeof(result.glance),
             "%s — %d traditions align",
             st.name, system_count);

    /* Detail: "{description}. Participating: sys1 (what1), sys2 (what2), ... {significance}." */
    int pos = 0;
    int remaining = (int)sizeof(result.detail);

    int n = snprintf(result.detail + pos, (size_t)remaining,
                     "%s. Participating: ", st.description);
    if (n > 0 && n < remaining) {
        pos += n;
        remaining -= n;
    }

    for (int i = 0; i < system_count && remaining > 1; i++) {
        ci_system_t sys = ci_system_data(systems[i]);
        if (i > 0) {
            n = snprintf(result.detail + pos, (size_t)remaining, ", ");
            if (n > 0 && n < remaining) {
                pos += n;
                remaining -= n;
            }
        }
        n = snprintf(result.detail + pos, (size_t)remaining,
                     "%s (%s)", sys.system_name, sys.what_aligns);
        if (n > 0 && n < remaining) {
            pos += n;
            remaining -= n;
        }
    }

    /* Append significance */
    snprintf(result.detail + pos, (size_t)remaining,
             ". %s.", st.significance);

    return result;
}

int ci_strength_count(void)
{
    return 4;
}

int ci_system_count(void)
{
    return CD_SYS_COUNT;
}

int ci_pattern_count(void)
{
    return 8;
}
