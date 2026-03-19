/* cultural_stories.h -- Cultural Story Database.
 *
 * 36 stories spanning 12 cultures: Norse, Greek, Roman, Indian, Chinese,
 * Buddhist, Celtic, Egyptian, Mayan, Hebrew, Islamic, and Persian.
 * Each story is tagged by knowledge-system domain and category for
 * algorithmic retrieval.
 *
 * Supports: culture-based search (case-insensitive substring), system-based
 * filtering, category-based filtering, distinct culture counting, and
 * per-culture story counting.
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_CULTURAL_STORIES_H
#define TIME_CULTURAL_STORIES_H

#include "../../ui/sensitivity.h"

#define CS_MAX_TAGS 4
#define CS_MAX_RESULTS 16

typedef enum {
    CS_SYSTEM_GREGORIAN = 0,
    CS_SYSTEM_HEBREW,
    CS_SYSTEM_ISLAMIC,
    CS_SYSTEM_CHINESE,
    CS_SYSTEM_HINDU,
    CS_SYSTEM_BUDDHIST,
    CS_SYSTEM_MAYAN,
    CS_SYSTEM_COPTIC,
    CS_SYSTEM_ETHIOPIAN,
    CS_SYSTEM_ZOROASTRIAN,
    CS_SYSTEM_CELTIC,
    CS_SYSTEM_MYANMAR,
    CS_SYSTEM_EGYPTIAN,
    CS_SYSTEM_ASTROLOGY,
    CS_SYSTEM_ICHING,
    CS_SYSTEM_HUMAN_DESIGN,
    CS_SYSTEM_KABBALAH,
    CS_SYSTEM_GEOLOGY,
    CS_SYSTEM_ASTRONOMY,
    CS_SYSTEM_COUNT
} cs_system_t;

typedef enum {
    CS_CAT_ORIGIN_MYTH = 0,
    CS_CAT_TIME_PHILOSOPHY,
    CS_CAT_KEY_FIGURE,
    CS_CAT_GOLDEN_AGE,
    CS_CAT_CALENDAR_REFORM,
    CS_CAT_COSMIC_CYCLE,
    CS_CAT_COUNT
} cs_category_t;

typedef struct {
    int id;
    const char *culture;
    const char *title;
    const char *text;           /* 2-4 sentences */
    const char *era;
    cs_category_t category;
    cs_system_t related_systems[CS_MAX_TAGS];
    int system_count;
    sensitivity_t sensitivity;
} cs_story_t;

/* Total number of stories in the database. */
int cs_story_count(void);

/* Get story by index. Returns struct with id=-1 for invalid index. */
cs_story_t cs_story_get(int index);

/* Find stories by culture (case-insensitive substring match).
 * Writes matching indices to results[]. Returns count found
 * (up to max_results). */
int cs_by_culture(const char *culture, int *results, int max_results);

/* Find stories by related system. Writes matching indices to results[].
 * Returns count found (up to max_results). */
int cs_by_system(cs_system_t system, int *results, int max_results);

/* Find stories by category. Writes matching indices to results[].
 * Returns count found (up to max_results). */
int cs_by_category(cs_category_t category, int *results, int max_results);

/* Get human-readable name for a category enum value.
 * Returns "Unknown" for invalid values. */
const char *cs_category_name(cs_category_t category);

/* Get human-readable name for a system enum value.
 * Returns "Unknown" for invalid values. */
const char *cs_system_name(cs_system_t system);

/* Count distinct culture strings represented in the database. */
int cs_distinct_cultures(void);

/* Count stories for a given culture (case-insensitive substring match).
 * Returns 0 for NULL. */
int cs_culture_story_count(const char *culture);

/* Find stories by sensitivity level. Writes matching indices to results[].
 * Returns count found (up to max_results). */
int cs_by_sensitivity(sensitivity_t level, int *results, int max_results);

#endif /* TIME_CULTURAL_STORIES_H */
