/* wisdom_engine.c -- Wisdom Correlation Engine.
 *
 * Scores and ranks wisdom quotes against a navigation context.
 * Deterministic scoring — no randomness, no heap, no globals.
 *
 * Scoring algorithm:
 *   base            +1 (every quote)
 *   view match      +3 if quote tags overlap view's preferred tags
 *   scale match     +2 if quote tags overlap scale's preferred tags
 *   system match    +1 per active system whose preferred tags overlap quote tags
 *   culture match   +2 if user seal maps to quote's culture
 *   convergence     +convergence_score if quote has UNITY or HARMONY tag */

#include "wisdom_engine.h"

#include <string.h>

/* ---------- weight constants ---------- */

static const int WEIGHT_BASE        = 1;
static const int WEIGHT_VIEW_MATCH  = 3;
static const int WEIGHT_SCALE_MATCH = 2;
static const int WEIGHT_SYSTEM_MATCH = 1;
static const int WEIGHT_CULTURE     = 2;
/* convergence weight = ctx->convergence_score (dynamic) */

/* ---------- view name table ---------- */

static const char *VIEW_NAMES[] = {
    "Space",
    "Earth",
    "Tzolkin",
    "I Ching",
    "Calendar",
    "Deep Time"
};

/* ---------- scale name table ---------- */

static const char *SCALE_NAMES[] = {
    "Human",
    "Historical",
    "Geological",
    "Cosmic"
};

/* ---------- view-to-tags mapping ---------- */

/* Each view has a set of preferred wisdom tags.
 * If a quote has ANY of these tags, view match fires (+3). */

static const wisdom_tag_t VIEW_TAGS[][4] = {
    /* WE_VIEW_SPACE    */ { WISDOM_TAG_ASTRONOMY, WISDOM_TAG_COSMOS, WISDOM_TAG_COUNT, WISDOM_TAG_COUNT },
    /* WE_VIEW_EARTH    */ { WISDOM_TAG_NATURE, WISDOM_TAG_EARTH, WISDOM_TAG_COUNT, WISDOM_TAG_COUNT },
    /* WE_VIEW_TZOLKIN  */ { WISDOM_TAG_CYCLES, WISDOM_TAG_CALENDAR, WISDOM_TAG_TIME, WISDOM_TAG_COUNT },
    /* WE_VIEW_ICHING   */ { WISDOM_TAG_HARMONY, WISDOM_TAG_CYCLES, WISDOM_TAG_UNITY, WISDOM_TAG_COUNT },
    /* WE_VIEW_CALENDAR */ { WISDOM_TAG_TIME, WISDOM_TAG_CALENDAR, WISDOM_TAG_POWER, WISDOM_TAG_COUNT },
    /* WE_VIEW_DEEP_TIME*/ { WISDOM_TAG_COSMOS, WISDOM_TAG_EARTH, WISDOM_TAG_TIME, WISDOM_TAG_COUNT },
};

static const int VIEW_TAGS_COLS = 4;

/* ---------- scale-to-tags mapping ---------- */

/* Each scale has preferred tags. If quote matches any, scale match fires (+2). */

static const wisdom_tag_t SCALE_TAGS[][3] = {
    /* WE_SCALE_HUMAN      */ { WISDOM_TAG_COUNT, WISDOM_TAG_COUNT, WISDOM_TAG_COUNT },
    /* WE_SCALE_HISTORICAL */ { WISDOM_TAG_TIME, WISDOM_TAG_CYCLES, WISDOM_TAG_CALENDAR },
    /* WE_SCALE_GEOLOGICAL */ { WISDOM_TAG_EARTH, WISDOM_TAG_NATURE, WISDOM_TAG_COSMOS },
    /* WE_SCALE_COSMIC     */ { WISDOM_TAG_COSMOS, WISDOM_TAG_ASTRONOMY, WISDOM_TAG_UNITY },
};

static const int SCALE_TAGS_COLS = 3;

/* ---------- tag-to-systems mapping ---------- */

