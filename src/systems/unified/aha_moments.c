/* aha_moments.c -- Aha Moments insight database implementation.
 *
 * 50 pre-authored insight cards that surface when specific conditions
 * align. Each has a trigger condition, depth level, and system mask.
 *
 * Pure module: no globals, no malloc, no side effects. */

#include "aha_moments.h"

#include <string.h>

/* -------------------------------------------------------------------
 * Internal: popcount for system bitmask
 * ------------------------------------------------------------------- */
static int popcount_u(unsigned int x)
{
    int c = 0;
    while (x) {
        c += (int)(x & 1u);
        x >>= 1;
    }
    return c;
}

/* -------------------------------------------------------------------
 * Internal: simple insertion sort on candidates by depth descending
 * ------------------------------------------------------------------- */
typedef struct {
    int index;
    int depth;
} aha_candidate_t;

static void sort_candidates(aha_candidate_t *arr, int n)
{
    for (int i = 1; i < n; i++) {
        aha_candidate_t tmp = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].depth < tmp.depth) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = tmp;
    }
}

/* -------------------------------------------------------------------
 * The 50 insight entries
 *
 * Flags for lunar sub-types (encoded in system_mask upper bits):
 *   bit 16 = full-moon-specific
 *   bit 17 = new-moon-specific
 *   (if neither set, any lunar phase matches)
 * ------------------------------------------------------------------- */
#define LUNAR_FULL  (1u << 16)
#define LUNAR_NEW   (1u << 17)

/* System bitmask constants */
#define SYS_ASTRONOMY    (1u << 0)
#define SYS_ASTROLOGY    (1u << 1)
#define SYS_TZOLKIN      (1u << 2)
#define SYS_ICHING       (1u << 3)
#define SYS_CHINESE      (1u << 4)
#define SYS_HUMAN_DESIGN (1u << 5)
#define SYS_HEBREW       (1u << 6)
#define SYS_ISLAMIC      (1u << 7)
#define SYS_BUDDHIST     (1u << 8)
#define SYS_HINDU        (1u << 9)
#define SYS_KABBALAH     (1u << 10)
#define SYS_GREGORIAN    (1u << 11)
#define SYS_GEOLOGY      (1u << 12)
#define SYS_EARTH        (1u << 13)

/* Internal entry with const char* for static init */
typedef struct {
    int id;
    const char *title;
    const char *text;
    aha_trigger_t trigger;
    unsigned int system_mask;
    int min_convergence;
    int depth;
} aha_entry_internal_t;

