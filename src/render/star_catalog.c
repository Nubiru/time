#include "star_catalog.h"

#include <math.h>

#define PI 3.14159265358979323846

/* ──────────────────────────────────────────────────────────────────────────
 * Bright star catalog — 195 stars.
 * Indices 0-98: original 99 brightest, sorted by V magnitude.
 * Indices 99-194: extended set (supplementary stars for constellations
 *   and sky coverage). Sorted by V magnitude within this range.
 * Data: RA (hours J2000), Dec (degrees J2000), V magnitude, B-V color.
 *
 * Index reference (0-98: original):
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
 *
 * Index reference (99-194: extended, sorted by V magnitude):
 *  99 Menkent      100 Delta Vel    101 Ankaa        102 Eta Cen
 * 103 Kappa Sco    104 Epsilon Sco  105 Acrab        106 Phact
 * 107 Eta Boo      108 Iota Aur     109 Mu Vel       110 Lesath
 * 111 Kaus Media   112 Porrima      113 Eta Dra      114 Iota Cen
 * 115 Theta Car    116 Gamma Lup    117 Iota Ori     118 Zeta Her
 * 119 Rutilicus    120 Lambda Sgr   121 Tau Sco      122 Vindemiatrix
 * 123 Sigma Sco    124 Alcyone      125 Tejat        126 Pi Sco
 * 127 Delta Crv    128 Eta Peg      129 Epsilon Gem  130 Gamma Hya
 * 131 Zeta Tau     132 Mu Cen       133 Mu Sco       134 Pherkad
 * 135 Mu UMa       136 Mebsuta     137 Delta Dra    138 Ascella
 * 139 Nu Hya       140 Eta Sgr     141 Nasl         142 Wazn
 * 143 Iota UMa     144 Delta Her   145 Pi Her       146 Nu Pup
 * 147 Theta Aur    148 Phi Sgr     149 Alpha Lyn    150 Iota Dra
 * 151 Kaus Bor.    152 Delta Aql   153 Muscida      154 Heze
 * 155 Wasat        156 Ain         157 Zeta Peg     158 Al Niyat
 * 159 Mu Her       160 Eta Cas     161 Lambda Aql   162 Delta Leo
 * 163 Gamma Ara    164 Alula Bor.  165 Thuban       166 Propus
 * 167 Delta Cap    168 Gamma CrB   169 Giausar      170 Theta Her
 * 171 Rho Sco      172 Tau Her     173 Epsilon Her  174 Gamma Tri
 * 175 Theta Boo    176 Epsilon CrB 177 Yildun       178 Delta Cas B
 * 179 Alpha TrA    180 Nu Cen      181 Xi Pup       182 Omega CMa
 * 183 Tau Pup      184 Gamma Psc   185 Omega Cap    186 Mu Leo
 * 187 Eps Peg      188 Theta Peg   189 Iota Psc     190 Beta Tri
 * 191 Gamma Cap    192 Beta Eri    193 Mu Sgr       194 Sigma Pup
 * ────────────────────────────────────────────────────────────────────── */

#define CATALOG_SIZE 195

