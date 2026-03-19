/* text_choreo.c — Text Animation Choreography
 *
 * Phi-derived animation presets for each text category.
 * Uses text_reveal for enter animations, simple fade for exit.
 * Pure functions: no globals, no malloc, no side effects. */

#include "text_choreo.h"

/* ---- Phi constants for timing ---- */

#define PHI        1.6180339887f
#define PHI_INV    0.6180339887f   /* 1/phi */
#define PHI_INV2   0.3819660113f   /* 1/phi^2 */
#define PHI_INV3   0.2360679775f   /* 1/phi^3 */

/* ---- Static preset table ---- */

/*
 * Timing rationale (all derived from phi):
 *
 * WISDOM:  typewriter enter (1.5s = slow), hold 8s (epic), fade out 1.5s
 *          → contemplative, slow reveal, long presence
 *
 * TOAST:   instant enter, hold 2.6s (dramatic*phi^-1), fade out 0.38s
 *          → quick notification, doesn't linger
 *
 * CARD:    word fade-in (0.62s = slow*phi^-1), no auto-exit (persistent)
 *          → user-triggered, stays until dismissed
 *
 * NARRATIVE: word fade-in (1.0s = base), hold 6.18s, fade out 1.0s
 *          → paragraphs appear gently, held for reading, soft exit
 *
 * HUD:     instant, persistent (no hold timer, no exit)
 *          → data displays, always-on
 */

static const tc_preset_t PRESETS[TC_CATEGORY_COUNT] = {
    [TC_WISDOM] = {
        .enter_style    = TEXT_REVEAL_TYPEWRITER,
        .enter_easing   = EASE_TYPE_OUT_CUBIC,
        .enter_duration = 1.5f,                 /* slow: ~phi^-1 * 2.4 */
        .hold_duration  = 8.0f,                 /* epic: ~phi^2 * 3 */
        .exit_duration  = 1.5f,                 /* slow: matches enter */
        .exit_easing    = EASE_TYPE_IN_SINE
    },
    [TC_TOAST] = {
        .enter_style    = TEXT_REVEAL_INSTANT,
        .enter_easing   = EASE_TYPE_LINEAR,
        .enter_duration = 0.0f,                 /* instant */
        .hold_duration  = 2.618f,               /* phi^2 seconds */
        .exit_duration  = PHI_INV2,             /* ~0.38s: fast fade */
        .exit_easing    = EASE_TYPE_OUT_QUAD
    },
    [TC_CARD] = {
        .enter_style    = TEXT_REVEAL_FADE_WORD,
        .enter_easing   = EASE_TYPE_OUT_CUBIC,
        .enter_duration = PHI_INV,              /* ~0.618s */
        .hold_duration  = 0.0f,                 /* persistent */
        .exit_duration  = 0.0f,                 /* no auto-exit */
        .exit_easing    = EASE_TYPE_LINEAR
    },
    [TC_NARRATIVE] = {
        .enter_style    = TEXT_REVEAL_FADE_WORD,
        .enter_easing   = EASE_TYPE_OUT_SINE,
        .enter_duration = 1.0f,                 /* base unit */
        .hold_duration  = 6.18f,                /* phi * base * ~3.8 */
        .exit_duration  = 1.0f,                 /* base unit */
        .exit_easing    = EASE_TYPE_IN_SINE
    },
    [TC_HUD] = {
        .enter_style    = TEXT_REVEAL_INSTANT,
        .enter_easing   = EASE_TYPE_LINEAR,
        .enter_duration = 0.0f,
        .hold_duration  = 0.0f,                 /* persistent */
        .exit_duration  = 0.0f,
        .exit_easing    = EASE_TYPE_LINEAR
    }
};

static const char *CATEGORY_NAMES[TC_CATEGORY_COUNT] = {
    "Wisdom", "Toast", "Card", "Narrative", "HUD"
};

/* ---- Public API ---- */

tc_preset_t tc_preset(tc_category_t category)
{
    if (category < 0 || category >= TC_CATEGORY_COUNT) {
        return PRESETS[TC_HUD];
    }
    return PRESETS[category];
}

