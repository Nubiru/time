#ifndef TIME_KIN_SOCIAL_H
#define TIME_KIN_SOCIAL_H

/* Kin Social — Dreamspell kin relationship and compatibility.
 *
 * Given two kin numbers (1-260), determines their oracle relationship
 * (analog, antipode, occult, guide) and group membership (seal tribe,
 * wavespell, color family, earth family).
 *
 * Priority order for relationship type:
 *   SAME > ANALOG > ANTIPODE > OCCULT > GUIDE >
 *   SEAL_TRIBE > WAVESPELL > EARTH_FAMILY > COLOR_FAMILY > NONE
 *
 * Source: Arguelles, "Dreamspell: The Journey of Timeship Earth 2013" (1990).
 */

typedef enum {
    KIN_REL_NONE = 0,
    KIN_REL_SAME,
    KIN_REL_ANALOG,
    KIN_REL_ANTIPODE,
    KIN_REL_OCCULT,
    KIN_REL_GUIDE,
    KIN_REL_HIDDEN,
    KIN_REL_WAVESPELL,
    KIN_REL_SEAL_TRIBE,
    KIN_REL_COLOR_FAMILY,
    KIN_REL_EARTH_FAMILY,
    KIN_REL_TYPE_COUNT
} kin_rel_type_t;

typedef struct {
    kin_rel_type_t type;
    const char *description;
    int shared_seal;
    int shared_wavespell;
    int shared_color;
    int shared_earth_family;
    int compatibility_score;
} kin_relationship_t;

/* Determine the relationship between two kins (1-260).
 * Returns NONE with score 0 for invalid inputs. */
kin_relationship_t kin_relationship(int kin_a, int kin_b);

/* List all kins sharing the given seal (0-19).
 * Writes up to max kin numbers into out. Returns count written.
 * Each seal has 13 members. Returns 0 for invalid seal. */
int kin_tribe_members(int seal, int *out, int max);

/* List all kins in the given wavespell (1-20).
 * Writes up to max kin numbers into out. Returns count written.
 * Each wavespell has 13 members. Returns 0 for invalid wavespell. */
int kin_wavespell_members(int wavespell, int *out, int max);

/* List all kins sharing the given color (0-3: Red/White/Blue/Yellow).
 * Writes up to max kin numbers into out. Returns count written.
 * Each color has 65 members. Returns 0 for invalid color. */
int kin_color_family(int color, int *out, int max);

/* List all kins sharing the given earth family (0-4).
 * Writes up to max kin numbers into out. Returns count written.
 * Each earth family has 52 members. Returns 0 for invalid family. */
int kin_earth_family_members(int family, int *out, int max);

/* Compute compatibility score between two kins (1-260).
 * Returns 0-100. Returns 0 for invalid inputs. */
int kin_compatibility_score(int kin_a, int kin_b);

/* Human-readable name for a relationship type.
 * Returns "?" for invalid type. */
const char *kin_rel_type_name(kin_rel_type_t type);

/* Count how many groups two kins share (seal, wavespell, color, earth family).
 * Returns 0-4. Returns 0 for invalid inputs. */
int kin_shared_groups(int kin_a, int kin_b);

#endif /* TIME_KIN_SOCIAL_H */
