#ifndef TIME_SCALE_HUD_H
#define TIME_SCALE_HUD_H

/* HUD visibility flags for each data category.
 * Each flag is 0 (hidden) or 1 (visible) at the current scale. */
typedef struct {
    int show_gregorian;      /* date/time — always visible */
    int show_jd;             /* Julian Day number */
    int show_sidereal;       /* GMST/GAST/LST */
    int show_speed;          /* time speed indicator */
    int show_scale_name;     /* current scale label */
    int show_sun_sign;       /* zodiac sun sign */
    int show_tzolkin;        /* Mayan Tzolkin kin */
    int show_iching;         /* I Ching hexagram */
    int show_chinese;        /* Chinese calendar year */
    int show_hd;             /* Human Design gate */
    int show_astrology;      /* full astrology panel (aspects, houses) */
    int show_cosmic;         /* cosmic time metrics */
    int show_aspects;        /* aspect lines list */
    int show_houses;         /* house cusp info */
} hud_visibility_t;

/* Determine which HUD elements are visible at a given scale level (0-6). */
hud_visibility_t hud_visibility_at_scale(int scale_id);

/* Count how many items are visible. */
int hud_visible_count(hud_visibility_t vis);

/* Get a compact label for the HUD mode at a given scale.
 * e.g. "Personal", "Solar", "Cosmic". Returns static string. */
const char *hud_mode_name(int scale_id);

/* Merge two visibility structs (logical OR of all fields). */
hud_visibility_t hud_visibility_merge(hud_visibility_t a, hud_visibility_t b);

/* Default visibility (scale 3 = Solar System). */
hud_visibility_t hud_visibility_default(void);

#endif
