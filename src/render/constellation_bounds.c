/*
 * constellation_bounds.c — IAU constellation boundary polygon data
 *
 * Provides simplified boundary polygons for all 88 IAU constellations.
 * Boundaries encode the official IAU sky partitioning (Delporte 1930,
 * precessed to J2000.0 by Davenhall & Leggett 1989).
 *
 * Each boundary is a closed polygon of (RA_hours, Dec_deg) vertices
 * following constant-RA or constant-Dec arcs. Simplified to ~5-20
 * vertices per constellation for visual purposes.
 *
 * Attribution:
 *   Eugène Delporte — original IAU boundary definitions (1930)
 *   Clive Davenhall & Suzanne Leggett — J2000.0 precession (1989)
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "constellation_bounds.h"

#include <math.h>
#include <stddef.h>

/* ══════════════════════════════════════════════════════════════════════
 * Internal: RA/Dec to Cartesian conversion
 * ══════════════════════════════════════════════════════════════════════ */

static void radec_to_xyz(float ra_hours, float dec_deg, float radius,
                         float *x, float *y, float *z)
{
    float ra_rad = ra_hours * (float)(PI / 12.0);
    float dec_rad = dec_deg * (float)(PI / 180.0);
    float cd = cosf(dec_rad);
    *x = radius * cd * cosf(ra_rad);
    *y = radius * sinf(dec_rad);
    *z = radius * cd * sinf(ra_rad);
}

/* ══════════════════════════════════════════════════════════════════════
 * Boundary vertex data for all 88 constellations
 *
 * Each constellation is a closed polygon. Vertices listed in order;
 * the last vertex implicitly connects back to the first.
 * Coordinates: RA in hours (0-24), Dec in degrees (-90 to +90).
 * Simplified from the official IAU boundaries for visual use.
 * ══════════════════════════════════════════════════════════════════════ */

/* Maximum vertices per constellation in this dataset */
#define MAX_VERTS 28

typedef struct {
    const char *name;
    const char *abbr;
    int is_zodiac;
    int nv;
    float verts[MAX_VERTS][2]; /* [ra_hours, dec_deg] */
} cb_data_t;

