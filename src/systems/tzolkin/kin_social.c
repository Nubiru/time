#include "kin_social.h"
#include "dreamspell.h"

/*
 * Kin Social Compatibility — Dreamspell kin-to-kin relationships.
 *
 * Detects all oracle relationships and social groupings between
 * two kins, returning them as a multi-item match result.
 *
 * Oracle strengths:
 *   Same Kin = 100, Guide = 95, Analog = 90,
 *   Occult = 85, Hidden = 85, Antipode = 80
 *
 * Social grouping bonuses (added to highest oracle score):
 *   Wavespell = +10, Tribe = +8, Earth Family = +7,
 *   Color = +5, Castle = +5
 *
 * Total capped at 100.
 *
 * Source: Arguelles, "Dreamspell" (1990).
 */

/* --- Relationship type names --- */

static const char *REL_NAMES[] = {
    "None",
    "Analog",
    "Antipode",
    "Occult",
    "Guide",
    "Hidden Teacher",
    "Same Kin",
    "Wavespell",
    "Tribe",
    "Color Family",
    "Earth Family",
    "Castle"
};

#define REL_NAME_COUNT 12

/* --- Relationship descriptions --- */

static const char *REL_MEANINGS[] = {
    "No significant relationship",
    "Supportive partner — like-minded energy",
    "Challenge and strengthening partner",
    "Hidden power — unexpected support, complements to 261",
    "Guide — higher self, teaching energy, same color",
    "Hidden teacher — secret wisdom, 261 complement",
    "Same kin — identical galactic signature",
    "Wavespell family — shared 13-day purpose cycle",
    "Seal tribe — shared solar seal archetype across the Tzolkin",
    "Color family — shared creative process (Red/White/Blue/Yellow)",
    "Earth family — shared planetary service group",
    "Castle — shared 52-day court of development"
};

/* --- Oracle relationship strengths --- */

static const int ORACLE_STRENGTHS[] = {
    0,    /* NONE */
    90,   /* ANALOG */
    80,   /* ANTIPODE */
    85,   /* OCCULT */
    95,   /* GUIDE */
    85,   /* HIDDEN */
    100   /* SAME_KIN */
};

/* --- Social grouping bonus points --- */

#define BONUS_WAVESPELL    10
#define BONUS_TRIBE         8
#define BONUS_EARTH_FAMILY  7
#define BONUS_COLOR         5
#define BONUS_CASTLE        5

/* --- Internal helpers --- */

static int valid_kin(int kin)
{
    return kin >= 1 && kin <= 260;
}

static int seal_of(int kin)
{
    return (kin - 1) % 20;
}

static int color_of(int kin)
{
    return seal_of(kin) % 4;
}

static int wavespell_of(int kin)
{
    return (kin - 1) / 13 + 1;
}

static int earth_family_of(int kin)
{
    return seal_of(kin) % 5;
}

static int castle_of(int kin)
{
    return (kin - 1) / 52 + 1;
}

static void add_item(ks_match_t *m, ks_relationship_type_t type, int strength)
{
    if (m->count >= KS_MAX_RELATIONSHIPS)
        return;
    m->items[m->count].type = type;
    m->items[m->count].name = REL_NAMES[type];
    m->items[m->count].description = REL_MEANINGS[type];
    m->items[m->count].strength = strength;
    m->count++;
}

/* --- Public API --- */

