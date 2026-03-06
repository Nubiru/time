/* four_worlds.h — Kabbalah Four Worlds (Olamot) data module
 *
 * Pure data module providing the 4 Kabbalistic Worlds mapped to
 * camera scale levels. Each world corresponds to a range of Sefirot
 * and a range of camera scales for visual rendering.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_FOUR_WORLDS_H
#define TIME_FOUR_WORLDS_H

#include <stdbool.h>

#define FOUR_WORLDS_COUNT 4

typedef struct {
    float r, g, b;
} world_color_t;

typedef struct {
    int id;                   /* 0-3 */
    const char *name;         /* Hebrew name */
    const char *meaning;      /* English meaning */
    const char *description;  /* Brief characterization */
    int scale_min;            /* Minimum camera scale level (0-6) */
    int scale_max;            /* Maximum camera scale level (0-6) */
    world_color_t color;      /* Characteristic color */
    int sefirot_start;        /* First Sefirah in this world (index into sefirot) */
    int sefirot_end;          /* Last Sefirah in this world (index into sefirot) */
} kabbalistic_world_t;

/* Get world by index (0-3). Returns zeroed struct for invalid. */
kabbalistic_world_t four_worlds_get(int index);

/* Get the world active at a given camera scale level (0-6).
 * Scale 0-1 -> Assiah(3), 2-3 -> Yetzirah(2), 4-5 -> Beriah(1), 6 -> Atzilut(0).
 * Clamps out-of-range values: <0 -> Assiah, >6 -> Atzilut. */
kabbalistic_world_t four_worlds_at_scale(int scale_level);

/* World name by index. Returns "?" for invalid. */
const char *four_worlds_name(int index);

/* World meaning by index. Returns "?" for invalid. */
const char *four_worlds_meaning(int index);

/* Does this Sefirah (0-9) belong to this world (0-3)? */
bool four_worlds_contains_sefirah(int world_index, int sefirah_index);

/* Get world index for a given Sefirah index (0-9).
 * Keter(0) -> Atzilut(0), Chokmah-Binah(1-2) -> Beriah(1),
 * Chesed-Yesod(3-8) -> Yetzirah(2), Malkuth(9) -> Assiah(3).
 * Returns -1 for invalid sefirah_index. */
int four_worlds_for_sefirah(int sefirah_index);

/* Number of Sefirot in this world. Returns 0 for invalid. */
int four_worlds_sefirot_count(int world_index);

#endif /* TIME_FOUR_WORLDS_H */
