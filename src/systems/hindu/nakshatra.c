#include "nakshatra.h"

#include <math.h>

#define SPAN_DEG (360.0 / 27.0)   /* 13.333... degrees per nakshatra */
#define PADA_DEG (360.0 / 108.0)  /* 3.333... degrees per pada */

/* ──────────────────────────────────────────────────────────────────────────
 * Ruler names — indexed 0-8.
 * 0=Ketu, 1=Venus, 2=Sun, 3=Moon, 4=Mars, 5=Rahu,
 * 6=Jupiter, 7=Saturn, 8=Mercury
 * ────────────────────────────────────────────────────────────────────── */

static const char *ruler_names[9] = {
    "Ketu", "Venus", "Sun", "Moon", "Mars",
    "Rahu", "Jupiter", "Saturn", "Mercury"
};

/* ──────────────────────────────────────────────────────────────────────────
 * Static Nakshatra data table.
 * Fields: name, meaning, deity, symbol, quality, yogatara_catalog_index
 *
 * Yogatara catalog indices (hardcoded from 195-star catalog):
 *   Aldebaran=13, Betelgeuse=9, Pollux=16, Regulus=20,
 *   Spica=15, Arcturus=3, Antares=14, Altair=11.
 *   All others = -1 (not in catalog).
 *
 * Quality: Deva/Manushya/Rakshasa per standard Jyotish classification.
 * ────────────────────────────────────────────────────────────────────── */

typedef struct {
    const char *name;
    const char *meaning;
    const char *deity;
    const char *symbol;
    const char *quality;
    int yogatara_catalog_index;
} nakshatra_data_t;

static const nakshatra_data_t data[27] = {
    /* 0  Ashwini */
    { "Ashwini", "Horse riders", "Ashwini Kumaras",
      "Horse head", "Deva", -1 },
    /* 1  Bharani */
    { "Bharani", "She who bears", "Yama",
      "Yoni", "Manushya", -1 },
    /* 2  Krittika */
    { "Krittika", "The cutters", "Agni",
      "Razor", "Rakshasa", -1 },
    /* 3  Rohini */
    { "Rohini", "The red one", "Brahma",
      "Ox cart", "Manushya", 13 },
    /* 4  Mrigashira */
    { "Mrigashira", "Deer head", "Soma",
      "Deer head", "Deva", -1 },
    /* 5  Ardra */
    { "Ardra", "The moist one", "Rudra",
      "Teardrop", "Manushya", 9 },
    /* 6  Punarvasu */
    { "Punarvasu", "Return of light", "Aditi",
      "Bow and quiver", "Deva", 16 },
    /* 7  Pushya */
    { "Pushya", "Nourisher", "Brihaspati",
      "Cow udder", "Deva", -1 },
    /* 8  Ashlesha */
    { "Ashlesha", "The embracer", "Nagas",
      "Coiled serpent", "Rakshasa", -1 },
    /* 9  Magha */
    { "Magha", "The great one", "Pitris",
      "Royal throne", "Rakshasa", 20 },
    /* 10 Purva Phalguni */
    { "Purva Phalguni", "Former red one", "Bhaga",
      "Front legs of bed", "Manushya", -1 },
    /* 11 Uttara Phalguni */
    { "Uttara Phalguni", "Latter red one", "Aryaman",
      "Back legs of bed", "Manushya", -1 },
    /* 12 Hasta */
    { "Hasta", "The hand", "Savitar",
      "Open hand", "Deva", -1 },
    /* 13 Chitra */
    { "Chitra", "The brilliant", "Vishvakarma",
      "Bright jewel", "Rakshasa", 15 },
    /* 14 Swati */
    { "Swati", "The sword", "Vayu",
      "Young sprout", "Deva", 3 },
    /* 15 Vishakha */
    { "Vishakha", "The forked", "Indra-Agni",
      "Triumphal arch", "Rakshasa", -1 },
    /* 16 Anuradha */
    { "Anuradha", "Following Radha", "Mitra",
      "Lotus", "Deva", -1 },
    /* 17 Jyeshtha */
    { "Jyeshtha", "The eldest", "Indra",
      "Circular amulet", "Rakshasa", 14 },
    /* 18 Mula */
    { "Mula", "The root", "Nirriti",
      "Bunch of roots", "Rakshasa", -1 },
    /* 19 Purva Ashadha */
    { "Purva Ashadha", "Former invincible", "Apas",
      "Elephant tusk", "Manushya", -1 },
    /* 20 Uttara Ashadha */
    { "Uttara Ashadha", "Latter invincible", "Vishvadevas",
      "Elephant tusk", "Manushya", -1 },
    /* 21 Shravana */
    { "Shravana", "The listener", "Vishnu",
      "Three footprints", "Deva", 11 },
    /* 22 Dhanishtha */
    { "Dhanishtha", "The wealthiest", "Vasus",
      "Drum", "Rakshasa", -1 },
    /* 23 Shatabhisha */
    { "Shatabhisha", "Hundred healers", "Varuna",
      "Empty circle", "Rakshasa", -1 },
    /* 24 Purva Bhadrapada */
    { "Purva Bhadrapada", "Former lucky feet", "Aja Ekapada",
      "Front of funeral cot", "Manushya", -1 },
    /* 25 Uttara Bhadrapada */
    { "Uttara Bhadrapada", "Latter lucky feet", "Ahir Budhnya",
      "Back of funeral cot", "Manushya", -1 },
    /* 26 Revati */
    { "Revati", "The wealthy", "Pushan",
      "Fish", "Deva", -1 }
};

