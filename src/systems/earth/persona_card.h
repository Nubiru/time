/* persona_card.h — PERSONA data to card_content_t adapter.
 *
 * Adapts PERSONA data structs (today briefing, cosmic fingerprint,
 * author info, cycles, notifications, snapshots, birth entry) to
 * card_content_t for the card rendering pipeline.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_PERSONA_CARD_H
#define TIME_PERSONA_CARD_H

#include "../../ui/card_data.h"

/* Return an empty card (all fields zeroed). */
card_content_t pc_empty_card(void);

/* Format a "Today for You" briefing as a card.
 * greeting, headline, energy → card fields.
 * Returns empty card if any string is NULL (safe). */
card_content_t pc_today_card(const char *greeting, const char *headline,
                             const char *energy_name, int highlight_count,
                             int days_until_birthday, int show_birthday);

/* Format one system's identity entry as a card.
 * system_name: "Tzolkin", headline: "Yellow Rhythmic Warrior", etc. */
card_content_t pc_fingerprint_card(const char *system_name,
                                    const char *headline,
                                    const char *subtitle,
                                    int glyph_id);

/* Format the author card. */
card_content_t pc_author_card(const char *name, const char *title_str,
                               const char *statement, const char *tech_line,
                               int contributor_count);

/* Format a single cycle status as a card. */
card_content_t pc_cycle_card(const char *cycle_name, const char *cycle_value,
                              int days_remaining, double progress);

/* Format a notification as a card. */
card_content_t pc_notification_card(const char *category_name,
                                     const char *title_str,
                                     const char *body,
                                     const char *urgency_name,
                                     int days_until);

/* Format a moment snapshot as a compact card. */
card_content_t pc_snapshot_card(int year, int month, int day,
                                 const char *compact_summary,
                                 const char *note,
                                 int system_count);

/* Format birth entry progress as a card. */
card_content_t pc_birth_entry_card(const char *step_name,
                                    int year, int month, int day,
                                    int date_valid,
                                    int location_set, int location_skipped);

#endif /* TIME_PERSONA_CARD_H */
