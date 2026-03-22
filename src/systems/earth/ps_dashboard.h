/* ps_dashboard.h — Personal space dashboard layout.
 *
 * Computes a 4-section dashboard using golden rectangle subdivision:
 *   TL: Active Cycles    TR: Today for You
 *   BL: Cosmic Fingerprint  BR: Exploration Progress
 *
 * Tidwell "Dashboard" pattern (ACTION 7 from Designing Interfaces V2).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_PS_DASHBOARD_H
#define TIME_PS_DASHBOARD_H

#include "../../ui/golden_layout.h"

/* Dashboard section identifiers */
typedef enum {
    PS_ACTIVE_CYCLES = 0,   /* top-left: current active cycles */
    PS_TODAY_FOR_YOU,        /* top-right: personalized daily briefing */
    PS_FINGERPRINT,          /* bottom-left: cosmic fingerprint summary */
    PS_PROGRESS,             /* bottom-right: exploration progress */
    PS_SECTION_COUNT
} ps_section_id_t;

#define PS_TITLE_MAX 48

/* A single dashboard section */
typedef struct {
    char title[PS_TITLE_MAX];
    gl_rect_t bounds;        /* normalized position and size */
} ps_section_t;

/* Complete dashboard layout */
typedef struct {
    ps_section_t sections[PS_SECTION_COUNT];
    gl_rect_t container;     /* full dashboard area */
    float gap;               /* spacing between sections (phi-derived) */
} ps_dashboard_t;

/* Compute dashboard layout from a viewport rect and base spacing.
 * Splits viewport into 4 sections using golden rectangle subdivision.
 * gap = base_spacing * phi^-1 (between sections). */
ps_dashboard_t ps_compute(gl_rect_t viewport, float base_spacing);

/* Get section by ID. Returns NULL for invalid ID. */
const ps_section_t *ps_section(const ps_dashboard_t *dash, ps_section_id_t id);

/* Get section title string for an ID. */
const char *ps_section_title(ps_section_id_t id);

/* Check if a point (normalized) falls within a section.
 * Returns the section ID, or -1 if no hit. */
int ps_hit_test(const ps_dashboard_t *dash, float x, float y);

#endif /* TIME_PS_DASHBOARD_H */
