/* Command palette data module — searchable registry of all application commands.
 * Pure module: static const data, no allocation, no globals, no side effects. */

#include "command_palette.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* -------------------------------------------------------------------
 * Static command registry — all application commands
 * ------------------------------------------------------------------- */

static const cmd_entry_t s_commands[] = {
    /* TIME (CMD_CAT_TIME) — 8 commands */
    { "Pause",                "Space",   "Pause or resume time flow",       CMD_CAT_TIME },
    { "Stop",                 "0",       "Stop time (0x speed)",            CMD_CAT_TIME },
    { "Real-time",            "1",       "Set real-time speed (1x)",        CMD_CAT_TIME },
    { "Fast",                 "2",       "Set fast speed (60x)",            CMD_CAT_TIME },
    { "Faster",               "3",       "Set faster speed (3600x)",        CMD_CAT_TIME },
    { "Day per second",       "4",       "One day per second (86400x)",     CMD_CAT_TIME },
    { "Ten days per second",  "5",       "Ten days per second",             CMD_CAT_TIME },
    { "Reverse",              "-",       "Reverse time direction",          CMD_CAT_TIME },

    /* CAMERA (CMD_CAT_CAMERA) — 7 commands */
    { "Personal Chart",       "Shift+0", "Zoom to personal chart scale",    CMD_CAT_CAMERA },
    { "Earth-Moon",           "Shift+1", "Zoom to Earth-Moon scale",        CMD_CAT_CAMERA },
    { "Inner Solar System",   "Shift+2", "Zoom to inner solar system",      CMD_CAT_CAMERA },
    { "Solar System",         "Shift+3", "Zoom to solar system (default)",  CMD_CAT_CAMERA },
    { "Solar Neighborhood",   "Shift+4", "Zoom to solar neighborhood",      CMD_CAT_CAMERA },
    { "Milky Way",            "Shift+5", "Zoom to Milky Way scale",         CMD_CAT_CAMERA },
    { "Observable Universe",  "Shift+6", "Zoom to observable universe",     CMD_CAT_CAMERA },

    /* LAYER (CMD_CAT_LAYER) — 5 commands */
    { "Toggle Stars",         "S",       "Show or hide star field",         CMD_CAT_LAYER },
    { "Toggle Orbits",        "T",       "Show or hide orbit trails",       CMD_CAT_LAYER },
    { "Toggle Grid",          "G",       "Show or hide ecliptic grid",      CMD_CAT_LAYER },
    { "Toggle Labels",        "L",       "Show or hide text labels",        CMD_CAT_LAYER },
    { "Toggle HUD",           "H",       "Show or hide heads-up display",   CMD_CAT_LAYER },

    /* VIEW (CMD_CAT_VIEW) — 2 commands */
    { "Earth View",           "E",       "Switch to Earth surface view",    CMD_CAT_VIEW },
    { "Overview",             "Escape",  "Return to overview / dismiss panel", CMD_CAT_VIEW },

    /* SYSTEM (CMD_CAT_SYSTEM) — 5 commands */
    { "Astrology",            "A",       "Focus on astrology system",       CMD_CAT_SYSTEM },
    { "Tzolkin",              "K",       "Focus on Mayan Tzolkin calendar", CMD_CAT_SYSTEM },
    { "I Ching",              "I",       "Focus on I Ching hexagrams",      CMD_CAT_SYSTEM },
    { "Chinese Calendar",     "C",       "Focus on Chinese calendar",       CMD_CAT_SYSTEM },
    { "Human Design",         "D",       "Focus on Human Design system",    CMD_CAT_SYSTEM },

    /* NAVIGATE (CMD_CAT_NAVIGATE) — 3 commands */
    { "Jump to Now",          "N",       "Set time to current moment",      CMD_CAT_NAVIGATE },
    { "Jump to Date",         NULL,      "Enter a specific date to visit",  CMD_CAT_NAVIGATE },
    { "Set Location",         NULL,      "Change observer location",        CMD_CAT_NAVIGATE },

    /* UI (CMD_CAT_UI) — 3 commands */
    { "Show Help",            "?",       "Display keyboard shortcut reference", CMD_CAT_UI },
    { "Command Palette",      "/",       "Open searchable command list",    CMD_CAT_UI },
    { "Toggle Theme",         NULL,      "Switch between Cosmos and Dawn themes", CMD_CAT_UI },
};

static const int s_command_count =
    (int)(sizeof(s_commands) / sizeof(s_commands[0]));

/* Category display names */
static const char *s_category_names[CMD_CAT_COUNT] = {
    "Time",
    "Camera",
    "Layer",
    "View",
    "System",
    "Navigate",
    "UI"
};

/* -------------------------------------------------------------------
 * Internal helpers
 * ------------------------------------------------------------------- */

/* Case-insensitive substring search (ASCII only).
 * Returns 1 if needle is found in haystack, 0 otherwise. */
static int ci_contains(const char *haystack, const char *needle)
{
    if (haystack == NULL || needle == NULL) {
        return 0;
    }

    int hlen = (int)strlen(haystack);
    int nlen = (int)strlen(needle);

    if (nlen == 0) {
        return 1;
    }
    if (nlen > hlen) {
        return 0;
    }

    for (int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (int j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i + j]) !=
                tolower((unsigned char)needle[j])) {
                match = 0;
                break;
            }
        }
        if (match) {
            return 1;
        }
    }
    return 0;
}

/* -------------------------------------------------------------------
 * Public API
 * ------------------------------------------------------------------- */

int cmd_palette_count(void)
{
    return s_command_count;
}

