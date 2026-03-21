/* wisdom.h -- Curated wisdom quotes from great minds across civilizations.
 *
 * 192 quotes spanning Greek, Eastern, Islamic, Indian, Mayan/Indigenous,
 * Hebrew, Modern Science, and Other traditions. Each quote is tagged by
 * knowledge-system domain for algorithmic retrieval.
 *
 * Supports: tag-based filtering, culture-based search, author search,
 * culture counting, and distinct culture enumeration.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_WISDOM_H
#define TIME_WISDOM_H

#include "../../ui/sensitivity.h"

#define WISDOM_MAX_TAGS 6
#define WISDOM_MAX_RESULTS 32

/* Knowledge system tags for quote correlation */
typedef enum {
    WISDOM_TAG_ASTRONOMY = 0,
    WISDOM_TAG_TIME,
    WISDOM_TAG_CYCLES,
    WISDOM_TAG_UNITY,
    WISDOM_TAG_MATHEMATICS,
    WISDOM_TAG_NATURE,
    WISDOM_TAG_CONSCIOUSNESS,
    WISDOM_TAG_CALENDAR,
    WISDOM_TAG_COSMOS,
    WISDOM_TAG_EARTH,
    WISDOM_TAG_HARMONY,
    WISDOM_TAG_POWER,
    WISDOM_TAG_COUNT
} wisdom_tag_t;

/* A wisdom quote */
typedef struct {
    int id;
    const char *text;
    const char *author;
    const char *work;          /* source work, NULL if general attribution */
    int year;                  /* year CE (negative for BCE) */
    const char *culture;       /* "Greek", "Indian", "Islamic", "Mayan", etc. */
    wisdom_tag_t tags[WISDOM_MAX_TAGS];
    int tag_count;
    sensitivity_t sensitivity;
} wisdom_t;

/* Total number of quotes. */
int wisdom_quote_count(void);

/* Get quote by index. Returns struct with id=-1 for invalid. */
wisdom_t wisdom_quote_get(int index);

/* Find quotes by tag. Writes matching indices to results[].
 * Returns count found (up to max_results). */
int wisdom_by_tag(wisdom_tag_t tag, int *results, int max_results);

/* Find quotes by culture (case-insensitive substring match).
 * Writes matching indices to results[]. Returns count. */
int wisdom_by_culture(const char *culture, int *results, int max_results);

/* Find quotes by author (case-insensitive substring match).
 * Returns first matching index, or -1 if not found. */
int wisdom_by_author(const char *author);

/* Get tag name string. */
const char *wisdom_tag_name(wisdom_tag_t tag);

/* Count quotes for a given culture. */
int wisdom_culture_count(const char *culture);

/* Count distinct cultures represented. */
int wisdom_distinct_cultures(void);

/* Find quotes by sensitivity level. Writes matching indices to results[].
 * Returns count found (up to max_results). */
int wisdom_by_sensitivity(sensitivity_t level, int *results, int max_results);

#endif /* TIME_WISDOM_H */
