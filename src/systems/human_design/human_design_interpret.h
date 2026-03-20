/* human_design_interpret.h — Human Design interpretation data
 *
 * Archetype data for the 5 Types, 7 Authorities, 12 Profiles,
 * and 9 Centers of the Human Design system.  Composition function
 * produces tiered interpretation text (glyph / glance / detail).
 *
 * Human Design synthesizes astrology, I Ching, Kabbalah, Hindu
 * chakras, and quantum physics into a bodygraph map of individual
 * differentiation.  The interpretation layer surfaces the meaning
 * behind the mechanics.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_HUMAN_DESIGN_INTERPRET_H
#define TIME_HUMAN_DESIGN_INTERPRET_H

#include "../../ui/i18n.h"  /* i18n_locale_t */

/* ================================================================
 * Type enumeration (5 types)
 * ================================================================ */

enum {
    HDI_MANIFESTOR            = 0,
    HDI_GENERATOR             = 1,
    HDI_MANIFESTING_GENERATOR = 2,
    HDI_PROJECTOR             = 3,
    HDI_REFLECTOR             = 4
};

/* ================================================================
 * Authority enumeration (7 authorities)
 * ================================================================ */

enum {
    HDI_AUTH_EMOTIONAL      = 0,
    HDI_AUTH_SACRAL         = 1,
    HDI_AUTH_SPLENIC        = 2,
    HDI_AUTH_EGO            = 3,
    HDI_AUTH_SELF_PROJECTED = 4,
    HDI_AUTH_MENTAL         = 5,
    HDI_AUTH_LUNAR          = 6
};

/* ================================================================
 * Data structures
 * ================================================================ */

/* Per-type archetype */
typedef struct {
    int type;                   /* 0-4 (enum value) */
    const char *name;           /* "Generator", etc. */
    const char *strategy;       /* How to make decisions */
    const char *signature;      /* Feeling when aligned */
    const char *not_self;       /* Feeling when misaligned */
    const char *aura;           /* Aura quality */
    const char *brief;          /* One-sentence meaning */
} hdi_type_t;

/* Per-authority archetype */
typedef struct {
    int authority;              /* 0-6 (enum value) */
    const char *name;           /* "Emotional", etc. */
    const char *process;        /* How decisions are made */
    const char *signal;         /* What to listen for */
    const char *brief;          /* One-sentence meaning */
} hdi_authority_t;

/* Per-profile archetype (12 valid combinations) */
typedef struct {
    int line1;                  /* 1-6 (conscious line) */
    int line2;                  /* 1-6 (unconscious line) */
    const char *name;           /* "1/3", "4/6", etc. */
    const char *theme;          /* "Investigator / Martyr", etc. */
    const char *conscious_role; /* Conscious line meaning */
    const char *unconscious_role; /* Unconscious line meaning */
    const char *brief;          /* One-sentence meaning */
} hdi_profile_t;

/* Per-center interpretation (9 centers) */
typedef struct {
    int center;                 /* 0-8 (matches bg_center_id_t) */
    const char *name;           /* "Head", "Sacral", etc. */
    const char *theme_defined;  /* Meaning when defined (colored) */
    const char *theme_open;     /* Meaning when open (white) */
    const char *question;       /* The open center's existential question */
    const char *biological;     /* Body connection */
} hdi_center_theme_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: type abbreviation (3 chars) */
    char glance[128];           /* T1: "Generator 1/3 — Gate 1: The Creative" */
    char detail[1024];          /* T3: Full narrative */
} human_design_interp_t;

/* ================================================================
 * Public API
 * ================================================================ */

/* Return archetype data for an HD type (0-4).
 * Invalid index: returns struct with type=-1, all strings "?". */
hdi_type_t hdi_type_data(int type);

/* Return archetype data for an HD authority (0-6).
 * Invalid index: returns struct with authority=-1, all strings "?". */
hdi_authority_t hdi_authority_data(int authority);

/* Return archetype data for an HD profile (line1/line2).
 * Only 12 valid combinations exist (1/3, 1/4, 2/4, 2/5, 3/5, 3/6,
 * 4/6, 4/1, 5/1, 5/2, 6/2, 6/3).
 * Invalid pair: returns struct with line1=-1, all strings "?". */
hdi_profile_t hdi_profile_data(int line1, int line2);

/* Return interpretation data for an HD center (0-8).
 * Invalid index: returns struct with center=-1, all strings "?". */
hdi_center_theme_t hdi_center_data(int center);

/* Compose tiered interpretation from chart components.
 * type: 0-4, authority: 0-6, line1/line2: profile lines,
 * sun_gate: 1-64 (Sun gate number from incarnation cross).
 * Invalid type: "?" strings in result. */
human_design_interp_t hdi_interpret(int type, int authority,
                                    int line1, int line2,
                                    int sun_gate);

/* Returns 5 (number of HD types). */
int hdi_type_count(void);

/* Returns 7 (number of HD authorities). */
int hdi_authority_count(void);

/* Returns 9 (number of HD centers). */
int hdi_center_count(void);

/* Locale-aware interpretation. Same as hdi_interpret but produces
 * translated output for the given locale (falls back to English).
 * Requires content_i18n module. */
human_design_interp_t hdi_interpret_locale(int type, int authority,
                                           int line1, int line2,
                                           int sun_gate,
                                           i18n_locale_t locale);

#endif /* TIME_HUMAN_DESIGN_INTERPRET_H */
