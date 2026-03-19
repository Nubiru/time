/* pass_schedule.c — Render pass scheduling and orchestration.
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "pass_schedule.h"

/* Draw order constants for Space View (background to foreground) */
#define ORDER_MILKYWAY       0
#define ORDER_STARS           1
#define ORDER_CONSTELLATION   2
#define ORDER_DEEP_SKY        3
#define ORDER_PLANET          4
#define ORDER_MOON            5
#define ORDER_SATURN          6
#define ORDER_ORBIT_TRAIL     7
#define ORDER_ZODIAC          8
#define ORDER_EARTH           9
#define ORDER_BODYGRAPH      10
#define ORDER_HEXAGRAM       11
#define ORDER_TREE           12
#define ORDER_CARD           13
#define ORDER_TEXT           14
#define ORDER_DIFFRACTION    15
#define ORDER_POST           16

/* LOD tier values (matching render_lod.h without including it) */
#define PS_LOD_ULTRA   0
#define PS_LOD_HIGH    1
#define PS_LOD_MEDIUM  2
#define PS_LOD_LOW     3
#define PS_LOD_MINIMAL 4

/* View IDs (matching view_registry.h without including it) */
#define PS_VIEW_SPACE  0
#define PS_VIEW_EARTH  1
#define PS_VIEW_GALAXY 2

/* Focus modes (matching focus_mode.h without including it) */
#define PS_FOCUS_OVERVIEW  0
#define PS_FOCUS_ASTROLOGY 1
#define PS_FOCUS_KIN       2
#define PS_FOCUS_ICHING    3
#define PS_FOCUS_CHINESE   4
#define PS_FOCUS_HD        5

/* ---------- internal helpers ---------- */

static int count_enabled(const pass_schedule_t *s) {
    int n = 0;
    for (int i = 0; i < PASS_COUNT; i++) {
        if (s->passes[i].enabled) {
            n++;
        }
    }
    return n;
}

static pass_schedule_t make_all_enabled(void) {
    pass_schedule_t s;
    /* Default draw orders */
    static const int draw_orders[PASS_COUNT] = {
        ORDER_STARS,
        ORDER_MILKYWAY,
        ORDER_CONSTELLATION,
        ORDER_PLANET,
        ORDER_MOON,
        ORDER_SATURN,
        ORDER_ORBIT_TRAIL,
        ORDER_ZODIAC,
        ORDER_DIFFRACTION,
        ORDER_DEEP_SKY,
        ORDER_EARTH,
        ORDER_BODYGRAPH,
        ORDER_HEXAGRAM,
        ORDER_TREE,
        ORDER_CARD,
        ORDER_TEXT,
        ORDER_POST
    };

    for (int i = 0; i < PASS_COUNT; i++) {
        s.passes[i].enabled = 1;
        s.passes[i].draw_order = draw_orders[i];
        s.passes[i].opacity_scale = 1.0f;
    }
    s.active_count = PASS_COUNT;
    return s;
}

static void disable_pass(pass_schedule_t *s, pass_id_t p) {
    if (p >= 0 && p < PASS_COUNT) {
        s->passes[p].enabled = 0;
    }
}

static void apply_lod_reductions(pass_schedule_t *s, int lod_tier) {
    /* LOD_MEDIUM: disable orbit_trail, deep_sky */
    if (lod_tier >= PS_LOD_MEDIUM) {
        disable_pass(s, PASS_ORBIT_TRAIL);
        disable_pass(s, PASS_DEEP_SKY);
    }

    /* LOD_LOW: also disable diffraction, constellation, bodygraph, hexagram, tree */
    if (lod_tier >= PS_LOD_LOW) {
        disable_pass(s, PASS_DIFFRACTION);
        disable_pass(s, PASS_CONSTELLATION);
        disable_pass(s, PASS_BODYGRAPH);
        disable_pass(s, PASS_HEXAGRAM);
        disable_pass(s, PASS_TREE);
    }

    /* LOD_MINIMAL: only stars, planets, zodiac, card, text, post */
    if (lod_tier >= PS_LOD_MINIMAL) {
        disable_pass(s, PASS_MILKYWAY);
        disable_pass(s, PASS_MOON);
        disable_pass(s, PASS_SATURN);
        disable_pass(s, PASS_EARTH);
    }

    s->active_count = count_enabled(s);
}

/* ---------- public API ---------- */

pass_schedule_t ps_space_view(int lod_tier) {
    pass_schedule_t s = make_all_enabled();
    apply_lod_reductions(&s, lod_tier);
    return s;
}

