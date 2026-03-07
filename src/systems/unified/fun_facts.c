/* fun_facts.c -- Fun Fact Generator implementation.
 *
 * Curated database of 85 cross-system fun facts with trigger conditions,
 * scoring algorithm, and seen-fact exclusion for rotation.
 *
 * Pure module: no globals, no malloc, no side effects. */

#include "fun_facts.h"

#include <string.h>

/* System bitmask constants for readability */
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

static const ff_fact_t FACTS[] = {
    /* === Cross-System (0-19, priority=3) === */
    { 0,
      "64 I Ching hexagrams = 64 Human Design gates = 64 DNA codons"
      " -- the universal binary: 2^6",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_ICHING | SYS_HUMAN_DESIGN, 3 },
    { 1,
      "12 zodiac signs = 12 Hebrew simple letters = 12 Chinese Earthly"
      " Branches -- the twelvefold division spans civilizations",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_ASTROLOGY | SYS_HEBREW | SYS_CHINESE, 3 },
    { 2,
      "7 classical planets = 7 days of the week = 7 chakras"
      " = 7 Hebrew double letters",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_ASTRONOMY | SYS_HEBREW, 3 },
    { 3,
      "22 Kabbalah paths = 22 Hebrew letters = 22 Major Arcana"
      " -- three systems, one structure",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_KABBALAH | SYS_HEBREW, 3 },
    { 4,
      "The Tzolkin Calendar Round (260 x 365 = 18,980 days) completes"
      " every 52 years -- a Mesoamerican century",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_VIEW_TZOLKIN,
      SYS_TZOLKIN, 3 },
    { 5,
      "A Buddhist kalpa and a geological eon are both roughly 4 billion"
      " years -- different continents, same cosmic scale",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_VIEW_DEEP_TIME,
      SYS_BUDDHIST | SYS_GEOLOGY, 3 },
    { 6,
      "27 Nakshatras x 4 padas = 108, sacred across Hinduism,"
      " Buddhism, and Jainism",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_HINDU | SYS_BUDDHIST, 3 },
    { 7,
      "The I Ching's 6-line structure maps exactly to Human Design's"
      " 64 gates -- each hexagram IS a gate",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_VIEW_ICHING,
      SYS_ICHING | SYS_HUMAN_DESIGN, 3 },
    { 8,
      "Both Kabbalah and Human Design have a tree-like structure:"
      " 10 Sefirot map to 9 HD Centers",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_KABBALAH | SYS_HUMAN_DESIGN, 3 },
    { 9,
      "The zodiac's 12 divisions appear independently in Chinese,"
      " Western, and Vedic astrology",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_ASTROLOGY | SYS_CHINESE | SYS_HINDU, 3 },
    { 10,
      "Venus traces a perfect pentagram in the sky over 8 years"
      " -- 5 synodic cycles of 583.9 days",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY | SYS_ASTROLOGY, 3 },
    { 11,
      "The golden ratio phi (1.618) appears in planetary orbital"
      " period ratios and spiral galaxy arms",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 3 },
    { 12,
      "Precession of the equinoxes takes 25,772 years"
      " -- the Platonic Great Year",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY | SYS_ASTROLOGY, 3 },
    { 13,
      "The Hebrew calendar's Metonic cycle: exactly 19 solar years"
      " = 235 lunar months",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_VIEW_CALENDAR,
      SYS_HEBREW | SYS_ASTRONOMY, 3 },
    { 14,
      "The Hindu tithi and Hebrew molad calculate lunar months to"
      " within 0.5 seconds of each other",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_HINDU | SYS_HEBREW, 3 },
    { 15,
      "360 degrees in a circle comes from the Babylonian base-60"
      " system -- and 360 is close to the days in a year",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_ASTRONOMY, 3 },
    { 16,
      "The Fibonacci sequence appears in sunflower seeds, pine cones,"
      " and spiral galaxy arms",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_ASTRONOMY, 3 },
    { 17,
      "The Egyptian civil calendar (365 days, no leap) drifted through"
      " all seasons over 1,461 years -- the Sothic cycle",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_ASTRONOMY, 3 },
    { 18,
      "Sirius's heliacal rising marked the Egyptian New Year"
      " and predicted the Nile flood",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_ALWAYS,
      SYS_ASTRONOMY, 3 },
    { 19,
      "The number 52 connects Mayan (52-year Calendar Round)"
      " and playing cards (52 cards = 52 weeks)",
      FF_CAT_CROSS_SYSTEM, FF_TRIG_VIEW_TZOLKIN,
      SYS_TZOLKIN, 3 },

    /* === Historical (20-34, priority=2) === */
    { 20,
      "In 1582, Pope Gregory XIII deleted 10 days"
      " -- October 4 was followed by October 15",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },
    { 21,
      "Julius Caesar introduced the Julian calendar in 45 BCE"
      " -- the year had 445 days to realign",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },
    { 22,
      "Ethiopia lives 7-8 years behind Gregorian time"
      " -- it is currently a different year there",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },
    { 23,
      "Russia did not adopt the Gregorian calendar until 1918"
      " -- their October Revolution actually happened in November",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },
    { 24,
      "The Islamic calendar is purely lunar: 354-355 days per year,"
      " so holidays migrate through all seasons",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_ISLAMIC, 2 },
    { 25,
      "The Hebrew calendar adds a 13th month (Adar II) seven times"
      " every 19 years to stay lunisolar",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_HEBREW, 2 },
    { 26,
      "Japan adopted the Gregorian calendar in 1873 but still"
      " counts years by imperial era",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },
    { 27,
      "The French Revolutionary Calendar (1793-1805) had 10-day"
      " weeks -- workers hated it",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },
    { 28,
      "Easter's date requires both solar and lunar calculations"
      " -- the most complex date in the Gregorian calendar",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },
    { 29,
      "The Mayan Long Count began on August 11, 3114 BCE"
      " (GMT correlation 584283)",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_TZOLKIN,
      SYS_TZOLKIN, 2 },
    { 30,
      "The Chinese calendar predates the Gregorian by ~2,000 years"
      " and is still used for festivals",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_CHINESE, 2 },
    { 31,
      "The Buddhist calendar counts from 543 BCE"
      " -- it is currently in the 2560s BE",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_BUDDHIST, 2 },
    { 32,
      "The Coptic calendar descends directly from ancient Egypt"
      " -- the oldest solar calendar still in use",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },
    { 33,
      "The Persian Nowruz has been celebrated at the spring equinox"
      " for over 3,000 years",
      FF_CAT_HISTORICAL, FF_TRIG_ALWAYS,
      SYS_ASTRONOMY, 2 },
    { 34,
      "Zoroastrian months have 30 days each, with 5 Gatha days"
      " at year's end -- no leap year adjustment",
      FF_CAT_HISTORICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },

    /* === Astronomical (35-49, priority=2) === */
    { 35,
      "The moon is receding from Earth at 3.8 cm per year"
      " -- in 600 million years, no more total eclipses",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY | SYS_EARTH, 2 },
    { 36,
      "Jupiter's orbital period is 11.86 years"
      " -- close to the 12-year Chinese zodiac cycle",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY | SYS_CHINESE, 2 },
    { 37,
      "Saturn return (29.5 years) is both an astrological milestone"
      " and roughly one human generation",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY | SYS_ASTROLOGY, 2 },
    { 38,
      "The sun completes one galactic orbit every 225-250 million"
      " years -- a cosmic year",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_ASTRONOMY | SYS_GEOLOGY, 2 },
    { 39,
      "Mercury's orbit is so elliptical that from its surface,"
      " the sun would appear to reverse direction",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 2 },
    { 40,
      "The Tropic of Cancer got its name when the sun entered Cancer"
      " at summer solstice -- precession has shifted it to Gemini",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY | SYS_ASTROLOGY, 2 },
    { 41,
      "A day on Venus (243 Earth days) is longer than a year on"
      " Venus (225 Earth days)",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 2 },
    { 42,
      "The Saros cycle: eclipses repeat every 18 years, 11 days,"
      " 8 hours -- known since Babylon",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 2 },
    { 43,
      "Light from the sun takes 8 minutes 20 seconds to reach Earth"
      " -- you always see the sun as it was",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 2 },
    { 44,
      "The North Star changes over millennia due to precession"
      " -- Thuban was the pole star when pyramids were built",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 2 },
    { 45,
      "Earth's axial tilt (23.44 degrees) gives us seasons"
      " -- without it, no winter, no summer",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_EARTH,
      SYS_ASTRONOMY | SYS_EARTH, 2 },
    { 46,
      "The Drake Equation estimates communicating civilizations"
      " in our galaxy -- every variable is uncertain",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 2 },
    { 47,
      "Halley's Comet returns every 75-79 years"
      " -- Mark Twain was born and died during its appearances",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 2 },
    { 48,
      "The cosmic microwave background radiation is the afterglow"
      " of the Big Bang -- it is everywhere, always",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_ASTRONOMY, 2 },
    { 49,
      "Neutron stars spin up to 716 times per second"
      " -- faster than a kitchen blender",
      FF_CAT_ASTRONOMICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 2 },

    /* === Mathematical (50-59, priority=2) === */
    { 50,
      "Phi (the golden ratio) = (1+sqrt(5))/2 = 1.6180339..."
      " -- it appears in art, architecture, and nature",
      FF_CAT_MATHEMATICAL, FF_TRIG_ALWAYS,
      SYS_ASTRONOMY, 2 },
    { 51,
      "The golden angle (137.508 degrees) is how sunflowers pack"
      " seeds -- the most efficient packing in nature",
      FF_CAT_MATHEMATICAL, FF_TRIG_ALWAYS,
      SYS_ASTRONOMY, 2 },
    { 52,
      "Pi appears in the formula for Kepler's third law"
      " -- connecting orbital periods to distances",
      FF_CAT_MATHEMATICAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 2 },
    { 53,
      "The Gregorian leap year rule: divisible by 4, except"
      " centuries, except 400s -- 97 leap years per 400",
      FF_CAT_MATHEMATICAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 2 },
    { 54,
      "Base 60 (Babylonian) gave us 60 seconds, 60 minutes,"
      " 360 degrees -- and it divides evenly many ways",
      FF_CAT_MATHEMATICAL, FF_TRIG_ALWAYS,
      SYS_ASTRONOMY, 2 },
    { 55,
      "The Mayan vigesimal (base-20) system used dots and bars"
      " -- 0 was a shell glyph",
      FF_CAT_MATHEMATICAL, FF_TRIG_VIEW_TZOLKIN,
      SYS_TZOLKIN, 2 },
    { 56,
      "The Hebrew gematria assigns numerical values to letters"
      " -- every word IS a number",
      FF_CAT_MATHEMATICAL, FF_TRIG_ALWAYS,
      SYS_HEBREW | SYS_KABBALAH, 2 },
    { 57,
      "108 = 27 Nakshatras x 4 padas = number of beads on a mala"
      " -- sacred in Hinduism, Buddhism, Jainism",
      FF_CAT_MATHEMATICAL, FF_TRIG_ALWAYS,
      SYS_HINDU | SYS_BUDDHIST, 2 },
    { 58,
      "The I Ching's binary encoding (yin=0, yang=1) predates"
      " Leibniz's formal binary by 3,000+ years",
      FF_CAT_MATHEMATICAL, FF_TRIG_VIEW_ICHING,
      SYS_ICHING, 2 },
    { 59,
      "The Maya independently invented the concept of zero"
      " -- one of only three civilizations to do so",
      FF_CAT_MATHEMATICAL, FF_TRIG_VIEW_TZOLKIN,
      SYS_TZOLKIN, 2 },

    /* === Geological (60-69, priority=2) === */
    { 60,
      "Earth is 4.54 billion years old -- if compressed to 24 hours,"
      " humans arrive 5.7 seconds before midnight",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY | SYS_EARTH, 2 },
    { 61,
      "The Great Oxidation Event (2.4 Ga) was the largest mass"
      " extinction ever -- but enabled complex life",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY, 2 },
    { 62,
      "Antarctica was covered in tropical forests 50 million years"
      " ago -- tectonic drift moved it south",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY | SYS_EARTH, 2 },
    { 63,
      "The oldest known rocks on Earth are 4.03 billion years old"
      " -- the Acasta Gneiss in Canada",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY, 2 },
    { 64,
      "Pangaea began breaking apart 200 million years ago"
      " -- the continents are still moving at ~2.5 cm/year",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY | SYS_EARTH, 2 },
    { 65,
      "The Chicxulub asteroid that killed the dinosaurs (66 Ma)"
      " left a crater 180 km wide in Mexico",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY, 2 },
    { 66,
      "The Cambrian Explosion (541 Ma) saw most major animal phyla"
      " appear within ~20 million years",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY, 2 },
    { 67,
      "Earth's magnetic field has reversed hundreds of times"
      " -- the last reversal was 780,000 years ago",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY | SYS_EARTH, 2 },
    { 68,
      "Snowball Earth events (715-635 Ma) covered the entire planet"
      " in ice -- possibly reaching the equator",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY | SYS_EARTH, 2 },
    { 69,
      "If Earth's history were a 1,000-page book, all recorded human"
      " history fits on the last word of the last page",
      FF_CAT_GEOLOGICAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_GEOLOGY, 2 },

    /* === Cultural (70-84, priority=1) === */
    { 70,
      "Dreamspell: 'Time is not money. Time is art.'"
      " -- Jose Arguelles reimagined the Mayan calendar for modern use",
      FF_CAT_CULTURAL, FF_TRIG_VIEW_TZOLKIN,
      SYS_TZOLKIN, 1 },
    { 71,
      "The Chinese zodiac animal cycle takes 12 years"
      " -- matching Jupiter's orbital period around the sun",
      FF_CAT_CULTURAL, FF_TRIG_ALWAYS,
      SYS_CHINESE | SYS_ASTRONOMY, 1 },
    { 72,
      "In Hindu cosmology, Brahma's day (kalpa) is 4.32 billion years"
      " -- remarkably close to Earth's actual age",
      FF_CAT_CULTURAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_HINDU | SYS_GEOLOGY, 1 },
    { 73,
      "The Tree of Life in Kabbalah has 10 Sefirot connected by"
      " 22 paths -- mirroring the 22 Hebrew letters",
      FF_CAT_CULTURAL, FF_TRIG_ALWAYS,
      SYS_KABBALAH | SYS_HEBREW, 1 },
    { 74,
      "Human Design synthesis: I Ching hexagrams + astrology"
      " + Kabbalah + chakras + quantum physics",
      FF_CAT_CULTURAL, FF_TRIG_ALWAYS,
      SYS_HUMAN_DESIGN | SYS_ICHING | SYS_ASTROLOGY | SYS_KABBALAH, 1 },
    { 75,
      "The Dreamspell assigns one of 20 solar seals and 13 tones"
      " to each day -- 260 unique combinations",
      FF_CAT_CULTURAL, FF_TRIG_VIEW_TZOLKIN,
      SYS_TZOLKIN, 1 },
    { 76,
      "Buddhist Uposatha observance days align with moon phases"
      " -- full moon and new moon are sacred",
      FF_CAT_CULTURAL, FF_TRIG_ALWAYS,
      SYS_BUDDHIST | SYS_ASTRONOMY, 1 },
    { 77,
      "The Islamic Friday prayer (Jumu'ah) + Jewish Shabbat eve"
      " can overlap -- a shared day of rest",
      FF_CAT_CULTURAL, FF_TRIG_ALWAYS,
      SYS_ISLAMIC | SYS_HEBREW, 1 },
    { 78,
      "Ancient Egyptians divided night into 12 hours using 36 decans"
      " -- star groups that rise every 10 days",
      FF_CAT_CULTURAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 1 },
    { 79,
      "The Yuga cycle (Hindu) maps cosmic ages to declining virtue"
      " -- we are in Kali Yuga, the age of conflict",
      FF_CAT_CULTURAL, FF_TRIG_VIEW_DEEP_TIME,
      SYS_HINDU, 1 },
    { 80,
      "Shabbat begins at sunset Friday -- the Hebrew day starts at"
      " evening, not midnight",
      FF_CAT_CULTURAL, FF_TRIG_ALWAYS,
      SYS_HEBREW, 1 },
    { 81,
      "The Aztec Sun Stone is NOT a calendar -- it is a sacrificial"
      " altar depicting the five cosmic ages",
      FF_CAT_CULTURAL, FF_TRIG_VIEW_TZOLKIN,
      SYS_TZOLKIN, 1 },
    { 82,
      "Stonehenge aligns with the summer solstice sunrise and"
      " winter solstice sunset",
      FF_CAT_CULTURAL, FF_TRIG_VIEW_EARTH,
      SYS_ASTRONOMY | SYS_EARTH, 1 },
    { 83,
      "The oldest known star catalog was created by Hipparchus"
      " around 129 BCE -- 850 stars",
      FF_CAT_CULTURAL, FF_TRIG_VIEW_SPACE,
      SYS_ASTRONOMY, 1 },
    { 84,
      "The word 'calendar' comes from Latin 'kalendae' (calends)"
      " -- the first day of each Roman month",
      FF_CAT_CULTURAL, FF_TRIG_VIEW_CALENDAR,
      SYS_GREGORIAN, 1 },
};