static const cb_data_t CB_DATA[CB_CONSTELLATION_COUNT] = {
    /* ── Zodiac constellations (12) ── */
    /* 0: Aries */
    {"Aries", "Ari", 1, 8, {
        {1.46, 10.0}, {1.46, 25.5}, {2.06, 25.5}, {2.06, 31.2},
        {3.50, 31.2}, {3.50, 10.0}, {2.46, 10.0}, {1.76, 10.0}
    }},
    /* 1: Taurus */
    {"Taurus", "Tau", 1, 10, {
        {3.50, 10.0}, {3.50, 31.2}, {4.37, 31.2}, {4.37, 28.0},
        {5.13, 28.0}, {5.98, 28.0}, {5.98, 18.0}, {5.98, 0.0},
        {5.17, 0.0}, {3.50, 0.0}
    }},
    /* 2: Gemini */
    {"Gemini", "Gem", 1, 8, {
        {5.98, 10.0}, {5.98, 28.0}, {6.30, 28.0}, {6.30, 35.0},
        {8.08, 35.0}, {8.08, 10.0}, {7.07, 10.0}, {6.30, 10.0}
    }},
    /* 3: Cancer */
    {"Cancer", "Cnc", 1, 6, {
        {8.08, 10.0}, {8.08, 33.5}, {9.18, 33.5}, {9.18, 10.0},
        {8.63, 10.0}, {8.08, 10.0}
    }},
    /* 4: Leo */
    {"Leo", "Leo", 1, 8, {
        {9.18, -6.0}, {9.18, 33.5}, {10.27, 33.5}, {11.58, 28.0},
        {11.87, 10.0}, {11.87, -6.0}, {10.76, -6.0}, {9.18, -6.0}
    }},
    /* 5: Virgo */
    {"Virgo", "Vir", 1, 10, {
        {11.87, -22.0}, {11.87, 14.0}, {12.37, 14.0}, {13.50, 14.0},
        {14.77, 14.0}, {14.77, -6.0}, {15.08, -6.0}, {15.08, -22.0},
        {13.50, -22.0}, {11.87, -22.0}
    }},
    /* 6: Libra */
    {"Libra", "Lib", 1, 6, {
        {14.22, -30.0}, {14.22, 0.0}, {15.08, 0.0}, {16.03, 0.0},
        {16.03, -30.0}, {14.22, -30.0}
    }},
    /* 7: Scorpius */
    {"Scorpius", "Sco", 1, 8, {
        {15.83, -45.5}, {15.83, -19.5}, {16.03, -19.5}, {16.49, -19.5},
        {17.50, -19.5}, {17.83, -37.0}, {17.83, -45.5}, {15.83, -45.5}
    }},
    /* 8: Sagittarius */
    {"Sagittarius", "Sgr", 1, 8, {
        {17.83, -45.5}, {17.83, -12.0}, {18.87, -12.0}, {19.27, -12.0},
        {20.42, -12.0}, {20.42, -27.5}, {20.42, -45.5}, {17.83, -45.5}
    }},
    /* 9: Capricornus */
    {"Capricornus", "Cap", 1, 6, {
        {20.07, -27.5}, {20.07, -8.0}, {21.47, -8.0}, {21.47, -27.5},
        {20.87, -27.5}, {20.07, -27.5}
    }},
    /* 10: Aquarius */
    {"Aquarius", "Aqr", 1, 8, {
        {20.87, -25.0}, {20.87, 2.0}, {21.47, 2.0}, {22.87, 2.0},
        {23.83, 2.0}, {23.83, -25.0}, {22.38, -25.0}, {20.87, -25.0}
    }},
    /* 11: Pisces */
    {"Pisces", "Psc", 1, 8, {
        {22.87, -6.0}, {22.87, 2.0}, {23.83, 2.0}, {23.83, 33.7},
        {0.48, 33.7}, {0.48, 2.0}, {1.46, 2.0}, {1.46, -6.0}
    }},

    /* ── Non-zodiac constellations (76) ── */
    /* 12: Andromeda */
    {"Andromeda", "And", 0, 8, {
        {22.57, 28.0}, {22.57, 53.2}, {0.14, 53.2}, {0.14, 48.0},
        {1.41, 48.0}, {1.57, 37.0}, {1.57, 28.0}, {22.57, 28.0}
    }},
    /* 13: Antlia */
    {"Antlia", "Ant", 0, 6, {
        {9.27, -24.5}, {9.27, -37.0}, {10.93, -37.0}, {10.93, -24.5},
        {10.10, -24.5}, {9.27, -24.5}
    }},
    /* 14: Apus */
    {"Apus", "Aps", 0, 6, {
        {13.83, -67.5}, {13.83, -83.1}, {18.45, -83.1}, {18.45, -67.5},
        {16.14, -67.5}, {13.83, -67.5}
    }},
    /* 15: Aquila */
    {"Aquila", "Aql", 0, 8, {
        {18.58, -12.0}, {18.58, 18.7}, {19.40, 18.7}, {20.38, 18.7},
        {20.38, 2.0}, {20.07, -4.0}, {20.07, -12.0}, {18.58, -12.0}
    }},
    /* 16: Ara */
    {"Ara", "Ara", 0, 6, {
        {16.37, -46.0}, {16.37, -67.5}, {18.11, -67.5}, {18.11, -46.0},
        {17.24, -46.0}, {16.37, -46.0}
    }},
    /* 17: Auriga */
    {"Auriga", "Aur", 0, 8, {
        {4.97, 28.0}, {4.97, 56.2}, {6.08, 56.2}, {7.03, 56.2},
        {7.03, 44.5}, {7.03, 28.0}, {6.00, 28.0}, {4.97, 28.0}
    }},
    /* 18: Bootes */
    {"Bootes", "Boo", 0, 8, {
        {13.37, 7.5}, {13.37, 55.0}, {14.72, 55.0}, {15.78, 55.0},
        {15.78, 28.0}, {15.08, 7.5}, {14.22, 7.5}, {13.37, 7.5}
    }},
    /* 19: Caelum */
    {"Caelum", "Cae", 0, 5, {
        {4.35, -27.0}, {4.35, -49.0}, {5.07, -49.0}, {5.07, -27.0},
        {4.35, -27.0}
    }},
    /* 20: Camelopardalis */
    {"Camelopardalis", "Cam", 0, 8, {
        {3.50, 52.5}, {3.50, 86.1}, {8.88, 86.1}, {14.12, 82.0},
        {14.12, 68.0}, {7.00, 68.0}, {7.00, 52.5}, {3.50, 52.5}
    }},
    /* 21: Canis Major */
    {"Canis Major", "CMa", 0, 8, {
        {6.12, -11.0}, {6.12, -33.2}, {7.37, -33.2}, {7.37, -26.0},
        {7.37, -11.0}, {6.93, -11.0}, {6.53, -11.0}, {6.12, -11.0}
    }},
    /* 22: Canis Minor */
    {"Canis Minor", "CMi", 0, 5, {
        {7.07, 0.0}, {7.07, 13.2}, {8.20, 13.2}, {8.20, 0.0},
        {7.07, 0.0}
    }},
    /* 23: Capricornus is zodiac (index 9), this is Carina */
    {"Carina", "Car", 0, 8, {
        {6.03, -52.5}, {6.03, -64.0}, {8.00, -64.0}, {9.22, -64.0},
        {11.20, -64.0}, {11.20, -55.0}, {9.22, -50.7}, {6.03, -52.5}
    }},
    /* 24: Cassiopeia */
    {"Cassiopeia", "Cas", 0, 8, {
        {22.57, 53.2}, {22.57, 77.7}, {0.00, 77.7}, {3.42, 77.7},
        {3.50, 63.0}, {3.50, 53.2}, {1.63, 46.1}, {0.17, 53.2}
    }},
    /* 25: Centaurus */
    {"Centaurus", "Cen", 0, 10, {
        {11.05, -33.0}, {11.05, -64.7}, {13.18, -64.7}, {14.92, -64.7},
        {14.92, -42.0}, {14.42, -42.0}, {14.42, -33.0}, {13.18, -33.0},
        {12.08, -33.0}, {11.05, -33.0}
    }},
    /* 26: Cepheus */
    {"Cepheus", "Cep", 0, 8, {
        {20.00, 61.5}, {20.00, 88.0}, {8.00, 88.0}, {0.00, 77.7},
        {22.87, 69.5}, {22.17, 61.5}, {21.08, 61.5}, {20.00, 61.5}
    }},
    /* 27: Cetus */
    {"Cetus", "Cet", 0, 8, {
        {23.83, -25.0}, {23.83, -6.0}, {1.46, -6.0}, {1.46, 10.0},
        {2.46, 10.0}, {2.46, -25.0}, {1.67, -25.0}, {0.00, -25.0}
    }},
    /* 28: Chamaeleon */
    {"Chamaeleon", "Cha", 0, 5, {
        {7.67, -75.3}, {7.67, -83.1}, {13.83, -83.1}, {13.83, -75.3},
        {7.67, -75.3}
    }},
    /* 29: Circinus */
    {"Circinus", "Cir", 0, 6, {
        {13.58, -55.0}, {13.58, -67.5}, {15.30, -67.5}, {15.30, -55.0},
        {14.44, -55.0}, {13.58, -55.0}
    }},
    /* 30: Columba */
    {"Columba", "Col", 0, 6, {
        {5.07, -27.0}, {5.07, -43.0}, {6.12, -43.0}, {6.87, -43.0},
        {6.87, -27.0}, {5.07, -27.0}
    }},
    /* 31: Coma Berenices */
    {"Coma Berenices", "Com", 0, 5, {
        {11.87, 14.0}, {11.87, 33.5}, {13.37, 33.5}, {13.37, 14.0},
        {11.87, 14.0}
    }},
    /* 32: Corona Australis */
    {"Corona Australis", "CrA", 0, 5, {
        {17.83, -37.0}, {17.83, -45.5}, {19.18, -45.5}, {19.18, -37.0},
        {17.83, -37.0}
    }},
    /* 33: Corona Borealis */
    {"Corona Borealis", "CrB", 0, 5, {
        {15.15, 26.0}, {15.15, 40.0}, {16.37, 40.0}, {16.37, 26.0},
        {15.15, 26.0}
    }},
    /* 34: Corvus */
    {"Corvus", "Crv", 0, 5, {
        {11.83, -11.7}, {11.83, -24.5}, {12.57, -24.5}, {12.57, -11.7},
        {11.83, -11.7}
    }},
    /* 35: Crater */
    {"Crater", "Crt", 0, 5, {
        {10.87, -6.6}, {10.87, -25.0}, {11.83, -25.0}, {11.83, -6.6},
        {10.87, -6.6}
    }},
    /* 36: Crux */
    {"Crux", "Cru", 0, 5, {
        {11.93, -55.7}, {11.93, -64.7}, {12.58, -64.7}, {12.58, -55.7},
        {11.93, -55.7}
    }},
    /* 37: Cygnus */
    {"Cygnus", "Cyg", 0, 8, {
        {19.17, 28.0}, {19.17, 61.5}, {20.00, 61.5}, {21.75, 61.5},
        {21.75, 44.0}, {21.75, 28.0}, {20.46, 28.0}, {19.17, 28.0}
    }},
    /* 38: Delphinus */
    {"Delphinus", "Del", 0, 5, {
        {20.14, 2.0}, {20.14, 21.1}, {20.93, 21.1}, {20.93, 2.0},
        {20.14, 2.0}
    }},
    /* 39: Dorado */
    {"Dorado", "Dor", 0, 6, {
        {3.87, -49.0}, {3.87, -70.1}, {5.65, -70.1}, {6.58, -70.1},
        {6.58, -49.0}, {3.87, -49.0}
    }},
    /* 40: Draco */
    {"Draco", "Dra", 0, 10, {
        {9.22, 54.0}, {9.22, 86.1}, {20.55, 86.1}, {20.55, 75.0},
        {20.55, 65.5}, {18.00, 65.5}, {18.00, 51.0}, {15.00, 51.0},
        {15.00, 54.0}, {9.22, 54.0}
    }},
    /* 41: Equuleus */
    {"Equuleus", "Equ", 0, 5, {
        {20.93, 2.0}, {20.93, 13.0}, {21.47, 13.0}, {21.47, 2.0},
        {20.93, 2.0}
    }},
    /* 42: Eridanus */
    {"Eridanus", "Eri", 0, 10, {
        {1.39, -39.6}, {1.39, 0.0}, {3.25, 0.0}, {3.75, 0.0},
        {5.17, 0.0}, {5.17, -11.0}, {4.80, -24.0}, {3.75, -39.6},
        {2.92, -39.6}, {1.39, -39.6}
    }},
    /* 43: Fornax */
    {"Fornax", "For", 0, 6, {
        {1.78, -24.0}, {1.78, -40.0}, {3.50, -40.0}, {3.87, -24.0},
        {2.83, -24.0}, {1.78, -24.0}
    }},
    /* 44: Gemini is zodiac (index 2), this is Grus */
    {"Grus", "Gru", 0, 6, {
        {21.47, -37.0}, {21.47, -57.0}, {23.27, -57.0}, {23.27, -37.0},
        {22.37, -37.0}, {21.47, -37.0}
    }},
    /* 45: Hercules — trimmed at RA 18.15 to avoid overlapping Lyra */
    {"Hercules", "Her", 0, 10, {
        {15.78, 14.0}, {15.78, 51.0}, {16.37, 51.0}, {17.25, 51.0},
        {18.00, 51.0}, {18.15, 51.0}, {18.15, 14.0}, {17.97, 14.0},
        {16.87, 14.0}, {15.78, 14.0}
    }},
    /* 46: Horologium */
    {"Horologium", "Hor", 0, 6, {
        {2.37, -40.0}, {2.37, -67.0}, {4.33, -67.0}, {4.33, -40.0},
        {3.35, -40.0}, {2.37, -40.0}
    }},
    /* 47: Hydra */
    {"Hydra", "Hya", 0, 12, {
        {8.12, -6.6}, {8.12, 7.0}, {9.18, 7.0}, {9.18, -6.6},
        {9.72, -6.6}, {10.87, -6.6}, {10.87, -25.0}, {11.83, -25.0},
        {14.77, -25.0}, {14.77, -33.0}, {11.87, -33.0}, {8.12, -11.5}
    }},
    /* 48: Hydrus */
    {"Hydrus", "Hyi", 0, 6, {
        {0.00, -57.8}, {0.00, -82.0}, {4.35, -82.0}, {4.35, -67.0},
        {3.40, -57.8}, {0.00, -57.8}
    }},
    /* 49: Indus */
    {"Indus", "Ind", 0, 6, {
        {20.42, -45.5}, {20.42, -74.5}, {21.73, -74.5}, {23.27, -57.0},
        {21.47, -45.5}, {20.42, -45.5}
    }},
    /* 50: Lacerta */
    {"Lacerta", "Lac", 0, 6, {
        {21.97, 35.0}, {21.97, 56.5}, {22.87, 56.5}, {22.87, 35.0},
        {22.42, 35.0}, {21.97, 35.0}
    }},
    /* 51: Leo Minor */
    {"Leo Minor", "LMi", 0, 5, {
        {9.22, 33.5}, {9.22, 42.0}, {11.07, 42.0}, {11.07, 33.5},
        {9.22, 33.5}
    }},
    /* 52: Lepus */
    {"Lepus", "Lep", 0, 6, {
        {4.92, -11.0}, {4.92, -27.0}, {6.12, -27.0}, {6.12, -11.0},
        {5.52, -11.0}, {4.92, -11.0}
    }},
    /* 53: Lupus */
    {"Lupus", "Lup", 0, 6, {
        {14.17, -33.0}, {14.17, -55.0}, {16.12, -55.0}, {16.12, -33.0},
        {15.14, -33.0}, {14.17, -33.0}
    }},
    /* 54: Lynx */
    {"Lynx", "Lyn", 0, 6, {
        {6.17, 33.0}, {6.17, 62.0}, {9.22, 62.0}, {9.22, 33.0},
        {7.69, 33.0}, {6.17, 33.0}
    }},
    /* 55: Lyra */
    {"Lyra", "Lyr", 0, 6, {
        {18.15, 25.5}, {18.15, 47.7}, {19.40, 47.7}, {19.40, 25.5},
        {18.78, 25.5}, {18.15, 25.5}
    }},
    /* 56: Mensa */
    {"Mensa", "Men", 0, 5, {
        {3.50, -70.1}, {3.50, -85.3}, {7.67, -85.3}, {7.67, -70.1},
        {3.50, -70.1}
    }},
    /* 57: Microscopium */
    {"Microscopium", "Mic", 0, 5, {
        {20.42, -27.5}, {20.42, -45.1}, {21.47, -45.1}, {21.47, -27.5},
        {20.42, -27.5}
    }},
    /* 58: Monoceros */
    {"Monoceros", "Mon", 0, 8, {
        {5.83, -11.0}, {5.83, 0.0}, {5.98, 0.0}, {7.07, 0.0},
        {8.08, 0.0}, {8.08, -11.0}, {6.96, -11.0}, {5.83, -11.0}
    }},
    /* 59: Musca */
    {"Musca", "Mus", 0, 5, {
        {11.32, -64.7}, {11.32, -75.7}, {13.58, -75.7}, {13.58, -64.7},
        {11.32, -64.7}
    }},
    /* 60: Norma */
    {"Norma", "Nor", 0, 6, {
        {15.62, -42.0}, {15.62, -60.4}, {16.45, -60.4}, {16.45, -42.0},
        {16.03, -42.0}, {15.62, -42.0}
    }},
    /* 61: Octans — south polar cap; polygon avoids 0/24 RA wrap */
    {"Octans", "Oct", 0, 10, {
        {0.50, -82.0}, {4.00, -82.0}, {8.00, -82.0}, {12.00, -82.0},
        {16.00, -82.0}, {20.00, -82.0}, {23.50, -82.0},
        {23.50, -90.0}, {12.00, -90.0}, {0.50, -90.0}
    }},
    /* 62: Ophiuchus */
    {"Ophiuchus", "Oph", 0, 10, {
        {16.03, -8.0}, {16.03, 14.0}, {16.53, 14.0}, {16.53, 24.0},
        {17.58, 24.0}, {17.97, 14.0}, {18.58, 2.5}, {18.58, -8.0},
        {17.17, -30.0}, {16.03, -30.0}
    }},
    /* 63: Orion */
    {"Orion", "Ori", 0, 8, {
        {4.67, -11.0}, {4.67, 0.0}, {5.17, 0.0}, {5.98, 0.0},
        {5.98, 10.0}, {6.38, 22.9}, {6.38, -3.5}, {4.92, -11.0}
    }},
    /* 64: Pavo */
    {"Pavo", "Pav", 0, 6, {
        {18.15, -57.0}, {18.15, -74.5}, {21.47, -74.5}, {21.47, -57.0},
        {19.81, -57.0}, {18.15, -57.0}
    }},
    /* 65: Pegasus */
    {"Pegasus", "Peg", 0, 8, {
        {21.13, 2.0}, {21.13, 36.6}, {22.87, 36.6}, {22.87, 28.0},
        {23.83, 28.0}, {23.83, 2.0}, {22.48, 2.0}, {21.13, 2.0}
    }},
    /* 66: Perseus */
    {"Perseus", "Per", 0, 8, {
        {1.66, 31.2}, {1.66, 59.1}, {3.50, 59.1}, {4.52, 59.1},
        {4.52, 31.2}, {4.07, 31.2}, {2.86, 31.2}, {1.66, 31.2}
    }},
    /* 67: Phoenix */
    {"Phoenix", "Phe", 0, 6, {
        {23.27, -39.2}, {23.27, -57.8}, {1.58, -57.8}, {1.58, -39.2},
        {0.43, -39.2}, {23.27, -39.2}
    }},
    /* 68: Pictor */
    {"Pictor", "Pic", 0, 6, {
        {4.53, -43.0}, {4.53, -64.2}, {6.03, -64.2}, {6.85, -53.0},
        {6.03, -43.0}, {4.53, -43.0}
    }},
    /* 69: Piscis Austrinus */
    {"Piscis Austrinus", "PsA", 0, 5, {
        {21.47, -25.0}, {21.47, -37.0}, {23.27, -37.0}, {23.27, -25.0},
        {21.47, -25.0}
    }},
    /* 70: Puppis */
    {"Puppis", "Pup", 0, 8, {
        {6.03, -11.0}, {6.03, -43.0}, {7.37, -43.0}, {8.12, -43.0},
        {8.12, -40.0}, {8.12, -24.5}, {7.73, -11.0}, {6.03, -11.0}
    }},
    /* 71: Pyxis */
    {"Pyxis", "Pyx", 0, 5, {
        {8.43, -17.4}, {8.43, -37.3}, {9.48, -37.3}, {9.48, -17.4},
        {8.43, -17.4}
    }},
    /* 72: Reticulum */
    {"Reticulum", "Ret", 0, 5, {
        {3.33, -53.0}, {3.33, -67.0}, {4.60, -67.0}, {4.60, -53.0},
        {3.33, -53.0}
    }},
    /* 73: Sagitta */
    {"Sagitta", "Sge", 0, 5, {
        {18.87, 16.2}, {18.87, 21.2}, {20.20, 21.2}, {20.20, 16.2},
        {18.87, 16.2}
    }},
    /* 74: Sculptor */
    {"Sculptor", "Scl", 0, 6, {
        {23.27, -25.0}, {23.27, -39.2}, {1.78, -39.2}, {1.78, -25.0},
        {0.48, -25.0}, {23.27, -25.0}
    }},
    /* 75: Scutum */
    {"Scutum", "Sct", 0, 5, {
        {18.22, -4.0}, {18.22, -15.9}, {18.87, -15.9}, {18.87, -4.0},
        {18.22, -4.0}
    }},
    /* 76: Serpens (combined head + tail) */
    {"Serpens", "Ser", 0, 8, {
        {15.08, -3.7}, {15.08, 25.5}, {16.08, 25.5}, {16.08, 6.0},
        {17.97, 6.0}, {18.22, -4.0}, {17.17, -4.0}, {15.08, -3.7}
    }},
    /* 77: Sextans */
    {"Sextans", "Sex", 0, 5, {
        {9.87, -11.7}, {9.87, 6.4}, {10.87, 6.4}, {10.87, -11.7},
        {9.87, -11.7}
    }},
    /* 78: Triangulum */
    {"Triangulum", "Tri", 0, 5, {
        {1.32, 25.6}, {1.32, 37.3}, {2.50, 37.3}, {2.50, 25.6},
        {1.32, 25.6}
    }},
    /* 79: Triangulum Australe */
    {"Triangulum Australe", "TrA", 0, 5, {
        {14.92, -60.3}, {14.92, -70.5}, {17.00, -70.5}, {17.00, -60.3},
        {14.92, -60.3}
    }},
    /* 80: Tucana */
    {"Tucana", "Tuc", 0, 6, {
        {22.30, -57.0}, {22.30, -75.3}, {0.00, -75.3}, {1.33, -75.3},
        {1.33, -57.0}, {22.30, -57.0}
    }},
    /* 81: Ursa Major */
    {"Ursa Major", "UMa", 0, 10, {
        {8.08, 33.5}, {8.08, 73.5}, {9.22, 73.5}, {11.50, 73.5},
        {14.12, 73.5}, {14.12, 54.0}, {12.07, 44.5}, {11.07, 33.5},
        {9.22, 33.5}, {8.08, 33.5}
    }},
    /* 82: Ursa Minor — polar cap; polygon traces the RA circle at dec 77.7
     * then returns along dec 90. Uses small segments to avoid big RA jumps. */
    {"Ursa Minor", "UMi", 0, 10, {
        {0.50, 77.7}, {4.00, 77.7}, {8.00, 77.7}, {12.00, 77.7},
        {16.00, 77.7}, {20.00, 77.7}, {23.50, 77.7},
        {23.50, 90.0}, {12.00, 90.0}, {0.50, 90.0}
    }},
    /* 83: Vela */
    {"Vela", "Vel", 0, 8, {
        {8.00, -37.0}, {8.00, -56.6}, {9.37, -56.6}, {11.20, -56.6},
        {11.20, -46.0}, {9.83, -37.0}, {8.93, -37.0}, {8.00, -37.0}
    }},
    /* 84: Volans */
    {"Volans", "Vol", 0, 6, {
        {6.58, -64.1}, {6.58, -75.3}, {9.03, -75.3}, {9.03, -64.1},
        {7.81, -64.1}, {6.58, -64.1}
    }},
    /* 85: Vulpecula */
    {"Vulpecula", "Vul", 0, 6, {
        {18.87, 19.2}, {18.87, 28.0}, {21.08, 28.0}, {21.08, 19.2},
        {19.97, 19.2}, {18.87, 19.2}
    }},
    /* 86: Telescopium */
    {"Telescopium", "Tel", 0, 5, {
        {18.15, -45.5}, {18.15, -57.0}, {20.42, -57.0}, {20.42, -45.5},
        {18.15, -45.5}
    }},
    /* 87: Canes Venatici */
    {"Canes Venatici", "CVn", 0, 5, {
        {12.07, 28.0}, {12.07, 52.4}, {14.07, 52.4}, {14.07, 28.0},
        {12.07, 28.0}
    }},
};

