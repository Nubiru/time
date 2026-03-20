/* birth_sky.c — Birth moment visual state.
 *
 * Bridges birth_profile_t to the visual layer: ring highlights,
 * featured system selection, card content for birth display.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "birth_sky.h"
#include "../systems/unified/today_summary.h"
#include "../systems/human_design/human_design.h"
#include "../systems/astronomy/planets.h"
#include "../systems/astrology/aspects.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

/* Natal chart orb: wider than live aspects (standard is 8-10 degrees) */
#define NATAL_ORB 10.0

/* ---- helpers ---- */

static void add_featured(birth_sky_t *bs, int system_id)
{
    if (bs->featured_count >= BS_MAX_FEATURED) {
        return;
    }
    /* avoid duplicates */
    for (int i = 0; i < bs->featured_count; i++) {
        if (bs->featured[i] == system_id) {
            return;
        }
    }
    bs->featured[bs->featured_count++] = system_id;
}

static birth_card_t empty_card(void)
{
    birth_card_t card;
    memset(&card, 0, sizeof(card));
    return card;
}

/* ---- public API ---- */

birth_sky_t birth_sky_from_profile(const birth_profile_t *profile)
{
    birth_sky_t bs;
    memset(&bs, 0, sizeof(bs));

    if (!profile) {
        return bs;
    }

    bs.jd = profile->jd;
    bs.active = true;

    /* Compute ring indices at birth moment.
     * Moon longitude unknown from date-only profile — use 0.0. */
    bs.ring_indices = ring_today_compute(profile->jd,
                                          profile->astrology.sun_longitude,
                                          0.0);

    /* Always featured: top 3 + I Ching */
    add_featured(&bs, TS_SYS_TZOLKIN);
    add_featured(&bs, TS_SYS_ASTROLOGY);
    add_featured(&bs, TS_SYS_CHINESE);
    add_featured(&bs, TS_SYS_ICHING);

    /* Conditionally add Human Design if gate data present */
    if (profile->astrology.hd_sun_gate > 0) {
        add_featured(&bs, TS_SYS_HUMAN_DESIGN);
    }

    /* Conditionally add Hebrew if month data present */
    if (profile->hebrew.month > 0) {
        add_featured(&bs, TS_SYS_HEBREW);
    }

    /* Compute natal planet positions and aspects */
    {
        solar_system_t ss = planets_at(profile->jd);
        /* Convert heliocentric to approximate geocentric longitudes.
         * Sun = Earth helio + 180°, others ≈ helio (first-order approx). */
        double geo[8];
        for (int i = 0; i < 8; i++) {
            geo[i] = ss.positions[i].longitude;
        }
        /* Sun's geocentric longitude = Earth's helio longitude + 180° */
        double sun_geo = fmod(ss.positions[PLANET_EARTH].longitude + 180.0, 360.0);
        /* Shift: slot 0 = Sun (replacing Mercury's helio for natal chart).
         * Standard natal: Sun, Moon(placeholder), Mercury..Neptune.
         * Use Earth slot for Sun position since natal charts are geocentric. */
        geo[PLANET_EARTH] = sun_geo; /* Earth slot → Sun geocentric */

        memcpy(bs.geo_longitudes, geo, sizeof(geo));
        bs.natal_aspects = aspects_find_all(geo, NATAL_ORB);
    }

    return bs;
}

birth_sky_t birth_sky_empty(void)
{
    birth_sky_t bs;
    memset(&bs, 0, sizeof(bs));
    bs.active = false;
    /* Initialize all ring indices to -1 */
    for (int i = 0; i < RT_MAX_SYSTEMS; i++) {
        bs.ring_indices.indices[i] = -1;
    }
    return bs;
}

bool birth_sky_is_valid(const birth_sky_t *bs)
{
    if (!bs) {
        return false;
    }
    return bs->active && bs->jd > 0.0;
}

birth_card_t birth_card_tzolkin(const birth_profile_t *profile)
{
    birth_card_t card = empty_card();

    if (!profile) {
        return card;
    }

    const bp_tzolkin_t *tz = &profile->tzolkin;

    snprintf(card.title, sizeof(card.title),
             "Kin %d -- %s %s",
             tz->kin, tz->seal_name, tz->tone_name);

    snprintf(card.line1, sizeof(card.line1),
             "Wavespell %d | Castle %d",
             tz->wavespell, tz->castle);

    snprintf(card.line2, sizeof(card.line2),
             "Guide: Kin %d | Analog: Kin %d",
             tz->guide_kin, tz->analog_kin);

    snprintf(card.line3, sizeof(card.line3),
             "Antipode: Kin %d | Occult: Kin %d",
             tz->antipode_kin, tz->occult_kin);

    snprintf(card.detail, sizeof(card.detail),
             "Your Galactic Signature is %s %s, Kin %d.",
             tz->seal_name, tz->tone_name, tz->kin);

    return card;
}

