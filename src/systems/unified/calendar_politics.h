/* calendar_politics.h -- Pure data module documenting key historical
 * moments where political power shaped global timekeeping.
 *
 * Each decision records who decided, when, what changed, what natural
 * pattern was lost, and who benefited. 10 entries spanning 46 BCE to
 * 1930 CE, sorted chronologically.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_CALENDAR_POLITICS_H
#define TIME_CALENDAR_POLITICS_H

#define CP_MAX_DECISIONS 16
#define CP_MAX_DESC 256

/* Category of political calendar decision */
typedef enum {
    CP_CAT_RELIGIOUS = 0,
    CP_CAT_IMPERIAL,
    CP_CAT_COMMERCIAL,
    CP_CAT_SCIENTIFIC,
    CP_CAT_COUNT
} cp_category_t;

/* A political decision that shaped timekeeping */
typedef struct {
    const char *title;          /* short name */
    int year;                   /* year CE (negative for BCE) */
    const char *who;            /* decision maker */
    const char *what_changed;   /* what was altered */
    const char *what_lost;      /* what natural pattern was lost */
    const char *who_benefited;  /* who gained from the change */
    cp_category_t category;
} cp_decision_t;

/* Get number of registered decisions. */
int cp_decision_count(void);

/* Get decision by index (chronological order).
 * Returns struct with NULL title for invalid index. */
cp_decision_t cp_decision_get(int index);

/* Get category name string. */
const char *cp_category_name(cp_category_t category);

/* Count decisions in a specific category. */
int cp_decisions_by_category(cp_category_t category);

/* Find decision by title keyword (case-insensitive substring match).
 * Returns index or -1 if not found. */
int cp_find_by_keyword(const char *keyword);

/* Get decisions within a year range (inclusive).
 * Writes matching indices to out[], returns count. */
int cp_decisions_in_range(int year_start, int year_end,
                           int *out, int max_out);

/* Get the "lost nature" summary: how many decisions had natural
 * cycles overridden (what_lost is non-empty). */
int cp_natural_losses_count(void);

/* Arguelles quote: "Time is not money. Time is art." */
const char *cp_arguelles_quote(void);

#endif /* TIME_CALENDAR_POLITICS_H */
