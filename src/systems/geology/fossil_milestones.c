/* fossil_milestones.c — Key milestones in Earth's biological history.
 * Pure static const data. No malloc, no globals, no side effects.
 * Data sources: Wikipedia Timeline of Evolution (public domain),
 * paleontological consensus dates. */

#include "fossil_milestones.h"
#include <math.h>
#include <stddef.h>

/* --- Static milestone data (chronological, oldest first) --- */

static const fossil_milestone_t FOSSIL_MILESTONES[] = {
    { /* 0 */
        .id = 0,
        .name = "First Life",
        .ma = 3500.0,
        .what_appeared = "Stromatolites and prokaryotes",
        .what_changed = "Self-replicating chemistry becomes biology",
        .significance = "Life begins on Earth",
        .category = FOSSIL_CAT_ORIGIN
    },
    { /* 1 */
        .id = 1,
        .name = "Photosynthesis",
        .ma = 2700.0,
        .what_appeared = "Cyanobacteria",
        .what_changed = "Oxygen production begins transforming atmosphere",
        .significance = "Foundation for aerobic life and ozone layer",
        .category = FOSSIL_CAT_INNOVATION
    },
    { /* 2 */
        .id = 2,
        .name = "First Eukaryotes",
        .ma = 2100.0,
        .what_appeared = "Complex cells with nuclei",
        .what_changed = "Endosymbiosis creates mitochondria and organelles",
        .significance = "Cellular complexity enables all complex life",
        .category = FOSSIL_CAT_INNOVATION
    },
    { /* 3 */
        .id = 3,
        .name = "Sexual Reproduction",
        .ma = 1200.0,
        .what_appeared = "Genetic recombination",
        .what_changed = "Accelerated evolutionary adaptation through gene mixing",
        .significance = "Drives biodiversity and accelerates evolution",
        .category = FOSSIL_CAT_INNOVATION
    },
    { /* 4 */
        .id = 4,
        .name = "First Multicellular Life",
        .ma = 600.0,
        .what_appeared = "Ediacaran biota",
        .what_changed = "Cells cooperate as organisms",
        .significance = "Opens path to all complex body plans",
        .category = FOSSIL_CAT_ORIGIN
    },
    { /* 5 */
        .id = 5,
        .name = "Cambrian Explosion",
        .ma = 541.0,
        .what_appeared = "Almost all animal phyla",
        .what_changed = "Rapid diversification of body plans in ~20 million years",
        .significance = "Foundation of all modern animal groups",
        .category = FOSSIL_CAT_DIVERSIFICATION
    },
    { /* 6 */
        .id = 6,
        .name = "First Land Plants",
        .ma = 470.0,
        .what_appeared = "Bryophytes colonize land",
        .what_changed = "Photosynthesis moves onto continents",
        .significance = "Soil formation begins; terrestrial ecosystems emerge",
        .category = FOSSIL_CAT_COLONIZATION
    },
    { /* 7 */
        .id = 7,
        .name = "First Insects",
        .ma = 400.0,
        .what_appeared = "Hexapoda",
        .what_changed = "Arthropods diversify into most species-rich animal group",
        .significance = "Insects become dominant terrestrial invertebrates",
        .category = FOSSIL_CAT_ORIGIN
    },
    { /* 8 */
        .id = 8,
        .name = "First Forests",
        .ma = 385.0,
        .what_appeared = "Archaeopteris trees",
        .what_changed = "Tall woody plants create forest canopy ecosystems",
        .significance = "Carbon sequestration transforms atmosphere and climate",
        .category = FOSSIL_CAT_COLONIZATION
    },
    { /* 9 */
        .id = 9,
        .name = "First Tetrapods",
        .ma = 375.0,
        .what_appeared = "Tiktaalik — fish walk onto land",
        .what_changed = "Vertebrates colonize terrestrial environments",
        .significance = "Ancestor of all amphibians, reptiles, birds, mammals",
        .category = FOSSIL_CAT_COLONIZATION
    },
    { /* 10 */
        .id = 10,
        .name = "Late Devonian Extinction",
        .ma = 372.0,
        .what_appeared = "75% marine species lost",
        .what_changed = "Reef ecosystems collapse; placoderms vanish",
        .significance = "Second of the Big Five mass extinctions",
        .category = FOSSIL_CAT_EXTINCTION
    },
    { /* 11 */
        .id = 11,
        .name = "First Reptiles",
        .ma = 312.0,
        .what_appeared = "Amniotic egg — no water needed for reproduction",
        .what_changed = "Vertebrates fully independent of aquatic breeding",
        .significance = "Unlocks continental interiors for vertebrate life",
        .category = FOSSIL_CAT_INNOVATION
    },
    { /* 12 */
        .id = 12,
        .name = "Permian-Triassic Extinction",
        .ma = 252.0,
        .what_appeared = "96% marine, 70% land species lost",
        .what_changed = "Siberian Traps volcanism devastates biosphere",
        .significance = "The Great Dying — largest mass extinction in history",
        .category = FOSSIL_CAT_EXTINCTION
    },
    { /* 13 */
        .id = 13,
        .name = "First Dinosaurs",
        .ma = 230.0,
        .what_appeared = "Archosauria diversification",
        .what_changed = "Dinosaurs fill ecological niches left by Permian extinction",
        .significance = "Begin 165-million-year dominance of terrestrial ecosystems",
        .category = FOSSIL_CAT_DIVERSIFICATION
    },
    { /* 14 */
        .id = 14,
        .name = "First Mammals",
        .ma = 225.0,
        .what_appeared = "Small nocturnal insectivores",
        .what_changed = "Synapsids evolve warm blood, fur, lactation",
        .significance = "Mammalian lineage survives alongside dinosaurs",
        .category = FOSSIL_CAT_ORIGIN
    },
    { /* 15 */
        .id = 15,
        .name = "First Flowering Plants",
        .ma = 130.0,
        .what_appeared = "Angiosperms",
        .what_changed = "Co-evolution with pollinators transforms ecosystems",
        .significance = "Angiosperms become dominant plant group on Earth",
        .category = FOSSIL_CAT_INNOVATION
    },
    { /* 16 */
        .id = 16,
        .name = "K-Pg Extinction",
        .ma = 66.0,
        .what_appeared = "Dinosaurs end, mammals inherit",
        .what_changed = "Chicxulub asteroid impact ends Mesozoic Era",
        .significance = "Clears ecological space for mammalian radiation",
        .category = FOSSIL_CAT_EXTINCTION
    },
    { /* 17 */
        .id = 17,
        .name = "First Primates",
        .ma = 55.0,
        .what_appeared = "Plesiadapiformes",
        .what_changed = "Arboreal mammals develop grasping hands and binocular vision",
        .significance = "Primate lineage leading to humans begins",
        .category = FOSSIL_CAT_ORIGIN
    },
    { /* 18 */
        .id = 18,
        .name = "First Hominids",
        .ma = 7.0,
        .what_appeared = "Sahelanthropus tchadensis",
        .what_changed = "Bipedalism emerges in African apes",
        .significance = "Human lineage diverges from other great apes",
        .category = FOSSIL_CAT_ORIGIN
    },
    { /* 19 */
        .id = 19,
        .name = "First Stone Tools",
        .ma = 3.3,
        .what_appeared = "Lomekwian tools",
        .what_changed = "Intentional tool manufacture demonstrates planning",
        .significance = "Technology begins; culture can accumulate across generations",
        .category = FOSSIL_CAT_INNOVATION
    },
    { /* 20 */
        .id = 20,
        .name = "Homo sapiens",
        .ma = 0.3,
        .what_appeared = "Anatomically modern humans",
        .what_changed = "Language, abstract thought, symbolic culture emerge",
        .significance = "Species capable of understanding its own origins",
        .category = FOSSIL_CAT_ORIGIN
    }
};

