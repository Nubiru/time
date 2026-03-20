/* earth_voyage.c — Earth History Voyage: chapter-based narrative of 4.54 Ga.
 * Pure static const data. No malloc, no globals, no side effects.
 * 12 chapters telling Earth's story from molten birth to human civilization.
 * Data synthesized from geological consensus, ICS chart, paleontology. */

#include "earth_voyage.h"
#include <stddef.h>
#include <stdio.h>

/* --- Static chapter data (chronological, oldest first) --- */

static const ev_chapter_t EV_CHAPTERS[EV_CHAPTER_COUNT] = {
    { /* 0 */
        .index = 0,
        .title = "Birth of Earth",
        .start_ma = 4540.0,
        .end_ma = 4000.0,
        .narrative =
            "Earth coalesces from the solar nebula, a molten ball of rock "
            "and metal bombarded by planetesimals. A Mars-sized body named "
            "Theia strikes the young Earth, ejecting debris that forms the "
            "Moon. The surface is a magma ocean under a steam atmosphere, "
            "slowly cooling as the Late Heavy Bombardment rains asteroids "
            "for hundreds of millions of years.",
        .visual_hint = "molten_surface",
        .event_count = 5,
        .events = {
            "Solar nebula accretion (4540 Ma)",
            "Core-mantle differentiation (4530 Ma)",
            "Theia impact and Moon formation (4510 Ma)",
            "Magma ocean crystallization (4400 Ma)",
            "Late Heavy Bombardment begins (4100 Ma)"
        }
    },
    { /* 1 */
        .index = 1,
        .title = "The First Oceans",
        .start_ma = 4000.0,
        .end_ma = 3500.0,
        .narrative =
            "The surface cools enough for water vapor to condense into vast "
            "oceans. The first protocontinental crust forms as volcanic "
            "islands rise from the global sea. The atmosphere is thick with "
            "carbon dioxide and nitrogen, devoid of free oxygen. Zircon "
            "crystals from this era prove liquid water existed, setting the "
            "stage for the chemistry of life.",
        .visual_hint = "ocean_world",
        .event_count = 4,
        .events = {
            "First liquid water oceans (4000 Ma)",
            "Oldest surviving zircon crystals (4400 Ma)",
            "First protocontinental crust (3800 Ma)",
            "Prebiotic chemistry in hydrothermal vents (3700 Ma)"
        }
    },
    { /* 2 */
        .index = 2,
        .title = "Life Begins",
        .start_ma = 3500.0,
        .end_ma = 2500.0,
        .narrative =
            "Stromatolites — layered mats of cyanobacteria — appear in "
            "shallow seas, the oldest evidence of life on Earth. "
            "Photosynthesis evolves, and cyanobacteria begin to produce "
            "oxygen as a waste product, though it is immediately consumed "
            "by dissolved iron. The continents Vaalbara and Ur take shape "
            "as Earth's first landmasses.",
        .visual_hint = "stromatolite_seas",
        .event_count = 5,
        .events = {
            "Oldest stromatolites (3500 Ma)",
            "First prokaryotic cells (3500 Ma)",
            "Supercontinent Vaalbara forms (3300 Ma)",
            "Oxygenic photosynthesis evolves (2700 Ma)",
            "Supercontinent Kenorland assembles (2700 Ma)"
        }
    },
    { /* 3 */
        .index = 3,
        .title = "The Great Oxidation",
        .start_ma = 2500.0,
        .end_ma = 1800.0,
        .narrative =
            "Free oxygen accumulates in the atmosphere for the first time, "
            "poisoning most anaerobic life in Earth's first mass extinction "
            "— the Oxygen Catastrophe. Iron in the oceans rusts, creating "
            "the banded iron formations that we mine today. The Huronian "
            "glaciation buries the planet in ice. Eukaryotic cells appear, "
            "complex life born from endosymbiosis.",
        .visual_hint = "rusting_world",
        .event_count = 5,
        .events = {
            "Great Oxidation Event begins (2400 Ma)",
            "Banded iron formations deposited (2500-1800 Ma)",
            "Huronian glaciation (2400-2100 Ma)",
            "First eukaryotic cells (2100 Ma)",
            "Supercontinent Columbia assembles (1800 Ma)"
        }
    },
    { /* 4 */
        .index = 4,
        .title = "The Boring Billion",
        .start_ma = 1800.0,
        .end_ma = 720.0,
        .narrative =
            "For over a billion years, Earth enters a period of apparent "
            "stability — the Boring Billion. But beneath the calm, "
            "evolution quietly innovates: sexual reproduction appears, "
            "shuffling genes in new combinations. Multicellular organisms "
            "emerge. The supercontinent Rodinia forms, then begins to "
            "break apart, setting the stage for dramatic change.",
        .visual_hint = "stable_continents",
        .event_count = 5,
        .events = {
            "Supercontinent Columbia stable (1800-1500 Ma)",
            "Sexual reproduction evolves (1200 Ma)",
            "Supercontinent Rodinia assembles (1100 Ma)",
            "First multicellular algae (1000 Ma)",
            "Rodinia begins rifting (750 Ma)"
        }
    },
    { /* 5 */
        .index = 5,
        .title = "Snowball Earth",
        .start_ma = 720.0,
        .end_ma = 540.0,
        .narrative =
            "As Rodinia breaks apart, massive volcanic CO2 drawdown "
            "triggers the Cryogenian glaciations — Snowball Earth events "
            "where ice reaches the equator. When the ice finally melts, "
            "the Ediacaran period blooms with the first complex animals: "
            "soft-bodied creatures unlike anything alive today. Life is "
            "poised for its greatest leap.",
        .visual_hint = "ice_planet",
        .event_count = 5,
        .events = {
            "Sturtian glaciation (717-660 Ma)",
            "Marinoan glaciation (650-635 Ma)",
            "Ediacaran biota appears (575 Ma)",
            "First animals with bilateral symmetry (555 Ma)",
            "Supercontinent Pannotia forms briefly (600 Ma)"
        }
    },
    { /* 6 */
        .index = 6,
        .title = "The Cambrian Explosion",
        .start_ma = 540.0,
        .end_ma = 420.0,
        .narrative =
            "In a burst of creativity spanning just 20 million years, "
            "nearly all modern animal phyla appear in the fossil record. "
            "Trilobites dominate the seas, the first vertebrates swim "
            "alongside bizarre creatures like Anomalocaris. Eyes evolve, "
            "triggering an evolutionary arms race between predators and "
            "prey that shapes all animal life to come.",
        .visual_hint = "cambrian_seas",
        .event_count = 6,
        .events = {
            "Cambrian Explosion begins (541 Ma)",
            "Trilobites diversify (521 Ma)",
            "First vertebrates — jawless fish (500 Ma)",
            "Late Ordovician extinction (445 Ma)",
            "First land plants colonize shores (470 Ma)",
            "First jawed fish (430 Ma)"
        }
    },
    { /* 7 */
        .index = 7,
        .title = "Life Conquers Land",
        .start_ma = 420.0,
        .end_ma = 300.0,
        .narrative =
            "Plants spread across the continents, growing from mosses "
            "into towering forests that pump oxygen to record levels. "
            "Insects take to the air — dragonflies with wingspans of "
            "70 centimeters. Fish crawl onto land, evolving into the "
            "first amphibians. The supercontinent Pangaea begins to "
            "assemble as Gondwana and Laurussia collide.",
        .visual_hint = "ancient_forests",
        .event_count = 6,
        .events = {
            "First insects (400 Ma)",
            "Late Devonian extinction (372 Ma)",
            "First seed plants (360 Ma)",
            "First amphibians walk on land (365 Ma)",
            "Carboniferous coal forests (350-300 Ma)",
            "Giant insects — high oxygen era (320 Ma)"
        }
    },
    { /* 8 */
        .index = 8,
        .title = "Age of Reptiles Begins",
        .start_ma = 300.0,
        .end_ma = 200.0,
        .narrative =
            "Pangaea stands assembled, the largest supercontinent since "
            "Rodinia. Reptiles free themselves from water with the "
            "amniotic egg. Then catastrophe: the Permian-Triassic "
            "extinction wipes out 96 percent of marine species and 70 "
            "percent of land vertebrates — the worst mass extinction in "
            "Earth's history. From the survivors, the first dinosaurs "
            "and the first mammals emerge.",
        .visual_hint = "pangaea_desert",
        .event_count = 6,
        .events = {
            "Pangaea fully assembled (300 Ma)",
            "First reptiles diversify (310 Ma)",
            "Permian-Triassic extinction (252 Ma)",
            "First dinosaurs appear (233 Ma)",
            "First mammals appear (225 Ma)",
            "Triassic-Jurassic extinction (201 Ma)"
        }
    },
    { /* 9 */
        .index = 9,
        .title = "Dinosaur Dominion",
        .start_ma = 200.0,
        .end_ma = 66.0,
        .narrative =
            "Dinosaurs reign supreme for 135 million years across the "
            "Jurassic and Cretaceous periods. Pangaea splits into "
            "Laurasia and Gondwana, then fragments further toward modern "
            "continents. The Atlantic Ocean opens. Flowering plants "
            "transform landscapes and co-evolve with insects. Birds "
            "evolve from theropod dinosaurs. Tyrannosaurus rex and "
            "Triceratops roam the final Cretaceous forests.",
        .visual_hint = "dinosaur_world",
        .event_count = 6,
        .events = {
            "Pangaea breakup accelerates (180 Ma)",
            "First birds evolve from theropods (150 Ma)",
            "Atlantic Ocean begins opening (130 Ma)",
            "Flowering plants appear (125 Ma)",
            "Tyrannosaurus rex era (68 Ma)",
            "Peak dinosaur diversity (75 Ma)"
        }
    },
    { /* 10 */
        .index = 10,
        .title = "The Impact",
        .start_ma = 66.0,
        .end_ma = 23.0,
        .narrative =
            "A 10-kilometer asteroid strikes the Yucatan Peninsula, "
            "ending the Cretaceous and killing all non-avian dinosaurs. "
            "Mammals, freed from dinosaur competition, diversify "
            "explosively. Whales return to the sea, bats take to the "
            "air, primates climb into the trees. India collides with "
            "Asia, raising the Himalayas. Grasslands spread across "
            "the cooling continents.",
        .visual_hint = "asteroid_aftermath",
        .event_count = 6,
        .events = {
            "Chicxulub asteroid impact (66 Ma)",
            "K-Pg mass extinction (66 Ma)",
            "Mammalian radiation begins (65 Ma)",
            "First primates appear (55 Ma)",
            "India-Asia collision / Himalayas rise (50 Ma)",
            "Grasslands spread globally (30 Ma)"
        }
    },
    { /* 11 */
        .index = 11,
        .title = "Rise of Humanity",
        .start_ma = 23.0,
        .end_ma = 0.0,
        .narrative =
            "The Miocene gives rise to the great apes. In Africa, "
            "hominids diverge from other primates, walking upright on "
            "the savanna. Ice ages sculpt the Pleistocene landscape as "
            "glaciers advance and retreat. Homo sapiens emerges 300,000 "
            "years ago, develops language, art, agriculture, and "
            "civilization — transforming Earth's surface in a geological "
            "instant.",
        .visual_hint = "human_dawn",
        .event_count = 6,
        .events = {
            "First great apes (20 Ma)",
            "Hominids diverge in Africa (7 Ma)",
            "First stone tools (3.3 Ma)",
            "Pleistocene ice ages begin (2.6 Ma)",
            "Homo sapiens appears (0.3 Ma)",
            "Agriculture and civilization (0.01 Ma)"
        }
    }
};

