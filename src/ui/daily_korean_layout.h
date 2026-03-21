/* daily_korean_layout.h — Daily Korean calendar card layout
 *
 * Computes a card layout showing today's Korean date with Dangun
 * era year, zodiac animal, element, polarity, and active festival.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_KOREAN_LAYOUT_H
#define TIME_DAILY_KOREAN_LAYOUT_H

#include "../systems/korean/korean_calendar.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} korean_slot_t;

typedef struct {
    /* Date */
    korean_date_t date;           /* dangun_year, animal, element, etc. */

    /* Zodiac interpretation */
    const char *animal_name;      /* Korean zodiac animal name */
    const char *element_name;     /* "Mok (Wood)", etc. */
    const char *polarity_name;    /* "Yang" or "Yin" */
    const char *sexagenary;       /* Full cycle name */

    /* Festival (if active) */
    const char *festival_name;    /* Festival name or "" if none */
    const char *festival_korean;  /* Korean script or "" if none */
    int has_festival;             /* 1 if a festival is active today */

    /* Layout slots */
    korean_slot_t title_slot;     /* "Korean Calendar" header */
    korean_slot_t date_slot;      /* Dangun year + Gregorian */
    korean_slot_t animal_slot;    /* zodiac animal + element */
    korean_slot_t cycle_slot;     /* sexagenary cycle name */
    korean_slot_t festival_slot;  /* festival (if active) */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_korean_layout_t;

/* Compute the daily Korean card layout.
 * jd: Julian Day number. */
daily_korean_layout_t daily_korean_compute(double jd);

/* Get the zodiac animal name. Returns "?" if NULL layout. */
const char *daily_korean_animal(const daily_korean_layout_t *layout);

/* Check if the given date falls on a Korean festival.
 * Returns 1 if festival active, 0 otherwise. Returns 0 for NULL layout. */
int daily_korean_has_festival(const daily_korean_layout_t *layout);

#endif /* TIME_DAILY_KOREAN_LAYOUT_H */
