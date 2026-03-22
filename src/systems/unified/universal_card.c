/* universal_card.c — Universal card text format (LANGUAGE L2.1)
 *
 * Generates layered card text for all 20 time/calendar systems.
 * Each system gets appropriate sensitivity, essence, and source. */

#include "universal_card.h"

/* ===================================================================
 * System metadata table
 * =================================================================== */

typedef struct {
    const char   *name;
    const char   *essence_template;  /* %s = date_str */
    const char   *source;
    sensitivity_t sensitivity;
} sys_meta_t;

static const sys_meta_t SYS_META[TS_SYS_COUNT] = {
    [TS_SYS_GREGORIAN] = {
        "Gregorian",
        "The civil calendar marks %s — time as the world counts it.",
        "Gregorian Reform, 1582 CE",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_TZOLKIN] = {
        "Tzolkin",
        "The sacred 260-day count shows %s — a unique energy signature.",
        "Dreamspell / Jose Arguelles",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_HAAB] = {
        "Haab'",
        "The solar calendar of the Maya reads %s — the earthly companion to the Tzolkin.",
        "Maya Solar Calendar, 365 days",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_CHINESE] = {
        "Chinese",
        "The Chinese calendar reveals %s — heaven, earth, and animal spirit interwoven.",
        "Traditional Chinese Lunisolar Calendar",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_HEBREW] = {
        "Hebrew",
        "The Hebrew calendar reads %s — time measured since creation.",
        "Hebrew Lunisolar Calendar",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_ISLAMIC] = {
        "Islamic",
        "The Hijri calendar marks %s — time measured from the Prophet's migration.",
        "Islamic Lunar Calendar (Hijri)",
        SENSITIVITY_SACRED
    },
    [TS_SYS_BUDDHIST] = {
        "Buddhist",
        "The Buddhist era shows %s — time measured from the Buddha's parinibbana.",
        "Buddhasakarat (Buddhist Era)",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_HINDU] = {
        "Hindu",
        "The Panchanga reveals %s — five limbs of cosmic time.",
        "Hindu Panchanga / Vedic Calendar",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_ICHING] = {
        "I Ching",
        "The oracle speaks: %s — change as the constant.",
        "I Ching / King Wen Sequence",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_ASTROLOGY] = {
        "Astrology",
        "The sky shows %s — celestial archetypes in motion.",
        "Western Tropical Astrology",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_HUMAN_DESIGN] = {
        "Human Design",
        "The Rave mandala activates %s — your energetic blueprint today.",
        "Human Design System / Ra Uru Hu",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_KABBALAH] = {
        "Kabbalah",
        "The Tree of Life illuminates %s — divine emanation made visible.",
        "Kabbalistic Tradition / Sefer Yetzirah",
        SENSITIVITY_SACRED
    },
    [TS_SYS_COPTIC] = {
        "Coptic",
        "The Coptic calendar reads %s — ancient Egypt's Christian inheritance.",
        "Coptic Calendar (Alexandrian)",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_ETHIOPIAN] = {
        "Ethiopian",
        "The Ethiopian calendar shows %s — a calendar that runs seven years behind.",
        "Ethiopian Calendar (Ge'ez)",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_PERSIAN] = {
        "Persian",
        "The Solar Hijri marks %s — spring as the eternal new year.",
        "Solar Hijri / Iranian Calendar",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_JAPANESE] = {
        "Japanese",
        "The era calendar shows %s — time marked by imperial reign.",
        "Japanese Era Calendar (Nengo)",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_KOREAN] = {
        "Korean",
        "The Korean calendar reads %s — Dangun's count from the beginning.",
        "Korean Calendar (Dangi)",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_THAI] = {
        "Thai",
        "The Thai calendar shows %s — Buddhist time in the Kingdom.",
        "Thai Solar Calendar",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_GEOLOGICAL] = {
        "Geological",
        "Deep time: %s — the Earth remembers in stone.",
        "International Commission on Stratigraphy",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_COSMIC] = {
        "Cosmic",
        "The universe is %s — every atom in your body was forged in a star.",
        "Standard Cosmological Model",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_EARTH] = {
        "Earth",
        "Right now on Earth: %s — daylight, season, and sun position shape your day.",
        "Solar geometry, axial tilt",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_ASTRONOMY] = {
        "Astronomy",
        "The night sky shows %s — the Moon's eternal conversation with the Sun.",
        "Observational Astronomy / Jean Meeus",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_TAROT] = {
        "Tarot",
        "Today's arcanum: %s — the universe deals one card from the major mysteries.",
        "Thoth Tarot / Aleister Crowley & Frieda Harris",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_NUMEROLOGY] = {
        "Numerology",
        "The numbers reveal %s — digits reduced to their root essence.",
        "Pythagorean Numerology",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_CHAKRA] = {
        "Chakra",
        "Today's energy center: %s — the body's map of consciousness.",
        "Vedic Chakra System",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_ZOROASTRIAN] = {
        "Zoroastrian",
        "The Yazdgerdi calendar reads %s — the fire temple's count of days.",
        "Zoroastrian (Yazdgerdi) Calendar",
        SENSITIVITY_SACRED
    },
    [TS_SYS_BALINESE] = {
        "Balinese",
        "The Pawukon cycle shows %s — ten concurrent weeks weaving sacred time.",
        "Balinese Pawukon Calendar",
        SENSITIVITY_SACRED
    },
    [TS_SYS_FRENCH_REPUBLICAN] = {
        "French Republican",
        "The calendar of reason reads %s — nature replaces saints.",
        "French Republican Calendar (Romme)",
        SENSITIVITY_GENERAL
    },
    [TS_SYS_AZTEC] = {
        "Aztec",
        "The Tonalpohualli reveals %s — the sacred count of days.",
        "Aztec Sacred Calendar",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_EGYPTIAN] = {
        "Egyptian",
        "The calendar of the pharaohs reads %s — time by the Nile's rhythm.",
        "Ancient Egyptian Civil Calendar",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_CELTIC] = {
        "Celtic",
        "The tree calendar whispers %s — Ogham letters written in wood.",
        "Celtic Tree Calendar (Robert Graves)",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_LAO] = {
        "Lao",
        "The Lao calendar shows %s — Buddhist time along the Mekong.",
        "Lao Buddhist Calendar",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_MYANMAR] = {
        "Myanmar",
        "The Myanmar calendar reads %s — a calendar shaped by monsoons and moons.",
        "Myanmar (Burmese) Calendar",
        SENSITIVITY_RESPECTFUL
    },
    [TS_SYS_BAHAI] = {
        "Baha'i",
        "The Badi' calendar marks %s — nineteen times nineteen, unity in form.",
        "Baha'i (Badi') Calendar",
        SENSITIVITY_SACRED
    },
    [TS_SYS_TAMIL] = {
        "Tamil",
        "The Tamil calendar shows %s — time by the sidereal sun's passage.",
        "Tamil Solar Calendar (Thiruvalluvar)",
        SENSITIVITY_RESPECTFUL
    }
};

