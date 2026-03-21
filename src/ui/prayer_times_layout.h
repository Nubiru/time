/* prayer_times_layout.h — Islamic prayer times vertical timeline layout
 *
 * Displays 6 daily Islamic prayer times (Fajr, Sunrise, Dhuhr, Asr,
 * Maghrib, Isha) as a vertical timeline, showing each prayer name,
 * time, and the current active prayer window.
 * All positions in normalized 0-1 coordinate space.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_PRAYER_TIMES_LAYOUT_H
#define TIME_PRAYER_TIMES_LAYOUT_H

#include "../systems/islamic/prayer_times.h"

typedef struct {
    float x, y, w, h;
} pt_slot_t;

typedef struct {
    int index;              /* 0=Fajr, 1=Sunrise, 2=Dhuhr, 3=Asr, 4=Maghrib, 5=Isha */
    const char *name;       /* "Fajr", "Sunrise", "Dhuhr", "Asr", "Maghrib", "Isha" */
    double jd;              /* JD of this prayer time */
    pt_slot_t slot;         /* position in the layout */
    int is_next;            /* 1 if this is the next upcoming prayer */
} pt_prayer_slot_t;

#define PT_PRAYER_COUNT 6   /* Fajr, Sunrise, Dhuhr, Asr, Maghrib, Isha */

typedef struct {
    pt_prayer_slot_t prayers[PT_PRAYER_COUNT];
    int prayer_count;       /* always 6 */
    const char *method_name;

    pt_slot_t title_slot;
    pt_slot_t method_slot;

    float card_x, card_y, card_w, card_h;
} prayer_times_layout_t;

/* Compute prayer time layout.
 * jd_noon: noon UTC of the day.
 * lat, lon: observer location in degrees.
 * method: 0-4 (prayer_method_t).
 * current_jd: used to determine which prayer is next. */
prayer_times_layout_t prayer_times_layout_compute(
    double jd_noon, double lat, double lon,
    int method, double current_jd);

/* Return name of the next upcoming prayer, or "?" if none/NULL. */
const char *prayer_times_next_name(const prayer_times_layout_t *layout);

#endif /* TIME_PRAYER_TIMES_LAYOUT_H */
