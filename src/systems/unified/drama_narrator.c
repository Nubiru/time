/* drama_narrator.c — Dramatization narrative for historical data modules.
 * Pre-authored dramatic voice for achievement, calendar_reform, and earth_drama.
 * All data is static const. No malloc, no globals, no side effects.
 *
 * Data sources: achievement.h (17), calendar_reform.h (10), earth_drama.h (12).
 * Voice: reverent but accessible, dramatic but factual. */

#include "drama_narrator.h"

#include <stddef.h>

/* ===== Source name strings ===== */

static const char *const SOURCE_NAMES[DRAMA_SRC_COUNT] = {
    "Achievement",
    "Calendar",
    "Earth"
};

/* ===== Achievement dramatizations (17 entries) ===== */

#define ACH_COUNT 17

static const drama_entry_t ACHIEVEMENTS[ACH_COUNT] = {
    /* 0: Eratosthenes — Earth circumference */
    {
        .id = 0,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 0,
        .hook = "In 240 BCE, Eratosthenes measured the Earth with a stick "
                "and geometry.",
        .body = "A stick, its shadow, and a 5,000-stadia walk between two "
                "Egyptian cities. That is all Eratosthenes had. No satellites, "
                "no lasers, no computers. He measured the Earth's circumference "
                "to within 700 kilometers of the value we know today "
                "-- a 1.7% error, using tools a child could hold."
    },
    /* 1: Hipparchus — tropical year */
    {
        .id = 1,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 1,
        .hook = "In 130 BCE, Hipparchus measured the year to within "
                "6.5 minutes.",
        .body = "With nothing but an armillary sphere and the patience of "
                "decades, Hipparchus determined the tropical year at 365.2467 "
                "days. The true value is 365.2422. His error: 6.5 minutes per "
                "year. He also discovered that the stars themselves were slowly "
                "drifting -- the precession of the equinoxes -- a motion so "
                "subtle it takes 26,000 years to complete one cycle."
    },
    /* 2: Hipparchus — precession */
    {
        .id = 2,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 2,
        .hook = "Hipparchus noticed the stars were moving -- and he was right.",
        .body = "By comparing his own star catalog with observations made 150 "
                "years earlier, Hipparchus detected a drift so slow that no "
                "human lifetime could perceive it. The equinox points were "
                "sliding along the ecliptic at roughly 46 arcseconds per year. "
                "The true rate is 50.3. He discovered precession -- the "
                "26,000-year wobble of Earth's axis -- using nothing but "
                "records and geometry."
    },
    /* 3: Ptolemy — obliquity */
    {
        .id = 3,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 3,
        .hook = "Ptolemy measured the tilt of the Earth to within a tenth "
                "of a degree.",
        .body = "In 150 CE, Ptolemy used a mural quadrant to measure the "
                "obliquity of the ecliptic -- the angle between Earth's "
                "equator and its orbit around the Sun. He found 23.855 "
                "degrees. The modern value is 23.71. Eight centuries of Greek "
                "astronomical observation, systematized in the Almagest, gave "
                "humanity its first complete model of the sky."
    },
    /* 4: Aryabhata — Earth circumference */
    {
        .id = 4,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 4,
        .hook = "In 499 CE, with no telescope, no satellite, no computer "
                "-- Aryabhata calculated Earth's circumference to within "
                "107 kilometers.",
        .body = "Working in Pataliputra with mathematical models and a gnomon, "
                "Aryabhata computed the Earth's circumference at 39,968 "
                "kilometers. The modern value measured by GPS is 40,075. His "
                "error was 0.27% -- one thousand years before European "
                "precision would match this. He also proposed that the Earth "
                "rotates on its axis, a millennium before Copernicus."
    },
    /* 5: Aryabhata — sidereal year */
    {
        .id = 5,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 5,
        .hook = "Aryabhata measured the year with a 3-minute error -- in the "
                "5th century.",
        .body = "365.25858 days. That was Aryabhata's calculation of the "
                "sidereal year in 499 CE. The modern value is 365.25636 days. "
                "The difference: 3.2 minutes per year. A thousand years before "
                "telescopes, before clocks, before the scientific revolution "
                "-- an Indian mathematician achieved precision that would take "
                "Europe another millennium to equal."
    },
    /* 6: Aryabhata — lunar month */
    {
        .id = 6,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 6,
        .hook = "Aryabhata's lunar month was accurate to less than one second.",
        .body = "29.5306 days. That was Aryabhata's synodic lunar month -- the "
                "time from new moon to new moon. The modern value is 29.53059 "
                "days. The error: 0.86 seconds per month. Calculated "
                "independently of Greek astronomy, using mathematical models "
                "that the Western world would not discover for centuries."
    },
    /* 7: Brahmagupta — lunar month */
    {
        .id = 7,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 7,
        .hook = "Brahmagupta measured the lunar month with a 0.6-second error.",
        .body = "In 628 CE, Brahmagupta refined the synodic lunar month to "
                "29.530582 days. The modern value is 29.530589. The "
                "difference: 0.6 seconds per month. No telescope. No clock. "
                "Just mathematical genius applied to naked-eye observations "
                "over generations. The moon's cycle, tracked to sub-second "
                "accuracy, thirteen centuries ago."
    },
    /* 8: Hillel II — molad */
    {
        .id = 8,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 8,
        .hook = "The Hebrew calendar still uses Hillel II's measurement "
                "from 359 CE.",
        .body = "In 359 CE, Hillel II fixed the mean lunar month -- the "
                "molad -- at 29.530594 days. The modern value is 29.530589 "
                "days. The error: 0.4 seconds per month. This value was built "
                "into the fixed Hebrew calendar, and it is still used today, "
                "unchanged, over 1,600 years later. The calendar that "
                "determines Passover, Rosh Hashanah, and Yom Kippur runs on a "
                "4th-century measurement accurate to less than half a second."
    },
    /* 9: Al-Khwarizmi — sine */
    {
        .id = 9,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 9,
        .hook = "Al-Khwarizmi's algorithms were used for 400 years "
                "without correction.",
        .body = "The word 'algorithm' comes from his name. In 820 CE, "
                "Al-Khwarizmi computed the sine of one degree to a precision "
                "of 0.01% using the Hindu numeral system and geometric "
                "methods. His mathematical tables were so reliable that "
                "astronomers across the Islamic world and medieval Europe used "
                "them for four centuries. The foundation of computational "
                "astronomy was laid in Baghdad."
    },
    /* 10: Maya — Venus */
    {
        .id = 10,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 10,
        .hook = "The Maya tracked Venus with a 2-hour error per "
                "584-day cycle.",
        .body = "From stone temples in the Yucatan jungle, with no metal "
                "instruments, no telescope, no written mathematical notation "
                "as we know it, Maya astronomers determined the synodic period "
                "of Venus at 583.92 days. The modern value is 583.93. Their "
                "error: 2 hours across a cycle that takes nearly two years. "
                "The Dresden Codex, a bark-paper book, contains Venus tables "
                "that predicted its appearances for centuries."
    },
    /* 11: Maya — lunar */
    {
        .id = 11,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 11,
        .hook = "Maya eclipse prediction: accurate to hours over 32 years.",
        .body = "Over 405 lunations -- 32.8 years of lunar cycles -- Maya "
                "astronomers calculated a total of 11,959.888 days. The "
                "modern value is 11,959.889. The error: 86 seconds across "
                "three decades. Written in the Dresden Codex with a "
                "bar-and-dot numeral system, this is one of the most precise "
                "astronomical measurements of the ancient world, from a "
                "civilization that never used a wheel."
    },
    /* 12: Ulugh Beg — sidereal year */
    {
        .id = 12,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 12,
        .hook = "Ulugh Beg's year measurement was essentially perfect "
                "-- without a telescope.",
        .body = "In 1437 CE, from a 40-meter sextant at his observatory in "
                "Samarkand, Ulugh Beg measured the sidereal year at 365.25636 "
                "days. The modern value? 365.25636 days. Exact to the "
                "precision of his measurement. A Timurid prince who was also "
                "an astronomer built the most accurate pre-telescopic "
                "observatory in human history and produced star catalogs that "
                "remained unsurpassed for two centuries."
    },
    /* 13: Su Song — tropical year */
    {
        .id = 13,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 13,
        .hook = "Su Song measured the year using the world's first "
                "astronomical clock tower.",
        .body = "In 1088 CE, Su Song built a 12-meter water-powered "
                "astronomical clock tower in Kaifeng -- the world's first. "
                "Using its armillary sphere, he calculated the tropical year "
                "at 365.2436 days. The true value is 365.2422. His error: "
                "2 minutes per year. The clock tower was dismantled when the "
                "Jurchen conquered Kaifeng in 1127 -- its technology would "
                "not be replicated for centuries."
    },
    /* 14: Copernicus — sidereal year */
    {
        .id = 14,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 14,
        .hook = "Copernicus reframed the solar system -- but Ulugh Beg had "
                "a better year.",
        .body = "In 1543, Copernicus published De Revolutionibus, placing the "
                "Sun at the center of the solar system. His sidereal year of "
                "365.25671 days was remarkably accurate -- but not as accurate "
                "as Ulugh Beg's measurement made 106 years earlier in "
                "Samarkand. The revolutionary genius who displaced Earth from "
                "the center of the universe was outperformed on year length by "
                "a Timurid prince."
    },
    /* 15: Aristarchus — Sun distance */
    {
        .id = 15,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 15,
        .hook = "Aristarchus was off by a factor of 20 -- but his method "
                "was perfect.",
        .body = "In 270 BCE, Aristarchus measured the angle between the Sun "
                "and Moon at half-moon and used geometry to estimate the Sun "
                "was 19 times farther than the Moon. The true ratio is 389 "
                "times. He was wildly wrong -- but his method was flawless. "
                "The measurement was impossible with naked-eye precision. He "
                "proved the Sun was enormously far away, and that the cosmos "
                "was vastly larger than anyone imagined."
    },
    /* 16: Tycho Brahe — star positions */
    {
        .id = 16,
        .source = DRAMA_SRC_ACHIEVEMENT,
        .source_index = 16,
        .hook = "Tycho Brahe recorded the most accurate star positions ever "
                "achieved without a telescope.",
        .body = "For 20 years on the island of Hven, Tycho Brahe measured "
                "star positions to 1 arcminute accuracy using custom-built "
                "instruments of unprecedented size. He cataloged over 1,000 "
                "stars. His data was so precise that Johannes Kepler was able "
                "to derive the laws of planetary motion from it -- the "
                "foundation of modern astronomy, built on the naked eye of "
                "a Danish nobleman."
    }
};

