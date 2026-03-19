/* myanmar_interpret.h — Myanmar calendar interpretation data
 *
 * Archetype data for the 12 Myanmar months and the Thingyan
 * (water festival / New Year).  The Myanmar calendar is a
 * sophisticated lunisolar system with three year types
 * (common/small watat/big watat) and a 19-year Metonic-style
 * intercalation.  Thingyan is Southeast Asia's grandest water
 * celebration, spanning 4-5 days of ritual cleansing.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_MYANMAR_INTERPRET_H
#define TIME_MYANMAR_INTERPRET_H

/* Per-month interpretation (12 months) */
typedef struct {
    int month;                  /* 0-11 (Tagu through Tabaung) */
    const char *name;           /* Myanmar name */
    const char *season;         /* Seasonal quality */
    const char *festival;       /* Major festival or observance */
    const char *brief;          /* One-sentence meaning */
} mmi_month_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: month abbreviation */
    char glance[128];           /* T1: "Tagu 1 — Thingyan (Water Festival)" */
    char detail[512];           /* T3: Full narrative */
} myanmar_interp_t;

/* Return interpretation for a Myanmar month (0-11).
 * Invalid: returns struct with month=-1, all strings "?". */
mmi_month_t mmi_month_data(int month);

/* Compose tiered interpretation.
 * month: 0-11, day: 1-30, is_thingyan: 1 if during Thingyan.
 * year_type: 0=common, 1=small watat, 2=big watat.
 * Invalid month: "?" strings. */
myanmar_interp_t mmi_interpret(int month, int day, int is_thingyan,
                               int year_type);

int mmi_month_count(void);     /* Returns 12 */

#endif /* TIME_MYANMAR_INTERPRET_H */
