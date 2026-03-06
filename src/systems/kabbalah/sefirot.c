/* sefirot.c — Kabbalah Tree of Life: 10 Sefirot data implementation
 *
 * Pure data module. All data is static const. No malloc, no globals,
 * no side effects.
 *
 * Sources: Traditional Kabbalistic Tree of Life structure,
 *          Golden Dawn Queen Scale color correspondences,
 *          Planetary attributions per Western esoteric tradition. */

#include "sefirot.h"
#include <string.h>

/* The 10 Sefirot, indexed 0 (Keter) through 9 (Malkuth). */
static const sefirah_t SEFIROT_DATA[SEFIROT_COUNT] = {
    { /* 0 */ .id = 0, .name = "Keter",    .meaning = "Crown",
      .planet_index = 7,  /* Neptune */
      .color = { 1.0f, 1.0f, 1.0f },  /* white */
      .pos_x = 0.0f, .pos_y = 0.0f,
      .pillar = 0, .triad = 0 },

    { /* 1 */ .id = 1, .name = "Chokmah",  .meaning = "Wisdom",
      .planet_index = 6,  /* Uranus */
      .color = { 0.5f, 0.5f, 0.5f },  /* gray */
      .pos_x = 0.5f, .pos_y = 0.17f,
      .pillar = 1, .triad = 0 },

    { /* 2 */ .id = 2, .name = "Binah",    .meaning = "Understanding",
      .planet_index = 5,  /* Saturn */
      .color = { 0.1f, 0.1f, 0.1f },  /* black */
      .pos_x = -0.5f, .pos_y = 0.17f,
      .pillar = 2, .triad = 0 },

    { /* 3 */ .id = 3, .name = "Chesed",   .meaning = "Mercy",
      .planet_index = 4,  /* Jupiter */
      .color = { 0.2f, 0.4f, 0.8f },  /* blue */
      .pos_x = 0.5f, .pos_y = 0.42f,
      .pillar = 1, .triad = 1 },

    { /* 4 */ .id = 4, .name = "Gevurah",  .meaning = "Strength",
      .planet_index = 3,  /* Mars */
      .color = { 0.8f, 0.15f, 0.15f },  /* red */
      .pos_x = -0.5f, .pos_y = 0.42f,
      .pillar = 2, .triad = 1 },

    { /* 5 */ .id = 5, .name = "Tiferet",  .meaning = "Beauty",
      .planet_index = -1,  /* Sun (not in planets.h) */
      .color = { 0.9f, 0.8f, 0.2f },  /* gold */
      .pos_x = 0.0f, .pos_y = 0.5f,
      .pillar = 0, .triad = 1 },

    { /* 6 */ .id = 6, .name = "Netzach",  .meaning = "Victory",
      .planet_index = 1,  /* Venus */
      .color = { 0.2f, 0.7f, 0.3f },  /* green */
      .pos_x = 0.5f, .pos_y = 0.67f,
      .pillar = 1, .triad = 2 },

    { /* 7 */ .id = 7, .name = "Hod",      .meaning = "Splendor",
      .planet_index = 0,  /* Mercury */
      .color = { 0.9f, 0.5f, 0.1f },  /* orange */
      .pos_x = -0.5f, .pos_y = 0.67f,
      .pillar = 2, .triad = 2 },

    { /* 8 */ .id = 8, .name = "Yesod",    .meaning = "Foundation",
      .planet_index = -1,  /* Moon (not in planets.h) */
      .color = { 0.5f, 0.2f, 0.7f },  /* violet */
      .pos_x = 0.0f, .pos_y = 0.83f,
      .pillar = 0, .triad = 2 },

    { /* 9 */ .id = 9, .name = "Malkuth",  .meaning = "Kingdom",
      .planet_index = 2,  /* Earth */
      .color = { 0.5f, 0.4f, 0.2f },  /* citrine */
      .pos_x = 0.0f, .pos_y = 1.0f,
      .pillar = 0, .triad = 3 }
};

/* Planet index (0-7) to Sefirah index mapping.
 * Mercury(0)->Hod(7), Venus(1)->Netzach(6), Earth(2)->Malkuth(9),
 * Mars(3)->Gevurah(4), Jupiter(4)->Chesed(3), Saturn(5)->Binah(2),
 * Uranus(6)->Chokmah(1), Neptune(7)->Keter(0). */
static const int PLANET_TO_SEFIRAH[8] = {
    7, 6, 9, 4, 3, 2, 1, 0
};

static const char *PILLAR_NAMES[3] = {
    "Balance", "Mercy", "Severity"
};

static const char *TRIAD_NAMES[4] = {
    "Supernal", "Moral", "Material", "Kingdom"
};

sefirah_t sefirot_get(int index)
{
    if (index < 0 || index >= SEFIROT_COUNT) {
        sefirah_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return SEFIROT_DATA[index];
}

sefirah_t sefirot_for_planet(int planet_index)
{
    if (planet_index < 0 || planet_index >= 8) {
        sefirah_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return SEFIROT_DATA[PLANET_TO_SEFIRAH[planet_index]];
}

const char *sefirot_name(int index)
{
    if (index < 0 || index >= SEFIROT_COUNT) {
        return "?";
    }
    return SEFIROT_DATA[index].name;
}

const char *sefirot_meaning(int index)
{
    if (index < 0 || index >= SEFIROT_COUNT) {
        return "?";
    }
    return SEFIROT_DATA[index].meaning;
}

const char *sefirot_pillar_name(int pillar)
{
    if (pillar < 0 || pillar >= 3) {
        return "?";
    }
    return PILLAR_NAMES[pillar];
}

const char *sefirot_triad_name(int triad)
{
    if (triad < 0 || triad >= 4) {
        return "?";
    }
    return TRIAD_NAMES[triad];
}

bool sefirot_is_middle_pillar(int index)
{
    if (index < 0 || index >= SEFIROT_COUNT) {
        return false;
    }
    return SEFIROT_DATA[index].pillar == 0;
}

sefirah_t sefirot_get_daat(void)
{
    sefirah_t daat = {
        .id = 10,
        .name = "Da'at",
        .meaning = "Knowledge",
        .planet_index = -1,
        .color = { 0.7f, 0.7f, 0.8f },  /* lavender */
        .pos_x = 0.0f,
        .pos_y = 0.33f,
        .pillar = 0,   /* Middle */
        .triad = 0     /* Supernal */
    };
    return daat;
}