/* ===== Calendar reform dramatizations (10 entries) ===== */

#define CAL_COUNT 10
#define CAL_OFFSET ACH_COUNT  /* 17 */

static const drama_entry_t CALENDARS[CAL_COUNT] = {
    /* 0: Egyptian Civil Calendar */
    {
        .id = 17,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 0,
        .hook = "Five thousand years ago, Egypt invented the solar calendar.",
        .body = "Around 3000 BCE, Egyptian priests created the first solar "
                "calendar: 12 months of 30 days, plus 5 extra days at the "
                "end -- the epagomenal days, considered unlucky. No leap year. "
                "The calendar drifted one day every four years, cycling "
                "through the seasons over 1,460 years. Despite this flaw, its "
                "elegant simplicity influenced every calendar that followed."
    },
    /* 1: Babylonian Calendar */
    {
        .id = 18,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 1,
        .hook = "The king decided when to add a month -- and that was "
                "the problem.",
        .body = "The Babylonian calendar tracked both the Moon and the "
                "seasons by adding a 13th month when needed. But who decided "
                "when? The king. Intercalation was a political tool -- extend "
                "a favorable month, shorten a rival's reign. For 2,000 years, "
                "Mesopotamian timekeeping served power as much as astronomy."
    },
    /* 2: Roman Republican Calendar */
    {
        .id = 19,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 2,
        .hook = "Roman politicians weaponized the calendar.",
        .body = "The Roman Republican calendar had 355 days and relied on the "
                "Pontifex Maximus to insert extra months. In practice, "
                "pontiffs extended the terms of political allies and shortened "
                "those of enemies. By the time Julius Caesar took power, the "
                "calendar was three months ahead of the seasons. March was "
                "happening in winter."
    },
    /* 3: Julian Calendar */
    {
        .id = 20,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 3,
        .hook = "Julius Caesar fixed the calendar -- with an extra-long year "
                "of 445 days.",
        .body = "In 46 BCE, Caesar hired the astronomer Sosigenes to end the "
                "calendar chaos. The fix required a single extraordinary year "
                "-- the Year of Confusion -- lasting 445 days. Then a new "
                "rule: one leap day every four years. 365.25 days per year. "
                "Clean. Simple. But 11.5 minutes too long. Over the next "
                "1,628 years, the error would accumulate to 10 full days."
    },
    /* 4: Gregorian Calendar */
    {
        .id = 21,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 4,
        .hook = "In 1582, Pope Gregory XIII deleted 10 days from October.",
        .body = "People who went to sleep on October 4th, 1582, woke up on "
                "October 15th. Ten days, erased. Pope Gregory XIII, advised "
                "by the mathematician Aloysius Lilius, corrected the Julian "
                "calendar's 1,600-year drift. The new rule: no leap year on "
                "century years unless divisible by 400. Average year: "
                "365.2425 days -- only 26 seconds too long. Protestant "
                "nations refused the reform for centuries. Britain adopted it "
                "in 1752."
    },
    /* 5: French Republican Calendar */
    {
        .id = 22,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 5,
        .hook = "Revolutionary France tried decimal time: 10 hours per day, "
                "100 minutes per hour.",
        .body = "In 1793, the French National Convention replaced not just the "
                "monarchy -- they replaced time itself. Twelve months of 30 "
                "days, named after nature: Brumaire for fog, Thermidor for "
                "heat, Fructidor for fruit. Decimal time: 10 hours per day, "
                "each 100 minutes long. Rational, elegant, and utterly "
                "impractical. It lasted 12 years before Napoleon restored "
                "the Gregorian calendar."
    },
    /* 6: 13 Moon Calendar */
    {
        .id = 23,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 6,
        .hook = "Arguelles proposed perfect symmetry: 13 months of exactly "
                "28 days.",
        .body = "In 1992, Jose Arguelles proposed the 13 Moon calendar -- 13 "
                "months of precisely 28 days, plus one Day Out of Time on "
                "July 25th. Every month starts on the same day. Every year is "
                "identical. The 28-day month aligns with biological rhythms "
                "and the Moon's cycle. No nation has adopted it, but "
                "communities worldwide use it, believing the Gregorian "
                "calendar disconnects humanity from natural time."
    },
    /* 7: World Calendar */
    {
        .id = 24,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 7,
        .hook = "The League of Nations almost adopted a new calendar in 1937.",
        .body = "Elisabeth Achelis designed the World Calendar: four identical "
                "quarters of 91 days, with one Worldsday at year's end that "
                "belongs to no week. Every date falls on the same weekday "
                "forever. January 1 is always Sunday. The League of Nations "
                "came close to adopting it in 1937. Religious leaders killed "
                "it -- Worldsday would break the unbroken seven-day week "
                "cycle that Judaism, Christianity, and Islam all require."
    },
    /* 8: International Fixed Calendar */
    {
        .id = 25,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 8,
        .hook = "Kodak used a 13-month calendar internally for 61 years.",
        .body = "Moses Cotsworth's International Fixed Calendar added a 13th "
                "month called Sol between June and July. Every month had "
                "exactly 28 days. Eastman Kodak adopted it for internal "
                "accounting in 1928 and used it until 1989. The problem: 13 "
                "does not divide by 2 or 4, making half-year and quarterly "
                "reports impossible. A mathematically perfect calendar, "
                "defeated by the needs of corporate accounting."
    },
    /* 9: Seasonal Hours */
    {
        .id = 26,
        .source = DRAMA_SRC_CALENDAR,
        .source_index = 9,
        .hook = "For most of human history, an hour in summer was longer "
                "than an hour in winter.",
        .body = "Before mechanical clocks, time flexed with the sun. Day was "
                "divided into 12 hours of light and 12 of darkness, regardless "
                "of season. A summer day-hour in Rome could last 75 minutes; "
                "a winter day-hour, only 45. Jewish law, Japanese timekeeping "
                "until 1873, and Roman daily life all used this system. When "
                "clocks standardized the hour, we gained precision -- but lost "
                "a relationship with natural time that had lasted millennia."
    }
};

