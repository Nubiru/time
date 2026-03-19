/* brain_explain.c — Explanation engine implementation.
 *
 * Static database of cross-system explanations, categorized by
 * relationship type (shared source, transmission, convergent evolution).
 *
 * Sources: structural_map concordance, Calendrical Calculations,
 * knowledge_graph transmission chains, cultural astronomy research.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_explain.h"
#include "convergence_detect.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* ===================================================================
 * Explanation database
 * =================================================================== */

static const br_explanation_t EXPLANATIONS[] = {
    /* --- SHARED SOURCE: same natural phenomenon --- */

    { CD_SYS_ASTRONOMY, CD_SYS_ASTROLOGY, BR_EXPLAIN_SHARED_SOURCE,
      "Same sky, different interpretation",
      "Both observe identical celestial positions. Astronomy measures; "
      "astrology interprets through archetypal symbolism." },

    { CD_SYS_ASTRONOMY, CD_SYS_CELTIC, BR_EXPLAIN_SHARED_SOURCE,
      "Solar and lunar markers",
      "Celtic Sabbats mark solstices, equinoxes, and cross-quarter days "
      "— the same astronomical events that define Earth's seasons." },

    { CD_SYS_ASTRONOMY, CD_SYS_HEBREW, BR_EXPLAIN_SHARED_SOURCE,
      "Lunar observation",
      "Hebrew months begin at the new moon. The calendar directly tracks "
      "the synodic month — the same cycle astronomy measures." },

    { CD_SYS_ASTRONOMY, CD_SYS_ISLAMIC, BR_EXPLAIN_SHARED_SOURCE,
      "Crescent moon sighting",
      "Islamic months begin when the crescent moon is sighted. This is "
      "direct astronomical observation embedded in religious practice." },

    { CD_SYS_HEBREW, CD_SYS_ISLAMIC, BR_EXPLAIN_SHARED_SOURCE,
      "Both track the same moon",
      "Both are lunar calendars counting from the same synodic month "
      "(29.53 days). Hebrew adds intercalary months to stay solar-aligned; "
      "Islamic does not, drifting through the seasons." },

    { CD_SYS_HEBREW, CD_SYS_BUDDHIST, BR_EXPLAIN_SHARED_SOURCE,
      "Lunar month boundaries",
      "Both use the lunar cycle for sacred observance. Hebrew months begin "
      "at new moon; Buddhist Uposatha days mark quarter-moon phases." },

    { CD_SYS_ISLAMIC, CD_SYS_BUDDHIST, BR_EXPLAIN_SHARED_SOURCE,
      "Pure lunar calendars",
      "Both are lunar calendars tied to moon observation. Islamic months "
      "begin at crescent sighting; Buddhist observance days align with "
      "full and new moons." },

    /* --- TRANSMISSION: historical knowledge transfer --- */

    { CD_SYS_CHINESE, CD_SYS_KOREAN, BR_EXPLAIN_TRANSMISSION,
      "Korean calendar derives from Chinese",
      "Korea adopted the Chinese astronomical calendar system. Both use "
      "the sexagenary cycle (60-year) and lunisolar month structure." },

    { CD_SYS_CHINESE, CD_SYS_JAPANESE, BR_EXPLAIN_TRANSMISSION,
      "Japanese adopted Chinese calendar",
      "Japan historically used the Chinese lunisolar calendar before "
      "switching to Gregorian in 1873. Rokuyo and sekki remain." },

    { CD_SYS_CHINESE, CD_SYS_THAI, BR_EXPLAIN_TRANSMISSION,
      "Thai calendar shares Chinese lunisolar roots",
      "Thai calendar uses lunisolar structure influenced by both Chinese "
      "and Indian astronomical traditions." },

    { CD_SYS_HINDU, CD_SYS_BUDDHIST, BR_EXPLAIN_TRANSMISSION,
      "Buddhist calendar from Surya Siddhanta",
      "The Buddhist calendar in Southeast Asia derives from the Hindu "
      "Surya Siddhanta astronomical text (4th century CE)." },

    { CD_SYS_HINDU, CD_SYS_THAI, BR_EXPLAIN_TRANSMISSION,
      "Thai calendar uses Hindu constants",
      "Thai astronomical calendar parameters come from the Indian Surya "
      "Siddhanta, transmitted through Khmer civilization." },

    { CD_SYS_HINDU, CD_SYS_MYANMAR, BR_EXPLAIN_TRANSMISSION,
      "Myanmar calendar from Surya Siddhanta",
      "Myanmar's calendar uses Surya Siddhanta parameters for solar and "
      "lunar calculations, adopted via Indian astronomical influence." },

    { CD_SYS_ICHING, CD_SYS_CHINESE, BR_EXPLAIN_TRANSMISSION,
      "I Ching is Chinese divination",
      "The I Ching is a Chinese text (c. 1000 BCE). Its 8 trigrams map "
      "to compass directions, seasons, and family relationships in "
      "Chinese cosmology." },

    { CD_SYS_ZOROASTRIAN, CD_SYS_PERSIAN, BR_EXPLAIN_TRANSMISSION,
      "Same calendar, different eras",
      "The Zoroastrian calendar IS the pre-Islamic Persian calendar. "
      "Both use 12 months of 30 days plus 5 epagomenal days." },

    { CD_SYS_BAHAI, CD_SYS_PERSIAN, BR_EXPLAIN_TRANSMISSION,
      "Bahai anchored to Naw-Ruz",
      "The Bahai calendar begins its year at Naw-Ruz (Persian New Year, "
      "spring equinox), directly inheriting this anchor from Persian "
      "tradition." },

    { CD_SYS_COPTIC, CD_SYS_PERSIAN, BR_EXPLAIN_SHARED_SOURCE,
      "Both solar with 12x30+5 structure",
      "Both calendars use twelve 30-day months plus 5 (or 6) epagomenal "
      "days. This structure derives from the ancient Egyptian calendar." },

    /* --- CONVERGENT: independent parallel development --- */

    { CD_SYS_HEBREW, CD_SYS_CHINESE, BR_EXPLAIN_CONVERGENT,
      "Independent lunisolar solutions",
      "Both cultures independently developed lunisolar calendars that add "
      "intercalary months to keep lunar months aligned with solar seasons. "
      "Hebrew uses a 19-year Metonic cycle; Chinese uses astronomical "
      "observation." },

    { CD_SYS_HEBREW, CD_SYS_HINDU, BR_EXPLAIN_CONVERGENT,
      "Parallel lunisolar traditions",
      "Both independently developed intercalary month systems. Hebrew "
      "uses the Metonic cycle (19 years); Hindu uses the adhika masa "
      "based on Surya Siddhanta calculations." },

    { CD_SYS_CHINESE, CD_SYS_HINDU, BR_EXPLAIN_CONVERGENT,
      "Convergent lunisolar engineering",
      "Both civilizations solved the same problem — aligning lunar months "
      "with solar years — using different astronomical traditions but "
      "arriving at structurally similar intercalation." },

    { CD_SYS_TZOLKIN, CD_SYS_HEBREW, BR_EXPLAIN_CONVERGENT,
      "Unrelated calendar traditions",
      "Mesoamerican and Near Eastern calendar systems developed with no "
      "known contact. The Tzolkin's 260-day cycle and the Hebrew Metonic "
      "cycle solve different problems." },

    { CD_SYS_TZOLKIN, CD_SYS_ISLAMIC, BR_EXPLAIN_CONVERGENT,
      "Independent sacred counting",
      "The Tzolkin (260-day) and Islamic (pure lunar) calendars are "
      "entirely independent inventions from civilizations with no "
      "historical contact." },

    { CD_SYS_TZOLKIN, CD_SYS_HINDU, BR_EXPLAIN_CONVERGENT,
      "Possible precession awareness",
      "Both cultures encode very long cycles. The Maya Long Count's "
      "5,125-year Great Cycle and Hindu Yuga system both approach "
      "precession-scale periods (~26,000 years), independently." },

    { CD_SYS_TZOLKIN, CD_SYS_CHINESE, BR_EXPLAIN_CONVERGENT,
      "Cyclic counting systems",
      "Both use cyclic day-counting: the Tzolkin's 260-day cycle (20x13) "
      "and the Chinese sexagenary cycle (10x12=60). Different numbers, "
      "same principle of interlocking sub-cycles." },

    { CD_SYS_ICHING, CD_SYS_HEBREW, BR_EXPLAIN_CONVERGENT,
      "Different traditions, no contact",
      "Chinese divination (I Ching) and Hebrew calendar — entirely "
      "separate cultural streams with no known historical transmission." },

    { CD_SYS_CELTIC, CD_SYS_TZOLKIN, BR_EXPLAIN_CONVERGENT,
      "No known contact",
      "Pre-Christian European and Mesoamerican traditions developed "
      "independently. Any alignment is coincidental or reflects shared "
      "astronomical observation (solstices, equinoxes)." },

    { CD_SYS_COPTIC, CD_SYS_EGYPTIAN, BR_EXPLAIN_TRANSMISSION,
      "Coptic inherits Egyptian",
      "The Coptic calendar is a direct descendant of the ancient Egyptian "
      "civil calendar, preserving the 12x30+5 day structure." },
};

