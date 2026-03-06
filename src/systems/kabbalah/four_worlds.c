/* four_worlds.c — Kabbalah Four Worlds (Olamot) data implementation
 *
 * Pure data module. All data is static const. No malloc, no globals,
 * no side effects.
 *
 * Sources: Traditional Kabbalistic Four Worlds framework.
 *          Atzilut (Emanation), Beriah (Creation),
 *          Yetzirah (Formation), Assiah (Action).
 *
 * Scale mapping designed for Time project camera system:
 *   close-in scales -> material Assiah
 *   cosmic scales -> archetypal Atzilut */

#include "four_worlds.h"
#include <string.h>

/* The 4 Worlds, indexed 0 (Atzilut) through 3 (Assiah).
 * Ordered from highest/most divine to lowest/most material. */
static const kabbalistic_world_t WORLDS_DATA[FOUR_WORLDS_COUNT] = {
    { /* 0 — Atzilut */
      .id = 0,
      .name = "Atzilut",
      .meaning = "Emanation",
      .description = "Archetypal world, divine nearness",
      .scale_min = 6, .scale_max = 6,
      .color = { 1.0f, 0.95f, 0.7f },   /* gold-white */
      .sefirot_start = 0, .sefirot_end = 0 },

    { /* 1 — Beriah */
      .id = 1,
      .name = "Beriah",
      .meaning = "Creation",
      .description = "Intellectual world, pure thought",
      .scale_min = 4, .scale_max = 5,
      .color = { 0.3f, 0.4f, 0.8f },     /* blue */
      .sefirot_start = 1, .sefirot_end = 2 },

    { /* 2 — Yetzirah */
      .id = 2,
      .name = "Yetzirah",
      .meaning = "Formation",
      .description = "Emotional world, angels, patterns",
      .scale_min = 2, .scale_max = 3,
      .color = { 0.6f, 0.3f, 0.7f },     /* purple */
      .sefirot_start = 3, .sefirot_end = 8 },

    { /* 3 — Assiah */
      .id = 3,
      .name = "Assiah",
      .meaning = "Action",
      .description = "Physical world, manifest reality",
      .scale_min = 0, .scale_max = 1,
      .color = { 0.5f, 0.4f, 0.25f },    /* earth */
      .sefirot_start = 9, .sefirot_end = 9 }
};

/* Scale level (0-6) to world index mapping.
 * Index into this array by scale level. */
static const int SCALE_TO_WORLD[7] = {
    3, 3, 2, 2, 1, 1, 0
};

kabbalistic_world_t four_worlds_get(int index)
{
    if (index < 0 || index >= FOUR_WORLDS_COUNT) {
        kabbalistic_world_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return WORLDS_DATA[index];
}

kabbalistic_world_t four_worlds_at_scale(int scale_level)
{
    /* Clamp to valid range */
    if (scale_level < 0) {
        scale_level = 0;
    }
    if (scale_level > 6) {
        scale_level = 6;
    }
    return WORLDS_DATA[SCALE_TO_WORLD[scale_level]];
}

const char *four_worlds_name(int index)
{
    if (index < 0 || index >= FOUR_WORLDS_COUNT) {
        return "?";
    }
    return WORLDS_DATA[index].name;
}

const char *four_worlds_meaning(int index)
{
    if (index < 0 || index >= FOUR_WORLDS_COUNT) {
        return "?";
    }
    return WORLDS_DATA[index].meaning;
}

bool four_worlds_contains_sefirah(int world_index, int sefirah_index)
{
    if (world_index < 0 || world_index >= FOUR_WORLDS_COUNT) {
        return false;
    }
    if (sefirah_index < 0 || sefirah_index > 9) {
        return false;
    }
    return sefirah_index >= WORLDS_DATA[world_index].sefirot_start &&
           sefirah_index <= WORLDS_DATA[world_index].sefirot_end;
}

int four_worlds_for_sefirah(int sefirah_index)
{
    if (sefirah_index < 0 || sefirah_index > 9) {
        return -1;
    }
    for (int i = 0; i < FOUR_WORLDS_COUNT; i++) {
        if (sefirah_index >= WORLDS_DATA[i].sefirot_start &&
            sefirah_index <= WORLDS_DATA[i].sefirot_end) {
            return i;
        }
    }
    return -1; /* should not reach here with valid data */
}

int four_worlds_sefirot_count(int world_index)
{
    if (world_index < 0 || world_index >= FOUR_WORLDS_COUNT) {
        return 0;
    }
    return WORLDS_DATA[world_index].sefirot_end -
           WORLDS_DATA[world_index].sefirot_start + 1;
}
