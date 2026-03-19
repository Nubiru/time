/* hindu_interpret.h -- Hindu tithi interpretation data
 *
 * Static interpretation for 30 tithis (lunar days): presiding deity,
 * quality, recommended activity, and brief meaning.
 * Composition function producing tiered interpretation text.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_HINDU_INTERPRET_H
#define TIME_HINDU_INTERPRET_H

#include "panchanga.h"

/* Per-tithi interpretation */
typedef struct {
    int number;                 /* 1-30 */
    const char *deity;          /* presiding deity */
    const char *quality;        /* "Auspicious", "Inauspicious", "Neutral", "Very Auspicious" */
    const char *activity;       /* recommended activity */
    const char *brief;          /* one-sentence meaning */
} hdi_tithi_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: "S3" for Shukla Tritiya, "K12" for Krishna Dvadashi */
    char glance[128];           /* T1: "{tithi_name} -- {deity} . {quality} . {activity}" */
    char detail[512];           /* T3: Full narrative */
} hindu_interp_t;

/* Return interpretation data for a tithi (1-30).
 * Invalid: returns struct with number=-1, all strings "?". */
hdi_tithi_t hdi_tithi_data(int tithi);

/* Compose tiered interpretation from tithi number + optional context.
 * nakshatra_name: if non-NULL, included in detail.
 * yoga_name: if non-NULL, included in detail. */
hindu_interp_t hdi_interpret(int tithi, const char *nakshatra_name,
                             const char *yoga_name);

/* Returns 30 (total number of tithis). */
int hdi_tithi_count(void);

#endif /* TIME_HINDU_INTERPRET_H */
