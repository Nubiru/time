/* brain_narrative.c — Daily narrative composer implementation.
 *
 * Selects narrative template based on the top insight type, then
 * fills headline, summary, and thread type. Extracts featured systems
 * from the top insights. Enriches headlines with real astronomical
 * and calendar events (retrograde, moon phase, festivals).
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_narrative.h"
#include "convergence_detect.h"
#include "../../systems/astronomy/retrograde.h"
#include "../../systems/astronomy/lunar.h"
#include "../../systems/astronomy/planets.h"
#include "../../systems/astrology/zodiac.h"
#include <string.h>
#include <stdio.h>

/* ===================================================================
 * Internal: headline formatters per insight type
 * =================================================================== */

/* Build a comma-separated system name list with "and" before the last.
 * e.g. "Astronomy, Islamic, and Bahai". Returns chars written. */
static int format_system_names(const int *systems, int count,
                               char *buf, int buf_size) {
    int pos = 0;
    int remaining = buf_size;

    for (int i = 0; i < count && remaining > 1; i++) {
        const char *name = cd_system_name((cd_system_t)systems[i]);
        int n;

        if (i == 0) {
            n = snprintf(buf + pos, (size_t)remaining, "%s", name);
        } else if (i == count - 1) {
            n = snprintf(buf + pos, (size_t)remaining,
                         count == 2 ? " and %s" : ", and %s", name);
        } else {
            n = snprintf(buf + pos, (size_t)remaining, ", %s", name);
        }

        if (n > 0) {
            int used = n < remaining ? n : remaining - 1;
            pos += used;
            remaining -= used;
        }
    }
    return pos;
}

static int headline_convergence(const br_insight_t *insight,
                                char *buf, int buf_size) {
    if (insight->system_count >= 2) {
        /* Build: "Astronomy, Islamic, and Bahai converge" */
        char names[96];
        format_system_names(insight->systems, insight->system_count,
                            names, sizeof(names));
        return snprintf(buf, (size_t)buf_size, "%s converge", names);
    }
    return snprintf(buf, (size_t)buf_size, "%s", insight->headline);
}

static int headline_quiet(char *buf, int buf_size) {
    return snprintf(buf, (size_t)buf_size,
                    "A quiet day — the cycles turn in silence");
}

static int headline_festival(const br_insight_t *insight,
                             char *buf, int buf_size) {
    return snprintf(buf, (size_t)buf_size,
                    "Festival day: %s", insight->headline);
}

static int headline_number(const br_insight_t *insight,
                           char *buf, int buf_size) {
    return snprintf(buf, (size_t)buf_size,
                    "Sacred number active: %s", insight->headline);
}

static int headline_cycle(const br_insight_t *insight,
                          char *buf, int buf_size) {
    return snprintf(buf, (size_t)buf_size,
                    "Cycle milestone: %s", insight->headline);
}

static int headline_astronomical(const br_insight_t *insight,
                                 char *buf, int buf_size) {
    return snprintf(buf, (size_t)buf_size,
                    "Celestial event: %s", insight->headline);
}

/* ===================================================================
 * Internal: astronomical/calendar event enrichment
 * =================================================================== */

/* Build an event context string from real astronomical + calendar data.
 * e.g. "Mercury retrograde. Full Moon in Pisces. Spring Equinox."
 * Returns chars written. Pure: all data computed from jd. */