/* ══════════════════════════════════════════════════════════════════════
 * cb_get
 * ══════════════════════════════════════════════════════════════════════ */

cb_constellation_t cb_get(int index)
{
    cb_constellation_t result;
    if (index < 0 || index >= CB_CONSTELLATION_COUNT) {
        result.name = NULL;
        result.abbr = NULL;
        result.vertex_count = 0;
        result.is_zodiac = 0;
        return result;
    }
    const cb_data_t *d = &CB_DATA[index];
    result.name = d->name;
    result.abbr = d->abbr;
    result.vertex_count = d->nv;
    result.is_zodiac = d->is_zodiac;
    return result;
}

/* ══════════════════════════════════════════════════════════════════════
 * cb_vertex_count
 * ══════════════════════════════════════════════════════════════════════ */

int cb_vertex_count(int index)
{
    if (index < 0 || index >= CB_CONSTELLATION_COUNT) return 0;
    return CB_DATA[index].nv;
}

/* ══════════════════════════════════════════════════════════════════════
 * cb_vertex
 * ══════════════════════════════════════════════════════════════════════ */

cb_point_t cb_vertex(int constellation_index, int vertex_index)
{
    cb_point_t p = {0.0f, 0.0f};
    if (constellation_index < 0 || constellation_index >= CB_CONSTELLATION_COUNT) {
        return p;
    }
    const cb_data_t *d = &CB_DATA[constellation_index];
    if (vertex_index < 0 || vertex_index >= d->nv) {
        return p;
    }
    p.ra_hours = d->verts[vertex_index][0];
    p.dec_deg = d->verts[vertex_index][1];
    return p;
}

