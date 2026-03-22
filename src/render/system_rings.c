/* system_rings.c — Knowledge system concentric ring definitions.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "system_rings.h"
#include "color_theory.h"
#include "../systems/unified/today_summary.h"
#include "../systems/unified/depth_ring.h"

/* Number of entries in CYCLE_LENGTHS table */
#define CYCLE_TABLE_SIZE ((int)TS_SYS_COUNT)

/* Natural cycle length for each system.
 * Default 12 for systems without a specific value. */
static const int CYCLE_LENGTHS[TS_SYS_COUNT] = {
    [TS_SYS_GREGORIAN]    = 12,   /* 12 months */
    [TS_SYS_TZOLKIN]      = 260,  /* 260 kin (20 seals x 13 tones) */
    [TS_SYS_HAAB]         = 19,   /* 18 months + Wayeb = 19 "months" */
    [TS_SYS_CHINESE]      = 60,   /* 60-year stem-branch cycle */
    [TS_SYS_HEBREW]       = 13,   /* 12 or 13 months (use 13 for max) */
    [TS_SYS_ISLAMIC]      = 12,   /* 12 lunar months */
    [TS_SYS_BUDDHIST]     = 12,   /* 12 months */
    [TS_SYS_HINDU]        = 30,   /* 30 tithis (lunar phases) */
    [TS_SYS_ICHING]       = 64,   /* 64 hexagrams */
    [TS_SYS_ASTROLOGY]    = 12,   /* 12 zodiac signs */
    [TS_SYS_HUMAN_DESIGN] = 64,   /* 64 gates */
    [TS_SYS_KABBALAH]     = 22,   /* 22 paths on Tree of Life */
    [TS_SYS_COPTIC]       = 13,   /* 13 months */
    [TS_SYS_ETHIOPIAN]    = 13,   /* 13 months */
    [TS_SYS_PERSIAN]      = 12,   /* 12 months */
    [TS_SYS_JAPANESE]     = 12,   /* 12 months */
    [TS_SYS_KOREAN]       = 12,   /* 12 animals */
    [TS_SYS_THAI]         = 12,   /* 12 months */
    [TS_SYS_GEOLOGICAL]   = 10,   /* 10 geological periods (simplified) */
    [TS_SYS_COSMIC]       = 8,    /* 8 cosmic epochs (simplified) */
    [TS_SYS_ASTRONOMY]    = 8,    /* 8 moon phases */
    [TS_SYS_TAROT]        = 22,   /* 22 Major Arcana */
    [TS_SYS_NUMEROLOGY]   = 9,    /* 9 root numbers */
    [TS_SYS_CHAKRA]       = 7,    /* 7 chakras */
    [TS_SYS_ZOROASTRIAN]  = 12,   /* 12 months */
    [TS_SYS_BALINESE]     = 210,  /* 210-day Pawukon cycle */
    [TS_SYS_FRENCH_REPUBLICAN] = 12, /* 12 months */
    [TS_SYS_AZTEC]        = 260,  /* 260-day Tonalpohualli */
    [TS_SYS_EGYPTIAN]     = 12,   /* 12 months + epagomenal */
    [TS_SYS_CELTIC]       = 13,   /* 13 tree months */
    [TS_SYS_LAO]          = 12,   /* 12 months */
    [TS_SYS_MYANMAR]      = 12,   /* 12 months */
    [TS_SYS_BAHAI]        = 19,   /* 19 months */
    [TS_SYS_TAMIL]        = 12,   /* 12 months */
};

/* Map ts_system_t -> ct_system_t.
 * -1 means no direct color_theory palette (use neutral fallback). */
