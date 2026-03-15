/* layer_panel.c — Structured layer/system data for the layer toggle panel UI.
 *
 * All data is static const. No malloc, no globals, no side effects.
 * Render layer IDs are raw integers to avoid coupling to render_layers.h. */

#include "layer_panel.h"

#include <stdio.h>
#include <string.h>

/* ---- Category names ---- */

static const char *s_category_names[LPANEL_CAT_COUNT] = {
    "Astronomy",
    "Astrology",
    "Calendar Systems",
    "Sacred Systems",
    "Earth",
    "Display"
};

/* ---- Static item definitions per category ---- */

static const lpanel_item_t s_astronomy_items[] = {
    { "Stars",        "S",  LPANEL_CAT_ASTRONOMY,  0, 1 },
    { "Galaxy",       NULL, LPANEL_CAT_ASTRONOMY,  1, 1 },
    { "Orbits",       NULL, LPANEL_CAT_ASTRONOMY,  2, 1 },
    { "Planets",      NULL, LPANEL_CAT_ASTRONOMY,  3, 1 },
    { "Orbit Trails", "T",  LPANEL_CAT_ASTRONOMY, -1, 0 }
};

static const lpanel_item_t s_astrology_items[] = {
    { "Zodiac Ring",     NULL, LPANEL_CAT_ASTROLOGY, 4, 1 },
    { "Aspects",         NULL, LPANEL_CAT_ASTROLOGY, -1, 1 },
    { "Houses",          NULL, LPANEL_CAT_ASTROLOGY, -1, 1 },
    { "Planetary Hours", NULL, LPANEL_CAT_ASTROLOGY, -1, 0 }
};

static const lpanel_item_t s_calendar_items[] = {
    { "Gregorian", NULL, LPANEL_CAT_CALENDAR, -1, 1 },
    { "Chinese",   "C",  LPANEL_CAT_CALENDAR, -1, 1 },
    { "Hebrew",    NULL, LPANEL_CAT_CALENDAR, -1, 0 },
    { "Islamic",   NULL, LPANEL_CAT_CALENDAR, -1, 0 },
    { "Buddhist",  NULL, LPANEL_CAT_CALENDAR, -1, 0 }
};

static const lpanel_item_t s_sacred_items[] = {
    { "Tzolkin",      "K",  LPANEL_CAT_SACRED, -1, 1 },
    { "I Ching",      "I",  LPANEL_CAT_SACRED, -1, 1 },
    { "Human Design", "D",  LPANEL_CAT_SACRED, -1, 0 },
    { "Kabbalah",     NULL, LPANEL_CAT_SACRED, -1, 0 }
};

static const lpanel_item_t s_earth_items[] = {
    { "Tides",    NULL, LPANEL_CAT_EARTH, -1, 0 },
    { "Weather",  NULL, LPANEL_CAT_EARTH, -1, 0 },
    { "Daylight", NULL, LPANEL_CAT_EARTH, -1, 0 },
    { "Geology",  NULL, LPANEL_CAT_EARTH, -1, 0 }
};

static const lpanel_item_t s_display_items[] = {
    { "Labels", "L",  LPANEL_CAT_DISPLAY, 5, 1 },
    { "Cards",  NULL, LPANEL_CAT_DISPLAY, 6, 1 },
    { "HUD",    "H",  LPANEL_CAT_DISPLAY, 7, 1 },
    { "Grid",   "G",  LPANEL_CAT_DISPLAY, -1, 0 }
};

/* Item arrays + counts for indexed access */
static const lpanel_item_t *s_all_items[LPANEL_CAT_COUNT] = {
    s_astronomy_items,
    s_astrology_items,
    s_calendar_items,
    s_sacred_items,
    s_earth_items,
    s_display_items
};

static const int s_item_counts[LPANEL_CAT_COUNT] = {
    (int)(sizeof(s_astronomy_items) / sizeof(s_astronomy_items[0])),
    (int)(sizeof(s_astrology_items) / sizeof(s_astrology_items[0])),
    (int)(sizeof(s_calendar_items)  / sizeof(s_calendar_items[0])),
    (int)(sizeof(s_sacred_items)    / sizeof(s_sacred_items[0])),
    (int)(sizeof(s_earth_items)     / sizeof(s_earth_items[0])),
    (int)(sizeof(s_display_items)   / sizeof(s_display_items[0]))
};

/* ---- API implementation ---- */

const char *lpanel_category_name(lpanel_category_t cat)
{
    if (cat < 0 || cat >= LPANEL_CAT_COUNT) {
        return NULL;
    }
    return s_category_names[cat];
}

