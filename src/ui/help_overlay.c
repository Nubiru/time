#include "help_overlay.h"

#include <stdio.h>
#include <string.h>

/* ---- Category metadata ---- */

static const char *const CATEGORY_NAMES[HELP_CAT_COUNT] = {
    "Time Controls",
    "Camera",
    "Layers",
    "Views",
    "Systems",
    "UI"
};

static const char *const CATEGORY_DESCS[HELP_CAT_COUNT] = {
    "Control playback speed, pause, and time direction",
    "Navigate the camera: zoom, rotate, jump to scale",
    "Toggle visual layers on and off",
    "Switch between view modes",
    "Focus on a specific knowledge system",
    "Interface controls and overlays"
};

/* ---- Static shortcut data ---- */

/* Time Controls */
static const help_entry_t TIME_ENTRIES[] = {
    { "Space", "Toggle pause/resume",       HELP_CAT_TIME },
    { "0",     "Pause (0x speed)",           HELP_CAT_TIME },
    { "1",     "Real-time (1x)",             HELP_CAT_TIME },
    { "2",     "Fast (60x)",                 HELP_CAT_TIME },
    { "3",     "Faster (3600x)",             HELP_CAT_TIME },
    { "4",     "Day per second (86400x)",    HELP_CAT_TIME },
    { "5",     "10 days per second (864000x)", HELP_CAT_TIME },
    { "-",     "Reverse time direction",     HELP_CAT_TIME },
};
#define TIME_ENTRY_COUNT 8

/* Camera */
static const help_entry_t CAMERA_ENTRIES[] = {
    { "Scroll",  "Zoom in/out",              HELP_CAT_CAMERA },
    { "Drag",    "Rotate camera",            HELP_CAT_CAMERA },
    { "Shift+0", "Personal Chart scale",     HELP_CAT_CAMERA },
    { "Shift+1", "Earth-Moon scale",         HELP_CAT_CAMERA },
    { "Shift+2", "Inner Solar System",       HELP_CAT_CAMERA },
    { "Shift+3", "Solar System (default)",   HELP_CAT_CAMERA },
    { "Shift+4", "Solar Neighborhood",       HELP_CAT_CAMERA },
    { "Shift+5", "Milky Way",               HELP_CAT_CAMERA },
    { "Shift+6", "Observable Universe",      HELP_CAT_CAMERA },
};
#define CAMERA_ENTRY_COUNT 9

/* Layers */
static const help_entry_t LAYER_ENTRIES[] = {
    { "T", "Toggle orbit trails",      HELP_CAT_LAYERS },
    { "H", "Toggle HUD overlay",       HELP_CAT_LAYERS },
    { "S", "Toggle star field",         HELP_CAT_LAYERS },
    { "G", "Toggle grid/ecliptic",      HELP_CAT_LAYERS },
    { "L", "Toggle labels",             HELP_CAT_LAYERS },
};
#define LAYER_ENTRY_COUNT 5

/* Views */
static const help_entry_t VIEW_ENTRIES[] = {
    { "E",      "Earth view",                HELP_CAT_VIEWS },
    { "Escape", "Overview / dismiss panel",  HELP_CAT_VIEWS },
};
#define VIEW_ENTRY_COUNT 2

/* Systems */
static const help_entry_t SYSTEM_ENTRIES[] = {
    { "A", "Astrology focus",          HELP_CAT_SYSTEMS },
    { "K", "Kin/Tzolkin focus",        HELP_CAT_SYSTEMS },
    { "I", "I Ching focus",            HELP_CAT_SYSTEMS },
    { "C", "Chinese calendar focus",   HELP_CAT_SYSTEMS },
    { "D", "Human Design focus",       HELP_CAT_SYSTEMS },
    { "T", "Kabbalah/Tree of Life",    HELP_CAT_SYSTEMS },
};
#define SYSTEM_ENTRY_COUNT 6

/* UI */
static const help_entry_t UI_ENTRIES[] = {
    { "?", "Show help overlay",        HELP_CAT_UI },
    { "/", "Command palette (future)", HELP_CAT_UI },
    { ",", "Settings panel",           HELP_CAT_UI },
};
#define UI_ENTRY_COUNT 3

/* Entry table indexed by category */
static const help_entry_t *const ENTRY_TABLES[HELP_CAT_COUNT] = {
    TIME_ENTRIES,
    CAMERA_ENTRIES,
    LAYER_ENTRIES,
    VIEW_ENTRIES,
    SYSTEM_ENTRIES,
    UI_ENTRIES,
};

