/* tzolkin_today.c — Full Tzolkin "Today" page builder.
 *
 * Assembles all Dreamspell subsystems into a single page struct
 * suitable for display rendering.
 *
 * Pure: no globals, no malloc, no GL, no side effects.
 */

#include "tzolkin_today.h"
#include "tzolkin.h"
#include "dreamspell.h"
#include "tzolkin_board.h"
#include "haab.h"
#include "cr_cycle.h"
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Internal helpers                                                    */
/* ------------------------------------------------------------------ */

/* Decode seal and tone from a kin number (1-260). */
static int kin_seal(int kin) { return (kin - 1) % 20; }
static int kin_tone(int kin) { return (kin - 1) % 13 + 1; }

/* Format one oracle position line into buf.
 * Example: "Guide: Kin 47 Blue Galactic Hand" */
static int fmt_oracle_line(char *buf, int sz, const char *label, int kin)
{
    int seal = kin_seal(kin);
    int tone = kin_tone(kin);
    dreamspell_color_t col = dreamspell_color(seal);
    return snprintf(buf, (size_t)sz, "%s: Kin %d %s %s %s",
                    label, kin, col.name,
                    tzolkin_tone_name(tone),
                    tzolkin_seal_name(seal));
}

/* ------------------------------------------------------------------ */
/* zt_today                                                            */
/* ------------------------------------------------------------------ */

zt_page_t zt_today(double jd)
{
    zt_page_t p;
    memset(&p, 0, sizeof(p));

    /* Core Tzolkin data */
    tzolkin_day_t day = tzolkin_from_jd(jd);
    p.kin = day.kin;
    p.is_portal = tzolkin_board_is_portal(day.kin) ? 1 : 0;
    p.section_count = 7;
    p.has_data = 1;

    /* Dreamspell lookups */
    dreamspell_color_t  color = dreamspell_color(day.seal);
    dreamspell_tone_t   tone  = dreamspell_tone(day.tone);
    dreamspell_oracle_t orc   = dreamspell_oracle(day.kin);
    dreamspell_wavespell_t ws = dreamspell_wavespell(day.kin);
    dreamspell_castle_t cas   = dreamspell_castle(day.kin);
    dreamspell_family_t fam   = dreamspell_earth_family(day.seal);

    /* Haab / Calendar Round */
    haab_date_t haab = haab_from_jd(jd);

    /* 52-year cycle */
    cr_cycle_t cyc = cr_cycle_from_jd(jd);
    int days_rem   = cr_cycle_days_remaining(jd);

    /* ---- page_title ---- */
    snprintf(p.page_title, ZT_TITLE_MAX,
             "Kin %d — %s %s %s",
             day.kin, color.name,
             tzolkin_tone_name(day.tone),
             tzolkin_seal_name(day.seal));

    /* ---- kin_section ---- */
    snprintf(p.kin_section, ZT_SECTION_MAX,
             "Seal: %s — %s, %s\n"
             "Tone: %d %s — %s, %s, %s\n"
             "Color: %s — %s, %s",
             tzolkin_seal_name(day.seal),
             tone.action, tone.power,
             day.tone, tzolkin_tone_name(day.tone),
             tone.action, tone.power,
             tone.name ? tone.name : "?",
             color.name, color.action, color.quality);

    /* ---- oracle_section ---- */
    {
        char guide[128], analog[128], anti[128], occ[128];
        fmt_oracle_line(guide,  (int)sizeof(guide),  "Guide",    orc.guide);
        fmt_oracle_line(analog, (int)sizeof(analog), "Analog",   orc.analog);
        fmt_oracle_line(anti,   (int)sizeof(anti),   "Antipode", orc.antipode);
        fmt_oracle_line(occ,    (int)sizeof(occ),    "Occult",   orc.occult);
        snprintf(p.oracle_section, ZT_SECTION_MAX,
                 "%s\n%s\n%s\n%s",
                 guide, analog, anti, occ);
    }

    /* ---- wavespell_section ---- */
    {
        int pos = dreamspell_wavespell_position(day.kin);
        snprintf(p.wavespell_section, ZT_SECTION_MAX,
                 "Wavespell %d: %s — %s\n"
                 "Day %d of 13 | Action: %s | Power: %s",
                 ws.number,
                 ws.purpose ? ws.purpose : "?",
                 ws.purpose ? ws.purpose : "?",
                 pos,
                 ws.action ? ws.action : "?",
                 ws.power  ? ws.power  : "?");
    }

    /* ---- castle_section ---- */
    snprintf(p.castle_section, ZT_SECTION_MAX,
             "Castle: %s (%s) — %s\n"
             "Kin range: %d-%d",
             cas.name  ? cas.name  : "?",
             cas.color ? cas.color : "?",
             cas.theme ? cas.theme : "?",
             cas.start_kin, cas.end_kin);

    /* ---- family_section ---- */
    snprintf(p.family_section, ZT_SECTION_MAX,
             "Earth Family: %s (%s)\n"
             "Members: %s, %s, %s, %s",
             fam.name      ? fam.name      : "?",
             fam.direction  ? fam.direction  : "?",
             tzolkin_seal_name(fam.members[0]),
             tzolkin_seal_name(fam.members[1]),
             tzolkin_seal_name(fam.members[2]),
             tzolkin_seal_name(fam.members[3]));

    /* ---- round_section ---- */
    {
        char haab_str[64];
        haab_fmt(haab, haab_str, sizeof(haab_str));
        snprintf(p.round_section, ZT_SECTION_MAX,
                 "Tzolkin: Kin %d %s %s\n"
                 "Haab: %s\n"
                 "Calendar Round: Kin %d / %s",
                 day.kin,
                 tzolkin_tone_name(day.tone),
                 tzolkin_seal_name(day.seal),
                 haab_str,
                 day.kin, haab_str);
    }

    /* ---- cycle_section ---- */
    snprintf(p.cycle_section, ZT_SECTION_MAX,
             "52-Year Cycle: Year %d of 52\n"
             "Day %d of 18,980\n"
             "%d days remaining in this round",
             cyc.year_in_round + 1,
             cyc.day_in_round + 1,
             days_rem);

    return p;
}

/* ------------------------------------------------------------------ */
/* zt_format                                                           */
/* ------------------------------------------------------------------ */

int zt_format(const zt_page_t *page, char *buf, int buf_size)
{
    if (!page || !buf || buf_size <= 0) {
        return 0;
    }

    int n = snprintf(buf, (size_t)buf_size,
                     "=== %s ===\n"
                     "%s\n\n"
                     "--- Kin ---\n%s\n\n"
                     "--- Oracle ---\n%s\n\n"
                     "--- Wavespell ---\n%s\n\n"
                     "--- Castle ---\n%s\n\n"
                     "--- Earth Family ---\n%s\n\n"
                     "--- Calendar Round ---\n%s\n\n"
                     "--- 52-Year Cycle ---\n%s\n",
                     page->page_title,
                     page->is_portal
                         ? "*** Galactic Activation Portal Day ***"
                         : "",
                     page->kin_section,
                     page->oracle_section,
                     page->wavespell_section,
                     page->castle_section,
                     page->family_section,
                     page->round_section,
                     page->cycle_section);

    if (n < 0) {
        buf[0] = '\0';
        return 0;
    }

    /* snprintf may have truncated — return actual chars in buffer */
    if (n >= buf_size) {
        return buf_size - 1;
    }
    return n;
}
