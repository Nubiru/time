/* structural_insight.c — Cross-system structural insight database
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Sefer Yetzirah, Ra Uru Hu (Human Design), Jose Arguelles
 * (Dreamspell), Reingold/Dershowitz (Calendrical Calculations),
 * Hindu Panchanga, Buddhist Abhidharma, geological time literature. */

#include "structural_insight.h"

#include <string.h>

/* ========== Insight database (30 pre-authored entries) ========== */

static const si_entry_t s_insights[] = {
    /* 0: KABBALAH <-> HUMAN_DESIGN */
    {
        .system_a = TS_SYS_KABBALAH,
        .system_b = TS_SYS_HUMAN_DESIGN,
        .title    = "The Tree and the Graph",
        .insight  = "The 10 Sefirot map structurally to 9 HD Centers "
                    "-- a topological reduction where Da'at (knowledge) "
                    "collapses into the Ajna-Head connection",
        .depth    = 3
    },
    /* 1: KABBALAH <-> HUMAN_DESIGN */
    {
        .system_a = TS_SYS_KABBALAH,
        .system_b = TS_SYS_HUMAN_DESIGN,
        .title    = "22 Paths, 36 Channels",
        .insight  = "The 22 paths on the Tree of Life parallel the 36 "
                    "channels in Human Design -- both connect centers of "
                    "consciousness through defined pathways",
        .depth    = 2
    },
    /* 2: ICHING <-> HUMAN_DESIGN */
    {
        .system_a = TS_SYS_ICHING,
        .system_b = TS_SYS_HUMAN_DESIGN,
        .title    = "The 64-fold Mirror",
        .insight  = "64 I Ching hexagrams map exactly to 64 Human Design "
                    "gates -- Ra Uru Hu built the entire gate system from "
                    "the hexagram sequence",
        .depth    = 2
    },
    /* 3: ICHING <-> TZOLKIN */
    {
        .system_a = TS_SYS_ICHING,
        .system_b = TS_SYS_TZOLKIN,
        .title    = "Binary Harmonics",
        .insight  = "260 kin / 4 colors = 65 harmonics. 64 hexagrams + 1 "
                    "(the Tao) = 65. Two ancient systems independently "
                    "converge on the same number",
        .depth    = 3
    },
    /* 4: TZOLKIN <-> ICHING */
    {
        .system_a = TS_SYS_TZOLKIN,
        .system_b = TS_SYS_ICHING,
        .title    = "Sacred Cycle Resonance",
        .insight  = "The Tzolkin's 260-day cycle and the I Ching's 64 "
                    "hexagrams share a deep structural kinship through the "
                    "number 4: both are built on quaternary foundations",
        .depth    = 2
    },
    /* 5: ASTROLOGY <-> KABBALAH */
    {
        .system_a = TS_SYS_ASTROLOGY,
        .system_b = TS_SYS_KABBALAH,
        .title    = "Twelve and Twenty-Two",
        .insight  = "12 zodiac signs correspond to 12 Hebrew simple letters "
                    "on the Tree of Life -- Aries through Pisces each "
                    "governs one diagonal path",
        .depth    = 2
    },
    /* 6: ASTROLOGY <-> KABBALAH */
    {
        .system_a = TS_SYS_ASTROLOGY,
        .system_b = TS_SYS_KABBALAH,
        .title    = "Seven Wanderers",
        .insight  = "7 classical planets map to 7 Hebrew double letters and "
                    "7 vertical paths -- as above in the sky, so below on "
                    "the Tree",
        .depth    = 1
    },
    /* 7: HEBREW <-> KABBALAH */
    {
        .system_a = TS_SYS_HEBREW,
        .system_b = TS_SYS_KABBALAH,
        .title    = "Letters of Creation",
        .insight  = "The 22 Hebrew letters ARE the 22 paths -- the Sefer "
                    "Yetzirah teaches that God created the world through "
                    "letter-combinations on these paths",
        .depth    = 3
    },
    /* 8: HEBREW <-> ISLAMIC */
    {
        .system_a = TS_SYS_HEBREW,
        .system_b = TS_SYS_ISLAMIC,
        .title    = "Children of Abraham",
        .insight  = "Both lunisolar (Hebrew) and pure lunar (Islamic) "
                    "calendars begin months with the new moon -- different "
                    "children of the same patriarch watching the same sky",
        .depth    = 1
    },
    /* 9: HEBREW <-> ISLAMIC */
    {
        .system_a = TS_SYS_HEBREW,
        .system_b = TS_SYS_ISLAMIC,
        .title    = "Lunar Brothers",
        .insight  = "The Hebrew molad and Islamic hilal both track lunar "
                    "conjunctions -- different mathematical models of the "
                    "same celestial event, accurate to within hours over "
                    "centuries",
        .depth    = 2
    },
    /* 10: HINDU <-> BUDDHIST */
    {
        .system_a = TS_SYS_HINDU,
        .system_b = TS_SYS_BUDDHIST,
        .title    = "The Cosmic Clock",
        .insight  = "Hindu Maha-Yuga (4.32 million years) and Buddhist "
                    "Maha-Kalpa (1.344 billion years) both model cosmic "
                    "cycles of creation and destruction -- different scales, "
                    "same intuition",
        .depth    = 2
    },
    /* 11: HINDU <-> BUDDHIST */
    {
        .system_a = TS_SYS_HINDU,
        .system_b = TS_SYS_BUDDHIST,
        .title    = "Shared Lunar Wisdom",
        .insight  = "Buddhist Uposatha and Hindu Purnima/Amavasya both mark "
                    "the full and new moon as sacred -- the same lunar rhythm "
                    "observed through different spiritual lenses",
        .depth    = 1
    },
    /* 12: HINDU <-> ASTROLOGY */
    {
        .system_a = TS_SYS_HINDU,
        .system_b = TS_SYS_ASTROLOGY,
        .title    = "27 and 12",
        .insight  = "27 Nakshatras x 4 padas = 108. 108 / 12 signs = 9 "
                    "Navamsa per sign. The Hindu system nests within the "
                    "zodiac through the sacred number 108",
        .depth    = 3
    },
    /* 13: BUDDHIST <-> GEOLOGICAL */
    {
        .system_a = TS_SYS_BUDDHIST,
        .system_b = TS_SYS_GEOLOGICAL,
        .title    = "Kalpas and Eons",
        .insight  = "One Buddhist Kalpa (336 million years) approximates one "
                    "geological era. One Maha-Kalpa (1.344 billion years) is "
                    "close to one geological eon. Independent derivations of "
                    "deep time",
        .depth    = 2
    },
    /* 14: TZOLKIN <-> CHINESE */
    {
        .system_a = TS_SYS_TZOLKIN,
        .system_b = TS_SYS_CHINESE,
        .title    = "The Calendar Round",
        .insight  = "Mayan 260 x 365 = 18,980-day Calendar Round echoes the "
                    "Chinese 60-year sexagenary cycle -- both are LCM-based "
                    "systems creating long cycles from short ones",
        .depth    = 2
    },
    /* 15: ASTROLOGY <-> ICHING */
    {
        .system_a = TS_SYS_ASTROLOGY,
        .system_b = TS_SYS_ICHING,
        .title    = "Celestial Binary",
        .insight  = "12 zodiac signs x 2 polarities = 24 solar terms "
                    "(Chinese). 8 trigrams x 8 trigrams = 64 hexagrams. Both "
                    "systems double and recombine to generate complexity from "
                    "simplicity",
        .depth    = 2
    },
    /* 16: GEOLOGICAL <-> COSMIC */
    {
        .system_a = TS_SYS_GEOLOGICAL,
        .system_b = TS_SYS_COSMIC,
        .title    = "Deep Time Unity",
        .insight  = "Earth's age (4.54 billion years) fits almost exactly in "
                    "one Hindu Kalpa (4.32 billion years). The ancients "
                    "intuited deep time without radiometric dating",
        .depth    = 3
    },
    /* 17: KABBALAH <-> ASTROLOGY */
    {
        .system_a = TS_SYS_KABBALAH,
        .system_b = TS_SYS_ASTROLOGY,
        .title    = "Four Worlds, Four Elements",
        .insight  = "The Four Worlds of Kabbalah (Atzilut/Beriah/Yetzirah/"
                    "Assiah) mirror the four classical elements (Fire/Water/"
                    "Air/Earth) governing astrological interpretation",
        .depth    = 1
    },
    /* 18: TZOLKIN <-> ASTROLOGY */
    {
        .system_a = TS_SYS_TZOLKIN,
        .system_b = TS_SYS_ASTROLOGY,
        .title    = "20 and 12",
        .insight  = "20 Tzolkin seals and 12 zodiac signs -- both divide "
                    "the cycle of experience into archetypal stations, one "
                    "solar and one sacred",
        .depth    = 1
    },
    /* 19: ISLAMIC <-> BUDDHIST */
    {
        .system_a = TS_SYS_ISLAMIC,
        .system_b = TS_SYS_BUDDHIST,
        .title    = "Lunar Observance",
        .insight  = "Islamic prayer times and Buddhist Uposatha both "
                    "structure daily/weekly practice around celestial "
                    "mechanics -- the Moon governs the rhythm of devotion "
                    "in both traditions",
        .depth    = 1
    },
    /* 20: HEBREW <-> HINDU */
    {
        .system_a = TS_SYS_HEBREW,
        .system_b = TS_SYS_HINDU,
        .title    = "19 and 27",
        .insight  = "The Hebrew 19-year Metonic cycle and the 27 Nakshatras: "
                    "19 x 27 = 513 ~ 512 = 2^9. Number theory connects "
                    "these apparently unrelated cycles",
        .depth    = 3
    },
    /* 21: GREGORIAN <-> TZOLKIN */
    {
        .system_a = TS_SYS_GREGORIAN,
        .system_b = TS_SYS_TZOLKIN,
        .title    = "365 and 260",
        .insight  = "The Gregorian 365-day year and Tzolkin 260-day sacred "
                    "cycle share no common factor (GCD=5). Their LCM = "
                    "18,980 days = exactly 52 years -- the Calendar Round",
        .depth    = 2
    },
    /* 22: GREGORIAN <-> ISLAMIC */
    {
        .system_a = TS_SYS_GREGORIAN,
        .system_b = TS_SYS_ISLAMIC,
        .title    = "Wandering Year",
        .insight  = "The Islamic calendar loses ~11 days against the "
                    "Gregorian each year, cycling through all seasons in "
                    "~33 years. This is not a flaw but a feature -- every "
                    "month experiences every season",
        .depth    = 1
    },
    /* 23: CHINESE <-> HEBREW */
    {
        .system_a = TS_SYS_CHINESE,
        .system_b = TS_SYS_HEBREW,
        .title    = "Lunisolar Siblings",
        .insight  = "Both use 19-year Metonic intercalation to synchronize "
                    "lunar months with solar years -- independently developed "
                    "on opposite sides of the Earth",
        .depth    = 2
    },
    /* 24: COPTIC <-> EGYPTIAN */
    {
        .system_a = TS_SYS_COPTIC,
        .system_b = SI_SYS_EGYPTIAN,
        .title    = "The Oldest Calendar",
        .insight  = "The Coptic calendar descends directly from the ancient "
                    "Egyptian civil calendar -- the oldest 365-day solar "
                    "calendar in human history, now 5000+ years old and "
                    "still in daily use",
        .depth    = 1
    },
    /* 25: CELTIC <-> ASTROLOGY */
    {
        .system_a = SI_SYS_CELTIC,
        .system_b = TS_SYS_ASTROLOGY,
        .title    = "Eight and Four",
        .insight  = "The 8 Celtic Sabbats divide the year at solstices, "
                    "equinoxes, and cross-quarters. Astrology uses the same "
                    "4 cardinal points but adds 8 more. Same sky, different "
                    "granularity",
        .depth    = 1
    },
    /* 26: PERSIAN <-> ZOROASTRIAN */
    {
        .system_a = TS_SYS_PERSIAN,
        .system_b = SI_SYS_ZOROASTRIAN,
        .title    = "Fire and Equinox",
        .insight  = "Both Persian (Nowruz) and Zoroastrian calendars anchor "
                    "their year to the spring equinox -- the cosmic renewal "
                    "that Zarathustra saw as Ahura Mazda's victory over "
                    "darkness",
        .depth    = 2
    },
    /* 27: ICHING <-> KABBALAH */
    {
        .system_a = TS_SYS_ICHING,
        .system_b = TS_SYS_KABBALAH,
        .title    = "Binary and Ternary",
        .insight  = "I Ching uses binary (yin/yang, 2^6=64). Kabbalah uses "
                    "ternary (3 pillars, 3 triads). Together they span the "
                    "fundamental number bases of symbolic mathematics",
        .depth    = 3
    },
    /* 28: HINDU <-> KABBALAH */
    {
        .system_a = TS_SYS_HINDU,
        .system_b = TS_SYS_KABBALAH,
        .title    = "10 and 10",
        .insight  = "10 Sefirot and 10 avatars of Vishnu -- both traditions "
                    "organize divine manifestation through a tenfold descent "
                    "from transcendent to immanent",
        .depth    = 2
    },
    /* 29: TZOLKIN <-> HEBREW */
    {
        .system_a = TS_SYS_TZOLKIN,
        .system_b = TS_SYS_HEBREW,
        .title    = "13 and 13",
        .insight  = "The Tzolkin has 13 tones. The Hebrew calendar has 13 "
                    "months in leap years. 13 appears sacred in both "
                    "traditions for different reasons -- one solar-galactic, "
                    "one lunisolar",
        .depth    = 2
    },
};