static const float catalog_ra[CATALOG_SIZE] = {
    /* 0-9 */
     6.75f,  6.40f, 14.66f, 14.26f, 18.62f,  5.28f,  5.24f,  7.65f,  1.63f,  5.92f,
    /* 10-19 */
    14.06f, 19.85f, 12.44f,  4.60f, 16.49f, 13.42f,  7.76f, 22.96f, 20.69f, 12.80f,
    /* 20-29 */
    10.14f,  6.98f,  7.58f, 17.56f, 12.52f,  5.42f,  5.44f,  9.22f,  5.60f, 22.14f,
    /* 30-39 */
     5.68f, 12.90f,  8.16f, 11.06f,  3.41f,  7.14f, 18.40f,  8.38f, 13.79f, 17.62f,
    /* 40-49 */
     6.00f, 16.81f,  6.63f, 20.43f,  6.38f,  9.46f,  2.12f,  2.53f,  0.73f, 18.92f,
    /* 50-59 */
     0.14f,  1.16f, 17.58f, 14.85f,  5.80f,  3.14f, 11.82f, 20.37f,  9.13f,  5.53f,
    /* 60-69 */
     0.68f, 17.94f, 15.58f,  8.06f,  2.07f,  0.15f, 13.40f, 10.33f, 14.70f, 16.01f,
    /* 70-79 */
    14.75f, 11.03f, 21.74f, 23.06f, 17.17f, 11.90f,  7.40f, 20.77f,  0.95f, 23.08f,
    /* 80-89 */
    11.24f, 12.26f, 15.28f,  1.91f, 12.57f, 15.74f,  1.43f, 19.77f, 14.85f, 16.50f,
    /* 90-98 */
    17.51f,  5.13f,  0.22f, 12.50f, 12.17f, 19.51f, 18.98f, 12.26f,  1.91f,
    /* ---- Extended (99-194) sorted by V magnitude ---- */
    14.11f, /*  99 Menkent       V=2.06 */
     8.74f, /* 100 Delta Vel     V=1.96 */
     0.44f, /* 101 Ankaa         V=2.37 */
    14.59f, /* 102 Eta Cen       V=2.35 */
    17.71f, /* 103 Kappa Sco     V=2.39 */
    16.84f, /* 104 Epsilon Sco   V=2.29 */
    16.09f, /* 105 Acrab         V=2.62 */
     5.66f, /* 106 Phact         V=2.64 */
    13.91f, /* 107 Eta Boo       V=2.68 */
     4.95f, /* 108 Iota Aur      V=2.69 */
    10.78f, /* 109 Mu Vel        V=2.69 */
    17.53f, /* 110 Lesath        V=2.69 */
    18.35f, /* 111 Kaus Media    V=2.70 */
    12.69f, /* 112 Porrima       V=2.74 */
    16.40f, /* 113 Eta Dra       V=2.74 */
    13.34f, /* 114 Iota Cen      V=2.75 */
    10.72f, /* 115 Theta Car     V=2.76 */
    15.59f, /* 116 Gamma Lup     V=2.78 */
     5.59f, /* 117 Iota Ori      V=2.77 */
    16.69f, /* 118 Zeta Her      V=2.81 */
    17.25f, /* 119 Rutilicus     V=2.81 */
    18.47f, /* 120 Lambda Sgr    V=2.81 */
    16.60f, /* 121 Tau Sco       V=2.82 */
    13.04f, /* 122 Vindemiatrix  V=2.83 */
    16.35f, /* 123 Sigma Sco     V=2.88 */
     3.79f, /* 124 Alcyone       V=2.87 */
     6.38f, /* 125 Tejat         V=2.88 */
    15.98f, /* 126 Pi Sco        V=2.89 */
    12.33f, /* 127 Eta Vir        V=3.89 */
    22.72f, /* 128 Eta Peg       V=2.94 */
     6.73f, /* 129 Epsilon Gem   V=2.98 */
    13.32f, /* 130 Gamma Hya     V=3.00 */
     5.63f, /* 131 Zeta Tau      V=3.03 */
    13.83f, /* 132 Mu Cen        V=3.04 */
    16.87f, /* 133 Mu Sco        V=3.04 */
    15.35f, /* 134 Pherkad       V=3.05 */
    10.37f, /* 135 Mu UMa        V=3.05 */
     6.72f, /* 136 Mebsuta       V=3.06 */
    19.21f, /* 137 Delta Dra     V=3.07 */
    19.04f, /* 138 Ascella        V=3.10 */
    10.83f, /* 139 Nu Hya        V=3.11 */
    18.29f, /* 140 Eta Sgr       V=3.11 */
    18.10f, /* 141 Nasl          V=3.11 */
     5.85f, /* 142 Wazn          V=3.12 */
     8.99f, /* 143 Iota UMa      V=3.14 */
    16.71f, /* 144 Eta Her        V=3.49 */
    17.25f, /* 145 Pi Her        V=3.16 */
     6.63f, /* 146 Nu Pup        V=3.17 */
     5.99f, /* 147 Theta Aur     V=3.17 */
    18.76f, /* 148 Phi Sgr       V=3.17 */
     9.35f, /* 149 Alpha Lyn     V=3.13 */
    15.42f, /* 150 Iota Dra      V=3.29 */
    18.45f, /* 151 Kaus Bor.     V=3.34 */
    19.43f, /* 152 Delta Aql     V=3.36 */
     8.51f, /* 153 Muscida       V=3.36 */
    13.58f, /* 154 Heze          V=3.37 */
     7.34f, /* 155 Wasat         V=3.53 */
     4.48f, /* 156 Ain           V=3.53 */
    22.69f, /* 157 Zeta Peg      V=3.40 */
    16.11f, /* 158 Omega Sco     V=3.96 */
    17.77f, /* 159 Mu Her        V=3.42 */
     0.83f, /* 160 Eta Cas       V=3.44 */
    19.10f, /* 161 Lambda Aql    V=3.44 */
     9.76f, /* 162 Epsilon Leo   V=2.98 */
    17.42f, /* 163 Gamma Ara     V=3.34 */
    11.31f, /* 164 Alula Bor.    V=3.49 */
    14.07f, /* 165 Thuban        V=3.65 */
     6.25f, /* 166 Propus        V=3.36 */
    21.78f, /* 167 Delta Cap     V=3.57 */
    15.71f, /* 168 Gamma CrB     V=3.84 */
    11.52f, /* 169 Giausar       V=3.85 */
    17.94f, /* 170 Theta Her     V=3.86 */
    15.95f, /* 171 Rho Sco       V=3.88 */
    16.33f, /* 172 Tau Her       V=3.89 */
    17.00f, /* 173 Epsilon Her   V=3.92 */
     2.29f, /* 174 Gamma Tri     V=4.01 */
    14.42f, /* 175 Theta Boo     V=4.05 */
    15.96f, /* 176 Epsilon CrB   V=4.15 */
    17.54f, /* 177 Yildun        V=4.36 */
     1.36f, /* 178 Delta Cas B   V=3.68 */
    15.92f, /* 179 Beta TrA      V=2.85 */
    13.83f, /* 180 Nu Cen        V=3.41 */
     7.29f, /* 181 Xi Pup        V=3.34 */
     7.03f, /* 182 Omega CMa     V=3.85 */
     8.17f, /* 183 Tau Pup       V=2.93 */
    23.29f, /* 184 Gamma Psc     V=3.69 */
    20.46f, /* 185 Omega Cap     V=4.12 */
     9.88f, /* 186 Mu Leo        V=3.88 */
    22.83f, /* 187 Mu Peg        V=3.48 */
    22.17f, /* 188 Theta Peg     V=3.53 */
    23.66f, /* 189 Iota Psc      V=4.13 */
     2.16f, /* 190 Beta Tri      V=3.00 */
    21.62f, /* 191 Gamma Cap     V=3.68 */
     2.94f, /* 192 Eta Eri       V=3.89 */
    18.23f, /* 193 Mu Sgr        V=3.86 */
     7.29f  /* 194 Sigma Pup     V=3.25 */
};

