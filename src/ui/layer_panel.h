/* layer_panel.h — Structured layer/system data for the layer toggle panel UI.
 *
 * Organizes all toggleable visual layers and knowledge systems into
 * categorized groups. Pure data module — no GL, no malloc, no globals. */

#ifndef TIME_LAYER_PANEL_H
#define TIME_LAYER_PANEL_H

#define LPANEL_MAX_ITEMS_PER_GROUP 16
#define LPANEL_MAX_GROUPS          8

/* Layer panel categories — groups for the accordion UI */
typedef enum {
    LPANEL_CAT_ASTRONOMY = 0,  /* Stars, Galaxy, Orbits, Planets */
    LPANEL_CAT_ASTROLOGY,      /* Zodiac Ring, Aspects, Houses */
    LPANEL_CAT_CALENDAR,       /* Gregorian, Chinese, Hebrew, Islamic, Buddhist */
    LPANEL_CAT_SACRED,         /* Tzolkin, I Ching, Human Design, Kabbalah */
    LPANEL_CAT_EARTH,          /* Tides, Weather, Daylight, Geology */
    LPANEL_CAT_DISPLAY,        /* Labels, Cards, HUD */
    LPANEL_CAT_COUNT
} lpanel_category_t;

/* A single toggleable item in the layer panel */
typedef struct {
    const char *name;           /* Display name: "Stars", "Tzolkin" */
    const char *shortcut;       /* Keyboard shortcut: "S", "K" (NULL if none) */
    lpanel_category_t category; /* Which group this belongs to */
    int render_layer_id;        /* Maps to layer_id_t from render_layers.h (-1 if not a render layer) */
    int default_on;             /* 1 = enabled by default, 0 = disabled */
} lpanel_item_t;

/* A group of related items */
typedef struct {
    const char *name;           /* Category display name: "Astronomy" */
    const char *icon;           /* Category icon/emoji hint: can be NULL */
    lpanel_category_t category;
    lpanel_item_t items[LPANEL_MAX_ITEMS_PER_GROUP];
    int count;                  /* Number of items in this group */
} lpanel_group_t;

/* Get the display name for a category. Returns static string. */
const char *lpanel_category_name(lpanel_category_t cat);

/* Get a fully populated group for a category. */
lpanel_group_t lpanel_get_group(lpanel_category_t cat);

/* Get all items for a category. Returns count written. */
int lpanel_get_items(lpanel_category_t cat, lpanel_item_t *out, int max_items);

/* Count total items across ALL categories. */
int lpanel_total_items(void);

/* Number of categories (equals LPANEL_CAT_COUNT). */
int lpanel_category_count(void);

/* Find an item by name (case-sensitive). Returns the item, or an item with name=NULL if not found. */
lpanel_item_t lpanel_find_item(const char *name);

/* Find an item by keyboard shortcut. Returns the item, or an item with name=NULL if not found. */
lpanel_item_t lpanel_find_by_shortcut(const char *shortcut);

/* Count items that are default-on in a category. */
int lpanel_default_on_count(lpanel_category_t cat);

/* Format a group as text for display.
 * Format: "=== Category Name ===" header, then "  [x] Name    (Key)" per item.
 * Returns chars written. */
int lpanel_format_group(lpanel_category_t cat, char *buf, int buf_size);

/* Format ALL groups as complete panel text. Returns chars written. */
int lpanel_format_all(char *buf, int buf_size);

#endif /* TIME_LAYER_PANEL_H */