ks_match_t ks_match(int kin_a, int kin_b)
{
    ks_match_t m;
    m.count = 0;
    m.total_score = 0;
    for (int i = 0; i < KS_MAX_RELATIONSHIPS; i++) {
        m.items[i].type = KS_REL_NONE;
        m.items[i].name = REL_NAMES[0];
        m.items[i].description = REL_MEANINGS[0];
        m.items[i].strength = 0;
    }

    if (!valid_kin(kin_a) || !valid_kin(kin_b))
        return m;

    int highest_oracle = 0;
    int social_bonus = 0;

    /* --- Oracle relationships --- */

    if (kin_a == kin_b) {
        add_item(&m, KS_REL_SAME_KIN, ORACLE_STRENGTHS[KS_REL_SAME_KIN]);
        if (ORACLE_STRENGTHS[KS_REL_SAME_KIN] > highest_oracle)
            highest_oracle = ORACLE_STRENGTHS[KS_REL_SAME_KIN];
    } else {
        dreamspell_oracle_t o = dreamspell_oracle(kin_a);

        if (kin_b == o.analog) {
            add_item(&m, KS_REL_ANALOG, ORACLE_STRENGTHS[KS_REL_ANALOG]);
            if (ORACLE_STRENGTHS[KS_REL_ANALOG] > highest_oracle)
                highest_oracle = ORACLE_STRENGTHS[KS_REL_ANALOG];
        }

        if (kin_b == o.antipode) {
            add_item(&m, KS_REL_ANTIPODE, ORACLE_STRENGTHS[KS_REL_ANTIPODE]);
            if (ORACLE_STRENGTHS[KS_REL_ANTIPODE] > highest_oracle)
                highest_oracle = ORACLE_STRENGTHS[KS_REL_ANTIPODE];
        }

        /* occult and hidden are the same in dreamspell.c (261-kin),
         * so we only add OCCULT to avoid duplication */
        if (kin_b == o.occult) {
            add_item(&m, KS_REL_OCCULT, ORACLE_STRENGTHS[KS_REL_OCCULT]);
            if (ORACLE_STRENGTHS[KS_REL_OCCULT] > highest_oracle)
                highest_oracle = ORACLE_STRENGTHS[KS_REL_OCCULT];
        }

        /* Guide: only if different from destiny (self-guiding tones 1,6,11 skip) */
        if (kin_b == o.guide && o.guide != kin_a) {
            /* Avoid duplicate if guide == analog or guide == antipode etc */
            if (kin_b != o.analog && kin_b != o.antipode && kin_b != o.occult) {
                add_item(&m, KS_REL_GUIDE, ORACLE_STRENGTHS[KS_REL_GUIDE]);
                if (ORACLE_STRENGTHS[KS_REL_GUIDE] > highest_oracle)
                    highest_oracle = ORACLE_STRENGTHS[KS_REL_GUIDE];
            }
        }
    }

    /* --- Social groupings (accumulated as bonuses) --- */

    if (seal_of(kin_a) == seal_of(kin_b) && kin_a != kin_b) {
        add_item(&m, KS_REL_TRIBE, 60);
        social_bonus += BONUS_TRIBE;
    }

    if (wavespell_of(kin_a) == wavespell_of(kin_b) && kin_a != kin_b) {
        add_item(&m, KS_REL_WAVESPELL, 70);
        social_bonus += BONUS_WAVESPELL;
    }

    if (earth_family_of(kin_a) == earth_family_of(kin_b) && kin_a != kin_b) {
        add_item(&m, KS_REL_EARTH_FAMILY, 55);
        social_bonus += BONUS_EARTH_FAMILY;
    }

    if (color_of(kin_a) == color_of(kin_b) && kin_a != kin_b) {
        add_item(&m, KS_REL_COLOR, 50);
        social_bonus += BONUS_COLOR;
    }

    if (castle_of(kin_a) == castle_of(kin_b) && kin_a != kin_b) {
        add_item(&m, KS_REL_CASTLE, 30);
        social_bonus += BONUS_CASTLE;
    }

    /* --- Total score --- */

    if (m.count > 0) {
        if (highest_oracle > 0) {
            /* Oracle + social bonuses */
            m.total_score = highest_oracle + social_bonus;
        } else {
            /* Social only: use highest social strength */
            int best = 0;
            for (int i = 0; i < m.count; i++) {
                if (m.items[i].strength > best)
                    best = m.items[i].strength;
            }
            m.total_score = best;
        }
        if (m.total_score > 100)
            m.total_score = 100;
    }

    return m;
}

const char *ks_relationship_name(ks_relationship_type_t type)
{
    if (type < 0 || type >= REL_NAME_COUNT)
        return "?";
    return REL_NAMES[type];
}

const char *ks_relationship_meaning(ks_relationship_type_t type)
{
    if (type < 0 || type >= REL_NAME_COUNT)
        return "?";
    return REL_MEANINGS[type];
}

int ks_tribe_members(int seal, int *out_kins, int max)
{
    if (seal < 0 || seal > 19 || max <= 0)
        return 0;

    int count = 0;
    for (int i = 0; i < 13 && count < max; i++) {
        out_kins[count++] = seal + 1 + i * 20;
    }
    return count;
}

int ks_wavespell_members(int wavespell, int *out_kins, int max)
{
    if (wavespell < 1 || wavespell > 20 || max <= 0)
        return 0;

    int count = 0;
    int start = (wavespell - 1) * 13 + 1;
    for (int i = 0; i < 13 && count < max; i++) {
        out_kins[count++] = start + i;
    }
    return count;
}

int ks_color_family_members(int color, int *out_kins, int max)
{
    if (color < 0 || color > 3 || max <= 0)
        return 0;

    int count = 0;
    /* 5 seals per color: seal = color, color+4, color+8, color+12, color+16 */
    for (int si = 0; si < 5 && count < max; si++) {
        int seal = color + si * 4;
        for (int i = 0; i < 13 && count < max; i++) {
            out_kins[count++] = seal + 1 + i * 20;
        }
    }
    return count;
}

int ks_same_wavespell(int kin_a, int kin_b)
{
    if (!valid_kin(kin_a) || !valid_kin(kin_b))
        return 0;
    return wavespell_of(kin_a) == wavespell_of(kin_b) ? 1 : 0;
}

int ks_same_tribe(int kin_a, int kin_b)
{
    if (!valid_kin(kin_a) || !valid_kin(kin_b))
        return 0;
    return seal_of(kin_a) == seal_of(kin_b) ? 1 : 0;
}

int ks_same_color(int kin_a, int kin_b)
{
    if (!valid_kin(kin_a) || !valid_kin(kin_b))
        return 0;
    return color_of(kin_a) == color_of(kin_b) ? 1 : 0;
}

int ks_same_earth_family(int kin_a, int kin_b)
{
    if (!valid_kin(kin_a) || !valid_kin(kin_b))
        return 0;
    return earth_family_of(kin_a) == earth_family_of(kin_b) ? 1 : 0;
}

int ks_same_castle(int kin_a, int kin_b)
{
    if (!valid_kin(kin_a) || !valid_kin(kin_b))
        return 0;
    return castle_of(kin_a) == castle_of(kin_b) ? 1 : 0;
}

int ks_seal_from_kin(int kin)
{
    if (!valid_kin(kin))
        return -1;
    return seal_of(kin);
}

int ks_color_from_kin(int kin)
{
    if (!valid_kin(kin))
        return -1;
    return color_of(kin);
}

int ks_relationship_type_count(void)
{
    /* 11 types: ANALOG through CASTLE (excluding NONE) */
    return KS_REL_CASTLE;  /* == 11 */
}