static const float catalog_dec[CATALOG_SIZE] = {
    /* 0-9 */
   -16.72f, -52.70f, -60.83f,  19.18f,  38.78f,  46.00f,  -8.20f,   5.22f, -57.24f,   7.41f,
    /* 10-19 */
   -60.37f,   8.87f, -63.10f,  16.51f, -26.43f, -11.16f,  28.03f, -29.62f,  45.28f, -59.69f,
    /* 20-29 */
    11.97f, -28.97f,  31.89f, -37.10f, -57.11f,   6.35f,  28.61f, -69.72f,  -1.20f, -46.96f,
    /* 30-39 */
    -1.94f,  55.96f, -47.34f,  61.75f,  49.86f, -26.39f, -34.38f, -59.51f,  49.31f, -42.99f,
    /* 40-49 */
    44.95f, -69.03f,  16.40f, -56.74f, -17.96f,  -8.66f,  23.46f,  89.26f, -17.99f, -26.30f,
    /* 50-59 */
    29.09f,  35.62f,  12.56f,  74.16f,  -9.67f,  40.96f,  14.57f,  40.26f, -43.43f,  -0.30f,
    /* 60-69 */
    56.54f,  51.49f,  26.71f, -40.00f,  42.33f,  59.15f,  54.93f,  19.84f, -47.39f, -22.62f,
    /* 70-79 */
    27.07f,  56.38f,   9.88f,  28.08f, -15.72f,  53.69f, -29.30f,  33.97f,  60.72f,  15.21f,
    /* 80-89 */
    20.52f, -17.54f,  -9.38f,  20.81f, -23.40f,   6.43f,  60.24f,  10.61f, -16.04f,  21.49f,
    /* 90-98 */
    52.30f,  -5.09f,  15.18f, -16.52f, -22.62f,  27.96f,  32.69f,  57.03f,  63.67f,
    /* ---- Extended (99-194) ---- */
   -36.37f, /*  99 Menkent */
   -54.71f, /* 100 Delta Vel */
   -42.31f, /* 101 Ankaa */
   -42.16f, /* 102 Eta Cen */
   -39.03f, /* 103 Kappa Sco */
   -34.29f, /* 104 Epsilon Sco */
   -19.81f, /* 105 Acrab */
   -34.07f, /* 106 Phact */
    18.40f, /* 107 Eta Boo */
    33.17f, /* 108 Iota Aur */
   -49.42f, /* 109 Mu Vel */
   -37.29f, /* 110 Lesath */
   -29.83f, /* 111 Kaus Media */
    -1.45f, /* 112 Porrima */
    61.51f, /* 113 Eta Dra */
   -36.71f, /* 114 Iota Cen */
   -64.39f, /* 115 Theta Car */
   -41.17f, /* 116 Gamma Lup */
    -5.91f, /* 117 Iota Ori */
    31.60f, /* 118 Zeta Her */
    24.84f, /* 119 Rutilicus */
   -25.42f, /* 120 Lambda Sgr */
   -28.22f, /* 121 Tau Sco */
    10.96f, /* 122 Vindemiatrix */
   -25.59f, /* 123 Sigma Sco */
    24.11f, /* 124 Alcyone */
    22.51f, /* 125 Tejat */
   -26.11f, /* 126 Pi Sco */
    -0.67f, /* 127 Eta Vir */
    30.22f, /* 128 Eta Peg */
    25.13f, /* 129 Epsilon Gem */
   -23.17f, /* 130 Gamma Hya */
    21.14f, /* 131 Zeta Tau */
   -42.47f, /* 132 Mu Cen */
   -38.05f, /* 133 Mu Sco */
    71.83f, /* 134 Pherkad */
    41.50f, /* 135 Mu UMa */
    25.13f, /* 136 Mebsuta */
    67.66f, /* 137 Delta Dra */
   -29.88f, /* 138 Ascella */
   -16.19f, /* 139 Nu Hya */
   -36.76f, /* 140 Eta Sgr */
   -30.42f, /* 141 Nasl */
   -35.77f, /* 142 Wazn */
    48.04f, /* 143 Iota UMa */
    38.92f, /* 144 Eta Her */
    36.81f, /* 145 Pi Her */
   -43.20f, /* 146 Nu Pup */
    37.21f, /* 147 Theta Aur */
   -26.99f, /* 148 Phi Sgr */
    34.39f, /* 149 Alpha Lyn */
    58.97f, /* 150 Iota Dra */
   -25.42f, /* 151 Kaus Bor. */
     3.11f, /* 152 Delta Aql */
    60.72f, /* 153 Muscida */
    -0.60f, /* 154 Heze */
    21.98f, /* 155 Wasat */
    19.18f, /* 156 Ain */
    10.83f, /* 157 Zeta Peg */
   -20.67f, /* 158 Omega Sco */
    27.72f, /* 159 Mu Her */
    57.82f, /* 160 Eta Cas */
    -4.88f, /* 161 Lambda Aql */
    23.77f, /* 162 Epsilon Leo */
   -56.38f, /* 163 Gamma Ara */
    44.50f, /* 164 Alula Bor. */
    64.38f, /* 165 Thuban */
    22.51f, /* 166 Propus */
   -16.13f, /* 167 Delta Cap */
    26.30f, /* 168 Gamma CrB */
    69.33f, /* 169 Giausar */
    37.25f, /* 170 Theta Her */
   -29.21f, /* 171 Rho Sco */
    46.31f, /* 172 Tau Her */
    30.93f, /* 173 Epsilon Her */
    33.85f, /* 174 Gamma Tri */
    51.85f, /* 175 Theta Boo */
    26.88f, /* 176 Epsilon CrB */
    86.59f, /* 177 Yildun */
    59.95f, /* 178 Delta Cas B */
   -63.43f, /* 179 Beta TrA */
   -41.18f, /* 180 Nu Cen */
   -24.86f, /* 181 Xi Pup */
   -26.39f, /* 182 Omega CMa */
   -50.61f, /* 183 Tau Pup */
     3.28f, /* 184 Gamma Psc */
   -26.92f, /* 185 Omega Cap */
    26.01f, /* 186 Mu Leo */
    24.60f, /* 187 Mu Peg */
    23.74f, /* 188 Theta Peg */
     5.63f, /* 189 Iota Psc */
    34.99f, /* 190 Beta Tri */
   -16.66f, /* 191 Gamma Cap */
    -8.90f, /* 192 Eta Eri */
   -21.06f, /* 193 Mu Sgr */
   -43.30f  /* 194 Sigma Pup */
};

