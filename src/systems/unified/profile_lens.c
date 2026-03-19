/* profile_lens.c — Same birth identity through 4 narrative frames.
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "profile_lens.h"

#include <stdio.h>
#include <string.h>

/* ===== Static Data ===== */

static const char *const s_lens_names[PL_LENS_COUNT] = {
    "Your Pattern",
    "Your Chart",
    "Your Calendar",
    "Your Energy"
};

static const char *const s_lens_subtitles[PL_LENS_COUNT] = {
    "Personality, tendencies, and archetypal themes",
    "Astronomical positions and celestial cycles",
    "Your birth across the world's calendar traditions",
    "Gates, seals, and hexagrams of your inner landscape"
};

/* Which systems each lens includes, in presentation order */
static const cf_system_t PATTERN_SYSTEMS[] = {
    CF_SYS_ASTROLOGY, CF_SYS_TZOLKIN, CF_SYS_ICHING, CF_SYS_CHINESE
};
#define PATTERN_COUNT 4

static const cf_system_t CHART_SYSTEMS[] = {
    CF_SYS_ASTROLOGY, CF_SYS_HUMAN_DESIGN, CF_SYS_GREGORIAN
};
#define CHART_COUNT 3

static const cf_system_t CALENDAR_SYSTEMS[] = {
    CF_SYS_GREGORIAN, CF_SYS_CHINESE, CF_SYS_HEBREW,
    CF_SYS_ISLAMIC, CF_SYS_BUDDHIST
};
#define CALENDAR_COUNT 5

static const cf_system_t ENERGY_SYSTEMS[] = {
    CF_SYS_HUMAN_DESIGN, CF_SYS_TZOLKIN, CF_SYS_ICHING
};
#define ENERGY_COUNT 3

/* ===== Section Title Templates per Lens ===== */

/* Pattern lens titles */
static const char *const PATTERN_TITLES[] = {
    "Sun Sign Archetype",
    "Kin Archetype",
    "Wisdom Pattern",
    "Elemental Nature"
};

/* Chart lens titles */
static const char *const CHART_TITLES[] = {
    "Solar Position",
    "Design Gates",
    "Birth Coordinates"
};

/* Calendar lens titles */
static const char *const CALENDAR_TITLES[] = {
    "Western Date",
    "Chinese Year",
    "Hebrew Date",
    "Islamic Date",
    "Buddhist Era"
};

/* Energy lens titles */
static const char *const ENERGY_TITLES[] = {
    "Life Force Gate",
    "Galactic Signature",
    "Hexagram Wisdom"
};

/* ===== Internal: Build a section ===== */

static pl_section_t build_section(const cf_entry_t *entry,
                                   const char *title) {
    pl_section_t sec;
    memset(&sec, 0, sizeof(sec));

    if (!entry || !entry->available) return sec;

    sec.system = entry->system;
    snprintf(sec.title, PL_TITLE_MAX, "%s", title);
    snprintf(sec.body, PL_BODY_MAX, "%s — %s",
             entry->headline, entry->subtitle);
    return sec;
}

/* ===== Internal: Compose per lens ===== */

static pl_view_t compose_lens(const cf_fingerprint_t *fp,
                               pl_lens_t lens,
                               const cf_system_t *systems,
                               int system_count,
                               const char *const *titles) {
    pl_view_t v;
    memset(&v, 0, sizeof(v));

    v.lens = lens;
    snprintf(v.title, PL_TITLE_MAX, "%s", s_lens_names[lens]);
    snprintf(v.subtitle, PL_BODY_MAX, "%s", s_lens_subtitles[lens]);

    v.section_count = 0;
    for (int i = 0; i < system_count && v.section_count < PL_MAX_SECTIONS; i++) {
        cf_system_t sys = systems[i];
        const cf_entry_t *entry = cf_entry(fp, sys);
        if (!entry) continue;

        v.sections[v.section_count] = build_section(entry, titles[i]);
        if (v.sections[v.section_count].title[0] != '\0') {
            v.section_count++;
        }
    }

    return v;
}

/* ===== Public API ===== */

const char *pl_lens_name(pl_lens_t lens) {
    if ((int)lens < 0 || lens >= PL_LENS_COUNT) return "?";
    return s_lens_names[lens];
}

const char *pl_lens_subtitle(pl_lens_t lens) {
    if ((int)lens < 0 || lens >= PL_LENS_COUNT) return "?";
    return s_lens_subtitles[lens];
}

int pl_lens_count(void) {
    return PL_LENS_COUNT;
}

pl_view_t pl_compose(const cf_fingerprint_t *fp, pl_lens_t lens) {
    pl_view_t v;
    memset(&v, 0, sizeof(v));

    if (!fp || (int)lens < 0 || lens >= PL_LENS_COUNT) return v;

    switch (lens) {
        case PL_LENS_PATTERN:
            return compose_lens(fp, lens, PATTERN_SYSTEMS,
                                PATTERN_COUNT, PATTERN_TITLES);
        case PL_LENS_CHART:
            return compose_lens(fp, lens, CHART_SYSTEMS,
                                CHART_COUNT, CHART_TITLES);
        case PL_LENS_CALENDAR:
            return compose_lens(fp, lens, CALENDAR_SYSTEMS,
                                CALENDAR_COUNT, CALENDAR_TITLES);
        case PL_LENS_ENERGY:
            return compose_lens(fp, lens, ENERGY_SYSTEMS,
                                ENERGY_COUNT, ENERGY_TITLES);
        default:
            return v;
    }
}

int pl_format_view(const pl_view_t *view, char *buf, int buf_size) {
    if (!buf || buf_size <= 0) return 0;
    buf[0] = '\0';
    if (!view) return 0;

    int pos = 0;
    int rem = buf_size;

    /* Title */
    int n = snprintf(buf + pos, (size_t)rem, "%s\n%s\n\n",
                     view->title, view->subtitle);
    if (n < 0) return 0;
    if (n >= rem) { buf[buf_size - 1] = '\0'; return buf_size - 1; }
    pos += n;
    rem -= n;

    /* Sections */
    for (int i = 0; i < view->section_count && rem > 1; i++) {
        n = snprintf(buf + pos, (size_t)rem, "%s\n  %s\n",
                     view->sections[i].title, view->sections[i].body);
        if (n < 0) break;
        if (n >= rem) {
            pos = buf_size - 1;
            buf[pos] = '\0';
            break;
        }
        pos += n;
        rem -= n;
    }

    return pos;
}
