/* brain_depth.c — Layered depth content implementation.
 *
 * Provides tiered content for each knowledge system at a given JD.
 * Uses today_summary's ts_compute to get current values, then
 * formats at the requested depth tier.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_depth.h"
#include <string.h>
#include <stdio.h>

/* ===================================================================
 * Internal: content generators per system
 *
 * Each returns chars written. Implementations grow as more systems
 * are fully wired. Currently covers: Gregorian, Tzolkin, Hebrew,
 * Islamic, Chinese, Astrology, I Ching, Buddhist.
 * =================================================================== */

static int fmt_glyph(const ts_entry_t *entry, char *buf, int buf_size) {
    if (!entry || !entry->active) {
        return snprintf(buf, (size_t)buf_size, "-");
    }
    /* Extract first word/symbol from date_str */
    const char *src = entry->date_str;
    int i = 0;
    while (i < buf_size - 1 && src[i] != '\0' && src[i] != ' ' &&
           i < BR_DEPTH_GLYPH_MAX - 1) {
        buf[i] = src[i];
        i++;
    }
    buf[i] = '\0';
    return i;
}

static int fmt_glance(const ts_entry_t *entry, char *buf, int buf_size) {
    if (!entry || !entry->active) {
        return snprintf(buf, (size_t)buf_size, "No data");
    }
    return snprintf(buf, (size_t)buf_size, "%s", entry->date_str);
}

static int fmt_card(const ts_entry_t *entry, const char *system_name,
                    char *buf, int buf_size) {
    if (!entry || !entry->active) {
        return snprintf(buf, (size_t)buf_size, "%s: No data available",
                        system_name);
    }
    if (entry->note[0] != '\0') {
        return snprintf(buf, (size_t)buf_size, "%s: %s. %s",
                        system_name, entry->date_str, entry->note);
    }
    return snprintf(buf, (size_t)buf_size, "%s: %s",
                    system_name, entry->date_str);
}

static int fmt_panel(const ts_entry_t *entry, const char *system_name,
                     ts_system_t system, char *buf, int buf_size) {
    if (!entry || !entry->active) {
        return snprintf(buf, (size_t)buf_size,
                        "%s: Data not available for this date.", system_name);
    }

    int written = 0;
    int remaining = buf_size;

    /* Date line */
    int n = snprintf(buf + written, (size_t)remaining,
                     "%s: %s.", system_name, entry->date_str);
    if (n > 0) { int u = n < remaining ? n : remaining - 1; written += u; remaining -= u; }

    /* Note if present */
    if (entry->note[0] != '\0') {
        n = snprintf(buf + written, (size_t)remaining, " %s.", entry->note);
        if (n > 0) { int u = n < remaining ? n : remaining - 1; written += u; remaining -= u; }
    }

    /* System-specific context */
    switch (system) {
        case TS_SYS_TZOLKIN:
            n = snprintf(buf + written, (size_t)remaining,
                         " The Tzolkin is a 260-day sacred calendar "
                         "(20 day signs x 13 tones).");
            break;
        case TS_SYS_HEBREW:
            n = snprintf(buf + written, (size_t)remaining,
                         " The Hebrew calendar is lunisolar, using "
                         "a 19-year Metonic cycle for intercalation.");
            break;
        case TS_SYS_ISLAMIC:
            n = snprintf(buf + written, (size_t)remaining,
                         " The Islamic calendar is purely lunar (354 days), "
                         "drifting through the seasons.");
            break;
        case TS_SYS_CHINESE:
            n = snprintf(buf + written, (size_t)remaining,
                         " The Chinese calendar combines a 60-year "
                         "sexagenary cycle with lunisolar months.");
            break;
        case TS_SYS_ICHING:
            n = snprintf(buf + written, (size_t)remaining,
                         " The I Ching maps 64 hexagrams (6-line binary "
                         "figures) to natural processes.");
            break;
        case TS_SYS_ASTROLOGY:
            n = snprintf(buf + written, (size_t)remaining,
                         " Western astrology divides the ecliptic into "
                         "12 zodiac signs of 30 degrees each.");
            break;
        case TS_SYS_BUDDHIST:
            n = snprintf(buf + written, (size_t)remaining,
                         " The Buddhist calendar counts from the "
                         "Parinirvana of the Buddha (543 BCE).");
            break;
        default:
            n = 0;
            break;
    }
    if (n > 0) { int u = n < remaining ? n : remaining - 1; written += u; remaining -= u; }

    return written;
}

