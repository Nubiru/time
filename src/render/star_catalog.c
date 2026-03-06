#include "star_catalog.h"

#include <math.h>

#define PI 3.14159265358979323846

/* ──────────────────────────────────────────────────────────────────────────
 * Bright star catalog — 99 brightest stars sorted by visual magnitude.
 * Data: RA (hours), Dec (degrees), visual magnitude, B-V color index.
 *
 * Index reference:
 *  0 Sirius         1 Canopus        2 Rigil Kent     3 Arcturus       4 Vega
 *  5 Capella        6 Rigel          7 Procyon        8 Achernar       9 Betelgeuse
 * 10 Hadar         11 Altair        12 Acrux         13 Aldebaran     14 Antares
 * 15 Spica         16 Pollux        17 Fomalhaut     18 Deneb         19 Mimosa
 * 20 Regulus       21 Adhara        22 Castor        23 Shaula        24 Gacrux
 * 25 Bellatrix     26 Elnath        27 Miaplacidus   28 Alnilam       29 Alnair
 * 30 Alnitak       31 Alioth        32 Regor         33 Dubhe         34 Mirfak
 * 35 Wezen         36 Kaus Aust.    37 Avior         38 Alkaid        39 Sargas
 * 40 Menkalinan    41 Atria         42 Alhena        43 Peacock       44 Mirzam
 * 45 Alphard       46 Hamal         47 Polaris       48 Diphda        49 Nunki
 * 50 Alpheratz     51 Mirach        52 Rasalhague    53 Kochab        54 Saiph
 * 55 Algol         56 Denebola      57 Sadr          58 Suhail        59 Mintaka
 * 60 Schedar       61 Eltanin       62 Alphecca      63 Naos          64 Almach
 * 65 Caph          66 Mizar         67 Algieba       68 Alpha Lupi    69 Dschubba
 * 70 Izar          71 Merak         72 Enif          73 Scheat        74 Sabik
 * 75 Phecda        76 Aludra        77 Gienah Cyg    78 Navi          79 Markab
 * 80 Zosma         81 Gienah Crv    82 Zuben.mali    83 Sheratan      84 Kraz
 * 85 Unukalhai     86 Ruchbah       87 Tarazed       88 Zuben.nubi    89 Kornephoros
 * 90 Rastaban      91 Cursa         92 Algenib       93 Algorab       94 Minkar
 * 95 Albireo       96 Sulafat       97 Megrez        98 Segin
 * ────────────────────────────────────────────────────────────────────── */

#define CATALOG_SIZE 99

static const float catalog_ra[CATALOG_SIZE] = {
     6.75f,  6.40f, 14.66f, 14.26f, 18.62f,  5.28f,  5.24f,  7.65f,  1.63f,  5.92f,
    14.06f, 19.85f, 12.44f,  4.60f, 16.49f, 13.42f,  7.76f, 22.96f, 20.69f, 12.80f,
    10.14f,  6.98f,  7.58f, 17.56f, 12.52f,  5.42f,  5.44f,  9.22f,  5.60f, 22.14f,
     5.68f, 12.90f,  8.16f, 11.06f,  3.41f,  7.14f, 18.40f,  8.38f, 13.79f, 17.62f,
     6.00f, 16.81f,  6.63f, 20.43f,  6.38f,  9.46f,  2.12f,  2.53f,  0.73f, 18.92f,
     0.14f,  1.16f, 17.58f, 14.85f,  5.80f,  3.14f, 11.82f, 20.37f,  9.13f,  5.53f,
     0.68f, 17.94f, 15.58f,  8.06f,  2.07f,  0.15f, 13.40f, 10.33f, 14.70f, 16.01f,
    14.75f, 11.03f, 21.74f, 23.06f, 17.17f, 11.90f,  7.40f, 20.77f,  0.95f, 23.08f,
    11.24f, 12.26f, 15.28f,  1.91f, 12.57f, 15.74f,  1.43f, 19.77f, 14.85f, 16.50f,
    17.51f,  5.13f,  0.22f, 12.50f, 12.17f, 19.51f, 18.98f, 12.26f,  1.91f
};

