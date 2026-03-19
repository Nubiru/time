/* aha_moments.h -- Pure module providing 50 pre-authored insight cards.
 *
 * Each "aha moment" surfaces when specific conditions align in the user's
 * experience: calendar convergences, lunar phases, birth profile resonance,
 * deep time scale, or structural parallels across knowledge systems.
 *
 * System bitmask bits (same as fun_facts.h):
 *   0=astronomy, 1=astrology, 2=tzolkin, 3=iching, 4=chinese,
 *   5=human_design, 6=hebrew, 7=islamic, 8=buddhist, 9=hindu,
 *   10=kabbalah, 11=gregorian, 12=geology, 13=earth
 *
 * All functions are pure: no globals, no malloc, no side effects. */

#ifndef TIME_AHA_MOMENTS_H
#define TIME_AHA_MOMENTS_H

#define AHA_MAX_RESULTS 8
#define AHA_TEXT_MAX    256
#define AHA_TITLE_MAX   64

/* Trigger categories */
typedef enum {
    AHA_TRIG_CONVERGENCE = 0,   /* Multiple systems align */
    AHA_TRIG_PERSONAL,          /* Birth profile resonance */
    AHA_TRIG_NUMBER,            /* Sacred number appears across systems */
    AHA_TRIG_LUNAR,             /* Lunar phase + calendar alignment */
    AHA_TRIG_SEASONAL,          /* Solstice/equinox + cultural alignment */
    AHA_TRIG_DEEP_TIME,         /* Geological/cosmic scale insight */
    AHA_TRIG_STRUCTURAL,        /* Cross-system structural parallel */
    AHA_TRIG_ALWAYS,            /* Always available (general wisdom) */
    AHA_TRIG_COUNT
} aha_trigger_t;

/* A single aha moment */
typedef struct {
    int id;
    char title[AHA_TITLE_MAX];
    char text[AHA_TEXT_MAX];
    aha_trigger_t trigger;
    unsigned int system_mask;   /* Bitmask: which systems must be active */
    int min_convergence;        /* Minimum convergence score (0 = none) */
    int depth;                  /* 1=surface, 2=intermediate, 3=deep */
} aha_entry_t;

/* Evaluation context */
typedef struct {
    unsigned int active_systems; /* Bitmask of currently visible systems */
    int convergence_score;       /* 0-10 */
    int has_birth;               /* 1 if birth profile loaded */
    int is_full_moon;            /* 1 if current lunar phase is full */
    int is_new_moon;
    int is_solstice;             /* 1 if near solstice */
    int is_equinox;
    int scale_level;             /* Camera scale 0-6 */
} aha_context_t;

/* Evaluation result */
typedef struct {
    int indices[AHA_MAX_RESULTS];
    int count;
} aha_result_t;

/* Evaluate all 50 insights against context. Returns matching indices
 * (up to AHA_MAX_RESULTS), sorted by depth descending.
 * NULL ctx -> empty result. */
aha_result_t aha_evaluate(const aha_context_t *ctx);

/* Return insight by ID (0-49). Invalid -> id=-1, title="?", text="?". */
aha_entry_t aha_get(int id);

/* Return total number of insights (50). */
int aha_count(void);

/* Return human-readable trigger category name. Invalid -> "?". */
const char *aha_trigger_name(aha_trigger_t trigger);

/* Return a zeroed context. */
aha_context_t aha_default_context(void);

#endif /* TIME_AHA_MOMENTS_H */
