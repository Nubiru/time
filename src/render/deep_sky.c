#include "deep_sky.h"

#include <stddef.h>

/* ──────────────────────────────────────────────────────────────────────────
 * Messier catalog — all 110 objects.
 * Data: J2000 RA (decimal hours), Dec (decimal degrees), type, integrated
 *       apparent magnitude, angular size (arcminutes, major axis), color hint.
 * Sources: SIMBAD, NED, NGC/IC Project (public domain).
 * ────────────────────────────────────────────────────────────────────── */

#define CATALOG_SIZE 110

static const dso_entry_t catalog[CATALOG_SIZE] = {
    /* M1   */ {"M1",   "Crab Nebula",           5.575,  22.017, DSO_SUPERNOVA_REMNANT,  8.4,   6.0,  {0.80f, 0.30f, 0.20f}},
    /* M2   */ {"M2",   NULL,                   21.558,  -0.817, DSO_GLOBULAR_CLUSTER,   6.3,  12.9,  {1.00f, 0.90f, 0.70f}},
    /* M3   */ {"M3",   NULL,                   13.703,  28.377, DSO_GLOBULAR_CLUSTER,   6.2,  18.0,  {1.00f, 0.90f, 0.70f}},
    /* M4   */ {"M4",   NULL,                   16.393, -26.526, DSO_GLOBULAR_CLUSTER,   5.6,  36.0,  {1.00f, 0.90f, 0.70f}},
    /* M5   */ {"M5",   NULL,                   15.310,   2.083, DSO_GLOBULAR_CLUSTER,   5.6,  23.0,  {1.00f, 0.90f, 0.70f}},
    /* M6   */ {"M6",   "Butterfly Cluster",    17.668, -32.217, DSO_OPEN_CLUSTER,       4.2,  25.0,  {0.70f, 0.80f, 1.00f}},
    /* M7   */ {"M7",   "Ptolemy's Cluster",    17.898, -34.817, DSO_OPEN_CLUSTER,       3.3,  80.0,  {0.70f, 0.80f, 1.00f}},
    /* M8   */ {"M8",   "Lagoon Nebula",        18.063, -24.383, DSO_EMISSION_NEBULA,    6.0,  90.0,  {0.90f, 0.30f, 0.40f}},
    /* M9   */ {"M9",   NULL,                   17.321, -18.517, DSO_GLOBULAR_CLUSTER,   7.7,  12.0,  {1.00f, 0.90f, 0.70f}},
    /* M10  */ {"M10",  NULL,                   16.952, -4.100,  DSO_GLOBULAR_CLUSTER,   6.6,  20.0,  {1.00f, 0.90f, 0.70f}},
    /* M11  */ {"M11",  "Wild Duck Cluster",    18.851, -6.267,  DSO_OPEN_CLUSTER,       6.3,  14.0,  {0.70f, 0.80f, 1.00f}},
    /* M12  */ {"M12",  NULL,                   16.787, -1.950,  DSO_GLOBULAR_CLUSTER,   6.7,  16.0,  {1.00f, 0.90f, 0.70f}},
    /* M13  */ {"M13",  "Great Globular",       16.695,  36.467, DSO_GLOBULAR_CLUSTER,   5.8,  20.0,  {1.00f, 0.90f, 0.70f}},
    /* M14  */ {"M14",  NULL,                   17.627, -3.250,  DSO_GLOBULAR_CLUSTER,   7.6,  11.0,  {1.00f, 0.90f, 0.70f}},
    /* M15  */ {"M15",  NULL,                   21.500,  12.167, DSO_GLOBULAR_CLUSTER,   6.2,  18.0,  {1.00f, 0.90f, 0.70f}},
    /* M16  */ {"M16",  "Eagle Nebula",         18.313, -13.783, DSO_EMISSION_NEBULA,    6.0,  35.0,  {0.90f, 0.30f, 0.40f}},
    /* M17  */ {"M17",  "Omega Nebula",         18.347, -16.183, DSO_EMISSION_NEBULA,    6.0,  46.0,  {0.90f, 0.30f, 0.40f}},
    /* M18  */ {"M18",  NULL,                   18.332, -17.133, DSO_OPEN_CLUSTER,       7.5,   9.0,  {0.70f, 0.80f, 1.00f}},
    /* M19  */ {"M19",  NULL,                   17.043, -26.267, DSO_GLOBULAR_CLUSTER,   6.8,  17.0,  {1.00f, 0.90f, 0.70f}},
    /* M20  */ {"M20",  "Trifid Nebula",        18.043, -23.033, DSO_EMISSION_NEBULA,    6.3,  28.0,  {0.90f, 0.30f, 0.40f}},
    /* M21  */ {"M21",  NULL,                   18.073, -22.500, DSO_OPEN_CLUSTER,       6.5,  13.0,  {0.70f, 0.80f, 1.00f}},
    /* M22  */ {"M22",  NULL,                   18.607, -23.900, DSO_GLOBULAR_CLUSTER,   5.1,  32.0,  {1.00f, 0.90f, 0.70f}},
    /* M23  */ {"M23",  NULL,                   17.948, -19.017, DSO_OPEN_CLUSTER,       6.9,  27.0,  {0.70f, 0.80f, 1.00f}},
    /* M24  */ {"M24",  "Sagittarius Star Cloud", 18.283, -18.517, DSO_OPEN_CLUSTER,     4.6,  90.0,  {0.70f, 0.80f, 1.00f}},
    /* M25  */ {"M25",  NULL,                   18.527, -19.250, DSO_OPEN_CLUSTER,       4.6,  40.0,  {0.70f, 0.80f, 1.00f}},
    /* M26  */ {"M26",  NULL,                   18.752, -9.400,  DSO_OPEN_CLUSTER,       8.0,  15.0,  {0.70f, 0.80f, 1.00f}},
    /* M27  */ {"M27",  "Dumbbell Nebula",      19.993,  22.717, DSO_PLANETARY_NEBULA,   7.5,   8.0,  {0.30f, 0.80f, 0.60f}},
    /* M28  */ {"M28",  NULL,                   18.410, -24.867, DSO_GLOBULAR_CLUSTER,   6.8,  11.2,  {1.00f, 0.90f, 0.70f}},
    /* M29  */ {"M29",  NULL,                   20.397,  38.517, DSO_OPEN_CLUSTER,       7.1,   7.0,  {0.70f, 0.80f, 1.00f}},
    /* M30  */ {"M30",  NULL,                   21.673, -23.183, DSO_GLOBULAR_CLUSTER,   7.2,  12.0,  {1.00f, 0.90f, 0.70f}},
    /* M31  */ {"M31",  "Andromeda Galaxy",      0.712,  41.267, DSO_SPIRAL_GALAXY,      3.4, 190.0,  {0.90f, 0.85f, 0.70f}},
    /* M32  */ {"M32",  NULL,                    0.712,  40.867, DSO_ELLIPTICAL_GALAXY,   8.1,   8.7,  {1.00f, 0.90f, 0.60f}},
    /* M33  */ {"M33",  "Triangulum Galaxy",     1.565,  30.650, DSO_SPIRAL_GALAXY,      5.7,  73.0,  {0.85f, 0.80f, 0.70f}},
    /* M34  */ {"M34",  NULL,                    2.702,  42.783, DSO_OPEN_CLUSTER,       5.5,  35.0,  {0.70f, 0.80f, 1.00f}},
    /* M35  */ {"M35",  NULL,                    6.148,  24.333, DSO_OPEN_CLUSTER,       5.3,  28.0,  {0.70f, 0.80f, 1.00f}},
    /* M36  */ {"M36",  NULL,                    5.602,  34.133, DSO_OPEN_CLUSTER,       6.3,  12.0,  {0.70f, 0.80f, 1.00f}},
    /* M37  */ {"M37",  NULL,                    5.873,  32.550, DSO_OPEN_CLUSTER,       6.2,  24.0,  {0.70f, 0.80f, 1.00f}},
    /* M38  */ {"M38",  NULL,                    5.478,  35.833, DSO_OPEN_CLUSTER,       7.4,  21.0,  {0.70f, 0.80f, 1.00f}},
    /* M39  */ {"M39",  NULL,                   21.535,  48.433, DSO_OPEN_CLUSTER,       4.6,  32.0,  {0.70f, 0.80f, 1.00f}},
    /* M40  */ {"M40",  "Winnecke 4",           12.370,  58.083, DSO_OPEN_CLUSTER,       8.4,   0.8,  {0.70f, 0.80f, 1.00f}},
    /* M41  */ {"M41",  NULL,                    6.783, -20.733, DSO_OPEN_CLUSTER,       4.5,  38.0,  {0.70f, 0.80f, 1.00f}},
    /* M42  */ {"M42",  "Orion Nebula",          5.588,  -5.383, DSO_EMISSION_NEBULA,    4.0,  85.0,  {0.90f, 0.40f, 0.50f}},
    /* M43  */ {"M43",  "De Mairan's Nebula",    5.593,  -5.267, DSO_EMISSION_NEBULA,    9.0,  20.0,  {0.90f, 0.30f, 0.40f}},
    /* M44  */ {"M44",  "Beehive Cluster",       8.673,  19.983, DSO_OPEN_CLUSTER,       3.7,  95.0,  {0.70f, 0.80f, 1.00f}},
    /* M45  */ {"M45",  "Pleiades",              3.783,  24.117, DSO_OPEN_CLUSTER,       1.6, 110.0,  {0.60f, 0.70f, 1.00f}},
    /* M46  */ {"M46",  NULL,                    7.698, -14.817, DSO_OPEN_CLUSTER,       6.1,  27.0,  {0.70f, 0.80f, 1.00f}},
    /* M47  */ {"M47",  NULL,                    7.610, -14.500, DSO_OPEN_CLUSTER,       4.4,  30.0,  {0.70f, 0.80f, 1.00f}},
    /* M48  */ {"M48",  NULL,                    8.228,  -5.800, DSO_OPEN_CLUSTER,       5.8,  54.0,  {0.70f, 0.80f, 1.00f}},
    /* M49  */ {"M49",  NULL,                   12.498,   8.000, DSO_ELLIPTICAL_GALAXY,   8.4,  10.2,  {1.00f, 0.90f, 0.60f}},
    /* M50  */ {"M50",  NULL,                    7.053,  -8.333, DSO_OPEN_CLUSTER,       5.9,  16.0,  {0.70f, 0.80f, 1.00f}},
    /* M51  */ {"M51",  "Whirlpool Galaxy",     13.498,  47.200, DSO_SPIRAL_GALAXY,      8.4,  11.0,  {0.80f, 0.75f, 0.60f}},
    /* M52  */ {"M52",  NULL,                   23.408,  61.583, DSO_OPEN_CLUSTER,       7.3,  13.0,  {0.70f, 0.80f, 1.00f}},
    /* M53  */ {"M53",  NULL,                   13.215,  18.167, DSO_GLOBULAR_CLUSTER,   7.6,  13.0,  {1.00f, 0.90f, 0.70f}},
    /* M54  */ {"M54",  NULL,                   18.918, -30.483, DSO_GLOBULAR_CLUSTER,   7.6,  12.0,  {1.00f, 0.90f, 0.70f}},
    /* M55  */ {"M55",  NULL,                   19.668, -30.967, DSO_GLOBULAR_CLUSTER,   6.3,  19.0,  {1.00f, 0.90f, 0.70f}},
    /* M56  */ {"M56",  NULL,                   19.278,  30.183, DSO_GLOBULAR_CLUSTER,   8.3,   8.8,  {1.00f, 0.90f, 0.70f}},
    /* M57  */ {"M57",  "Ring Nebula",          18.893,  33.033, DSO_PLANETARY_NEBULA,   8.8,   1.4,  {0.30f, 0.80f, 0.60f}},
    /* M58  */ {"M58",  NULL,                   12.627,  11.817, DSO_SPIRAL_GALAXY,      9.7,   5.5,  {0.90f, 0.85f, 0.70f}},
    /* M59  */ {"M59",  NULL,                   12.700,  11.650, DSO_ELLIPTICAL_GALAXY,   9.6,   5.4,  {1.00f, 0.90f, 0.60f}},
    /* M60  */ {"M60",  NULL,                   12.728,  11.550, DSO_ELLIPTICAL_GALAXY,   8.8,   7.6,  {1.00f, 0.90f, 0.60f}},
    /* M61  */ {"M61",  NULL,                   12.370,   4.467, DSO_SPIRAL_GALAXY,      9.7,   6.5,  {0.90f, 0.85f, 0.70f}},
    /* M62  */ {"M62",  NULL,                   17.018, -30.117, DSO_GLOBULAR_CLUSTER,   6.5,  15.0,  {1.00f, 0.90f, 0.70f}},
    /* M63  */ {"M63",  "Sunflower Galaxy",     13.265,  42.033, DSO_SPIRAL_GALAXY,      8.6,  12.6,  {0.90f, 0.85f, 0.70f}},
    /* M64  */ {"M64",  "Black Eye Galaxy",     12.945,  21.683, DSO_SPIRAL_GALAXY,      8.5,  10.0,  {0.90f, 0.85f, 0.70f}},
    /* M65  */ {"M65",  NULL,                   11.315,  13.083, DSO_SPIRAL_GALAXY,      9.3,  10.0,  {0.90f, 0.85f, 0.70f}},
    /* M66  */ {"M66",  NULL,                   11.338,  12.983, DSO_SPIRAL_GALAXY,      8.9,   9.1,  {0.90f, 0.85f, 0.70f}},
    /* M67  */ {"M67",  NULL,                    8.843,  11.817, DSO_OPEN_CLUSTER,       6.1,  30.0,  {0.70f, 0.80f, 1.00f}},
    /* M68  */ {"M68",  NULL,                   12.657, -26.750, DSO_GLOBULAR_CLUSTER,   7.8,  11.0,  {1.00f, 0.90f, 0.70f}},
    /* M69  */ {"M69",  NULL,                   18.523, -32.350, DSO_GLOBULAR_CLUSTER,   7.6,   9.8,  {1.00f, 0.90f, 0.70f}},
    /* M70  */ {"M70",  NULL,                   18.720, -32.300, DSO_GLOBULAR_CLUSTER,   7.9,   8.0,  {1.00f, 0.90f, 0.70f}},
    /* M71  */ {"M71",  NULL,                   19.897,  18.783, DSO_GLOBULAR_CLUSTER,   8.2,   7.2,  {1.00f, 0.90f, 0.70f}},
    /* M72  */ {"M72",  NULL,                   20.893, -12.533, DSO_GLOBULAR_CLUSTER,   9.3,   6.6,  {1.00f, 0.90f, 0.70f}},
    /* M73  */ {"M73",  NULL,                   20.983, -12.633, DSO_OPEN_CLUSTER,       9.0,   2.8,  {0.70f, 0.80f, 1.00f}},
    /* M74  */ {"M74",  "Phantom Galaxy",        1.612,  15.783, DSO_SPIRAL_GALAXY,      9.4,  10.5,  {0.90f, 0.85f, 0.70f}},
    /* M75  */ {"M75",  NULL,                   20.103, -21.917, DSO_GLOBULAR_CLUSTER,   8.5,   6.8,  {1.00f, 0.90f, 0.70f}},
    /* M76  */ {"M76",  "Little Dumbbell",       1.703,  51.567, DSO_PLANETARY_NEBULA,  10.1,   2.7,  {0.30f, 0.80f, 0.60f}},
    /* M77  */ {"M77",  "Cetus A",               2.712,  -0.017, DSO_SPIRAL_GALAXY,      8.9,   7.1,  {0.90f, 0.85f, 0.70f}},
    /* M78  */ {"M78",  NULL,                    5.778,   0.050, DSO_REFLECTION_NEBULA,  8.3,   8.0,  {0.40f, 0.50f, 1.00f}},
    /* M79  */ {"M79",  NULL,                    5.407, -24.517, DSO_GLOBULAR_CLUSTER,   7.7,   9.6,  {1.00f, 0.90f, 0.70f}},
    /* M80  */ {"M80",  NULL,                   16.283, -22.983, DSO_GLOBULAR_CLUSTER,   7.3,  10.0,  {1.00f, 0.90f, 0.70f}},
    /* M81  */ {"M81",  "Bode's Galaxy",         9.927,  69.067, DSO_SPIRAL_GALAXY,      6.9,  26.9,  {0.90f, 0.85f, 0.70f}},
    /* M82  */ {"M82",  "Cigar Galaxy",          9.930,  69.683, DSO_IRREGULAR_GALAXY,   8.4,  11.2,  {0.60f, 0.70f, 0.90f}},
    /* M83  */ {"M83",  "Southern Pinwheel",    13.617, -29.867, DSO_SPIRAL_GALAXY,      7.5,  12.9,  {0.90f, 0.85f, 0.70f}},
    /* M84  */ {"M84",  NULL,                   12.418,  12.883, DSO_ELLIPTICAL_GALAXY,   9.1,   6.5,  {1.00f, 0.90f, 0.60f}},
    /* M85  */ {"M85",  NULL,                   12.418,  18.183, DSO_ELLIPTICAL_GALAXY,   9.1,   7.1,  {1.00f, 0.90f, 0.60f}},
    /* M86  */ {"M86",  NULL,                   12.438,  12.950, DSO_ELLIPTICAL_GALAXY,   8.9,   8.9,  {1.00f, 0.90f, 0.60f}},
    /* M87  */ {"M87",  "Virgo A",              12.513,  12.400, DSO_ELLIPTICAL_GALAXY,   8.6,   8.3,  {1.00f, 0.90f, 0.60f}},
    /* M88  */ {"M88",  NULL,                   12.533,  14.417, DSO_SPIRAL_GALAXY,      9.6,   6.9,  {0.90f, 0.85f, 0.70f}},
    /* M89  */ {"M89",  NULL,                   12.593,  12.550, DSO_ELLIPTICAL_GALAXY,   9.8,   5.1,  {1.00f, 0.90f, 0.60f}},
    /* M90  */ {"M90",  NULL,                   12.613,  13.167, DSO_SPIRAL_GALAXY,      9.5,   9.5,  {0.90f, 0.85f, 0.70f}},
    /* M91  */ {"M91",  NULL,                   12.593,  14.500, DSO_SPIRAL_GALAXY,     10.2,   5.4,  {0.90f, 0.85f, 0.70f}},
    /* M92  */ {"M92",  NULL,                   17.285,  43.133, DSO_GLOBULAR_CLUSTER,   6.4,  14.0,  {1.00f, 0.90f, 0.70f}},
    /* M93  */ {"M93",  NULL,                    7.745, -23.867, DSO_OPEN_CLUSTER,       6.0,  22.0,  {0.70f, 0.80f, 1.00f}},
    /* M94  */ {"M94",  NULL,                   12.847,  41.117, DSO_SPIRAL_GALAXY,      8.2,  11.2,  {0.90f, 0.85f, 0.70f}},
    /* M95  */ {"M95",  NULL,                   10.730,  11.700, DSO_SPIRAL_GALAXY,      9.7,   7.4,  {0.90f, 0.85f, 0.70f}},
    /* M96  */ {"M96",  NULL,                   10.785,  11.817, DSO_SPIRAL_GALAXY,      9.2,   7.6,  {0.90f, 0.85f, 0.70f}},
    /* M97  */ {"M97",  "Owl Nebula",           11.247,  55.017, DSO_PLANETARY_NEBULA,   9.9,   3.4,  {0.30f, 0.80f, 0.60f}},
    /* M98  */ {"M98",  NULL,                   12.228,  14.900, DSO_SPIRAL_GALAXY,     10.1,   9.8,  {0.90f, 0.85f, 0.70f}},
    /* M99  */ {"M99",  NULL,                   12.313,  14.417, DSO_SPIRAL_GALAXY,      9.9,   5.4,  {0.90f, 0.85f, 0.70f}},
    /* M100 */ {"M100", "Mirror Galaxy",        12.383,  15.817, DSO_SPIRAL_GALAXY,      9.3,   7.4,  {0.90f, 0.85f, 0.70f}},
    /* M101 */ {"M101", "Pinwheel Galaxy",      14.053,  54.350, DSO_SPIRAL_GALAXY,      7.9,  28.8,  {0.85f, 0.80f, 0.70f}},
    /* M102 */ {"M102", "Spindle Galaxy",       15.113,  55.767, DSO_SPIRAL_GALAXY,      9.9,   6.5,  {0.90f, 0.85f, 0.70f}},
    /* M103 */ {"M103", NULL,                    1.557,  60.700, DSO_OPEN_CLUSTER,       7.4,   6.0,  {0.70f, 0.80f, 1.00f}},
    /* M104 */ {"M104", "Sombrero Galaxy",      12.667, -11.617, DSO_SPIRAL_GALAXY,      8.0,   9.0,  {0.90f, 0.80f, 0.60f}},
    /* M105 */ {"M105", NULL,                   10.797,  12.583, DSO_ELLIPTICAL_GALAXY,   9.3,   5.4,  {1.00f, 0.90f, 0.60f}},
    /* M106 */ {"M106", NULL,                   12.317,  47.300, DSO_SPIRAL_GALAXY,      8.4,  18.6,  {0.90f, 0.85f, 0.70f}},
    /* M107 */ {"M107", NULL,                   16.542, -13.050, DSO_GLOBULAR_CLUSTER,   7.9,  13.0,  {1.00f, 0.90f, 0.70f}},
    /* M108 */ {"M108", "Surfboard Galaxy",     11.190,  55.667, DSO_SPIRAL_GALAXY,     10.0,   8.7,  {0.90f, 0.85f, 0.70f}},
    /* M109 */ {"M109", NULL,                   11.963,  53.383, DSO_SPIRAL_GALAXY,      9.8,   7.6,  {0.90f, 0.85f, 0.70f}},
    /* M110 */ {"M110", NULL,                    0.673,  41.683, DSO_ELLIPTICAL_GALAXY,   8.5,  21.9,  {1.00f, 0.90f, 0.60f}}
};

