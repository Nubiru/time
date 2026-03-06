/* planet_data.c — Physical properties for the 8 planets.
 * Data source: NASA Planetary Fact Sheet (public domain).
 * All functions are pure (no side effects, no global mutation). */

#include "planet_data.h"

/* Earth radius for relative size calculations */
static const double EARTH_RADIUS_KM = 6371.0;

/* Static planet data table (indexed by PLANET_MERCURY=0 .. PLANET_NEPTUNE=7) */
static const planet_physical_t PLANET_TABLE[PLANET_DATA_COUNT] = {
    /* Mercury */
    {
        .radius_km = 2439.7,
        .mass_kg = 0.33011e24,
        .albedo = 0.068,
        .axial_tilt_deg = 0.034,
        .rotation_hours = 1407.6,
        .ring_inner_radii = 0.0,
        .ring_outer_radii = 0.0,
        .color     = { 0.66f, 0.66f, 0.66f },
        .color_alt = { 0.55f, 0.55f, 0.55f },
        .has_rings = 0,
        .is_gas_giant = 0
    },
    /* Venus */
    {
        .radius_km = 6051.8,
        .mass_kg = 4.8675e24,
        .albedo = 0.689,
        .axial_tilt_deg = 177.4,
        .rotation_hours = -5832.5,
        .ring_inner_radii = 0.0,
        .ring_outer_radii = 0.0,
        .color     = { 0.90f, 0.78f, 0.55f },
        .color_alt = { 0.85f, 0.75f, 0.50f },
        .has_rings = 0,
        .is_gas_giant = 0
    },
    /* Earth */
    {
        .radius_km = 6371.0,
        .mass_kg = 5.9724e24,
        .albedo = 0.367,
        .axial_tilt_deg = 23.44,
        .rotation_hours = 23.934,
        .ring_inner_radii = 0.0,
        .ring_outer_radii = 0.0,
        .color     = { 0.14f, 0.25f, 0.51f },
        .color_alt = { 0.30f, 0.55f, 0.25f },
        .has_rings = 0,
        .is_gas_giant = 0
    },
    /* Mars */
    {
        .radius_km = 3389.5,
        .mass_kg = 0.64171e24,
        .albedo = 0.150,
        .axial_tilt_deg = 25.19,
        .rotation_hours = 24.623,
        .ring_inner_radii = 0.0,
        .ring_outer_radii = 0.0,
        .color     = { 0.73f, 0.38f, 0.22f },
        .color_alt = { 0.60f, 0.30f, 0.15f },
        .has_rings = 0,
        .is_gas_giant = 0
    },
    /* Jupiter */
    {
        .radius_km = 69911.0,
        .mass_kg = 1898.19e24,
        .albedo = 0.343,
        .axial_tilt_deg = 3.13,
        .rotation_hours = 9.925,
        .ring_inner_radii = 0.0,
        .ring_outer_radii = 0.0,
        .color     = { 0.78f, 0.67f, 0.51f },
        .color_alt = { 0.65f, 0.50f, 0.35f },
        .has_rings = 0,
        .is_gas_giant = 1
    },
    /* Saturn */
    {
        .radius_km = 58232.0,
        .mass_kg = 568.34e24,
        .albedo = 0.342,
        .axial_tilt_deg = 26.73,
        .rotation_hours = 10.656,
        .ring_inner_radii = 1.239,
        .ring_outer_radii = 2.270,
        .color     = { 0.82f, 0.75f, 0.59f },
        .color_alt = { 0.75f, 0.68f, 0.52f },
        .has_rings = 1,
        .is_gas_giant = 1
    },
    /* Uranus */
    {
        .radius_km = 25362.0,
        .mass_kg = 86.813e24,
        .albedo = 0.300,
        .axial_tilt_deg = 97.77,
        .rotation_hours = -17.24,
        .ring_inner_radii = 0.0,
        .ring_outer_radii = 0.0,
        .color     = { 0.71f, 0.82f, 0.86f },
        .color_alt = { 0.60f, 0.75f, 0.80f },
        .has_rings = 0,
        .is_gas_giant = 1
    },
    /* Neptune */
    {
        .radius_km = 24622.0,
        .mass_kg = 102.413e24,
        .albedo = 0.290,
        .axial_tilt_deg = 28.32,
        .rotation_hours = 16.11,
        .ring_inner_radii = 0.0,
        .ring_outer_radii = 0.0,
        .color     = { 0.24f, 0.35f, 0.67f },
        .color_alt = { 0.20f, 0.30f, 0.60f },
        .has_rings = 0,
        .is_gas_giant = 1
    }
};