static const int TS_TO_CT[TS_SYS_COUNT] = {
    [TS_SYS_GREGORIAN]    = CT_SYSTEM_GREGORIAN,
    [TS_SYS_TZOLKIN]      = CT_SYSTEM_TZOLKIN,
    [TS_SYS_HAAB]         = CT_SYSTEM_TZOLKIN,     /* Haab shares Mayan palette */
    [TS_SYS_CHINESE]      = CT_SYSTEM_CHINESE,
    [TS_SYS_HEBREW]       = CT_SYSTEM_HEBREW,
    [TS_SYS_ISLAMIC]      = CT_SYSTEM_ISLAMIC,
    [TS_SYS_BUDDHIST]     = CT_SYSTEM_BUDDHIST,
    [TS_SYS_HINDU]        = CT_SYSTEM_HINDU,
    [TS_SYS_ICHING]       = CT_SYSTEM_ICHING,
    [TS_SYS_ASTROLOGY]    = CT_SYSTEM_ASTROLOGY,
    [TS_SYS_HUMAN_DESIGN] = CT_SYSTEM_HUMAN_DESIGN,
    [TS_SYS_KABBALAH]     = CT_SYSTEM_KABBALAH,
    [TS_SYS_COPTIC]       = CT_SYSTEM_COPTIC,
    [TS_SYS_ETHIOPIAN]    = CT_SYSTEM_ETHIOPIAN,
    [TS_SYS_PERSIAN]      = CT_SYSTEM_PERSIAN,
    [TS_SYS_JAPANESE]     = CT_SYSTEM_JAPANESE,
    [TS_SYS_KOREAN]       = CT_SYSTEM_KOREAN,
    [TS_SYS_THAI]         = CT_SYSTEM_THAI,
    [TS_SYS_GEOLOGICAL]   = CT_SYSTEM_GEOLOGY,
    [TS_SYS_COSMIC]       = CT_SYSTEM_ASTRONOMY,   /* Cosmic shares Astronomy palette */
    [TS_SYS_ASTRONOMY]    = CT_SYSTEM_ASTRONOMY,
    [TS_SYS_TAROT]        = CT_SYSTEM_KABBALAH,   /* Tree of Life connection */
    [TS_SYS_NUMEROLOGY]   = CT_SYSTEM_GREGORIAN,  /* mathematical/numerical */
    [TS_SYS_CHAKRA]       = CT_SYSTEM_HINDU,      /* Vedic tradition */
    [TS_SYS_ZOROASTRIAN]  = CT_SYSTEM_PERSIAN,    /* Persian heritage */
    [TS_SYS_BALINESE]     = CT_SYSTEM_BUDDHIST,   /* Buddhist influenced */
    [TS_SYS_FRENCH_REPUBLICAN] = CT_SYSTEM_GREGORIAN,
    [TS_SYS_AZTEC]        = CT_SYSTEM_TZOLKIN,    /* Mesoamerican */
    [TS_SYS_EGYPTIAN]     = CT_SYSTEM_COPTIC,     /* Egyptian heritage */
    [TS_SYS_CELTIC]       = CT_SYSTEM_GEOLOGY,    /* Earth-based */
    [TS_SYS_LAO]          = CT_SYSTEM_BUDDHIST,
    [TS_SYS_MYANMAR]      = CT_SYSTEM_BUDDHIST,
    [TS_SYS_BAHAI]        = CT_SYSTEM_PERSIAN,    /* Persian origin */
    [TS_SYS_TAMIL]        = CT_SYSTEM_HINDU,      /* Indian tradition */
};

/* Human-readable labels for each system */
static const char *SYSTEM_LABELS[TS_SYS_COUNT] = {
    [TS_SYS_GREGORIAN]    = "Gregorian",
    [TS_SYS_TZOLKIN]      = "Tzolkin",
    [TS_SYS_HAAB]         = "Haab",
    [TS_SYS_CHINESE]      = "Chinese",
    [TS_SYS_HEBREW]       = "Hebrew",
    [TS_SYS_ISLAMIC]      = "Islamic",
    [TS_SYS_BUDDHIST]     = "Buddhist",
    [TS_SYS_HINDU]        = "Hindu",
    [TS_SYS_ICHING]       = "I Ching",
    [TS_SYS_ASTROLOGY]    = "Astrology",
    [TS_SYS_HUMAN_DESIGN] = "Human Design",
    [TS_SYS_KABBALAH]     = "Kabbalah",
    [TS_SYS_COPTIC]       = "Coptic",
    [TS_SYS_ETHIOPIAN]    = "Ethiopian",
    [TS_SYS_PERSIAN]      = "Persian",
    [TS_SYS_JAPANESE]     = "Japanese",
    [TS_SYS_KOREAN]       = "Korean",
    [TS_SYS_THAI]         = "Thai",
    [TS_SYS_GEOLOGICAL]   = "Geological",
    [TS_SYS_COSMIC]       = "Cosmic",
    [TS_SYS_EARTH]        = "Earth",
    [TS_SYS_ASTRONOMY]    = "Astronomy",
    [TS_SYS_TAROT]        = "Tarot",
    [TS_SYS_NUMEROLOGY]   = "Numerology",
    [TS_SYS_CHAKRA]       = "Chakra",
    [TS_SYS_ZOROASTRIAN]  = "Zoroastrian",
    [TS_SYS_BALINESE]     = "Balinese",
    [TS_SYS_FRENCH_REPUBLICAN] = "French Republican",
    [TS_SYS_AZTEC]        = "Aztec",
    [TS_SYS_EGYPTIAN]     = "Egyptian",
    [TS_SYS_CELTIC]       = "Celtic",
    [TS_SYS_LAO]          = "Lao",
    [TS_SYS_MYANMAR]      = "Myanmar",
    [TS_SYS_BAHAI]        = "Baha'i",
    [TS_SYS_TAMIL]        = "Tamil",
};

