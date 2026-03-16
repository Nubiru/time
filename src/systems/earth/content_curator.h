/* content_curator.h -- Content Curator for the introvert layer.
 *
 * Surfaces personalized content (wisdom quotes, fun facts, cultural stories)
 * based on user profile, current moment, and exploration history.
 * Implements relevance scoring (affinity * temporal * novelty), learning
 * progression tracking, and a "Today for you" digest of up to 5 curated
 * insights with type diversity.
 *
 * Track 49.2 — Earth Introvert Layer completion.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CONTENT_CURATOR_H
#define TIME_CONTENT_CURATOR_H

#include "interest_profile.h"
#include "personal_space.h"

#define CC_MAX_SUGGESTIONS 16
#define CC_DIGEST_SIZE 5
#define CC_MAX_SEEN 64
#define CC_TOTAL_SYSTEMS 16

/* Source index offsets — encode content type into the seen index.
 * Wisdom: 0..999, Fun Fact: 1000..1999, Cultural Story: 2000..2999. */
#define CC_SOURCE_OFFSET_WISDOM  0
#define CC_SOURCE_OFFSET_FACT    1000
#define CC_SOURCE_OFFSET_STORY   2000

/* Content types the curator can suggest */
typedef enum {
    CC_TYPE_WISDOM = 0,       /* wisdom quote */
    CC_TYPE_FUN_FACT,         /* fun fact */
    CC_TYPE_CULTURAL_STORY,   /* cultural story */
    CC_TYPE_SYSTEM_CARD,      /* knowledge system card (explore prompt) */
    CC_TYPE_COUNT
} cc_type_t;

/* A scored content suggestion */
typedef struct {
    cc_type_t type;
    int source_index;           /* type-encoded index (with offset) */
    double relevance;           /* 0.0-1.0 composite score */
    double affinity;            /* 0.0-1.0 system match */
    double temporal;            /* 0.0-1.0 time relevance */
    double novelty;             /* 0.0-1.0 (1.0 = never seen) */
} cc_suggestion_t;

/* Curator context — what to curate for */
typedef struct {
    const ip_profile_t *profile;       /* user interests (NULL = no personalization) */
    const ps_space_t *space;           /* personal space (NULL = new user) */
    double current_jd;                  /* current time */
    int active_system;                  /* currently focused system (-1 = none) */
    int scale_level;                    /* camera zoom level (0-6) */
    int seen_indices[CC_MAX_SEEN];      /* previously shown content indices */
    int seen_count;
} cc_context_t;

/* Daily digest result */
typedef struct {
    cc_suggestion_t items[CC_DIGEST_SIZE];
    int count;                          /* 0-5 items */
} cc_digest_t;

/* Create default context (no personalization). */
cc_context_t cc_context_default(double current_jd);

/* Mark a content item as seen (for novelty penalty). Returns modified context. */
cc_context_t cc_mark_seen(cc_context_t ctx, int source_index);

/* Score a wisdom quote for this context. */
double cc_score_wisdom(int quote_index, const cc_context_t *ctx);

/* Score a fun fact for this context. */
double cc_score_fact(int fact_index, const cc_context_t *ctx);

/* Generate top-N suggestions across all content types. Sorted by relevance. */
int cc_suggest(const cc_context_t *ctx, cc_suggestion_t *out, int max);

/* Generate "Today for you" digest (up to 5 best items, diverse types). */
cc_digest_t cc_digest(const cc_context_t *ctx);

/* Suggest next system to explore (one the user hasn't explored yet). */
int cc_suggest_system(const cc_context_t *ctx);

/* Type name string. */
const char *cc_type_name(cc_type_t type);

/* Check if an index has been seen in this context. */
int cc_is_seen(const cc_context_t *ctx, int source_index);

#endif /* TIME_CONTENT_CURATOR_H */
