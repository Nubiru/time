/* universal_card.h — Universal card text format (LANGUAGE L2.1)
 *
 * Unified card format for all 20 time/calendar systems.
 * Each card provides layered text: title, subtitle, essence,
 * detail, and source attribution. Cultural sensitivity tagged.
 *
 * Consumes ts_entry_t from today_summary. Produces ucard_t.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 *
 * Sources: today_summary.h (ts_entry_t), sensitivity.h (tags). */

#ifndef TIME_UNIVERSAL_CARD_H
#define TIME_UNIVERSAL_CARD_H

#include "today_summary.h"
#include "../../ui/sensitivity.h"
#include <string.h>
#include <stdio.h>

/* ===================================================================
 * Constants
 * =================================================================== */

#define UCARD_TITLE_MAX     64
#define UCARD_SUBTITLE_MAX  128
#define UCARD_ESSENCE_MAX   256
#define UCARD_DETAIL_MAX    512
#define UCARD_SOURCE_MAX    128

/* ===================================================================
 * Universal card — text for any system
 * =================================================================== */

typedef struct {
    char          title[UCARD_TITLE_MAX];       /* native/primary name */
    char          subtitle[UCARD_SUBTITLE_MAX]; /* transliterated + English */
    char          essence[UCARD_ESSENCE_MAX];   /* one sentence: soul of this card */
    char          detail[UCARD_DETAIL_MAX];     /* paragraph: what this means today */
    char          source[UCARD_SOURCE_MAX];     /* attribution */
    int           system_id;                    /* ts_system_t or -1 */
    sensitivity_t sensitivity;                  /* cultural sensitivity level */
    int           significance;                 /* 0-3 from entry */
} ucard_t;

/* ===================================================================
 * API
 * =================================================================== */

/* Produce a universal card from a today_summary entry.
 * Returns card with system_id=-1 and empty title for NULL/inactive entry. */
ucard_t ucard_for_system(const ts_entry_t *entry);

/* System display name for card header. "?" for invalid. */
const char *ucard_system_name(int system_id);

/* Format card as brief text (title + essence, ~1-2 lines).
 * Returns characters written. */
int ucard_format_brief(const ucard_t *card, char *buf, int buf_size);

/* Format card as full text (all fields, multi-line).
 * Returns characters written. */
int ucard_format_full(const ucard_t *card, char *buf, int buf_size);

#endif /* TIME_UNIVERSAL_CARD_H */