/* ──────────────────────────────────────────────────────────────────────────
 * Type name strings.
 * ────────────────────────────────────────────────────────────────────── */

static const char *type_names[DSO_TYPE_COUNT] = {
    "Open Cluster",
    "Globular Cluster",
    "Emission Nebula",
    "Reflection Nebula",
    "Planetary Nebula",
    "Dark Nebula",
    "Supernova Remnant",
    "Spiral Galaxy",
    "Elliptical Galaxy",
    "Irregular Galaxy"
};

/* ──────────────────────────────────────────────────────────────────────────
 * Default rendering colors per type.
 * ────────────────────────────────────────────────────────────────────── */

static const dso_color_t type_colors[DSO_TYPE_COUNT] = {
    {0.70f, 0.80f, 1.00f},  /* Open Cluster       — blue-white young stars */
    {1.00f, 0.90f, 0.70f},  /* Globular Cluster    — warm old stars */
    {0.90f, 0.30f, 0.40f},  /* Emission Nebula     — H-alpha red/pink */
    {0.40f, 0.50f, 1.00f},  /* Reflection Nebula   — blue scattered light */
    {0.30f, 0.80f, 0.60f},  /* Planetary Nebula    — O-III green/teal */
    {0.10f, 0.10f, 0.10f},  /* Dark Nebula         — absorption */
    {0.80f, 0.30f, 0.20f},  /* Supernova Remnant   — mixed emission */
    {0.90f, 0.85f, 0.70f},  /* Spiral Galaxy       — yellowish integrated light */
    {1.00f, 0.90f, 0.60f},  /* Elliptical Galaxy   — old star population */
    {0.60f, 0.70f, 0.90f}   /* Irregular Galaxy    — mixed populations */
};

