/* persona_card.c — PERSONA data to card_content_t adapter.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "persona_card.h"

#include <stdio.h>
#include <string.h>

/* --- Helper: safe string copy into fixed buffer --- */

static void safe_str(char *dst, int dst_size, const char *src) {
    if (!src) {
        dst[0] = '\0';
        return;
    }
    snprintf(dst, (size_t)dst_size, "%s", src);
}

/* --- pc_empty_card --- */

card_content_t pc_empty_card(void) {
    card_content_t c;
    memset(&c, 0, sizeof(c));
    return c;
}

/* --- pc_today_card --- */

card_content_t pc_today_card(const char *greeting, const char *headline,
                             const char *energy_name, int highlight_count,
                             int days_until_birthday, int show_birthday) {
    card_content_t c = pc_empty_card();

    snprintf(c.title, sizeof(c.title), "Today for You");
    safe_str(c.line1, (int)sizeof(c.line1), greeting);
    safe_str(c.line2, (int)sizeof(c.line2), headline);

    /* line3: energy · N highlights */
    const char *en = energy_name ? energy_name : "";
    snprintf(c.line3, sizeof(c.line3), "%s \xc2\xb7 %d highlights",
             en, highlight_count);

    /* detail: birthday countdown or empty */
    if (show_birthday) {
        snprintf(c.detail, sizeof(c.detail), "Birthday in %d days",
                 days_until_birthday);
    }

    return c;
}

/* --- pc_fingerprint_card --- */

card_content_t pc_fingerprint_card(const char *system_name,
                                    const char *headline,
                                    const char *subtitle,
                                    int glyph_id) {
    card_content_t c = pc_empty_card();

    safe_str(c.title, (int)sizeof(c.title), system_name);
    safe_str(c.line1, (int)sizeof(c.line1), headline);
    safe_str(c.line2, (int)sizeof(c.line2), subtitle);
    /* line3 reserved for glyph rendering */

    if (glyph_id >= 0) {
        snprintf(c.detail, sizeof(c.detail), "Glyph %d", glyph_id);
    }

    return c;
}

/* --- pc_author_card --- */

card_content_t pc_author_card(const char *name, const char *title_str,
                               const char *statement, const char *tech_line,
                               int contributor_count) {
    card_content_t c = pc_empty_card();

    safe_str(c.title, (int)sizeof(c.title), name);
    safe_str(c.line1, (int)sizeof(c.line1), title_str);
    safe_str(c.line2, (int)sizeof(c.line2), tech_line);
    snprintf(c.line3, sizeof(c.line3), "Built with %d contributors",
             contributor_count);
    safe_str(c.detail, (int)sizeof(c.detail), statement);

    return c;
}

/* --- pc_cycle_card --- */

card_content_t pc_cycle_card(const char *cycle_name, const char *cycle_value,
                              int days_remaining, double progress) {
    card_content_t c = pc_empty_card();

    snprintf(c.title, sizeof(c.title), "Cycle Status");
    safe_str(c.line1, (int)sizeof(c.line1), cycle_name);
    safe_str(c.line2, (int)sizeof(c.line2), cycle_value);
    snprintf(c.line3, sizeof(c.line3), "%d days remaining", days_remaining);
    snprintf(c.detail, sizeof(c.detail), "Progress: %d%%",
             (int)(progress * 100.0));

    return c;
}

/* --- pc_notification_card --- */

card_content_t pc_notification_card(const char *category_name,
                                     const char *title_str,
                                     const char *body,
                                     const char *urgency_name,
                                     int days_until) {
    card_content_t c = pc_empty_card();

    safe_str(c.title, (int)sizeof(c.title), category_name);
    safe_str(c.line1, (int)sizeof(c.line1), title_str);
    safe_str(c.line2, (int)sizeof(c.line2), body);

    const char *urg = urgency_name ? urgency_name : "";
    snprintf(c.line3, sizeof(c.line3), "%s \xc2\xb7 in %d days",
             urg, days_until);
    /* detail left empty — notifications are brief */

    return c;
}

/* --- pc_snapshot_card --- */

card_content_t pc_snapshot_card(int year, int month, int day,
                                 const char *compact_summary,
                                 const char *note,
                                 int system_count) {
    card_content_t c = pc_empty_card();

    snprintf(c.title, sizeof(c.title), "Bookmarked Moment");
    snprintf(c.line1, sizeof(c.line1), "%04d-%02d-%02d", year, month, day);
    safe_str(c.line2, (int)sizeof(c.line2), compact_summary);
    snprintf(c.line3, sizeof(c.line3), "%d systems captured", system_count);

    if (note && note[0] != '\0') {
        safe_str(c.detail, (int)sizeof(c.detail), note);
    }

    return c;
}

/* --- pc_birth_entry_card --- */

card_content_t pc_birth_entry_card(const char *step_name,
                                    int year, int month, int day,
                                    int date_valid,
                                    int location_set, int location_skipped) {
    card_content_t c = pc_empty_card();

    snprintf(c.title, sizeof(c.title), "Birth Data");
    safe_str(c.line1, (int)sizeof(c.line1), step_name);

    if (date_valid) {
        snprintf(c.line2, sizeof(c.line2), "%04d-%02d-%02d", year, month, day);
    } else {
        snprintf(c.line2, sizeof(c.line2), "Enter your birth date");
    }

    if (location_set) {
        snprintf(c.line3, sizeof(c.line3), "Location set");
    } else if (location_skipped) {
        snprintf(c.line3, sizeof(c.line3), "Location skipped");
    } else {
        snprintf(c.line3, sizeof(c.line3), "Enter location");
    }
    /* detail left empty */

    return c;
}
