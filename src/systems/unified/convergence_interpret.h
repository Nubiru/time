/* convergence_interpret.h -- Interpretive narratives for convergence events
 *
 * Given convergence strength and participating systems, produces narrative
 * text explaining WHY the alignment matters.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_CONVERGENCE_INTERPRET_H
#define TIME_CONVERGENCE_INTERPRET_H

#include "convergence_detect.h"

/* Per-strength-level interpretation */
typedef struct {
    cd_strength_t strength;
    const char *name;           /* "Minor Alignment", "Notable Convergence", etc. */
    const char *description;    /* What this level means */
    const char *significance;   /* Why it matters */
} ci_strength_t;

/* Per-system contribution to convergence */
typedef struct {
    cd_system_t system;
    const char *system_name;    /* Human name */
    const char *tradition;      /* Cultural/religious tradition */
    const char *what_aligns;    /* What significant moment this system is at */
} ci_system_t;

/* Common convergence patterns */
typedef struct {
    const char *name;           /* Pattern name: "Lunar Triple", "Sacred Month", etc. */
    const char *description;    /* What this pattern means when it occurs */
    int systems[6];             /* cd_system_t values involved (-1 terminated) */
} ci_pattern_t;

/* Tiered interpretation result */
typedef struct {
    char glyph[8];              /* T0: strength indicator (e.g., "***") */
    char glance[128];           /* T1: "Notable Convergence -- 3 traditions align" */
    char detail[512];           /* T3: Full narrative */
} convergence_interp_t;

/* Lookup strength interpretation data.
 * Invalid strength (0 or >4): returns struct with name="?". */
ci_strength_t ci_strength_data(cd_strength_t strength);

/* Lookup system contribution data.
 * Invalid system: returns struct with system_name="?". */
ci_system_t ci_system_data(cd_system_t system);

/* Lookup named convergence pattern by index (0-7).
 * Invalid index: returns struct with name="?". */
ci_pattern_t ci_pattern_data(int index);

/* Compose tiered interpretation from strength + participating systems.
 * If systems is NULL or count<=0, produces minimal output. */
convergence_interp_t ci_interpret(cd_strength_t strength,
                                  const cd_system_t *systems,
                                  int system_count);

/* Number of strength levels (always 4). */
int ci_strength_count(void);

/* Number of system types (always CD_SYS_COUNT = 21). */
int ci_system_count(void);

/* Number of named patterns (always 8). */
int ci_pattern_count(void);

#endif /* TIME_CONVERGENCE_INTERPRET_H */