/* ===== Earth drama dramatizations (12 entries) ===== */

#define EARTH_COUNT 12
#define EARTH_OFFSET (ACH_COUNT + CAL_COUNT)  /* 27 */

static const drama_entry_t EARTH[EARTH_COUNT] = {
    /* 0: Formation (4540-3800 Ma) */
    {
        .id = 27,
        .source = DRAMA_SRC_EARTH,
        .source_index = 0,
        .hook = "Earth begins as molten rock under cosmic bombardment.",
        .body = "For 740 million years, Earth is a ball of molten rock. The "
                "surface glows red. Asteroids the size of countries slam into "
                "it regularly. There is no atmosphere worth mentioning -- just "
                "outgassing from volcanoes. The Moon was born when a "
                "Mars-sized body crashed into the young Earth, flinging enough "
                "debris into orbit to coalesce into our satellite. This is "
                "not yet a planet. It is a forge."
    },
    /* 1: First life (3800-2700 Ma) */
    {
        .id = 28,
        .source = DRAMA_SRC_EARTH,
        .source_index = 1,
        .hook = "The first life appears -- invisible, single-celled, "
                "unstoppable.",
        .body = "In hydrothermal vents on the ocean floor, chemistry becomes "
                "biology. The first prokaryotes are microscopic, "
                "single-celled, and absolutely tenacious. They require no "
                "oxygen, no sunlight -- just chemicals seeping from volcanic "
                "rock. For over a billion years, they are the only life on "
                "Earth. Invisible to any observer, they are already rewriting "
                "the planet's future."
    },
    /* 2: Cyanobacteria (2700-2400 Ma) */
    {
        .id = 29,
        .source = DRAMA_SRC_EARTH,
        .source_index = 2,
        .hook = "Cyanobacteria invent photosynthesis and begin poisoning "
                "the atmosphere.",
        .body = "Around 2.7 billion years ago, cyanobacteria develop the "
                "ability to split water molecules using sunlight, releasing "
                "oxygen as waste. This is the most consequential invention in "
                "Earth's history. Oxygen is toxic to nearly every living thing "
                "on the planet. The cyanobacteria do not care. They keep "
                "photosynthesizing. The atmosphere will never be the same."
    },
    /* 3: Great Oxidation Event (2400-2000 Ma) */
    {
        .id = 30,
        .source = DRAMA_SRC_EARTH,
        .source_index = 3,
        .hook = "The Great Oxidation Event -- free oxygen nearly sterilizes "
                "the planet.",
        .body = "Oxygen accumulates. Methane, the greenhouse gas keeping "
                "Earth warm, reacts with oxygen and disappears. Global "
                "temperatures plunge. Ice sheets advance from the poles to "
                "the equator. Most anaerobic life -- the majority of living "
                "things -- dies. This is Earth's first mass extinction, caused "
                "not by an asteroid but by a microbe's metabolic waste. The "
                "survivors will inherit a fundamentally different world."
    },
    /* 4: Boring Billion (2000-1000 Ma) */
    {
        .id = 31,
        .source = DRAMA_SRC_EARTH,
        .source_index = 4,
        .hook = "For a billion years, almost nothing happens.",
        .body = "The Boring Billion. From 2000 to 1000 million years ago, "
                "Earth's climate is stable, oxygen levels plateau, and "
                "evolution creeps forward at a glacial pace. Eukaryotes -- "
                "cells with nuclei -- quietly evolve. Sexual reproduction "
                "appears. But the planet looks the same century after century, "
                "millennium after millennium. The most dramatic thing about "
                "this era is how undramatic it is."
    },
    /* 5: Snowball -> Ediacaran (1000-541 Ma) */
    {
        .id = 32,
        .source = DRAMA_SRC_EARTH,
        .source_index = 5,
        .hook = "Snowball Earth melts, and life begins to experiment "
                "with form.",
        .body = "For 85 million years, Snowball Earth is locked in ice from "
                "pole to equator. Then, somehow, life survives. When the ice "
                "melts around 635 million years ago, something extraordinary "
                "happens: multicellular organisms begin experimenting with "
                "shapes. The Ediacaran fauna -- quilted, fractal, unlike "
                "anything alive today -- appear in the oceans. Life is "
                "learning to build bodies."
    },
    /* 6: Cambrian Explosion (541-470 Ma) */
    {
        .id = 33,
        .source = DRAMA_SRC_EARTH,
        .source_index = 6,
        .hook = "Every animal body plan appears in a geological instant.",
        .body = "In the span of 20 million years -- a blink in geological "
                "time -- nearly every major animal body plan appears. Eyes. "
                "Shells. Claws. Spines. The Cambrian Explosion is the most "
                "creative burst in the history of life. Anomalocaris, a "
                "meter-long predator, hunts trilobites in warm shallow seas. "
                "In this single period, evolution invents the basic toolkit "
                "that all animals will use for the next half-billion years."
    },
    /* 7: Plants on land (470-375 Ma) */
    {
        .id = 34,
        .source = DRAMA_SRC_EARTH,
        .source_index = 7,
        .hook = "Plants leave the ocean and build the first forests.",
        .body = "For three billion years, land was bare rock. Then plants "
                "made the crossing. First mosses, then ferns, then the first "
                "trees. By 380 million years ago, forests cover the "
                "continents. Atmospheric oxygen soars to levels never seen "
                "before. The partnership between plants and mycorrhizal fungi "
                "-- a relationship that still feeds every forest on Earth -- "
                "begins here. The land turns green for the first time."
    },
    /* 8: Tetrapods and coal (375-252 Ma) */
    {
        .id = 35,
        .source = DRAMA_SRC_EARTH,
        .source_index = 8,
        .hook = "Fish crawl ashore, and dead forests lock carbon underground "
                "for 300 million years.",
        .body = "Tiktaalik, a fish with wrist bones, hauls itself onto a "
                "mudflat 375 million years ago. Tetrapods -- four-limbed "
                "vertebrates -- colonize the land. Meanwhile, vast coal swamps "
                "lock atmospheric carbon into the ground, where it will remain "
                "until humans dig it up as coal and oil. The Carboniferous "
                "forests are still powering our civilization."
    },
    /* 9: Dinosaurs (252-66 Ma) */
    {
        .id = 36,
        .source = DRAMA_SRC_EARTH,
        .source_index = 9,
        .hook = "Dinosaurs reign for 186 million years -- 620 times longer "
                "than human civilization.",
        .body = "After the Permian extinction kills 96% of marine species, "
                "dinosaurs inherit a devastated world and rule it for 186 "
                "million years. They evolve into thousands of species, from "
                "hummingbird-sized fliers to 40-meter sauropods. Mammals "
                "exist the entire time -- small, nocturnal, hiding in the "
                "shadows. All of recorded human history fits into the final "
                "0.00003% of the age of dinosaurs."
    },
    /* 10: After asteroid (66-0.3 Ma) */
    {
        .id = 37,
        .source = DRAMA_SRC_EARTH,
        .source_index = 10,
        .hook = "An asteroid ends the dinosaurs -- and mammals inherit "
                "a shattered world.",
        .body = "66 million years ago, a 12-kilometer asteroid strikes the "
                "Yucatan Peninsula. The impact energy equals 10 billion "
                "nuclear bombs. Darkness. Cold. Acid rain. 75% of all species "
                "die. But mammals survive. With the dinosaurs gone, mammals "
                "diversify explosively: bats, whales, horses, primates. 65 "
                "million years of evolution leads to a species that will, one "
                "day, figure out what happened."
    },
    /* 11: Humans (0.3-0 Ma) */
    {
        .id = 38,
        .source = DRAMA_SRC_EARTH,
        .source_index = 11,
        .hook = "Homo sapiens arrives in the final heartbeat of "
                "geological time.",
        .body = "If Earth's 4.54-billion-year history were compressed to 24 "
                "hours, Homo sapiens arrives less than one second before "
                "midnight. In that fraction of a fraction of a day, we "
                "develop language, agriculture, cities, mathematics, "
                "telescopes, spaceflight -- and the ability to measure the "
                "age of the planet whose surface we so recently learned to "
                "walk on."
    }
};

