/* buddhist_interpret.h -- Buddhist time interpretation data
 *
 * Static data for 5 Uposatha types, 4 kalpa phases, 12 Pali months.
 * Composition function producing tiered interpretation text.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_BUDDHIST_INTERPRET_H
#define TIME_BUDDHIST_INTERPRET_H

#include "buddhist.h"
#include "kalpa.h"
#include "../../ui/i18n.h"  /* i18n_locale_t */

/* Per-Uposatha-type interpretation */
typedef struct {
    uposatha_type_t type;       /* UPOSATHA_NONE through LAST_QUARTER */
    const char *name;           /* "Full Moon Observance" etc. */
    const char *practice;       /* What practitioners do */
    const char *meaning;        /* Spiritual significance */
} bi_uposatha_t;

/* Per-kalpa-phase interpretation */
typedef struct {
    kalpa_phase_t phase;        /* FORMATION through EMPTINESS */
    const char *name;           /* English name */
    const char *pali;           /* Pali/Sanskrit term */
    const char *description;    /* What happens in this phase */
    const char *meaning;        /* Spiritual significance */
} bi_kalpa_t;

/* Per-month interpretation */
typedef struct {
    int month;                  /* 1-12 */
    const char *pali_name;      /* Pali month name */
    const char *season;         /* Hot/Rainy/Cool */
    const char *observance;     /* Major observance or "" */
    const char *meaning;        /* Brief significance */
} bi_month_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[8];              /* T0: dharma wheel or phase symbol */
    char glance[128];           /* T1: "BE 2569 · Vesakha · Full Moon Uposatha" */
    char detail[512];           /* T3: Full narrative */
} buddhist_interp_t;

/* Return interpretation for the given Uposatha type (0-4).
 * Invalid type: returns struct with type=UPOSATHA_NONE variant. */
bi_uposatha_t bi_uposatha_data(uposatha_type_t type);

/* Return interpretation for kalpa phase (0-3).
 * Invalid: returns struct with phase value and name="?". */
bi_kalpa_t bi_kalpa_data(kalpa_phase_t phase);

/* Return interpretation for month (1-12).
 * Invalid: returns struct with month=-1. */
bi_month_t bi_month_data(int month);

/* Compose tiered interpretation from Buddhist time data.
 * Glyph: "BE" (Buddhist Era marker).
 * Glance: "BE {year} · {month_name} · {uposatha_name or season}".
 * Detail: narrative combining month meaning + uposatha practice + kalpa context. */
buddhist_interp_t bi_interpret(int be_year, int month,
                               uposatha_type_t uposatha,
                               kalpa_phase_t kalpa_phase);

/* Returns 5 (including NONE). */
int bi_uposatha_count(void);

/* Returns 4. */
int bi_kalpa_phase_count(void);

/* Returns 12. */
int bi_month_count(void);

/* Locale-aware interpretation. Same as bi_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
buddhist_interp_t bi_interpret_locale(int be_year, int month,
                                      uposatha_type_t uposatha,
                                      kalpa_phase_t kalpa_phase,
                                      i18n_locale_t locale);

#endif /* TIME_BUDDHIST_INTERPRET_H */
