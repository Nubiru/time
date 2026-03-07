/* chakra.c — 7-chakra knowledge system implementation
 *
 * Pure data module. No globals, no malloc, no side effects.
 *
 * Data sources:
 * - Traditional Hindu/yogic 7-chakra system
 * - Solfeggio frequencies from Gregorian chant tradition
 * - Planetary correspondences from Western esoteric tradition */

#include "chakra.h"
#include <math.h>

/* Static data table: 7 chakras, root to crown */
static const chakra_info_t CHAKRA_DATA[CHAKRA_COUNT] = {
    {
        .id = CHAKRA_MULADHARA,
        .sanskrit_name = "Muladhara",
        .english_name = "Root",
        .location = "Base of spine",
        .bija_mantra = "LAM",
        .element = CHAKRA_ELEM_EARTH,
        .color = {1.0f, 0.0f, 0.0f},
        .frequency_hz = 396.0,
        .petal_count = 4,
        .planet = "Saturn"
    },
    {
        .id = CHAKRA_SVADHISTHANA,
        .sanskrit_name = "Svadhisthana",
        .english_name = "Sacral",
        .location = "Lower abdomen",
        .bija_mantra = "VAM",
        .element = CHAKRA_ELEM_WATER,
        .color = {1.0f, 0.5f, 0.0f},
        .frequency_hz = 417.0,
        .petal_count = 6,
        .planet = "Jupiter"
    },
    {
        .id = CHAKRA_MANIPURA,
        .sanskrit_name = "Manipura",
        .english_name = "Solar Plexus",
        .location = "Upper abdomen",
        .bija_mantra = "RAM",
        .element = CHAKRA_ELEM_FIRE,
        .color = {1.0f, 1.0f, 0.0f},
        .frequency_hz = 528.0,
        .petal_count = 10,
        .planet = "Mars"
    },
    {
        .id = CHAKRA_ANAHATA,
        .sanskrit_name = "Anahata",
        .english_name = "Heart",
        .location = "Center of chest",
        .bija_mantra = "YAM",
        .element = CHAKRA_ELEM_AIR,
        .color = {0.0f, 0.8f, 0.0f},
        .frequency_hz = 639.0,
        .petal_count = 12,
        .planet = "Venus"
    },
    {
        .id = CHAKRA_VISHUDDHA,
        .sanskrit_name = "Vishuddha",
        .english_name = "Throat",
        .location = "Throat",
        .bija_mantra = "HAM",
        .element = CHAKRA_ELEM_ETHER,
        .color = {0.0f, 0.5f, 1.0f},
        .frequency_hz = 741.0,
        .petal_count = 16,
        .planet = "Mercury"
    },
    {
        .id = CHAKRA_AJNA,
        .sanskrit_name = "Ajna",
        .english_name = "Third Eye",
        .location = "Between eyebrows",
        .bija_mantra = "OM",
        .element = CHAKRA_ELEM_LIGHT,
        .color = {0.3f, 0.0f, 0.5f},
        .frequency_hz = 852.0,
        .petal_count = 2,
        .planet = "Moon"
    },
    {
        .id = CHAKRA_SAHASRARA,
        .sanskrit_name = "Sahasrara",
        .english_name = "Crown",
        .location = "Top of head",
        .bija_mantra = "AUM",
        .element = CHAKRA_ELEM_THOUGHT,
        .color = {0.6f, 0.0f, 0.8f},
        .frequency_hz = 963.0,
        .petal_count = 1000,
        .planet = "Sun"
    }
};

static const char *ELEMENT_NAMES[CHAKRA_ELEM_COUNT] = {
    "Earth", "Water", "Fire", "Air", "Ether", "Light", "Thought"
};

static bool chakra_valid(chakra_t chakra)
{
    return (int)chakra >= 0 && (int)chakra < CHAKRA_COUNT;
}

chakra_info_t chakra_info(chakra_t chakra)
{
    if (!chakra_valid(chakra)) {
        chakra_info_t empty = {0};
        return empty;
    }
    return CHAKRA_DATA[chakra];
}

const char *chakra_sanskrit_name(chakra_t chakra)
{
    if (!chakra_valid(chakra)) return "?";
    return CHAKRA_DATA[chakra].sanskrit_name;
}

const char *chakra_english_name(chakra_t chakra)
{
    if (!chakra_valid(chakra)) return "?";
    return CHAKRA_DATA[chakra].english_name;
}

const char *chakra_location(chakra_t chakra)
{
    if (!chakra_valid(chakra)) return "?";
    return CHAKRA_DATA[chakra].location;
}

const char *chakra_bija(chakra_t chakra)
{
    if (!chakra_valid(chakra)) return "?";
    return CHAKRA_DATA[chakra].bija_mantra;
}

chakra_element_t chakra_element(chakra_t chakra)
{
    if (!chakra_valid(chakra)) return CHAKRA_ELEM_COUNT;
    return CHAKRA_DATA[chakra].element;
}

const char *chakra_element_name(chakra_element_t element)
{
    if ((int)element < 0 || (int)element >= CHAKRA_ELEM_COUNT) return "?";
    return ELEMENT_NAMES[element];
}

chakra_color_t chakra_color(chakra_t chakra)
{
    if (!chakra_valid(chakra)) {
        chakra_color_t black = {0.0f, 0.0f, 0.0f};
        return black;
    }
    return CHAKRA_DATA[chakra].color;
}

double chakra_frequency(chakra_t chakra)
{
    if (!chakra_valid(chakra)) return 0.0;
    return CHAKRA_DATA[chakra].frequency_hz;
}

int chakra_petals(chakra_t chakra)
{
    if (!chakra_valid(chakra)) return 0;
    return CHAKRA_DATA[chakra].petal_count;
}

const char *chakra_planet(chakra_t chakra)
{
    if (!chakra_valid(chakra)) return "?";
    return CHAKRA_DATA[chakra].planet;
}

int chakra_count(void)
{
    return CHAKRA_COUNT;
}

int chakra_from_frequency(double freq_hz, double tolerance_hz)
{
    int best = -1;
    double best_dist = tolerance_hz + 1.0;

    for (int i = 0; i < CHAKRA_COUNT; i++) {
        double dist = fabs(freq_hz - CHAKRA_DATA[i].frequency_hz);
        if (dist <= tolerance_hz && dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    return best;
}

chakra_t chakra_from_color(float r, float g, float b)
{
    chakra_t best = CHAKRA_MULADHARA;
    float best_dist = 1e30f;

    for (int i = 0; i < CHAKRA_COUNT; i++) {
        float dr = r - CHAKRA_DATA[i].color.r;
        float dg = g - CHAKRA_DATA[i].color.g;
        float db = b - CHAKRA_DATA[i].color.b;
        float dist = dr * dr + dg * dg + db * db;
        if (dist < best_dist) {
            best_dist = dist;
            best = (chakra_t)i;
        }
    }
    return best;
}
