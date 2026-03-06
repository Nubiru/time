/* sacred_numbers — cross-system sacred number registry.
 * Pure lookup module: static const data, no malloc, no globals, no side effects.
 * Maps numbers that recur across knowledge systems (3, 7, 12, 22, 64, 108, 260...). */

#ifndef TIME_SACRED_NUMBERS_H
#define TIME_SACRED_NUMBERS_H

#include <stdbool.h>

#define SACRED_NUMBER_MAX_SYSTEMS 12

typedef enum {
    SYSTEM_ASTRONOMY = 0,
    SYSTEM_ASTROLOGY,
    SYSTEM_TZOLKIN,
    SYSTEM_ICHING,
    SYSTEM_CHINESE,
    SYSTEM_HUMAN_DESIGN,
    SYSTEM_HEBREW,
    SYSTEM_ISLAMIC,
    SYSTEM_BUDDHIST,
    SYSTEM_HINDU,
    SYSTEM_KABBALAH,
    SYSTEM_GEOLOGY,
    SYSTEM_GREGORIAN,
    SYSTEM_MUSIC,
    SYSTEM_BIOLOGY,
    SYSTEM_MATHEMATICS,
    SYSTEM_COUNT
} knowledge_system_t;

typedef struct {
    knowledge_system_t system;
    const char *context;
} sacred_appearance_t;

typedef struct {
    int value;
    const char *name;
    const char *significance;
    int appearance_count;
    sacred_appearance_t appearances[SACRED_NUMBER_MAX_SYSTEMS];
} sacred_number_t;

/* Lookup a sacred number by its numeric value.
 * Returns entry with value=0 and name=NULL if not found. */
sacred_number_t sacred_number_get(int value);

/* Lookup a sacred number by its index in the sorted registry.
 * Returns entry with value=0 and name=NULL if index out of range. */
sacred_number_t sacred_number_by_index(int index);

/* Total number of sacred numbers in the registry. */
int sacred_number_count(void);

/* How many systems a given sacred number appears in.
 * Returns 0 if the value is not in the registry. */
int sacred_number_system_count(int value);

/* Check if a sacred number appears in a specific knowledge system. */
bool sacred_number_in_system(int value, knowledge_system_t system);

/* Human-readable name for a knowledge system enum value.
 * Returns "?" for invalid/out-of-range values. */
const char *sacred_number_system_name(knowledge_system_t system);

/* Find all sacred numbers that appear in a given system.
 * Writes indices into out_indices, up to max_results.
 * Returns the number of results written. */
int sacred_number_find_by_system(knowledge_system_t system, int *out_indices, int max_results);

/* Find factor relationship with sacred numbers.
 * If value is itself sacred, returns that value.
 * If value is an exact multiple of a sacred number, returns the smallest such sacred number.
 * If a sacred number is an exact multiple of value, returns that sacred number.
 * Returns 0 if no relationship found. */
int sacred_number_factor_of(int value);

#endif /* TIME_SACRED_NUMBERS_H */
