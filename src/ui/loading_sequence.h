/* loading_sequence.h — Loading screen content and progress milestones.
 *
 * Defines the sequence of loading phases, progress milestones,
 * and inspirational quotes shown during Time's initialization.
 *
 * The loading screen is itself part of the artwork: it reveals
 * layers of meaning as systems come online.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_LOADING_SEQUENCE_H
#define TIME_LOADING_SEQUENCE_H

#define LS_MAX_PHASES 8
#define LS_MAX_QUOTES 12

/* Loading phases — correspond to initialization order */
typedef enum {
    LS_PHASE_INIT       = 0,   /* WebGL context, shaders */
    LS_PHASE_ASTRONOMY  = 1,   /* Star catalog, planet data */
    LS_PHASE_CALENDARS  = 2,   /* All calendar systems */
    LS_PHASE_KNOWLEDGE  = 3,   /* Knowledge system data */
    LS_PHASE_GEOMETRY   = 4,   /* Ring, globe, dome meshes */
    LS_PHASE_TEXTURES   = 5,   /* Font atlas, symbol atlas */
    LS_PHASE_RENDER     = 6,   /* Render passes initialization */
    LS_PHASE_COMPLETE   = 7,   /* Everything loaded */
    LS_PHASE_COUNT      = 8
} ls_phase_t;

/* Phase metadata */
typedef struct {
    ls_phase_t phase;
    const char *name;           /* "Astronomy", "Calendars", etc. */
    const char *description;    /* what's happening in this phase */
    float weight;               /* relative duration weight (0-1, sums to 1) */
    float progress_start;       /* normalized progress at phase start (0-1) */
    float progress_end;         /* normalized progress at phase end (0-1) */
} ls_phase_info_t;

/* Inspirational quote */
typedef struct {
    const char *text;
    const char *author;
    int phase_affinity;         /* which phase this quote relates to (-1 = any) */
} ls_quote_t;

/* Loading state */
typedef struct {
    ls_phase_t current_phase;
    float phase_progress;       /* 0-1 within current phase */
    float total_progress;       /* 0-1 overall */
    int quote_index;            /* which quote is showing */
} ls_state_t;

/* Initialize loading state. */
ls_state_t ls_init(void);

/* Get phase info by index. Invalid returns a sentinel. */
ls_phase_info_t ls_phase_info(ls_phase_t phase);

/* Advance to next phase. Returns updated state. */
ls_state_t ls_advance_phase(ls_state_t state);

/* Set progress within current phase. Returns updated state. */
ls_state_t ls_set_progress(ls_state_t state, float progress);

/* Compute total progress from phase + phase_progress. */
float ls_total_progress(ls_phase_t phase, float phase_progress);

/* Get a quote for the current phase.
 * seed: any integer for deterministic selection (e.g., Julian Day). */
ls_quote_t ls_quote_for_phase(ls_phase_t phase, int seed);

/* Total number of quotes. */
int ls_quote_count(void);

/* Get quote by index. Invalid returns empty quote. */
ls_quote_t ls_quote_at(int index);

/* Phase count. */
int ls_phase_count(void);

/* Is loading complete? */
int ls_is_complete(ls_state_t state);

/* Display title for the loading screen. */
const char *ls_title(void);

/* Subtitle/tagline. */
const char *ls_subtitle(void);

#endif /* TIME_LOADING_SEQUENCE_H */