static const aha_entry_internal_t DB[50] = {
    /* === Convergence triggers (IDs 0-9) === */
    { 0, "The Same Moon",
      "Right now, three different continents look at the same moon and "
      "call it sacred -- Hebrew Rosh Chodesh, Islamic Hilal, Buddhist "
      "Uposatha",
      AHA_TRIG_CONVERGENCE,
      SYS_HEBREW | SYS_ISLAMIC | SYS_BUDDHIST | LUNAR_FULL,
      2, 2 },

    { 1, "Triple Renewal",
      "Hebrew, Islamic, and Chinese calendars all start months with the "
      "new moon -- three civilizations that never met, watching the same "
      "sky",
      AHA_TRIG_CONVERGENCE,
      SYS_HEBREW | SYS_ISLAMIC | SYS_CHINESE,
      2, 2 },

    { 2, "The Great Alignment",
      "When four or more time systems mark today as significant, the "
      "ancients would have called this a day of power",
      AHA_TRIG_CONVERGENCE,
      0, 3, 3 },

    { 3, "Echoes Across Millennia",
      "The systems converging today were invented thousands of years "
      "and thousands of miles apart -- yet they agree on this moment",
      AHA_TRIG_CONVERGENCE,
      0, 2, 3 },

    { 4, "Festival of Festivals",
      "Multiple cultural festivals occur simultaneously -- the world "
      "celebrates together without knowing it",
      AHA_TRIG_CONVERGENCE,
      0, 3, 2 },

    { 5, "The Lunar Consensus",
      "Every lunar calendar on Earth agrees: this is a significant "
      "moment in the moon's cycle",
      AHA_TRIG_CONVERGENCE,
      SYS_HEBREW | SYS_ISLAMIC | SYS_BUDDHIST,
      2, 2 },

    { 6, "Solar Agreement",
      "Solstice alignments across traditions -- from Celtic Sabbats to "
      "Chinese solar terms to Zoroastrian Gahanbars",
      AHA_TRIG_CONVERGENCE,
      0, 2, 2 },

    { 7, "The Convergence Window",
      "Moments like this -- when multiple ancient calendars align -- "
      "occur only a few times per year",
      AHA_TRIG_CONVERGENCE,
      0, 3, 2 },

    { 8, "One Sky, Many Eyes",
      "The same astronomical event that triggers today's convergence "
      "was observed by every civilization in this alignment",
      AHA_TRIG_CONVERGENCE,
      SYS_ASTRONOMY, 2, 2 },

    { 9, "Calendar Chorus",
      "Each calendar system is a voice. Today, several sing the same "
      "note -- a momentary harmony in the choir of time",
      AHA_TRIG_CONVERGENCE,
      0, 2, 1 },

    /* === Personal triggers (IDs 10-14) === */
    { 10, "Galactic Birthday",
      "Your exact Tzolkin kin returns today -- in the Dreamspell, this "
      "is your galactic birthday, a day of renewed purpose",
      AHA_TRIG_PERSONAL,
      SYS_TZOLKIN, 0, 2 },

    { 11, "Oracle Activation",
      "A kin in your Dreamspell oracle cross is active today -- the "
      "universe is speaking to you through your birth signature",
      AHA_TRIG_PERSONAL,
      SYS_TZOLKIN, 0, 2 },

    { 12, "Birth Month Return",
      "You are in your Hebrew birth month -- in Kabbalistic tradition, "
      "this is when your soul's energy is strongest",
      AHA_TRIG_PERSONAL,
      SYS_HEBREW | SYS_KABBALAH, 0, 2 },

    { 13, "Sign Season",
      "The Sun is in your birth sign -- your annual solar return "
      "approaches, a time of renewal and reflection",
      AHA_TRIG_PERSONAL,
      SYS_ASTROLOGY, 0, 1 },

    { 14, "The Number in Your Chart",
      "The number 7 appears across your chart: this is the number of "
      "completion, rest, and divine pattern",
      AHA_TRIG_PERSONAL,
      0, 0, 2 },

    /* === Number triggers (IDs 15-19) === */
    { 15, "The Number 7",
      "7 classical planets, 7 Hebrew double letters, 7 chakras, 7 days "
      "of the week -- the most sacred number in human civilization",
      AHA_TRIG_NUMBER,
      SYS_ASTRONOMY | SYS_HEBREW, 0, 2 },

    { 16, "The Number 12",
      "12 zodiac signs, 12 Hebrew tribes, 12 Chinese branches, 12 "
      "months -- the number of cosmic completion",
      AHA_TRIG_NUMBER,
      SYS_ASTROLOGY | SYS_HEBREW | SYS_CHINESE | SYS_GREGORIAN, 0, 2 },

    { 17, "The Number 22",
      "22 Hebrew letters, 22 paths on the Tree of Life, 22 Major "
      "Arcana -- the number of creation's alphabet",
      AHA_TRIG_NUMBER,
      SYS_HEBREW | SYS_KABBALAH, 0, 2 },

    { 18, "The Number 64",
      "64 I Ching hexagrams, 64 Human Design gates, 64 DNA codons -- "
      "the binary perfection of 2^6 appears in nature and wisdom",
      AHA_TRIG_NUMBER,
      SYS_ICHING | SYS_HUMAN_DESIGN, 0, 3 },

    { 19, "108 Sacred",
      "27 Nakshatras x 4 padas = 108 -- the number of prayer beads, "
      "Sun diameters to Earth, and the bridge between Hindu astronomy "
      "and devotion",
      AHA_TRIG_NUMBER,
      SYS_HINDU | SYS_ASTRONOMY, 0, 3 },

    /* === Lunar triggers (IDs 20-24) === */
    { 20, "Full Moon Across Traditions",
      "Tonight's full moon is Purnima (Hindu), a potential Uposatha "
      "(Buddhist), and near the middle of Hebrew and Islamic months -- "
      "fullness recognized everywhere",
      AHA_TRIG_LUNAR,
      SYS_HINDU | SYS_BUDDHIST | SYS_HEBREW | SYS_ISLAMIC | LUNAR_FULL,
      0, 2 },

    { 21, "New Moon Unity",
      "The new moon tonight marks new months in Hebrew, Islamic, and "
      "Chinese calendars -- darkness as shared beginning",
      AHA_TRIG_LUNAR,
      SYS_HEBREW | SYS_ISLAMIC | SYS_CHINESE | LUNAR_NEW,
      0, 2 },

    { 22, "The Tidal Pull",
      "The same gravitational force that creates tonight's lunar phase "
      "moves the oceans, marks calendar months, and once guided "
      "navigation",
      AHA_TRIG_LUNAR,
      SYS_ASTRONOMY | SYS_EARTH, 0, 2 },

    { 23, "Moonlight Mathematics",
      "The synodic month (29.530588853 days) is the most precisely "
      "measured cycle in pre-telescopic astronomy -- every lunar "
      "calendar depends on these exact seconds",
      AHA_TRIG_LUNAR,
      SYS_ASTRONOMY, 0, 3 },

    { 24, "Eclipse Memory",
      "Every lunar calendar carries the memory of eclipses in its "
      "intercalation rules -- the mathematics of fear become the "
      "mathematics of faith",
      AHA_TRIG_LUNAR,
      0, 0, 3 },

    /* === Seasonal triggers (IDs 25-29) === */
    { 25, "Solstice Agreement",
      "Every culture with a calendar marks the solstice -- from "
      "Stonehenge to Inti Raymi to Yalda Night, humanity watches the "
      "sun turn",
      AHA_TRIG_SEASONAL,
      SYS_ASTRONOMY, 0, 2 },

    { 26, "Equinox Balance",
      "Nowruz, Ostara, and the vernal equinox -- when night and day "
      "balance, civilizations from Persia to Ireland mark the new year",
      AHA_TRIG_SEASONAL,
      SYS_ASTRONOMY, 0, 2 },

    { 27, "Cross-Quarter Wisdom",
      "The Celtic cross-quarter days (Samhain, Imbolc, Beltane, "
      "Lughnasadh) mark what the solstices and equinoxes miss: the "
      "midpoints where seasons actually change",
      AHA_TRIG_SEASONAL,
      0, 0, 2 },

    { 28, "Winter's Teaching",
      "The darkest month in the Hebrew calendar (Tevet) and the "
      "longest night (Yalda in Persian) both teach: even in deepest "
      "darkness, light returns",
      AHA_TRIG_SEASONAL,
      SYS_HEBREW, 0, 3 },

    { 29, "Summer Peak",
      "Litha, summer solstice, peak of light -- but Celtic wisdom "
      "notes this is when darkness begins its return. Joy carries its "
      "own impermanence",
      AHA_TRIG_SEASONAL,
      0, 0, 3 },

    /* === Deep time triggers (IDs 30-34) === */
    { 30, "Kalpa and Eon",
      "One Buddhist Kalpa (336 million years) approximates one "
      "geological era. Ancient monks measured deep time without rocks "
      "or radiation",
      AHA_TRIG_DEEP_TIME,
      SYS_BUDDHIST | SYS_GEOLOGY, 0, 3 },

    { 31, "Earth's Age in Scripture",
      "Hindu scriptures place Earth's age at 4.32 billion years (one "
      "Kalpa of Brahma). Modern radiometric dating gives 4.54 billion. "
      "The agreement is stunning",
      AHA_TRIG_DEEP_TIME,
      SYS_HINDU | SYS_GEOLOGY, 0, 3 },

    { 32, "The Boring Billion",
      "For a billion years (1.8-0.8 Ga), almost nothing changed on "
      "Earth. Then everything did. Deep time teaches patience on a "
      "planetary scale",
      AHA_TRIG_DEEP_TIME,
      SYS_GEOLOGY, 0, 3 },

    { 33, "Snowball Earth",
      "At least twice, the entire planet froze solid -- and life "
      "survived. The geological record is a story of resilience at "
      "scales we cannot comprehend",
      AHA_TRIG_DEEP_TIME,
      SYS_GEOLOGY, 0, 3 },

    { 34, "Your Carbon",
      "Every carbon atom in your body was forged in a star. You are "
      "literally made of stellar nuclear fusion -- and you are reading "
      "this on a Tuesday",
      AHA_TRIG_DEEP_TIME,
      SYS_ASTRONOMY | SYS_GEOLOGY, 0, 2 },

    /* === Structural triggers (IDs 35-44) === */
    { 35, "Binary Universe",
      "I Ching hexagrams use 6-bit binary (2^6 = 64). DNA codons use "
      "6-bit encoding (4^3 = 64). Life's code and wisdom's code share "
      "the same structure",
      AHA_TRIG_STRUCTURAL,
      SYS_ICHING | SYS_HUMAN_DESIGN, 0, 3 },

    { 36, "The Tree and the Body",
      "Kabbalah's Tree of Life and Human Design's Bodygraph are "
      "different maps of the same territory -- 10 nodes, connecting "
      "paths, energy flowing through form",
      AHA_TRIG_STRUCTURAL,
      SYS_KABBALAH | SYS_HUMAN_DESIGN, 0, 3 },

    { 37, "Letters Become Worlds",
      "In Kabbalah, the 22 Hebrew letters are not symbols of creation "
      "-- they ARE creation. Each letter is a force, each word a world",
      AHA_TRIG_STRUCTURAL,
      SYS_KABBALAH | SYS_HEBREW, 0, 3 },

    { 38, "Planetary Sevenfold",
      "The 7 classical planets govern the 7 days, the 7 chakras, the "
      "7 metals of alchemy, and the 7 double letters of Hebrew -- one "
      "pattern, many reflections",
      AHA_TRIG_STRUCTURAL,
      SYS_ASTRONOMY | SYS_HEBREW, 0, 2 },

    { 39, "The Zodiac in Every System",
      "The 12-fold zodiac appears in Hebrew (tribes), Chinese "
      "(branches), Tarot (zodiac Majors), and Kabbalah (simple "
      "letters) -- the most universal sacred number",
      AHA_TRIG_STRUCTURAL,
      SYS_ASTROLOGY | SYS_HEBREW | SYS_CHINESE | SYS_KABBALAH, 0, 2 },

    { 40, "Calendar Round Magic",
      "The Mayan Calendar Round (52 years) is the LCM of 260 and 365. "
      "Similarly, the Chinese sexagenary (60 years) is the LCM of 10 "
      "stems and 12 branches. Math creates ritual",
      AHA_TRIG_STRUCTURAL,
      SYS_TZOLKIN | SYS_CHINESE, 0, 3 },

    { 41, "Precession's Gift",
      "The 25,920-year precession cycle was known to the Egyptians, "
      "encoded in the Hindu Yugas, and tracked by the Maya. The wobble "
      "of Earth's axis united astronomy across civilizations",
      AHA_TRIG_STRUCTURAL,
      SYS_ASTRONOMY | SYS_HINDU | SYS_TZOLKIN, 0, 3 },

    { 42, "Four Worlds Everywhere",
      "Kabbalah's four worlds, the four Hindu Vedas, the four "
      "classical elements, the four Buddhist Noble Truths -- the "
      "quaternary appears wherever wisdom organizes reality",
      AHA_TRIG_STRUCTURAL,
      SYS_KABBALAH | SYS_HINDU | SYS_BUDDHIST, 0, 2 },

    { 43, "Sacred Geometry Lives Here",
      "The golden ratio (phi = 1.618...) governs this app's layout, "
      "the spiral of a nautilus shell, the arrangement of sunflower "
      "seeds, and the proportions of the Parthenon",
      AHA_TRIG_STRUCTURAL,
      0, 0, 1 },

    { 44, "The 260-Day Mystery",
      "Why 260? It is the human gestation period. It is 20 x 13. It "
      "shares no factor with 365. The Maya chose it -- and it turns "
      "out to be astronomically significant too",
      AHA_TRIG_STRUCTURAL,
      SYS_TZOLKIN, 0, 2 },

    /* === Always triggers (IDs 45-49) === */
    { 45, "Time Is Art",
      "Jose Arguelles: 'Time is not money. Time is art.' Every "
      "calendar is a work of art -- a civilization's attempt to give "
      "rhythm to existence",
      AHA_TRIG_ALWAYS,
      SYS_TZOLKIN, 0, 1 },

    { 46, "The Moving Image",
      "Plato called time 'the moving image of eternity.' Every tick of "
      "every clock is a photograph of infinity",
      AHA_TRIG_ALWAYS,
      0, 0, 2 },

    { 47, "Impermanence",
      "The Buddhist teaching of anicca: all conditioned things arise "
      "and pass away. This moment -- this exact configuration of every "
      "calendar -- will never recur",
      AHA_TRIG_ALWAYS,
      SYS_BUDDHIST, 0, 3 },

    { 48, "Synchronicity",
      "Jung's synchronicity: meaningful coincidence without causal "
      "connection. When unrelated calendars align, is it coincidence "
      "or pattern? The question itself is the gift",
      AHA_TRIG_ALWAYS,
      0, 0, 2 },

    { 49, "The Red Thread",
      "Gabriel's vision: 'the unseen red thread that connects humans "
      "together, that even though we all have different faces and "
      "beliefs, we are actually all one and the same, cut from the "
      "same cloth'",
      AHA_TRIG_ALWAYS,
      0, 0, 3 },
};

