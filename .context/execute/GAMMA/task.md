# Task: Calendar Politics

**Agent**: GAMMA
**Roadmap Reference**: Track 29.5 — "Patriarchal Decision (Agent B)"
**Date**: 2026-03-07
**Status**: CLAIMED

## Goal

Pure data module documenting key historical moments where political power shaped global timekeeping. Each decision recorded with who decided, when, what changed, what was lost, and who benefited. The question: is our calendar natural — or political?

## Files to Create

- `src/systems/unified/calendar_politics.h`
- `src/systems/unified/calendar_politics.c`
- `tests/systems/unified/test_calendar_politics.c`

## API

```c
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

/* Get decision by index (chronological order). */
cp_decision_t cp_decision_get(int index);

/* Get category name string. */
const char *cp_category_name(cp_category_t category);

/* Count decisions in a specific category. */
int cp_decisions_by_category(cp_category_t category);

/* Find decision by title keyword (case-insensitive substring match).
 * Returns index or -1 if not found. */
int cp_find_by_keyword(const char *keyword);

/* Get decisions within a year range.
 * Writes matching indices to out[], returns count. */
int cp_decisions_in_range(int year_start, int year_end,
                           int *out, int max_out);

/* Get the "lost nature" summary: how many natural cycles
 * were overridden by political decisions. */
int cp_natural_losses_count(void);

/* Arguelles quote: "Time is not money. Time is art." */
const char *cp_arguelles_quote(void);

#endif /* TIME_CALENDAR_POLITICS_H */
```

## Decision Data

| # | Year | Title | Who | Category |
|---|------|-------|-----|----------|
| 1 | -46 | Julian Calendar Reform | Julius Caesar | IMPERIAL |
| 2 | 321 | Constantine's Sabbath Shift | Emperor Constantine | RELIGIOUS |
| 3 | 325 | Council of Nicaea | Emperor Constantine / Bishops | RELIGIOUS |
| 4 | 525 | Anno Domini Epoch | Dionysius Exiguus | RELIGIOUS |
| 5 | 1582 | Gregorian Calendar Reform | Pope Gregory XIII | SCIENTIFIC |
| 6 | 1752 | British Calendar Act | British Parliament | IMPERIAL |
| 7 | 1883 | Railroad Standard Time | US/Canadian railroads | COMMERCIAL |
| 8 | 1884 | International Date Line | International Meridian Conference | COMMERCIAL |
| 9 | 1918 | Standard Time Act | US Congress | COMMERCIAL |
| 10 | 1930 | Soviet 5-Day Week | Joseph Stalin | IMPERIAL |

## DONE WHEN

- [ ] 10+ political decisions with full metadata
- [ ] cp_decision_get returns correct data for all entries
- [ ] Category filtering works
- [ ] Keyword search (case-insensitive substring)
- [ ] Year range queries
- [ ] Arguelles quote accessible
- [ ] >= 40 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- `#include <string.h>` for strstr, strlen
- `#include <ctype.h>` for tolower (case-insensitive search)
- Standalone module
- All data as `static const` arrays
