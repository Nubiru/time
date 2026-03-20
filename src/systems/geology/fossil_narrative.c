/* fossil_narrative.c — Display-ready narrative arcs for life on Earth.
 * Pure static const data. No malloc, no globals, no side effects.
 * Data sources: paleontological consensus, geological time scale (ICS 2023). */

#include "fossil_narrative.h"
#include <stddef.h>
#include <stdio.h>

/* --- Static era data (chronological, oldest first) --- */

static const fn_era_t ERAS[FN_ERA_COUNT] = {
    { /* 0 */
        .index = 0,
        .name = "Primordial Soup",
        .start_ma = 4000.0,
        .end_ma = 3500.0,
        .narrative =
            "For five hundred million years after Earth cooled enough for liquid water, "
            "the oceans were a vast chemistry experiment. Lightning, ultraviolet radiation, "
            "and hydrothermal vents drove reactions that built amino acids, nucleotides, "
            "and lipid membranes from simple molecules. Somewhere in this primordial "
            "broth, RNA molecules began copying themselves \xe2\x80\x94 the first spark of "
            "self-replication that would eventually become life.",
        .key_species = "RNA replicators, protocells, lipid vesicles",
        .rise_story =
            "Chemical evolution concentrated organic molecules in tidal pools "
            "and deep-sea vents until self-replicating RNA emerged from the noise",
        .fall_story =
            "The rise of true cellular life (prokaryotes) overtook and enclosed "
            "the free-floating chemistry, ending the purely chemical era"
    },
    { /* 1 */
        .index = 1,
        .name = "Age of Bacteria",
        .start_ma = 3500.0,
        .end_ma = 2100.0,
        .narrative =
            "For over a billion years, bacteria were the only life on Earth and they "
            "transformed the planet utterly. Cyanobacteria invented oxygenic photosynthesis, "
            "slowly poisoning the atmosphere with oxygen \xe2\x80\x94 a catastrophe for anaerobic life "
            "but the foundation for everything that followed. Stromatolite reefs lined "
            "shallow seas while methanogens thrived in the deep. These microscopic architects "
            "created the oxygen we breathe today.",
        .key_species = "Cyanobacteria, methanogens, stromatolites, purple sulfur bacteria",
        .rise_story =
            "Prokaryotes dominated every available niche from boiling vents to "
            "frozen surfaces, inventing photosynthesis and nitrogen fixation",
        .fall_story =
            "The Great Oxidation Event (2.4 Ga) set the stage for eukaryotes, "
            "which engulfed bacteria to create mitochondria and chloroplasts"
    },
    { /* 2 */
        .index = 2,
        .name = "Age of Protists",
        .start_ma = 2100.0,
        .end_ma = 600.0,
        .narrative =
            "The longest era in biological history saw the most profound innovations "
            "in cellular architecture. Eukaryotic cells engulfed bacteria to create "
            "mitochondria and chloroplasts \xe2\x80\x94 endosymbiosis, the greatest merger in the "
            "history of life. Sexual reproduction appeared, shuffling genes in new ways. "
            "Then cells learned to cooperate: the first multicellular organisms emerged, "
            "setting the stage for the Cambrian explosion to come.",
        .key_species = "Amoebae, foraminifera, red algae, green algae, slime molds",
        .rise_story =
            "Eukaryotes gained enormous versatility through endosymbiosis, "
            "sexual reproduction, and increasing cell specialization",
        .fall_story =
            "Multicellular body plans became so successful that single-celled "
            "protists yielded dominance to animal and plant kingdoms"
    },
    { /* 3 */
        .index = 3,
        .name = "Age of Invertebrates",
        .start_ma = 600.0,
        .end_ma = 480.0,
        .narrative =
            "In one of evolution's most dramatic chapters, the Ediacaran fauna gave way to "
            "the Cambrian explosion: in barely twenty million years, nearly every animal "
            "phylum appeared in the fossil record. Trilobites patrolled the seafloor with "
            "compound eyes. Anomalocaris hunted with grasping appendages. The arms race "
            "between predator and prey drove the evolution of shells, spines, and speed \xe2\x80\x94 "
            "an innovation arms race unmatched in Earth's history.",
        .key_species = "Trilobites, Anomalocaris, Hallucigenia, Opabinia, brachiopods",
        .rise_story =
            "Rising oxygen levels and the evolution of eyes triggered an "
            "evolutionary arms race that produced all modern animal body plans",
        .fall_story =
            "Vertebrates \xe2\x80\x94 fish with internal skeletons \xe2\x80\x94 began to outcompete "
            "invertebrates for top-predator roles in the oceans"
    },
    { /* 4 */
        .index = 4,
        .name = "Age of Fish",
        .start_ma = 480.0,
        .end_ma = 360.0,
        .narrative =
            "The oceans belonged to fish for over a hundred million years. First came the "
            "jawless ostracoderms in their bony armor, then the revolution of jaws transformed "
            "feeding forever. Placoderms like the ten-meter Dunkleosteus became the first "
            "mega-predators. Sharks perfected their design so well they have barely changed "
            "since. Meanwhile, on land, the first plants crept from water's edge, and "
            "lobe-finned fish began the long walk toward becoming us.",
        .key_species = "Dunkleosteus, Coelacanth, early sharks, ostracoderms, Cooksonia",
        .rise_story =
            "The evolution of jaws from gill arches gave fish devastating "
            "predatory power, and bony skeletons provided structural advantage",
        .fall_story =
            "The Late Devonian extinction devastated reef ecosystems and "
            "placoderms, while lobe-finned fish crawled onto land as tetrapods"
    },
    { /* 5 */
        .index = 5,
        .name = "Age of Amphibians",
        .start_ma = 360.0,
        .end_ma = 300.0,
        .narrative =
            "Life's greatest migration was underway: from water to land. Tiktaalik, with its "
            "fish body and proto-wrists, bridged the gap. The first true tetrapods ventured "
            "into swampy forests of giant ferns and horsetails. Carboniferous coal swamps "
            "teemed with life: dragonflies with two-foot wingspans hunted through canopies "
            "while meter-long millipedes crawled below. Oxygen levels peaked at 35%, fueling "
            "the age of giants.",
        .key_species = "Tiktaalik, Ichthyostega, Meganeura, Arthropleura, giant lycopods",
        .rise_story =
            "High oxygen levels and lush swamp forests provided abundant food "
            "and habitat for the first land vertebrates and giant arthropods",
        .fall_story =
            "Drying climates collapsed the coal swamps, and the evolution of "
            "the amniotic egg freed reptiles from dependence on water"
    },
    { /* 6 */
        .index = 6,
        .name = "Age of Reptiles",
        .start_ma = 300.0,
        .end_ma = 66.0,
        .narrative =
            "For 234 million years, reptiles ruled every ecosystem on Earth. Dinosaurs "
            "thundered across Pangaea and its fragments, from pole to pole. Pterosaurs "
            "commanded the skies with wingspans up to twelve meters. Ichthyosaurs and "
            "plesiosaurs dominated the seas. Flowers appeared and transformed terrestrial "
            "ecosystems. Feathered theropods took flight, becoming birds. This was the "
            "longest and most spectacular reign of any animal group in Earth's history.",
        .key_species = "T. rex, Brachiosaurus, Triceratops, Stegosaurus, Archaeopteryx, Pteranodon",
        .rise_story =
            "After the Permian extinction wiped out 96% of species, archosaurs "
            "filled the empty world and evolved into the dinosaurs",
        .fall_story =
            "The Chicxulub asteroid impact 66 million years ago ended the "
            "non-avian dinosaurs, though birds carry their lineage forward"
    },
    { /* 7 */
        .index = 7,
        .name = "Age of Mammals",
        .start_ma = 66.0,
        .end_ma = 0.0,
        .narrative =
            "From the ashes of the asteroid impact, tiny mammals emerged into a devastated "
            "world and radiated into every niche the dinosaurs left behind. Within ten million "
            "years, whales returned to the sea, bats took to the air, and the first primates "
            "appeared in tropical forests. Grasslands spread, horses galloped, and elephants "
            "grew enormous. Then, in Africa, a line of apes stood upright, made tools, "
            "mastered fire, and began to ask where they came from.",
        .key_species = "Whales, horses, elephants, great apes, Homo sapiens",
        .rise_story =
            "The K-Pg extinction cleared every large-body niche, and mammals "
            "diversified explosively to fill them within ten million years",
        .fall_story = "Ongoing \xe2\x80\x94 we are living in the Age of Mammals"
    }
};