static const int FACT_COUNT = (int)(sizeof(FACTS) / sizeof(FACTS[0]));

/* Count bits set in a 32-bit unsigned integer */
static int popcount_u32(unsigned int x)
{
    int count = 0;
    while (x) {
        count += (int)(x & 1u);
        x >>= 1;
    }
    return count;
}

/* Check if a fact index is in the seen array */
static int is_seen(int index, const int *seen, int seen_count)
{
    if (!seen || seen_count <= 0) {
        return 0;
    }
    for (int i = 0; i < seen_count; i++) {
        if (seen[i] == index) {
            return 1;
        }
    }
    return 0;
}

/* Map view integer to trigger enum */
static ff_trigger_t view_to_trigger(int view)
{
    switch (view) {
    case 0: return FF_TRIG_VIEW_SPACE;
    case 1: return FF_TRIG_VIEW_EARTH;
    case 2: return FF_TRIG_VIEW_TZOLKIN;
    case 3: return FF_TRIG_VIEW_ICHING;
    case 4: return FF_TRIG_VIEW_CALENDAR;
    case 5: return FF_TRIG_VIEW_DEEP_TIME;
    default: return FF_TRIG_ALWAYS;
    }
}

/* Score a single fact against a context */
static int score_fact(const ff_fact_t *fact, const ff_context_t *ctx)
{
    int score = fact->priority;

    /* Trigger match: +3 if trigger matches current view/state */
    ff_trigger_t view_trig = view_to_trigger(ctx->view);
    if (fact->trigger == view_trig && fact->trigger != FF_TRIG_ALWAYS) {
        score += 3;
    }

    /* ALWAYS-trigger facts get +1 bonus */
    if (fact->trigger == FF_TRIG_ALWAYS) {
        score += 1;
    }

    /* System overlap: +1 per active system that overlaps */
    if (ctx->active_systems != 0 && fact->system_mask != 0) {
        unsigned int overlap = ctx->active_systems & fact->system_mask;
        score += popcount_u32(overlap);
    }

    /* Scale bonus: +2 if scale matches category */
    /* scale: 0=human, 1=historical, 2=geological, 3=cosmic */
    if (ctx->scale == 2 && fact->category == FF_CAT_GEOLOGICAL) {
        score += 2;
    }
    if (ctx->scale == 3 && fact->category == FF_CAT_ASTRONOMICAL) {
        score += 2;
    }
    if (ctx->scale == 1 && fact->category == FF_CAT_HISTORICAL) {
        score += 2;
    }

    /* Convergence bonus: +2 if convergence_active and trigger is convergence */
    if (ctx->convergence_active && fact->trigger == FF_TRIG_CONVERGENCE) {
        score += 2;
    }

    return score;
}

