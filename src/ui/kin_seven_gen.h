#ifndef TIME_KIN_SEVEN_GEN_H
#define TIME_KIN_SEVEN_GEN_H

/* Kin Seven Generations Oracle — three stacked oracle crosses (L18).
 *
 * Three levels of 5-kin oracle crosses arranged vertically:
 *   Level 0 (Crown): Oracle of the kin itself
 *   Level 1 (Heart): Oracle of the kin's analog partner
 *   Level 2 (Root):  Oracle of the kin's guide
 *
 * Each level is a complete Dreamspell oracle with destiny (center),
 * guide (top), analog (right), antipode (left), occult (bottom).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_SEVEN_GEN_LEVELS 3
#define KIN_ORACLE_SIZE 5

typedef struct {
    int kin;         /* 1-260 */
    int seal;        /* 0-19 */
    int tone;        /* 1-13 */
    int color;       /* 0-3 */
} kin_seven_gen_cell_t;

typedef struct {
    const char *level_name;  /* "Crown", "Heart", "Root" */
    kin_seven_gen_cell_t destiny;   /* center */
    kin_seven_gen_cell_t guide;     /* top */
    kin_seven_gen_cell_t analog;    /* right */
    kin_seven_gen_cell_t antipode;  /* left */
    kin_seven_gen_cell_t occult;    /* bottom */
    float center_x, center_y;      /* center position of this level's cross */
} kin_seven_gen_level_t;

typedef struct {
    kin_seven_gen_level_t levels[KIN_SEVEN_GEN_LEVELS];
    int source_kin;  /* the kin that started the oracle chain */
} kin_seven_gen_t;

/* Compute the Seven Generations oracle for a kin (1-260).
 * Level 0 (Crown): Oracle of the kin itself
 * Level 1 (Heart): Oracle of the kin's analog partner
 * Level 2 (Root): Oracle of the kin's guide
 * Each level is a complete 5-position oracle cross.
 *
 * Positions:
 *   Level 0 center: (0.5, 0.15)
 *   Level 1 center: (0.5, 0.50)
 *   Level 2 center: (0.5, 0.85)
 *
 * Returns zeroed struct for invalid kin. */
kin_seven_gen_t kin_seven_gen_compute(int kin);

#endif /* TIME_KIN_SEVEN_GEN_H */