/* ══════════════════════════════════════════════════════════════════════
 * cb_find
 * ══════════════════════════════════════════════════════════════════════ */

int cb_find(const char *abbr)
{
    if (abbr == NULL || abbr[0] == '\0') return -1;
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        const char *a = CB_DATA[i].abbr;
        const char *b = abbr;
        int match = 1;
        while (*a && *b) {
            if (*a != *b) { match = 0; break; }
            a++;
            b++;
        }
        if (*a != *b) match = 0;
        if (match) return i;
    }
    return -1;
}

/* ══════════════════════════════════════════════════════════════════════
 * cb_total_segments
 * ══════════════════════════════════════════════════════════════════════ */

int cb_total_segments(void)
{
    int total = 0;
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        total += CB_DATA[i].nv;
    }
    return total;
}

/* ══════════════════════════════════════════════════════════════════════
 * cb_containing — ray-casting point-in-polygon on RA/Dec
 *
 * Uses the standard ray-casting algorithm with a horizontal ray
 * cast from the test point. Counts edge crossings. Handles
 * RA wrap-around by normalizing edges relative to the test point.
 * ══════════════════════════════════════════════════════════════════════ */

static int point_in_polygon(float test_ra, float test_dec,
                            const float verts[][2], int nv)
{
    /*
     * Ray-casting in RA/Dec space. Cast a ray from (test_ra, test_dec)
     * in the +Dec direction and count edge crossings.
     *
     * To handle RA wrap-around at 0h/24h, we unwrap all polygon vertex
     * RAs into a contiguous range centered on test_ra. Each vertex RA is
     * adjusted so that consecutive vertices never jump more than 12h,
     * maintaining the polygon's continuity.
     */
    if (nv < 3) return 0;

    /* Build unwrapped RA array. Start from vertex 0, unwrap each
     * subsequent vertex relative to its predecessor. */
    float unwrapped_ra[MAX_VERTS];
    unwrapped_ra[0] = verts[0][0];

    /* Adjust first vertex relative to test_ra: put it in (-12,+12] of test */
    float d0 = unwrapped_ra[0] - test_ra;
    while (d0 > 12.0f) { d0 -= 24.0f; unwrapped_ra[0] -= 24.0f; }
    while (d0 <= -12.0f) { d0 += 24.0f; unwrapped_ra[0] += 24.0f; }

    for (int i = 1; i < nv; i++) {
        unwrapped_ra[i] = verts[i][0];
        /* Unwrap relative to previous vertex so the polygon is continuous */
        float diff = unwrapped_ra[i] - unwrapped_ra[i - 1];
        while (diff > 12.0f) { diff -= 24.0f; unwrapped_ra[i] -= 24.0f; }
        while (diff <= -12.0f) { diff += 24.0f; unwrapped_ra[i] += 24.0f; }
    }

    /* Now do standard ray-casting with unwrapped RAs.
     * Test RA = test_ra (original, not shifted). */
    int crossings = 0;
    for (int i = 0; i < nv; i++) {
        int j = (i + 1) % nv;

        float ri = unwrapped_ra[i];
        float rj = unwrapped_ra[j];
        float dec_i = verts[i][1];
        float dec_j = verts[j][1];

        /* Check if edge straddles test_ra */
        float ra_min = ri < rj ? ri : rj;
        float ra_max = ri > rj ? ri : rj;

        /* Skip if edge doesn't cross test_ra */
        if (test_ra < ra_min || test_ra >= ra_max) continue;

        /* Compute Dec at crossing */
        float frac = (test_ra - ri) / (rj - ri);
        float cross_dec = dec_i + frac * (dec_j - dec_i);

        if (test_dec < cross_dec) {
            crossings++;
        }
    }
    return (crossings % 2) == 1;
}

