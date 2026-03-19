/* cosmic_fingerprint.c — Display-ready multi-system identity from birth data.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects
 * (except static buffer in cf_system_name). */

#include "cosmic_fingerprint.h"

#include <stdio.h>
#include <string.h>

/* ---- system names ---- */

static const char *SYSTEM_NAMES[CF_SYS_COUNT] = {
    "Gregorian",
    "Astrology",
    "Chinese",
    "Tzolkin",
    "I Ching",
    "Human Design",
    "Hebrew",
    "Islamic",
    "Buddhist"
};

/* Reveal order: familiar first, exotic later, calendar dates last. */
static const cf_system_t REVEAL_ORDER[CF_SYS_COUNT] = {
    CF_SYS_GREGORIAN,
    CF_SYS_ASTROLOGY,
    CF_SYS_CHINESE,
    CF_SYS_TZOLKIN,
    CF_SYS_ICHING,
    CF_SYS_HUMAN_DESIGN,
    CF_SYS_HEBREW,
    CF_SYS_ISLAMIC,
    CF_SYS_BUDDHIST
};

/* Month names for Gregorian headline. */
static const char *MONTH_NAMES[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

/* ---- entry builders ---- */

static cf_entry_t build_gregorian(const birth_profile_t *p)
{
    cf_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = CF_SYS_GREGORIAN;
    e.system_name = SYSTEM_NAMES[CF_SYS_GREGORIAN];
    e.available = 1;

    const char *month = (p->month >= 1 && p->month <= 12)
        ? MONTH_NAMES[p->month - 1] : "?";
    snprintf(e.headline, CF_HEADLINE_MAX, "%s %d, %d", month, p->day, p->year);
    snprintf(e.subtitle, CF_SUBTITLE_MAX, "Day %d of the year", p->day_of_year);
    e.glyph_id = p->month - 1; /* month index as glyph */
    return e;
}

static cf_entry_t build_astrology(const birth_profile_t *p)
{
    cf_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = CF_SYS_ASTROLOGY;
    e.system_name = SYSTEM_NAMES[CF_SYS_ASTROLOGY];
    e.available = 1;

    snprintf(e.headline, CF_HEADLINE_MAX, "Sun in %s",
             p->astrology.sun_sign_name);
    snprintf(e.subtitle, CF_SUBTITLE_MAX, "%.1f\xc2\xb0 ecliptic longitude",
             p->astrology.sun_longitude);
    e.glyph_id = p->astrology.sun_sign;
    return e;
}

static cf_entry_t build_chinese(const birth_profile_t *p)
{
    cf_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = CF_SYS_CHINESE;
    e.system_name = SYSTEM_NAMES[CF_SYS_CHINESE];
    e.available = 1;

    const char *pol = p->chinese.polarity == 0 ? "Yang" : "Yin";
    snprintf(e.headline, CF_HEADLINE_MAX, "%s %s",
             p->chinese.element_name, p->chinese.animal_name);
    snprintf(e.subtitle, CF_SUBTITLE_MAX, "%s, Stem %d, Branch %d",
             pol, p->chinese.stem, p->chinese.branch);
    e.glyph_id = p->chinese.animal;
    return e;
}

static cf_entry_t build_tzolkin(const birth_profile_t *p)
{
    cf_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = CF_SYS_TZOLKIN;
    e.system_name = SYSTEM_NAMES[CF_SYS_TZOLKIN];
    e.available = 1;

    snprintf(e.headline, CF_HEADLINE_MAX, "%s %s",
             p->tzolkin.tone_name, p->tzolkin.seal_name);
    snprintf(e.subtitle, CF_SUBTITLE_MAX, "Kin %d, Wavespell %d, Castle %d",
             p->tzolkin.kin, p->tzolkin.wavespell, p->tzolkin.castle);
    e.glyph_id = p->tzolkin.seal;
    return e;
}

static cf_entry_t build_iching(const birth_profile_t *p)
{
    cf_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = CF_SYS_ICHING;
    e.system_name = SYSTEM_NAMES[CF_SYS_ICHING];
    e.available = 1;

    snprintf(e.headline, CF_HEADLINE_MAX, "%s", p->iching.name);
    snprintf(e.subtitle, CF_SUBTITLE_MAX, "Hexagram %d, King Wen sequence",
             p->iching.king_wen);
    e.glyph_id = p->iching.king_wen;
    return e;
}

static cf_entry_t build_human_design(const birth_profile_t *p)
{
    cf_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = CF_SYS_HUMAN_DESIGN;
    e.system_name = SYSTEM_NAMES[CF_SYS_HUMAN_DESIGN];
    e.available = 1;

    snprintf(e.headline, CF_HEADLINE_MAX, "Gate %d — %s",
             p->astrology.hd_sun_gate, p->astrology.hd_sun_gate_name);
    snprintf(e.subtitle, CF_SUBTITLE_MAX, "Sun Gate %d, Earth Gate %d",
             p->astrology.hd_sun_gate, p->astrology.hd_earth_gate);
    e.glyph_id = p->astrology.hd_sun_gate;
    return e;
}

static cf_entry_t build_hebrew(const birth_profile_t *p)
{
    cf_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = CF_SYS_HEBREW;
    e.system_name = SYSTEM_NAMES[CF_SYS_HEBREW];
    e.available = 1;

    snprintf(e.headline, CF_HEADLINE_MAX, "%d %s %d",
             p->hebrew.day, p->hebrew.month_name, p->hebrew.year);
    snprintf(e.subtitle, CF_SUBTITLE_MAX, "Year %d in the Hebrew calendar",
             p->hebrew.year);
    e.glyph_id = p->hebrew.month - 1;
    return e;
}

static cf_entry_t build_islamic(const birth_profile_t *p)
{
    cf_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = CF_SYS_ISLAMIC;
    e.system_name = SYSTEM_NAMES[CF_SYS_ISLAMIC];
    e.available = 1;

    snprintf(e.headline, CF_HEADLINE_MAX, "%d %s %d AH",
             p->islamic.day, p->islamic.month_name, p->islamic.year);
    snprintf(e.subtitle, CF_SUBTITLE_MAX, "Year %d AH",
             p->islamic.year);
    e.glyph_id = p->islamic.month - 1;
    return e;
}

static cf_entry_t build_buddhist(const birth_profile_t *p)
{
    cf_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = CF_SYS_BUDDHIST;
    e.system_name = SYSTEM_NAMES[CF_SYS_BUDDHIST];
    e.available = 1;

    snprintf(e.headline, CF_HEADLINE_MAX, "Year %d", p->buddhist.era_year);
    snprintf(e.subtitle, CF_SUBTITLE_MAX, "BE %d in the Buddhist Era",
             p->buddhist.era_year);
    e.glyph_id = 0;
    return e;
}

/* ---- public API ---- */

cf_fingerprint_t cf_compute(const birth_profile_t *profile)
{
    cf_fingerprint_t fp;
    memset(&fp, 0, sizeof(fp));

    if (!profile) return fp;

    fp.birth_year  = profile->year;
    fp.birth_month = profile->month;
    fp.birth_day   = profile->day;

    fp.entries[CF_SYS_GREGORIAN]    = build_gregorian(profile);
    fp.entries[CF_SYS_ASTROLOGY]    = build_astrology(profile);
    fp.entries[CF_SYS_CHINESE]      = build_chinese(profile);
    fp.entries[CF_SYS_TZOLKIN]      = build_tzolkin(profile);
    fp.entries[CF_SYS_ICHING]       = build_iching(profile);
    fp.entries[CF_SYS_HUMAN_DESIGN] = build_human_design(profile);
    fp.entries[CF_SYS_HEBREW]       = build_hebrew(profile);
    fp.entries[CF_SYS_ISLAMIC]      = build_islamic(profile);
    fp.entries[CF_SYS_BUDDHIST]     = build_buddhist(profile);

    /* Assign reveal orders */
    for (int i = 0; i < CF_SYS_COUNT; i++) {
        fp.entries[REVEAL_ORDER[i]].reveal_order = i + 1;
    }

    fp.count = CF_SYS_COUNT;
    return fp;
}

const cf_entry_t *cf_entry(const cf_fingerprint_t *fp, cf_system_t system)
{
    if (!fp || (int)system < 0 || system >= CF_SYS_COUNT)
        return NULL;
    if (!fp->entries[system].available)
        return NULL;
    return &fp->entries[system];
}

const char *cf_headline(const cf_fingerprint_t *fp, cf_system_t system)
{
    const cf_entry_t *e = cf_entry(fp, system);
    if (!e) return "";
    return e->headline;
}

const char *cf_system_name(cf_system_t system)
{
    if ((int)system < 0 || system >= CF_SYS_COUNT) return NULL;
    return SYSTEM_NAMES[system];
}

int cf_reveal_order(const cf_fingerprint_t *fp, cf_system_t *out_systems,
                    int max)
{
    if (!fp || !out_systems || max <= 0) return 0;

    int n = 0;
    for (int i = 0; i < CF_SYS_COUNT && n < max; i++) {
        if (fp->entries[REVEAL_ORDER[i]].available) {
            out_systems[n++] = REVEAL_ORDER[i];
        }
    }
    return n;
}

int cf_format_summary(const cf_fingerprint_t *fp, char *buf, int buf_size)
{
    if (!fp || !buf || buf_size <= 0) return 0;

    int pos = 0;
    int rem = buf_size;

    for (int i = 0; i < CF_SYS_COUNT && rem > 1; i++) {
        cf_system_t sys = REVEAL_ORDER[i];
        const cf_entry_t *e = &fp->entries[sys];
        if (!e->available) continue;

        int written = snprintf(buf + pos, (size_t)rem, "%s: %s\n",
                               e->system_name, e->headline);
        if (written < 0) break;
        if (written >= rem) {
            pos = buf_size - 1;
            buf[pos] = '\0';
            break;
        }
        pos += written;
        rem -= written;
    }

    return pos;
}

int cf_available_count(const cf_fingerprint_t *fp)
{
    if (!fp) return 0;
    int n = 0;
    for (int i = 0; i < CF_SYS_COUNT; i++) {
        if (fp->entries[i].available) n++;
    }
    return n;
}
