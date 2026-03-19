/* chakra_interpret.h — Chakra system interpretation data
 *
 * Archetype data for the 7 chakras of the Hindu/yogic tradition.
 * Each chakra is an energy center with psychological, physical,
 * and spiritual dimensions.  The chakra system maps the journey
 * from survival (root) to transcendence (crown) — a vertical
 * axis of consciousness that parallels the Kabbalistic Tree of
 * Life and the Human Design bodygraph.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#ifndef TIME_CHAKRA_INTERPRET_H
#define TIME_CHAKRA_INTERPRET_H

/* Per-chakra interpretation (7 chakras) */
typedef struct {
    int chakra;                 /* 0-6 (Muladhara through Sahasrara) */
    const char *sanskrit;       /* Sanskrit name */
    const char *english;        /* English name */
    const char *archetype;      /* Psychological archetype */
    const char *theme;          /* Core theme when balanced */
    const char *shadow;         /* Imbalance / shadow expression */
    const char *question;       /* The existential question this chakra asks */
    const char *brief;          /* One-sentence meaning */
} chi_chakra_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[16];             /* T0: bija mantra */
    char glance[128];           /* T1: "Anahata (Heart) — Unconditional love" */
    char detail[512];           /* T3: Full narrative */
} chakra_interp_t;

/* Return interpretation for a chakra (0-6).
 * Invalid: returns struct with chakra=-1, all strings "?". */
chi_chakra_t chi_chakra_data(int chakra);

/* Compose tiered interpretation.
 * chakra: 0-6. Invalid: "?" strings. */
chakra_interp_t chi_interpret(int chakra);

int chi_chakra_count(void);    /* Returns 7 */

#endif /* TIME_CHAKRA_INTERPRET_H */
