#ifndef TIME_KIN_CARD_LAYOUT_H
#define TIME_KIN_CARD_LAYOUT_H

/* Kin Card Layout — enhanced daily info card for a Tzolkin kin.
 *
 * Computes all display data for a kin: identity, oracle relationships,
 * wavespell/castle membership, portal/power flags, and formatted text
 * strings ready for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_CARD_MAX_TEXT 64

typedef struct {
    /* Identity */
    int kin;                          /* 1-260 */
    int seal;                         /* 0-19 */
    int tone;                         /* 1-13 */
    int color;                        /* 0=Red, 1=White, 2=Blue, 3=Yellow */

    /* Display text */
    char title[KIN_CARD_MAX_TEXT];    /* e.g. "Kin 131" */
    char seal_name[KIN_CARD_MAX_TEXT];/* e.g. "Blue Monkey" */
    char tone_name[KIN_CARD_MAX_TEXT];/* e.g. "Magnetic" */
    char oracle_summary[KIN_CARD_MAX_TEXT]; /* e.g. "Guide: Blue Eagle" */
    char wavespell_info[KIN_CARD_MAX_TEXT]; /* e.g. "WS 11 / Pos 1" */
    char castle_name[KIN_CARD_MAX_TEXT];   /* e.g. "Blue Castle of Burning" */

    /* Tone dots: number of dots to render (1-13) */
    int tone_dots;

    /* Oracle kins for sub-display */
    int guide_kin;
    int analog_kin;
    int antipode_kin;
    int occult_kin;

    /* Flags */
    int is_portal;                    /* Galactic Activation Portal */
    int is_power_day;                 /* wavespell power position (1,5,9,13) */
} kin_card_layout_t;

/* Build the complete kin card data for a kin (1-260).
 * Populates all text fields, oracle kins, flags.
 * Returns zeroed struct for invalid kin. */
kin_card_layout_t kin_card_compute(int kin);

/* Get the full color name with seal: "Red Dragon", "Blue Monkey", etc.
 * Writes to buf (max buf_size). Returns buf, or "" if invalid. */
const char *kin_card_full_name(int seal, char *buf, int buf_size);

#endif /* TIME_KIN_CARD_LAYOUT_H */