/* ===================================================================
 * ucard_system_name
 * =================================================================== */

const char *ucard_system_name(int system_id)
{
    if (system_id < 0 || system_id >= TS_SYS_COUNT) return "?";
    return SYS_META[system_id].name;
}

/* ===================================================================
 * ucard_for_system
 * =================================================================== */

ucard_t ucard_for_system(const ts_entry_t *entry)
{
    ucard_t card;
    memset(&card, 0, sizeof(card));
    card.system_id = -1;

    if (!entry || !entry->active) return card;

    int sid = (int)entry->system;
    if (sid < 0 || sid >= TS_SYS_COUNT) return card;

    const sys_meta_t *meta = &SYS_META[sid];
    card.system_id = sid;
    card.sensitivity = meta->sensitivity;
    card.significance = entry->significance;

    /* Title: system name */
    snprintf(card.title, UCARD_TITLE_MAX, "%s", meta->name);

    /* Subtitle: the date string from today_summary */
    snprintf(card.subtitle, UCARD_SUBTITLE_MAX, "%s", entry->date_str);

    /* Essence: template with date filled in */
    snprintf(card.essence, UCARD_ESSENCE_MAX, meta->essence_template,
             entry->date_str);

    /* Detail: entry note if present, else a significance-based line */
    if (entry->note[0]) {
        snprintf(card.detail, UCARD_DETAIL_MAX, "%s. %s",
                 entry->date_str, entry->note);
    } else {
        switch (entry->significance) {
        case 3:
            snprintf(card.detail, UCARD_DETAIL_MAX,
                     "%s — an extraordinary moment in this system.",
                     entry->date_str);
            break;
        case 2:
            snprintf(card.detail, UCARD_DETAIL_MAX,
                     "%s — a significant marker in the cycle.",
                     entry->date_str);
            break;
        case 1:
            snprintf(card.detail, UCARD_DETAIL_MAX,
                     "%s — a notable day.",
                     entry->date_str);
            break;
        default:
            snprintf(card.detail, UCARD_DETAIL_MAX,
                     "%s — the cycle continues.",
                     entry->date_str);
            break;
        }
    }

    /* Source attribution */
    snprintf(card.source, UCARD_SOURCE_MAX, "%s", meta->source);

    return card;
}

/* ===================================================================
 * ucard_format_brief
 * =================================================================== */

int ucard_format_brief(const ucard_t *card, char *buf, int buf_size)
{
    if (!card || !buf || buf_size <= 0) return 0;
    buf[0] = '\0';
    if (card->system_id < 0) return 0;

    return snprintf(buf, (size_t)buf_size, "%s: %s",
                    card->title, card->essence);
}

/* ===================================================================
 * ucard_format_full
 * =================================================================== */

int ucard_format_full(const ucard_t *card, char *buf, int buf_size)
{
    if (!card || !buf || buf_size <= 0) return 0;
    buf[0] = '\0';
    if (card->system_id < 0) return 0;

    int pos = 0;
    int w;

    /* Title line */
    w = snprintf(buf + pos, (size_t)(buf_size - pos),
                 "%s\n%s\n", card->title, card->subtitle);
    if (w > 0) pos += w;

    /* Essence */
    if (card->essence[0] && pos < buf_size - 1) {
        w = snprintf(buf + pos, (size_t)(buf_size - pos),
                     "\n%s\n", card->essence);
        if (w > 0) pos += w;
    }

    /* Detail */
    if (card->detail[0] && pos < buf_size - 1) {
        w = snprintf(buf + pos, (size_t)(buf_size - pos),
                     "\n%s\n", card->detail);
        if (w > 0) pos += w;
    }

    /* Source */
    if (card->source[0] && pos < buf_size - 1) {
        w = snprintf(buf + pos, (size_t)(buf_size - pos),
                     "\n— %s", card->source);
        if (w > 0) pos += w;
    }

    return pos;
}