/* --- Invalid chapter sentinel --- */

static const ev_chapter_t EV_INVALID = {
    .index = -1,
    .title = NULL,
    .start_ma = 0.0,
    .end_ma = 0.0,
    .narrative = NULL,
    .visual_hint = NULL,
    .event_count = 0,
    .events = { NULL, NULL, NULL, NULL, NULL, NULL }
};

/* --- Public API --- */

ev_chapter_t ev_chapter_get(int index)
{
    if (index < 0 || index >= EV_CHAPTER_COUNT) {
        return EV_INVALID;
    }
    return EV_CHAPTERS[index];
}

int ev_chapter_count(void)
{
    return EV_CHAPTER_COUNT;
}

ev_chapter_t ev_chapter_at_time(double ma)
{
    if (ma < 0.0 || ma > EV_CHAPTERS[0].start_ma) {
        return EV_INVALID;
    }
    /* 0.0 Ma (present) belongs to last chapter */
    if (ma == 0.0) {
        return EV_CHAPTERS[EV_CHAPTER_COUNT - 1];
    }
    for (int i = 0; i < EV_CHAPTER_COUNT; i++) {
        /* Chapter covers (end_ma, start_ma] — start_ma excluded (belongs
         * to previous/older chapter) except for the very first chapter. */
        if (i == 0) {
            /* First chapter: includes its own start_ma */
            if (ma > EV_CHAPTERS[i].end_ma && ma <= EV_CHAPTERS[i].start_ma) {
                return EV_CHAPTERS[i];
            }
        } else {
            if (ma > EV_CHAPTERS[i].end_ma && ma <= EV_CHAPTERS[i].start_ma) {
                return EV_CHAPTERS[i];
            }
        }
    }
    return EV_INVALID;
}

