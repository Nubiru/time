/* ui_html.c -- Generate HTML snippets from UI data modules.
 *
 * All output via snprintf to caller-provided buffers.
 * No malloc, no globals, no side effects. */

#include "ui_html.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Internal append helper                                              */
/* ------------------------------------------------------------------ */

/* Append formatted text to buf at position pos.
 * Returns new position. Never writes past buf_size - 1. */
static int append(char *buf, int buf_size, int pos, const char *fmt, ...)
{
    if (!buf || pos < 0 || pos >= buf_size - 1) {
        return pos;
    }

    va_list args;
    va_start(args, fmt);
    int remaining = buf_size - pos;
    int written = vsnprintf(buf + pos, (size_t)remaining, fmt, args);
    va_end(args);

    if (written < 0) {
        return pos;
    }
    if (written >= remaining) {
        /* Truncated -- position at last valid byte */
        return buf_size - 1;
    }
    return pos + written;
}

/* ------------------------------------------------------------------ */
/* Level name lookup for toasts                                        */
/* ------------------------------------------------------------------ */

static const char *toast_level_css(toast_level_t level)
{
    switch (level) {
    case TOAST_INFO:    return "info";
    case TOAST_SUCCESS: return "success";
    case TOAST_WARNING: return "warning";
    default:            return "info";
    }
}

/* ------------------------------------------------------------------ */
/* ui_html_help                                                        */
/* ------------------------------------------------------------------ */

int ui_html_help(char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) {
        return 0;
    }
    buf[0] = '\0';

    int pos = 0;
    int cat_count = help_category_count();

    for (int c = 0; c < cat_count; c++) {
        help_group_t group = help_get_group((help_category_t)c);

        pos = append(buf, buf_size, pos,
                     "<div class=\"help-group\">"
                     "<div class=\"help-group-title\">%s</div>",
                     group.name);

        for (int i = 0; i < group.count; i++) {
            pos = append(buf, buf_size, pos,
                         "<div class=\"help-entry\">"
                         "<span class=\"help-action\">%s</span>"
                         "<kbd>%s</kbd>"
                         "</div>",
                         group.entries[i].action,
                         group.entries[i].key);
        }

        pos = append(buf, buf_size, pos, "</div>");
    }

    return pos;
}

/* ------------------------------------------------------------------ */
/* ui_html_cmd_results                                                 */
/* ------------------------------------------------------------------ */

int ui_html_cmd_results(const cmd_result_t *results, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) {
        return 0;
    }
    buf[0] = '\0';

    if (!results || results->count <= 0) {
        return 0;
    }

    int pos = 0;

    for (int i = 0; i < results->count; i++) {
        const cmd_entry_t *entry = &results->entries[i];

        pos = append(buf, buf_size, pos,
                     "<div class=\"cmd-item\">"
                     "<span class=\"cmd-item-name\">%s</span>",
                     entry->name);

        if (entry->shortcut != NULL) {
            pos = append(buf, buf_size, pos,
                         "<kbd class=\"cmd-item-shortcut\">%s</kbd>",
                         entry->shortcut);
        }

        pos = append(buf, buf_size, pos, "</div>");
    }

    return pos;
}

/* ------------------------------------------------------------------ */
/* ui_html_layers                                                      */
/* ------------------------------------------------------------------ */

int ui_html_layers(unsigned int active_mask, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) {
        return 0;
    }
    buf[0] = '\0';

    int pos = 0;
    int cat_count = lpanel_category_count();
    int bit_index = 0;

    for (int c = 0; c < cat_count; c++) {
        lpanel_group_t group = lpanel_get_group((lpanel_category_t)c);

        pos = append(buf, buf_size, pos,
                     "<div class=\"layer-group\">"
                     "<div class=\"layer-group-title\">%s</div>",
                     group.name);

        for (int i = 0; i < group.count; i++) {
            int active = (active_mask >> (unsigned)bit_index) & 1u;
            const char *dot_class = active ? "layer-dot active" : "layer-dot";

            pos = append(buf, buf_size, pos,
                         "<div class=\"layer-item\">"
                         "<span class=\"%s\"></span>"
                         "<span class=\"layer-name\">%s</span>",
                         dot_class,
                         group.items[i].name);

            if (group.items[i].shortcut != NULL) {
                pos = append(buf, buf_size, pos,
                             "<kbd>%s</kbd>",
                             group.items[i].shortcut);
            }

            pos = append(buf, buf_size, pos, "</div>");
            bit_index++;
        }

        pos = append(buf, buf_size, pos, "</div>");
    }

    return pos;
}

/* ------------------------------------------------------------------ */
/* ui_html_toasts                                                      */
/* ------------------------------------------------------------------ */

int ui_html_toasts(const toast_queue_t *queue, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) {
        return 0;
    }
    buf[0] = '\0';

    if (!queue || queue->count <= 0) {
        return 0;
    }

    int pos = 0;

    for (int i = 0; i < queue->count; i++) {
        const toast_t *t = &queue->items[i];
        if (!t->active) {
            continue;
        }

        pos = append(buf, buf_size, pos,
                     "<div class=\"toast toast-%s\" style=\"opacity: %.2f\">"
                     "%s</div>",
                     toast_level_css(t->level),
                     (double)t->opacity,
                     t->text);
    }

    return pos;
}

/* ------------------------------------------------------------------ */
/* ui_html_time_bar                                                    */
/* ------------------------------------------------------------------ */

int ui_html_time_bar(int is_paused, const char *speed_str,
                      const char *date_str, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) {
        return 0;
    }
    buf[0] = '\0';

    if (!speed_str || !date_str) {
        return 0;
    }

    int pos = 0;

    /* Play/pause button with appropriate icon */
    if (is_paused) {
        pos = append(buf, buf_size, pos,
                     "<button class=\"tb-btn\">&#9654;</button>");
    } else {
        pos = append(buf, buf_size, pos,
                     "<button class=\"tb-btn\">&#9646;&#9646;</button>");
    }

    /* Speed display */
    pos = append(buf, buf_size, pos,
                 "<span class=\"text-secondary\">%s</span>",
                 speed_str);

    /* Date display */
    pos = append(buf, buf_size, pos,
                 "<span class=\"text-primary\">%s</span>",
                 date_str);

    /* Now button */
    pos = append(buf, buf_size, pos,
                 "<button class=\"tb-btn text-accent\">Now</button>");

    return pos;
}