static const int EXPLANATION_COUNT =
    (int)(sizeof(EXPLANATIONS) / sizeof(EXPLANATIONS[0]));

static const char *const EXPLAIN_TYPE_NAMES[] = {
    "Shared Source",
    "Transmission",
    "Convergent Evolution",
    "Unknown"
};

/* ===================================================================
 * Public API
 * =================================================================== */

const br_explanation_t *br_explain_lookup(int system_a, int system_b) {
    for (int i = 0; i < EXPLANATION_COUNT; i++) {
        if ((EXPLANATIONS[i].system_a == system_a &&
             EXPLANATIONS[i].system_b == system_b) ||
            (EXPLANATIONS[i].system_a == system_b &&
             EXPLANATIONS[i].system_b == system_a)) {
            return &EXPLANATIONS[i];
        }
    }
    return NULL;
}

int br_explain(int system_a, int system_b, char *buf, int buf_size) {
    if (!buf || buf_size <= 0) return 0;
    buf[0] = '\0';

    const br_explanation_t *entry = br_explain_lookup(system_a, system_b);
    if (entry && entry->detail) {
        int len = snprintf(buf, (size_t)buf_size, "%s", entry->detail);
        if (len < 0) return 0;
        return len < buf_size ? len : buf_size - 1;
    }

    /* Fallback for unknown pairs */
    int len = snprintf(buf, (size_t)buf_size,
                       "These systems may align by coincidence or through "
                       "shared observation of natural cycles.");
    if (len < 0) return 0;
    return len < buf_size ? len : buf_size - 1;
}

const char *br_explain_type_name(br_explain_type_t type) {
    if (type < 0 || type >= BR_EXPLAIN_TYPE_COUNT) return "?";
    return EXPLAIN_TYPE_NAMES[type];
}

int br_explain_count(void) {
    return EXPLANATION_COUNT;
}
