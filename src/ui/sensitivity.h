/* sensitivity.h -- Cultural sensitivity levels for content modules.
 *
 * Shared enum used by wisdom, cultural_stories, and fun_facts to tag
 * content with appropriate handling context.
 *
 * Pure module: no globals, no malloc, no side effects. */

#ifndef TIME_SENSITIVITY_H
#define TIME_SENSITIVITY_H

typedef enum {
    SENSITIVITY_GENERAL = 0,   /* safe in any context */
    SENSITIVITY_RESPECTFUL,    /* needs attribution context */
    SENSITIVITY_SACRED,        /* source language required, framing needed */
    SENSITIVITY_COUNT
} sensitivity_t;

/* Get human-readable name for sensitivity level. */
const char *sensitivity_name(sensitivity_t level);

#endif /* TIME_SENSITIVITY_H */