/* Total count across all sources */
#define TOTAL_COUNT (ACH_COUNT + CAL_COUNT + EARTH_COUNT)

/* ===== Invalid entry template ===== */

static const drama_entry_t INVALID_ENTRY = {
    .id = -1,
    .source = DRAMA_SRC_COUNT,
    .source_index = -1,
    .hook = NULL,
    .body = NULL
};

/* ===== Public API ===== */

int drama_narrator_count(void)
{
    return TOTAL_COUNT;
}

drama_entry_t drama_narrator_get(int index)
{
    if (index < 0 || index >= TOTAL_COUNT) {
        return INVALID_ENTRY;
    }
    if (index < ACH_COUNT) {
        return ACHIEVEMENTS[index];
    }
    if (index < ACH_COUNT + CAL_COUNT) {
        return CALENDARS[index - ACH_COUNT];
    }
    return EARTH[index - EARTH_OFFSET];
}

drama_entry_t drama_narrator_for(drama_source_t source, int source_index)
{
    if (source_index < 0) {
        return INVALID_ENTRY;
    }

    switch (source) {
    case DRAMA_SRC_ACHIEVEMENT:
        if (source_index >= ACH_COUNT) {
            return INVALID_ENTRY;
        }
        return ACHIEVEMENTS[source_index];

    case DRAMA_SRC_CALENDAR:
        if (source_index >= CAL_COUNT) {
            return INVALID_ENTRY;
        }
        return CALENDARS[source_index];

    case DRAMA_SRC_EARTH:
        if (source_index >= EARTH_COUNT) {
            return INVALID_ENTRY;
        }
        return EARTH[source_index];

    default:
        return INVALID_ENTRY;
    }
}

