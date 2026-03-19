/* narrative.h — Narrative composer (LANGUAGE L1.1)
 *
 * Composes human-readable narrative paragraphs from brain intelligence
 * (via brain_language_bridge) + wisdom quotes + depth tier context.
 * Template-based: selects template by thread type, fills slots, adjusts
 * verbosity by depth level.
 *
 * Novelty tracking: records recently-used template indices so the
 * composer can vary output across a session.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 *
 * Sources: brain_language_bridge.h (template slots), wisdom.h (quotes),
 * depth_tier.h (detail levels). */

#ifndef TIME_NARRATIVE_H
#define TIME_NARRATIVE_H

#include "brain_language_bridge.h"
#include <string.h>
#include <stdio.h>

/* ===================================================================
 * Constants
 * =================================================================== */

#define NARR_BODY_MAX       512
#define NARR_WISDOM_MAX     256
#define NARR_PERSONAL_MAX   256
#define NARR_HEADLINE_MAX   128
#define NARR_MAX_SEEN        32
#define NARR_THREAD_COUNT     6

/* ===================================================================
 * Depth levels — how much text to produce
 * =================================================================== */

typedef enum {
    NARR_DEPTH_SURFACE = 0,  /* one line: headline only */
    NARR_DEPTH_CONTEXT = 1,  /* headline + one-sentence body */
    NARR_DEPTH_CYCLE   = 2,  /* full paragraph with cycle context */
    NARR_DEPTH_FULL    = 3,  /* multi-sentence: narrative + wisdom + personal */
    NARR_DEPTH_COUNT   = 4
} narr_depth_t;

/* ===================================================================
 * Composer state — tracks novelty across calls
 * =================================================================== */

typedef struct {
    int  seen_templates[NARR_MAX_SEEN]; /* template indices used */
    int  seen_count;
    int  compose_count;                 /* total compositions so far */
} narr_state_t;

/* ===================================================================
 * Input — everything the composer needs
 * =================================================================== */

typedef struct {
    const br_lang_context_t *brain_ctx;  /* from brain_language_bridge */
    const char  *wisdom_text;            /* quote text, NULL if none */
    const char  *wisdom_author;          /* author name, NULL if none */
    const char  *wisdom_work;            /* source work, NULL if general */
    int          wisdom_year;            /* year CE (negative for BCE), 0 if unknown */
    narr_depth_t depth;                  /* how much detail to produce */
    const char  *system_name;            /* primary system name, NULL if none */
} narr_input_t;

/* ===================================================================
 * Output — the composed narrative
 * =================================================================== */

typedef struct {
    char  headline[NARR_HEADLINE_MAX];   /* extracted/composed headline */
    char  body[NARR_BODY_MAX];           /* main narrative text */
    char  wisdom_text[NARR_WISDOM_MAX];  /* formatted wisdom + attribution */
    char  personal[NARR_PERSONAL_MAX];   /* personal note if present */
    int   has_wisdom;                    /* 1 if wisdom section filled */
    int   has_personal;                  /* 1 if personal section filled */
    int   template_index;               /* which template was used */
    int   thread_index;                 /* which thread type (0-5) */
} narr_output_t;

/* ===================================================================
 * API
 * =================================================================== */

/* Initialize composer state. Must be called before first compose. */
void narr_init(narr_state_t *state);

/* Compose a narrative from input.
 * state: mutable novelty tracker (updated on success).
 * input: all data needed for composition.
 * out: filled on success.
 * Returns 1 on success, 0 on failure (NULL args, missing brain_ctx). */
int narr_compose(narr_state_t *state,
                 const narr_input_t *input,
                 narr_output_t *out);

/* Get thread index from thread type string.
 * Returns 0-5 for known types, -1 for unknown/NULL. */
int narr_thread_index(const char *thread_type);

/* Get depth level name. "?" for invalid. */
const char *narr_depth_name(narr_depth_t depth);

/* Format wisdom attribution string.
 * "Author, Work (Year BCE/CE)" or "Author (Year)" or "Author".
 * Returns characters written (0 for NULL author). */
int narr_format_attribution(const char *author, const char *work,
                            int year, char *buf, int buf_size);

#endif /* TIME_NARRATIVE_H */
