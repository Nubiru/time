/* korean_interpret.h — Korean calendar interpretation data
 *
 * Archetype data for the 7 traditional festivals and 5 elemental
 * qualities of the Korean lunisolar calendar.  Korea shares the
 * Chinese sexagenary system but with distinct Korean names and
 * unique festival traditions rooted in agricultural and Confucian
 * culture.  The Dangun era (2333 BCE) is the oldest continuous
 * national calendar system.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_KOREAN_INTERPRET_H
#define TIME_KOREAN_INTERPRET_H

/* Per-festival interpretation (7 festivals) */
typedef struct {
    int festival;               /* 0-6 */
    const char *name;           /* Korean romanized name */
    const char *korean;         /* Korean script */
    const char *theme;          /* Core cultural theme */
    const char *practice;       /* Traditional observance */
    const char *brief;          /* One-sentence meaning */
} ki_festival_t;

/* Per-element interpretation (5 elements) */
typedef struct {
    int element;                /* 0-4 (Wood, Fire, Earth, Metal, Water) */
    const char *korean;         /* Korean name */
    const char *meaning;        /* Essential nature */
    const char *season;         /* Associated season */
    const char *brief;          /* One-sentence meaning */
} ki_element_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: animal abbreviation (3 chars) */
    char glance[128];           /* T1: "Dangun 4359 — Year of the Wood Dragon" */
    char detail[512];           /* T3: Full narrative */
} korean_interp_t;

/* Return interpretation for a Korean festival (0-6).
 * Invalid: returns struct with festival=-1, all strings "?". */
ki_festival_t ki_festival_data(int festival);

/* Return interpretation for an element (0-4).
 * 0=Wood, 1=Fire, 2=Earth, 3=Metal, 4=Water.
 * Invalid: returns struct with element=-1, all strings "?". */
ki_element_t ki_element_data(int element);

/* Compose tiered interpretation from Korean calendar components.
 * dangun_year: Dangun era year.
 * animal: 0-11, element: 0-4, polarity: 0=Yang/1=Yin.
 * festival: 0-6 if active, -1 if none.
 * Invalid animal: "?" strings in result. */
korean_interp_t ki_interpret(int dangun_year, int animal, int element,
                             int polarity, int festival);

/* Returns 7 (number of festivals). */
int ki_festival_count(void);

/* Returns 5 (number of elements). */
int ki_element_count(void);

#endif /* TIME_KOREAN_INTERPRET_H */