/* --- Static dinosaur event data (chronological, oldest first) --- */

static const fn_dino_event_t DINO_EVENTS[FN_DINO_EVENT_COUNT] = {
    { /* 0 */
        .index = 0,
        .title = "First Archosaurs",
        .ma = 250.0,
        .story =
            "In the aftermath of the Permian extinction, a group of reptiles called archosaurs "
            "began to diversify across the emptied landscape. These ancestors of both dinosaurs "
            "and crocodilians developed an upright gait, efficient lungs, and increasingly "
            "active metabolisms. Two lineages diverged: one toward crocodilians, the other "
            "toward dinosaurs and eventually birds.",
        .significance =
            "Archosaurs established the body plan and physiology that would "
            "enable dinosaurs to dominate for 165 million years"
    },
    { /* 1 */
        .index = 1,
        .title = "Dawn of Dinosaurs",
        .ma = 230.0,
        .story =
            "In the Late Triassic of what is now Argentina, small bipedal predators like "
            "Eoraptor and Herrerasaurus appeared among a world still dominated by other "
            "archosaurs. These early dinosaurs were modest creatures, no larger than a dog, "
            "but their upright posture and efficient breathing gave them an edge. They were "
            "bit players in a Triassic world \xe2\x80\x94 not yet the rulers they would become.",
        .significance =
            "The first true dinosaurs appeared as minor components of their "
            "ecosystems, awaiting the extinction that would clear their path"
    },
    { /* 2 */
        .index = 2,
        .title = "Triassic-Jurassic Extinction",
        .ma = 201.0,
        .story =
            "Massive volcanic eruptions from the Central Atlantic Magmatic Province flooded "
            "the atmosphere with CO2 and sulfur. Eighty percent of species vanished in the "
            "resulting climate chaos. Crucially, most of the large crurotarsans \xe2\x80\x94 the "
            "crocodilian relatives that had dominated alongside early dinosaurs \xe2\x80\x94 were "
            "wiped out. The dinosaurs survived and inherited a world emptied of competitors.",
        .significance =
            "This extinction removed dinosaur competitors and opened every "
            "major ecological niche for dinosaurian radiation"
    },
    { /* 3 */
        .index = 3,
        .title = "Giants of the Jurassic",
        .ma = 160.0,
        .story =
            "With competitors gone, dinosaurs grew to staggering sizes. Brachiosaurus raised "
            "its head thirteen meters into the canopy. Diplodocus stretched over thirty meters "
            "from nose to tail whip. Allosaurus hunted in packs, bringing down prey many times "
            "its size. The arms race between predator and prey drove both to ever more extreme "
            "proportions in what became the golden age of the giant reptiles.",
        .significance =
            "The Jurassic giants represent the peak of terrestrial vertebrate "
            "body size, a record that may never be surpassed"
    },
    { /* 4 */
        .index = 4,
        .title = "First Bird",
        .ma = 150.0,
        .story =
            "In the limestone lagoons of Late Jurassic Bavaria, Archaeopteryx preserved a "
            "snapshot of one of evolution's most remarkable transitions. With the teeth and "
            "tail of a small dinosaur but the feathered wings of a bird, it was a living "
            "bridge between two worlds. Feathered dinosaurs had been diversifying for millions "
            "of years, and Archaeopteryx shows that powered flight emerged from gliding theropods.",
        .significance =
            "Archaeopteryx proved that birds are living dinosaurs, connecting "
            "the Mesozoic world directly to the 10,000 bird species alive today"
    },
    { /* 5 */
        .index = 5,
        .title = "Cretaceous Bloom",
        .ma = 125.0,
        .story =
            "Flowering plants appeared and spread across every continent, transforming "
            "terrestrial ecosystems forever. Co-evolution with insect pollinators created "
            "an explosion of new species on both sides. Forests changed from conifer-dominated "
            "to angiosperm-rich landscapes. Herbivorous dinosaurs adapted to this new food "
            "source, and the entire food web restructured around the innovation of the flower.",
        .significance =
            "Flowering plants restructured every land ecosystem and created "
            "the pollinator partnerships that sustain most food production today"
    },
    { /* 6 */
        .index = 6,
        .title = "Spinosaurus: River Hunter",
        .ma = 95.0,
        .story =
            "In the rivers of Cretaceous North Africa, a predator larger than T. rex "
            "stalked the waterways. Spinosaurus aegyptiacus, with its crocodile-like snout, "
            "conical teeth, and massive sail, was the first known semi-aquatic dinosaur. "
            "At over fifteen meters long, it hunted giant sawfish and coelacanths. Its "
            "discovery rewrote our understanding of how diverse dinosaur lifestyles truly were.",
        .significance =
            "Spinosaurus proved dinosaurs conquered aquatic habitats too, "
            "showing their adaptability extended beyond land into rivers and lakes"
    },
    { /* 7 */
        .index = 7,
        .title = "T. rex: Tyrant King",
        .ma = 68.0,
        .story =
            "Tyrannosaurus rex was the apex predator of Late Cretaceous North America: twelve "
            "meters long, six tons heavy, with jaws that could crush bone. Its binocular vision "
            "was sharper than a hawk's, and its brain was large for a dinosaur, suggesting keen "
            "intelligence. Each tooth was the size of a banana and could exert a bite force of "
            "over 12,000 pounds. It was evolution's masterwork in predatory engineering.",
        .significance =
            "T. rex represents the pinnacle of theropod evolution, the most "
            "powerful terrestrial predator Earth has ever produced"
    },
    { /* 8 */
        .index = 8,
        .title = "Triceratops: Last Stand",
        .ma = 67.0,
        .story =
            "Among the very last dinosaurs to walk the Earth, Triceratops was a nine-meter "
            "tank of bone and muscle. Its three horns and massive frill served both as defense "
            "against T. rex and as display for mates. Herds of Triceratops roamed the "
            "floodplains of western North America in the final million years before the "
            "impact. Their fossils are so common in the Hell Creek Formation that they paint "
            "a picture of a thriving world about to end.",
        .significance =
            "Triceratops proves dinosaur ecosystems were healthy and diverse "
            "right up to the asteroid impact \xe2\x80\x94 extinction was sudden, not gradual"
    },
    { /* 9 */
        .index = 9,
        .title = "The Asteroid",
        .ma = 66.0,
        .story =
            "A rock ten kilometers wide struck the Yucatan Peninsula at twenty kilometers "
            "per second, releasing the energy of a billion nuclear weapons. The impact ignited "
            "global firestorms, triggered mega-tsunamis, and launched so much debris into the "
            "atmosphere that sunlight was blocked for years. Photosynthesis collapsed, food "
            "chains disintegrated, and seventy-six percent of all species vanished \xe2\x80\x94 including "
            "every non-avian dinosaur.",
        .significance =
            "The Chicxulub impact ended 165 million years of dinosaur dominance "
            "and opened the door for mammals to inherit the Earth"
    }
};