pass_schedule_t ps_earth_view(int lod_tier) {
    pass_schedule_t s = make_all_enabled();

    /* Disabled in Earth View: zodiac ring, orbit trails, saturn,
     * earth globe, bodygraph, hexagram, tree */
    disable_pass(&s, PASS_ZODIAC);
    disable_pass(&s, PASS_ORBIT_TRAIL);
    disable_pass(&s, PASS_SATURN);
    disable_pass(&s, PASS_EARTH);
    disable_pass(&s, PASS_BODYGRAPH);
    disable_pass(&s, PASS_HEXAGRAM);
    disable_pass(&s, PASS_TREE);

    apply_lod_reductions(&s, lod_tier);
    return s;
}

pass_schedule_t ps_galaxy_view(int lod_tier) {
    pass_schedule_t s = make_all_enabled();

    /* Disabled at galaxy scale: planets, moon, saturn, orbit trail,
     * zodiac, earth, bodygraph, hexagram, tree, card, diffraction */
    disable_pass(&s, PASS_PLANET);
    disable_pass(&s, PASS_MOON);
    disable_pass(&s, PASS_SATURN);
    disable_pass(&s, PASS_ORBIT_TRAIL);
    disable_pass(&s, PASS_ZODIAC);
    disable_pass(&s, PASS_EARTH);
    disable_pass(&s, PASS_BODYGRAPH);
    disable_pass(&s, PASS_HEXAGRAM);
    disable_pass(&s, PASS_TREE);
    disable_pass(&s, PASS_CARD);
    disable_pass(&s, PASS_DIFFRACTION);

    apply_lod_reductions(&s, lod_tier);
    return s;
}

pass_schedule_t ps_for_view(int view_id, int lod_tier) {
    switch (view_id) {
    case PS_VIEW_SPACE:  return ps_space_view(lod_tier);
    case PS_VIEW_EARTH:  return ps_earth_view(lod_tier);
    case PS_VIEW_GALAXY: return ps_galaxy_view(lod_tier);
    default:             return ps_space_view(lod_tier);
    }
}

pass_schedule_t ps_apply_focus(pass_schedule_t schedule, int focus_mode) {
    if (focus_mode == PS_FOCUS_OVERVIEW) {
        return schedule;
    }

    /* Dim all enabled passes first */
    float dim = 0.3f;
    if (focus_mode == PS_FOCUS_KIN) {
        dim = 0.2f;
    }

    for (int i = 0; i < PASS_COUNT; i++) {
        if (schedule.passes[i].enabled) {
            schedule.passes[i].opacity_scale = dim;
        }
    }

    /* Then brighten the focused passes */
    switch (focus_mode) {
    case PS_FOCUS_ASTROLOGY:
        schedule.passes[PASS_ZODIAC].opacity_scale = 1.0f;
        schedule.passes[PASS_CARD].opacity_scale = 1.0f;
        schedule.passes[PASS_TEXT].opacity_scale = 1.0f;
        break;
    case PS_FOCUS_KIN:
        schedule.passes[PASS_CARD].opacity_scale = 1.0f;
        schedule.passes[PASS_TEXT].opacity_scale = 1.0f;
        break;
    case PS_FOCUS_ICHING:
        schedule.passes[PASS_HEXAGRAM].opacity_scale = 1.0f;
        schedule.passes[PASS_CARD].opacity_scale = 1.0f;
        schedule.passes[PASS_TEXT].opacity_scale = 1.0f;
        break;
    case PS_FOCUS_CHINESE:
        schedule.passes[PASS_CARD].opacity_scale = 1.0f;
        schedule.passes[PASS_TEXT].opacity_scale = 1.0f;
        break;
    case PS_FOCUS_HD:
        schedule.passes[PASS_BODYGRAPH].opacity_scale = 1.0f;
        schedule.passes[PASS_CARD].opacity_scale = 1.0f;
        schedule.passes[PASS_TEXT].opacity_scale = 1.0f;
        break;
    default:
        break;
    }

    /* Post always full opacity */
    schedule.passes[PASS_POST].opacity_scale = 1.0f;

    return schedule;
}

const char *ps_pass_name(pass_id_t pass) {
    static const char *names[PASS_COUNT] = {
        "Stars",
        "Milky Way",
        "Constellation",
        "Planet",
        "Moon",
        "Saturn",
        "Orbit Trail",
        "Zodiac",
        "Diffraction",
        "Deep Sky",
        "Earth",
        "Bodygraph",
        "Hexagram",
        "Tree",
        "Card",
        "Text",
        "Post"
    };

    if (pass < 0 || pass >= PASS_COUNT) {
        return "Unknown";
    }
    return names[pass];
}

int ps_is_enabled(const pass_schedule_t *schedule, pass_id_t pass) {
    if (pass < 0 || pass >= PASS_COUNT) {
        return 0;
    }
    return schedule->passes[pass].enabled;
}

int ps_active_count(const pass_schedule_t *schedule) {
    return count_enabled(schedule);
}