/* ──────────────────────────────────────────────────────────────────────────
 * Normalize longitude to [0, 360).
 * ────────────────────────────────────────────────────────────────────── */

static double normalize_longitude(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0) {
        deg += 360.0;
    }
    return deg;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Build a nakshatra_t from index.
 * ────────────────────────────────────────────────────────────────────── */

static nakshatra_t build_nakshatra(int index)
{
    nakshatra_t n;
    n.id = index;
    n.name = data[index].name;
    n.meaning = data[index].meaning;
    n.start_deg = (double)index * SPAN_DEG;
    n.end_deg = (double)(index + 1) * SPAN_DEG;
    n.ruling_planet = index % 9;
    n.deity = data[index].deity;
    n.symbol = data[index].symbol;
    n.quality = data[index].quality;
    n.yogatara_catalog_index = data[index].yogatara_catalog_index;
    return n;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Build an invalid sentinel nakshatra.
 * ────────────────────────────────────────────────────────────────────── */

static nakshatra_t invalid_nakshatra(void)
{
    nakshatra_t n;
    n.id = -1;
    n.name = (void *)0;
    n.meaning = (void *)0;
    n.start_deg = 0.0;
    n.end_deg = 0.0;
    n.ruling_planet = -1;
    n.deity = (void *)0;
    n.symbol = (void *)0;
    n.quality = (void *)0;
    n.yogatara_catalog_index = -1;
    return n;
}

/* ──────────────────────────────────────────────────────────────────────── */

int nakshatra_count(void)
{
    return NAKSHATRA_COUNT;
}

nakshatra_t nakshatra_get(int index)
{
    if (index < 0 || index >= NAKSHATRA_COUNT) {
        return invalid_nakshatra();
    }
    return build_nakshatra(index);
}

const char *nakshatra_name(int index)
{
    if (index < 0 || index >= NAKSHATRA_COUNT) {
        return (void *)0;
    }
    return data[index].name;
}

nakshatra_t nakshatra_from_longitude(double sidereal_longitude)
{
    double lon = normalize_longitude(sidereal_longitude);
    int index = (int)(lon / SPAN_DEG);
    if (index >= NAKSHATRA_COUNT) {
        index = NAKSHATRA_COUNT - 1;
    }
    return build_nakshatra(index);
}

nakshatra_pada_t nakshatra_pada_from_longitude(double sidereal_longitude)
{
    double lon = normalize_longitude(sidereal_longitude);
    int global_pada = (int)(lon / PADA_DEG);
    if (global_pada >= NAKSHATRA_TOTAL_PADAS) {
        global_pada = NAKSHATRA_TOTAL_PADAS - 1;
    }

    nakshatra_pada_t p;
    p.nakshatra_id = global_pada / NAKSHATRA_PADA_COUNT;
    p.pada_number = (global_pada % NAKSHATRA_PADA_COUNT) + 1;
    p.navamsa_sign = global_pada % 12;
    p.start_deg = (double)global_pada * PADA_DEG;
    p.end_deg = (double)(global_pada + 1) * PADA_DEG;
    return p;
}

nakshatra_pada_t nakshatra_pada_get(int nakshatra_id, int pada)
{
    nakshatra_pada_t p;
    if (nakshatra_id < 0 || nakshatra_id >= NAKSHATRA_COUNT ||
        pada < 1 || pada > NAKSHATRA_PADA_COUNT) {
        p.nakshatra_id = -1;
        p.pada_number = -1;
        p.navamsa_sign = -1;
        p.start_deg = 0.0;
        p.end_deg = 0.0;
        return p;
    }

    int global_pada = nakshatra_id * NAKSHATRA_PADA_COUNT + (pada - 1);
    p.nakshatra_id = nakshatra_id;
    p.pada_number = pada;
    p.navamsa_sign = global_pada % 12;
    p.start_deg = (double)global_pada * PADA_DEG;
    p.end_deg = (double)(global_pada + 1) * PADA_DEG;
    return p;
}

int nakshatra_navamsa(double sidereal_longitude)
{
    double lon = normalize_longitude(sidereal_longitude);
    int global_pada = (int)(lon / PADA_DEG);
    if (global_pada >= NAKSHATRA_TOTAL_PADAS) {
        global_pada = NAKSHATRA_TOTAL_PADAS - 1;
    }
    return global_pada % 12;
}

double nakshatra_tropical_to_sidereal(double tropical_longitude,
                                       double ayanamsa_deg)
{
    double sid = tropical_longitude - ayanamsa_deg;
    return normalize_longitude(sid);
}

double nakshatra_lahiri_ayanamsa(double jd)
{
    return 23.85 + 0.01396 * (jd - 2451545.0) / 365.25;
}

const char *nakshatra_ruler_name(int ruler_index)
{
    if (ruler_index < 0 || ruler_index > 8) {
        return (void *)0;
    }
    return ruler_names[ruler_index];
}
