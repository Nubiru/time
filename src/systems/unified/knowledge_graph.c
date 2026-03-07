/* knowledge_graph.c — contributor knowledge transmission network implementation.
 * Pure data + lookup. All data is static const. No malloc, no globals. */

#include "knowledge_graph.h"
#include <stddef.h>
#include <string.h>

/* Number of contributors in the graph */
#define KG_CONTRIBUTOR_COUNT 20

/* Static contributor data table.
 * influenced_by / influenced arrays use IDs (= indices) into this table.
 * Bidirectional consistency: if A->influenced includes B,
 * then B->influenced_by includes A. */
static const kg_contributor_t CONTRIBUTORS[KG_CONTRIBUTOR_COUNT] = {
    /* 0: King Wen */
    {
        .id = 0,
        .name = "King Wen",
        .year = -1050,
        .culture = "Chinese",
        .era = ERA_ANCIENT,
        .systems = { KG_SYS_ICHING },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = { 6 },
        .successor_count = 1,
        .key_work = "Hexagram ordering"
    },
    /* 1: Siddhartha Gautama */
    {
        .id = 1,
        .name = "Siddhartha Gautama",
        .year = -500,
        .culture = "Indian",
        .era = ERA_ANCIENT,
        .systems = { KG_SYS_BUDDHIST },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Foundational teachings of Buddhism"
    },
    /* 2: Hipparchus */
    {
        .id = 2,
        .name = "Hipparchus",
        .year = -130,
        .culture = "Greek",
        .era = ERA_CLASSICAL,
        .systems = { KG_SYS_ASTRONOMY },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = { 4 },
        .successor_count = 1,
        .key_work = "Star catalog, precession discovery"
    },
    /* 3: Eratosthenes */
    {
        .id = 3,
        .name = "Eratosthenes",
        .year = -240,
        .culture = "Greek",
        .era = ERA_CLASSICAL,
        .systems = { KG_SYS_ASTRONOMY },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Earth circumference measurement"
    },
    /* 4: Ptolemy */
    {
        .id = 4,
        .name = "Ptolemy",
        .year = 150,
        .culture = "Greek",
        .era = ERA_CLASSICAL,
        .systems = { KG_SYS_ASTRONOMY, KG_SYS_ASTROLOGY },
        .system_count = 2,
        .influenced_by = { 2 },
        .influence_count = 1,
        .influenced = { 5, 10, 8 },
        .successor_count = 3,
        .key_work = "Almagest — systematized astronomy"
    },
    /* 5: Aryabhata */
    {
        .id = 5,
        .name = "Aryabhata",
        .year = 499,
        .culture = "Indian",
        .era = ERA_CLASSICAL,
        .systems = { KG_SYS_ASTRONOMY, KG_SYS_HINDU },
        .system_count = 2,
        .influenced_by = { 4 },
        .influence_count = 1,
        .influenced = { 7 },
        .successor_count = 1,
        .key_work = "Aryabhatiya — Earth rotation, sine tables"
    },
    /* 6: Richard Wilhelm */
    {
        .id = 6,
        .name = "Richard Wilhelm",
        .year = 1923,
        .culture = "German",
        .era = ERA_MODERN,
        .systems = { KG_SYS_ICHING },
        .system_count = 1,
        .influenced_by = { 0 },
        .influence_count = 1,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "I Ching translation to German"
    },
    /* 7: Brahmagupta */
    {
        .id = 7,
        .name = "Brahmagupta",
        .year = 628,
        .culture = "Indian",
        .era = ERA_MEDIEVAL,
        .systems = { KG_SYS_ASTRONOMY, KG_SYS_HINDU },
        .system_count = 2,
        .influenced_by = { 5 },
        .influence_count = 1,
        .influenced = { 8 },
        .successor_count = 1,
        .key_work = "Brahmasphutasiddhanta"
    },
    /* 8: al-Khwarizmi */
    {
        .id = 8,
        .name = "al-Khwarizmi",
        .year = 820,
        .culture = "Arab",
        .era = ERA_MEDIEVAL,
        .systems = { KG_SYS_ASTRONOMY, KG_SYS_ISLAMIC },
        .system_count = 2,
        .influenced_by = { 4, 7 },
        .influence_count = 2,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Zij al-Sindhind — astronomical tables, algorithms"
    },
    /* 9: Hillel II */
    {
        .id = 9,
        .name = "Hillel II",
        .year = 359,
        .culture = "Jewish",
        .era = ERA_CLASSICAL,
        .systems = { KG_SYS_HEBREW },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Fixed Hebrew calendar"
    },
    /* 10: Copernicus */
    {
        .id = 10,
        .name = "Copernicus",
        .year = 1543,
        .culture = "Polish",
        .era = ERA_EARLY_MODERN,
        .systems = { KG_SYS_ASTRONOMY },
        .system_count = 1,
        .influenced_by = { 4 },
        .influence_count = 1,
        .influenced = { 11 },
        .successor_count = 1,
        .key_work = "De revolutionibus"
    },
    /* 11: Kepler */
    {
        .id = 11,
        .name = "Kepler",
        .year = 1609,
        .culture = "German",
        .era = ERA_EARLY_MODERN,
        .systems = { KG_SYS_ASTRONOMY },
        .system_count = 1,
        .influenced_by = { 10, 12 },
        .influence_count = 2,
        .influenced = { 13 },
        .successor_count = 1,
        .key_work = "Planetary motion laws"
    },
    /* 12: Tycho Brahe */
    {
        .id = 12,
        .name = "Tycho Brahe",
        .year = 1590,
        .culture = "Danish",
        .era = ERA_EARLY_MODERN,
        .systems = { KG_SYS_ASTRONOMY },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = { 11 },
        .successor_count = 1,
        .key_work = "Naked-eye precision observations"
    },
    /* 13: Jean Meeus */
    {
        .id = 13,
        .name = "Jean Meeus",
        .year = 1991,
        .culture = "Belgian",
        .era = ERA_MODERN,
        .systems = { KG_SYS_ASTRONOMY, KG_SYS_GREGORIAN },
        .system_count = 2,
        .influenced_by = { 11 },
        .influence_count = 1,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Astronomical Algorithms"
    },
    /* 14: Jose Arguelles */
    {
        .id = 14,
        .name = "Jose Arguelles",
        .year = 1987,
        .culture = "American",
        .era = ERA_MODERN,
        .systems = { KG_SYS_TZOLKIN },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Dreamspell / 13 Moon calendar"
    },
    /* 15: Ra Uru Hu */
    {
        .id = 15,
        .name = "Ra Uru Hu",
        .year = 1992,
        .culture = "Canadian",
        .era = ERA_MODERN,
        .systems = { KG_SYS_HUMAN_DESIGN },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Human Design system"
    },
    /* 16: Moses de Leon */
    {
        .id = 16,
        .name = "Moses de Leon",
        .year = 1290,
        .culture = "Spanish",
        .era = ERA_MEDIEVAL,
        .systems = { KG_SYS_KABBALAH },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = { 17 },
        .successor_count = 1,
        .key_work = "Zohar authorship"
    },
    /* 17: Isaac Luria */
    {
        .id = 17,
        .name = "Isaac Luria",
        .year = 1570,
        .culture = "Jewish",
        .era = ERA_EARLY_MODERN,
        .systems = { KG_SYS_KABBALAH },
        .system_count = 1,
        .influenced_by = { 16 },
        .influence_count = 1,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Lurianic Kabbalah"
    },
    /* 18: Pope Gregory XIII */
    {
        .id = 18,
        .name = "Pope Gregory XIII",
        .year = 1582,
        .culture = "Italian",
        .era = ERA_EARLY_MODERN,
        .systems = { KG_SYS_GREGORIAN },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Gregorian calendar reform"
    },
    /* 19: Ulugh Beg */
    {
        .id = 19,
        .name = "Ulugh Beg",
        .year = 1437,
        .culture = "Timurid",
        .era = ERA_MEDIEVAL,
        .systems = { KG_SYS_ASTRONOMY },
        .system_count = 1,
        .influenced_by = {0},
        .influence_count = 0,
        .influenced = {0},
        .successor_count = 0,
        .key_work = "Zij-i Sultani, sidereal year measurement"
    }
};

