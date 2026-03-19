/* celtic_interpret.h — Celtic calendar interpretation data
 *
 * Archetype data for the 13 tree months of the Celtic tree calendar
 * (Robert Graves system) and the 8 festivals of the Wheel of the Year.
 * Composition function produces tiered interpretation text
 * (glyph / glance / detail).
 *
 * The Celtic calendar is a nature-based system where each lunar month
 * corresponds to a sacred tree with unique spiritual qualities.  The 8
 * festivals mark the turning points of the solar year — a dialogue
 * between the dark and light halves of existence.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_CELTIC_INTERPRET_H
#define TIME_CELTIC_INTERPRET_H

/* Per-tree archetype (14 entries: 0=intercalary Yew, 1-13=months) */
typedef struct {
    int month;                  /* 0-13 (0=Yew intercalary, 1-13=months) */
    const char *tree;           /* English tree name */
    const char *ogham;          /* Ogham name */
    const char *archetype;      /* The tree's essential nature */
    const char *personality;    /* Character traits of those born under this tree */
    const char *strengths;      /* Key positive qualities */
    const char *challenges;     /* Shadow aspects */
    const char *brief;          /* One-sentence meaning */
} cti_tree_t;

/* Per-festival interpretation (8 festivals) */
typedef struct {
    int festival;               /* 0-7 (Samhain through Mabon) */
    const char *name;           /* English name */
    const char *theme;          /* Core spiritual theme */
    const char *practice;       /* Traditional observance */
    const char *reflection;     /* Inner work of this season */
    const char *brief;          /* One-sentence meaning */
} cti_festival_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: Ogham letter (1-3 chars) */
    char glance[128];           /* T1: "Oak — The Steadfast Protector" */
    char detail[1024];          /* T3: Full narrative */
} celtic_interp_t;

/* Return archetype data for a Celtic tree month (0-13).
 * 0 = Yew intercalary day, 1-13 = Birch through Elder.
 * Invalid index: returns struct with month=-1, all strings "?". */
cti_tree_t cti_tree_data(int month);

/* Return interpretation data for a Wheel of the Year festival (0-7).
 * 0=Samhain, 1=Yule, 2=Imbolc, 3=Ostara, 4=Beltane, 5=Litha,
 * 6=Lughnasadh, 7=Mabon.
 * Invalid index: returns struct with festival=-1, all strings "?". */
cti_festival_t cti_festival_data(int festival);

/* Compose tiered interpretation from Celtic calendar components.
 * tree_month: 0-13 (Celtic tree month from celtic_tree_from_jd).
 * tree_day: 1-28 (day within the tree month).
 * festival: 0-7 if a festival is active, -1 if none.
 * Invalid tree_month: "?" strings in result. */
celtic_interp_t cti_interpret(int tree_month, int tree_day, int festival);

/* Returns 14 (number of tree entries: 1 intercalary + 13 months). */
int cti_tree_count(void);

/* Returns 8 (number of Wheel of the Year festivals). */
int cti_festival_count(void);

#endif /* TIME_CELTIC_INTERPRET_H */
