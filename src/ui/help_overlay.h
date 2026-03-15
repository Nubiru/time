#ifndef TIME_HELP_OVERLAY_H
#define TIME_HELP_OVERLAY_H

/* Categorized keyboard shortcut data for the help overlay system.
 * Pure module: all output via snprintf to caller-provided buffers.
 * No allocation, no globals, no side effects. */

/* Maximum entries per category and formatting limits */
#define HELP_MAX_ENTRIES_PER_CAT 16
#define HELP_MAX_CATEGORIES      8
#define HELP_FORMAT_BUF_SIZE     4096

/* Help categories — groups of related keyboard shortcuts */
typedef enum {
    HELP_CAT_TIME = 0,     /* Time controls: speed, pause, reverse */
    HELP_CAT_CAMERA,       /* Camera/zoom: scroll, drag, scale jumps */
    HELP_CAT_LAYERS,       /* Layer toggles: T, H, S, G, L */
    HELP_CAT_VIEWS,        /* View modes: E, Escape */
    HELP_CAT_SYSTEMS,      /* System focus: A, K, I, C, D */
    HELP_CAT_UI,           /* UI controls: ?, / */
    HELP_CAT_COUNT
} help_category_t;

/* A single keyboard shortcut entry */
typedef struct {
    const char *key;          /* Display string: "Space", "Shift+3", "?" */
    const char *action;       /* What it does: "Toggle pause/resume" */
    help_category_t category; /* Which group it belongs to */
} help_entry_t;

/* A group of related shortcuts */
typedef struct {
    const char *name;         /* Category display name: "Time Controls" */
    const char *description;  /* Brief description of the category */
    help_entry_t entries[HELP_MAX_ENTRIES_PER_CAT];
    int count;                /* Number of entries in this group */
} help_group_t;

/* Get the display name for a help category. Returns static string. */
const char *help_category_name(help_category_t cat);

/* Get the description for a help category. Returns static string. */
const char *help_category_desc(help_category_t cat);

/* Get all entries for a specific category.
 * Writes entries to out array, returns count written.
 * max_entries: capacity of out array. */
int help_get_entries(help_category_t cat, help_entry_t *out, int max_entries);

/* Get a fully populated help group for a category. */
help_group_t help_get_group(help_category_t cat);

/* Count total entries across ALL categories. */
int help_total_entries(void);

/* Format a single entry as aligned text.
 * Format: "  Key          Action"
 * Returns chars written (excluding null). */
int help_format_entry(const help_entry_t *entry, char *buf, int buf_size);

/* Format an entire category group as text block.
 * Includes header line + all entries.
 * Returns chars written. */
int help_format_group(help_category_t cat, char *buf, int buf_size);

/* Format ALL categories as complete help text.
 * Categories separated by blank lines.
 * Returns chars written. */
int help_format_all(char *buf, int buf_size);

/* Number of categories (equals HELP_CAT_COUNT). */
int help_category_count(void);

#endif /* TIME_HELP_OVERLAY_H */
