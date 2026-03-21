/* daily_hebrew_layout.c — Daily Hebrew calendar card layout
 *
 * Converts JD to Hebrew date, looks up month interpretation, and
 * computes card slot positions for rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_hebrew_layout.h"
#include "../systems/hebrew/hebrew.h"
#include "../systems/hebrew/hebrew_interpret.h"
#include <string.h>
#include <math.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.10f

#define TITLE_Y      0.05f
#define DATE_Y       0.15f
#define MONTH_Y      0.28f
#define LETTER_Y     0.40f
#define TRIBE_Y      0.52f
#define QUALITY_Y    0.65f
#define BRIEF_Y      0.78f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

/* Day-of-week from Julian Day.
 * JD 0 = Monday.  (int)(jd + 0.5) % 7: 0=Mon, 1=Tue, ..., 5=Sat, 6=Sun */
static int day_of_week_from_jd(double jd)
{
    long day_number = (long)floor(jd + 0.5);
    int dow = (int)(day_number % 7);
    if (dow < 0) {
        dow += 7;
    }
    return dow;  /* 0=Mon, 1=Tue, 2=Wed, 3=Thu, 4=Fri, 5=Sat, 6=Sun */
}

daily_hebrew_layout_t daily_hebrew_compute(double jd)
{
    daily_hebrew_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    /* Convert JD to Hebrew date */
    layout.date = hebrew_from_jd(jd);

    /* Determine leap year and month name */
    layout.is_leap = hebrew_is_leap(layout.date.year) ? 1 : 0;
    layout.month_name = hebrew_month_name(layout.date.month,
                                          layout.is_leap != 0);

    /* Get month interpretation */
    hi_month_t m = hi_month_data(layout.date.month);
    layout.hebrew_letter = m.hebrew_letter;
    layout.tribe = m.tribe;
    layout.sense = m.sense;
    layout.quality = m.quality;
    layout.brief = m.brief;

    /* Title slot */
    layout.title_slot.x = SLOT_X;
    layout.title_slot.y = TITLE_Y;
    layout.title_slot.w = SLOT_W;
    layout.title_slot.h = SLOT_H;

    /* Date slot */
    layout.date_slot.x = SLOT_X;
    layout.date_slot.y = DATE_Y;
    layout.date_slot.w = SLOT_W;
    layout.date_slot.h = SLOT_H;

    /* Month slot */
    layout.month_slot.x = SLOT_X;
    layout.month_slot.y = MONTH_Y;
    layout.month_slot.w = SLOT_W;
    layout.month_slot.h = SLOT_H;

    /* Letter slot */
    layout.letter_slot.x = SLOT_X;
    layout.letter_slot.y = LETTER_Y;
    layout.letter_slot.w = SLOT_W;
    layout.letter_slot.h = SLOT_H;

    /* Tribe slot */
    layout.tribe_slot.x = SLOT_X;
    layout.tribe_slot.y = TRIBE_Y;
    layout.tribe_slot.w = SLOT_W;
    layout.tribe_slot.h = SLOT_H;

    /* Quality slot */
    layout.quality_slot.x = SLOT_X;
    layout.quality_slot.y = QUALITY_Y;
    layout.quality_slot.w = SLOT_W;
    layout.quality_slot.h = SLOT_H;

    /* Brief slot */
    layout.brief_slot.x = SLOT_X;
    layout.brief_slot.y = BRIEF_Y;
    layout.brief_slot.w = SLOT_W;
    layout.brief_slot.h = SLOT_H;

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_hebrew_month_quality(const daily_hebrew_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->quality;
}

int daily_hebrew_is_shabbat(const daily_hebrew_layout_t *layout)
{
    if (!layout) {
        return 0;
    }
    /* Reconstruct JD from the stored date to compute day-of-week */
    double jd = hebrew_to_jd(layout->date);
    int dow = day_of_week_from_jd(jd);
    return (dow == 5) ? 1 : 0;  /* 5 = Saturday */
}
