#ifndef TIME_COLOR_PALETTE_H
#define TIME_COLOR_PALETTE_H

#include "../math/color.h"

/* Zodiac sign colors (0=Aries..11=Pisces). Returns gray for invalid. */
color_rgb_t color_zodiac_sign(int sign);

/* Zodiac element colors (0=Fire, 1=Earth, 2=Air, 3=Water). */
color_rgb_t color_zodiac_element(int element);

/* Planet colors (0=Sun..7=Neptune). Returns gray for invalid. */
color_rgb_t color_planet(int planet_id);

/* Aspect colors (0=conjunction, 1=opposition, 2=trine, 3=square, 4=sextile). */
color_rgb_t color_aspect(int aspect_type);

/* Tzolkin seal family colors (0=Red, 1=White, 2=Blue, 3=Yellow). */
color_rgb_t color_tzolkin_family(int family);

/* Chinese element colors (0=Wood, 1=Fire, 2=Earth, 3=Metal, 4=Water). */
color_rgb_t color_chinese_element(int element);

/* Scale a color by factor (0=black, 1=unchanged, >1=brighter). */
color_rgb_t color_scale(color_rgb_t c, float factor);

/* Clamp all components to [0.0, 1.0]. */
color_rgb_t color_clamp(color_rgb_t c);

#endif
