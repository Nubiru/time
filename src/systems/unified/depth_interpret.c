/* depth_interpret.c — Unified interpretation dispatcher
 *
 * Routes interpretation requests to system-specific modules and
 * normalizes results into depth_interp_t format.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "depth_interpret.h"
#include "../hebrew/hebrew_interpret.h"
#include "../islamic/islamic_interpret.h"
#include "../buddhist/buddhist_interpret.h"
#include "../hindu/hindu_interpret.h"
#include "../iching/iching_interpret.h"
#include "../kabbalah/kabbalah_interpret.h"
#include "../../ui/content_i18n.h"
#include <string.h>
#include <stdio.h>

/* ===================================================================
 * Internal helpers
 * =================================================================== */

/* Content keys for system names — enables locale-aware display.
 * Falls back to English array when no translation found. */
static const char *di_system_keys[TS_SYS_COUNT] = {
    "sys.name.gregorian", "sys.name.tzolkin", "sys.name.haab",
    "sys.name.chinese", "sys.name.hebrew", "sys.name.islamic",
    "sys.name.buddhist", "sys.name.hindu", "sys.name.iching",
    "sys.name.astrology", "sys.name.human_design", "sys.name.kabbalah",
    "sys.name.coptic", "sys.name.ethiopian", "sys.name.persian",
    "sys.name.japanese", "sys.name.korean", "sys.name.thai",
    "sys.name.geological", "sys.name.cosmic", "sys.name.earth"
};

static const char *di_system_fallback[TS_SYS_COUNT] = {
    "Gregorian", "Tzolkin", "Haab", "Chinese", "Hebrew",
    "Islamic", "Buddhist", "Hindu", "I Ching", "Astrology",
    "Human Design", "Kabbalah", "Coptic", "Ethiopian", "Persian",
    "Japanese", "Korean", "Thai", "Geological", "Cosmic", "Earth"
};

static const char *di_system_name(int system_id, i18n_locale_t locale)
{
    if (system_id < 0 || system_id >= TS_SYS_COUNT) {
        return "?";
    }
    const char *name = content_get(di_system_keys[system_id], locale);
    /* content_get returns the key itself when not found */
    if (name == di_system_keys[system_id]) {
        return di_system_fallback[system_id];
    }
    return name;
}

static depth_interp_t make_empty(int system_id)
{
    depth_interp_t r;
    memset(&r, 0, sizeof(r));
    r.system_id = system_id;
    r.has_data = 0;
    return r;
}

static depth_interp_t make_unsupported(int system_id, i18n_locale_t locale)
{
    depth_interp_t r = make_empty(system_id);
    const char *name = di_system_name(system_id, locale);
    snprintf(r.glyph, sizeof(r.glyph), "-");
    snprintf(r.glance, sizeof(r.glance), "%s: %s", name,
             content_get("depth.unsupported.glance", locale));
    snprintf(r.detail, sizeof(r.detail), "%s %s",
             name, content_get("depth.unsupported.detail", locale));
    return r;
}

static depth_interp_t make_invalid(int system_id, i18n_locale_t locale)
{
    depth_interp_t r = make_empty(system_id);
    snprintf(r.glyph, sizeof(r.glyph), "?");
    snprintf(r.glance, sizeof(r.glance), "%s",
             content_get("depth.invalid.glance", locale));
    snprintf(r.detail, sizeof(r.detail), "%s %d.",
             content_get("depth.invalid.detail", locale), system_id);
    return r;
}

/* ===================================================================
 * System-specific dispatch functions
 * =================================================================== */

static depth_interp_t interpret_hebrew(const di_params_t *p)
{
    if (p->hebrew_month == 0) {
        return make_empty(TS_SYS_HEBREW);
    }

    hebrew_date_t date;
    date.year = p->hebrew_year;
    date.month = p->hebrew_month;
    date.day = p->hebrew_day;

    hebrew_interp_t hi = hi_interpret_locale(date, p->hebrew_sabbatical,
                                             p->hebrew_is_leap, p->locale);

    depth_interp_t r;
    memset(&r, 0, sizeof(r));
    r.system_id = TS_SYS_HEBREW;
    r.has_data = 1;
    snprintf(r.glyph, sizeof(r.glyph), "%s", hi.glyph);
    snprintf(r.glance, sizeof(r.glance), "%s", hi.glance);
    snprintf(r.detail, sizeof(r.detail), "%s", hi.detail);
    return r;
}

static depth_interp_t interpret_islamic(const di_params_t *p)
{
    if (p->islamic_month == 0) {
        return make_empty(TS_SYS_ISLAMIC);
    }

    hijri_date_t date;
    date.year = p->islamic_year;
    date.month = p->islamic_month;
    date.day = p->islamic_day;

    islamic_interp_t ii = isi_interpret_locale(date, p->locale);

    depth_interp_t r;
    memset(&r, 0, sizeof(r));
    r.system_id = TS_SYS_ISLAMIC;
    r.has_data = 1;
    snprintf(r.glyph, sizeof(r.glyph), "%s", ii.glyph);
    snprintf(r.glance, sizeof(r.glance), "%s", ii.glance);
    snprintf(r.detail, sizeof(r.detail), "%s", ii.detail);
    return r;
}

