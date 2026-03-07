/* knowledge_graph — contributor knowledge transmission network.
 * Pure lookup module: static const data, no malloc, no globals, no side effects.
 * Maps contributors to the Time systems they enabled, with influence edges
 * encoding knowledge transmission chains across centuries. */

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