int cmd_palette_all(cmd_entry_t *out, int max)
{
    if (out == NULL || max <= 0) {
        return 0;
    }

    int n = s_command_count < max ? s_command_count : max;
    for (int i = 0; i < n; i++) {
        out[i] = s_commands[i];
    }
    return n;
}

cmd_result_t cmd_palette_search(const char *query)
{
    cmd_result_t result;
    result.count = 0;

    /* NULL or empty query: return all commands */
    if (query == NULL || query[0] == '\0') {
        int n = s_command_count < CMD_MAX_RESULTS
                    ? s_command_count : CMD_MAX_RESULTS;
        for (int i = 0; i < n; i++) {
            result.entries[result.count++] = s_commands[i];
        }
        return result;
    }

    /* Pass 1: exact name match */
    for (int i = 0; i < s_command_count; i++) {
        if (result.count >= CMD_MAX_RESULTS) {
            break;
        }
        if (ci_contains(s_commands[i].name, query) &&
            (int)strlen(s_commands[i].name) == (int)strlen(query)) {
            result.entries[result.count++] = s_commands[i];
        }
    }

    /* Pass 2: name contains query (skip already-added exact matches) */
    for (int i = 0; i < s_command_count; i++) {
        if (result.count >= CMD_MAX_RESULTS) {
            break;
        }
        if (ci_contains(s_commands[i].name, query)) {
            /* Skip if exact match already added */
            int already = 0;
            for (int j = 0; j < result.count; j++) {
                if (result.entries[j].name == s_commands[i].name) {
                    already = 1;
                    break;
                }
            }
            if (!already) {
                result.entries[result.count++] = s_commands[i];
            }
        }
    }

    /* Pass 3: shortcut matches query */
    for (int i = 0; i < s_command_count; i++) {
        if (result.count >= CMD_MAX_RESULTS) {
            break;
        }
        if (ci_contains(s_commands[i].shortcut, query)) {
            /* Skip if already added */
            int already = 0;
            for (int j = 0; j < result.count; j++) {
                if (result.entries[j].name == s_commands[i].name) {
                    already = 1;
                    break;
                }
            }
            if (!already) {
                result.entries[result.count++] = s_commands[i];
            }
        }
    }

    /* Pass 4: description contains query */
    for (int i = 0; i < s_command_count; i++) {
        if (result.count >= CMD_MAX_RESULTS) {
            break;
        }
        if (ci_contains(s_commands[i].description, query)) {
            /* Skip if already added */
            int already = 0;
            for (int j = 0; j < result.count; j++) {
                if (result.entries[j].name == s_commands[i].name) {
                    already = 1;
                    break;
                }
            }
            if (!already) {
                result.entries[result.count++] = s_commands[i];
            }
        }
    }

    return result;
}

int cmd_palette_by_category(cmd_category_t cat, cmd_entry_t *out, int max)
{
    if (out == NULL || max <= 0 || cat < 0 || cat >= CMD_CAT_COUNT) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < s_command_count && count < max; i++) {
        if (s_commands[i].category == cat) {
            out[count++] = s_commands[i];
        }
    }
    return count;
}

int cmd_palette_category_count(cmd_category_t cat)
{
    if (cat < 0 || cat >= CMD_CAT_COUNT) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < s_command_count; i++) {
        if (s_commands[i].category == cat) {
            count++;
        }
    }
    return count;
}

const char *cmd_category_name(cmd_category_t cat)
{
    if (cat < 0 || cat >= CMD_CAT_COUNT) {
        return NULL;
    }
    return s_category_names[cat];
}

int cmd_category_total(void)
{
    return CMD_CAT_COUNT;
}

cmd_entry_t cmd_palette_find(const char *name)
{
    cmd_entry_t not_found = { NULL, NULL, NULL, CMD_CAT_TIME };

    if (name == NULL) {
        return not_found;
    }

    for (int i = 0; i < s_command_count; i++) {
        if (strcmp(s_commands[i].name, name) == 0) {
            return s_commands[i];
        }
    }
    return not_found;
}

cmd_entry_t cmd_palette_find_by_shortcut(const char *shortcut)
{
    cmd_entry_t not_found = { NULL, NULL, NULL, CMD_CAT_TIME };

    if (shortcut == NULL) {
        return not_found;
    }

    for (int i = 0; i < s_command_count; i++) {
        if (s_commands[i].shortcut != NULL &&
            strcmp(s_commands[i].shortcut, shortcut) == 0) {
            return s_commands[i];
        }
    }
    return not_found;
}

int cmd_format_entry(const cmd_entry_t *entry, char *buf, int buf_size)
{
    if (entry == NULL || buf == NULL || buf_size <= 0 || entry->name == NULL) {
        return 0;
    }

    const char *shortcut = entry->shortcut != NULL ? entry->shortcut : "";
    int written = snprintf(buf, (size_t)buf_size, "%-24s %-10s %s",
                           entry->name, shortcut, entry->description);

    if (written < 0) {
        return 0;
    }
    if (written >= buf_size) {
        return buf_size - 1;
    }
    return written;
}

int cmd_format_results(const cmd_result_t *results, char *buf, int buf_size)
{
    if (results == NULL || buf == NULL || buf_size <= 0 ||
        results->count <= 0) {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < results->count; i++) {
        int remaining = buf_size - total;
        if (remaining <= 1) {
            break;
        }

        int written = cmd_format_entry(&results->entries[i],
                                       buf + total, remaining);
        total += written;

        /* Add newline after each entry except the last */
        if (i < results->count - 1 && total < buf_size - 1) {
            buf[total] = '\n';
            total++;
        }
    }

    if (total < buf_size) {
        buf[total] = '\0';
    }
    return total;
}