int drama_narrator_count_by_source(drama_source_t source)
{
    switch (source) {
    case DRAMA_SRC_ACHIEVEMENT: return ACH_COUNT;
    case DRAMA_SRC_CALENDAR:    return CAL_COUNT;
    case DRAMA_SRC_EARTH:       return EARTH_COUNT;
    default:                    return 0;
    }
}

int drama_narrator_by_source(drama_source_t source, int *out_indices, int out_max)
{
    if (out_indices == NULL || out_max <= 0) {
        return 0;
    }

    int offset;
    int count;

    switch (source) {
    case DRAMA_SRC_ACHIEVEMENT:
        offset = 0;
        count = ACH_COUNT;
        break;
    case DRAMA_SRC_CALENDAR:
        offset = CAL_OFFSET;
        count = CAL_COUNT;
        break;
    case DRAMA_SRC_EARTH:
        offset = EARTH_OFFSET;
        count = EARTH_COUNT;
        break;
    default:
        return 0;
    }

    int filled = 0;
    for (int i = 0; i < count && filled < out_max; i++) {
        out_indices[filled] = offset + i;
        filled++;
    }
    return filled;
}

const char *drama_source_name(drama_source_t source)
{
    if ((int)source < 0 || (int)source >= DRAMA_SRC_COUNT) {
        return "?";
    }
    return SOURCE_NAMES[(int)source];
}
