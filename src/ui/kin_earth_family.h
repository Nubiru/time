#ifndef TIME_KIN_EARTH_FAMILY_H
#define TIME_KIN_EARTH_FAMILY_H

/* Earth Family Pentacle — 5-pointed star of the 5 earth families.
 *
 * Each family contains 4 member seals. Today's seal is highlighted
 * in its family. Families are arranged as a pentacle (regular pentagon).
 *
 * Family mapping (seal % 5):
 *   0 = Cardinal (East):  Dragon(0), WB(5), Monkey(10), Warrior(15)
 *   1 = Core (South):     Wind(1), Hand(6), Human(11), Earth(16)
 *   2 = Signal (West):    Night(2), Star(7), Skywalker(12), Mirror(17)
 *   3 = Gateway (Center): Seed(3), Moon(8), Wizard(13), Storm(18)
 *   4 = Polar (North):    Serpent(4), Dog(9), Eagle(14), Sun(19)
 *
 * Source: Arguelles, "Dreamspell" (1990).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 */

#define KIN_FAMILY_COUNT 5
#define KIN_FAMILY_MEMBERS 4

typedef struct {
    const char *name;           /* "Cardinal", "Core", "Signal", "Gateway", "Polar" */
    const char *direction;      /* from dreamspell_earth_family */
    int seals[KIN_FAMILY_MEMBERS]; /* 4 seal indices */
    float x, y;                 /* position on pentacle */
    int contains_today;         /* 1 if today's seal is in this family */
} kin_family_node_t;

typedef struct {
    kin_family_node_t families[KIN_FAMILY_COUNT];
    int today_seal;             /* 0-19 */
    int today_family;           /* 0-4: which family today's seal belongs to */
} kin_earth_family_t;

/* Compute the earth family pentacle for a seal (0-19).
 * 5 families positioned as a pentacle (5-pointed star) around center (0.5, 0.5):
 *   Node 0 (Cardinal): top          (0.50, 0.10)
 *   Node 1 (Core):     upper-right  (0.88, 0.38)
 *   Node 2 (Signal):   lower-right  (0.74, 0.85)
 *   Node 3 (Gateway):  lower-left   (0.26, 0.85)
 *   Node 4 (Polar):    upper-left   (0.12, 0.38)
 * The family containing today's seal is marked contains_today=1.
 * Returns zeroed struct for invalid seal. */
kin_earth_family_t kin_family_compute(int today_seal);

/* Get which family index (0-4) a seal belongs to.
 * Returns -1 for invalid seal. */
int kin_family_for_seal(int seal);

/* Get earth family from kin number. Extracts seal = (kin-1)%20.
 * Returns zeroed struct for invalid kin. */
kin_earth_family_t kin_family_from_kin(int kin);

#endif /* TIME_KIN_EARTH_FAMILY_H */
