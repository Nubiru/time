/* content_curator.c -- Content Curator implementation.
 *
 * Relevance scoring: relevance = affinity * temporal * novelty.
 * Digest diversity: penalize duplicate types to spread across content kinds.
 *
 * System ID mapping (0-15):
 *   0=astronomy, 1=astrology, 2=tzolkin, 3=iching, 4=chinese,
 *   5=human_design, 6=gregorian, 7=hebrew, 8=islamic, 9=buddhist,
 *   10=hindu, 11=kabbalah, 12=geology, 13=earth, 14=unified, 15=reserved
 *
 * Pure module: no globals, no malloc, no side effects. */

#include "content_curator.h"

#include <string.h>

#include "../unified/wisdom.h"
#include "../unified/fun_facts.h"
#include "../unified/cultural_stories.h"

/* ---------- internal helpers ---------- */

/* Map a system_id (0-15) to a wisdom tag, or -1 if no direct mapping. */
static int system_to_wisdom_tag(int system_id)
{
    switch (system_id) {
    case 0:  return WISDOM_TAG_ASTRONOMY;    /* astronomy */
    case 7:  return WISDOM_TAG_CALENDAR;     /* hebrew */
    case 8:  return WISDOM_TAG_CALENDAR;     /* islamic */
    case 9:  return WISDOM_TAG_CONSCIOUSNESS;/* buddhist */
    case 10: return WISDOM_TAG_HARMONY;      /* hindu */
    case 12: return WISDOM_TAG_EARTH;        /* geology */
    case 13: return WISDOM_TAG_EARTH;        /* earth */
    case 14: return WISDOM_TAG_UNITY;        /* unified */
    default: return -1;
    }
}

/* Map a system_id (0-15) to a fun_facts bitmask bit. */
static unsigned int system_to_ff_bit(int system_id)
{
    /* fun_facts bitmask:
     * 0=astronomy, 1=astrology, 2=tzolkin, 3=iching, 4=chinese,
     * 5=human_design, 6=hebrew, 7=islamic, 8=buddhist, 9=hindu,
     * 10=kabbalah, 11=gregorian, 12=geology, 13=earth */
    if (system_id < 0 || system_id > 13) return 0;
    /* System IDs 0-5 map directly. 6=gregorian in our scheme but 6=hebrew in ff.
     * Our mapping: 6=gregorian, 7=hebrew, 8=islamic, etc.
     * FF mapping: 6=hebrew, 7=islamic, 8=buddhist, 9=hindu, 10=kabbalah, 11=gregorian, 12=geology, 13=earth
     * We need to remap. */
    static const int remap[] = {
        0,  /* 0: astronomy -> bit 0 */
        1,  /* 1: astrology -> bit 1 */
        2,  /* 2: tzolkin -> bit 2 */
        3,  /* 3: iching -> bit 3 */
        4,  /* 4: chinese -> bit 4 */
        5,  /* 5: human_design -> bit 5 */
        11, /* 6: gregorian -> bit 11 */
        6,  /* 7: hebrew -> bit 6 */
        7,  /* 8: islamic -> bit 7 */
        8,  /* 9: buddhist -> bit 8 */
        9,  /* 10: hindu -> bit 9 */
        10, /* 11: kabbalah -> bit 10 */
        12, /* 12: geology -> bit 12 */
        13, /* 13: earth -> bit 13 */
    };
    return 1u << remap[system_id];
}

/* Map a system_id (0-15) to a cs_system_t, or -1 if no direct mapping. */
static int system_to_cs_system(int system_id)
{
    switch (system_id) {
    case 0:  return CS_SYSTEM_ASTRONOMY;
    case 1:  return CS_SYSTEM_ASTROLOGY;
    case 2:  return CS_SYSTEM_MAYAN;
    case 3:  return CS_SYSTEM_ICHING;
    case 4:  return CS_SYSTEM_CHINESE;
    case 5:  return CS_SYSTEM_HUMAN_DESIGN;
    case 6:  return CS_SYSTEM_GREGORIAN;
    case 7:  return CS_SYSTEM_HEBREW;
    case 8:  return CS_SYSTEM_ISLAMIC;
    case 9:  return CS_SYSTEM_BUDDHIST;
    case 10: return CS_SYSTEM_HINDU;
    case 11: return CS_SYSTEM_KABBALAH;
    case 12: return CS_SYSTEM_GEOLOGY;
    case 13: return CS_SYSTEM_EGYPTIAN; /* earth->closest cultural mapping */
    default: return -1;
    }
}

/* Check if a wisdom quote relates to a given system_id. */
static int wisdom_relates_to_system(const wisdom_t *w, int system_id)
{
    int tag = system_to_wisdom_tag(system_id);
    if (tag < 0) return 0;
    for (int i = 0; i < w->tag_count; i++) {
        if ((int)w->tags[i] == tag) return 1;
    }
    return 0;
}

