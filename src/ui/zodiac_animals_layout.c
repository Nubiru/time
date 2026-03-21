/* zodiac_animals_layout.c — Chinese zodiac wheel layout
 *
 * Places 12 Chinese zodiac animals on a circle at 30-degree intervals.
 * Traditional Chinese order starting from North (top): Rat at 0 degrees,
 * Ox at 30 degrees clockwise, etc.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "zodiac_animals_layout.h"
#include "../systems/chinese/chinese.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Wheel geometry (normalized 0-1 coordinates). */
#define CENTER_X  0.50f
#define CENTER_Y  0.50f
#define RADIUS    0.38f

static float deg_to_rad(float deg)
{
    return deg * (float)(M_PI / 180.0);
}

zodiac_animals_layout_t zodiac_animals_compute(double jd)
{
    zodiac_animals_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    layout.center_x = CENTER_X;
    layout.center_y = CENTER_Y;
    layout.radius   = RADIUS;

    /* Get current year's animal from JD */
    chinese_year_t cy = chinese_year_from_jd(jd);
    layout.current_animal       = cy.animal;
    layout.current_element      = cy.element;
    layout.current_element_name = chinese_element_name(cy.element);

    /* Place 12 animals at 30-degree intervals.
     * North=top convention: sin for x, -cos for y.
     * Rat(0) at 0 degrees (top), clockwise. */
    for (int i = 0; i < ZA_ANIMAL_COUNT; i++) {
        layout.animals[i].animal    = i;
        layout.animals[i].name      = chinese_animal_name(i);
        layout.animals[i].symbol    = chinese_animal_symbol(i);
        layout.animals[i].angle_deg = (float)(i * 30);

        float rad = deg_to_rad(layout.animals[i].angle_deg);
        layout.animals[i].x = CENTER_X + RADIUS * sinf(rad);
        layout.animals[i].y = CENTER_Y - RADIUS * cosf(rad);

        layout.animals[i].highlighted = (i == cy.animal) ? 1 : 0;
    }

    return layout;
}

za_animal_t zodiac_animals_at(const zodiac_animals_layout_t *layout, int index)
{
    za_animal_t empty;
    memset(&empty, 0, sizeof(empty));
    empty.animal = -1;
    empty.name   = "?";
    empty.symbol = "?";

    if (!layout || index < 0 || index >= ZA_ANIMAL_COUNT) {
        return empty;
    }

    return layout->animals[index];
}