tc_seq_t tc_create(tc_category_t category, int count)
{
    tc_preset_t p = tc_preset(category);
    tc_seq_t seq;
    seq.category = category;
    seq.phase = TC_PHASE_ENTER;
    seq.elapsed = 0.0f;
    seq.phase_time = 0.0f;
    seq.element_count = count > 0 ? count : 1;
    seq.done = 0;

    /* Set up reveal for enter phase */
    seq.reveal = text_reveal_create(p.enter_style, seq.element_count, p.enter_duration);
    seq.reveal = text_reveal_set_easing(seq.reveal, p.enter_easing);
    seq.reveal = text_reveal_start(seq.reveal);

    return seq;
}

tc_seq_t tc_tick(tc_seq_t seq, float dt)
{
    if (seq.done || dt <= 0.0f) return seq;

    tc_preset_t p = tc_preset(seq.category);
    seq.elapsed += dt;
    seq.phase_time += dt;

    /* Loop to handle multi-phase transitions in a single tick
     * (e.g., enter_duration=0 skips straight through to HOLD). */
    for (int guard = 0; guard < 4 && !seq.done; guard++) {
        switch (seq.phase) {
        case TC_PHASE_ENTER:
            seq.reveal = text_reveal_tick(seq.reveal, dt);
            if (p.enter_duration <= 0.0f || seq.phase_time >= p.enter_duration) {
                seq.phase = TC_PHASE_HOLD;
                seq.phase_time -= p.enter_duration;
                if (seq.phase_time < 0.0f) seq.phase_time = 0.0f;
                continue; /* re-evaluate in HOLD with remaining time */
            }
            return seq;

        case TC_PHASE_HOLD:
            /* Persistent (hold=0, exit=0): stay in HOLD forever */
            if (p.hold_duration <= 0.0f && p.exit_duration <= 0.0f) {
                return seq;
            }
            if (p.hold_duration > 0.0f && seq.phase_time >= p.hold_duration) {
                if (p.exit_duration > 0.0f) {
                    seq.phase = TC_PHASE_EXIT;
                    seq.phase_time -= p.hold_duration;
                    if (seq.phase_time < 0.0f) seq.phase_time = 0.0f;
                    continue; /* re-evaluate in EXIT with remaining time */
                } else {
                    seq.phase = TC_PHASE_DONE;
                    seq.done = 1;
                }
            }
            return seq;

        case TC_PHASE_EXIT:
            if (seq.phase_time >= p.exit_duration) {
                seq.phase = TC_PHASE_DONE;
                seq.done = 1;
            }
            return seq;

        case TC_PHASE_DONE:
            seq.done = 1;
            return seq;
        }
    }

    return seq;
}

float tc_opacity(tc_seq_t seq)
{
    tc_preset_t p = tc_preset(seq.category);

    switch (seq.phase) {
    case TC_PHASE_ENTER:
        if (p.enter_duration <= 0.0f) return 1.0f;
        return text_reveal_progress(seq.reveal);

    case TC_PHASE_HOLD:
        return 1.0f;

    case TC_PHASE_EXIT: {
        if (p.exit_duration <= 0.0f) return 0.0f;
        float t = seq.phase_time / p.exit_duration;
        if (t > 1.0f) t = 1.0f;
        /* Apply exit easing */
        float eased = anim_apply_easing(p.exit_easing, t);
        return 1.0f - eased; /* 1→0 */
    }

    case TC_PHASE_DONE:
        return 0.0f;
    }

    return 0.0f;
}

float tc_char_visibility(tc_seq_t seq, int index)
{
    switch (seq.phase) {
    case TC_PHASE_ENTER:
        return text_reveal_visibility(seq.reveal, index);
    case TC_PHASE_HOLD:
        return 1.0f;
    case TC_PHASE_EXIT:
        return tc_opacity(seq); /* uniform fade */
    case TC_PHASE_DONE:
        return 0.0f;
    }
    return 0.0f;
}

const char *tc_category_name(tc_category_t category)
{
    if (category < 0 || category >= TC_CATEGORY_COUNT) return "Unknown";
    return CATEGORY_NAMES[category];
}