static int enrich_with_events(double jd, char *buf, int buf_size)
{
    int pos = 0;
    int remaining = buf_size;

    if (buf_size <= 0) return 0;
    buf[0] = '\0';

    /* 1. Check retrograde planets (Mercury is most notable) */
    for (int p = PLANET_MERCURY; p <= PLANET_SATURN; p++) {
        if (p == PLANET_EARTH) continue;
        retrograde_info_t info = retrograde_check(p, jd);
        if (info.motion == MOTION_RETROGRADE) {
            int n = snprintf(buf + pos, (size_t)remaining,
                             "%s%s retrograde",
                             pos > 0 ? ". " : "",
                             planet_name(p));
            if (n > 0) {
                int used = n < remaining ? n : remaining - 1;
                pos += used;
                remaining -= used;
            }
        } else if (info.motion == MOTION_STATIONARY && remaining > 1) {
            int n = snprintf(buf + pos, (size_t)remaining,
                             "%s%s stationary",
                             pos > 0 ? ". " : "",
                             planet_name(p));
            if (n > 0) {
                int used = n < remaining ? n : remaining - 1;
                pos += used;
                remaining -= used;
            }
        }
    }

    /* 2. Moon phase (only report significant phases) */
    lunar_info_t moon = lunar_phase(jd);
    if (moon.phase == MOON_FULL || moon.phase == MOON_NEW) {
        int moon_sign = lunar_zodiac_sign(jd);
        const char *sign_name = zodiac_sign_name(moon_sign);
        int n = snprintf(buf + pos, (size_t)remaining,
                         "%s%s in %s",
                         pos > 0 ? ". " : "",
                         lunar_phase_name(moon.phase),
                         sign_name);
        if (n > 0) {
            int used = n < remaining ? n : remaining - 1;
            pos += used;
            remaining -= used;
        }
    }

    /* 3. Moon phase detail for quarters */
    if (moon.phase == MOON_FIRST_QUARTER || moon.phase == MOON_LAST_QUARTER) {
        int n = snprintf(buf + pos, (size_t)remaining,
                         "%s%s",
                         pos > 0 ? ". " : "",
                         lunar_phase_name(moon.phase));
        if (n > 0) {
            int used = n < remaining ? n : remaining - 1;
            pos += used;
            remaining -= used;
        }
    }

    if (pos > 0 && remaining > 1) {
        buf[pos] = '.';
        pos++;
        if (pos < buf_size) buf[pos] = '\0';
    }

    return pos;
}

/* ===================================================================
 * Internal: summary composers
 * =================================================================== */

static int summary_from_insights(const br_result_t *result,
                                 char *buf, int buf_size) {
    int written = 0;
    int remaining = buf_size;

    /* Lead with the top insight — use system names for convergence */
    if (result->insight_count > 0 && result->top_insight_index >= 0 &&
        result->top_insight_index < result->insight_count) {
        const br_insight_t *top = &result->insights[result->top_insight_index];

        if (top->type == BR_INSIGHT_CONVERGENCE && top->system_count >= 2) {
            char names[96];
            format_system_names(top->systems, top->system_count,
                                names, sizeof(names));
            int n = snprintf(buf + written, (size_t)remaining,
                             "%s share this moment.", names);
            if (n > 0) {
                int used = n < remaining ? n : remaining - 1;
                written += used;
                remaining -= used;
            }
        } else if (top->type != BR_INSIGHT_QUIET && top->headline[0] != '\0') {
            int n = snprintf(buf + written, (size_t)remaining,
                             "%s.", top->headline);
            if (n > 0) {
                int used = n < remaining ? n : remaining - 1;
                written += used;
                remaining -= used;
            }
        }
    }

    /* Add secondary insights (up to 2 more) */
    int added = 0;
    for (int i = 0; i < result->insight_count && added < 2 && remaining > 2; i++) {
        if (i == result->top_insight_index) continue;
        const br_insight_t *ins = &result->insights[i];
        if (ins->type == BR_INSIGHT_QUIET) continue;
        if (ins->headline[0] == '\0') continue;

        int n = snprintf(buf + written, (size_t)remaining,
                         " %s.", ins->headline);
        if (n > 0) {
            int used = n < remaining ? n : remaining - 1;
            written += used;
            remaining -= used;
            added++;
        }
    }

    /* Quiet day fallback */
    if (written == 0) {
        int n = snprintf(buf, (size_t)buf_size,
                         "No major convergence today. The cycles continue "
                         "their slow dance, each system counting its own "
                         "rhythm.");
        if (n > 0) written = n < buf_size ? n : buf_size - 1;
    }

    return written;
}

/* ===================================================================
 * Internal: thread type selector
 * =================================================================== */

static const char *thread_for_type(br_insight_type_t type) {
    switch (type) {
        case BR_INSIGHT_CONVERGENCE:  return BR_THREAD_CONVERGENCE;
        case BR_INSIGHT_NUMBER:       return BR_THREAD_NUMBER;
        case BR_INSIGHT_CYCLE:        return BR_THREAD_CYCLE;
        case BR_INSIGHT_FESTIVAL:     return BR_THREAD_FESTIVAL;
        case BR_INSIGHT_ASTRONOMICAL: return BR_THREAD_ASTRONOMICAL;
        case BR_INSIGHT_QUIET:        return BR_THREAD_QUIET;
        default:                      return BR_THREAD_QUIET;
    }
}