int ff_fact_count(void)
{
    return FACT_COUNT;
}

ff_fact_t ff_fact_get(int index)
{
    if (index < 0 || index >= FACT_COUNT) {
        ff_fact_t empty = { -1, "", FF_CAT_CROSS_SYSTEM,
                            FF_TRIG_ALWAYS, 0, 0 };
        return empty;
    }
    return FACTS[index];
}

int ff_by_category(ff_category_t cat, int *out, int max_out)
{
    if (!out || max_out <= 0 || cat < 0 || cat >= FF_CAT_COUNT) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < FACT_COUNT && count < max_out; i++) {
        if (FACTS[i].category == cat) {
            out[count++] = i;
        }
    }
    return count;
}

ff_selection_t ff_evaluate(const ff_context_t *ctx,
                           const int *seen, int seen_count)
{
    ff_selection_t sel;
    sel.count = 0;

    if (!ctx) {
        return sel;
    }

    /* Score all non-seen facts into a temporary buffer */
    ff_result_t candidates[256];
    int n_candidates = 0;

    for (int i = 0; i < FACT_COUNT && i < 256; i++) {
        if (is_seen(i, seen, seen_count)) {
            continue;
        }
        int s = score_fact(&FACTS[i], ctx);
        candidates[n_candidates].fact_index = i;
        candidates[n_candidates].score = s;
        n_candidates++;
    }

    /* Insertion sort by score descending (stable for small N) */
    for (int i = 1; i < n_candidates; i++) {
        ff_result_t key = candidates[i];
        int j = i - 1;
        while (j >= 0 && candidates[j].score < key.score) {
            candidates[j + 1] = candidates[j];
            j--;
        }
        candidates[j + 1] = key;
    }

    /* Copy top FF_MAX_RESULTS */
    int limit = n_candidates < FF_MAX_RESULTS ? n_candidates : FF_MAX_RESULTS;
    for (int i = 0; i < limit; i++) {
        sel.results[i] = candidates[i];
    }
    sel.count = limit;

    return sel;
}

