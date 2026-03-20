/* kabbalah_interpret.h — Kabbalah Tree of Life interpretation data
 *
 * Static interpretation data for 10 Sefirot and 22 paths.
 * Each Sefirah has a meditation focus, a contemplative question,
 * and a one-sentence essence. Each path has a journey description
 * and a lesson.
 *
 * All functions are pure: no globals, no malloc, no side effects.
 *
 * Sources: Traditional Kabbalistic meditations,
 *          Golden Dawn path attributions (Hebrew letter + Tarot). */

#ifndef TIME_KABBALAH_INTERPRET_H
#define TIME_KABBALAH_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-Sefirah interpretation */
typedef struct {
    int index;                  /* 0-9 (Keter to Malkuth) */
    const char *meditation;     /* contemplative focus */
    const char *question;       /* the question this Sefirah asks */
    const char *brief;          /* one-sentence essence */
} ki_sefirah_t;

/* Per-path interpretation */
typedef struct {
    int index;                  /* 0-21 */
    const char *journey;        /* what traveling this path means */
    const char *lesson;         /* what you learn on this path */
} ki_path_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: Sefirah name abbreviation */
    char glance[128];           /* T1: "{name} — {brief}" */
    char detail[512];           /* T3: Full meditation narrative */
} kabbalah_interp_t;

/* Get interpretation data for a Sefirah (0-9).
 * Invalid index: returns struct with index=-1, all strings "?". */
ki_sefirah_t ki_sefirah_data(int index);

/* Get interpretation data for a path (0-21).
 * Invalid index: returns struct with index=-1, all strings "?". */
ki_path_t ki_path_data(int index);

/* Compose tiered interpretation for a Sefirah.
 * Glyph: first 3 chars of Sefirah name.
 * Glance: "{name} — {brief}".
 * Detail: "{name}: {meditation}. The question: {question}. Planet: {planet_name}."
 * NULL planet_name uses "unassigned".
 * Invalid index: all fields "?". */
kabbalah_interp_t ki_interpret_sefirah(int index, const char *planet_name);

/* Count of Sefirot interpretations (always 10). */
int ki_sefirah_count(void);

/* Count of path interpretations (always 22). */
int ki_path_count(void);

/* Locale-aware Sefirah interpretation. Falls back to English. */
kabbalah_interp_t ki_interpret_sefirah_locale(int index, const char *planet_name,
                                              i18n_locale_t locale);

#endif /* TIME_KABBALAH_INTERPRET_H */