#define SI_DB_COUNT (int)(sizeof(s_insights) / sizeof(s_insights[0]))

/* ========== Internal helpers ========== */

/* Check if two entries match a system pair (order-independent). */
static int pair_matches(const si_entry_t *e, int sa, int sb)
{
    return (e->system_a == sa && e->system_b == sb) ||
           (e->system_a == sb && e->system_b == sa);
}

/* Check if an entry involves a given system. */
static int involves_system(const si_entry_t *e, int sys)
{
    return (e->system_a == sys || e->system_b == sys);
}

/* Get the partner system for an entry given one system. */
static int partner_of(const si_entry_t *e, int sys)
{
    return (e->system_a == sys) ? e->system_b : e->system_a;
}

/* ========== Public API ========== */

si_result_t si_query(int system_a, int system_b)
{
    si_result_t result;
    memset(&result, 0, sizeof(result));

    if (system_a < 0 || system_b < 0) return result;
    if (system_a >= SI_SYS_EXT_COUNT || system_b >= SI_SYS_EXT_COUNT)
        return result;
    if (system_a == system_b) return result;

    for (int i = 0; i < SI_DB_COUNT && result.count < SI_MAX_INSIGHTS; i++) {
        if (pair_matches(&s_insights[i], system_a, system_b)) {
            result.entries[result.count] = s_insights[i];
            result.count++;
        }
    }

    return result;
}