/* Check if a fun fact relates to a given system_id. */
static int fact_relates_to_system(const ff_fact_t *f, int system_id)
{
    unsigned int bit = system_to_ff_bit(system_id);
    return (f->system_mask & bit) ? 1 : 0;
}

/* Check if a cultural story relates to a given system_id. */
static int story_relates_to_system(const cs_story_t *s, int system_id)
{
    int cs_sys = system_to_cs_system(system_id);
    if (cs_sys < 0) return 0;
    for (int i = 0; i < s->system_count; i++) {
        if ((int)s->related_systems[i] == cs_sys) return 1;
    }
    return 0;
}

/* Compute affinity for a content item that relates to content_system_id.
 * Priority: active_system match > favorite > explored > default. */
static double compute_affinity(int content_system_id, const cc_context_t *ctx)
{
    if (content_system_id < 0) return 0.1;

    /* Active system match => highest affinity */
    if (ctx->active_system >= 0 && content_system_id == ctx->active_system) {
        return 1.0;
    }

    /* Check favorites and explored in personal space */
    if (ctx->space != NULL) {
        if (ps_is_favorite(ctx->space, content_system_id)) {
            return 0.5;
        }
        if (ps_is_explored(ctx->space, content_system_id)) {
            return 0.3;
        }
    }

    return 0.1;
}

/* Compute temporal relevance — 0.5 base (no seasonal logic yet). */
static double compute_temporal(void)
{
    return 0.5;
}

/* Compute novelty: 1.0 if not seen, 0.1 if seen. */
static double compute_novelty(int source_index, const cc_context_t *ctx)
{
    return cc_is_seen(ctx, source_index) ? 0.1 : 1.0;
}

/* Find which system_id a wisdom quote best maps to, or -1. */
static int wisdom_best_system(const wisdom_t *w)
{
    for (int sys = 0; sys < CC_TOTAL_SYSTEMS; sys++) {
        if (wisdom_relates_to_system(w, sys)) return sys;
    }
    return -1;
}

/* Find which system_id a fun fact best maps to, or -1. */
static int fact_best_system(const ff_fact_t *f)
{
    for (int sys = 0; sys < CC_TOTAL_SYSTEMS; sys++) {
        if (fact_relates_to_system(f, sys)) return sys;
    }
    return -1;
}

/* Find which system_id a cultural story best maps to, or -1. */
static int story_best_system(const cs_story_t *s)
{
    for (int sys = 0; sys < CC_TOTAL_SYSTEMS; sys++) {
        if (story_relates_to_system(s, sys)) return sys;
    }
    return -1;
}

/* Simple insertion sort for suggestions descending by relevance. */
static void sort_suggestions(cc_suggestion_t *arr, int n)
{
    for (int i = 1; i < n; i++) {
        cc_suggestion_t key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].relevance < key.relevance) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* ---------- public API ---------- */

cc_context_t cc_context_default(double current_jd)
{
    cc_context_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.current_jd = current_jd;
    ctx.active_system = -1;
    ctx.scale_level = 0;
    ctx.profile = NULL;
    ctx.space = NULL;
    ctx.seen_count = 0;
    return ctx;
}

cc_context_t cc_mark_seen(cc_context_t ctx, int source_index)
{
    if (ctx.seen_count >= CC_MAX_SEEN) return ctx;
    ctx.seen_indices[ctx.seen_count] = source_index;
    ctx.seen_count++;
    return ctx;
}

int cc_is_seen(const cc_context_t *ctx, int source_index)
{
    if (ctx == NULL) return 0;
    for (int i = 0; i < ctx->seen_count; i++) {
        if (ctx->seen_indices[i] == source_index) return 1;
    }
    return 0;
}

double cc_score_wisdom(int quote_index, const cc_context_t *ctx)
{
    if (ctx == NULL) return 0.0;
    wisdom_t w = wisdom_quote_get(quote_index);
    if (w.id < 0) return 0.0;

    int sys = wisdom_best_system(&w);

    /* If active_system is set and this quote relates to it, use active */
    int content_sys = -1;
    if (ctx->active_system >= 0 && wisdom_relates_to_system(&w, ctx->active_system)) {
        content_sys = ctx->active_system;
    } else {
        content_sys = sys;
    }

    double aff = compute_affinity(content_sys, ctx);
    double temp = compute_temporal();
    int src_idx = CC_SOURCE_OFFSET_WISDOM + quote_index;
    double nov = compute_novelty(src_idx, ctx);

    return aff * temp * nov;
}

