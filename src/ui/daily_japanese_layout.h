/* daily_japanese_layout.h — Daily Japanese calendar card layout
 *
 * Computes a card layout showing today's Japanese date with imperial
 * era, rokuyo divination, sekki solar term, and zodiac animal.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DAILY_JAPANESE_LAYOUT_H
#define TIME_DAILY_JAPANESE_LAYOUT_H

#include "../systems/japanese/japanese.h"

/* Slot rectangle for positioning text elements. */
typedef struct {
    float x, y;
    float w, h;
} japanese_slot_t;

typedef struct {
    /* Date */
    japanese_date_t date;         /* era, era_year, greg_year, month, day */
    const char *era_name;         /* "Reiwa", etc. */
    const char *era_kanji;        /* Japanese characters */

    /* Rokuyo (6-day divination cycle) */
    int rokuyo;                   /* 0-5 */
    const char *rokuyo_name;      /* "Taian", etc. */
    const char *rokuyo_meaning;   /* "Great peace", etc. */

    /* Sekki (24 solar terms) */
    int sekki;                    /* 0-23 */
    const char *sekki_name;       /* Romanized name */
    const char *sekki_kanji;      /* Japanese characters */

    /* Zodiac animal */
    int animal;                   /* 0-11 */
    const char *animal_name;      /* Japanese zodiac name */
    const char *animal_kanji;     /* Japanese characters */

    /* Layout slots */
    japanese_slot_t title_slot;   /* "Japanese Calendar" header */
    japanese_slot_t date_slot;    /* era + year + month/day */
    japanese_slot_t rokuyo_slot;  /* rokuyo name + meaning */
    japanese_slot_t sekki_slot;   /* sekki solar term */
    japanese_slot_t animal_slot;  /* zodiac animal */

    /* Card frame */
    float card_x, card_y, card_w, card_h;
} daily_japanese_layout_t;

/* Compute the daily Japanese card layout.
 * jd: Julian Day number.
 * solar_longitude_deg: Sun's ecliptic longitude in degrees (for sekki). */
daily_japanese_layout_t daily_japanese_compute(double jd,
                                               double solar_longitude_deg);

/* Get the rokuyo meaning string. Returns "?" if NULL layout. */
const char *daily_japanese_rokuyo(const daily_japanese_layout_t *layout);

/* Get the sekki name string. Returns "?" if NULL layout. */
const char *daily_japanese_sekki(const daily_japanese_layout_t *layout);

#endif /* TIME_DAILY_JAPANESE_LAYOUT_H */