static const int ENTRY_COUNTS[HELP_CAT_COUNT] = {
    TIME_ENTRY_COUNT,
    CAMERA_ENTRY_COUNT,
    LAYER_ENTRY_COUNT,
    VIEW_ENTRY_COUNT,
    SYSTEM_ENTRY_COUNT,
    UI_ENTRY_COUNT,
};

/* ---- Public API ---- */

const char *help_category_name(help_category_t cat)
{
    if (cat < 0 || cat >= HELP_CAT_COUNT) return "Unknown";
    return CATEGORY_NAMES[cat];
}

const char *help_category_desc(help_category_t cat)
{
    if (cat < 0 || cat >= HELP_CAT_COUNT) return "Unknown";
    return CATEGORY_DESCS[cat];
}

int help_get_entries(help_category_t cat, help_entry_t *out, int max_entries)
{
    if (!out || max_entries <= 0) return 0;
    if (cat < 0 || cat >= HELP_CAT_COUNT) return 0;

    int count = ENTRY_COUNTS[cat];
    if (count > max_entries) count = max_entries;

    const help_entry_t *src = ENTRY_TABLES[cat];
    for (int i = 0; i < count; i++) {
        out[i] = src[i];
    }
    return count;
}

help_group_t help_get_group(help_category_t cat)
{
    help_group_t group;
    memset(&group, 0, sizeof(group));

    if (cat < 0 || cat >= HELP_CAT_COUNT) {
        group.name = "Unknown";
        group.description = "Unknown";
        group.count = 0;
        return group;
    }

    group.name = CATEGORY_NAMES[cat];
    group.description = CATEGORY_DESCS[cat];
    group.count = help_get_entries(cat, group.entries, HELP_MAX_ENTRIES_PER_CAT);
    return group;
}

int help_total_entries(void)
{
    int total = 0;
    for (int i = 0; i < HELP_CAT_COUNT; i++) {
        total += ENTRY_COUNTS[i];
    }
    return total;
}

int help_format_entry(const help_entry_t *entry, char *buf, int buf_size)
{
    if (!entry || !buf || buf_size <= 0) return 0;

    const char *key = entry->key ? entry->key : "???";
    const char *action = entry->action ? entry->action : "";

    int n = snprintf(buf, (size_t)buf_size, "  %-14s%s", key, action);

    if (n < 0) return 0;
    if (n >= buf_size) n = buf_size - 1;
    return n;
}

int help_format_group(help_category_t cat, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) return 0;
    if (cat < 0 || cat >= HELP_CAT_COUNT) return 0;

    int total = 0;

    /* Header line: "=== Category Name ===" */
    int n = snprintf(buf, (size_t)buf_size, "=== %s ===\n",
                     CATEGORY_NAMES[cat]);
    if (n < 0) return 0;
    if (n >= buf_size) return buf_size - 1;
    total += n;

    /* Entries */
    int count = ENTRY_COUNTS[cat];
    const help_entry_t *entries = ENTRY_TABLES[cat];

    for (int i = 0; i < count; i++) {
        if (total >= buf_size - 1) break;

        int remaining = buf_size - total;
        int en = help_format_entry(&entries[i], buf + total, remaining);
        total += en;

        /* Newline after each entry */
        if (total < buf_size - 1) {
            buf[total] = '\n';
            total++;
        }
    }

    if (total < buf_size) {
        buf[total] = '\0';
    } else {
        buf[buf_size - 1] = '\0';
        total = buf_size - 1;
    }

    return total;
}

int help_format_all(char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) return 0;

    int total = 0;

    for (int cat = 0; cat < HELP_CAT_COUNT; cat++) {
        if (total >= buf_size - 1) break;

        /* Blank line between categories (not before first) */
        if (cat > 0 && total < buf_size - 1) {
            buf[total] = '\n';
            total++;
        }

        int remaining = buf_size - total;
        int n = help_format_group((help_category_t)cat, buf + total, remaining);
        total += n;
    }

    if (total < buf_size) {
        buf[total] = '\0';
    } else {
        buf[buf_size - 1] = '\0';
        total = buf_size - 1;
    }

    return total;
}

int help_category_count(void)
{
    return HELP_CAT_COUNT;
}