static const float catalog_mag[CATALOG_SIZE] = {
    /* 0-9 */
    -1.46f, -0.74f, -0.27f, -0.05f,  0.03f,  0.08f,  0.13f,  0.34f,  0.46f,  0.50f,
    /* 10-19 */
     0.61f,  0.77f,  0.77f,  0.85f,  0.96f,  0.97f,  1.14f,  1.16f,  1.25f,  1.25f,
    /* 20-29 */
     1.35f,  1.50f,  1.58f,  1.63f,  1.63f,  1.64f,  1.65f,  1.68f,  1.69f,  1.74f,
    /* 30-39 */
     1.77f,  1.77f,  1.78f,  1.79f,  1.79f,  1.84f,  1.85f,  1.86f,  1.86f,  1.87f,
    /* 40-49 */
     1.90f,  1.92f,  1.93f,  1.94f,  1.98f,  1.98f,  2.00f,  2.02f,  2.02f,  2.02f,
    /* 50-59 */
     2.06f,  2.06f,  2.07f,  2.08f,  2.09f,  2.12f,  2.14f,  2.20f,  2.21f,  2.23f,
    /* 60-69 */
     2.23f,  2.23f,  2.23f,  2.25f,  2.26f,  2.27f,  2.27f,  2.28f,  2.30f,  2.32f,
    /* 70-79 */
     2.37f,  2.37f,  2.39f,  2.42f,  2.43f,  2.44f,  2.45f,  2.46f,  2.47f,  2.49f,
    /* 80-89 */
     2.56f,  2.59f,  2.61f,  2.64f,  2.65f,  2.65f,  2.68f,  2.72f,  2.75f,  2.77f,
    /* 90-98 */
     2.79f,  2.79f,  2.83f,  2.95f,  3.02f,  3.08f,  3.24f,  3.31f,  3.37f,
    /* ---- Extended (99-194) sorted by V magnitude ---- */
     2.06f, /*  99 Menkent */
     1.96f, /* 100 Delta Vel */
     2.37f, /* 101 Ankaa */
     2.35f, /* 102 Eta Cen */
     2.39f, /* 103 Kappa Sco */
     2.29f, /* 104 Epsilon Sco */
     2.62f, /* 105 Acrab */
     2.64f, /* 106 Phact */
     2.68f, /* 107 Eta Boo */
     2.69f, /* 108 Iota Aur */
     2.69f, /* 109 Mu Vel */
     2.69f, /* 110 Lesath */
     2.70f, /* 111 Kaus Media */
     2.74f, /* 112 Porrima */
     2.74f, /* 113 Eta Dra */
     2.75f, /* 114 Iota Cen */
     2.76f, /* 115 Theta Car */
     2.78f, /* 116 Gamma Lup */
     2.77f, /* 117 Iota Ori */
     2.81f, /* 118 Zeta Her */
     2.81f, /* 119 Rutilicus */
     2.81f, /* 120 Lambda Sgr */
     2.82f, /* 121 Tau Sco */
     2.83f, /* 122 Vindemiatrix */
     2.88f, /* 123 Sigma Sco */
     2.87f, /* 124 Alcyone */
     2.88f, /* 125 Tejat */
     2.89f, /* 126 Pi Sco */
     3.89f, /* 127 Eta Vir */
     2.94f, /* 128 Eta Peg */
     2.98f, /* 129 Epsilon Gem */
     3.00f, /* 130 Gamma Hya */
     3.03f, /* 131 Zeta Tau */
     3.04f, /* 132 Mu Cen */
     3.04f, /* 133 Mu Sco */
     3.05f, /* 134 Pherkad */
     3.05f, /* 135 Mu UMa */
     3.06f, /* 136 Mebsuta */
     3.07f, /* 137 Delta Dra */
     3.10f, /* 138 Ascella */
     3.11f, /* 139 Nu Hya */
     3.11f, /* 140 Eta Sgr */
     3.11f, /* 141 Nasl */
     3.12f, /* 142 Wazn */
     3.14f, /* 143 Iota UMa */
     3.49f, /* 144 Eta Her */
     3.16f, /* 145 Pi Her */
     3.17f, /* 146 Nu Pup */
     3.17f, /* 147 Theta Aur */
     3.17f, /* 148 Phi Sgr */
     3.13f, /* 149 Alpha Lyn */
     3.29f, /* 150 Iota Dra */
     3.34f, /* 151 Kaus Bor. */
     3.36f, /* 152 Delta Aql */
     3.36f, /* 153 Muscida */
     3.37f, /* 154 Heze */
     3.53f, /* 155 Wasat */
     3.53f, /* 156 Ain */
     3.40f, /* 157 Zeta Peg */
     3.96f, /* 158 Omega Sco */
     3.42f, /* 159 Mu Her */
     3.44f, /* 160 Eta Cas */
     3.44f, /* 161 Lambda Aql */
     2.98f, /* 162 Epsilon Leo */
     3.34f, /* 163 Gamma Ara */
     3.49f, /* 164 Alula Bor. */
     3.65f, /* 165 Thuban */
     3.36f, /* 166 Propus */
     3.57f, /* 167 Delta Cap */
     3.84f, /* 168 Gamma CrB */
     3.85f, /* 169 Giausar */
     3.86f, /* 170 Theta Her */
     3.88f, /* 171 Rho Sco */
     3.89f, /* 172 Tau Her */
     3.92f, /* 173 Epsilon Her */
     4.01f, /* 174 Gamma Tri */
     4.05f, /* 175 Theta Boo */
     4.15f, /* 176 Epsilon CrB */
     4.36f, /* 177 Yildun */
     3.68f, /* 178 Delta Cas B */
     2.85f, /* 179 Beta TrA */
     3.41f, /* 180 Nu Cen */
     3.34f, /* 181 Xi Pup */
     3.85f, /* 182 Omega CMa */
     2.93f, /* 183 Tau Pup */
     3.69f, /* 184 Gamma Psc */
     4.12f, /* 185 Omega Cap */
     3.88f, /* 186 Mu Leo */
     3.48f, /* 187 Mu Peg */
     3.53f, /* 188 Theta Peg */
     4.13f, /* 189 Iota Psc */
     3.00f, /* 190 Beta Tri */
     3.68f, /* 191 Gamma Cap */
     3.89f, /* 192 Eta Eri */
     3.86f, /* 193 Mu Sgr */
     3.25f  /* 194 Sigma Pup */
};

