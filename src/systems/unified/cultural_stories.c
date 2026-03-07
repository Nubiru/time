/* cultural_stories.c -- Cultural Story Database.
 *
 * 36 stories spanning 12 cultures: Norse, Greek, Roman, Indian, Chinese,
 * Buddhist, Celtic, Egyptian, Mayan, Hebrew, Islamic, and Persian.
 *
 * All functions are pure: no globals, no malloc, no side effects.
 * Case-insensitive search uses inline tolower (no <ctype.h>). */

#include "cultural_stories.h"

#include <string.h>

/* ---------- helpers ---------- */

/* Inline lowercase for ASCII a-z only (no <ctype.h>). */
static char cs_tolower(char c)
{
    if (c >= 'A' && c <= 'Z') return (char)(c + ('a' - 'A'));
    return c;
}

/* Case-insensitive substring search. */
static int cs_ci_contains(const char *haystack, const char *needle)
{
    if (haystack == NULL || needle == NULL) return 0;
    int hlen = (int)strlen(haystack);
    int nlen = (int)strlen(needle);
    if (nlen == 0) return 1;
    if (nlen > hlen) return 0;
    for (int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (int j = 0; j < nlen; j++) {
            if (cs_tolower(haystack[i + j]) != cs_tolower(needle[j])) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

/* Case-insensitive exact string compare. */
static int cs_ci_equals(const char *a, const char *b)
{
    if (a == NULL || b == NULL) return 0;
    int la = (int)strlen(a);
    int lb = (int)strlen(b);
    if (la != lb) return 0;
    for (int i = 0; i < la; i++) {
        if (cs_tolower(a[i]) != cs_tolower(b[i])) return 0;
    }
    return 1;
}

/* ---------- story database ---------- */

static const cs_story_t STORIES[] = {

    /* ===== NORSE (4 stories, indices 0-3) ===== */
    { 0, "Norse", "Yggdrasil -- The World Tree",
      "In Norse cosmology, Yggdrasil is the immense ash tree that connects "
      "nine worlds and serves as the axis of all existence. Its roots reach "
      "into the wells of wisdom, fate, and the underworld. The tree is tended "
      "by the Norns, who water it daily from the Well of Urd.",
      "Pre-Viking Age (~800 BCE)", CS_CAT_ORIGIN_MYTH,
      { CS_SYSTEM_GREGORIAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 1, "Norse", "Ragnarok -- Twilight of the Gods",
      "Ragnarok is the prophesied destruction and rebirth of the cosmos. "
      "The gods fall in battle against the forces of chaos, the world is "
      "consumed by fire and flood, then rises anew from the sea. Two human "
      "survivors repopulate a renewed, green earth.",
      "Pre-Viking Age (~800 BCE)", CS_CAT_COSMIC_CYCLE,
      { CS_SYSTEM_ASTRONOMY, CS_SYSTEM_GREGORIAN, 0, 0 }, 2 },

    { 2, "Norse", "The Norns -- Weavers of Fate",
      "Urd (past), Verdandi (present), and Skuld (future) are three female "
      "figures who dwell at the Well of Urd beneath Yggdrasil. They carve "
      "runes into the tree's trunk, determining the fate of gods and mortals. "
      "Their weaving represents time as an interconnected fabric, not a line.",
      "Pre-Viking Age (~800 BCE)", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_GREGORIAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 3, "Norse", "Weekday Names -- Gods in Our Calendar",
      "Tuesday (Tyr), Wednesday (Odin/Woden), Thursday (Thor), and Friday "
      "(Frigg) embed Norse deities into the Gregorian week. Saturday, Sunday, "
      "and Monday derive from Roman and celestial sources. This linguistic "
      "fossil reveals how deeply Norse cosmology shaped Western timekeeping.",
      "Germanic Iron Age (~200 CE)", CS_CAT_TIME_PHILOSOPHY,
      { CS_SYSTEM_GREGORIAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    /* ===== GREEK (4 stories, indices 4-7) ===== */
    { 4, "Greek", "Chronos vs Kairos -- Two Faces of Time",
      "The Greeks distinguished Chronos (sequential, measurable time) from "
      "Kairos (the opportune, qualitative moment). Chronos is the ticking "
      "clock; Kairos is the archer releasing at the perfect instant. Together "
      "they reveal that time has both quantity and quality.",
      "Classical Greece (~500 BCE)", CS_CAT_TIME_PHILOSOPHY,
      { CS_SYSTEM_ASTRONOMY, CS_SYSTEM_ASTROLOGY, 0, 0 }, 2 },

    { 5, "Greek", "Plato's Timaeus -- Time as Moving Image of Eternity",
      "In the Timaeus, Plato describes the Demiurge creating time as a "
      "moving image of eternity, structured by the celestial rotations. The "
      "planets become the instruments of time, each orbit a measure. This "
      "dialogue established the philosophical link between astronomy and time.",
      "Classical Greece (~360 BCE)", CS_CAT_ORIGIN_MYTH,
      { CS_SYSTEM_ASTRONOMY, CS_SYSTEM_ASTROLOGY, 0, 0 }, 2 },

    { 6, "Greek", "Pythagoras -- Harmony of the Spheres",
      "Pythagoras taught that planetary orbits produce a celestial music "
      "inaudible to human ears, each sphere emitting a tone proportional "
      "to its distance and speed. This concept united mathematics, astronomy, "
      "and aesthetics into a single vision of cosmic order.",
      "Classical Greece (~530 BCE)", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_ASTRONOMY, CS_SYSTEM_ASTROLOGY, 0, 0 }, 2 },

    { 7, "Greek", "Aristarchus -- The First Heliocentric Model",
      "Around 270 BCE, Aristarchus of Samos proposed that the Earth revolves "
      "around the Sun, anticipating Copernicus by eighteen centuries. He also "
      "estimated the relative sizes of the Sun and Moon. His work was rejected "
      "in his time but proved prescient.",
      "Hellenistic Period (~270 BCE)", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_ASTRONOMY, CS_SYSTEM_ASTROLOGY, 0, 0 }, 2 },

    /* ===== ROMAN (3 stories, indices 8-10) ===== */
    { 8, "Roman", "Caesar's Calendar Reform -- The 445-Day Year",
      "In 46 BCE, Julius Caesar reformed the chaotic Roman calendar by adding "
      "67 extra days, creating a 445-day 'Year of Confusion.' The resulting "
      "Julian calendar introduced the 365.25-day year with a leap day every "
      "four years, serving Western civilization for over 1,600 years.",
      "Late Republic (46 BCE)", CS_CAT_CALENDAR_REFORM,
      { CS_SYSTEM_GREGORIAN, 0, 0, 0 }, 1 },

    { 9, "Roman", "July and August -- Ego in the Calendar",
      "The months Quintilis and Sextilis were renamed July (for Julius Caesar) "
      "and August (for Augustus Caesar), embedding imperial ego into the "
      "calendar itself. Augustus reportedly added a day to 'his' month so it "
      "would not be shorter than Julius's.",
      "Early Empire (~8 BCE)", CS_CAT_CALENDAR_REFORM,
      { CS_SYSTEM_GREGORIAN, 0, 0, 0 }, 1 },

    { 10, "Roman", "Pax Romana -- Two Centuries of Stability",
      "From 27 BCE to 180 CE, the Roman Empire experienced an unprecedented "
      "era of peace and prosperity. During this period, standardized "
      "timekeeping, road networks, and administrative calendars unified a "
      "vast territory from Britain to Egypt.",
      "Early Empire (27 BCE - 180 CE)", CS_CAT_GOLDEN_AGE,
      { CS_SYSTEM_GREGORIAN, 0, 0, 0 }, 1 },

    /* ===== INDIAN (4 stories, indices 11-14) ===== */
    { 11, "Indian", "Dance of Shiva -- Nataraja as Cosmic Rhythm",
      "Shiva Nataraja dances within a ring of fire, his movements creating "
      "and destroying the universe in endless cycles. The drum in his right "
      "hand beats the rhythm of creation; the flame in his left hand "
      "represents dissolution. The dance is time itself.",
      "Chola Dynasty (~10th century CE)", CS_CAT_ORIGIN_MYTH,
      { CS_SYSTEM_HINDU, CS_SYSTEM_BUDDHIST, CS_SYSTEM_ASTROLOGY, 0 }, 3 },

    { 12, "Indian", "Aryabhata -- Earth Rotation and Pi",
      "In the 5th century CE, Aryabhata declared that the Earth rotates on "
      "its axis, explaining the apparent motion of stars. He calculated Pi "
      "to 3.1416 and developed a sine table fundamental to Indian astronomy. "
      "His Aryabhatiya influenced both Islamic and European mathematics.",
      "Gupta Empire (499 CE)", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_HINDU, CS_SYSTEM_ASTRONOMY, CS_SYSTEM_ASTROLOGY, 0 }, 3 },

    { 13, "Indian", "Yuga Cycle -- 4.32 Million Years of Cosmic Time",
      "Hindu cosmology divides cosmic time into four Yugas totaling 4.32 "
      "million years: Satya (golden), Treta, Dvapara, and Kali (dark). Each "
      "Yuga marks progressive decline in dharma. We currently inhabit the "
      "Kali Yuga, the age of strife, which began around 3102 BCE.",
      "Vedic Period (~1500 BCE)", CS_CAT_COSMIC_CYCLE,
      { CS_SYSTEM_HINDU, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 14, "Indian", "Vedic Golden Age -- The Surya Siddhanta",
      "The Surya Siddhanta, a foundational text of Indian astronomy, "
      "describes planetary motions with remarkable accuracy. Attributed to "
      "a divine revelation from the Sun god, it calculates the sidereal year "
      "to within seconds of modern values and influenced all subsequent "
      "Indian astronomical traditions.",
      "Gupta Period (~400 CE)", CS_CAT_GOLDEN_AGE,
      { CS_SYSTEM_HINDU, CS_SYSTEM_ASTRONOMY, CS_SYSTEM_ASTROLOGY, 0 }, 3 },

    /* ===== CHINESE (4 stories, indices 15-18) ===== */
    { 15, "Chinese", "Mandate of Heaven -- Calendar as Legitimacy",
      "In Chinese political philosophy, an accurate calendar proved the "
      "emperor's connection to cosmic order. Predicting eclipses and solstices "
      "demonstrated the Mandate of Heaven. A failed prediction could signal "
      "that heaven had withdrawn its favor from the ruling dynasty.",
      "Zhou Dynasty (~1046 BCE)", CS_CAT_TIME_PHILOSOPHY,
      { CS_SYSTEM_CHINESE, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 16, "Chinese", "I Ching -- The Cosmic Computer",
      "The Book of Changes contains 64 hexagrams, each a six-line binary "
      "figure representing a state of cosmic flux. Used for over 3,000 years "
      "as a divination and philosophical system, the I Ching maps all "
      "possible transformations of yin and yang through time.",
      "Western Zhou (~1000 BCE)", CS_CAT_ORIGIN_MYTH,
      { CS_SYSTEM_CHINESE, CS_SYSTEM_ICHING, 0, 0 }, 2 },

    { 17, "Chinese", "Sima Qian -- The Grand Historian",
      "Sima Qian (145-86 BCE) wrote the Shiji, a comprehensive history of "
      "China spanning 2,500 years. He established the model of dynastic "
      "history and integrated astronomical records with political events. "
      "He endured castration rather than abandon his life's work.",
      "Han Dynasty (~94 BCE)", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_CHINESE, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 18, "Chinese", "Dynasty Calendar Reforms",
      "Each new Chinese dynasty typically issued a new calendar to demonstrate "
      "celestial mandate. Over 100 calendar reforms occurred across 2,000 "
      "years, each refining lunar and solar calculations. The Shoushi calendar "
      "of 1281 achieved accuracy rivaling Gregorian reform three centuries later.",
      "Multiple Dynasties (~200 BCE - 1644 CE)", CS_CAT_CALENDAR_REFORM,
      { CS_SYSTEM_CHINESE, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    /* ===== BUDDHIST (4 stories, indices 19-22) ===== */
    { 19, "Buddhist", "Anicca -- The Law of Impermanence",
      "The Buddha taught that all conditioned phenomena are impermanent "
      "(anicca). Nothing that arises fails to cease. This insight transforms "
      "the experience of time from something to grasp into something to "
      "observe with equanimity and wisdom.",
      "5th century BCE", CS_CAT_TIME_PHILOSOPHY,
      { CS_SYSTEM_BUDDHIST, CS_SYSTEM_MYANMAR, 0, 0 }, 2 },

    { 20, "Buddhist", "Kalpas -- Cosmic Time Beyond Measure",
      "A kalpa is a vast unit of cosmic time in Buddhist cosmology. The "
      "Buddha compared it to the time it would take to wear away a mountain "
      "by brushing it with a silk cloth once per century. Worlds arise and "
      "dissolve through countless kalpas in endless succession.",
      "5th century BCE", CS_CAT_COSMIC_CYCLE,
      { CS_SYSTEM_BUDDHIST, CS_SYSTEM_MYANMAR, 0, 0 }, 2 },

    { 21, "Buddhist", "Siddhartha Gautama -- The Present Moment",
      "The historical Buddha, born Siddhartha Gautama around 563 BCE, taught "
      "that the present moment is the only reality. Past is memory, future "
      "is projection. His meditation practices train attention on immediate "
      "experience, dissolving the illusion of linear time.",
      "5th century BCE", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_BUDDHIST, CS_SYSTEM_MYANMAR, 0, 0 }, 2 },

    { 22, "Buddhist", "Uposatha -- Lunar Observance Days",
      "Buddhist communities observe Uposatha on the new moon, full moon, and "
      "quarter-moon days of the lunar cycle. Laypeople undertake additional "
      "precepts and monastics recite the Patimokkha. This practice weaves "
      "spiritual discipline into the rhythm of the Moon.",
      "5th century BCE onward", CS_CAT_COSMIC_CYCLE,
      { CS_SYSTEM_BUDDHIST, CS_SYSTEM_MYANMAR, CS_SYSTEM_ASTRONOMY, 0 }, 3 },

    /* ===== CELTIC (3 stories, indices 23-25) ===== */
    { 23, "Celtic", "Newgrange -- 5,000-Year-Old Solstice Marker",
      "Built around 3200 BCE in Ireland, Newgrange is a passage tomb aligned "
      "so that sunlight floods its inner chamber for 17 minutes at the winter "
      "solstice dawn. Older than Stonehenge and the Pyramids, it demonstrates "
      "Neolithic mastery of solar astronomy.",
      "Neolithic (~3200 BCE)", CS_CAT_ORIGIN_MYTH,
      { CS_SYSTEM_CELTIC, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 24, "Celtic", "Thin Places -- Samhain and Beltane",
      "The Celts believed certain times were 'thin,' when the boundary "
      "between worlds became permeable. Samhain (October 31) marked the "
      "year's end and honored the dead; Beltane (May 1) celebrated fertility "
      "and renewal. Both were fire festivals aligning to cross-quarter days.",
      "Iron Age (~500 BCE)", CS_CAT_TIME_PHILOSOPHY,
      { CS_SYSTEM_CELTIC, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 25, "Celtic", "Wheel of the Year -- Eight Festivals",
      "The Celtic year was structured around eight festivals: four solar "
      "events (solstices and equinoxes) and four cross-quarter fire festivals "
      "(Samhain, Imbolc, Beltane, Lughnasadh). This eightfold wheel reflects "
      "agricultural, astronomical, and spiritual cycles in unity.",
      "Iron Age (~500 BCE)", CS_CAT_COSMIC_CYCLE,
      { CS_SYSTEM_CELTIC, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    /* ===== EGYPTIAN (4 stories, indices 26-29) ===== */
    { 26, "Egyptian", "Ma'at -- Cosmic Order and Truth",
      "Ma'at was both a goddess and a principle: the cosmic order that "
      "maintained truth, justice, balance, and the regularity of the seasons. "
      "The pharaoh's primary duty was to uphold Ma'at. Without it, the Nile "
      "would not flood, crops would fail, and chaos would reign.",
      "Old Kingdom (~2600 BCE)", CS_CAT_ORIGIN_MYTH,
      { CS_SYSTEM_EGYPTIAN, CS_SYSTEM_COPTIC, CS_SYSTEM_ASTRONOMY, 0 }, 3 },

    { 27, "Egyptian", "Sothic Cycle -- 1,461 Years of Sirius",
      "The Egyptian civil calendar of 365 days drifted one day every four "
      "years against the heliacal rising of Sirius (Sopdet). After 1,461 "
      "years, the calendar realigned with the star. This Sothic cycle was "
      "the longest recognized astronomical period in the ancient world.",
      "Old Kingdom (~2800 BCE)", CS_CAT_COSMIC_CYCLE,
      { CS_SYSTEM_EGYPTIAN, CS_SYSTEM_COPTIC, CS_SYSTEM_ASTRONOMY, 0 }, 3 },

    { 28, "Egyptian", "Imhotep -- Architect, Priest, Astronomer",
      "Imhotep served Pharaoh Djoser around 2650 BCE as architect of the "
      "first pyramid, as high priest, and as keeper of astronomical records. "
      "He was later deified as a god of wisdom and medicine. His integration "
      "of building, ritual, and celestial observation defined Egyptian culture.",
      "Old Kingdom (~2650 BCE)", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_EGYPTIAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 29, "Egyptian", "Old Kingdom -- Age of Pyramid Builders",
      "The Old Kingdom (2686-2181 BCE) saw the construction of the Great "
      "Pyramids, aligned with extraordinary precision to cardinal directions "
      "and stellar positions. This era's astronomical knowledge, mathematical "
      "sophistication, and monumental architecture remain unparalleled in "
      "antiquity.",
      "Old Kingdom (2686-2181 BCE)", CS_CAT_GOLDEN_AGE,
      { CS_SYSTEM_EGYPTIAN, CS_SYSTEM_COPTIC, CS_SYSTEM_ASTRONOMY, 0 }, 3 },

    /* ===== MAYAN (4 stories, indices 30-33) ===== */
    { 30, "Mayan", "Popol Vuh -- The Hero Twins",
      "The Popol Vuh tells of the Hero Twins, Hunahpu and Xbalanque, who "
      "descend into Xibalba (the underworld), defeat the Lords of Death "
      "through cunning, and rise as the Sun and Moon. This origin narrative "
      "encodes the cycles of maize, death, and celestial rebirth.",
      "Classic Period (~200-900 CE)", CS_CAT_ORIGIN_MYTH,
      { CS_SYSTEM_MAYAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 31, "Mayan", "Long Count -- 5,125 Years of Time",
      "The Mayan Long Count calendar measures time from a mythical creation "
      "date (August 11, 3114 BCE in the GMT correlation). One Great Cycle "
      "spans 13 Baktuns or about 5,125 years. The calendar's completion on "
      "December 21, 2012 marked a new cycle, not an apocalypse.",
      "Preclassic Period (~400 BCE)", CS_CAT_COSMIC_CYCLE,
      { CS_SYSTEM_MAYAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 32, "Mayan", "Each Day a Living Entity",
      "In Mayan time philosophy, each day carries its own character, energy, "
      "and purpose through the Tzolkin's 20 day signs and 13 tones. Days are "
      "not empty containers to be filled but living beings that bring specific "
      "qualities. Time is not a resource to spend but an art to live.",
      "Classic Period (~200-900 CE)", CS_CAT_TIME_PHILOSOPHY,
      { CS_SYSTEM_MAYAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 33, "Mayan", "Pacal the Great of Palenque",
      "K'inich Janaab Pakal ruled Palenque for 68 years (615-683 CE), the "
      "longest reign in the Americas. His tomb lid depicts his descent into "
      "the underworld along the World Tree. The astronomical alignments of "
      "his Temple of Inscriptions encode sophisticated calendar knowledge.",
      "Classic Period (615-683 CE)", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_MAYAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    /* ===== HEBREW (2 stories, indices 34-35) ===== */
    { 34, "Hebrew", "Sabbath -- Sacred Architecture of Time",
      "The Hebrew Sabbath (Shabbat) sanctifies the seventh day as a "
      "cathedral in time, not space. Abraham Joshua Heschel called it 'a "
      "palace in time.' This weekly rhythm of work and rest structures Jewish "
      "life and influenced the global adoption of the seven-day week.",
      "Biblical Period (~1200 BCE)", CS_CAT_TIME_PHILOSOPHY,
      { CS_SYSTEM_HEBREW, CS_SYSTEM_GREGORIAN, 0, 0 }, 2 },

    { 35, "Hebrew", "Metonic Cycle -- Lunisolar Harmony",
      "The Hebrew calendar reconciles lunar months with solar years using "
      "the 19-year Metonic cycle, in which 235 lunar months almost exactly "
      "equal 19 solar years. Seven leap months are intercalated over 19 "
      "years, keeping Passover in spring and festivals aligned with seasons.",
      "Second Temple Period (~5th century BCE)", CS_CAT_COSMIC_CYCLE,
      { CS_SYSTEM_HEBREW, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    /* ===== ISLAMIC (2 stories, indices 36-37) ===== */
    { 36, "Islamic", "Hijra -- The Calendar's Zero Point",
      "The Islamic calendar begins with the Hijra, Muhammad's migration from "
      "Mecca to Medina in 622 CE. As a purely lunar calendar of 354 or 355 "
      "days, it drifts through the solar seasons over a 33-year cycle. This "
      "design ensures every generation experiences Ramadan in every season.",
      "622 CE", CS_CAT_CALENDAR_REFORM,
      { CS_SYSTEM_ISLAMIC, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 37, "Islamic", "Al-Khwarizmi -- Father of Algebra and Algorithms",
      "Muhammad ibn Musa al-Khwarizmi (780-850 CE) wrote foundational works "
      "on algebra, astronomical tables, and Hindu-Arabic numerals. His name "
      "gave us the word 'algorithm.' His Zij al-Sindhind astronomical tables "
      "calculated planetary positions used in Islamic calendar-making.",
      "Abbasid Caliphate (~820 CE)", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_ISLAMIC, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    /* ===== PERSIAN (2 stories, indices 38-39) ===== */
    { 38, "Persian", "Nowruz -- The Zoroastrian New Year",
      "Nowruz marks the spring equinox and has been celebrated for over 3,000 "
      "years across Persia and Central Asia. Rooted in Zoroastrian cosmology, "
      "it honors the triumph of light over darkness. The Haft-sin table's "
      "seven symbolic items represent renewal and cosmic harmony.",
      "Achaemenid Period (~550 BCE)", CS_CAT_COSMIC_CYCLE,
      { CS_SYSTEM_ZOROASTRIAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },

    { 39, "Persian", "Omar Khayyam -- Calendar Reformer and Poet",
      "Omar Khayyam (1048-1131 CE) led the committee that created the Jalali "
      "calendar, more accurate than the Gregorian reform five centuries later. "
      "A mathematician, astronomer, and poet, his Rubaiyat meditates on the "
      "preciousness of time: 'Be happy for this moment. This moment is your life.'",
      "Seljuk Empire (1079 CE)", CS_CAT_KEY_FIGURE,
      { CS_SYSTEM_ZOROASTRIAN, CS_SYSTEM_ASTRONOMY, 0, 0 }, 2 },
};

static const int STORY_COUNT = sizeof(STORIES) / sizeof(STORIES[0]);

/* ---------- name tables ---------- */

static const char *CATEGORY_NAMES[] = {
    "Origin Myth",
    "Time Philosophy",
    "Key Figure",
    "Golden Age",
    "Calendar Reform",
    "Cosmic Cycle"
};

static const char *SYSTEM_NAMES[] = {
    "Gregorian",
    "Hebrew",
    "Islamic",
    "Chinese",
    "Hindu",
    "Buddhist",
    "Mayan",
    "Coptic",
    "Ethiopian",
    "Zoroastrian",
    "Celtic",
    "Myanmar",
    "Egyptian",
    "Astrology",
    "I Ching",
    "Human Design",
    "Kabbalah",
    "Geology",
    "Astronomy"
};

/* ---------- public API ---------- */

int cs_story_count(void)
{
    return STORY_COUNT;
}

cs_story_t cs_story_get(int index)
{
    if (index < 0 || index >= STORY_COUNT) {
        cs_story_t invalid = { .id = -1, .culture = NULL, .title = NULL,
                               .text = NULL, .era = NULL,
                               .category = CS_CAT_ORIGIN_MYTH,
                               .related_systems = {0, 0, 0, 0},
                               .system_count = 0 };
        return invalid;
    }
    return STORIES[index];
}

int cs_by_culture(const char *culture, int *results, int max_results)
{
    if (culture == NULL || results == NULL || max_results <= 0) return 0;
    int count = 0;
    for (int i = 0; i < STORY_COUNT && count < max_results; i++) {
        if (cs_ci_contains(STORIES[i].culture, culture)) {
            results[count++] = i;
        }
    }
    return count;
}

int cs_by_system(cs_system_t system, int *results, int max_results)
{
    if (results == NULL || max_results <= 0) return 0;
    if ((int)system < 0 || system >= CS_SYSTEM_COUNT) return 0;
    int count = 0;
    for (int i = 0; i < STORY_COUNT && count < max_results; i++) {
        for (int j = 0; j < STORIES[i].system_count; j++) {
            if (STORIES[i].related_systems[j] == system) {
                results[count++] = i;
                break;
            }
        }
    }
    return count;
}

int cs_by_category(cs_category_t category, int *results, int max_results)
{
    if (results == NULL || max_results <= 0) return 0;
    if ((int)category < 0 || category >= CS_CAT_COUNT) return 0;
    int count = 0;
    for (int i = 0; i < STORY_COUNT && count < max_results; i++) {
        if (STORIES[i].category == category) {
            results[count++] = i;
        }
    }
    return count;
}

const char *cs_category_name(cs_category_t category)
{
    if ((int)category < 0 || category >= CS_CAT_COUNT) return "Unknown";
    return CATEGORY_NAMES[category];
}

const char *cs_system_name(cs_system_t system)
{
    if ((int)system < 0 || system >= CS_SYSTEM_COUNT) return "Unknown";
    return SYSTEM_NAMES[system];
}

int cs_distinct_cultures(void)
{
    const char *seen[32];
    int n_seen = 0;
    for (int i = 0; i < STORY_COUNT; i++) {
        int found = 0;
        for (int j = 0; j < n_seen; j++) {
            if (cs_ci_equals(seen[j], STORIES[i].culture)) {
                found = 1;
                break;
            }
        }
        if (!found && n_seen < 32) {
            seen[n_seen++] = STORIES[i].culture;
        }
    }
    return n_seen;
}

int cs_culture_story_count(const char *culture)
{
    if (culture == NULL) return 0;
    int count = 0;
    for (int i = 0; i < STORY_COUNT; i++) {
        if (cs_ci_contains(STORIES[i].culture, culture)) {
            count++;
        }
    }
    return count;
}