/* ===================================================================
 * Public API
 * =================================================================== */

int br_narrative_headline(const br_insight_t *insight, char *buf, int buf_size) {
    if (!insight || !buf || buf_size <= 0) return 0;
    buf[0] = '\0';

    int n = 0;
    switch (insight->type) {
        case BR_INSIGHT_CONVERGENCE:
            n = headline_convergence(insight, buf, buf_size);
            break;
        case BR_INSIGHT_FESTIVAL:
            n = headline_festival(insight, buf, buf_size);
            break;
        case BR_INSIGHT_NUMBER:
            n = headline_number(insight, buf, buf_size);
            break;
        case BR_INSIGHT_CYCLE:
            n = headline_cycle(insight, buf, buf_size);
            break;
        case BR_INSIGHT_ASTRONOMICAL:
            n = headline_astronomical(insight, buf, buf_size);
            break;
        case BR_INSIGHT_QUIET:
            n = headline_quiet(buf, buf_size);
            break;
        default:
            n = snprintf(buf, (size_t)buf_size, "%s", insight->headline);
            break;
    }

    if (n < 0) return 0;
    return n < buf_size ? n : buf_size - 1;
}

int br_narrative_summary(const br_result_t *result, char *buf, int buf_size) {
    if (!result || !buf || buf_size <= 0) return 0;
    buf[0] = '\0';
    return summary_from_insights(result, buf, buf_size);
}

int br_narrative_compose(const br_result_t *result, br_narrative_t *out) {
    if (!result || !out) return 0;
    memset(out, 0, sizeof(*out));

    /* Determine top insight */
    const br_insight_t *top = NULL;
    if (result->insight_count > 0 && result->top_insight_index >= 0 &&
        result->top_insight_index < result->insight_count) {
        top = &result->insights[result->top_insight_index];
    }

    /* Headline */
    if (top) {
        br_narrative_headline(top, out->headline, BR_HEADLINE_MAX);
    } else {
        headline_quiet(out->headline, BR_HEADLINE_MAX);
    }

    /* Summary: prepend real astronomical/calendar events */
    {
        char events[192];
        int elen = enrich_with_events(result->jd, events, sizeof(events));
        if (elen > 0) {
            /* Write events first, then append narrative summary */
            int pos = snprintf(out->summary, BR_SUMMARY_MAX, "%s ", events);
            if (pos > 0 && pos < BR_SUMMARY_MAX - 1) {
                br_narrative_summary(result,
                                     out->summary + pos,
                                     BR_SUMMARY_MAX - pos);
            }
        } else {
            br_narrative_summary(result, out->summary, BR_SUMMARY_MAX);
        }
    }

    /* Thread type */
    if (top) {
        strncpy(out->thread_type, thread_for_type(top->type),
                BR_THREAD_MAX - 1);
        out->thread_type[BR_THREAD_MAX - 1] = '\0';
    } else {
        strncpy(out->thread_type, BR_THREAD_QUIET, BR_THREAD_MAX - 1);
        out->thread_type[BR_THREAD_MAX - 1] = '\0';
    }

    /* Featured systems — collect from top 3 insights */
    out->system_count = 0;
    for (int i = 0; i < result->insight_count && i < 3; i++) {
        const br_insight_t *ins = &result->insights[i];
        for (int s = 0; s < ins->system_count &&
                        out->system_count < BR_MAX_SYSTEMS; s++) {
            /* Avoid duplicates */
            int dup = 0;
            for (int k = 0; k < out->system_count; k++) {
                if (out->systems_featured[k] == ins->systems[s]) {
                    dup = 1;
                    break;
                }
            }
            if (!dup) {
                out->systems_featured[out->system_count++] = ins->systems[s];
            }
        }
    }

    /* Confidence based on convergence strength */
    out->confidence = result->convergence_strength;

    /* Wisdom ID: use convergence strength to select domain.
     * 0 = generic, positive = domain-matched. Simple heuristic for now. */
    out->wisdom_id = (int)(result->convergence_strength * 10.0);

    return 1;
}