/* Human-readable era names, indexed by knowledge_era_t */
static const char *const ERA_NAMES[ERA_COUNT] = {
    "Ancient",
    "Classical",
    "Medieval",
    "Early Modern",
    "Modern"
};

/* Human-readable system names, indexed by kg_system_t */
static const char *const SYSTEM_NAMES[KG_SYS_COUNT] = {
    "Astronomy",
    "Astrology",
    "Tzolkin",
    "I Ching",
    "Chinese",
    "Human Design",
    "Gregorian",
    "Hebrew",
    "Islamic",
    "Buddhist",
    "Hindu",
    "Kabbalah",
    "Geology",
    "Unified"
};

/* Distinct cultures extracted from contributor data.
 * Sorted alphabetically for stable iteration. */
#define KG_CULTURE_COUNT 14
static const char *const CULTURES[KG_CULTURE_COUNT] = {
    "American",
    "Arab",
    "Belgian",
    "Canadian",
    "Chinese",
    "Danish",
    "German",
    "Greek",
    "Indian",
    "Italian",
    "Jewish",
    "Polish",
    "Spanish",
    "Timurid"
};

/* --- Helper: check if contributor works on a given system --- */
static int contributor_has_system(const kg_contributor_t *c, kg_system_t sys)
{
    for (int i = 0; i < c->system_count; i++) {
        if (c->systems[i] == sys) return 1;
    }
    return 0;
}