/* Depth ring representatives: one system per ring for the standard layout */
static const int DEPTH_REPS[DEPTH_RING_COUNT] = {
    [DEPTH_RING_COSMIC] = TS_SYS_GEOLOGICAL,   /* 10 segments */
    [DEPTH_RING_AGE]    = TS_SYS_HINDU,         /* 30 tithis */
    [DEPTH_RING_ERA]    = TS_SYS_KABBALAH,      /* 22 paths */
    [DEPTH_RING_YEAR]   = TS_SYS_ASTROLOGY,     /* 12 zodiac */
    [DEPTH_RING_CYCLE]  = TS_SYS_ICHING,        /* 64 hexagrams */
    [DEPTH_RING_TODAY]  = TS_SYS_TZOLKIN,        /* 260 kin */
};

/* Neutral fallback color for systems without a ct_system_t mapping */
static void sr_neutral_color(float out[3])
{
    out[0] = 0.6f;
    out[1] = 0.6f;
    out[2] = 0.6f;
}

int sr_cycle_length(int system_id)
{
    if (system_id < 0 || system_id >= CYCLE_TABLE_SIZE)
        return 12;
    int len = CYCLE_LENGTHS[system_id];
    return (len > 0) ? len : 12;
}

sr_system_def_t sr_system_def(int system_id)
{
    sr_system_def_t def;
    def.system_id = system_id;
    def.segment_count = sr_cycle_length(system_id);
    def.today_index = -1;

    /* Look up color from color_theory */
    if (system_id >= 0 && system_id < CYCLE_TABLE_SIZE) {
        int ct_id = TS_TO_CT[system_id];
        if (ct_id >= 0 && ct_id < (int)CT_SYSTEM_COUNT) {
            color_rgb_t c = ct_system_primary((ct_system_t)ct_id);
            def.color[0] = c.r;
            def.color[1] = c.g;
            def.color[2] = c.b;
        } else {
            sr_neutral_color(def.color);
        }
        def.label = SYSTEM_LABELS[system_id];
    } else {
        sr_neutral_color(def.color);
        def.label = "Unknown";
    }

    return def;
}

cr_layout_t sr_build_layout(const int *system_ids, const int *today_indices,
                             int count, float innermost_radius, float ring_gap)
{
    cr_layout_t layout;
    layout.innermost_radius = innermost_radius;
    layout.ring_gap = ring_gap;

    if (count > SR_MAX_SYSTEMS) count = SR_MAX_SYSTEMS;
    if (count > CR_MAX_RINGS) count = CR_MAX_RINGS;
    if (count < 0) count = 0;
    layout.ring_count = count;

    for (int i = 0; i < count; i++) {
        sr_system_def_t def = sr_system_def(system_ids[i]);
        layout.rings[i].segment_count = def.segment_count;
        layout.rings[i].highlight_index = today_indices ? today_indices[i] : -1;
        layout.rings[i].highlight_boost = 2.0f;
        layout.rings[i].segment_colors = (void *)0;  /* use uniform color */
        layout.rings[i].uniform_color[0] = def.color[0];
        layout.rings[i].uniform_color[1] = def.color[1];
        layout.rings[i].uniform_color[2] = def.color[2];
        layout.rings[i].base_alpha = 0.6f;
    }

    return layout;
}

cr_layout_t sr_depth_layout(const int *today_indices,
                             float innermost_radius, float ring_gap)
{
    int ids[DEPTH_RING_COUNT];
    for (int i = 0; i < DEPTH_RING_COUNT; i++) {
        ids[i] = DEPTH_REPS[i];
    }
    return sr_build_layout(ids, today_indices, DEPTH_RING_COUNT,
                            innermost_radius, ring_gap);
}

int sr_depth_representative(int ring)
{
    if (ring < 0 || ring >= DEPTH_RING_COUNT)
        return TS_SYS_GREGORIAN;
    return DEPTH_REPS[ring];
}

int sr_known_system_count(void)
{
    int count = 0;
    for (int i = 0; i < CYCLE_TABLE_SIZE; i++) {
        if (CYCLE_LENGTHS[i] > 0) count++;
    }
    return count;
}