birth_card_t birth_card_astrology(const birth_profile_t *profile)
{
    birth_card_t card = empty_card();

    if (!profile) {
        return card;
    }

    const bp_astrology_t *ast = &profile->astrology;

    snprintf(card.title, sizeof(card.title),
             "%s Sun", ast->sun_sign_name);

    snprintf(card.line1, sizeof(card.line1),
             "Sun longitude: %.1f%s",
             ast->sun_longitude, "\xc2\xb0");

    snprintf(card.line2, sizeof(card.line2),
             "HD Sun Gate %d: %s",
             ast->hd_sun_gate,
             ast->hd_sun_gate_name ? ast->hd_sun_gate_name : "?");

    snprintf(card.line3, sizeof(card.line3),
             "HD Earth Gate %d",
             ast->hd_earth_gate);

    snprintf(card.detail, sizeof(card.detail),
             "Your birth Sun is in %s at %.1f degrees.",
             ast->sun_sign_name, ast->sun_longitude);

    return card;
}

birth_card_t birth_card_chinese(const birth_profile_t *profile)
{
    birth_card_t card = empty_card();

    if (!profile) {
        return card;
    }

    const bp_chinese_t *ch = &profile->chinese;

    snprintf(card.title, sizeof(card.title),
             "%s %s",
             ch->element_name, ch->animal_name);

    /* cycle_year not directly in bp_chinese_t — derive from stem/branch.
     * sexagenary index = (stem * 12 + branch) mod 60, but the canonical
     * formula for the cycle position from stem (0-9) and branch (0-11)
     * is: ((stem - branch + 60) % 10 gives nothing useful).
     * Correct: cycle_year = ((stem % 10) * 6 + (branch % 12)) ... no.
     * Actually the sexagenary cycle number is computable but birth_profile
     * doesn't store cycle_year. We use stem and branch directly. */
    int cycle_pos = ((ch->stem % 10) - (ch->branch % 12) + 60) % 10;
    /* Simpler: just show stem/branch pairing */
    (void)cycle_pos;
    snprintf(card.line1, sizeof(card.line1),
             "Stem %d, Branch %d of 60-year cycle",
             ch->stem, ch->branch);

    snprintf(card.line2, sizeof(card.line2),
             "Stem %d | Branch %d",
             ch->stem, ch->branch);

    snprintf(card.line3, sizeof(card.line3),
             "Polarity: %s",
             ch->polarity == 0 ? "Yang" : "Yin");

    snprintf(card.detail, sizeof(card.detail),
             "Born in the year of the %s %s.",
             ch->element_name, ch->animal_name);

    return card;
}

birth_card_t birth_card_iching(const birth_profile_t *profile)
{
    birth_card_t card = empty_card();

    if (!profile) {
        return card;
    }

    const bp_iching_t *ic = &profile->iching;

    snprintf(card.title, sizeof(card.title),
             "Hexagram %d: %s",
             ic->king_wen, ic->name);

    snprintf(card.line1, sizeof(card.line1),
             "King Wen sequence #%d",
             ic->king_wen);

    card.line2[0] = '\0';
    card.line3[0] = '\0';

    snprintf(card.detail, sizeof(card.detail),
             "Your birth hexagram is %s (#%d).",
             ic->name, ic->king_wen);

    return card;
}

birth_card_t birth_card_for_system(const birth_profile_t *profile,
                                    int system_id)
{
    if (!profile) {
        return empty_card();
    }

    switch (system_id) {
    case TS_SYS_TZOLKIN:
        return birth_card_tzolkin(profile);
    case TS_SYS_ASTROLOGY:
        return birth_card_astrology(profile);
    case TS_SYS_CHINESE:
        return birth_card_chinese(profile);
    case TS_SYS_ICHING:
        return birth_card_iching(profile);
    default:
        return empty_card();
    }
}

const char *birth_sky_top_aspect(const birth_sky_t *bs, char *buf, int buf_size)
{
    if (!bs || !buf || buf_size < 1) {
        if (buf && buf_size > 0) buf[0] = '\0';
        return buf ? buf : "";
    }
    if (bs->natal_aspects.count == 0) {
        buf[0] = '\0';
        return buf;
    }

    /* Find the tightest aspect (smallest orb) */
    int best = 0;
    for (int i = 1; i < bs->natal_aspects.count; i++) {
        if (bs->natal_aspects.aspects[i].orb <
            bs->natal_aspects.aspects[best].orb) {
            best = i;
        }
    }

    const aspect_t *a = &bs->natal_aspects.aspects[best];
    snprintf(buf, (size_t)buf_size, "%s %s %s (%.1f%s orb)",
             planet_name(a->planet_a),
             aspect_symbol(a->type),
             planet_name(a->planet_b),
             a->orb, "\xC2\xB0"); /* degree sign UTF-8 */

    return buf;
}

int birth_sky_aspect_count(const birth_sky_t *bs, aspect_type_t type)
{
    if (!bs) return 0;
    int count = 0;
    for (int i = 0; i < bs->natal_aspects.count; i++) {
        if (bs->natal_aspects.aspects[i].type == type) {
            count++;
        }
    }
    return count;
}
