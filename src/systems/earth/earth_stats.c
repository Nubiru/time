/* earth_stats.c — Display-ready Earth/human statistics.
 * Pure functions: no GL, no malloc, no globals, no side effects.
 *
 * S91: Human existence as fraction of Earth's age, with visual analogies.
 * S92: Fraction of all humans who ever lived that are alive now.
 *
 * Constants from geological data (geo_time.h knowledge):
 *   Earth age:      4,540,000,000 years (4.54 Ga)
 *   Homo sapiens:   ~300,000 years (0.3 Ma)
 *
 * Population data from human_population.h:
 *   pop_alive_at_year()  — returns millions
 *   pop_ever_born_at_year() — returns billions */

#include "earth_stats.h"
#include "human_population.h"

#include <stdio.h>
#include <string.h>

/* ===== Constants ===== */

#define ES_EARTH_AGE_YEARS   4540000000.0
#define ES_HUMAN_AGE_YEARS   300000.0

/* ===== S91: Existence fraction ===== */

es_existence_t es_existence(void)
{
    es_existence_t ex;
    memset(&ex, 0, sizeof(ex));

    ex.earth_age_years = ES_EARTH_AGE_YEARS;
    ex.human_age_years = ES_HUMAN_AGE_YEARS;
    ex.fraction = ES_HUMAN_AGE_YEARS / ES_EARTH_AGE_YEARS;
    ex.percentage = ex.fraction * 100.0;

    /* Main text */
    snprintf(ex.fraction_text, ES_STAT_MAX,
             "Humans have existed for %.4f%% of Earth's history",
             ex.percentage);

    /* Ruler analogy: 30cm ruler, humans are last fraction*300mm */
    double ruler_mm = ex.fraction * 300.0;
    snprintf(ex.ruler_analogy, ES_ANALOGY_MAX,
             "If Earth's history were a 30cm ruler, humans are the last %.3fmm",
             ruler_mm);

    /* Day analogy: 24 hours = 86400 seconds */
    double day_seconds = ex.fraction * 86400.0;
    int rounded_seconds = (int)day_seconds;
    if (rounded_seconds < 1) {
        rounded_seconds = 1;
    }
    snprintf(ex.day_analogy, ES_ANALOGY_MAX,
             "If Earth's history were 24 hours, humans appeared %d seconds ago",
             rounded_seconds);

    /* Book analogy: 1000 pages, fraction*1000 < 1 page */
    snprintf(ex.book_analogy, ES_ANALOGY_MAX,
             "If Earth's history were a 1000-page book, humans appear in the last word");

    return ex;
}

/* ===== S92: Alive fraction ===== */

es_alive_t es_alive(int ce_year)
{
    es_alive_t al;
    memset(&al, 0, sizeof(al));

    double alive_millions = pop_alive_at_year(ce_year);
    double ever_born_billions = pop_ever_born_at_year(ce_year);

    al.alive_billions = alive_millions / 1000.0;
    al.ever_born_billions = ever_born_billions;

    /* Convert ever_born to millions for consistent division */
    double ever_born_millions = ever_born_billions * 1000.0;

    if (ever_born_millions > 0.0) {
        al.alive_fraction = alive_millions / ever_born_millions;
    }
    al.alive_percentage = al.alive_fraction * 100.0;

    /* Main text */
    snprintf(al.alive_text, ES_STAT_MAX,
             "%.1f%% of all humans who ever lived are alive right now",
             al.alive_percentage);

    /* Most populated moment */
    snprintf(al.moment_text, ES_STAT_MAX,
             "You are in the most populated moment in human history");

    /* Per-dead ratio: for every person alive, how many have died? */
    double alive_b = al.alive_billions;
    if (alive_b > 0.0) {
        double dead_billions = ever_born_billions - alive_b;
        int per_dead = (int)(dead_billions / alive_b);
        snprintf(al.per_dead_text, ES_STAT_MAX,
                 "For every person alive, ~%d have lived and died before",
                 per_dead);
    }

    return al;
}

/* ===== Combined format ===== */

int es_format(const es_existence_t *ex, const es_alive_t *al,
              char *buf, int buf_size)
{
    if (!ex || !al || !buf || buf_size <= 0) {
        return 0;
    }

    int pos = 0;
    int remaining;
    int written;

    /* S91 block */
    remaining = buf_size - pos;
    if (remaining <= 0) return pos;
    written = snprintf(buf + pos, (size_t)remaining, "%s\n", ex->fraction_text);
    if (written > 0 && written < remaining) {
        pos += written;
    } else if (written >= remaining) {
        return buf_size - 1;
    }

    remaining = buf_size - pos;
    if (remaining <= 0) return pos;
    written = snprintf(buf + pos, (size_t)remaining, "  %s\n", ex->ruler_analogy);
    if (written > 0 && written < remaining) {
        pos += written;
    } else if (written >= remaining) {
        return buf_size - 1;
    }

    remaining = buf_size - pos;
    if (remaining <= 0) return pos;
    written = snprintf(buf + pos, (size_t)remaining, "  %s\n", ex->day_analogy);
    if (written > 0 && written < remaining) {
        pos += written;
    } else if (written >= remaining) {
        return buf_size - 1;
    }

    remaining = buf_size - pos;
    if (remaining <= 0) return pos;
    written = snprintf(buf + pos, (size_t)remaining, "  %s\n", ex->book_analogy);
    if (written > 0 && written < remaining) {
        pos += written;
    } else if (written >= remaining) {
        return buf_size - 1;
    }

    /* S92 block */
    remaining = buf_size - pos;
    if (remaining <= 0) return pos;
    written = snprintf(buf + pos, (size_t)remaining, "%s\n", al->alive_text);
    if (written > 0 && written < remaining) {
        pos += written;
    } else if (written >= remaining) {
        return buf_size - 1;
    }

    remaining = buf_size - pos;
    if (remaining <= 0) return pos;
    written = snprintf(buf + pos, (size_t)remaining, "  %s\n", al->moment_text);
    if (written > 0 && written < remaining) {
        pos += written;
    } else if (written >= remaining) {
        return buf_size - 1;
    }

    remaining = buf_size - pos;
    if (remaining <= 0) return pos;
    written = snprintf(buf + pos, (size_t)remaining, "  %s\n", al->per_dead_text);
    if (written > 0 && written < remaining) {
        pos += written;
    } else if (written >= remaining) {
        return buf_size - 1;
    }

    return pos;
}