static depth_interp_t interpret_buddhist(const di_params_t *p)
{
    if (p->buddhist_year == 0) {
        return make_empty(TS_SYS_BUDDHIST);
    }

    buddhist_interp_t bi = bi_interpret_locale(
        p->buddhist_year,
        p->buddhist_month,
        (uposatha_type_t)p->buddhist_uposatha,
        (kalpa_phase_t)p->buddhist_kalpa_phase,
        p->locale
    );

    depth_interp_t r;
    memset(&r, 0, sizeof(r));
    r.system_id = TS_SYS_BUDDHIST;
    r.has_data = 1;
    snprintf(r.glyph, sizeof(r.glyph), "%s", bi.glyph);
    snprintf(r.glance, sizeof(r.glance), "%s", bi.glance);
    snprintf(r.detail, sizeof(r.detail), "%s", bi.detail);
    return r;
}

static depth_interp_t interpret_hindu(const di_params_t *p)
{
    if (p->hindu_tithi == 0) {
        return make_empty(TS_SYS_HINDU);
    }

    hindu_interp_t hi = hndi_interpret_locale(
        p->hindu_tithi,
        p->hindu_nakshatra,
        p->hindu_yoga,
        p->locale
    );

    depth_interp_t r;
    memset(&r, 0, sizeof(r));
    r.system_id = TS_SYS_HINDU;
    r.has_data = 1;
    snprintf(r.glyph, sizeof(r.glyph), "%s", hi.glyph);
    snprintf(r.glance, sizeof(r.glance), "%s", hi.glance);
    snprintf(r.detail, sizeof(r.detail), "%s", hi.detail);
    return r;
}

static depth_interp_t interpret_iching(const di_params_t *p)
{
    if (p->iching_king_wen == 0) {
        return make_empty(TS_SYS_ICHING);
    }

    iching_interp_t ii = ii_interpret_locale(
        p->iching_king_wen,
        p->iching_upper_tri,
        p->iching_lower_tri,
        p->locale
    );

    depth_interp_t r;
    memset(&r, 0, sizeof(r));
    r.system_id = TS_SYS_ICHING;
    r.has_data = 1;
    snprintf(r.glyph, sizeof(r.glyph), "%s", ii.glyph);
    snprintf(r.glance, sizeof(r.glance), "%s", ii.glance);
    snprintf(r.detail, sizeof(r.detail), "%s", ii.detail);
    return r;
}

static depth_interp_t interpret_kabbalah(const di_params_t *p)
{
    if (p->kabbalah_sefirah < 0) {
        return make_empty(TS_SYS_KABBALAH);
    }

    kabbalah_interp_t ki = ki_interpret_sefirah_locale(
        p->kabbalah_sefirah,
        p->kabbalah_planet,
        p->locale
    );

    depth_interp_t r;
    memset(&r, 0, sizeof(r));
    r.system_id = TS_SYS_KABBALAH;
    r.has_data = 1;
    snprintf(r.glyph, sizeof(r.glyph), "%s", ki.glyph);
    snprintf(r.glance, sizeof(r.glance), "%s", ki.glance);
    snprintf(r.detail, sizeof(r.detail), "%s", ki.detail);
    return r;
}

/* ===================================================================
 * Public API
 * =================================================================== */

depth_interp_t depth_interpret(int system_id, const di_params_t *params)
{
    if (params == NULL) {
        return make_empty(system_id);
    }

    if (system_id < 0 || system_id >= TS_SYS_COUNT) {
        return make_invalid(system_id, params->locale);
    }

    switch ((ts_system_t)system_id) {
    case TS_SYS_HEBREW:   return interpret_hebrew(params);
    case TS_SYS_ISLAMIC:  return interpret_islamic(params);
    case TS_SYS_BUDDHIST: return interpret_buddhist(params);
    case TS_SYS_HINDU:    return interpret_hindu(params);
    case TS_SYS_ICHING:   return interpret_iching(params);
    case TS_SYS_KABBALAH: return interpret_kabbalah(params);
    default:              return make_unsupported(system_id, params->locale);
    }
}

di_params_t depth_interpret_default_params(void)
{
    di_params_t p;
    memset(&p, 0, sizeof(p));
    p.kabbalah_sefirah = -1;
    return p;
}

int depth_interpret_supported(int system_id)
{
    switch (system_id) {
    case TS_SYS_HEBREW:
    case TS_SYS_ISLAMIC:
    case TS_SYS_BUDDHIST:
    case TS_SYS_HINDU:
    case TS_SYS_ICHING:
    case TS_SYS_KABBALAH:
        return 1;
    default:
        return 0;
    }
}

int depth_interpret_supported_count(void)
{
    return 6;
}