const char *ev_chapter_title(int index)
{
    if (index < 0 || index >= EV_CHAPTER_COUNT) {
        return "?";
    }
    return EV_CHAPTERS[index].title;
}

int ev_chapter_format(int index, char *buf, int buf_size)
{
    if (buf == NULL || buf_size <= 0 || index < 0 || index >= EV_CHAPTER_COUNT) {
        return 0;
    }
    const ev_chapter_t *ch = &EV_CHAPTERS[index];
    int written = snprintf(buf, (size_t)buf_size,
        "Chapter %d: %s\n"
        "Time: %.1f - %.1f Ma\n"
        "Visual: %s\n"
        "\n%s\n"
        "\nKey events:\n",
        ch->index, ch->title,
        ch->start_ma, ch->end_ma,
        ch->visual_hint,
        ch->narrative);

    if (written < 0) {
        buf[0] = '\0';
        return 0;
    }
    if (written >= buf_size) {
        /* Truncated — null-terminate and return what fit */
        buf[buf_size - 1] = '\0';
        return buf_size - 1;
    }

    for (int e = 0; e < ch->event_count && written < buf_size - 1; e++) {
        int added = snprintf(buf + written, (size_t)(buf_size - written),
            "  - %s\n", ch->events[e]);
        if (added < 0) {
            break;
        }
        if (added >= buf_size - written) {
            buf[buf_size - 1] = '\0';
            return buf_size - 1;
        }
        written += added;
    }

    return written;
}