static int fmt_board(const ts_entry_t *entry, const char *system_name,
                     ts_system_t system, char *buf, int buf_size) {
    /* Board = panel content + cycle parameters */
    int written = fmt_panel(entry, system_name, system, buf, buf_size);
    int remaining = buf_size - written;
    int n;

    switch (system) {
        case TS_SYS_TZOLKIN:
            n = snprintf(buf + written, (size_t)remaining,
                         " Cycle: 260 days. Part of 52-year Calendar Round "
                         "(18,980 days). 5 Great Cycles = ~25,627 years "
                         "(near precession of 25,772 years).");
            break;
        case TS_SYS_HEBREW:
            n = snprintf(buf + written, (size_t)remaining,
                         " Metonic cycle: 19 years (235 synodic months). "
                         "Sabbatical: 7 years. Jubilee: 50 years. "
                         "Epoch: 3761 BCE (Anno Mundi).");
            break;
        case TS_SYS_ISLAMIC:
            n = snprintf(buf + written, (size_t)remaining,
                         " 30-year cycle with 11 leap years. "
                         "Epoch: 622 CE (Hijra). No intercalation. "
                         "Months drift ~11 days/year vs solar.");
            break;
        case TS_SYS_CHINESE:
            n = snprintf(buf + written, (size_t)remaining,
                         " 10 Heavenly Stems x 12 Earthly Branches = "
                         "60-year cycle. 12 zodiac animals. "
                         "5 elements (Wood, Fire, Earth, Metal, Water).");
            break;
        case TS_SYS_ICHING:
            n = snprintf(buf + written, (size_t)remaining,
                         " 8 trigrams (2^3). 64 hexagrams (2^6). "
                         "King Wen sequence. Binary encoding parallels "
                         "64 DNA codons (Schonberger mapping).");
            break;
        case TS_SYS_ASTROLOGY:
            n = snprintf(buf + written, (size_t)remaining,
                         " 12 signs. 10 classical + modern planets. "
                         "12 houses. Aspects: conjunction, opposition, "
                         "trine, square, sextile.");
            break;
        case TS_SYS_BUDDHIST:
            n = snprintf(buf + written, (size_t)remaining,
                         " Buddhist Era (BE) = Gregorian + 543. "
                         "Uposatha on quarter-moon days. "
                         "Vesak: full moon of Visakha month.");
            break;
        default:
            n = snprintf(buf + written, (size_t)remaining,
                         " Full system data at this scale.");
            break;
    }
    if (n > 0) { int u = n < remaining ? n : remaining - 1; written += u; }

    return written;
}

/* ===================================================================
 * Systems with depth content
 * =================================================================== */

static const ts_system_t DEPTH_SYSTEMS[] = {
    TS_SYS_GREGORIAN, TS_SYS_TZOLKIN, TS_SYS_HEBREW,
    TS_SYS_ISLAMIC, TS_SYS_CHINESE, TS_SYS_ASTROLOGY,
    TS_SYS_ICHING, TS_SYS_BUDDHIST
};

static const int DEPTH_SYSTEM_COUNT =
    (int)(sizeof(DEPTH_SYSTEMS) / sizeof(DEPTH_SYSTEMS[0]));

/* ===================================================================
 * Public API
 * =================================================================== */

int br_depth_max_len(depth_tier_t tier) {
    switch (tier) {
        case DEPTH_TIER_GLYPH:  return BR_DEPTH_GLYPH_MAX;
        case DEPTH_TIER_GLANCE: return BR_DEPTH_GLANCE_MAX;
        case DEPTH_TIER_CARD:   return BR_DEPTH_CARD_MAX;
        case DEPTH_TIER_PANEL:  return BR_DEPTH_PANEL_MAX;
        case DEPTH_TIER_BOARD:  return BR_DEPTH_BOARD_MAX;
        default:                return 0;
    }
}

int br_depth_has_system(ts_system_t system) {
    for (int i = 0; i < DEPTH_SYSTEM_COUNT; i++) {
        if (DEPTH_SYSTEMS[i] == system) return 1;
    }
    return 0;
}

int br_depth_system_count(void) {
    return DEPTH_SYSTEM_COUNT;
}

int br_depth_get(double jd, ts_system_t system, depth_tier_t tier,
                 br_depth_content_t *out) {
    if (!out) return 0;
    memset(out, 0, sizeof(*out));
    out->tier = tier;
    out->system = system;

    if (tier < 0 || tier >= DEPTH_TIER_COUNT) return 0;

    /* Get today's entry for this system */
    ts_summary_t summary = ts_compute(jd);
    ts_entry_t entry = ts_get_system(&summary, system);
    const char *sys_name = ts_system_name(system);

    int len = 0;
    switch (tier) {
        case DEPTH_TIER_GLYPH:
            len = fmt_glyph(&entry, out->content, BR_DEPTH_GLYPH_MAX);
            break;
        case DEPTH_TIER_GLANCE:
            len = fmt_glance(&entry, out->content, BR_DEPTH_GLANCE_MAX);
            break;
        case DEPTH_TIER_CARD:
            len = fmt_card(&entry, sys_name, out->content, BR_DEPTH_CARD_MAX);
            break;
        case DEPTH_TIER_PANEL:
            len = fmt_panel(&entry, sys_name, system, out->content,
                            BR_DEPTH_PANEL_MAX);
            break;
        case DEPTH_TIER_BOARD:
            len = fmt_board(&entry, sys_name, system, out->content,
                            BR_DEPTH_BOARD_MAX);
            break;
        default:
            return 0;
    }

    out->content_len = len < 0 ? 0 : len;
    return 1;
}
