# Task: Knowledge Graph Data

**Agent**: GAMMA
**Roadmap Reference**: Track 28.1 — "Agent: Knowledge Graph Data (Agent B)"
**Date**: 2026-03-06
**Status**: CLAIMED

## Goal

Pure data module encoding the knowledge transmission network behind the Time codebase. Each contributor has an era, culture, systems they contributed to, who influenced them, and who they influenced. Provides functions to trace transmission chains (how knowledge traveled from ancient India through Baghdad to modern code) and find which contributors connect any two systems. Makes visible: human knowledge is ONE river with many tributaries.

## READ FIRST

- `data/contributors.json` — existing contributor database (84 entries)
- `src/systems/unified/structural_map.h` — system_id_t enum

## Files to Create

- `src/systems/unified/knowledge_graph.h`
- `src/systems/unified/knowledge_graph.c`
- `tests/systems/unified/test_knowledge_graph.c`

## API

```c
#ifndef TIME_KNOWLEDGE_GRAPH_H
#define TIME_KNOWLEDGE_GRAPH_H

/* Era classifications */
typedef enum {
    ERA_ANCIENT = 0,     /* Before 500 BCE */
    ERA_CLASSICAL,       /* 500 BCE - 500 CE */
    ERA_MEDIEVAL,        /* 500 - 1500 CE */
    ERA_EARLY_MODERN,    /* 1500 - 1800 CE */
    ERA_MODERN,          /* 1800 - present */
    ERA_COUNT
} knowledge_era_t;

/* Systems in the Time codebase */
typedef enum {
    KG_SYS_ASTRONOMY = 0,
    KG_SYS_ASTROLOGY,
    KG_SYS_TZOLKIN,
    KG_SYS_ICHING,
    KG_SYS_CHINESE,
    KG_SYS_HUMAN_DESIGN,
    KG_SYS_GREGORIAN,
    KG_SYS_HEBREW,
    KG_SYS_ISLAMIC,
    KG_SYS_BUDDHIST,
    KG_SYS_HINDU,
    KG_SYS_KABBALAH,
    KG_SYS_GEOLOGY,
    KG_SYS_UNIFIED,
    KG_SYS_COUNT
} kg_system_t;

/* Maximum connections per contributor */
#define KG_MAX_SYSTEMS 6
#define KG_MAX_INFLUENCES 4
#define KG_MAX_PATH 8

/* A contributor node in the knowledge graph */
typedef struct {
    int id;
    const char *name;
    int year;                        /* Primary active year (negative=BCE) */
    const char *culture;
    knowledge_era_t era;
    kg_system_t systems[KG_MAX_SYSTEMS]; /* Systems contributed to */
    int system_count;
    int influenced_by[KG_MAX_INFLUENCES]; /* IDs of predecessors */
    int influence_count;
    int influenced[KG_MAX_INFLUENCES];    /* IDs of successors */
    int successor_count;
    const char *key_work;            /* Most significant contribution */
} kg_contributor_t;

/* A transmission chain (path through the graph) */
typedef struct {
    int path[KG_MAX_PATH];           /* Contributor IDs in order */
    int length;                      /* Number of nodes in path */
} kg_path_t;

/* Total contributors in the graph. */
int kg_contributor_count(void);

/* Get contributor by index. Returns entry with id=-1 if invalid. */
kg_contributor_t kg_contributor_get(int index);

/* Find contributor by name (exact match). Returns index or -1 if not found. */
int kg_contributor_find(const char *name);

/* Get contributors for a system. Fills out_indices. Returns count. */
int kg_contributors_for_system(kg_system_t sys, int *out_indices, int out_max);

/* Get contributors by era. Fills out_indices. Returns count. */
int kg_contributors_by_era(knowledge_era_t era, int *out_indices, int out_max);

/* Get contributors by culture. Fills out_indices. Returns count. */
int kg_contributors_by_culture(const char *culture, int *out_indices, int out_max);

/* Find a transmission path between two systems.
 * Returns path through contributors connecting system_a to system_b.
 * Returns path with length=0 if no connection found. */
kg_path_t kg_knowledge_path(kg_system_t system_a, kg_system_t system_b);

/* Get all systems a contributor enabled. Fills out_systems. Returns count. */
int kg_contributor_systems(int contributor_id, kg_system_t *out_systems, int out_max);

/* Era name string. */
const char *kg_era_name(knowledge_era_t era);

/* System name string. */
const char *kg_system_name(kg_system_t sys);

/* How many distinct cultures? */
int kg_culture_count(void);

/* Get distinct culture by index. */
const char *kg_culture_get(int index);

/* How many transmission chains exist (edges in graph)? */
int kg_edge_count(void);

#endif /* TIME_KNOWLEDGE_GRAPH_H */
```