/* Which knowledge systems correlate with each wisdom tag */
static const unsigned int TAG_SYSTEM_MAP[] = {
    /* WISDOM_TAG_ASTRONOMY    */ WE_SYS_ASTRONOMY | WE_SYS_ASTROLOGY,
    /* WISDOM_TAG_TIME         */ WE_SYS_GREGORIAN | WE_SYS_CHINESE | WE_SYS_HEBREW | WE_SYS_ISLAMIC,
    /* WISDOM_TAG_CYCLES       */ WE_SYS_TZOLKIN | WE_SYS_ICHING | WE_SYS_CHINESE | WE_SYS_HINDU,
    /* WISDOM_TAG_UNITY        */ WE_SYS_KABBALAH | WE_SYS_HUMAN_DESIGN | WE_SYS_BUDDHIST,
    /* WISDOM_TAG_MATHEMATICS  */ WE_SYS_ASTRONOMY | WE_SYS_ICHING | WE_SYS_KABBALAH,
    /* WISDOM_TAG_NATURE       */ WE_SYS_EARTH | WE_SYS_GEOLOGY | WE_SYS_HINDU,
    /* WISDOM_TAG_CONSCIOUSNESS*/ WE_SYS_BUDDHIST | WE_SYS_HUMAN_DESIGN | WE_SYS_KABBALAH,
    /* WISDOM_TAG_CALENDAR     */ WE_SYS_GREGORIAN | WE_SYS_HEBREW | WE_SYS_ISLAMIC | WE_SYS_CHINESE | WE_SYS_TZOLKIN,
    /* WISDOM_TAG_COSMOS       */ WE_SYS_ASTRONOMY | WE_SYS_ASTROLOGY | WE_SYS_GEOLOGY,
    /* WISDOM_TAG_EARTH        */ WE_SYS_EARTH | WE_SYS_GEOLOGY,
    /* WISDOM_TAG_HARMONY      */ WE_SYS_HUMAN_DESIGN | WE_SYS_ICHING | WE_SYS_HINDU,
    /* WISDOM_TAG_POWER        */ WE_SYS_HUMAN_DESIGN | WE_SYS_TZOLKIN | WE_SYS_ISLAMIC,
};

/* ---------- seal-to-culture mapping ---------- */

/* Seal ranges map to cultural traditions:
 *   0-3  -> "Mayan"
 *   4-7  -> "Eastern"
 *   8-11 -> "Greek"
 *   12-15 -> "Indian"
 *   16-19 -> "Islamic"
 */
static const char *seal_to_culture(int seal)
{
    if (seal < 0 || seal > 19) return NULL;
    if (seal <= 3)  return "Mayan";
    if (seal <= 7)  return "Eastern";
    if (seal <= 11) return "Greek";
    if (seal <= 15) return "Indian";
    return "Islamic";
}

/* ---------- helpers ---------- */

