/* narrative.c — Narrative composer (LANGUAGE L1.1)
 *
 * Template-based narrative composition from brain intelligence data.
 * Selects template by thread type, fills slots, adjusts by depth. */

#include "narrative.h"

/* ===================================================================
 * Thread type table
 * =================================================================== */

static const char *const THREAD_NAMES[NARR_THREAD_COUNT] = {
    "convergence",
    "festival",
    "number",
    "cycle",
    "astronomical",
    "quiet"
};

/* ===================================================================
 * Depth name table
 * =================================================================== */

static const char *const DEPTH_NAMES[NARR_DEPTH_COUNT] = {
    "Surface",
    "Context",
    "Cycle",
    "Full"
};

/* ===================================================================
 * Narrative templates — one per (thread_type, variant) pair.
 *
 * Placeholder codes:
 *   %H = headline
 *   %A = convergence adjective
 *   %S = system count
 *   %N = system name
 *   %T = thread type
 * =================================================================== */

#define NARR_VARIANTS_PER_THREAD 5
#define NARR_TEMPLATE_MAX 256

typedef struct {
    char text[NARR_TEMPLATE_MAX];
} narr_template_t;

/* Templates indexed by [thread_index][variant]
 * Variants 0-2: original. Variants 3-4: voice-enriched.
 * Voice assignments from wisdom-voice-guide.md:
 *   convergence -> Rumi, festival -> Jesus, number -> Tesla,
 *   cycle -> Mandela/Lao Tzu, astronomical -> Newton, quiet -> Lao Tzu */
static const narr_template_t TEMPLATES[NARR_THREAD_COUNT][NARR_VARIANTS_PER_THREAD] = {
    /* convergence (+ Rumi voice) */
    {
        { "A %A convergence unfolds — %H. %S speak as one." },
        { "Today, %S align in a %A moment. %H." },
        { "%H — a %A alignment across %S." },
        { "Do you hear it? %S whispering the same truth — %H." },
        { "What you seek today, today seeks you. %H — a %A resonance." }
    },
    /* festival (+ Jesus parable voice) */
    {
        { "A celebration ripples through the calendar — %H." },
        { "Today marks a turning point. %H." },
        { "%H — the calendar remembers what matters." },
        { "Consider: %H. The smallest observance holds the largest truth." },
        { "They kept this day for a thousand years. %H. The keeping continues." }
    },
    /* number (+ Tesla precision-awe voice) */
    {
        { "A pattern emerges in the numbers — %H." },
        { "%H. The mathematics of time reveals a hidden thread." },
        { "Across %S, a shared number speaks — %H." },
        { "The frequency is unmistakable — %H. %S vibrate at the same harmonic." },
        { "%H. The numbers do not lie; they reveal what was always there." }
    },
    /* cycle (+ Mandela patience / Lao Tzu paradox voice) */
    {
        { "A cycle turns — %H." },
        { "%H. What was old begins again." },
        { "The wheel completes a revolution. %H." },
        { "The wheel has turned before. It will turn again. %H." },
        { "%H. What ends is also what begins — the cycle knows no difference." }
    },
    /* astronomical (+ Newton empirical wonder voice) */
    {
        { "The sky shifts — %H." },
        { "%H. The cosmos rearranges." },
        { "Above, a %A celestial event. %H." },
        { "%H. The mathematics is precise, but the beauty is beyond calculation." },
        { "Measured to the second, predicted for centuries — and still, %H." }
    },
    /* quiet (+ Lao Tzu voice) */
    {
        { "An ordinary day unfolds. %H." },
        { "%H. The cycles continue their quiet work." },
        { "Between the landmarks, the river still flows. %H." },
        { "%H. No convergence required. The ordinary is already enough." },
        { "The systems turn. None demand attention today. %H. This is enough." }
    }
};

/* ===================================================================
 * narr_init
 * =================================================================== */

void narr_init(narr_state_t *state)
{
    if (!state) return;
    memset(state, 0, sizeof(*state));
}

/* ===================================================================
 * narr_thread_index
 * =================================================================== */

int narr_thread_index(const char *thread_type)
{
    if (!thread_type) return -1;
    for (int i = 0; i < NARR_THREAD_COUNT; i++) {
        if (strcmp(thread_type, THREAD_NAMES[i]) == 0) {
            return i;
        }
    }
    return -1;
}

/* ===================================================================
 * narr_depth_name
 * =================================================================== */

const char *narr_depth_name(narr_depth_t depth)
{
    if (depth < 0 || depth >= NARR_DEPTH_COUNT) return "?";
    return DEPTH_NAMES[depth];
}

/* ===================================================================
 * narr_format_attribution
 * =================================================================== */

int narr_format_attribution(const char *author, const char *work,
                            int year, char *buf, int buf_size)
{
    if (!buf || buf_size <= 0) return 0;
    buf[0] = '\0';
    if (!author) return 0;

    int pos = 0;

    /* Author */
    int w = snprintf(buf + pos, (size_t)(buf_size - pos), "%s", author);
    if (w > 0) pos += w;

    /* Work */
    if (work && work[0]) {
        w = snprintf(buf + pos, (size_t)(buf_size - pos), ", %s", work);
        if (w > 0) pos += w;
    }

    /* Year */
    if (year != 0) {
        if (year < 0) {
            w = snprintf(buf + pos, (size_t)(buf_size - pos),
                         " (%d BCE)", -year);
        } else {
            w = snprintf(buf + pos, (size_t)(buf_size - pos),
                         " (%d CE)", year);
        }
        if (w > 0) pos += w;
    }

    return pos;
}