#define AHA_DB_COUNT 50

/* -------------------------------------------------------------------
 * Public API
 * ------------------------------------------------------------------- */

int aha_count(void)
{
    return AHA_DB_COUNT;
}

aha_entry_t aha_get(int id)
{
    aha_entry_t e;
    memset(&e, 0, sizeof(e));

    if (id < 0 || id >= AHA_DB_COUNT) {
        e.id = -1;
        strncpy(e.title, "?", AHA_TITLE_MAX - 1);
        e.title[AHA_TITLE_MAX - 1] = '\0';
        strncpy(e.text, "?", AHA_TEXT_MAX - 1);
        e.text[AHA_TEXT_MAX - 1] = '\0';
        return e;
    }

    const aha_entry_internal_t *src = &DB[id];
    e.id = src->id;
    strncpy(e.title, src->title, AHA_TITLE_MAX - 1);
    e.title[AHA_TITLE_MAX - 1] = '\0';
    strncpy(e.text, src->text, AHA_TEXT_MAX - 1);
    e.text[AHA_TEXT_MAX - 1] = '\0';
    e.trigger = src->trigger;
    e.system_mask = src->system_mask & 0xFFFFu; /* strip internal flags */
    e.min_convergence = src->min_convergence;
    e.depth = src->depth;
    return e;
}

