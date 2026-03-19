/* loading_sequence.c — Loading screen content and progress milestones.
 *
 * Pure functions producing loading sequence data.
 * No globals, no malloc, no side effects. */

#include "loading_sequence.h"

#include <stddef.h>

/* ---- Phase definitions ----
 * Weights: INIT=0.05, ASTRONOMY=0.20, CALENDARS=0.20, KNOWLEDGE=0.15,
 *          GEOMETRY=0.15, TEXTURES=0.10, RENDER=0.10, COMPLETE=0.05 */

static const ls_phase_info_t s_phases[LS_PHASE_COUNT] = {
    { LS_PHASE_INIT,      "Init",       "Preparing canvas",              0.05f, 0.00f, 0.05f },
    { LS_PHASE_ASTRONOMY, "Astronomy",  "Loading star catalog",          0.20f, 0.05f, 0.25f },
    { LS_PHASE_CALENDARS, "Calendars",  "Computing calendar systems",    0.20f, 0.25f, 0.45f },
    { LS_PHASE_KNOWLEDGE, "Knowledge",  "Connecting knowledge systems",  0.15f, 0.45f, 0.60f },
    { LS_PHASE_GEOMETRY,  "Geometry",   "Building meshes",               0.15f, 0.60f, 0.75f },
    { LS_PHASE_TEXTURES,  "Textures",   "Rendering glyphs and symbols",  0.10f, 0.75f, 0.85f },
    { LS_PHASE_RENDER,    "Render",     "Initializing render passes",    0.10f, 0.85f, 0.95f },
    { LS_PHASE_COMPLETE,  "Complete",   "Ready",                         0.05f, 0.95f, 1.00f },
};

/* ---- Quote database ---- */

#define QUOTE_COUNT 12

static const ls_quote_t s_quotes[QUOTE_COUNT] = {
    {
        "Time is the most valuable thing a man can spend.",
        "Theophrastus",
        LS_PHASE_INIT
    },
    {
        "The only reason for time is so that everything doesn't happen at once.",
        "Albert Einstein",
        LS_PHASE_ASTRONOMY
    },
    {
        "We are such stuff as dreams are made on, and our little life is rounded with a sleep.",
        "William Shakespeare",
        LS_PHASE_CALENDARS
    },
    {
        "Realize deeply that the present moment is all you ever have.",
        "Eckhart Tolle",
        LS_PHASE_KNOWLEDGE
    },
    {
        "In the beginning was the Word, and the Word was with God.",
        "John 1:1",
        LS_PHASE_GEOMETRY
    },
    {
        "The nitrogen in our DNA, the calcium in our teeth, the iron in our blood were made in the interiors of collapsing stars.",
        "Carl Sagan",
        LS_PHASE_TEXTURES
    },
    {
        "Look deep into nature, and then you will understand everything better.",
        "Albert Einstein",
        LS_PHASE_RENDER
    },
    {
        "Every moment is a fresh beginning.",
        "T.S. Eliot",
        LS_PHASE_COMPLETE
    },
    {
        "The cosmos is within us. We are made of star-stuff.",
        "Carl Sagan",
        -1
    },
    {
        "Yesterday is gone. Tomorrow has not yet come. We have only today.",
        "Mother Teresa",
        -1
    },
    {
        "Time you enjoy wasting is not wasted time.",
        "Marthe Troly-Curtin",
        -1
    },
    {
        "The universe is under no obligation to make sense to you.",
        "Neil deGrasse Tyson",
        -1
    },
};

/* ---- Helpers ---- */

static float clampf(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* ---- Public API ---- */

ls_state_t ls_init(void)
{
    ls_state_t s;
    s.current_phase = LS_PHASE_INIT;
    s.phase_progress = 0.0f;
    s.total_progress = 0.0f;
    s.quote_index = 0;
    return s;
}

ls_phase_info_t ls_phase_info(ls_phase_t phase)
{
    if (phase < 0 || phase >= LS_PHASE_COUNT) {
        ls_phase_info_t sentinel;
        sentinel.phase = (ls_phase_t)-1;
        sentinel.name = NULL;
        sentinel.description = NULL;
        sentinel.weight = 0.0f;
        sentinel.progress_start = 0.0f;
        sentinel.progress_end = 0.0f;
        return sentinel;
    }
    return s_phases[phase];
}

ls_state_t ls_advance_phase(ls_state_t state)
{
    if (state.current_phase < LS_PHASE_COMPLETE) {
        state.current_phase = (ls_phase_t)(state.current_phase + 1);
    }
    state.phase_progress = 0.0f;
    state.total_progress = ls_total_progress(state.current_phase, 0.0f);
    return state;
}

ls_state_t ls_set_progress(ls_state_t state, float progress)
{
    state.phase_progress = clampf(progress, 0.0f, 1.0f);
    state.total_progress = ls_total_progress(state.current_phase,
                                              state.phase_progress);
    return state;
}

float ls_total_progress(ls_phase_t phase, float phase_progress)
{
    if (phase < 0 || phase >= LS_PHASE_COUNT) {
        return 0.0f;
    }
    float pp = clampf(phase_progress, 0.0f, 1.0f);
    float start = s_phases[phase].progress_start;
    float end   = s_phases[phase].progress_end;
    return start + pp * (end - start);
}

ls_quote_t ls_quote_for_phase(ls_phase_t phase, int seed)
{
    /* First: collect quotes with matching phase affinity */
    int matching[QUOTE_COUNT];
    int match_count = 0;

    for (int i = 0; i < QUOTE_COUNT; i++) {
        if (s_quotes[i].phase_affinity == (int)phase ||
            s_quotes[i].phase_affinity == -1) {
            matching[match_count++] = i;
        }
    }

    if (match_count == 0) {
        /* Fallback: return first quote */
        return s_quotes[0];
    }

    /* Deterministic selection from matching quotes */
    int abs_seed = seed < 0 ? -seed : seed;
    int idx = abs_seed % match_count;
    return s_quotes[matching[idx]];
}

int ls_quote_count(void)
{
    return QUOTE_COUNT;
}

ls_quote_t ls_quote_at(int index)
{
    if (index < 0 || index >= QUOTE_COUNT) {
        ls_quote_t empty;
        empty.text = NULL;
        empty.author = NULL;
        empty.phase_affinity = -1;
        return empty;
    }
    return s_quotes[index];
}

int ls_phase_count(void)
{
    return LS_PHASE_COUNT;
}

int ls_is_complete(ls_state_t state)
{
    return state.current_phase == LS_PHASE_COMPLETE;
}

const char *ls_title(void)
{
    return "TIME";
}

const char *ls_subtitle(void)
{
    return "Every pixel earned.";
}