int ff_best_fact(const ff_context_t *ctx, const int *seen, int seen_count)
{
    ff_selection_t sel = ff_evaluate(ctx, seen, seen_count);
    if (sel.count == 0) {
        return -1;
    }
    return sel.results[0].fact_index;
}

const char *ff_category_name(ff_category_t cat)
{
    switch (cat) {
    case FF_CAT_CROSS_SYSTEM: return "Cross-System";
    case FF_CAT_HISTORICAL:   return "Historical";
    case FF_CAT_ASTRONOMICAL: return "Astronomical";
    case FF_CAT_MATHEMATICAL: return "Mathematical";
    case FF_CAT_GEOLOGICAL:   return "Geological";
    case FF_CAT_CULTURAL:     return "Cultural";
    default:                  return "Unknown";
    }
}

const char *ff_trigger_name(ff_trigger_t trig)
{
    switch (trig) {
    case FF_TRIG_ALWAYS:       return "Always";
    case FF_TRIG_VIEW_SPACE:   return "View: Space";
    case FF_TRIG_VIEW_EARTH:   return "View: Earth";
    case FF_TRIG_VIEW_TZOLKIN: return "View: Tzolkin";
    case FF_TRIG_VIEW_ICHING:  return "View: I Ching";
    case FF_TRIG_VIEW_CALENDAR: return "View: Calendar";
    case FF_TRIG_VIEW_DEEP_TIME: return "View: Deep Time";
    case FF_TRIG_CONVERGENCE:  return "Convergence";
    case FF_TRIG_SYSTEM_ACTIVE: return "System Active";
    default:                   return "Unknown";
    }
}

ff_context_t ff_context_default(void)
{
    ff_context_t ctx;
    ctx.view = 0;
    ctx.scale = 0;
    ctx.active_systems = 0;
    ctx.convergence_active = 0;
    ctx.user_seal = -1;
    return ctx;
}
