/* Command palette data module — searchable registry of all application commands.
 * Data backend for the VS Code/Figma-style command palette UI (triggered by /).
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_COMMAND_PALETTE_H
#define TIME_COMMAND_PALETTE_H

#define CMD_MAX_RESULTS 64
#define CMD_NAME_MAX    48
#define CMD_DESC_MAX    96

/* Command categories */
typedef enum {
    CMD_CAT_TIME = 0,      /* Time controls: pause, speed, reverse */
    CMD_CAT_CAMERA,        /* Camera: zoom, scale jumps */
    CMD_CAT_LAYER,         /* Layer toggles: stars, orbits, ring */
    CMD_CAT_VIEW,          /* View modes: earth, overview */
    CMD_CAT_SYSTEM,        /* System focus: astrology, tzolkin, etc */
    CMD_CAT_NAVIGATE,      /* Navigation: date jump, location */
    CMD_CAT_UI,            /* UI: help, palette, theme */
    CMD_CAT_COUNT
} cmd_category_t;

/* A single command entry */
typedef struct {
    const char *name;           /* Display name: "Pause", "Toggle Stars" */
    const char *shortcut;       /* Keyboard shortcut: "Space", "S" (NULL if none) */
    const char *description;    /* What it does: "Pause or resume time flow" */
    cmd_category_t category;    /* Category for grouping */
} cmd_entry_t;

/* Search results container */
typedef struct {
    cmd_entry_t entries[CMD_MAX_RESULTS];
    int count;
} cmd_result_t;

/* Get total number of registered commands. */
int cmd_palette_count(void);

/* Get all commands. Writes to out array, returns count written.
 * max: capacity of out array. */
int cmd_palette_all(cmd_entry_t *out, int max);

/* Search commands by query string (case-insensitive substring match).
 * Searches name, shortcut, and description fields.
 * Results ordered: name matches first, then description matches.
 * Empty query returns all commands. */
cmd_result_t cmd_palette_search(const char *query);

/* Get commands filtered by category.
 * Writes to out array, returns count written. */
int cmd_palette_by_category(cmd_category_t cat, cmd_entry_t *out, int max);

/* Count commands in a specific category. */
int cmd_palette_category_count(cmd_category_t cat);

/* Get the display name for a command category. Returns static string. */
const char *cmd_category_name(cmd_category_t cat);

/* Get the total number of categories. */
int cmd_category_total(void);

/* Find a command by exact name (case-sensitive).
 * Returns the entry, or entry with name=NULL if not found. */
cmd_entry_t cmd_palette_find(const char *name);

/* Find a command by keyboard shortcut.
 * Returns the entry, or entry with name=NULL if not found. */
cmd_entry_t cmd_palette_find_by_shortcut(const char *shortcut);

/* Format a command entry as display text.
 * Format: "Name              Shortcut   Description"
 * Returns chars written. */
int cmd_format_entry(const cmd_entry_t *entry, char *buf, int buf_size);

/* Format search results as text. Returns chars written. */
int cmd_format_results(const cmd_result_t *results, char *buf, int buf_size);

#endif /* TIME_COMMAND_PALETTE_H */
