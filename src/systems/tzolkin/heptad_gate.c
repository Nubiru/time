#include "heptad_gate.h"
#include <string.h>

/* Heptad Gate data — 7 archetype positions of the weekly cycle.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 *         (Boletin Intergalactico No 7).
 */
static const heptad_gate_t GATES[HEPTAD_GATE_COUNT] = {
    {   /* Day 1 */
        .day            = 1,
        .plasma         = "DALI",
        .seal_code      = 1,
        .seal_name      = "Dragon",
        .hk_number      = 108,
        .circuit        = "Opens Circuit 8",
        .mental_sphere  = "1st Sphere",
        .consciousness  = "Pre-conscious",
        .mantra         = "OM",
        .chakra         = "Crown"
    },
    {   /* Day 2 */
        .day            = 2,
        .plasma         = "SELI",
        .seal_code      = 7,
        .seal_name      = "Hand",
        .hk_number      = 291,
        .circuit        = "Opens Circuit 5",
        .mental_sphere  = "2nd Sphere",
        .consciousness  = "Sub-conscious",
        .mantra         = "HRAM",
        .chakra         = "Root"
    },
    {   /* Day 3 */
        .day            = 3,
        .plasma         = "GAMMA",
        .seal_code      = 2,
        .seal_name      = "Wind",
        .hk_number      = 144,
        .circuit        = "Closes Circuit 1",
        .mental_sphere  = "3rd Sphere",
        .consciousness  = "Conscious",
        .mantra         = "HRAHA",
        .chakra         = "Third Eye"
    },
    {   /* Day 4 */
        .day            = 4,
        .plasma         = "KALI",
        .seal_code      = 13,
        .seal_name      = "Skywalker",
        .hk_number      = 315,
        .circuit        = "Closes Circuit 5",
        .mental_sphere  = "4th Sphere",
        .consciousness  = "Continuous Conscious",
        .mantra         = "HRIM",
        .chakra         = "Sacral"
    },
    {   /* Day 5 */
        .day            = 5,
        .plasma         = "ALFA",
        .seal_code      = 20,
        .seal_name      = "Sun",
        .hk_number      = 414,
        .circuit        = "Opens Circuit 8",
        .mental_sphere  = "5th Sphere",
        .consciousness  = "Super-conscious",
        .mantra         = "HRAUM",
        .chakra         = "Throat"
    },
    {   /* Day 6 */
        .day            = 6,
        .plasma         = "LIMI",
        .seal_code      = 18,
        .seal_name      = "Mirror",
        .hk_number      = 402,
        .circuit        = "Closes Circuit 8",
        .mental_sphere  = "6th Sphere",
        .consciousness  = "Subliminal Conscious",
        .mantra         = "HRUM",
        .chakra         = "Solar Plexus"
    },
    {   /* Day 7 */
        .day            = 7,
        .plasma         = "SILIO",
        .seal_code      = 21,
        .seal_name      = "Hunab Ku",
        .hk_number      = 441,
        .circuit        = "Circuit 11",
        .mental_sphere  = "7th Sphere",
        .consciousness  = "Holomental Perceptor",
        .mantra         = "HRAIM",
        .chakra         = "Heart"
    }
};

heptad_gate_t heptad_gate_for_day(int day_of_week)
{
    if (day_of_week < 1 || day_of_week > HEPTAD_GATE_COUNT) {
        heptad_gate_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return GATES[day_of_week - 1];
}

const char *heptad_gate_plasma_name(int day_of_week)
{
    if (day_of_week < 1 || day_of_week > HEPTAD_GATE_COUNT)
        return NULL;
    return GATES[day_of_week - 1].plasma;
}

const char *heptad_gate_mantra(int day_of_week)
{
    if (day_of_week < 1 || day_of_week > HEPTAD_GATE_COUNT)
        return NULL;
    return GATES[day_of_week - 1].mantra;
}

const char *heptad_gate_chakra(int day_of_week)
{
    if (day_of_week < 1 || day_of_week > HEPTAD_GATE_COUNT)
        return NULL;
    return GATES[day_of_week - 1].chakra;
}

const char *heptad_gate_seal_name(int day_of_week)
{
    if (day_of_week < 1 || day_of_week > HEPTAD_GATE_COUNT)
        return NULL;
    return GATES[day_of_week - 1].seal_name;
}

int heptad_gate_hk_number(int day_of_week)
{
    if (day_of_week < 1 || day_of_week > HEPTAD_GATE_COUNT)
        return 0;
    return GATES[day_of_week - 1].hk_number;
}

int heptad_gate_count(void)
{
    return HEPTAD_GATE_COUNT;
}
