/* earth_fraction.c — Awe-inspiring fractions: human existence vs Earth/cosmos.
 * Pure functions: no GL, no malloc, no globals, no side effects.
 *
 * Constants:
 *   Earth age:           4,540,000,000 years (4.54 Ga, geological consensus)
 *   Homo sapiens:        ~300,000 years (earliest anatomically modern humans)
 *   Civilization:        ~5,000 years (Sumer, Egypt, Indus ~3000 BCE)
 *   Recorded history:    ~5,000 years (cuneiform tablets ~3100 BCE)
 *   Total humans born:   ~109 billion (Population Reference Bureau estimate)
 *   Current population:  ~8.1 billion (as of ~2024)
 *   Seconds per day:     86,400 */

#include "earth_fraction.h"

#include <stddef.h>

/* ===== Constants ===== */

#define EF_EARTH_AGE        4540000000.0   /* years */
#define EF_HUMAN_AGE        300000.0       /* years */
#define EF_CIVILIZATION_AGE 5000.0         /* years */
#define EF_RECORDED_HISTORY 5000.0         /* years */
#define EF_EVER_BORN        109000000000.0 /* people */
#define EF_CURRENT_POP      8100000000.0   /* people */
#define EF_SECONDS_PER_DAY  86400.0

/* ===== Derived constants ===== */

/* Human fraction of Earth's age as percentage */
#define EF_HUMAN_PCT    ((EF_HUMAN_AGE / EF_EARTH_AGE) * 100.0)

/* Civilization fraction as percentage */
#define EF_CIV_PCT      ((EF_CIVILIZATION_AGE / EF_EARTH_AGE) * 100.0)

/* Recorded history fraction as percentage */
#define EF_HISTORY_PCT  ((EF_RECORDED_HISTORY / EF_EARTH_AGE) * 100.0)

/* Percentage of all humans alive now */
#define EF_ALIVE_PCT    ((EF_CURRENT_POP / EF_EVER_BORN) * 100.0)

/* Seconds before midnight humans appear on a 24-hour clock */
#define EF_CLOCK_SEC    ((EF_HUMAN_AGE / EF_EARTH_AGE) * EF_SECONDS_PER_DAY)

/* Humans on a 1-meter ruler (in mm) */
#define EF_RULER_MM     ((EF_HUMAN_AGE / EF_EARTH_AGE) * 1000.0)

/* ===== Fact catalog ===== */

static const earth_fraction_t s_facts[] = {
    {
        "earth_age",
        "Earth is approximately 4.54 billion years old",
        EF_EARTH_AGE,
        "years",
        "If Earth's age were a book of 1000 pages, each page covers 4.54 million years"
    },
    {
        "human_age",
        "Homo sapiens have existed for approximately 300,000 years",
        EF_HUMAN_AGE,
        "years",
        "Modern humans are younger than many still-living tree species"
    },
    {
        "human_fraction",
        "Humans have existed for 0.0066% of Earth's history",
        EF_HUMAN_PCT,
        "%",
        "If Earth's history were 24 hours, humans appear at 23:59:56"
    },
    {
        "civilization_fraction",
        "Human civilization spans 0.00011% of Earth's history",
        EF_CIV_PCT,
        "%",
        "If Earth's history were a marathon, civilization is the last 2 millimeters"
    },
    {
        "recorded_history_fraction",
        "Recorded history covers 0.00011% of Earth's history",
        EF_HISTORY_PCT,
        "%",
        "Everything we have written down fits in Earth's last heartbeat"
    },
    {
        "alive_fraction",
        "About 7.4% of all humans who ever lived are alive right now",
        EF_ALIVE_PCT,
        "%",
        "For every person alive today, roughly 12 have lived and died before them"
    },
    {
        "total_ever_born",
        "An estimated 109 billion humans have ever been born",
        EF_EVER_BORN,
        "people",
        "If every human who ever lived held hands, the chain would reach the sun and back 70 times"
    },
    {
        "current_population",
        "Approximately 8.1 billion people are alive right now",
        EF_CURRENT_POP,
        "people",
        "More people are alive now than in all of human history before 1800"
    },
    {
        "clock_analogy",
        "On a 24-hour Earth clock, humans appear about 6 seconds before midnight",
        EF_CLOCK_SEC,
        "seconds",
        "If Earth's history were 24 hours, humans appear at 23:59:56"
    },
    {
        "ruler_analogy",
        "On a 1-meter ruler of Earth's history, humans occupy 0.066 mm",
        EF_RULER_MM,
        "mm",
        "The width of a human hair on a meter stick — that is all of human existence"
    }
};

#define EF_FACT_COUNT ((int)(sizeof(s_facts) / sizeof(s_facts[0])))

/* ===== Public functions ===== */

int ef_fact_count(void)
{
    return EF_FACT_COUNT;
}

earth_fraction_t ef_fact_get(int index)
{
    if (index < 0 || index >= EF_FACT_COUNT) {
        earth_fraction_t empty = {NULL, NULL, 0.0, NULL, NULL};
        return empty;
    }
    return s_facts[index];
}

double ef_human_fraction(void)
{
    return EF_HUMAN_PCT;
}

double ef_humans_ever_born(void)
{
    return EF_EVER_BORN;
}

double ef_current_population(void)
{
    return EF_CURRENT_POP;
}

double ef_alive_fraction(void)
{
    return EF_ALIVE_PCT;
}

double ef_earth_age_years(void)
{
    return EF_EARTH_AGE;
}

double ef_human_age_years(void)
{
    return EF_HUMAN_AGE;
}

double ef_clock_analogy_seconds(void)
{
    return EF_CLOCK_SEC;
}
