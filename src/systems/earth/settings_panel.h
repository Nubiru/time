/* settings_panel.h — Settings panel data layer.
 *
 * Produces display-ready data structures for the settings UI.
 * Given user_prefs, builds sections/options with labels, types,
 * ranges, and current values. Apply functions return updated prefs.
 *
 * PERSONA stream: Settings screen data model.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_SETTINGS_PANEL_H
#define TIME_SETTINGS_PANEL_H

#include "user_prefs.h"

#include <stdint.h>

/* --- constants --- */

#define SP_LABEL_MAX     48
#define SP_MAX_CHOICES   16
#define SP_CHOICE_MAX    32
#define SP_SECTION_MAX   32
#define SP_MAX_OPTIONS    8
#define SP_MAX_SECTIONS   6

/* --- enums --- */

/* Option types */
typedef enum {
    SP_OPT_TOGGLE = 0,   /* on/off */
    SP_OPT_CHOICE,        /* select from list */
    SP_OPT_SLIDER,        /* numeric range */
    SP_OPT_COUNT
} sp_option_type_t;

/* --- data structures --- */

/* A single setting option */
typedef struct {
    char label[SP_LABEL_MAX];
    sp_option_type_t type;
    /* TOGGLE fields */
    int toggle_value;
    /* CHOICE fields */
    char choices[SP_MAX_CHOICES][SP_CHOICE_MAX];
    int choice_count;
    int choice_selected;
    /* SLIDER fields */
    float slider_min;
    float slider_max;
    float slider_value;
    float slider_step;
} sp_option_t;

/* A section of settings */
typedef struct {
    char name[SP_SECTION_MAX];
    sp_option_t options[SP_MAX_OPTIONS];
    int option_count;
} sp_section_t;

/* Complete settings panel */
typedef struct {
    sp_section_t sections[SP_MAX_SECTIONS];
    int section_count;
    uint32_t collapsed_mask;  /* bitmask: bit N=1 means section N is collapsed */
} sp_panel_t;

/* --- functions --- */

/* Build panel from current preferences. */
sp_panel_t sp_build(const up_prefs_t *prefs);

/* Apply a toggle change. Returns updated prefs. */
up_prefs_t sp_apply_toggle(const up_prefs_t *prefs, int section, int option,
                           int value);

/* Apply a choice change. Returns updated prefs. */
up_prefs_t sp_apply_choice(const up_prefs_t *prefs, int section, int option,
                           int choice_index);

/* Apply a slider change. Returns updated prefs (value clamped to range). */
up_prefs_t sp_apply_slider(const up_prefs_t *prefs, int section, int option,
                           float value);

/* Get section count. */
int sp_section_count(const sp_panel_t *panel);

/* Get option count for a section. Returns 0 for invalid section. */
int sp_option_count(const sp_panel_t *panel, int section);

/* Get option label. Returns NULL for out-of-range indices. */
const char *sp_option_label(const sp_panel_t *panel, int section, int option);

/* --- Collapsible Sections (Tidwell: Collapsible Panels) ---
 * Bitmask tracks which sections are collapsed.
 * Persistable as a single uint32 via storage_bridge. */

/* Toggle collapsed state of a section. Returns updated panel. */
sp_panel_t sp_toggle_section(sp_panel_t panel, int section);

/* Check if a section is collapsed. Returns 1 if collapsed, 0 if expanded. */
int sp_is_collapsed(const sp_panel_t *panel, int section);

/* Get collapsed bitmask (for persistence). */
uint32_t sp_collapse_mask(const sp_panel_t *panel);

/* Set collapsed bitmask (from persisted state). Returns updated panel. */
sp_panel_t sp_set_collapse_mask(sp_panel_t panel, uint32_t mask);

/* Count of currently expanded (visible) sections. */
int sp_expanded_count(const sp_panel_t *panel);

#endif /* TIME_SETTINGS_PANEL_H */