/* ──────────────────────────────────────────────────────────────────────────
 * Case-insensitive single-char helper (no strcasecmp — not C11 standard).
 * ────────────────────────────────────────────────────────────────────── */

static char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z') return (char)(c + ('a' - 'A'));
    return c;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Public API.
 * ────────────────────────────────────────────────────────────────────── */

int dso_catalog_count(void)
{
    return CATALOG_SIZE;
}

dso_entry_t dso_catalog_get(int index)
{
    if (index < 0 || index >= CATALOG_SIZE) {
        dso_entry_t sentinel = {0};
        sentinel.designation = (void *)0;
        sentinel.name = (void *)0;
        return sentinel;
    }
    return catalog[index];
}

const char *dso_type_name(dso_type_t type)
{
    if ((int)type < 0 || (int)type >= DSO_TYPE_COUNT) return "Unknown";
    return type_names[(int)type];
}

dso_color_t dso_type_color(dso_type_t type)
{
    if ((int)type < 0 || (int)type >= DSO_TYPE_COUNT) {
        dso_color_t black = {0.0f, 0.0f, 0.0f};
        return black;
    }
    return type_colors[(int)type];
}

int dso_count_by_type(dso_type_t type)
{
    int count = 0;
    for (int i = 0; i < CATALOG_SIZE; i++) {
        if (catalog[i].type == type) count++;
    }
    return count;
}

int dso_find_messier(int messier_number)
{
    if (messier_number < 1 || messier_number > 110) return -1;
    /* Messier numbers are sequential: M1=index0, M2=index1, etc. */
    return messier_number - 1;
}

int dso_find_by_name(const char *name)
{
    if (name == (void *)0) return -1;

    /* Get length of search string */
    int name_len = 0;
    while (name[name_len] != '\0') name_len++;
    if (name_len == 0) return -1;

    for (int i = 0; i < CATALOG_SIZE; i++) {
        const char *entry_name = catalog[i].name;
        if (entry_name == (void *)0) continue;

        /* Case-insensitive substring search */
        for (int j = 0; entry_name[j] != '\0'; j++) {
            int match = 1;
            for (int k = 0; k < name_len; k++) {
                if (entry_name[j + k] == '\0' ||
                    to_lower(entry_name[j + k]) != to_lower(name[k])) {
                    match = 0;
                    break;
                }
            }
            if (match) return i;
        }
    }
    return -1;
}