static const float catalog_dec[CATALOG_SIZE] = {
   -16.72f, -52.70f, -60.83f,  19.18f,  38.78f,  46.00f,  -8.20f,   5.22f, -57.24f,   7.41f,
   -60.37f,   8.87f, -63.10f,  16.51f, -26.43f, -11.16f,  28.03f, -29.62f,  45.28f, -59.69f,
    11.97f, -28.97f,  31.89f, -37.10f, -57.11f,   6.35f,  28.61f, -69.72f,  -1.20f, -46.96f,
    -1.94f,  55.96f, -47.34f,  61.75f,  49.86f, -26.39f, -34.38f, -59.51f,  49.31f, -42.99f,
    44.95f, -69.03f,  16.40f, -56.74f, -17.96f,  -8.66f,  23.46f,  89.26f, -17.99f, -26.30f,
    29.09f,  35.62f,  12.56f,  74.16f,  -9.67f,  40.96f,  14.57f,  40.26f, -43.43f,  -0.30f,
    56.54f,  51.49f,  26.71f, -40.00f,  42.33f,  59.15f,  54.93f,  19.84f, -47.39f, -22.62f,
    27.07f,  56.38f,   9.88f,  28.08f, -15.72f,  53.69f, -29.30f,  33.97f,  60.72f,  15.21f,
    20.52f, -17.54f,  -9.38f,  20.81f, -23.40f,   6.43f,  60.24f,  10.61f, -16.04f,  21.49f,
    52.30f,  -5.09f,  15.18f, -16.52f, -22.62f,  27.96f,  32.69f,  57.03f,  63.67f
};

static const float catalog_mag[CATALOG_SIZE] = {
    -1.46f, -0.74f, -0.27f, -0.05f,  0.03f,  0.08f,  0.13f,  0.34f,  0.46f,  0.50f,
     0.61f,  0.77f,  0.77f,  0.85f,  0.96f,  0.97f,  1.14f,  1.16f,  1.25f,  1.25f,
     1.35f,  1.50f,  1.58f,  1.63f,  1.63f,  1.64f,  1.65f,  1.68f,  1.69f,  1.74f,
     1.77f,  1.77f,  1.78f,  1.79f,  1.79f,  1.84f,  1.85f,  1.86f,  1.86f,  1.87f,
     1.90f,  1.92f,  1.93f,  1.94f,  1.98f,  1.98f,  2.00f,  2.02f,  2.02f,  2.02f,
     2.06f,  2.06f,  2.07f,  2.08f,  2.09f,  2.12f,  2.14f,  2.20f,  2.21f,  2.23f,
     2.23f,  2.23f,  2.23f,  2.25f,  2.26f,  2.27f,  2.27f,  2.28f,  2.30f,  2.32f,
     2.37f,  2.37f,  2.39f,  2.42f,  2.43f,  2.44f,  2.45f,  2.46f,  2.47f,  2.49f,
     2.56f,  2.59f,  2.61f,  2.64f,  2.65f,  2.65f,  2.68f,  2.72f,  2.75f,  2.77f,
     2.79f,  2.79f,  2.83f,  2.95f,  3.02f,  3.08f,  3.24f,  3.31f,  3.37f
};

