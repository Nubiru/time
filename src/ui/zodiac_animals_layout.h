/* zodiac_animals_layout.h — Chinese zodiac wheel layout
 *
 * 12 Chinese zodiac animals arranged in a circle, with the current
 * year's animal highlighted. Each animal has position, element, polarity.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_ZODIAC_ANIMALS_LAYOUT_H
#define TIME_ZODIAC_ANIMALS_LAYOUT_H

#include "../systems/chinese/chinese.h"

typedef struct {
    int animal;             /* 0-11 */
    const char *name;       /* "Rat", "Ox", etc. */
    const char *symbol;     /* emoji */
    float x, y;             /* position on circle */
    float angle_deg;        /* angle from center */
    int highlighted;        /* 1 if this is current year's animal */
} za_animal_t;

#define ZA_ANIMAL_COUNT 12

typedef struct {
    za_animal_t animals[ZA_ANIMAL_COUNT];
    float center_x, center_y;
    float radius;
    int current_animal;     /* 0-11, the highlighted one */
    int current_element;    /* 0-4 */
    const char *current_element_name;
} zodiac_animals_layout_t;

/* Compute Chinese zodiac wheel layout for a given Julian Day.
 * Returns layout with center at (0.50, 0.50) and radius 0.38. */
zodiac_animals_layout_t zodiac_animals_compute(double jd);

/* Get animal data at index (0-11).
 * Returns animal with animal=-1 for invalid index or NULL layout. */
za_animal_t zodiac_animals_at(const zodiac_animals_layout_t *layout, int index);

#endif /* TIME_ZODIAC_ANIMALS_LAYOUT_H */
