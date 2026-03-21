#include "hunab_ku_21.h"
#include <string.h>

/* Hunab Ku 21 archetype data — 21 positions of the Galactic Tree.
 *
 * Source: Arguelles, "Hunab Ku 21 Playing Board"
 *         Arguelles, "Como Entrar a Hunab Ku 21"
 */
static const hk21_archetype_t ARCHETYPES[HK21_ARCHETYPE_COUNT] = {
    {   /* Seal 1 — Ring 1: Gate of Light */
        .seal_code  = 1,
        .seal_name  = "Dragon",
        .hk_number  = 108,
        .role       = HK21_GATE_OF_LIGHT,
        .domain     = HK21_KNOWLEDGE,
        .planet     = "Neptune",
        .flow       = HK21_GALACTIC_KARMIC,
        .statement  = "Gate of Being"
    },
    {   /* Seal 2 — Ring 1: Gate of Light */
        .seal_code  = 2,
        .seal_name  = "Wind",
        .hk_number  = 144,
        .role       = HK21_GATE_OF_LIGHT,
        .domain     = HK21_PROPHECY,
        .planet     = "Uranus",
        .flow       = HK21_GALACTIC_KARMIC,
        .statement  = "Gate of Spirit"
    },
    {   /* Seal 3 — Ring 1: Gate of Light */
        .seal_code  = 3,
        .seal_name  = "Night",
        .hk_number  = 126,
        .role       = HK21_GATE_OF_LIGHT,
        .domain     = HK21_LOVE,
        .planet     = "Saturn",
        .flow       = HK21_GALACTIC_KARMIC,
        .statement  = "Gate of Dreaming"
    },
    {   /* Seal 4 — Ring 1: Gate of Light */
        .seal_code  = 4,
        .seal_name  = "Seed",
        .hk_number  = 90,
        .role       = HK21_GATE_OF_LIGHT,
        .domain     = HK21_INTELLIGENCE,
        .planet     = "Jupiter",
        .flow       = HK21_GALACTIC_KARMIC,
        .statement  = "Gate of Consciousness"
    },
    {   /* Seal 5 — Ring 2: Gate of Power */
        .seal_code  = 5,
        .seal_name  = "Serpent",
        .hk_number  = 288,
        .role       = HK21_GATE_OF_POWER,
        .domain     = HK21_KNOWLEDGE,
        .planet     = "Maldek",
        .flow       = HK21_GALACTIC_KARMIC,
        .statement  = "Sex initiates Knowledge"
    },
    {   /* Seal 6 — Ring 3: Source of Power */
        .seal_code  = 6,
        .seal_name  = "Worldbridger",
        .hk_number  = 294,
        .role       = HK21_SOURCE_OF_POWER,
        .domain     = HK21_KNOWLEDGE,
        .planet     = "Mars",
        .flow       = HK21_GALACTIC_KARMIC,
        .statement  = "Death is the Source of Knowledge"
    },
    {   /* Seal 7 — Ring 4: Seat of Power */
        .seal_code  = 7,
        .seal_name  = "Monkey",      /* NOTE: HK21 Monkey, not Hand */
        .hk_number  = 291,
        .role       = HK21_SEAT_OF_POWER,
        .domain     = HK21_KNOWLEDGE,
        .planet     = "Earth",
        .flow       = HK21_GALACTIC_KARMIC,
        .statement  = "Cosmic Power of Knowledge"
    },
    {   /* Seal 8 — Ring 2: Gate of Power */
        .seal_code  = 8,
        .seal_name  = "Star",
        .hk_number  = 300,
        .role       = HK21_GATE_OF_POWER,
        .domain     = HK21_LOVE,
        .planet     = "Venus",
        .flow       = HK21_GALACTIC_KARMIC,
        .statement  = "Art initiates Love"
    },
    {   /* Seal 9 — Ring 3: Source of Power */
        .seal_code  = 9,
        .seal_name  = "Moon",
        .hk_number  = 306,
        .role       = HK21_SOURCE_OF_POWER,
        .domain     = HK21_LOVE,
        .planet     = "Mercury",
        .flow       = HK21_GALACTIC_KARMIC,
        .statement  = "Purification is the Source of Love"
    },
    {   /* Seal 10 — Ring 4: Seat of Power */
        .seal_code  = 10,
        .seal_name  = "Dog",
        .hk_number  = 303,
        .role       = HK21_SEAT_OF_POWER,
        .domain     = HK21_LOVE,
        .planet     = "Mercury",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Cosmic Power of Love"
    },
    {   /* Seal 11 — Ring 2: Gate of Power */
        .seal_code  = 11,
        .seal_name  = "Hand",        /* NOTE: HK21 Hand, not Monkey */
        .hk_number  = 312,
        .role       = HK21_GATE_OF_POWER,
        .domain     = HK21_PROPHECY,
        .planet     = "Venus",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Magic initiates Prophecy"
    },
    {   /* Seal 12 — Ring 3: Source of Power */
        .seal_code  = 12,
        .seal_name  = "Human",
        .hk_number  = 318,
        .role       = HK21_SOURCE_OF_POWER,
        .domain     = HK21_PROPHECY,
        .planet     = "Earth",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Wisdom is the Source of Prophecy"
    },
    {   /* Seal 13 — Ring 4: Seat of Power */
        .seal_code  = 13,
        .seal_name  = "Skywalker",
        .hk_number  = 315,
        .role       = HK21_SEAT_OF_POWER,
        .domain     = HK21_PROPHECY,
        .planet     = "Mars",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Cosmic Power of Prophecy"
    },
    {   /* Seal 14 — Ring 2: Gate of Power */
        .seal_code  = 14,
        .seal_name  = "Wizard",
        .hk_number  = 276,
        .role       = HK21_GATE_OF_POWER,
        .domain     = HK21_INTELLIGENCE,
        .planet     = "Maldek",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Timelessness initiates Intelligence"
    },
    {   /* Seal 15 — Ring 3: Source of Power */
        .seal_code  = 15,
        .seal_name  = "Eagle",
        .hk_number  = 282,
        .role       = HK21_SOURCE_OF_POWER,
        .domain     = HK21_INTELLIGENCE,
        .planet     = "Jupiter",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Vision is the Source of Intelligence"
    },
    {   /* Seal 16 — Ring 4: Seat of Power */
        .seal_code  = 16,
        .seal_name  = "Warrior",
        .hk_number  = 279,
        .role       = HK21_SEAT_OF_POWER,
        .domain     = HK21_INTELLIGENCE,
        .planet     = "Saturn",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Cosmic Power of Intelligence"
    },
    {   /* Seal 17 — Ring 5: Portal Matrix */
        .seal_code  = 17,
        .seal_name  = "Earth",
        .hk_number  = 396,
        .role       = HK21_PORTAL_MATRIX,
        .domain     = HK21_UNITY,
        .planet     = "Uranus",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Navigation"
    },
    {   /* Seal 18 — Ring 5: Portal Matrix */
        .seal_code  = 18,
        .seal_name  = "Mirror",
        .hk_number  = 402,
        .role       = HK21_PORTAL_MATRIX,
        .domain     = HK21_UNITY,
        .planet     = "Neptune",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Meditation"
    },
    {   /* Seal 19 — Ring 5: Portal Matrix */
        .seal_code  = 19,
        .seal_name  = "Storm",
        .hk_number  = 408,
        .role       = HK21_PORTAL_MATRIX,
        .domain     = HK21_UNITY,
        .planet     = "Pluto",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Self-Generation"
    },
    {   /* Seal 20 — Ring 5: Portal Matrix */
        .seal_code  = 20,
        .seal_name  = "Sun",
        .hk_number  = 414,
        .role       = HK21_PORTAL_MATRIX,
        .domain     = HK21_UNITY,
        .planet     = "Pluto",
        .flow       = HK21_SOLAR_PROPHETIC,
        .statement  = "Illumination"
    },
    {   /* Seal 21 — Center: Hunab Ku */
        .seal_code  = 21,
        .seal_name  = "Hunab Ku",
        .hk_number  = 441,
        .role       = HK21_CENTER,
        .domain     = HK21_UNITY,
        .planet     = "Center",
        .flow       = HK21_FLOW_NONE,
        .statement  = "Coordinates and radializes Totality"
    }
};