/* --- Invalid return values --- */

static fn_era_t fn_era_invalid(void) {
    fn_era_t e;
    e.index = -1;
    e.name = NULL;
    e.start_ma = 0.0;
    e.end_ma = 0.0;
    e.narrative = NULL;
    e.key_species = NULL;
    e.rise_story = NULL;
    e.fall_story = NULL;
    return e;
}

static fn_dino_event_t fn_dino_invalid(void) {
    fn_dino_event_t d;
    d.index = -1;
    d.title = NULL;
    d.ma = 0.0;
    d.story = NULL;
    d.significance = NULL;
    return d;
}

/* --- Public API --- */

int fn_era_count(void) {
    return FN_ERA_COUNT;
}

fn_era_t fn_era_get(int index) {
    if (index < 0 || index >= FN_ERA_COUNT) {
        return fn_era_invalid();
    }
    return ERAS[index];
}

fn_era_t fn_era_at_time(double ma) {
    for (int i = 0; i < FN_ERA_COUNT; i++) {
        /* start_ma exclusive (boundary belongs to next era), end_ma inclusive
         * Exception: first era (index 0) has start_ma inclusive */
        if (i == 0) {
            if (ma <= ERAS[i].start_ma && ma > ERAS[i].end_ma) {
                return ERAS[i];
            }
        } else {
            if (ma <= ERAS[i].start_ma && ma > ERAS[i].end_ma) {
                return ERAS[i];
            }
        }
    }
    /* Check if exactly at the very end (0.0 Ma for Age of Mammals) */
    if (ma >= 0.0 && ma <= ERAS[FN_ERA_COUNT - 1].end_ma + 0.001) {
        return ERAS[FN_ERA_COUNT - 1];
    }
    return fn_era_invalid();
}

