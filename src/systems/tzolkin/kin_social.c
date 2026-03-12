#include "kin_social.h"
#include "dreamspell.h"

/*
 * Kin Social — Dreamspell kin relationship and compatibility.
 *
 * Relationship detection priority:
 *   1. SAME (kin_a == kin_b)
 *   2. ANALOG (oracle analog partner)
 *   3. ANTIPODE (oracle antipode partner)
 *   4. OCCULT (oracle occult/hidden partner, 261 - kin)
 *   5. GUIDE (oracle guide, if different from destiny)
 *   6. SEAL_TRIBE (same seal, different kin)
 *   7. WAVESPELL (same wavespell group)
 *   8. EARTH_FAMILY (same earth family via seal % 5)
 *   9. COLOR_FAMILY (same color via seal % 4)
 *  10. NONE
 *
 * Source: Arguelles, "Dreamspell" (1990).
 */

/* --- Relationship type names --- */

static const char *REL_TYPE_NAMES[] = {
    "None",
    "Same Kin",
    "Analog",
    "Antipode",
    "Occult",
    "Guide",
    "Hidden Teacher",
    "Wavespell",
    "Seal Tribe",
    "Color Family",
    "Earth Family"
};

/* --- Compatibility scores by relationship type --- */

static const int COMPAT_SCORES[] = {
    0,    /* NONE */
    100,  /* SAME */
    80,   /* ANALOG */
    80,   /* ANTIPODE */
    80,   /* OCCULT */
    80,   /* GUIDE */
    70,   /* HIDDEN */
    35,   /* WAVESPELL */
    40,   /* SEAL_TRIBE */
    15,   /* COLOR_FAMILY */
    25    /* EARTH_FAMILY */
};

/* --- Relationship descriptions --- */

static const char *REL_DESCRIPTIONS[] = {
    "No significant relationship",
    "Same kin — identical galactic signature",
    "Analog partner — supportive, like-minded energy",
    "Antipode partner — challenge and strengthening",
    "Occult partner — hidden power, unexpected support",
    "Guide partner — higher self, teaching energy",
    "Hidden teacher — secret wisdom, 261 complement",
    "Wavespell family — shared purpose cycle",
    "Seal tribe — shared solar seal archetype",
    "Color family — shared creative process",
    "Earth family — shared planetary service"
};

/* --- Internal helpers --- */

static int valid_kin(int kin)
{
    return kin >= 1 && kin <= 260;
}

static int kin_seal(int kin)
{
    return (kin - 1) % 20;
}

static int kin_wavespell_num(int kin)
{
    return (kin - 1) / 13 + 1;
}

static int kin_color(int kin)
{
    return kin_seal(kin) % 4;
}

static int kin_earth_fam(int kin)
{
    return kin_seal(kin) % 5;
}

/* Compute shared group flags */
static void compute_shared(int kin_a, int kin_b,
                           int *seal, int *ws, int *color, int *earth)
{
    *seal  = (kin_seal(kin_a) == kin_seal(kin_b)) ? 1 : 0;
    *ws    = (kin_wavespell_num(kin_a) == kin_wavespell_num(kin_b)) ? 1 : 0;
    *color = (kin_color(kin_a) == kin_color(kin_b)) ? 1 : 0;
    *earth = (kin_earth_fam(kin_a) == kin_earth_fam(kin_b)) ? 1 : 0;
}

/* --- Public API --- */

