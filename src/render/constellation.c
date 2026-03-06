#include "constellation.h"

#include <math.h>

/* ──────────────────────────────────────────────────────────────────────────
 * Constellation stick figures — 40 constellations.
 * Star indices reference star_catalog (195-star expanded catalog).
 *
 * Index reference (from star_catalog.c):
 *  0 Sirius       1 Canopus      2 Rigil Kent   3 Arcturus     4 Vega
 *  5 Capella      6 Rigel        7 Procyon      8 Achernar     9 Betelgeuse
 * 10 Hadar       11 Altair      12 Acrux       13 Aldebaran   14 Antares
 * 15 Spica       16 Pollux      17 Fomalhaut   18 Deneb       19 Mimosa
 * 20 Regulus     21 Adhara      22 Castor      23 Shaula      24 Gacrux
 * 25 Bellatrix   26 Elnath      27 Miaplacidus 28 Alnilam     29 Alnair
 * 30 Alnitak     31 Alioth      32 Regor       33 Dubhe       34 Mirfak
 * 35 Wezen       36 Kaus Aust.  37 Avior       38 Alkaid      39 Sargas
 * 40 Menkalinan  41 Atria       42 Alhena      43 Peacock     44 Mirzam
 * 45 Alphard     46 Hamal       47 Polaris     48 Diphda      49 Nunki
 * 50 Alpheratz   51 Mirach      52 Rasalhague  53 Kochab      54 Saiph
 * 55 Algol       56 Denebola    57 Sadr        58 Suhail      59 Mintaka
 * 60 Schedar     61 Eltanin     62 Alphecca    63 Naos        64 Almach
 * 65 Caph        66 Mizar       67 Algieba     68 Alpha Lupi  69 Dschubba
 * 70 Izar        71 Merak       72 Enif        73 Scheat      74 Sabik
 * 75 Phecda      76 Aludra      77 Gienah Cyg  78 Navi        79 Markab
 * 80 Zosma       81 Gienah Crv  82 Zuben.mali  83 Sheratan    84 Kraz
 * 85 Unukalhai   86 Ruchbah     87 Tarazed     88 Zuben.nubi  89 Kornephoros
 * 90 Rastaban    91 Cursa       92 Algenib     93 Algorab     94 Minkar
 * 95 Albireo     96 Sulafat     97 Megrez      98 Segin
 *
 * Extended stars (99+):
 *  99 Menkent    100 Delta Vel  101 Ankaa      102 Eta Cen    103 Kappa Sco
 * 104 Eps Sco    105 Acrab      106 Phact      107 Eta Boo    108 Iota Aur
 * 109 Mu Vel     110 Lesath     111 Kaus Med.  112 Porrima    113 Eta Dra
 * 114 Iota Cen   115 Theta Car  116 Gamma Lup  117 Iota Ori   118 Zeta Her
 * 119 Rutilicus  120 Lambda Sgr 121 Tau Sco    122 Vindem.    123 Sigma Sco
 * 124 Alcyone    125 Tejat      126 Pi Sco     127 Eta Vir    128 Eta Peg
 * 129 Eps Gem    130 Gamma Hya  131 Zeta Tau   132 Mu Cen     133 Mu Sco
 * 134 Pherkad    135 Mu UMa     136 Mebsuta    137 Delta Dra  138 Ascella
 * 139 Nu Hya     140 Eta Sgr    141 Nasl       142 Wazn       143 Iota UMa
 * 144 Eta Her    145 Pi Her     146 Nu Pup     147 Theta Aur  148 Phi Sgr
 * 149 Alpha Lyn  150 Iota Dra   151 Kaus Bor.  152 Delta Aql  153 Muscida
 * 154 Heze       155 Wasat      156 Ain        157 Zeta Peg   158 Omega Sco
 * 159 Mu Her     160 Eta Cas    161 Lambda Aql 162 Eps Leo    163 Gamma Ara
 * 164 Alula Bor. 165 Thuban     166 Propus     167 Delta Cap  168 Gamma CrB
 * 169 Giausar    170 Theta Her  171 Rho Sco    172 Tau Her    173 Eps Her
 * 174 Gamma Tri  175 Theta Boo  176 Eps CrB    177 Yildun     178 Delta CasB
 * 179 Beta TrA   180 Nu Cen     181 Xi Pup     182 Omega CMa  183 Tau Pup
 * 184 Gamma Psc  185 Omega Cap  186 Mu Leo     187 Mu Peg     188 Theta Peg
 * 189 Iota Psc   190 Beta Tri   191 Gamma Cap  192 Eta Eri    193 Mu Sgr
 * 194 Sigma Pup
 * ────────────────────────────────────────────────────────────────────── */

