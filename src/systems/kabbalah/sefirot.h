/* sefirot.h — Kabbalah Tree of Life: 10 Sefirot data module
 *
 * Pure data module providing the 10 Sefirot (+ Da'at) with names,
 * meanings, planetary correspondences, colors (Queen Scale / Golden Dawn),
 * 2D positions on the Tree, pillar assignments, and triad groupings.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_SEFIROT_H
#define TIME_SEFIROT_H

#include <stdbool.h>

#define SEFIROT_COUNT 10
#define SEFIROT_COUNT_WITH_DAAT 11

typedef struct {
    float r, g, b;
} sefirah_color_t;

typedef struct {
    int id;                  /* 0-9 (0=Keter, 9=Malkuth) */
    const char *name;        /* Hebrew name */
    const char *meaning;     /* English meaning */
    int planet_index;        /* maps to planets.h index (0-7), or -1 for Sun/Moon */
    sefirah_color_t color;   /* Queen Scale color (Golden Dawn) */
    float pos_x;             /* 2D position x (-1.0 to 1.0, 3 pillars) */
    float pos_y;             /* 2D position y (0.0=top Keter, 1.0=bottom Malkuth) */
    int pillar;              /* 0=Middle/Balance, 1=Right/Mercy, 2=Left/Severity */
    int triad;               /* 0=Supernal, 1=Moral, 2=Material, 3=Kingdom(Malkuth) */
} sefirah_t;

/* Get Sefirah by index (0-9). Returns zeroed struct for invalid index. */
sefirah_t sefirot_get(int index);

/* Get the Sefirah associated with a planet index (0-7 from planets.h).
 * Mercury(0)->Hod(7), Venus(1)->Netzach(6), Earth(2)->Malkuth(9),
 * Mars(3)->Gevurah(4), Jupiter(4)->Chesed(3), Saturn(5)->Binah(2),
 * Uranus(6)->Chokmah(1), Neptune(7)->Keter(0).
 * Returns zeroed struct for invalid planet_index. */
sefirah_t sefirot_for_planet(int planet_index);

/* Sefirah name by index (0-9). Returns "?" for invalid. */
const char *sefirot_name(int index);

/* Sefirah meaning by index (0-9). Returns "?" for invalid. */
const char *sefirot_meaning(int index);

/* Pillar name: 0="Balance", 1="Mercy", 2="Severity". */
const char *sefirot_pillar_name(int pillar);

/* Triad name: 0="Supernal", 1="Moral", 2="Material", 3="Kingdom". */
const char *sefirot_triad_name(int triad);

/* True if Sefirah is on the middle pillar (Keter, Tiferet, Yesod, Malkuth). */
bool sefirot_is_middle_pillar(int index);

/* Da'at (hidden 11th Sefirah). Returns a sefirah_t with id=10. */
sefirah_t sefirot_get_daat(void);

#endif /* TIME_SEFIROT_H */