int si_all_count(void)
{
    return SI_DB_COUNT;
}

si_entry_t si_get(int index)
{
    if (index < 0 || index >= SI_DB_COUNT) {
        si_entry_t invalid = {
            .system_a = -1,
            .system_b = -1,
            .title    = "?",
            .insight  = "?",
            .depth    = 0
        };
        return invalid;
    }
    return s_insights[index];
}

int si_pairs_with(int system_id, int *out_partners, int max)
{
    if (system_id < 0 || system_id >= SI_SYS_EXT_COUNT) return 0;

    /* Collect unique partners */
    int partners[SI_DB_COUNT];
    int count = 0;

    for (int i = 0; i < SI_DB_COUNT; i++) {
        if (!involves_system(&s_insights[i], system_id)) continue;

        int p = partner_of(&s_insights[i], system_id);

        /* Check for duplicate */
        int dup = 0;
        for (int j = 0; j < count; j++) {
            if (partners[j] == p) { dup = 1; break; }
        }
        if (!dup) {
            partners[count] = p;
            count++;
        }
    }

    /* Write to output (or just return count if NULL) */
    if (out_partners != NULL) {
        int write_count = (count < max) ? count : max;
        for (int i = 0; i < write_count; i++) {
            out_partners[i] = partners[i];
        }
        return write_count;
    }

    return count;
}

const char *si_depth_name(int depth)
{
    switch (depth) {
    case 1: return "Surface";
    case 2: return "Intermediate";
    case 3: return "Deep";
    default: return "?";
    }
}