int cb_containing(float ra_hours, float dec_deg)
{
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        if (point_in_polygon(ra_hours, dec_deg,
                             CB_DATA[i].verts, CB_DATA[i].nv)) {
            return i;
        }
    }
    return -1;
}

/* ══════════════════════════════════════════════════════════════════════
 * Internal: pack segments for one constellation
 * Returns number of segments written.
 * ══════════════════════════════════════════════════════════════════════ */

static int pack_constellation(const cb_data_t *d, float sphere_radius,
                              float r, float g, float b, float alpha,
                              float *out, int out_offset)
{
    int nv = d->nv;
    for (int i = 0; i < nv; i++) {
        int j = (i + 1) % nv;
        float x0, y0, z0, x1, y1, z1;

        radec_to_xyz(d->verts[i][0], d->verts[i][1], sphere_radius,
                     &x0, &y0, &z0);
        radec_to_xyz(d->verts[j][0], d->verts[j][1], sphere_radius,
                     &x1, &y1, &z1);

        int base = out_offset + i * 2 * CB_VERTEX_FLOATS;

        /* Endpoint A */
        out[base + 0] = x0;
        out[base + 1] = y0;
        out[base + 2] = z0;
        out[base + 3] = r;
        out[base + 4] = g;
        out[base + 5] = b;
        out[base + 6] = alpha;

        /* Endpoint B */
        out[base + 7] = x1;
        out[base + 8] = y1;
        out[base + 9] = z1;
        out[base + 10] = r;
        out[base + 11] = g;
        out[base + 12] = b;
        out[base + 13] = alpha;
    }
    return nv;
}