/* --- Role name strings --- */

static const char *ROLE_NAMES[] = {
    "Gate of Light",
    "Gate of Power",
    "Source of Power",
    "Seat of Power",
    "Portal Matrix",
    "Center"
};

/* --- Domain name strings --- */

static const char *DOMAIN_NAMES[] = {
    "Knowledge",
    "Love",
    "Prophecy",
    "Intelligence",
    "Unity"
};

/* --- Flow name strings --- */

static const char *FLOW_NAMES[] = {
    "Galactic-Karmic",
    "Solar-Prophetic",
    "None"
};

/* --- Lens data --- */

static const int LENS_NUMBERS[HK21_LENS_COUNT] = {
    108, 144, 216, 288
};

static const char *LENS_NAMES[HK21_LENS_COUNT] = {
    "Cosmic Standard",
    "Cosmic Harmonic",
    "Cosmic Cube",
    "Ultimate Sphere"
};

/* --- Function implementations --- */

hk21_archetype_t hk21_archetype_for_seal(int seal_code)
{
    if (seal_code < 1 || seal_code > HK21_ARCHETYPE_COUNT) {
        hk21_archetype_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return ARCHETYPES[seal_code - 1];
}

int hk21_archetype_count(void)
{
    return HK21_ARCHETYPE_COUNT;
}

const char *hk21_role_name(hk21_role_t role)
{
    if (role < HK21_GATE_OF_LIGHT || role > HK21_CENTER)
        return "Unknown";
    return ROLE_NAMES[role];
}

const char *hk21_domain_name(hk21_domain_t domain)
{
    if (domain < HK21_KNOWLEDGE || domain > HK21_UNITY)
        return "Unknown";
    return DOMAIN_NAMES[domain];
}

const char *hk21_flow_name(hk21_flow_t flow)
{
    if (flow < HK21_GALACTIC_KARMIC || flow > HK21_FLOW_NONE)
        return "Unknown";
    return FLOW_NAMES[flow];
}

int hk21_lens_number(int lens)
{
    if (lens < 1 || lens > HK21_LENS_COUNT)
        return 0;
    return LENS_NUMBERS[lens - 1];
}

const char *hk21_lens_name(int lens)
{
    if (lens < 1 || lens > HK21_LENS_COUNT)
        return NULL;
    return LENS_NAMES[lens - 1];
}

hk21_domain_t hk21_domain_for_seal(int seal_code)
{
    if (seal_code < 1 || seal_code > HK21_ARCHETYPE_COUNT)
        return HK21_UNITY;
    return ARCHETYPES[seal_code - 1].domain;
}

int hk21_hk_number_for_seal(int seal_code)
{
    if (seal_code < 1 || seal_code > HK21_ARCHETYPE_COUNT)
        return 0;
    return ARCHETYPES[seal_code - 1].hk_number;
}