/* Major moon counts (approximate, well-known moons) */
static const int MOON_COUNTS[PLANET_DATA_COUNT] = {
    0,   /* Mercury */
    0,   /* Venus */
    1,   /* Earth */
    2,   /* Mars */
    95,  /* Jupiter */
    146, /* Saturn */
    28,  /* Uranus */
    16   /* Neptune */
};

static int valid_index(int i) {
    return i >= 0 && i < PLANET_DATA_COUNT;
}

planet_physical_t planet_data_get(int planet_index) {
    if (!valid_index(planet_index)) {
        planet_physical_t empty = {
            .radius_km = 0.0,
            .mass_kg = 0.0,
            .albedo = 0.0,
            .axial_tilt_deg = 0.0,
            .rotation_hours = 0.0,
            .ring_inner_radii = 0.0,
            .ring_outer_radii = 0.0,
            .color = { 0.0f, 0.0f, 0.0f },
            .color_alt = { 0.0f, 0.0f, 0.0f },
            .has_rings = 0,
            .is_gas_giant = 0
        };
        return empty;
    }
    return PLANET_TABLE[planet_index];
}

planet_color_t planet_data_color(int planet_index) {
    if (!valid_index(planet_index)) {
        planet_color_t black = { 0.0f, 0.0f, 0.0f };
        return black;
    }
    return PLANET_TABLE[planet_index].color;
}

double planet_data_radius(int planet_index) {
    if (!valid_index(planet_index)) {
        return 0.0;
    }
    return PLANET_TABLE[planet_index].radius_km;
}

double planet_data_relative_size(int planet_index) {
    if (!valid_index(planet_index)) {
        return 0.0;
    }
    return PLANET_TABLE[planet_index].radius_km / EARTH_RADIUS_KM;
}

const char *planet_data_type(int planet_index) {
    if (!valid_index(planet_index)) {
        return "Unknown";
    }
    if (!PLANET_TABLE[planet_index].is_gas_giant) {
        return "Terrestrial";
    }
    /* Uranus and Neptune are ice giants */
    if (planet_index == 6 || planet_index == 7) {
        return "Ice Giant";
    }
    return "Gas Giant";
}

int planet_data_major_moons(int planet_index) {
    if (!valid_index(planet_index)) {
        return 0;
    }
    return MOON_COUNTS[planet_index];
}

double planet_data_scene_radius(int planet_index, double base_scale) {
    if (!valid_index(planet_index)) {
        return 0.0;
    }
    return PLANET_TABLE[planet_index].radius_km * base_scale;
}

double planet_data_ring_inner(int planet_index, double base_scale) {
    if (!valid_index(planet_index)) {
        return 0.0;
    }
    if (!PLANET_TABLE[planet_index].has_rings) {
        return 0.0;
    }
    return PLANET_TABLE[planet_index].radius_km *
           PLANET_TABLE[planet_index].ring_inner_radii * base_scale;
}

double planet_data_ring_outer(int planet_index, double base_scale) {
    if (!valid_index(planet_index)) {
        return 0.0;
    }
    if (!PLANET_TABLE[planet_index].has_rings) {
        return 0.0;
    }
    return PLANET_TABLE[planet_index].radius_km *
           PLANET_TABLE[planet_index].ring_outer_radii * base_scale;
}