const char *fn_era_name(int index) {
    if (index < 0 || index >= FN_ERA_COUNT) {
        return "?";
    }
    return ERAS[index].name;
}

fn_dino_event_t fn_dino_event_get(int index) {
    if (index < 0 || index >= FN_DINO_EVENT_COUNT) {
        return fn_dino_invalid();
    }
    return DINO_EVENTS[index];
}

int fn_dino_event_count(void) {
    return FN_DINO_EVENT_COUNT;
}

int fn_era_format(int index, char *buf, int buf_size) {
    if (buf == NULL || buf_size <= 0) {
        return 0;
    }
    if (index < 0 || index >= FN_ERA_COUNT) {
        return 0;
    }
    const fn_era_t *e = &ERAS[index];
    int written = snprintf(buf, (size_t)buf_size,
        "%s (%.0f-%.0f Ma)\n%s\nKey species: %s",
        e->name, e->start_ma, e->end_ma, e->narrative, e->key_species);
    if (written < 0) {
        return 0;
    }
    if (written >= buf_size) {
        /* Truncated: ensure null termination, return actual written */
        buf[buf_size - 1] = '\0';
        return buf_size - 1;
    }
    return written;
}

int fn_dino_format(int index, char *buf, int buf_size) {
    if (buf == NULL || buf_size <= 0) {
        return 0;
    }
    if (index < 0 || index >= FN_DINO_EVENT_COUNT) {
        return 0;
    }
    const fn_dino_event_t *d = &DINO_EVENTS[index];
    int written = snprintf(buf, (size_t)buf_size,
        "%s (%.0f Ma)\n%s\n%s",
        d->title, d->ma, d->story, d->significance);
    if (written < 0) {
        return 0;
    }
    if (written >= buf_size) {
        buf[buf_size - 1] = '\0';
        return buf_size - 1;
    }
    return written;
}