## Contributor Data (minimum 20 nodes with influence edges)

Key contributors from data/contributors.json mapped into the graph:

0. **King Wen** (~1050 BCE, Chinese, ANCIENT): I Ching hexagram ordering. Influenced: Richard Wilhelm.
1. **Ptolemy** (150 CE, Greek, CLASSICAL): Almagest — astronomy, astrology. Influenced by: Hipparchus. Influenced: al-Khwarizmi, Copernicus.
2. **Aryabhata** (499 CE, Indian, CLASSICAL): Astronomy, math. Influenced: Brahmagupta.
3. **Brahmagupta** (628 CE, Indian, MEDIEVAL): Lunar month, math. Influenced by: Aryabhata. Influenced: al-Khwarizmi.
4. **Hillel II** (359 CE, Jewish, CLASSICAL): Hebrew calendar. Systems: HEBREW, CALENDAR.
5. **al-Khwarizmi** (820 CE, Arab, MEDIEVAL): Astronomy, math. Influenced by: Ptolemy, Brahmagupta. Influenced: European astronomers.
6. **Richard Wilhelm** (1923 CE, German, MODERN): I Ching translation. Influenced by: King Wen. Systems: ICHING.
7. **Jose Arguelles** (1987 CE, American, MODERN): Tzolkin/Dreamspell. Systems: TZOLKIN.
8. **Jean Meeus** (1991 CE, Belgian, MODERN): Astronomical Algorithms. Influenced by: Ptolemy (indirectly). Systems: ASTRONOMY, GREGORIAN.
9. **Ra Uru Hu** (1992 CE, Canadian, MODERN): Human Design system. Systems: HUMAN_DESIGN.
10. **Copernicus** (1543 CE, Polish, EARLY_MODERN): Heliocentric model. Influenced by: Ptolemy. Influenced: Kepler.
11. **Kepler** (1609 CE, German, EARLY_MODERN): Planetary laws. Influenced by: Copernicus, Tycho Brahe. Systems: ASTRONOMY.
12. **Hipparchus** (130 BCE, Greek, CLASSICAL): Star catalog, precession. Influenced: Ptolemy. Systems: ASTRONOMY.
13. **Eratosthenes** (240 BCE, Greek, CLASSICAL): Earth circumference. Systems: ASTRONOMY.
14. **Ulugh Beg** (1437 CE, Timurid, MEDIEVAL): Star catalog, sidereal year. Systems: ASTRONOMY.
15. **Surya Siddhanta author** (~400 CE, Indian, CLASSICAL): Hindu astronomy. Influenced: Aryabhata. Systems: HINDU, ASTRONOMY.
16. **Isaac Luria** (1570 CE, Jewish, EARLY_MODERN): Kabbalistic system. Systems: KABBALAH.
17. **Moses de Leon** (1290 CE, Spanish, MEDIEVAL): Zohar. Influenced: Isaac Luria. Systems: KABBALAH.
18. **Siddhartha Gautama** (~500 BCE, Indian, ANCIENT): Buddhism. Systems: BUDDHIST.
19. **Pope Gregory XIII** (1582 CE, Italian, EARLY_MODERN): Gregorian calendar. Systems: GREGORIAN.

## DONE WHEN

- [ ] >= 20 contributor nodes with influence edges
- [ ] knowledge_path finds transmission chains between systems
- [ ] Filter by system, era, culture
- [ ] Edge count reflects total influence relationships
- [ ] >= 30 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- Standalone module (no compile-time dependencies)
- out_indices/out_systems arrays provided by caller
- Path-finding: simple BFS over static data (20 nodes = trivial)
- All data as static const arrays