typedef struct {
    const char *name;
    const char *abbr;
    int line_count;
    cline_t lines[CONSTELLATION_MAX_LINES];
} cdata_t;

#define NUM_CONSTELLATIONS 40

static const cdata_t constellations[NUM_CONSTELLATIONS] = {
    /* 0. Orion: 7 lines */
    { "Orion", "Ori", 7, {
        { 9, 25}, /* Betelgeuse - Bellatrix (shoulders) */
        {30, 28}, /* Alnitak - Alnilam (belt) */
        {28, 59}, /* Alnilam - Mintaka (belt) */
        { 9, 30}, /* Betelgeuse - Alnitak (left torso) */
        {25, 59}, /* Bellatrix - Mintaka (right torso) */
        {30, 54}, /* Alnitak - Saiph (left leg) */
        {59,  6}, /* Mintaka - Rigel (right leg) */
        {0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 1. Crux (Southern Cross): 2 lines */
    { "Crux", "Cru", 2, {
        {12, 24}, /* Acrux - Gacrux (vertical beam) */
        {12, 19}, /* Acrux - Mimosa (horizontal beam) */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 2. Canis Major: 3 lines */
    { "Canis Major", "CMa", 3, {
        { 0, 44}, /* Sirius - Mirzam */
        { 0, 21}, /* Sirius - Adhara */
        {21, 35}, /* Adhara - Wezen */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 3. Gemini: 2 lines */
    { "Gemini", "Gem", 2, {
        {22, 16}, /* Castor - Pollux (the twins) */
        {16, 42}, /* Pollux - Alhena */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 4. Scorpius: 3 lines */
    { "Scorpius", "Sco", 3, {
        {14, 69}, /* Antares - Dschubba (head) */
        {14, 23}, /* Antares - Shaula (tail) */
        {23, 39}, /* Shaula - Sargas (stinger) */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 5. Centaurus: 1 line */
    { "Centaurus", "Cen", 1, {
        { 2, 10}, /* Rigil Kent - Hadar */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 6. Carina: 2 lines */
    { "Carina", "Car", 2, {
        { 1, 37}, /* Canopus - Avior */
        {37, 27}, /* Avior - Miaplacidus */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 7. Taurus: 1 line */
    { "Taurus", "Tau", 1, {
        {13, 26}, /* Aldebaran - Elnath */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 8. Sagittarius: 1 line */
    { "Sagittarius", "Sgr", 1, {
        {36, 49}, /* Kaus Australis - Nunki */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 9. Ursa Major (Big Dipper): 7 lines */
    { "Ursa Major", "UMa", 7, {
        {33, 71}, /* Dubhe - Merak (back of bowl) */
        {71, 75}, /* Merak - Phecda (bottom) */
        {75, 97}, /* Phecda - Megrez (front of bowl) */
        {97, 33}, /* Megrez - Dubhe (top, closes bowl) */
        {97, 31}, /* Megrez - Alioth (handle start) */
        {31, 66}, /* Alioth - Mizar (handle middle) */
        {66, 38}, /* Mizar - Alkaid (handle end) */
        {0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 10. Auriga: 1 line */
    { "Auriga", "Aur", 1, {
        { 5, 40}, /* Capella - Menkalinan */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 11. Summer Triangle (asterism): 3 lines */
    { "Summer Triangle", "Ast", 3, {
        { 4, 18}, /* Vega - Deneb */
        {18, 11}, /* Deneb - Altair */
        {11,  4}, /* Altair - Vega */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 12. Cassiopeia: 4 lines (W shape) */
    { "Cassiopeia", "Cas", 4, {
        {98, 86}, /* Segin - Ruchbah */
        {86, 78}, /* Ruchbah - Navi */
        {78, 60}, /* Navi - Schedar */
        {60, 65}, /* Schedar - Caph */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 13. Cygnus (Northern Cross): 3 lines */
    { "Cygnus", "Cyg", 3, {
        {18, 57}, /* Deneb - Sadr (spine) */
        {57, 95}, /* Sadr - Albireo (spine) */
        {77, 57}, /* Gienah Cyg - Sadr (wing) */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 14. Pegasus (Great Square): 4 lines */
    { "Pegasus", "Peg", 4, {
        {79, 73}, /* Markab - Scheat */
        {73, 50}, /* Scheat - Alpheratz */
        {50, 92}, /* Alpheratz - Algenib */
        {92, 79}, /* Algenib - Markab */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 15. Andromeda: 2 lines */
    { "Andromeda", "And", 2, {
        {50, 51}, /* Alpheratz - Mirach */
        {51, 64}, /* Mirach - Almach */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 16. Leo: 3 lines */
    { "Leo", "Leo", 3, {
        {20, 67}, /* Regulus - Algieba */
        {67, 80}, /* Algieba - Zosma */
        {80, 56}, /* Zosma - Denebola */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 17. Corvus: 4 lines (trapezoid) */
    { "Corvus", "Crv", 4, {
        {81, 93}, /* Gienah Crv - Algorab */
        {93, 84}, /* Algorab - Kraz */
        {84, 94}, /* Kraz - Minkar */
        {94, 81}, /* Minkar - Gienah Crv */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 18. Libra: 1 line */
    { "Libra", "Lib", 1, {
        {82, 88}, /* Zubeneschamali - Zubenelgenubi */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 19. Aquila: 1 line */
    { "Aquila", "Aql", 1, {
        {11, 87}, /* Altair - Tarazed */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 20. Draco: 1 line */
    { "Draco", "Dra", 1, {
        {61, 90}, /* Eltanin - Rastaban (head) */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 21. Perseus: 1 line */
    { "Perseus", "Per", 1, {
        {34, 55}, /* Mirfak - Algol */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 22. Lyra: 1 line */
    { "Lyra", "Lyr", 1, {
        { 4, 96}, /* Vega - Sulafat */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 23. Aries: 1 line */
    { "Aries", "Ari", 1, {
        {46, 83}, /* Hamal - Sheratan */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 24. Bootes: 1 line */
    { "Bootes", "Boo", 1, {
        { 3, 70}, /* Arcturus - Izar */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 25. Ophiuchus: 1 line */
    { "Ophiuchus", "Oph", 1, {
        {52, 74}, /* Rasalhague - Sabik */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* ---- NEW CONSTELLATIONS (26-39) ---- */
    /* 26. Virgo: 3 lines */
    { "Virgo", "Vir", 3, {
        {15, 112}, /* Spica - Porrima */
        {112, 122}, /* Porrima - Vindemiatrix */
        {15, 154}, /* Spica - Heze */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 27. Cancer: 2 lines */
    { "Cancer", "Cnc", 2, {
        {156, 155}, /* Ain area - Wasat (approx, faint const.) */
        {155, 130}, /* Wasat - Gamma Hya */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 28. Pisces: 2 lines */
    { "Pisces", "Psc", 2, {
        {128, 184}, /* Eta Peg - Gamma Psc */
        {184, 189}, /* Gamma Psc - Iota Psc */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 29. Aquarius: 2 lines */
    { "Aquarius", "Aqr", 2, {
        {152, 161}, /* Delta Aql area - Lambda Aql */
        {188, 157}, /* Theta Peg - Zeta Peg */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 30. Capricornus: 2 lines */
    { "Capricornus", "Cap", 2, {
        {167, 191}, /* Delta Cap - Gamma Cap */
        {191, 185}, /* Gamma Cap - Omega Cap */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 31. Canis Minor: 1 line */
    { "Canis Minor", "CMi", 1, {
        { 7, 129}, /* Procyon - Epsilon Gem (nearest bright) */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 32. Columba: 1 line */
    { "Columba", "Col", 1, {
        {106, 142}, /* Phact - Wazn */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 33. Triangulum: 2 lines */
    { "Triangulum", "Tri", 2, {
        {190, 174}, /* Beta Tri - Gamma Tri */
        {174, 190}, /* (close the triangle is 3 stars but only 2 needed) */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 34. Corona Borealis: 1 line */
    { "Corona Borealis", "CrB", 1, {
        {62, 168}, /* Alphecca - Gamma CrB */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 35. Serpens: 1 line */
    { "Serpens", "Ser", 1, {
        {85, 127}, /* Unukalhai - Eta Vir (head region) */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 36. Hercules: 4 lines (Keystone + arms) */
    { "Hercules", "Her", 4, {
        {89, 118}, /* Kornephoros - Zeta Her */
        {118, 119}, /* Zeta Her - Rutilicus */
        {119, 145}, /* Rutilicus - Pi Her */
        {145, 144}, /* Pi Her - Eta Her */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 37. Eridanus: 1 line */
    { "Eridanus", "Eri", 1, {
        { 8, 91}, /* Achernar - Cursa */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 38. Piscis Austrinus: 1 line */
    { "Piscis Austrinus", "PsA", 1, {
        {17, 29}, /* Fomalhaut - Alnair (nearby bright) */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
    /* 39. Lupus: 1 line */
    { "Lupus", "Lup", 1, {
        {68, 116}, /* Alpha Lupi - Gamma Lup */
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}
    }},
};

int constellation_count(void)
{
    return NUM_CONSTELLATIONS;
}

constellation_t constellation_get(int index)
{
    constellation_t c;
    if (index < 0 || index >= NUM_CONSTELLATIONS) {
        c.name = (void *)0;
        c.abbr = (void *)0;
        c.line_count = 0;
        return c;
    }
    c.name = constellations[index].name;
    c.abbr = constellations[index].abbr;
    c.line_count = constellations[index].line_count;
    for (int i = 0; i < c.line_count; i++)
        c.lines[i] = constellations[index].lines[i];
    return c;
}

int constellation_find(const char *name)
{
    if (!name) return -1;
    for (int i = 0; i < NUM_CONSTELLATIONS; i++) {
        const char *a = constellations[i].name;
        const char *b = name;
        int match = 1;
        while (*a && *b) {
            if (*a != *b) { match = 0; break; }
            a++; b++;
        }
        if (*a != *b) match = 0;
        if (match) return i;
    }
    return -1;
}

const char *constellation_abbr(int index)
{
    if (index < 0 || index >= NUM_CONSTELLATIONS) return (void *)0;
    return constellations[index].abbr;
}

int constellation_total_lines(void)
{
    int sum = 0;
    for (int i = 0; i < NUM_CONSTELLATIONS; i++)
        sum += constellations[i].line_count;
    return sum;
}

int constellation_zodiac_from_longitude(float longitude_deg)
{
    float lon = fmodf(longitude_deg, 360.0f);
    if (lon < 0.0f) lon += 360.0f;
    int sign = (int)(lon / 30.0f);
    if (sign > 11) sign = 11;
    return sign;
}
