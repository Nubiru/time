#include "kin_earth_family.h"
#include "../systems/tzolkin/dreamspell.h"
#include <string.h>

/* Pentacle vertex positions (regular pentagon, top-first, clockwise).
 * Matches family order: Cardinal(0), Core(1), Signal(2), Gateway(3), Polar(4). */
static const float PENTACLE_X[5] = { 0.50f, 0.88f, 0.74f, 0.26f, 0.12f };
static const float PENTACLE_Y[5] = { 0.10f, 0.38f, 0.85f, 0.85f, 0.38f };

kin_earth_family_t kin_family_compute(int today_seal)
{
    kin_earth_family_t ef;
    memset(&ef, 0, sizeof(ef));

    if (today_seal < 0 || today_seal > 19)
        return ef;

    ef.today_seal = today_seal;
    ef.today_family = today_seal % 5;

    /* Build each family node.
     * Seals 0-4 each belong to a different family (seal % 5),
     * so dreamspell_earth_family(i) for i=0..4 gives all 5 families. */
    for (int i = 0; i < KIN_FAMILY_COUNT; i++) {
        dreamspell_family_t df = dreamspell_earth_family(i);

        ef.families[i].name = df.name;
        ef.families[i].direction = df.direction;
        ef.families[i].seals[0] = df.members[0];
        ef.families[i].seals[1] = df.members[1];
        ef.families[i].seals[2] = df.members[2];
        ef.families[i].seals[3] = df.members[3];

        ef.families[i].x = PENTACLE_X[i];
        ef.families[i].y = PENTACLE_Y[i];

        /* Check if today's seal is in this family */
        ef.families[i].contains_today = 0;
        for (int m = 0; m < KIN_FAMILY_MEMBERS; m++) {
            if (df.members[m] == today_seal) {
                ef.families[i].contains_today = 1;
                break;
            }
        }
    }

    return ef;
}

int kin_family_for_seal(int seal)
{
    if (seal < 0 || seal > 19)
        return -1;
    return seal % 5;
}

kin_earth_family_t kin_family_from_kin(int kin)
{
    if (kin < 1 || kin > 260) {
        kin_earth_family_t ef;
        memset(&ef, 0, sizeof(ef));
        return ef;
    }
    int seal = (kin - 1) % 20;
    return kin_family_compute(seal);
}