static const float catalog_bv[CATALOG_SIZE] = {
    /* 0-9 */
     0.00f,  0.15f,  0.71f,  1.23f,  0.00f,  0.80f, -0.03f,  0.42f, -0.16f,  1.85f,
    /* 10-19 */
    -0.23f,  0.22f, -0.24f,  1.54f,  1.83f, -0.23f,  1.00f,  0.09f,  0.09f, -0.23f,
    /* 20-29 */
    -0.11f, -0.21f,  0.03f, -0.22f,  1.59f, -0.22f, -0.13f,  0.07f, -0.18f, -0.07f,
    /* 30-39 */
    -0.21f, -0.02f, -0.22f,  1.07f,  0.48f,  0.67f, -0.03f,  1.28f, -0.10f,  0.40f,
    /* 40-49 */
     0.08f,  1.44f,  0.00f, -0.20f, -0.24f,  1.44f,  1.15f,  0.60f,  1.02f, -0.13f,
    /* 50-59 */
    -0.11f,  1.58f,  0.15f,  1.47f, -0.18f, -0.05f,  0.09f,  0.67f,  1.66f, -0.21f,
    /* 60-69 */
     1.17f,  1.52f,  0.03f, -0.27f,  1.37f,  0.34f,  0.02f,  1.14f, -0.20f, -0.12f,
    /* 70-79 */
     0.97f,  0.03f,  1.53f,  1.67f,  0.06f,  0.04f, -0.09f,  1.03f, -0.15f, -0.04f,
    /* 80-89 */
     0.13f, -0.11f, -0.11f,  0.17f,  0.89f,  1.17f,  0.13f,  1.51f,  0.15f,  0.94f,
    /* 90-98 */
     0.98f,  0.13f, -0.22f, -0.05f,  1.33f,  0.86f, -0.05f,  0.08f, -0.15f,
    /* ---- Extended (99-194) ---- */
     1.01f, /*  99 Menkent */
     0.04f, /* 100 Delta Vel */
     1.09f, /* 101 Ankaa */
    -0.19f, /* 102 Eta Cen */
    -0.20f, /* 103 Kappa Sco */
     1.15f, /* 104 Epsilon Sco */
    -0.07f, /* 105 Acrab */
    -0.12f, /* 106 Phact */
     0.58f, /* 107 Eta Boo */
     1.53f, /* 108 Iota Aur */
     0.90f, /* 109 Mu Vel */
    -0.22f, /* 110 Lesath */
     1.38f, /* 111 Kaus Media */
     0.36f, /* 112 Porrima */
     0.91f, /* 113 Eta Dra */
    -0.05f, /* 114 Iota Cen */
     0.07f, /* 115 Theta Car */
    -0.20f, /* 116 Gamma Lup */
    -0.24f, /* 117 Iota Ori */
     0.65f, /* 118 Zeta Her */
     0.94f, /* 119 Rutilicus */
     1.04f, /* 120 Lambda Sgr */
    -0.25f, /* 121 Tau Sco */
     0.94f, /* 122 Vindemiatrix */
    -0.05f, /* 123 Sigma Sco */
    -0.09f, /* 124 Alcyone */
     1.64f, /* 125 Tejat */
    -0.19f, /* 126 Pi Sco */
     0.94f, /* 127 Eta Vir */
     0.95f, /* 128 Eta Peg */
     1.40f, /* 129 Epsilon Gem */
     0.92f, /* 130 Gamma Hya */
    -0.17f, /* 131 Zeta Tau */
    -0.22f, /* 132 Mu Cen */
    -0.20f, /* 133 Mu Sco */
     0.05f, /* 134 Pherkad */
     1.59f, /* 135 Mu UMa */
     1.44f, /* 136 Mebsuta */
     0.91f, /* 137 Delta Dra */
    -0.01f, /* 138 Ascella */
     1.04f, /* 139 Nu Hya */
     1.57f, /* 140 Eta Sgr */
     1.38f, /* 141 Nasl */
     0.82f, /* 142 Wazn */
     0.19f, /* 143 Iota UMa */
     0.94f, /* 144 Eta Her */
     1.04f, /* 145 Pi Her */
     1.57f, /* 146 Nu Pup */
     0.16f, /* 147 Theta Aur */
    -0.01f, /* 148 Phi Sgr */
     1.55f, /* 149 Alpha Lyn */
     1.17f, /* 150 Iota Dra */
     1.38f, /* 151 Kaus Bor. */
     0.32f, /* 152 Delta Aql */
     0.94f, /* 153 Muscida */
     0.11f, /* 154 Heze */
     0.34f, /* 155 Wasat */
     1.01f, /* 156 Ain */
    -0.07f, /* 157 Zeta Peg */
    -0.04f, /* 158 Omega Sco */
     0.94f, /* 159 Mu Her */
     0.57f, /* 160 Eta Cas */
     0.15f, /* 161 Lambda Aql */
     0.81f, /* 162 Epsilon Leo */
    -0.15f, /* 163 Gamma Ara */
     0.04f, /* 164 Alula Bor. */
    -0.05f, /* 165 Thuban */
     1.59f, /* 166 Propus */
     0.07f, /* 167 Delta Cap */
     0.06f, /* 168 Gamma CrB */
     1.52f, /* 169 Giausar */
     1.23f, /* 170 Theta Her */
    -0.17f, /* 171 Rho Sco */
    -0.05f, /* 172 Tau Her */
     0.04f, /* 173 Epsilon Her */
     0.02f, /* 174 Gamma Tri */
     0.50f, /* 175 Theta Boo */
     1.23f, /* 176 Epsilon CrB */
     0.02f, /* 177 Yildun */
    -0.07f, /* 178 Delta Cas B */
     0.29f, /* 179 Beta TrA */
    -0.22f, /* 180 Nu Cen */
    -0.26f, /* 181 Xi Pup */
    -0.08f, /* 182 Omega CMa */
     1.20f, /* 183 Tau Pup */
     0.93f, /* 184 Gamma Psc */
     0.92f, /* 185 Omega Cap */
     1.23f, /* 186 Mu Leo */
     0.93f, /* 187 Mu Peg */
     0.09f, /* 188 Theta Peg */
     0.56f, /* 189 Iota Psc */
     0.14f, /* 190 Beta Tri */
     0.07f, /* 191 Gamma Cap */
     1.23f, /* 192 Eta Eri */
    -0.01f, /* 193 Mu Sgr */
     1.57f  /* 194 Sigma Pup */
};