lpanel_group_t lpanel_get_group(lpanel_category_t cat)
{
    lpanel_group_t g;
    memset(&g, 0, sizeof(g));

    if (cat < 0 || cat >= LPANEL_CAT_COUNT) {
        return g;
    }

    g.name     = s_category_names[cat];
    g.icon     = NULL;
    g.category = cat;
    g.count    = s_item_counts[cat];

    int n = g.count;
    if (n > LPANEL_MAX_ITEMS_PER_GROUP) {
        n = LPANEL_MAX_ITEMS_PER_GROUP;
    }
    for (int i = 0; i < n; i++) {
        g.items[i] = s_all_items[cat][i];
    }
    g.count = n;

    return g;
}

int lpanel_get_items(lpanel_category_t cat, lpanel_item_t *out, int max_items)
{
    if (max_items <= 0 || out == NULL || cat < 0 || cat >= LPANEL_CAT_COUNT) {
        return 0;
    }

    int n = s_item_counts[cat];
    if (n > max_items) {
        n = max_items;
    }
    for (int i = 0; i < n; i++) {
        out[i] = s_all_items[cat][i];
    }
    return n;
}

int lpanel_total_items(void)
{
    int total = 0;
    for (int i = 0; i < LPANEL_CAT_COUNT; i++) {
        total += s_item_counts[i];
    }
    return total;
}

int lpanel_category_count(void)
{
    return LPANEL_CAT_COUNT;
}

lpanel_item_t lpanel_find_item(const char *name)
{
    lpanel_item_t not_found;
    memset(&not_found, 0, sizeof(not_found));

    if (name == NULL) {
        return not_found;
    }

    for (int i = 0; i < LPANEL_CAT_COUNT; i++) {
        for (int j = 0; j < s_item_counts[i]; j++) {
            if (strcmp(s_all_items[i][j].name, name) == 0) {
                return s_all_items[i][j];
            }
        }
    }
    return not_found;
}

lpanel_item_t lpanel_find_by_shortcut(const char *shortcut)
{
    lpanel_item_t not_found;
    memset(&not_found, 0, sizeof(not_found));

    if (shortcut == NULL) {
        return not_found;
    }

    for (int i = 0; i < LPANEL_CAT_COUNT; i++) {
        for (int j = 0; j < s_item_counts[i]; j++) {
            if (s_all_items[i][j].shortcut != NULL &&
                strcmp(s_all_items[i][j].shortcut, shortcut) == 0) {
                return s_all_items[i][j];
            }
        }
    }
    return not_found;
}

int lpanel_default_on_count(lpanel_category_t cat)
{
    if (cat < 0 || cat >= LPANEL_CAT_COUNT) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < s_item_counts[cat]; i++) {
        if (s_all_items[cat][i].default_on) {
            count++;
        }
    }
    return count;
}

int lpanel_format_group(lpanel_category_t cat, char *buf, int buf_size)
{
    if (buf == NULL || buf_size <= 0 || cat < 0 || cat >= LPANEL_CAT_COUNT) {
        return 0;
    }

    int pos = 0;
    int remaining = buf_size;

    /* Header */
    int written = snprintf(buf + pos, (size_t)remaining,
                           "=== %s ===\n", s_category_names[cat]);
    if (written < 0 || written >= remaining) {
        buf[0] = '\0';
        return 0;
    }
    pos += written;
    remaining -= written;

    /* Items */
    for (int i = 0; i < s_item_counts[cat]; i++) {
        const lpanel_item_t *item = &s_all_items[cat][i];
        const char *check = item->default_on ? "[x]" : "[ ]";

        if (item->shortcut != NULL) {
            written = snprintf(buf + pos, (size_t)remaining,
                               "  %s %-16s %s\n", check, item->name, item->shortcut);
        } else {
            written = snprintf(buf + pos, (size_t)remaining,
                               "  %s %-16s\n", check, item->name);
        }

        if (written < 0 || written >= remaining) {
            /* Truncate at last complete line */
            buf[pos] = '\0';
            return pos;
        }
        pos += written;
        remaining -= written;
    }

    return pos;
}

int lpanel_format_all(char *buf, int buf_size)
{
    if (buf == NULL || buf_size <= 0) {
        return 0;
    }

    int pos = 0;
    int remaining = buf_size;

    for (int i = 0; i < LPANEL_CAT_COUNT; i++) {
        /* Add blank line between groups (not before first) */
        if (i > 0 && remaining > 1) {
            buf[pos] = '\n';
            pos++;
            remaining--;
        }

        /* Format this group into remaining buffer */
        int written = lpanel_format_group((lpanel_category_t)i,
                                          buf + pos, remaining);
        if (written <= 0) {
            buf[pos] = '\0';
            if (pos == 0) {
                return 0;
            }
            return pos;
        }
        pos += written;
        remaining -= written;
    }

    return pos;
}