double cc_score_fact(int fact_index, const cc_context_t *ctx)
{
    if (ctx == NULL) return 0.0;
    ff_fact_t f = ff_fact_get(fact_index);
    if (f.id < 0) return 0.0;

    int content_sys = -1;
    if (ctx->active_system >= 0 && fact_relates_to_system(&f, ctx->active_system)) {
        content_sys = ctx->active_system;
    } else {
        content_sys = fact_best_system(&f);
    }

    double aff = compute_affinity(content_sys, ctx);
    double temp = compute_temporal();
    int src_idx = CC_SOURCE_OFFSET_FACT + fact_index;
    double nov = compute_novelty(src_idx, ctx);

    return aff * temp * nov;
}

/* Score a cultural story for this context (internal helper). */
static double score_story(int story_index, const cc_context_t *ctx)
{
    if (ctx == NULL) return 0.0;
    cs_story_t s = cs_story_get(story_index);
    if (s.id < 0) return 0.0;

    int content_sys = -1;
    if (ctx->active_system >= 0 && story_relates_to_system(&s, ctx->active_system)) {
        content_sys = ctx->active_system;
    } else {
        content_sys = story_best_system(&s);
    }

    double aff = compute_affinity(content_sys, ctx);
    double temp = compute_temporal();
    int src_idx = CC_SOURCE_OFFSET_STORY + story_index;
    double nov = compute_novelty(src_idx, ctx);

    return aff * temp * nov;
}

int cc_suggest(const cc_context_t *ctx, cc_suggestion_t *out, int max)
{
    if (ctx == NULL || out == NULL || max <= 0) return 0;

    /* Collect candidates from all sources into a temporary pool.
     * Each content type gets a fair share of pool slots to ensure diversity. */
    int pool_capacity = CC_MAX_SUGGESTIONS * 4; /* 64 total */
    int per_type = pool_capacity / 3;           /* ~21 per type */
    cc_suggestion_t pool[CC_MAX_SUGGESTIONS * 4];
    int pool_count = 0;

    /* Score wisdom quotes (up to per_type slots) */
    int wcount = wisdom_quote_count();
    int w_added = 0;
    for (int i = 0; i < wcount && w_added < per_type; i++) {
        double score = cc_score_wisdom(i, ctx);
        if (score > 0.0) {
            pool[pool_count].type = CC_TYPE_WISDOM;
            pool[pool_count].source_index = CC_SOURCE_OFFSET_WISDOM + i;
            pool[pool_count].relevance = score;
            pool[pool_count].affinity = 0.0;
            pool[pool_count].temporal = compute_temporal();
            pool[pool_count].novelty = compute_novelty(CC_SOURCE_OFFSET_WISDOM + i, ctx);
            pool_count++;
            w_added++;
        }
    }

    /* Score fun facts (up to per_type slots) */
    int fcount = ff_fact_count();
    int f_added = 0;
    for (int i = 0; i < fcount && f_added < per_type; i++) {
        double score = cc_score_fact(i, ctx);
        if (score > 0.0) {
            pool[pool_count].type = CC_TYPE_FUN_FACT;
            pool[pool_count].source_index = CC_SOURCE_OFFSET_FACT + i;
            pool[pool_count].relevance = score;
            pool[pool_count].affinity = 0.0;
            pool[pool_count].temporal = compute_temporal();
            pool[pool_count].novelty = compute_novelty(CC_SOURCE_OFFSET_FACT + i, ctx);
            pool_count++;
            f_added++;
        }
    }

    /* Score cultural stories (up to per_type slots) */
    int scount = cs_story_count();
    int s_added = 0;
    for (int i = 0; i < scount && s_added < per_type; i++) {
        double score = score_story(i, ctx);
        if (score > 0.0) {
            pool[pool_count].type = CC_TYPE_CULTURAL_STORY;
            pool[pool_count].source_index = CC_SOURCE_OFFSET_STORY + i;
            pool[pool_count].relevance = score;
            pool[pool_count].affinity = 0.0;
            pool[pool_count].temporal = compute_temporal();
            pool[pool_count].novelty = compute_novelty(CC_SOURCE_OFFSET_STORY + i, ctx);
            pool_count++;
            s_added++;
        }
    }

    sort_suggestions(pool, pool_count);

    int result = pool_count < max ? pool_count : max;
    for (int i = 0; i < result; i++) {
        out[i] = pool[i];
    }
    return result;
}