static const char *catalog_names[CATALOG_SIZE] = {
    /* 0-9 */
    "Sirius",      "Canopus",     "Rigil Kent",  "Arcturus",    "Vega",
    "Capella",     "Rigel",       "Procyon",      "Achernar",    "Betelgeuse",
    /* 10-19 */
    "Hadar",       "Altair",      "Acrux",        "Aldebaran",   "Antares",
    "Spica",       "Pollux",      "Fomalhaut",    "Deneb",       "Mimosa",
    /* 20-29 */
    "Regulus",     "Adhara",      "Castor",       "Shaula",      "Gacrux",
    "Bellatrix",   "Elnath",      "Miaplacidus",  "Alnilam",     "Alnair",
    /* 30-39 */
    "Alnitak",     "Alioth",      "Regor",        "Dubhe",       "Mirfak",
    "Wezen",       "Kaus Australis", "Avior",     "Alkaid",      "Sargas",
    /* 40-49 */
    "Menkalinan",  "Atria",       "Alhena",       "Peacock",     "Mirzam",
    "Alphard",     "Hamal",       "Polaris",      "Diphda",      "Nunki",
    /* 50-59 */
    "Alpheratz",   "Mirach",      "Rasalhague",   "Kochab",      "Saiph",
    "Algol",       "Denebola",    "Sadr",         "Suhail",      "Mintaka",
    /* 60-69 */
    "Schedar",     "Eltanin",     "Alphecca",     "Naos",        "Almach",
    "Caph",        "Mizar",       "Algieba",      "Alpha Lupi",  "Dschubba",
    /* 70-79 */
    "Izar",        "Merak",       "Enif",         "Scheat",      "Sabik",
    "Phecda",      "Aludra",      "Gienah Cyg",   "Navi",        "Markab",
    /* 80-89 */
    "Zosma",       "Gienah Crv",  "Zubeneschamali","Sheratan",   "Kraz",
    "Unukalhai",   "Ruchbah",     "Tarazed",      "Zubenelgenubi","Kornephoros",
    /* 90-98 */
    "Rastaban",    "Cursa",       "Algenib",      "Algorab",     "Minkar",
    "Albireo",     "Sulafat",     "Megrez",       "Segin",
    /* ---- Extended (99-194) ---- */
    "Menkent",           /*  99 */
    "Delta Velorum",     /* 100 */
    "Ankaa",             /* 101 */
    "Eta Centauri",      /* 102 */
    "Kappa Scorpii",     /* 103 */
    "Epsilon Scorpii",   /* 104 */
    "Acrab",             /* 105 */
    "Phact",             /* 106 */
    "Eta Bootis",        /* 107 */
    "Iota Aurigae",      /* 108 */
    "Mu Velorum",        /* 109 */
    "Lesath",            /* 110 */
    "Kaus Media",        /* 111 */
    "Porrima",           /* 112 */
    "Eta Draconis",      /* 113 */
    "Iota Centauri",     /* 114 */
    "Theta Carinae",     /* 115 */
    "Gamma Lupi",        /* 116 */
    "Iota Orionis",      /* 117 */
    "Zeta Herculis",     /* 118 */
    "Rutilicus",         /* 119 */
    "Lambda Sagittarii", /* 120 */
    "Tau Scorpii",       /* 121 */
    "Vindemiatrix",      /* 122 */
    "Sigma Scorpii",     /* 123 */
    "Alcyone",           /* 124 */
    "Tejat",             /* 125 */
    "Pi Scorpii",        /* 126 */
    "Eta Virginis",      /* 127 */
    "Eta Pegasi",        /* 128 */
    "Epsilon Geminorum", /* 129 */
    "Gamma Hydrae",      /* 130 */
    "Zeta Tauri",        /* 131 */
    "Mu Centauri",       /* 132 */
    "Mu Scorpii",        /* 133 */
    "Pherkad",           /* 134 */
    "Mu Ursae Majoris",  /* 135 */
    "Mebsuta",           /* 136 */
    "Delta Draconis",    /* 137 */
    "Ascella",           /* 138 */
    "Nu Hydrae",         /* 139 */
    "Eta Sagittarii",    /* 140 */
    "Nasl",              /* 141 */
    "Wazn",              /* 142 */
    "Iota Ursae Majoris",/* 143 */
    "Eta Herculis",      /* 144 */
    "Pi Herculis",       /* 145 */
    "Nu Puppis",         /* 146 */
    "Theta Aurigae",     /* 147 */
    "Phi Sagittarii",    /* 148 */
    "Alpha Lyncis",      /* 149 */
    "Iota Draconis",     /* 150 */
    "Kaus Borealis",     /* 151 */
    "Delta Aquilae",     /* 152 */
    "Muscida",           /* 153 */
    "Heze",              /* 154 */
    "Wasat",             /* 155 */
    "Ain",               /* 156 */
    "Zeta Pegasi",       /* 157 */
    "Omega Scorpii",     /* 158 */
    "Mu Herculis",       /* 159 */
    "Eta Cassiopeiae",   /* 160 */
    "Lambda Aquilae",    /* 161 */
    "Epsilon Leonis",    /* 162 */
    "Gamma Arae",        /* 163 */
    "Alula Borealis",    /* 164 */
    "Thuban",            /* 165 */
    "Propus",            /* 166 */
    "Delta Capricorni",  /* 167 */
    "Gamma Cor. Bor.",   /* 168 */
    "Giausar",           /* 169 */
    "Theta Herculis",    /* 170 */
    "Rho Scorpii",       /* 171 */
    "Tau Herculis",      /* 172 */
    "Epsilon Herculis",  /* 173 */
    "Gamma Trianguli",   /* 174 */
    "Theta Bootis",      /* 175 */
    "Epsilon Cor. Bor.", /* 176 */
    "Yildun",            /* 177 */
    "Delta Cas B",       /* 178 */
    "Beta Tri. Aust.",   /* 179 */
    "Nu Centauri",       /* 180 */
    "Xi Puppis",         /* 181 */
    "Omega Canis Maj.",  /* 182 */
    "Tau Puppis",        /* 183 */
    "Gamma Piscium",     /* 184 */
    "Omega Capricorni",  /* 185 */
    "Mu Leonis",         /* 186 */
    "Mu Pegasi",         /* 187 */
    "Theta Pegasi",      /* 188 */
    "Iota Piscium",      /* 189 */
    "Beta Trianguli",    /* 190 */
    "Gamma Capricorni",  /* 191 */
    "Eta Eridani",       /* 192 */
    "Mu Sagittarii",     /* 193 */
    "Sigma Puppis"       /* 194 */
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