/* --- Public API --- */

int kg_contributor_count(void)
{
    return KG_CONTRIBUTOR_COUNT;
}

kg_contributor_t kg_contributor_get(int index)
{
    if (index < 0 || index >= KG_CONTRIBUTOR_COUNT) {
        kg_contributor_t invalid = {0};
        invalid.id = -1;
        invalid.name = NULL;
        invalid.culture = NULL;
        invalid.key_work = NULL;
        return invalid;
    }
    return CONTRIBUTORS[index];
}

int kg_contributor_find(const char *name)
{
    if (name == NULL) return -1;
    for (int i = 0; i < KG_CONTRIBUTOR_COUNT; i++) {
        if (strcmp(CONTRIBUTORS[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int kg_contributors_for_system(kg_system_t sys, int *out_indices, int out_max)
{
    if (out_indices == NULL || out_max <= 0) return 0;
    int count = 0;
    for (int i = 0; i < KG_CONTRIBUTOR_COUNT && count < out_max; i++) {
        if (contributor_has_system(&CONTRIBUTORS[i], sys)) {
            out_indices[count++] = i;
        }
    }
    return count;
}

int kg_contributors_by_era(knowledge_era_t era, int *out_indices, int out_max)
{
    if (out_indices == NULL || out_max <= 0) return 0;
    int count = 0;
    for (int i = 0; i < KG_CONTRIBUTOR_COUNT && count < out_max; i++) {
        if (CONTRIBUTORS[i].era == era) {
            out_indices[count++] = i;
        }
    }
    return count;
}

int kg_contributors_by_culture(const char *culture, int *out_indices, int out_max)
{
    if (culture == NULL || out_indices == NULL || out_max <= 0) return 0;
    int count = 0;
    for (int i = 0; i < KG_CONTRIBUTOR_COUNT && count < out_max; i++) {
        if (strcmp(CONTRIBUTORS[i].culture, culture) == 0) {
            out_indices[count++] = i;
        }
    }
    return count;
}

int kg_contributor_systems(int contributor_id, kg_system_t *out_systems, int out_max)
{
    if (out_systems == NULL || out_max <= 0) return 0;
    if (contributor_id < 0 || contributor_id >= KG_CONTRIBUTOR_COUNT) return 0;
    const kg_contributor_t *c = &CONTRIBUTORS[contributor_id];
    int count = c->system_count < out_max ? c->system_count : out_max;
    for (int i = 0; i < count; i++) {
        out_systems[i] = c->systems[i];
    }
    return count;
}

const char *kg_era_name(knowledge_era_t era)
{
    if (era < 0 || era >= ERA_COUNT) return "?";
    return ERA_NAMES[era];
}

const char *kg_system_name(kg_system_t sys)
{
    if (sys < 0 || sys >= KG_SYS_COUNT) return "?";
    return SYSTEM_NAMES[sys];
}

int kg_culture_count(void)
{
    return KG_CULTURE_COUNT;
}

const char *kg_culture_get(int index)
{
    if (index < 0 || index >= KG_CULTURE_COUNT) return NULL;
    return CULTURES[index];
}

int kg_edge_count(void)
{
    int total = 0;
    for (int i = 0; i < KG_CONTRIBUTOR_COUNT; i++) {
        total += CONTRIBUTORS[i].successor_count;
    }
    return total;
}

/* BFS path finder between two systems using undirected traversal.
 * Neighbors of node N = N.influenced_by[] union N.influenced[].
 *
 * Algorithm:
 * 1. Check for direct bridge (contributor in both systems).
 * 2. BFS from all source system contributors, following undirected edges.
 * 3. If a target system contributor is reached, reconstruct path.
 * 4. Return path with length=0 if no connection. */
kg_path_t kg_knowledge_path(kg_system_t system_a, kg_system_t system_b)
{
    kg_path_t result;
    result.length = 0;

    /* Step 1: Check for direct bridge (contributor in both systems) */
    for (int i = 0; i < KG_CONTRIBUTOR_COUNT; i++) {
        if (contributor_has_system(&CONTRIBUTORS[i], system_a) &&
            contributor_has_system(&CONTRIBUTORS[i], system_b)) {
            result.path[0] = i;
            result.length = 1;
            return result;
        }
    }

    /* Step 2: BFS from source contributors, undirected traversal */
    /* With 20 nodes, all arrays fit on the stack easily. */
    int visited[KG_CONTRIBUTOR_COUNT];
    int parent[KG_CONTRIBUTOR_COUNT];
    int queue[KG_CONTRIBUTOR_COUNT];
    int q_head = 0;
    int q_tail = 0;

    for (int i = 0; i < KG_CONTRIBUTOR_COUNT; i++) {
        visited[i] = 0;
        parent[i] = -1;
    }

    /* Seed queue with all contributors in system_a */
    for (int i = 0; i < KG_CONTRIBUTOR_COUNT; i++) {
        if (contributor_has_system(&CONTRIBUTORS[i], system_a)) {
            visited[i] = 1;
            parent[i] = -1; /* root node */
            queue[q_tail++] = i;
        }
    }

    int target = -1;

    while (q_head < q_tail && target < 0) {
        int cur = queue[q_head++];
        const kg_contributor_t *c = &CONTRIBUTORS[cur];

        /* Check if current node is in target system (and not a seed) */
        if (parent[cur] != -1 || !contributor_has_system(c, system_a)) {
            /* Only check non-seed nodes, but actually we should check
             * all reached nodes that are in system_b.
             * However, seed nodes in system_a that are also in system_b
             * were already caught by the direct bridge check above.
             * So any newly reached node in system_b is a valid target. */
        }

        /* Explore neighbors: influenced (forward edges) */
        for (int i = 0; i < c->successor_count; i++) {
            int neighbor = c->influenced[i];
            if (neighbor >= 0 && neighbor < KG_CONTRIBUTOR_COUNT && !visited[neighbor]) {
                visited[neighbor] = 1;
                parent[neighbor] = cur;
                if (contributor_has_system(&CONTRIBUTORS[neighbor], system_b)) {
                    target = neighbor;
                    break;
                }
                queue[q_tail++] = neighbor;
            }
        }
        if (target >= 0) break;

        /* Explore neighbors: influenced_by (backward edges) */
        for (int i = 0; i < c->influence_count; i++) {
            int neighbor = c->influenced_by[i];
            if (neighbor >= 0 && neighbor < KG_CONTRIBUTOR_COUNT && !visited[neighbor]) {
                visited[neighbor] = 1;
                parent[neighbor] = cur;
                if (contributor_has_system(&CONTRIBUTORS[neighbor], system_b)) {
                    target = neighbor;
                    break;
                }
                queue[q_tail++] = neighbor;
            }
        }
    }

    if (target < 0) {
        /* No path found */
        return result;
    }

    /* Reconstruct path from target back to a source */
    int rev_path[KG_MAX_PATH];
    int rev_len = 0;
    int node = target;
    while (node >= 0 && rev_len < KG_MAX_PATH) {
        rev_path[rev_len++] = node;
        node = parent[node];
    }

    /* Reverse into result */
    result.length = rev_len;
    for (int i = 0; i < rev_len; i++) {
        result.path[i] = rev_path[rev_len - 1 - i];
    }

    return result;
}