/* Case-insensitive substring match (portable C11). */
static int ci_contains(const char *haystack, const char *needle)
{
    if (haystack == NULL || needle == NULL) return 0;
    int hlen = (int)strlen(haystack);
    int nlen = (int)strlen(needle);
    if (nlen == 0) return 1;
    if (nlen > hlen) return 0;
    for (int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (int j = 0; j < nlen; j++) {
            char h = haystack[i + j];
            char n = needle[j];
            if (h >= 'A' && h <= 'Z') h = (char)(h + 32);
            if (n >= 'A' && n <= 'Z') n = (char)(n + 32);
            if (h != n) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

/* Check if a quote has a specific tag. */
static int quote_has_tag(const wisdom_t *q, wisdom_tag_t tag)
{
    for (int i = 0; i < q->tag_count; i++) {
        if (q->tags[i] == tag) return 1;
    }
    return 0;
}

/* Check if quote_index is in the seen array. */
static int is_seen(int quote_index, const int *seen, int seen_count)
{
    if (seen == NULL || seen_count <= 0) return 0;
    for (int i = 0; i < seen_count; i++) {
        if (seen[i] == quote_index) return 1;
    }
    return 0;
}

/* ---------- public API ---------- */

we_context_t we_context_default(void)
{
    we_context_t ctx;
    ctx.view = WE_VIEW_SPACE;
    ctx.scale = WE_SCALE_HUMAN;
    ctx.active_systems = 0;
    ctx.user_seal = -1;
    ctx.user_tone = 0;
    ctx.convergence_score = 0;
    return ctx;
}

const char *we_view_name(we_view_t view)
{
    if (view < 0 || view >= WE_VIEW_COUNT) return "Unknown";
    return VIEW_NAMES[view];
}

const char *we_scale_name(we_scale_t scale)
{
    if (scale < 0 || scale >= WE_SCALE_COUNT) return "Unknown";
    return SCALE_NAMES[scale];
}

unsigned int we_tag_to_systems(wisdom_tag_t tag)
{
    if (tag < 0 || tag >= WISDOM_TAG_COUNT) return 0;
    return TAG_SYSTEM_MAP[tag];
}

int we_system_to_tags(we_system_flag_t sys, wisdom_tag_t *out, int max_out)
{
    if (out == NULL || max_out <= 0) return 0;
    int count = 0;
    for (int t = 0; t < WISDOM_TAG_COUNT && count < max_out; t++) {
        if (TAG_SYSTEM_MAP[t] & (unsigned int)sys) {
            out[count++] = (wisdom_tag_t)t;
        }
    }
    return count;
}

int we_score_quote(int quote_index, const we_context_t *ctx)
{
    if (ctx == NULL) return 0;

    int total = wisdom_quote_count();
    if (quote_index < 0 || quote_index >= total) return 0;

    wisdom_t q = wisdom_quote_get(quote_index);
    if (q.id < 0) return 0;

    int score = WEIGHT_BASE;

    /* View match: +3 if any quote tag matches view's preferred tags */
    if (ctx->view >= 0 && ctx->view < WE_VIEW_COUNT) {
        int view_matched = 0;
        for (int vt = 0; vt < VIEW_TAGS_COLS && !view_matched; vt++) {
            wisdom_tag_t preferred = VIEW_TAGS[ctx->view][vt];
            if (preferred == WISDOM_TAG_COUNT) break;
            if (quote_has_tag(&q, preferred)) {
                view_matched = 1;
            }
        }
        if (view_matched) score += WEIGHT_VIEW_MATCH;
    }

    /* Scale match: +2 if any quote tag matches scale's preferred tags */
    if (ctx->scale >= 0 && ctx->scale < WE_SCALE_COUNT) {
        int scale_matched = 0;
        for (int st = 0; st < SCALE_TAGS_COLS && !scale_matched; st++) {
            wisdom_tag_t preferred = SCALE_TAGS[ctx->scale][st];
            if (preferred == WISDOM_TAG_COUNT) break;
            if (quote_has_tag(&q, preferred)) {
                scale_matched = 1;
            }
        }
        if (scale_matched) score += WEIGHT_SCALE_MATCH;
    }

    /* System match: +1 per active system whose preferred tags overlap */
    if (ctx->active_systems != 0) {
        /* For each active system flag, get its tags and check overlap */
        for (int bit = 0; bit < WE_MAX_ACTIVE_SYSTEMS; bit++) {
            unsigned int flag = (1u << bit);
            if (!(ctx->active_systems & flag)) continue;

            /* Get tags for this system */
            wisdom_tag_t sys_tags[WISDOM_TAG_COUNT];
            int ntags = we_system_to_tags((we_system_flag_t)flag,
                                          sys_tags, WISDOM_TAG_COUNT);
            int sys_matched = 0;
            for (int st = 0; st < ntags && !sys_matched; st++) {
                if (quote_has_tag(&q, sys_tags[st])) {
                    sys_matched = 1;
                }
            }
            if (sys_matched) score += WEIGHT_SYSTEM_MATCH;
        }
    }

    /* Culture match: +2 if user seal maps to quote's culture */
    if (ctx->user_seal >= 0 && ctx->user_seal <= 19) {
        const char *culture = seal_to_culture(ctx->user_seal);
        if (culture != NULL && ci_contains(q.culture, culture)) {
            score += WEIGHT_CULTURE;
        }
    }

    /* Convergence boost: +convergence_score if quote has UNITY or HARMONY */
    if (ctx->convergence_score > 0) {
        if (quote_has_tag(&q, WISDOM_TAG_UNITY) ||
            quote_has_tag(&q, WISDOM_TAG_HARMONY)) {
            score += ctx->convergence_score;
        }
    }

    return score;
}

we_selection_t we_select(const we_context_t *ctx,
                         const int *seen, int seen_count)
{
    we_selection_t sel;
    sel.count = 0;

    if (ctx == NULL) return sel;

    /* If seen is NULL, treat as no exclusion regardless of seen_count. */
    if (seen == NULL) seen_count = 0;

    int total = wisdom_quote_count();

    /* Score all non-seen quotes and keep top WE_MAX_RESULTS.
     * Use a simple insertion approach: maintain sorted results array. */
    for (int i = 0; i < total; i++) {
        if (is_seen(i, seen, seen_count)) continue;

        int s = we_score_quote(i, ctx);
        if (s <= 0) continue;

        we_result_t entry;
        entry.quote_index = i;
        entry.score = s;

        if (sel.count < WE_MAX_RESULTS) {
            /* Room in results — insert and sort down */
            sel.results[sel.count] = entry;
            sel.count++;
            /* Bubble into correct sorted position */
            for (int j = sel.count - 1; j > 0; j--) {
                if (sel.results[j].score > sel.results[j - 1].score) {
                    we_result_t tmp = sel.results[j];
                    sel.results[j] = sel.results[j - 1];
                    sel.results[j - 1] = tmp;
                } else {
                    break;
                }
            }
        } else if (s > sel.results[WE_MAX_RESULTS - 1].score) {
            /* Better than worst — replace and re-sort */
            sel.results[WE_MAX_RESULTS - 1] = entry;
            for (int j = WE_MAX_RESULTS - 1; j > 0; j--) {
                if (sel.results[j].score > sel.results[j - 1].score) {
                    we_result_t tmp = sel.results[j];
                    sel.results[j] = sel.results[j - 1];
                    sel.results[j - 1] = tmp;
                } else {
                    break;
                }
            }
        }
    }

    return sel;
}

int we_best_quote(const we_context_t *ctx,
                  const int *seen, int seen_count)
{
    if (ctx == NULL) return -1;

    we_selection_t sel = we_select(ctx, seen, seen_count);
    if (sel.count <= 0) return -1;

    return sel.results[0].quote_index;
}