static const float catalog_bv[CATALOG_SIZE] = {
     0.00f,  0.15f,  0.71f,  1.23f,  0.00f,  0.80f, -0.03f,  0.42f, -0.16f,  1.85f,
    -0.23f,  0.22f, -0.24f,  1.54f,  1.83f, -0.23f,  1.00f,  0.09f,  0.09f, -0.23f,
    -0.11f, -0.21f,  0.03f, -0.22f,  1.59f, -0.22f, -0.13f,  0.07f, -0.18f, -0.07f,
    -0.21f, -0.02f, -0.22f,  1.07f,  0.48f,  0.67f, -0.03f,  1.28f, -0.10f,  0.40f,
     0.08f,  1.44f,  0.00f, -0.20f, -0.24f,  1.44f,  1.15f,  0.60f,  1.02f, -0.13f,
    -0.11f,  1.58f,  0.15f,  1.47f, -0.18f, -0.05f,  0.09f,  0.67f,  1.66f, -0.21f,
     1.17f,  1.52f,  0.03f, -0.27f,  1.37f,  0.34f,  0.02f,  1.14f, -0.20f, -0.12f,
     0.97f,  0.03f,  1.53f,  1.67f,  0.06f,  0.04f, -0.09f,  1.03f, -0.15f, -0.04f,
     0.13f, -0.11f, -0.11f,  0.17f,  0.89f,  1.17f,  0.13f,  1.51f,  0.15f,  0.94f,
     0.98f,  0.13f, -0.22f, -0.05f,  1.33f,  0.86f, -0.05f,  0.08f, -0.15f
};

static const char *catalog_names[CATALOG_SIZE] = {
    "Sirius",      "Canopus",     "Rigil Kent",  "Arcturus",    "Vega",
    "Capella",     "Rigel",       "Procyon",      "Achernar",    "Betelgeuse",
    "Hadar",       "Altair",      "Acrux",        "Aldebaran",   "Antares",
    "Spica",       "Pollux",      "Fomalhaut",    "Deneb",       "Mimosa",
    "Regulus",     "Adhara",      "Castor",       "Shaula",      "Gacrux",
    "Bellatrix",   "Elnath",      "Miaplacidus",  "Alnilam",     "Alnair",
    "Alnitak",     "Alioth",      "Regor",        "Dubhe",       "Mirfak",
    "Wezen",       "Kaus Australis", "Avior",     "Alkaid",      "Sargas",
    "Menkalinan",  "Atria",       "Alhena",       "Peacock",     "Mirzam",
    "Alphard",     "Hamal",       "Polaris",      "Diphda",      "Nunki",
    "Alpheratz",   "Mirach",      "Rasalhague",   "Kochab",      "Saiph",
    "Algol",       "Denebola",    "Sadr",         "Suhail",      "Mintaka",
    "Schedar",     "Eltanin",     "Alphecca",     "Naos",        "Almach",
    "Caph",        "Mizar",       "Algieba",      "Alpha Lupi",  "Dschubba",
    "Izar",        "Merak",       "Enif",         "Scheat",      "Sabik",
    "Phecda",      "Aludra",      "Gienah Cyg",   "Navi",        "Markab",
    "Zosma",       "Gienah Crv",  "Zubeneschamali","Sheratan",   "Kraz",
    "Unukalhai",   "Ruchbah",     "Tarazed",      "Zubenelgenubi","Kornephoros",
    "Rastaban",    "Cursa",       "Algenib",      "Algorab",     "Minkar",
    "Albireo",     "Sulafat",     "Megrez",       "Segin"
};

/* ──────────────────────────────────────────────────────────────────────────
 * B-V color index to RGB — piecewise linear interpolation.
 * Anchor points from stellar spectral classification.
 * ────────────────────────────────────────────────────────────────────── */

#define BV_ANCHORS 9

static const float bv_anchor_bv[BV_ANCHORS] = {
    -0.40f, -0.10f,  0.00f,  0.30f,  0.60f,  0.80f,  1.15f,  1.50f,  2.00f
};
static const float bv_anchor_r[BV_ANCHORS] = {
     0.55f,  0.70f,  0.85f,  1.00f,  1.00f,  1.00f,  1.00f,  1.00f,  1.00f
};
static const float bv_anchor_g[BV_ANCHORS] = {
     0.60f,  0.80f,  0.90f,  0.98f,  0.93f,  0.85f,  0.72f,  0.58f,  0.42f
};
static const float bv_anchor_b[BV_ANCHORS] = {
     1.00f,  1.00f,  1.00f,  0.95f,  0.78f,  0.62f,  0.42f,  0.28f,  0.20f
};

