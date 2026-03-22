/* daily_kabbalah_layout.c — Daily Kabbalah Tree of Life card layout
 *
 * Converts JD to daily Sefirah (mod 10) and path (mod 22),
 * looks up interpretation data, computes card slot positions.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_kabbalah_layout.h"
#include "../systems/kabbalah/sefirot.h"
#include "../systems/kabbalah/tree_geometry.h"
#include "../systems/kabbalah/kabbalah_interpret.h"
#include <string.h>
#include <math.h>

/* Slot layout constants */
#define SLOT_X       0.10f
#define SLOT_W       0.80f
#define SLOT_H       0.08f

#define TITLE_Y      0.03f
#define SEFIRAH_Y    0.12f
#define PILLAR_Y     0.22f
#define MEDITATE_Y   0.33f
#define QUESTION_Y   0.46f
#define PATH_Y       0.59f
#define JOURNEY_Y    0.72f

/* Card frame */
#define CARD_X       0.05f
#define CARD_Y       0.02f
#define CARD_W       0.90f
#define CARD_H       0.96f

static kabbalah_slot_t make_slot(float y)
{
    kabbalah_slot_t s;
    s.x = SLOT_X;
    s.y = y;
    s.w = SLOT_W;
    s.h = SLOT_H;
    return s;
}

/* Positive modulo for cycling through indices */
static int pos_mod(long val, int mod)
{
    int r = (int)(val % mod);
    if (r < 0) r += mod;
    return r;
}

daily_kabbalah_layout_t daily_kabbalah_compute(double jd)
{
    daily_kabbalah_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    long day_num = (long)floor(jd + 0.5);

    /* Today's Sefirah (cycle through 10) */
    layout.sefirah_index = pos_mod(day_num, SEFIROT_COUNT);
    sefirah_t sef = sefirot_get(layout.sefirah_index);
    layout.sefirah_name = sef.name;
    layout.sefirah_meaning = sef.meaning;
    layout.pillar_name = sefirot_pillar_name(sef.pillar);
    layout.triad_name = sefirot_triad_name(sef.triad);

    /* Sefirah interpretation */
    ki_sefirah_t interp = ki_sefirah_data(layout.sefirah_index);
    layout.meditation = interp.meditation;
    layout.question = interp.question;
    layout.brief = interp.brief;

    /* Today's path (cycle through 22) */
    layout.path_index = pos_mod(day_num, TREE_PATH_COUNT);
    tree_path_t path = tree_path_get(layout.path_index);
    layout.hebrew_letter = path.hebrew_letter;
    layout.tarot = path.tarot;

    /* Path interpretation */
    ki_path_t pi = ki_path_data(layout.path_index);
    layout.path_journey = pi.journey;

    /* Layout slots */
    layout.title_slot    = make_slot(TITLE_Y);
    layout.sefirah_slot  = make_slot(SEFIRAH_Y);
    layout.pillar_slot   = make_slot(PILLAR_Y);
    layout.meditate_slot = make_slot(MEDITATE_Y);
    layout.question_slot = make_slot(QUESTION_Y);
    layout.path_slot     = make_slot(PATH_Y);
    layout.journey_slot  = make_slot(JOURNEY_Y);

    /* Card frame */
    layout.card_x = CARD_X;
    layout.card_y = CARD_Y;
    layout.card_w = CARD_W;
    layout.card_h = CARD_H;

    return layout;
}

const char *daily_kabbalah_meditation(const daily_kabbalah_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->meditation;
}

const char *daily_kabbalah_letter(const daily_kabbalah_layout_t *layout)
{
    if (!layout) {
        return "?";
    }
    return layout->hebrew_letter;
}
