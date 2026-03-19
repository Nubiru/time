/* a11y_narrative.h — Rich accessibility narratives for screen readers.
 *
 * Upgrades basic ARIA labels to human-readable narrative descriptions.
 * Instead of "Sun in Aries at 15.0 degrees", produces:
 * "The Sun is high in the sky, moving through Aries."
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 * All output via snprintf to caller-provided buffers. */

#ifndef TIME_A11Y_NARRATIVE_H
#define TIME_A11Y_NARRATIVE_H

#include <stdbool.h>

#define A11Y_NARR_MAX 512

/* Position in the sky for narrative generation */
typedef enum {
    SKY_POS_RISING = 0,    /* just above eastern horizon */
    SKY_POS_LOW_EAST,      /* low in the east */
    SKY_POS_HIGH,          /* high in the sky */
    SKY_POS_OVERHEAD,      /* near zenith */
    SKY_POS_LOW_WEST,      /* low in the west */
    SKY_POS_SETTING,       /* near western horizon */
    SKY_POS_BELOW,         /* below horizon (not visible) */
    SKY_POS_COUNT
} sky_position_t;

/* Brightness level for narrative */
typedef enum {
    SKY_BRIGHT_BRILLIANT = 0,  /* brightest: Venus, Jupiter at peak */
    SKY_BRIGHT_BRIGHT,         /* clearly visible */
    SKY_BRIGHT_MODERATE,       /* visible but not prominent */
    SKY_BRIGHT_FAINT,          /* requires attention to spot */
    SKY_BRIGHT_INVISIBLE,      /* below naked-eye threshold */
    SKY_BRIGHT_COUNT
} sky_brightness_t;

/* Planet data for rich narrative */
typedef struct {
    const char *name;           /* "Mercury", "Venus", etc. */
    const char *sign;           /* "Aries", "Taurus", etc. */
    double degree;              /* degree within sign (0-30) */
    bool is_retrograde;
    sky_position_t position;    /* where in the sky */
    sky_brightness_t brightness;
    const char *near_object;    /* NULL or "the Pleiades", "Spica", etc. */
} a11y_planet_t;

/* Transition type for narrative */
typedef enum {
    A11Y_TRANS_ZOOM_IN = 0,
    A11Y_TRANS_ZOOM_OUT,
    A11Y_TRANS_VIEW_SPACE,      /* switched to space view */
    A11Y_TRANS_VIEW_EARTH,      /* switched to earth view */
    A11Y_TRANS_SYSTEM_ENTER,    /* entered a knowledge system */
    A11Y_TRANS_SYSTEM_EXIT,     /* exited a knowledge system */
    A11Y_TRANS_TIME_FORWARD,    /* time travel forward */
    A11Y_TRANS_TIME_BACKWARD,   /* time travel backward */
    A11Y_TRANS_COUNT
} a11y_transition_t;

/* Compose rich narrative sky description.
 * Output: "The night sky. Mercury is low in the west, moving through
 * Gemini. Venus shines brilliantly in Taurus, near the Pleiades."
 * Skips planets below the horizon.
 * Returns chars written. */
int a11y_narr_sky(const a11y_planet_t *planets, int count,
                  char *out, int max_len);

/* Compose narrative for a single planet.
 * Output: "Venus shines brilliantly, low in the west, in Taurus near
 * the Pleiades, moving retrograde."
 * Returns chars written. */
int a11y_narr_planet(const a11y_planet_t *planet, char *out, int max_len);

/* Compose narrative calendar description.
 * system_name: "Hebrew", "Islamic", etc.
 * date_str: "5 Shevat 5784"
 * meaning: NULL or "the new year for trees approaches"
 * Returns chars written. */
int a11y_narr_calendar(const char *system_name, const char *date_str,
                       const char *meaning, char *out, int max_len);

/* Compose narrative state transition.
 * detail: optional context ("Solar System", "Astrology", etc.)
 * Returns chars written. */
int a11y_narr_transition(a11y_transition_t type, const char *detail,
                         char *out, int max_len);

/* Compose narrative convergence description.
 * systems: array of system names participating
 * count: number of systems (1-16)
 * strength_adj: "notable", "significant", "rare", "extraordinary"
 * Returns chars written. */
int a11y_narr_convergence(const char *const *systems, int count,
                          const char *strength_adj,
                          char *out, int max_len);

/* Sky position description. "rising in the east", "high in the sky", etc. */
const char *a11y_sky_position_desc(sky_position_t pos);

/* Brightness description verb phrase. "shines brilliantly", "is visible", etc. */
const char *a11y_brightness_desc(sky_brightness_t bright);

/* Transition name string. Returns "?" for invalid. */
const char *a11y_transition_name(a11y_transition_t type);

/* Count helpers. */
int a11y_sky_position_count(void);
int a11y_brightness_count(void);
int a11y_transition_type_count(void);

#endif /* TIME_A11Y_NARRATIVE_H */
