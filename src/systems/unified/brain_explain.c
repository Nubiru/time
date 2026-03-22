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
      "Both track the same moon — and share divine vocabulary",
      "Both are lunar calendars counting from the same synodic month "
      "(29.53 days). Hebrew adds intercalary months; Islamic drifts through "
      "seasons. Beyond astronomy, they share theology: Quranic sakīnah "
      "(divine peace) directly parallels Hebrew shekinah (divine presence)." },

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
      "I Ching is the root of Chinese cosmology",
      "The I Ching (c. 1000 BCE) underpins Chinese cosmology. Its trigram "
      "arrangements derive from He Tu (55 dots) and Lo Shu (magic square, "
      "sum 15). Five Elements and Ten Stems were integrated with Eight "
      "Trigrams by Sung scholars, sharing a single numerical foundation." },

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
      "Combinatorial completeness from opposite sides of the world",
      "64 hexagrams exhaust all 6-line binary combinations. 22 Hebrew "
      "letters generate 231 gates (C(22,2) pairs). Both traditions "
      "independently discovered that all possible states can be enumerated "
      "from a small set of simple elements." },

    { CD_SYS_CELTIC, CD_SYS_TZOLKIN, BR_EXPLAIN_CONVERGENT,
      "No known contact",
      "Pre-Christian European and Mesoamerican traditions developed "
      "independently. Any alignment is coincidental or reflects shared "
      "astronomical observation (solstices, equinoxes)." },

    { CD_SYS_COPTIC, CD_SYS_EGYPTIAN, BR_EXPLAIN_TRANSMISSION,
      "Coptic inherits Egyptian",
      "The Coptic calendar is a direct descendant of the ancient Egyptian "
      "civil calendar, preserving the 12x30+5 day structure." },

    /* --- Equinox-cluster pairs (2026-03-20 narrative pipeline) --- */

    { CD_SYS_ASTRONOMY, CD_SYS_PERSIAN, BR_EXPLAIN_SHARED_SOURCE,
      "Nowruz anchored to vernal equinox",
      "The Persian New Year (Nowruz) is defined as the exact moment of the "
      "vernal equinox — a direct astronomical observation that has anchored "
      "the calendar for over 3,000 years." },

    { CD_SYS_ASTRONOMY, CD_SYS_BAHAI, BR_EXPLAIN_SHARED_SOURCE,
      "Naw-Ruz at vernal equinox",
      "The Bahai calendar begins its year at the vernal equinox (Naw-Ruz). "
      "This anchor point is determined by astronomical observation, directly "
      "inherited from Persian tradition." },

    { CD_SYS_ASTRONOMY, CD_SYS_HINDU, BR_EXPLAIN_SHARED_SOURCE,
      "Surya Siddhanta astronomical model",
      "Hindu calendar parameters derive from the Surya Siddhanta, an "
      "astronomical text that models solar and lunar motion. Tithi and "
      "nakshatra calculations are direct astronomical observations." },

    { CD_SYS_ASTRONOMY, CD_SYS_FRENCH, BR_EXPLAIN_SHARED_SOURCE,
      "French Republican year begins at autumn equinox",
      "The French Republican calendar anchored its year start to the "
      "autumn equinox — an astronomical event that coincided with the "
      "founding of the Republic in 1792." },

    { CD_SYS_ISLAMIC, CD_SYS_BAHAI, BR_EXPLAIN_TRANSMISSION,
      "Bahai emerged from Islamic culture",
      "The Bahai Faith arose in 19th-century Persia within an Islamic "
      "cultural context. While the Bahai calendar is distinct (19 months "
      "of 19 days), its origin is within the Islamic world." },

    { CD_SYS_BAHAI, CD_SYS_FRENCH, BR_EXPLAIN_CONVERGENT,
      "Both anchored to equinox, independently",
      "The Bahai calendar (vernal equinox) and French Republican calendar "
      "(autumn equinox) both chose equinoxes as their year start — one "
      "from religious revelation, the other from revolutionary rationalism." },

    { CD_SYS_PERSIAN, CD_SYS_FRENCH, BR_EXPLAIN_CONVERGENT,
      "Both solar, equinox-anchored",
      "Both the Persian (Solar Hijri) and French Republican calendars "
      "anchor their year to an equinox. The Persian calendar uses the "
      "vernal equinox; the French Republican used the autumn equinox." },

    /* --- Batch 2: high-frequency pairs from 2026 convergence report --- */

    { CD_SYS_ASTRONOMY, CD_SYS_BUDDHIST, BR_EXPLAIN_SHARED_SOURCE,
      "Lunar phase observation",
      "Buddhist Uposatha days align with quarter-moon phases (new, "
      "waxing, full, waning). Both systems observe the same lunar cycle "
      "— astronomy measures it, Buddhism sanctifies it." },

    { CD_SYS_ASTRONOMY, CD_SYS_TZOLKIN, BR_EXPLAIN_CONVERGENT,
      "Independent cycles occasionally overlap",
      "The 260-day Tzolkin count and the 29.5-day lunar cycle are "
      "mathematically unrelated — they share no common period. Any "
      "alignment is coincidental, yet recurring." },

    { CD_SYS_TZOLKIN, CD_SYS_BUDDHIST, BR_EXPLAIN_CONVERGENT,
      "No historical contact",
      "Mesoamerican day-counting and Southeast Asian lunar observance "
      "developed independently. The Tzolkin's 260-day cycle has no "
      "relation to the Buddhist lunar calendar." },

    { CD_SYS_COPTIC, CD_SYS_FRENCH, BR_EXPLAIN_SHARED_SOURCE,
      "Both inherit Egyptian 12x30+5 structure",
      "The Coptic calendar descends directly from ancient Egypt's civil "
      "calendar. The French Republican calendar adopted the same 12x30+5 "
      "structure, inspired by its rational simplicity." },

    { CD_SYS_JAPANESE, CD_SYS_FRENCH, BR_EXPLAIN_CONVERGENT,
      "Independent day-counting systems",
      "Japanese rokuyo (6-day fortune cycle) and the French Republican "
      "decade (10-day week) are unrelated counting systems that "
      "occasionally produce simultaneous significant days." },

    { CD_SYS_BUDDHIST, CD_SYS_JAPANESE, BR_EXPLAIN_TRANSMISSION,
      "Buddhism transmitted to Japan",
      "Buddhism reached Japan via Korea and China in the 6th century CE. "
      "Buddhist observance days (Uposatha) influenced Japanese religious "
      "practice alongside indigenous Shinto traditions." },

    { CD_SYS_BUDDHIST, CD_SYS_COPTIC, BR_EXPLAIN_CONVERGENT,
      "Independent lunar-sensitive traditions",
      "Buddhist Uposatha (quarter-moon days) and Coptic month boundaries "
      "are independently derived. Buddhism tracks the synodic month; "
      "the Coptic calendar uses a fixed solar structure." },

    { CD_SYS_CELTIC, CD_SYS_ASTROLOGY, BR_EXPLAIN_SHARED_SOURCE,
      "Both track solar position",
      "Celtic Sabbats mark solstices, equinoxes, and cross-quarter days. "
      "Western astrology divides the same ecliptic into 12 signs. Both "
      "systems read meaning from the Sun's annual journey." },

    { CD_SYS_ASTROLOGY, CD_SYS_JAPANESE, BR_EXPLAIN_CONVERGENT,
      "Both use solar longitude thresholds",
      "Western zodiac sign boundaries (every 30 degrees) and Japanese "
      "sekki solar terms (every 15 degrees) both divide the ecliptic "
      "by angular position. Same sky, different division schemes." },

    { CD_SYS_ASTRONOMY, CD_SYS_COPTIC, BR_EXPLAIN_SHARED_SOURCE,
      "Egyptian astronomical heritage",
      "The Coptic calendar inherits the ancient Egyptian civil calendar, "
      "which was originally calibrated to the heliacal rising of Sirius "
      "— a direct astronomical observation." },

    /* --- Batch 3: I Ching cross-system pairs (Digests 086, 087) --- */

    { CD_SYS_ICHING, CD_SYS_HINDU, BR_EXPLAIN_CONVERGENT,
      "Binary completeness in independent traditions",
      "The I Ching's 64 hexagrams form a complete 6-bit binary system (2^6). "
      "Hindu panchanga uses 30 tithis per month and 27 nakshatras — both "
      "are systematic divisions of cycles. Independent yet structurally "
      "analogous approaches to encoding cyclical time." },

    { CD_SYS_ICHING, CD_SYS_ASTROLOGY, BR_EXPLAIN_CONVERGENT,
      "Cyclic archetypes from different sources",
      "I Ching hexagrams cycle through 64 states of change. Western zodiac "
      "cycles through 12 archetypal energies. Both assign qualitative meaning "
      "to positions in a cycle. The I Ching uses binary logic; astrology uses "
      "celestial geometry." },

    /* --- Batch 4: Under-connected systems + high-value gaps --- */

    { CD_SYS_TAMIL, CD_SYS_HINDU, BR_EXPLAIN_TRANSMISSION,
      "Tamil is a regional Hindu calendar variant",
      "The Tamil calendar is the South Indian version of the Hindu lunisolar "
      "system. It uses the same Surya Siddhanta astronomical parameters, "
      "60-year Jupiter cycle (samvatsara), and solar month boundaries." },

    { CD_SYS_TAMIL, CD_SYS_ASTRONOMY, BR_EXPLAIN_SHARED_SOURCE,
      "60-year Jupiter cycle from astronomical observation",
      "The Tamil 60-year Jovian cycle tracks Jupiter's ~11.86-year orbital "
      "period. Five Jupiter orbits approximate 60 years, matching the "
      "sexagenary cycle — a direct astronomical observation." },

    { CD_SYS_TAMIL, CD_SYS_BUDDHIST, BR_EXPLAIN_CONVERGENT,
      "South and Southeast Asian parallels",
      "Tamil Hindu and Theravada Buddhist calendars share Indian astronomical "
      "roots but diverged. Tamil uses the Surya Siddhanta directly; Buddhist "
      "calendars in Southeast Asia use modified Indian parameters." },

    { CD_SYS_MYANMAR, CD_SYS_BUDDHIST, BR_EXPLAIN_TRANSMISSION,
      "Myanmar calendar serves Buddhist observance",
      "The Myanmar calendar exists primarily to determine Buddhist holy days. "
      "Uposatha (quarter-moon) and Thingyan (new year) timing are the "
      "calendar's central purpose." },

    { CD_SYS_BUDDHIST, CD_SYS_THAI, BR_EXPLAIN_TRANSMISSION,
      "Thai calendar counts Buddhist Era",
      "Thailand's official calendar counts from the Buddha's parinibbana "
      "(543 BCE). Buddhist Uposatha days are national holidays, and the "
      "Thai lunisolar calendar tracks them." },

    { CD_SYS_KOREAN, CD_SYS_JAPANESE, BR_EXPLAIN_TRANSMISSION,
      "Parallel adoptions from China",
      "Korea and Japan both adopted the Chinese lunisolar calendar. Korea "
      "kept closer to the Chinese model; Japan switched to Gregorian in 1873 "
      "but retained cultural elements (rokuyo, sekki)." },

    { CD_SYS_ISLAMIC, CD_SYS_PERSIAN, BR_EXPLAIN_TRANSMISSION,
      "Persian calendar epoch from Islamic conquest",
      "The Solar Hijri calendar uses the same epoch as the Islamic calendar "
      "(622 CE, Muhammad's hijra) but counts solar years instead of lunar. "
      "It replaced the Zoroastrian calendar after the Arab conquest." },

    { CD_SYS_ISLAMIC, CD_SYS_COPTIC, BR_EXPLAIN_CONVERGENT,
      "Different structures in the same region",
      "Both are Middle Eastern calendars but structurally opposite: Islamic "
      "is pure lunar (354 days), Coptic is solar (365 days). They share a "
      "geographic region but not a mathematical relationship." },

    { CD_SYS_ISLAMIC, CD_SYS_HINDU, BR_EXPLAIN_CONVERGENT,
      "Coexistence in South Asia",
      "Both calendars are used in South Asia — the Islamic Hijri and Hindu "
      "Panchanga. Mughal rule brought them into administrative parallel, but "
      "their structures (pure lunar vs lunisolar) are independent." },

    { CD_SYS_ASTROLOGY, CD_SYS_HINDU, BR_EXPLAIN_SHARED_SOURCE,
      "Both divide the ecliptic",
      "Western tropical zodiac (12 signs x 30 degrees) and Hindu Jyotish "
      "(12 rashis + 27 nakshatras) both divide the same ecliptic circle. "
      "The split: Western uses the vernal equinox; Hindu uses sidereal stars." },

    { CD_SYS_ASTROLOGY, CD_SYS_PERSIAN, BR_EXPLAIN_SHARED_SOURCE,
      "Shared Mesopotamian roots",
      "Western astrology and Persian astronomical tradition both trace to "
      "Babylonian celestial observation. The zodiac, planetary hours, and "
      "decan system were transmitted through the same Near Eastern lineage." },

    { CD_SYS_ASTROLOGY, CD_SYS_CHINESE, BR_EXPLAIN_CONVERGENT,
      "Independent zodiac traditions",
      "Western zodiac divides the ecliptic into 12 tropical signs. Chinese "
      "zodiac assigns 12 animals to a Jupiter-based yearly cycle. Same "
      "number, different astronomical basis, no historical connection." },

    { CD_SYS_ZOROASTRIAN, CD_SYS_BAHAI, BR_EXPLAIN_TRANSMISSION,
      "Bahai inherits Zoroastrian calendar anchor",
      "The Bahai calendar's Naw-Ruz (new year at vernal equinox) comes "
      "directly from the Zoroastrian/Persian Nowruz tradition. The Bab "
      "proclaimed his mission on Naw-Ruz 1844." },

    { CD_SYS_ZOROASTRIAN, CD_SYS_EGYPTIAN, BR_EXPLAIN_CONVERGENT,
      "Ancient solar calendars with epagomenal days",
      "Both Zoroastrian and Egyptian calendars use twelve 30-day months "
      "plus 5 epagomenal days (Gatha days / Epagomenai). Independent "
      "inventions of the same efficient solar calendar structure." },

    { CD_SYS_CELTIC, CD_SYS_FRENCH, BR_EXPLAIN_CONVERGENT,
      "European calendar traditions, different philosophies",
      "Celtic Sabbats mark the solar year through agricultural and spiritual "
      "observation. The French Republican calendar reimagined the year through "
      "Enlightenment rationalism. Same geography, opposite worldviews." },

    { CD_SYS_HEBREW, CD_SYS_BAHAI, BR_EXPLAIN_CONVERGENT,
      "Abrahamic heritage, distinct structures",
      "Both emerge from Abrahamic religious tradition, but their calendars "
      "are structurally different: Hebrew is lunisolar (Metonic 19-year), "
      "Bahai is solar (19 months x 19 days = 361 + intercalary)." },

    { CD_SYS_ICHING, CD_SYS_JAPANESE, BR_EXPLAIN_TRANSMISSION,
      "I Ching transmitted to Japan via China",
      "The I Ching reached Japan through Chinese cultural transmission. "
      "The text influenced Japanese philosophy and divinatory practice "
      "alongside indigenous Shinto traditions." },

    { CD_SYS_ICHING, CD_SYS_KOREAN, BR_EXPLAIN_TRANSMISSION,
      "I Ching foundational in Korean culture",
      "The Korean national flag displays four I Ching trigrams (Heaven, "
      "Earth, Water, Fire). The text has been central to Korean philosophy "
      "and governance since the Three Kingdoms period." },

    { CD_SYS_PERSIAN, CD_SYS_EGYPTIAN, BR_EXPLAIN_CONVERGENT,
      "Both 12x30+5 solar calendars",
      "The Persian and Egyptian calendars independently arrived at twelve "
      "30-day months plus 5 epagomenal days. Persia later refined this with "
      "a leap day system; Egypt's 365-day calendar drifted 1 day per 4 years." },

    { CD_SYS_THAI, CD_SYS_JAPANESE, BR_EXPLAIN_CONVERGENT,
      "Both blend imported and indigenous counting",
      "Thai and Japanese calendars both adopted foreign systems (Indian and "
      "Chinese respectively) while retaining local cultural overlays — Thai "
      "Buddhist festivals and Japanese rokuyo/sekki." },

    /* --- Batch 5: Minimum coverage for critically under-connected --- */

    { CD_SYS_MYANMAR, CD_SYS_THAI, BR_EXPLAIN_TRANSMISSION,
      "Southeast Asian Theravada calendar siblings",
      "Both Myanmar and Thai calendars derive from the same Surya Siddhanta "
      "astronomical parameters, transmitted through Indian influence. Both "
      "serve Theravada Buddhist observance in neighboring kingdoms." },

    { CD_SYS_MYANMAR, CD_SYS_CHINESE, BR_EXPLAIN_CONVERGENT,
      "Independent lunisolar traditions",
      "Myanmar and Chinese calendars both use lunisolar months with "
      "intercalation, but from different astronomical traditions. Myanmar "
      "uses Indian (Surya Siddhanta); Chinese uses its own observation." },

    { CD_SYS_KOREAN, CD_SYS_BUDDHIST, BR_EXPLAIN_TRANSMISSION,
      "Korean Buddhism via Chinese transmission",
      "Buddhism reached Korea through China in the 4th century CE. Korean "
      "Buddhist observance follows East Asian lunar calendar conventions "
      "derived from the Chinese lunisolar system." },

    { CD_SYS_EGYPTIAN, CD_SYS_ASTRONOMY, BR_EXPLAIN_SHARED_SOURCE,
      "Sirius and the decan system",
      "The Egyptian calendar was originally anchored to the heliacal rising "
      "of Sirius (Sopdet). Egyptian astronomers invented the decan system — "
      "36 star groups marking 10-day intervals across the sky." },

    { CD_SYS_EGYPTIAN, CD_SYS_FRENCH, BR_EXPLAIN_TRANSMISSION,
      "French Republic adopted Egyptian structure",
      "The French Republican calendar directly adopted the Egyptian 12x30+5 "
      "structure, renaming months with nature imagery and replacing the "
      "7-day week with a rational 10-day decade." },

    { CD_SYS_CELTIC, CD_SYS_HEBREW, BR_EXPLAIN_CONVERGENT,
      "Independent agricultural-lunar traditions",
      "Celtic and Hebrew traditions both mark seasonal transitions with "
      "religious significance, but from entirely separate cultures. Celtic "
      "Sabbats mark quarter/cross-quarter solar events; Hebrew festivals "
      "follow a lunisolar agricultural cycle." },

    { CD_SYS_THAI, CD_SYS_MYANMAR, BR_EXPLAIN_TRANSMISSION,
      "Neighboring Theravada calendar traditions",
      "Thai and Myanmar calendars share Surya Siddhanta parameters and "
      "Theravada Buddhist purpose. Historical warfare and cultural exchange "
      "between Siam and Burma reinforced their calendrical similarities." },

    /* --- Batch 6: Inbox consumption (27 items from DEPTH/NERVE/MEGA) --- */

    { CD_SYS_ASTRONOMY, CD_SYS_CHINESE, BR_EXPLAIN_SHARED_SOURCE,
      "24 solar terms track the ecliptic",
      "Chinese 24 jié qì (solar terms) divide the ecliptic into 15-degree "
      "segments — direct astronomical observation of solar longitude. The "
      "calendar's intercalation rule (no major solar term = leap month) is "
      "an astronomical algorithm." },

    { CD_SYS_CHINESE, CD_SYS_CELTIC, BR_EXPLAIN_CONVERGENT,
      "Independent 8-point year from same astronomy",
      "Chinese jié qì and Celtic Sabbats independently mark the same 8 "
      "astronomical points: 4 solstices/equinoxes plus 4 cross-quarter days. "
      "Different cultures, different continents, same sky geometry." },

    { CD_SYS_HEBREW, CD_SYS_PERSIAN, BR_EXPLAIN_CONVERGENT,
      "Spring anchors: Nisan and Nowruz",
      "Both calendars anchor their year to spring. Nisan (Hebrew) begins "
      "near the vernal equinox via Metonic intercalation. Nowruz (Persian) "
      "IS the exact equinox moment. Independent solutions to the same "
      "seasonal observation." },

    { CD_SYS_ICHING, CD_SYS_TZOLKIN, BR_EXPLAIN_CONVERGENT,
      "Both assign qualitative meaning to each day",
      "The I Ching's 64 hexagrams describe states of change. The Tzolkin's "
      "260 kin describe daily energies. Both are oracular systems that give "
      "each time-position a unique character — China and Mesoamerica, no "
      "contact, same impulse." },

    { CD_SYS_ICHING, CD_SYS_BUDDHIST, BR_EXPLAIN_CONVERGENT,
      "Present moment as complete reality",
      "I Ching hexagrams describe the quality of NOW — not prediction but "
      "diagnosis. Buddhism says the present moment is all that truly exists. "
      "Both traditions center on the radical sufficiency of this instant." },

    { CD_SYS_ICHING, CD_SYS_ISLAMIC, BR_EXPLAIN_CONVERGENT,
      "Number-symbol systems for decoding meaning",
      "Chinese I Ching uses hexagram numbers to encode all states of change. "
      "Islamic Jafr uses numerical values of Quranic letters to reveal hidden "
      "patterns. Independent number-symbol traditions from different "
      "civilizations." },

    { CD_SYS_HINDU, CD_SYS_PERSIAN, BR_EXPLAIN_CONVERGENT,
      "Indo-Iranian spring celebrations",
      "Hindu Vasant Panchami and Persian Nowruz both celebrate spring's "
      "arrival. Sharing Indo-Iranian linguistic roots (Avestan/Sanskrit), "
      "these traditions diverged millennia ago yet both anchor renewal "
      "to the same astronomical event." },

    { CD_SYS_ASTRONOMY, CD_SYS_JAPANESE, BR_EXPLAIN_SHARED_SOURCE,
      "Sekki solar terms from ecliptic observation",
      "Japanese 24 sekki (solar terms) divide the ecliptic into 15-degree "
      "segments, adopted from Chinese astronomical tradition. Risshin "
      "(spring start) at solar longitude 315 degrees is a direct "
      "astronomical measurement." },

    { CD_SYS_ASTRONOMY, CD_SYS_KOREAN, BR_EXPLAIN_SHARED_SOURCE,
      "Korean solar terms via Chinese astronomy",
      "Korea adopted the Chinese system of 24 solar terms tracking solar "
      "longitude along the ecliptic. These astronomical markers govern "
      "agricultural timing and festival dates." },

    { CD_SYS_ASTRONOMY, CD_SYS_MYANMAR, BR_EXPLAIN_SHARED_SOURCE,
      "Surya Siddhanta astronomical parameters",
      "Myanmar's calendar uses solar and lunar parameters from the Indian "
      "Surya Siddhanta astronomical text. Eclipse calculations and "
      "intercalation rules are direct astronomical computations." },

    { CD_SYS_ASTROLOGY, CD_SYS_BUDDHIST, BR_EXPLAIN_CONVERGENT,
      "Cosmic renewal cycles",
      "The astrological Great Year (~26,000 years of precession) and "
      "Buddhist Kalpas (world-cycles of creation and destruction) both "
      "describe cosmic-scale patterns of renewal. Hindu Yugas parallel "
      "both. Independent traditions, convergent intuition." },

    { CD_SYS_CHINESE, CD_SYS_ISLAMIC, BR_EXPLAIN_CONVERGENT,
      "Silk Road coexistence, independent structures",
      "Chinese lunisolar and Islamic pure lunar calendars coexisted across "
      "Central and South Asia via the Silk Road. Despite centuries of "
      "cultural exchange, their calendar structures remained independent — "
      "one tracks the sun and moon, the other the moon alone." },
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
