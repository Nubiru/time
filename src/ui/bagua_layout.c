/* bagua_layout.c — Bagua (8 trigrams) wheel layout
 *
 * Computes the Later Heaven (King Wen) octagonal arrangement.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "bagua_layout.h"
#include "../systems/iching/iching.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Wheel geometry */
#define CENTER_X 0.50f
#define CENTER_Y 0.50f
#define RADIUS   0.35f

/* Direction names for the 8 wheel positions (clockwise from South). */
static const char *DIRECTION_NAMES[8] = {
    "S", "SW", "W", "NW", "N", "NE", "E", "SE"
};

/* Later Heaven (King Wen) arrangement:
 * Position 0 (S):  Li    (Fire)      — iching trigram index 2
 * Position 1 (SW): Kun   (Earth)     — iching trigram index 7
 * Position 2 (W):  Dui   (Lake)      — iching trigram index 1
 * Position 3 (NW): Qian  (Heaven)    — iching trigram index 0
 * Position 4 (N):  Kan   (Water)     — iching trigram index 5
 * Position 5 (NE): Gen   (Mountain)  — iching trigram index 6
 * Position 6 (E):  Zhen  (Thunder)   — iching trigram index 3
 * Position 7 (SE): Xun   (Wind)      — iching trigram index 4
 */
static const int LH_TRIGRAM_INDEX[8] = {
    2, 7, 1, 0, 5, 6, 3, 4
};

/* English names indexed by iching trigram index (0-7). */
static const char *ENGLISH_NAMES[8] = {
    "Heaven",    /* 0: Qian */
    "Lake",      /* 1: Dui  */
    "Fire",      /* 2: Li   */
    "Thunder",   /* 3: Zhen */
    "Wind",      /* 4: Xun  */
    "Water",     /* 5: Kan  */
    "Mountain",  /* 6: Gen  */
    "Earth"      /* 7: Kun  */
};

/* Element associations indexed by iching trigram index (0-7). */
static const char *ELEMENT_NAMES[8] = {
    "Heaven",    /* 0: Qian */
    "Lake",      /* 1: Dui  */
    "Fire",      /* 2: Li   */
    "Thunder",   /* 3: Zhen */
    "Wind",      /* 4: Xun  */
    "Water",     /* 5: Kan  */
    "Mountain",  /* 6: Gen  */
    "Earth"      /* 7: Kun  */
};

/* Trigram lines indexed by iching trigram index (0-7).
 * Each entry: {bottom, middle, top}, 0=yin, 1=yang.
 * Matches iching.c TRIGRAM_LINES. */
static const int TRIGRAM_LINES[8][3] = {
    {1, 1, 1},  /* 0: Qian (Heaven)   */
    {1, 1, 0},  /* 1: Dui  (Lake)     */
    {1, 0, 1},  /* 2: Li   (Fire)     */
    {1, 0, 0},  /* 3: Zhen (Thunder)  */
    {0, 1, 1},  /* 4: Xun  (Wind)     */
    {0, 1, 0},  /* 5: Kan  (Water)    */
    {0, 0, 1},  /* 6: Gen  (Mountain) */
    {0, 0, 0},  /* 7: Kun  (Earth)    */
};

/* Angle for each wheel position. Starting at South (270 degrees in
 * standard math convention = bottom), going clockwise by 45 degrees.
 * Position 0 (S):  270 deg
 * Position 1 (SW): 225 deg (270 - 45)
 * Position 2 (W):  180 deg
 * Position 3 (NW): 135 deg
 * Position 4 (N):  90 deg
 * Position 5 (NE): 45 deg
 * Position 6 (E):  0 deg
 * Position 7 (SE): 315 deg (360 - 45) */
static float position_angle(int pos)
{
    /* Clockwise from S: each step subtracts 45 degrees */
    float angle = 270.0f - (float)pos * 45.0f;
    if (angle < 0.0f) angle += 360.0f;
    return angle;
}

bagua_layout_t bagua_layout_compute(int highlight_trigram)
{
    bagua_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    layout.center_x = CENTER_X;
    layout.center_y = CENTER_Y;
    layout.radius   = RADIUS;

    /* Validate highlight */
    if (highlight_trigram < 0 || highlight_trigram > 7) {
        layout.highlighted = -1;
    } else {
        layout.highlighted = highlight_trigram;
    }

    for (int pos = 0; pos < BAGUA_TRIGRAM_COUNT; pos++) {
        bagua_trigram_t *t = &layout.trigrams[pos];
        int tri = LH_TRIGRAM_INDEX[pos];

        t->trigram_index = tri;
        t->name          = ENGLISH_NAMES[tri];
        t->symbol        = iching_trigram_symbol(tri);
        t->direction     = DIRECTION_NAMES[pos];
        t->element       = ELEMENT_NAMES[tri];

        /* Position on wheel */
        float angle = position_angle(pos);
        t->angle_deg = angle;
        double rad = (double)angle * M_PI / 180.0;
        t->x = CENTER_X + RADIUS * (float)cos(rad);
        t->y = CENTER_Y - RADIUS * (float)sin(rad);  /* y flipped */

        /* Trigram lines */
        t->lines[0] = TRIGRAM_LINES[tri][0];
        t->lines[1] = TRIGRAM_LINES[tri][1];
        t->lines[2] = TRIGRAM_LINES[tri][2];
    }

    return layout;
}

bagua_trigram_t bagua_trigram_at(const bagua_layout_t *layout, int index)
{
    bagua_trigram_t empty;
    memset(&empty, 0, sizeof(empty));
    empty.trigram_index = -1;

    if (!layout || index < 0 || index >= BAGUA_TRIGRAM_COUNT) {
        return empty;
    }

    return layout->trigrams[index];
}

const char *bagua_direction_name(int position)
{
    if (position < 0 || position >= BAGUA_TRIGRAM_COUNT) {
        return "?";
    }
    return DIRECTION_NAMES[position];
}