/* ──────────────────────────────────────────────────────────────────────── */

int star_catalog_count(void)
{
    return CATALOG_SIZE;
}

star_entry_t star_catalog_entry(int index)
{
    star_entry_t e;
    if (index < 0 || index >= CATALOG_SIZE) {
        e.ra_hours = 0.0f;
        e.dec_deg = 0.0f;
        e.magnitude = 99.0f;
        e.bv = 0.0f;
        return e;
    }
    e.ra_hours = catalog_ra[index];
    e.dec_deg = catalog_dec[index];
    e.magnitude = catalog_mag[index];
    e.bv = catalog_bv[index];
    return e;
}

const char *star_catalog_name(int index)
{
    if (index < 0 || index >= CATALOG_SIZE) return (void *)0;
    return catalog_names[index];
}

star_xyz_t star_to_equatorial_xyz(float ra_hours, float dec_deg)
{
    star_xyz_t p;
    float ra_rad = ra_hours * (float)PI / 12.0f;
    float dec_rad = dec_deg * (float)PI / 180.0f;
    float cd = cosf(dec_rad);
    p.x = cd * cosf(ra_rad);
    p.y = cd * sinf(ra_rad);
    p.z = sinf(dec_rad);
    return p;
}

star_xyz_t star_to_ecliptic_xyz(float ra_hours, float dec_deg, float obliquity_deg)
{
    /* Step 1: equatorial Cartesian */
    star_xyz_t eq = star_to_equatorial_xyz(ra_hours, dec_deg);

    /* Step 2: rotate around X-axis by obliquity (equatorial -> ecliptic) */
    float obl = obliquity_deg * (float)PI / 180.0f;
    float co = cosf(obl);
    float so = sinf(obl);
    float x_ecl = eq.x;
    float y_ecl = eq.y * co + eq.z * so;
    float z_ecl = -eq.y * so + eq.z * co;

    /* Step 3: project convention — XZ = ecliptic plane, Y = ecliptic north */
    star_xyz_t p;
    p.x = x_ecl;
    p.y = z_ecl;
    p.z = y_ecl;
    return p;
}

void star_bv_to_rgb(float bv, float *r, float *g, float *b)
{
    /* Clamp to anchor range */
    if (bv <= bv_anchor_bv[0]) {
        *r = bv_anchor_r[0];
        *g = bv_anchor_g[0];
        *b = bv_anchor_b[0];
        return;
    }
    if (bv >= bv_anchor_bv[BV_ANCHORS - 1]) {
        *r = bv_anchor_r[BV_ANCHORS - 1];
        *g = bv_anchor_g[BV_ANCHORS - 1];
        *b = bv_anchor_b[BV_ANCHORS - 1];
        return;
    }

    /* Find segment and interpolate */
    for (int i = 0; i < BV_ANCHORS - 1; i++) {
        if (bv <= bv_anchor_bv[i + 1]) {
            float t = (bv - bv_anchor_bv[i]) / (bv_anchor_bv[i + 1] - bv_anchor_bv[i]);
            *r = bv_anchor_r[i] + t * (bv_anchor_r[i + 1] - bv_anchor_r[i]);
            *g = bv_anchor_g[i] + t * (bv_anchor_g[i + 1] - bv_anchor_g[i]);
            *b = bv_anchor_b[i] + t * (bv_anchor_b[i + 1] - bv_anchor_b[i]);
            return;
        }
    }

    /* Fallback (should not reach) */
    *r = 1.0f;
    *g = 1.0f;
    *b = 1.0f;
}

float star_mag_to_size(float magnitude, float base_size)
{
    /* Astronomical brightness: each magnitude step = 2.512x brightness.
     * Size proportional to sqrt(brightness) for area perception.
     * size = base_size * 10^(-0.2 * magnitude) */
    float exponent = -0.2f * magnitude;
    return base_size * powf(10.0f, exponent);
}