kin_relationship_t kin_relationship(int kin_a, int kin_b)
{
    kin_relationship_t r;
    r.type = KIN_REL_NONE;
    r.description = REL_DESCRIPTIONS[0];
    r.shared_seal = 0;
    r.shared_wavespell = 0;
    r.shared_color = 0;
    r.shared_earth_family = 0;
    r.compatibility_score = 0;

    if (!valid_kin(kin_a) || !valid_kin(kin_b))
        return r;

    /* Compute shared groups */
    compute_shared(kin_a, kin_b,
                   &r.shared_seal, &r.shared_wavespell,
                   &r.shared_color, &r.shared_earth_family);

    /* Priority 1: SAME */
    if (kin_a == kin_b) {
        r.type = KIN_REL_SAME;
        r.description = REL_DESCRIPTIONS[KIN_REL_SAME];
        r.compatibility_score = COMPAT_SCORES[KIN_REL_SAME];
        return r;
    }

    /* Priority 2-5: Oracle positions */
    dreamspell_oracle_t o = dreamspell_oracle(kin_a);

    if (kin_b == o.analog) {
        r.type = KIN_REL_ANALOG;
        r.description = REL_DESCRIPTIONS[KIN_REL_ANALOG];
        r.compatibility_score = COMPAT_SCORES[KIN_REL_ANALOG];
        return r;
    }

    if (kin_b == o.antipode) {
        r.type = KIN_REL_ANTIPODE;
        r.description = REL_DESCRIPTIONS[KIN_REL_ANTIPODE];
        r.compatibility_score = COMPAT_SCORES[KIN_REL_ANTIPODE];
        return r;
    }

    if (kin_b == o.occult) {
        r.type = KIN_REL_OCCULT;
        r.description = REL_DESCRIPTIONS[KIN_REL_OCCULT];
        r.compatibility_score = COMPAT_SCORES[KIN_REL_OCCULT];
        return r;
    }

    if (kin_b == o.guide) {
        r.type = KIN_REL_GUIDE;
        r.description = REL_DESCRIPTIONS[KIN_REL_GUIDE];
        r.compatibility_score = COMPAT_SCORES[KIN_REL_GUIDE];
        return r;
    }

    /* Priority 6: Seal tribe (same seal, not oracle partner) */
    if (r.shared_seal) {
        r.type = KIN_REL_SEAL_TRIBE;
        r.description = REL_DESCRIPTIONS[KIN_REL_SEAL_TRIBE];
        r.compatibility_score = COMPAT_SCORES[KIN_REL_SEAL_TRIBE];
        return r;
    }

    /* Priority 7: Wavespell */
    if (r.shared_wavespell) {
        r.type = KIN_REL_WAVESPELL;
        r.description = REL_DESCRIPTIONS[KIN_REL_WAVESPELL];
        r.compatibility_score = COMPAT_SCORES[KIN_REL_WAVESPELL];
        return r;
    }

    /* Priority 8: Earth family */
    if (r.shared_earth_family) {
        r.type = KIN_REL_EARTH_FAMILY;
        r.description = REL_DESCRIPTIONS[KIN_REL_EARTH_FAMILY];
        r.compatibility_score = COMPAT_SCORES[KIN_REL_EARTH_FAMILY];
        return r;
    }

    /* Priority 9: Color family */
    if (r.shared_color) {
        r.type = KIN_REL_COLOR_FAMILY;
        r.description = REL_DESCRIPTIONS[KIN_REL_COLOR_FAMILY];
        r.compatibility_score = COMPAT_SCORES[KIN_REL_COLOR_FAMILY];
        return r;
    }

    /* Priority 10: NONE */
    return r;
}

int kin_tribe_members(int seal, int *out, int max)
{
    if (seal < 0 || seal > 19 || max <= 0)
        return 0;

    int count = 0;
    /* Seal s appears at kins: s+1, s+21, s+41, ..., s+241 */
    for (int i = 0; i < 13 && count < max; i++) {
        out[count++] = seal + 1 + i * 20;
    }
    return count;
}

int kin_wavespell_members(int wavespell, int *out, int max)
{
    if (wavespell < 1 || wavespell > 20 || max <= 0)
        return 0;

    int count = 0;
    int start = (wavespell - 1) * 13 + 1;
    for (int i = 0; i < 13 && count < max; i++) {
        out[count++] = start + i;
    }
    return count;
}

int kin_color_family(int color, int *out, int max)
{
    if (color < 0 || color > 3 || max <= 0)
        return 0;

    int count = 0;
    /* 5 seals per color: seal = color, color+4, color+8, color+12, color+16 */
    for (int si = 0; si < 5 && count < max; si++) {
        int seal = color + si * 4;
        /* Each seal has 13 kins */
        for (int i = 0; i < 13 && count < max; i++) {
            out[count++] = seal + 1 + i * 20;
        }
    }
    return count;
}

int kin_earth_family_members(int family, int *out, int max)
{
    if (family < 0 || family > 4 || max <= 0)
        return 0;

    /* Earth family members are seals where seal % 5 == family.
     * That gives seals: family, family+5, family+10, family+15. */
    int count = 0;
    for (int si = 0; si < 4 && count < max; si++) {
        int seal = family + si * 5;
        for (int i = 0; i < 13 && count < max; i++) {
            out[count++] = seal + 1 + i * 20;
        }
    }
    return count;
}

int kin_compatibility_score(int kin_a, int kin_b)
{
    kin_relationship_t r = kin_relationship(kin_a, kin_b);
    return r.compatibility_score;
}

const char *kin_rel_type_name(kin_rel_type_t type)
{
    if (type < 0 || type >= KIN_REL_TYPE_COUNT)
        return "?";
    return REL_TYPE_NAMES[type];
}

int kin_shared_groups(int kin_a, int kin_b)
{
    if (!valid_kin(kin_a) || !valid_kin(kin_b))
        return 0;

    int seal, ws, color, earth;
    compute_shared(kin_a, kin_b, &seal, &ws, &color, &earth);
    return seal + ws + color + earth;
}