cc_digest_t cc_digest(const cc_context_t *ctx)
{
    cc_digest_t d;
    memset(&d, 0, sizeof(d));
    if (ctx == NULL) return d;

    /* Build one top candidate per content type, then pick diversely.
     * This guarantees type diversity even when all scores are equal. */
    cc_suggestion_t best_per_type[CC_TYPE_COUNT];
    int has_type[CC_TYPE_COUNT] = {0};

    /* Best wisdom */
    int wcount = wisdom_quote_count();
    double best_w = -1.0;
    for (int i = 0; i < wcount; i++) {
        double s = cc_score_wisdom(i, ctx);
        if (s > best_w) {
            best_w = s;
            best_per_type[CC_TYPE_WISDOM].type = CC_TYPE_WISDOM;
            best_per_type[CC_TYPE_WISDOM].source_index = CC_SOURCE_OFFSET_WISDOM + i;
            best_per_type[CC_TYPE_WISDOM].relevance = s;
            best_per_type[CC_TYPE_WISDOM].affinity = 0.0;
            best_per_type[CC_TYPE_WISDOM].temporal = compute_temporal();
            best_per_type[CC_TYPE_WISDOM].novelty = compute_novelty(
                CC_SOURCE_OFFSET_WISDOM + i, ctx);
            has_type[CC_TYPE_WISDOM] = 1;
        }
    }

    /* Best fun fact */
    int fcount = ff_fact_count();
    double best_f = -1.0;
    for (int i = 0; i < fcount; i++) {
        double s = cc_score_fact(i, ctx);
        if (s > best_f) {
            best_f = s;
            best_per_type[CC_TYPE_FUN_FACT].type = CC_TYPE_FUN_FACT;
            best_per_type[CC_TYPE_FUN_FACT].source_index = CC_SOURCE_OFFSET_FACT + i;
            best_per_type[CC_TYPE_FUN_FACT].relevance = s;
            best_per_type[CC_TYPE_FUN_FACT].affinity = 0.0;
            best_per_type[CC_TYPE_FUN_FACT].temporal = compute_temporal();
            best_per_type[CC_TYPE_FUN_FACT].novelty = compute_novelty(
                CC_SOURCE_OFFSET_FACT + i, ctx);
            has_type[CC_TYPE_FUN_FACT] = 1;
        }
    }

    /* Best cultural story */
    int scount = cs_story_count();
    double best_s = -1.0;
    for (int i = 0; i < scount; i++) {
        double s = score_story(i, ctx);
        if (s > best_s) {
            best_s = s;
            best_per_type[CC_TYPE_CULTURAL_STORY].type = CC_TYPE_CULTURAL_STORY;
            best_per_type[CC_TYPE_CULTURAL_STORY].source_index = CC_SOURCE_OFFSET_STORY + i;
            best_per_type[CC_TYPE_CULTURAL_STORY].relevance = s;
            best_per_type[CC_TYPE_CULTURAL_STORY].affinity = 0.0;
            best_per_type[CC_TYPE_CULTURAL_STORY].temporal = compute_temporal();
            best_per_type[CC_TYPE_CULTURAL_STORY].novelty = compute_novelty(
                CC_SOURCE_OFFSET_STORY + i, ctx);
            has_type[CC_TYPE_CULTURAL_STORY] = 1;
        }
    }

    /* Pick one of each available type first (diversity guarantee) */
    for (int t = 0; t < CC_TYPE_COUNT && d.count < CC_DIGEST_SIZE; t++) {
        if (has_type[t] && best_per_type[t].relevance > 0.0) {
            d.items[d.count] = best_per_type[t];
            d.count++;
        }
    }

    /* Fill remaining slots from cc_suggest pool (top suggestions overall) */
    if (d.count < CC_DIGEST_SIZE) {
        cc_suggestion_t extras[CC_MAX_SUGGESTIONS];
        int nextra = cc_suggest(ctx, extras, CC_MAX_SUGGESTIONS);
        for (int i = 0; i < nextra && d.count < CC_DIGEST_SIZE; i++) {
            /* Skip if we already have this exact source_index */
            int dup = 0;
            for (int j = 0; j < d.count; j++) {
                if (d.items[j].source_index == extras[i].source_index) {
                    dup = 1;
                    break;
                }
            }
            if (!dup) {
                d.items[d.count] = extras[i];
                d.count++;
            }
        }
    }

    /* Sort by relevance */
    sort_suggestions(d.items, d.count);

    return d;
}

int cc_suggest_system(const cc_context_t *ctx)
{
    if (ctx == NULL) return -1;

    /* If no space, suggest system 0 */
    if (ctx->space == NULL) return 0;

    /* Find first unexplored system */
    for (int sys = 0; sys < CC_TOTAL_SYSTEMS; sys++) {
        if (!ps_is_explored(ctx->space, sys)) {
            return sys;
        }
    }

    return -1; /* all explored */
}

const char *cc_type_name(cc_type_t type)
{
    switch (type) {
    case CC_TYPE_WISDOM:         return "Wisdom";
    case CC_TYPE_FUN_FACT:       return "Fun Fact";
    case CC_TYPE_CULTURAL_STORY: return "Cultural Story";
    case CC_TYPE_SYSTEM_CARD:    return "System Card";
    default:                     return "Unknown";
    }
}
