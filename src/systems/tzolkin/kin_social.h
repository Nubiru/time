#ifndef TIME_KIN_SOCIAL_H
#define TIME_KIN_SOCIAL_H

/* Kin Social Compatibility — Dreamspell kin-to-kin relationships.
 *
 * Given two kin numbers (1-260), detects all oracle relationships
 * (analog, antipode, occult, guide, hidden teacher, same kin) and
 * social groupings (wavespell, seal tribe, color family, earth family,
 * castle), returning them as a scored match result.
 *
 * Oracle relationship rules from dreamspell_oracle():
 *   Analog:   oracle.analog match — strength 90
 *   Antipode: oracle.antipode match — strength 80
 *   Occult:   oracle.occult match — strength 85
 *   Guide:    oracle.guide match — strength 95
 *   Hidden:   oracle.hidden match — strength 85
 *   Same Kin: kin_a == kin_b — strength 100
 *
 * Social groupings:
 *   Wavespell:    same 13-day wavespell — strength 70
 *   Seal Tribe:   same seal (kin-1)%20 — strength 60
 *   Color Family: same color seal%4 — strength 50
 *   Earth Family: dreamspell_earth_family() — strength 55
 *   Castle:       same castle (52 kins) — strength 30
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 */

#define KS_MAX_RELATIONSHIPS 6
#define KS_WAVESPELL_SIZE 13
#define KS_TRIBE_SIZE 13
#define KS_COLOR_FAMILY_SIZE 65

typedef enum {
    KS_REL_NONE = 0,
    KS_REL_ANALOG,       /* supportive partner */
    KS_REL_ANTIPODE,     /* challenge/strengthening */
    KS_REL_OCCULT,       /* hidden power — complements to 261 */
    KS_REL_GUIDE,        /* guide relationship — same color, tone-dependent */
    KS_REL_HIDDEN,       /* hidden teacher — 261 minus kin */
    KS_REL_SAME_KIN,     /* identical kin number */
    KS_REL_WAVESPELL,    /* same 13-day wavespell */
    KS_REL_TRIBE,        /* same seal */
    KS_REL_COLOR,        /* same color family */
    KS_REL_EARTH_FAMILY, /* same earth family */
    KS_REL_CASTLE        /* same castle */
} ks_relationship_type_t;

typedef struct {
    ks_relationship_type_t type;
    const char *name;
    const char *description;
    int strength;            /* 0-100 */
} ks_relationship_t;

typedef struct {
    int count;
    ks_relationship_t items[KS_MAX_RELATIONSHIPS];
    int total_score;         /* aggregate 0-100 */
} ks_match_t;

/* Compute all relationships between two kins (1-260).
 * Returns match with count=0 and total_score=0 for invalid inputs.
 * Oracle relationships appear first, social groupings add bonus.
 * total_score = highest oracle strength + social bonuses, capped at 100. */
ks_match_t ks_match(int kin_a, int kin_b);

/* Human-readable name for a relationship type.
 * Returns "?" for invalid type. */
const char *ks_relationship_name(ks_relationship_type_t type);

/* Human-readable meaning/description for a relationship type.
 * Returns "?" for invalid type. */
const char *ks_relationship_meaning(ks_relationship_type_t type);

/* List all kins sharing the given seal (0-19).
 * Writes up to max kin numbers into out_kins. Returns count written.
 * Each seal has 13 members. Returns 0 for invalid seal. */
int ks_tribe_members(int seal, int *out_kins, int max);

/* List all kins in the given wavespell (1-20).
 * Writes up to max kin numbers into out_kins. Returns count written.
 * Each wavespell has 13 members. Returns 0 for invalid wavespell. */
int ks_wavespell_members(int wavespell, int *out_kins, int max);

/* List all kins sharing the given color (0-3: Red/White/Blue/Yellow).
 * Writes up to max kin numbers into out_kins. Returns count written.
 * Each color has 65 members. Returns 0 for invalid color. */
int ks_color_family_members(int color, int *out_kins, int max);

/* Check if two kins share the same wavespell.
 * Returns 1 if yes, 0 if no or invalid. */
int ks_same_wavespell(int kin_a, int kin_b);

/* Check if two kins share the same seal (tribe).
 * Returns 1 if yes, 0 if no or invalid. */
int ks_same_tribe(int kin_a, int kin_b);

/* Check if two kins share the same color.
 * Returns 1 if yes, 0 if no or invalid. */
int ks_same_color(int kin_a, int kin_b);

/* Check if two kins share the same earth family.
 * Returns 1 if yes, 0 if no or invalid. */
int ks_same_earth_family(int kin_a, int kin_b);

/* Check if two kins share the same castle.
 * Returns 1 if yes, 0 if no or invalid. */
int ks_same_castle(int kin_a, int kin_b);

/* Extract seal index (0-19) from kin (1-260).
 * Returns -1 for invalid kin. */
int ks_seal_from_kin(int kin);

/* Extract color index (0-3) from kin (1-260).
 * Returns -1 for invalid kin. */
int ks_color_from_kin(int kin);

/* Number of relationship types (excluding NONE).
 * Returns count of valid enum values after NONE. */
int ks_relationship_type_count(void);

#endif /* TIME_KIN_SOCIAL_H */