aha_context_t aha_default_context(void)
{
    aha_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    return ctx;
}

const char *aha_trigger_name(aha_trigger_t trigger)
{
    switch (trigger) {
    case AHA_TRIG_CONVERGENCE: return "convergence";
    case AHA_TRIG_PERSONAL:    return "personal";
    case AHA_TRIG_NUMBER:      return "number";
    case AHA_TRIG_LUNAR:       return "lunar";
    case AHA_TRIG_SEASONAL:    return "seasonal";
    case AHA_TRIG_DEEP_TIME:   return "deep_time";
    case AHA_TRIG_STRUCTURAL:  return "structural";
    case AHA_TRIG_ALWAYS:      return "always";
    default:                   return "?";
    }
}

/* -------------------------------------------------------------------
 * match_entry: does this entry match the given context?
 * ------------------------------------------------------------------- */
static int match_entry(const aha_entry_internal_t *e,
                       const aha_context_t *ctx)
{
    switch (e->trigger) {
    case AHA_TRIG_CONVERGENCE:
        return ctx->convergence_score >= e->min_convergence
            && e->min_convergence > 0;

    case AHA_TRIG_PERSONAL:
        return ctx->has_birth == 1;

    case AHA_TRIG_NUMBER:
        return popcount_u(ctx->active_systems) >= 3;

    case AHA_TRIG_LUNAR: {
        int has_full = (e->system_mask & LUNAR_FULL) != 0;
        int has_new  = (e->system_mask & LUNAR_NEW)  != 0;
        if (has_full) return ctx->is_full_moon == 1;
        if (has_new)  return ctx->is_new_moon == 1;
        /* Neither flag: any lunar phase triggers */
        return ctx->is_full_moon == 1 || ctx->is_new_moon == 1;
    }

    case AHA_TRIG_SEASONAL:
        return ctx->is_solstice == 1 || ctx->is_equinox == 1;

    case AHA_TRIG_DEEP_TIME:
        return ctx->scale_level >= 4;

    case AHA_TRIG_STRUCTURAL:
        return popcount_u(ctx->active_systems) >= 2;

    case AHA_TRIG_ALWAYS:
        return 1;

    default:
        return 0;
    }
}

aha_result_t aha_evaluate(const aha_context_t *ctx)
{
    aha_result_t result;
    memset(&result, 0, sizeof(result));

    if (!ctx) {
        return result;
    }

    /* Collect all matching candidates */
    aha_candidate_t candidates[AHA_DB_COUNT];
    int n = 0;

    for (int i = 0; i < AHA_DB_COUNT; i++) {
        if (match_entry(&DB[i], ctx)) {
            candidates[n].index = i;
            candidates[n].depth = DB[i].depth;
            n++;
        }
    }

    /* Sort by depth descending */
    sort_candidates(candidates, n);

    /* Cap at AHA_MAX_RESULTS */
    int out_count = n < AHA_MAX_RESULTS ? n : AHA_MAX_RESULTS;
    for (int i = 0; i < out_count; i++) {
        result.indices[i] = candidates[i].index;
    }
    result.count = out_count;

    return result;
}
