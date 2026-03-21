#ifndef TIME_UI_HTML_H
#define TIME_UI_HTML_H

/* ui_html.h -- Generate HTML snippets from existing UI data modules.
 *
 * Produces DOM-ready HTML strings for help panel, command palette,
 * layer panel, toast notifications, and time bar. All output via
 * snprintf to caller-provided buffers.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "help_overlay.h"
#include "command_palette.h"
#include "layer_panel.h"
#include "toast_message.h"
#include "../systems/earth/settings_panel.h"

#define UI_HTML_BUF_SIZE 8192

/* Generate help panel HTML: categorized shortcut groups with CSS classes.
 * Returns chars written (excluding null). */
int ui_html_help(char *buf, int buf_size);

/* Generate command palette result items HTML.
 * Returns chars written. 0 if results empty. */
int ui_html_cmd_results(const cmd_result_t *results, char *buf, int buf_size);

/* Generate layer panel HTML: grouped items with toggle dots.
 * active_mask: bitmask of active layers (bit i = item i across all groups).
 * Returns chars written. */
int ui_html_layers(unsigned int active_mask, char *buf, int buf_size);

/* Generate toast notification divs with level CSS classes and opacity.
 * Returns chars written. 0 if queue empty. */
int ui_html_toasts(const toast_queue_t *queue, char *buf, int buf_size);

/* Generate time bar inner HTML with play/pause icon, speed, date, Now button.
 * Returns chars written. */
int ui_html_time_bar(int is_paused, const char *speed_str,
                      const char *date_str, char *buf, int buf_size);

/* Generate settings panel HTML: sections with toggles, choices, sliders.
 * Each interactive element has data-sec and data-opt attributes.
 * Returns chars written. */
int ui_html_settings(const sp_panel_t *panel, char *buf, int buf_size);

#endif /* TIME_UI_HTML_H */