/* ══════════════════════════════════════════════════════════════════════
 * cb_pack
 *
 * Packs all constellation boundaries. Zodiac first with zodiac_alpha,
 * then non-zodiac with base_alpha.
 * Boundary color: dim cyan (0.3, 0.5, 0.7) — subtler than constellation lines.
 * ══════════════════════════════════════════════════════════════════════ */

int cb_pack(float sphere_radius, float base_alpha, float zodiac_alpha,
            float *out)
{
    if (out == NULL) return 0;

    /* Boundary line color: dim cyan, distinct from constellation lines */
    float cr = 0.3f;
    float cg = 0.5f;
    float cb_color = 0.7f;

    int segments_written = 0;

    /* Pass 1: zodiac constellations first */
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        if (!CB_DATA[i].is_zodiac) continue;
        int out_off = segments_written * 2 * CB_VERTEX_FLOATS;
        int segs = pack_constellation(&CB_DATA[i], sphere_radius,
                                       cr, cg, cb_color, zodiac_alpha,
                                       out, out_off);
        segments_written += segs;
    }

    /* Pass 2: non-zodiac constellations */
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        if (CB_DATA[i].is_zodiac) continue;
        int out_off = segments_written * 2 * CB_VERTEX_FLOATS;
        int segs = pack_constellation(&CB_DATA[i], sphere_radius,
                                       cr, cg, cb_color, base_alpha,
                                       out, out_off);
        segments_written += segs;
    }

    return segments_written;
}