#define FOSSIL_MILESTONE_COUNT \
    ((int)(sizeof(FOSSIL_MILESTONES) / sizeof(FOSSIL_MILESTONES[0])))

/* --- Category name lookup --- */

static const char *const CATEGORY_NAMES[] = {
    "Origin",
    "Innovation",
    "Extinction",
    "Colonization",
    "Diversification"
};

#define CATEGORY_NAME_COUNT \
    ((int)(sizeof(CATEGORY_NAMES) / sizeof(CATEGORY_NAMES[0])))

/* --- Invalid milestone (returned for bad lookups) --- */

static fossil_milestone_t fossil_invalid(void) {
    fossil_milestone_t m;
    m.id = -1;
    m.name = NULL;
    m.ma = 0.0;
    m.what_appeared = NULL;
    m.what_changed = NULL;
    m.significance = NULL;
    m.category = FOSSIL_CAT_ORIGIN;
    return m;
}

/* --- Public API --- */

int fossil_milestone_count(void) {
    return FOSSIL_MILESTONE_COUNT;
}

fossil_milestone_t fossil_milestone_get(int index) {
    if (index < 0 || index >= FOSSIL_MILESTONE_COUNT) {
        return fossil_invalid();
    }
    return FOSSIL_MILESTONES[index];
}

const char *fossil_milestone_name(int index) {
    if (index < 0 || index >= FOSSIL_MILESTONE_COUNT) {
        return "?";
    }
    return FOSSIL_MILESTONES[index].name;
}

fossil_milestone_t fossil_milestone_at(double ma) {
    fossil_milestone_t best = fossil_invalid();
    double best_diff = -1.0;

    for (int i = 0; i < FOSSIL_MILESTONE_COUNT; i++) {
        double diff = fabs(ma - FOSSIL_MILESTONES[i].ma);
        if (best_diff < 0.0 || diff < best_diff) {
            best_diff = diff;
            best = FOSSIL_MILESTONES[i];
        }
    }

    return best;
}

int fossil_milestones_in_range(double start_ma, double end_ma,
                                int *out_indices, int max_results) {
    if (out_indices == NULL || max_results <= 0) {
        return 0;
    }

    int found = 0;
    for (int i = 0; i < FOSSIL_MILESTONE_COUNT && found < max_results; i++) {
        /* Milestone's ma falls within [end_ma, start_ma]
         * (larger Ma = further back in time, so start_ma >= end_ma) */
        if (FOSSIL_MILESTONES[i].ma <= start_ma &&
            FOSSIL_MILESTONES[i].ma >= end_ma) {
            out_indices[found++] = i;
        }
    }

    return found;
}

int fossil_milestones_of_category(fossil_category_t category) {
    int count = 0;
    for (int i = 0; i < FOSSIL_MILESTONE_COUNT; i++) {
        if (FOSSIL_MILESTONES[i].category == category) {
            count++;
        }
    }
    return count;
}

const char *fossil_category_name(fossil_category_t category) {
    int c = (int)category;
    if (c < 0 || c >= CATEGORY_NAME_COUNT) {
        return "?";
    }
    return CATEGORY_NAMES[c];
}

bool fossil_is_extinction(double ma, double tolerance_ma) {
    for (int i = 0; i < FOSSIL_MILESTONE_COUNT; i++) {
        if (FOSSIL_MILESTONES[i].category == FOSSIL_CAT_EXTINCTION &&
            fabs(ma - FOSSIL_MILESTONES[i].ma) <= tolerance_ma) {
            return true;
        }
    }
    return false;
}
