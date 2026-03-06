#ifndef TIME_SYSTEM_SCALE_MAP_H
#define TIME_SYSTEM_SCALE_MAP_H

#define KNOWLEDGE_SYSTEM_COUNT 7

typedef enum {
    SYS_GREGORIAN  = 0,
    SYS_ASTROLOGY  = 1,
    SYS_TZOLKIN    = 2,
    SYS_ICHING     = 3,
    SYS_CHINESE    = 4,
    SYS_HD         = 5,
    SYS_COSMIC     = 6
} knowledge_system_t;

typedef struct {
    knowledge_system_t id;
    const char *name;
    int home_scale;        /* scale level where this system is most prominent */
    float scale_min;       /* minimum log_zoom where visible (fade in) */
    float scale_max;       /* maximum log_zoom where visible (fade out) */
    float home_opacity;    /* opacity at home scale (0.0-1.0) */
} system_scale_info_t;

/* Get scale mapping for a knowledge system. */
system_scale_info_t system_scale_info(knowledge_system_t sys);

/* Compute opacity of a knowledge system at a given log_zoom.
 * Returns 0.0-1.0 with fade ramps at boundaries. */
float system_opacity_at_zoom(knowledge_system_t sys, float log_zoom);

/* Get the name of a knowledge system. */
const char *system_name(knowledge_system_t sys);

/* Get all systems visible at a given log_zoom (opacity > 0).
 * Writes system IDs to out[], returns count. max = KNOWLEDGE_SYSTEM_COUNT. */
int systems_visible_at_zoom(float log_zoom, knowledge_system_t *out, int max);

/* Get the home scale for a system (which scale level). */
int system_home_scale(knowledge_system_t sys);

/* Total number of knowledge systems. */
int system_count(void);

#endif