/* ══════════════════════════════════════════════════════════════════════
 * cb_pack_info
 * ══════════════════════════════════════════════════════════════════════ */

cb_pack_info_t cb_pack_info(int segment_count)
{
    cb_pack_info_t info;
    info.total_segments = segment_count;
    info.total_vertices = segment_count * 2;

    /* Count zodiac segments */
    int zodiac_segs = 0;
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        if (CB_DATA[i].is_zodiac) {
            zodiac_segs += CB_DATA[i].nv;
        }
    }
    info.zodiac_segments = zodiac_segs;

    /* Compute offsets and counts — zodiac first, then non-zodiac */
    int offset = 0;

    /* First pass: zodiac */
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        if (!CB_DATA[i].is_zodiac) continue;
        info.constellation_offsets[i] = offset;
        info.constellation_counts[i] = CB_DATA[i].nv;
        offset += CB_DATA[i].nv;
    }

    /* Second pass: non-zodiac */
    for (int i = 0; i < CB_CONSTELLATION_COUNT; i++) {
        if (CB_DATA[i].is_zodiac) continue;
        info.constellation_offsets[i] = offset;
        info.constellation_counts[i] = CB_DATA[i].nv;
        offset += CB_DATA[i].nv;
    }

    return info;
}

/* ══════════════════════════════════════════════════════════════════════
 * cb_pack_one
 * ══════════════════════════════════════════════════════════════════════ */

int cb_pack_one(int index, float sphere_radius, float alpha, float *out)
{
    if (index < 0 || index >= CB_CONSTELLATION_COUNT || out == NULL) return 0;

    float cr = 0.3f;
    float cg = 0.5f;
    float cb_color = 0.7f;

    return pack_constellation(&CB_DATA[index], sphere_radius,
                               cr, cg, cb_color, alpha, out, 0);
}

/* ══════════════════════════════════════════════════════════════════════
 * Shader source strings — GLSL ES 3.00 (WebGL2)
 *
 * Boundary line vertex shader: MVP transform, pass color.
 * Boundary line fragment shader: passthrough color.
 * ══════════════════════════════════════════════════════════════════════ */

static const char CB_VERT_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec4 a_color;\n"
    "\n"
    "out vec4 v_color;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    v_color = a_color;\n"
    "}\n";

static const char CB_FRAG_SOURCE[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    frag_color = v_color;\n"
    "}\n";

const char *cb_vert_source(void)
{
    return CB_VERT_SOURCE;
}

const char *cb_frag_source(void)
{
    return CB_FRAG_SOURCE;
}
