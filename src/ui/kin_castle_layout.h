#ifndef TIME_KIN_CASTLE_LAYOUT_H
#define TIME_KIN_CASTLE_LAYOUT_H

/* Kin Castle Layout — 4 wavespells forming a castle frame.
 *
 * A castle groups 4 consecutive wavespells (52 kin) in the 260-day
 * Tzolkin. There are 5 castles: Red (Birth), White (Death),
 * Blue (Magic), Yellow (Intelligence), Green (Enchantment).
 *
 * Each wavespell sub-section is positioned vertically:
 *   ws[0]: y=0.05, ws[1]: y=0.275, ws[2]: y=0.50, ws[3]: y=0.725
 *   all x=0.05, w=0.90, h=0.20
 *
 * The wavespell containing today's kin is marked contains_today=1.
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_CASTLE_WAVESPELLS 4
#define KIN_CASTLE_KIN 52

typedef struct {
    int wavespell_number;    /* 1-20 */
    int start_kin;           /* first kin in wavespell */
    int seal;                /* initiating seal (0-19) */
    const char *purpose;     /* wavespell purpose keyword */
    float x, y, w, h;       /* bounding box in normalized coords */
    int contains_today;      /* 1 if today's kin is in this wavespell */
} kin_castle_ws_t;

typedef struct {
    int castle_number;       /* 1-5 */
    const char *name;        /* "Red Castle of Turning" etc. */
    const char *color;       /* "Red", "White", "Blue", "Yellow", "Green" */
    const char *theme;       /* "Birth", "Death", etc. */
    int start_kin;           /* first kin in castle */
    int end_kin;             /* last kin in castle */
    kin_castle_ws_t wavespells[KIN_CASTLE_WAVESPELLS]; /* 4 wavespells */
    int today_kin;           /* which kin is highlighted (1-260, 0=none) */
    int today_wavespell;     /* 0-3: which wavespell contains today */
    int today_position;      /* 1-52: position within the 52-kin castle */
} kin_castle_layout_t;

/* Compute castle layout for a kin (1-260).
 * Returns the castle containing this kin, with 4 wavespells positioned:
 *   ws[0]: y=0.05, ws[1]: y=0.275, ws[2]: y=0.50, ws[3]: y=0.725
 *   all x=0.05, w=0.90, h=0.20
 * Today_kin is highlighted in the appropriate wavespell.
 * Returns zeroed struct for invalid kin. */
kin_castle_layout_t kin_castle_compute(int kin);

/* Get which castle number (1-5) a kin belongs to.
 * Kin 1-52=castle 1, 53-104=castle 2, 105-156=castle 3,
 * 157-208=castle 4, 209-260=castle 5. Returns 0 for invalid. */
int kin_castle_number(int kin);

#endif /* TIME_KIN_CASTLE_LAYOUT_H */