/* ===================================================================
 * Internal: select template variant (novelty-aware)
 * =================================================================== */

static int select_variant(const narr_state_t *state, int thread_idx)
{
    /* Simple rotation based on compose_count + thread offset */
    int base = state->compose_count + thread_idx;
    return base % NARR_VARIANTS_PER_THREAD;
}

/* ===================================================================
 * Internal: fill template placeholders
 * =================================================================== */

static void fill_template(const char *tmpl, const br_lang_context_t *ctx,
                           const char *system_name,
                           char *buf, int buf_size)
{
    const char *headline = br_lang_slot_value(ctx, BR_LANG_SLOT_HEADLINE);
    const char *conv_adj = br_lang_slot_value(ctx, BR_LANG_SLOT_CONVERGENCE_ADJ);
    const char *sys_count = br_lang_slot_value(ctx, BR_LANG_SLOT_SYSTEM_COUNT);
    const char *thread = br_lang_slot_value(ctx, BR_LANG_SLOT_THREAD_TYPE);

    if (!headline[0])  headline = "the moment unfolds";
    if (!conv_adj[0])  conv_adj = "quiet";
    if (!sys_count[0]) sys_count = "the systems";
    if (!thread[0])    thread = "day";
    if (!system_name)  system_name = "the calendar";

    int pos = 0;
    for (const char *p = tmpl; *p && pos < buf_size - 1; p++) {
        if (*p == '%' && *(p + 1)) {
            const char *insert = NULL;
            switch (*(p + 1)) {
            case 'H': insert = headline;  break;
            case 'A': insert = conv_adj;  break;
            case 'S': insert = sys_count; break;
            case 'N': insert = system_name; break;
            case 'T': insert = thread;    break;
            default:  break;
            }
            if (insert) {
                int w = snprintf(buf + pos, (size_t)(buf_size - pos),
                                 "%s", insert);
                if (w > 0) pos += w;
                p++; /* skip format char */
                continue;
            }
        }
        buf[pos++] = *p;
    }
    buf[pos] = '\0';
}

/* ===================================================================
 * Internal: record seen template
 * =================================================================== */

static void record_seen(narr_state_t *state, int template_index)
{
    if (state->seen_count < NARR_MAX_SEEN) {
        state->seen_templates[state->seen_count] = template_index;
        state->seen_count++;
    }
}

/* ===================================================================
 * narr_compose
 * =================================================================== */

int narr_compose(narr_state_t *state,
                 const narr_input_t *input,
                 narr_output_t *out)
{
    if (!state || !input || !out) return 0;
    if (!input->brain_ctx) return 0;

    memset(out, 0, sizeof(*out));

    const br_lang_context_t *ctx = input->brain_ctx;

    /* Determine thread type */
    const char *thread_str = br_lang_slot_value(ctx, BR_LANG_SLOT_THREAD_TYPE);
    int tidx = narr_thread_index(thread_str);
    if (tidx < 0) tidx = 5; /* default to quiet */
    out->thread_index = tidx;

    /* Select template variant */
    int variant = select_variant(state, tidx);
    int tmpl_idx = tidx * NARR_VARIANTS_PER_THREAD + variant;
    out->template_index = tmpl_idx;

    /* Extract headline */
    const char *headline = br_lang_slot_value(ctx, BR_LANG_SLOT_HEADLINE);
    if (headline[0]) {
        snprintf(out->headline, NARR_HEADLINE_MAX, "%s", headline);
    } else {
        snprintf(out->headline, NARR_HEADLINE_MAX, "The cycles continue");
    }

    /* Compose body based on depth */
    const char *tmpl_text = TEMPLATES[tidx][variant].text;

    if (input->depth == NARR_DEPTH_SURFACE) {
        /* Surface: headline only, no template expansion */
        snprintf(out->body, NARR_BODY_MAX, "%s", out->headline);
    } else {
        /* Context and deeper: use template */
        fill_template(tmpl_text, ctx, input->system_name,
                      out->body, NARR_BODY_MAX);
    }

    /* Wisdom: only at CONTEXT depth or deeper, and only if wisdom provided
     * and convergence warrants it (show_wisdom) or depth >= CYCLE */
    int show_wisdom = 0;
    if (input->depth >= NARR_DEPTH_CONTEXT && input->depth != NARR_DEPTH_SURFACE) {
        if (input->wisdom_text && input->wisdom_author) {
            if (ctx->show_wisdom || input->depth >= NARR_DEPTH_CYCLE) {
                show_wisdom = 1;
            }
        }
    }

    if (show_wisdom) {
        /* Format: "quote text" — attribution */
        char attr[128];
        narr_format_attribution(input->wisdom_author, input->wisdom_work,
                                input->wisdom_year, attr, (int)sizeof(attr));
        snprintf(out->wisdom_text, NARR_WISDOM_MAX,
                 "\"%s\" — %s", input->wisdom_text, attr);
        out->has_wisdom = 1;
    }

    /* Personal note: from brain context slot */
    const char *personal = br_lang_slot_value(ctx, BR_LANG_SLOT_PERSONAL_NOTE);
    if (personal[0] && input->depth >= NARR_DEPTH_CONTEXT) {
        snprintf(out->personal, NARR_PERSONAL_MAX, "%s", personal);
        out->has_personal = 1;
    }

    /* Novelty tracking */
    record_seen(state, tmpl_idx);
    state->compose_count++;

    return 1;
}
