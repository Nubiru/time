/* decan_stars.c — The 36 Egyptian decans as star groups.
 *
 * Implementation of the decan star data and star clock computations.
 * All functions are pure: no globals, no malloc, no side effects.
 *
 * Egyptian decan names follow the Neugebauer & Parker "Egyptian
 * Astronomical Texts" transliteration tradition. Principal star
 * identifications follow scholarly consensus (Conman, Lull & Belmonte).
 * Where identification is uncertain, the description field notes this.
 *
 * Attribution: Otto Neugebauer, Richard A. Parker, Joanne Conman,
 * Jose Lull, Juan Antonio Belmonte. */

#include "decan_stars.h"

#include <math.h>

#define PI 3.14159265358979323846

/* Zodiac triplicity elements: sign index -> element.
 * Aries=Fire, Taurus=Earth, Gemini=Air, Cancer=Water, ... repeating. */
static const decan_element_t SIGN_ELEMENTS[12] = {
    DECAN_ELEM_FIRE,  DECAN_ELEM_EARTH, DECAN_ELEM_AIR,   DECAN_ELEM_WATER,
    DECAN_ELEM_FIRE,  DECAN_ELEM_EARTH, DECAN_ELEM_AIR,   DECAN_ELEM_WATER,
    DECAN_ELEM_FIRE,  DECAN_ELEM_EARTH, DECAN_ELEM_AIR,   DECAN_ELEM_WATER
};

/* The 36 decan records. Indexed 0-35.
 * Egyptian names: Neugebauer & Parker (EAT) transliteration.
 * Stars: modern identifications where consensus exists.
 * Ecliptic longitudes: decan i covers [i*10, (i+1)*10) degrees. */
static const struct {
    const char *egyptian_name;
    const char *principal_star;
    const char *description;
} DECAN_DATA[DECAN_COUNT] = {
    /* Aries (sign 0, Fire) — decans 0, 1, 2 */
    { "Khentet-hrt",    "Alpha Arietis (Hamal)",
      "Head of the Ram; bright star in Aries" },
    { "Khentet-hrt-2",  "Beta Arietis (Sheratan)",
      "Second star of the Ram group" },
    { "Hry-ib-wiA",     "41 Arietis (Bharani)",
      "Central Aries stars; uncertain identification" },

    /* Taurus (sign 1, Earth) — decans 3, 4, 5 */
    { "pAwy-iry",       "Eta Tauri (Alcyone/Pleiades)",
      "The Pleiades cluster; prominent decan marker" },
    { "stwy",           "Alpha Tauri (Aldebaran)",
      "Eye of the Bull; Hyades group" },
    { "srt",            "Zeta Tauri (Tianguan)",
      "Tip of the Bull's southern horn" },

    /* Gemini (sign 2, Air) — decans 6, 7, 8 */
    { "sAwy-srt",       "Beta Geminorum (Pollux)",
      "The bright twin; northern Gemini" },
    { "Xry-xpd-srt",    "Alpha Geminorum (Castor)",
      "The other twin; multiple star system" },
    { "tpy-A-Xntt",     "Epsilon Geminorum (Mebsuta)",
      "Northern Gemini; uncertain identification" },

    /* Cancer (sign 3, Water) — decans 9, 10, 11 */
    { "kd",             "Delta Cancri (Asellus Australis)",
      "Southern Donkey star near Praesepe" },
    { "sAwy-kd",        "Gamma Cancri (Asellus Borealis)",
      "Northern Donkey star near the Beehive" },
    { "xAw",            "Alpha Cancri (Acubens)",
      "Claw of the Crab; faint decan marker" },

    /* Leo (sign 4, Fire) — decans 12, 13, 14 */
    { "Xry-xpd-xAw",   "Alpha Leonis (Regulus)",
      "Heart of the Lion; brightest in Leo" },
    { "HAt-DAt",        "Gamma Leonis (Algieba)",
      "Mane of the Lion; double star" },
    { "pHwy-DAt",       "Beta Leonis (Denebola)",
      "Tail of the Lion; marks Leo's end" },

    /* Virgo (sign 5, Earth) — decans 15, 16, 17 */
    { "tmAt-Hrt",       "Alpha Virginis (Spica)",
      "The Sheaf of Wheat; brightest in Virgo" },
    { "wSAti",          "Gamma Virginis (Porrima)",
      "Double star in central Virgo" },
    { "bkAti",          "Epsilon Virginis (Vindemiatrix)",
      "The Grape Gatherer; harvest star" },

    /* Libra (sign 6, Air) — decans 18, 19, 20 */
    { "tpy-A-Xnty",     "Alpha Librae (Zubenelgenubi)",
      "Southern Claw of the Scales" },
    { "Hry-ib-wiA-2",   "Beta Librae (Zubeneschamali)",
      "Northern Claw; faintly green to the eye" },
    { "Xntt-hrt",       "Sigma Librae (Brachium)",
      "Arm of the Scales; uncertain identification" },

    /* Scorpio (sign 7, Water) — decans 21, 22, 23 */
    { "Xntt-Xrt",       "Alpha Scorpii (Antares)",
      "Heart of the Scorpion; red supergiant" },
    { "tms-n-Xntt",     "Delta Scorpii (Dschubba)",
      "Forehead of the Scorpion" },
    { "spty-Xntt",      "Lambda Scorpii (Shaula)",
      "Sting of the Scorpion; tail stars" },

    /* Sagittarius (sign 8, Fire) — decans 24, 25, 26 */
    { "qd",             "Epsilon Sagittarii (Kaus Australis)",
      "Southern part of the Bow" },
    { "sAwy-qd",        "Sigma Sagittarii (Nunki)",
      "Ancient star name; prominent in the Archer" },
    { "Xry-xpd-qd",     "Delta Sagittarii (Kaus Media)",
      "Center of the Bow; Milky Way region" },

    /* Capricorn (sign 9, Earth) — decans 27, 28, 29 */
    { "Smdt",           "Delta Capricorni (Deneb Algedi)",
      "Tail of the Goat; eclipsing binary" },
    { "sAwy-Smdt",      "Beta Capricorni (Dabih)",
      "Slaughterer's star; double star" },
    { "kAwy-Smdt",      "Alpha Capricorni (Algedi)",
      "Horn of the Goat; optical double" },

    /* Aquarius (sign 10, Air) — decans 30, 31, 32 */
    { "tpy-A-smd",      "Beta Aquarii (Sadalsuud)",
      "Luckiest of the Lucky; bright Aquarius star" },
    { "Hry-ib-smd",     "Alpha Aquarii (Sadalmelik)",
      "Lucky star of the King" },
    { "sAwy-smd",       "Delta Aquarii (Skat)",
      "Shin of the Water-bearer" },

    /* Pisces (sign 11, Water) — decans 33, 34, 35 */
    { "knmt",           "Eta Piscium (Alpherg)",
      "Brightest in Pisces; near the knot" },
    { "sAwy-knmt",      "Alpha Piscium (Alrescha)",
      "The Knot tying the two Fish" },
    { "smd-n-knmt",     "Omega Piscium",
      "End of Pisces; approaches vernal equinox" },
};

/* Normalize angle to [0, 360) range. */
static double normalize_deg(double deg)
{
    double result = fmod(deg, 360.0);
    if (result < 0.0) {
        result += 360.0;
    }
    return result;
}

int decan_count(void)
{
    return DECAN_COUNT;
}

decan_t decan_get(int index)
{
    if (index < 0 || index >= DECAN_COUNT) {
        decan_t sentinel;
        sentinel.index = -1;
        sentinel.egyptian_name = "";
        sentinel.principal_star = "";
        sentinel.ecl_lon_start = 0.0;
        sentinel.ecl_lon_end = 0.0;
        sentinel.zodiac_sign = -1;
        sentinel.decan_in_sign = 0;
        sentinel.element = DECAN_ELEM_COUNT;
        sentinel.description = "";
        return sentinel;
    }

    int sign = index / 3;
    int decan_in_sign = (index % 3) + 1;

    decan_t d;
    d.index = index;
    d.egyptian_name = DECAN_DATA[index].egyptian_name;
    d.principal_star = DECAN_DATA[index].principal_star;
    d.ecl_lon_start = index * DECAN_DEGREES;
    d.ecl_lon_end = (index + 1) * DECAN_DEGREES;
    d.zodiac_sign = sign;
    d.decan_in_sign = decan_in_sign;
    d.element = SIGN_ELEMENTS[sign];
    d.description = DECAN_DATA[index].description;
    return d;
}

int decan_for_ecliptic_longitude(double ecl_lon_deg)
{
    double norm = normalize_deg(ecl_lon_deg);
    /* Handle exact 360.0 -> 0 */
    if (norm >= 360.0) {
        norm = 0.0;
    }
    int idx = (int)(norm / DECAN_DEGREES);
    if (idx >= DECAN_COUNT) {
        idx = DECAN_COUNT - 1;
    }
    return idx;
}

int decan_for_sign(int sign, int *out, int max_out)
{
    if (sign < 0 || sign > 11 || max_out <= 0) {
        return 0;
    }

    int base = sign * 3;
    int count = 3;
    if (count > max_out) {
        count = max_out;
    }

    for (int i = 0; i < count; i++) {
        out[i] = base + i;
    }
    return count;
}

const char *decan_element_name(decan_element_t elem)
{
    switch (elem) {
        case DECAN_ELEM_FIRE:  return "Fire";
        case DECAN_ELEM_EARTH: return "Earth";
        case DECAN_ELEM_AIR:   return "Air";
        case DECAN_ELEM_WATER: return "Water";
        default:               return "Unknown";
    }
}

double decan_star_longitude(int index)
{
    if (index < 0 || index >= DECAN_COUNT) {
        return -1.0;
    }
    return index * DECAN_DEGREES + DECAN_DEGREES / 2.0;
}

/* Compute the ecliptic longitude of the ascendant (rising point on
 * the eastern horizon) given local sidereal time and latitude.
 *
 * Uses the standard astrological ascendant formula:
 * tan(ASC) = cos(LST) / -(sin(LST) * cos(obliquity) + tan(lat) * sin(obliquity))
 *
 * Simplified here for the decan star clock: we use the approximation
 * that the ascendant ecliptic longitude roughly equals the LST converted
 * from equatorial to ecliptic coordinates. For tropical latitudes near
 * the Egyptian context (~30 deg N), this is a reasonable approximation.
 *
 * Full formula from Meeus "Astronomical Algorithms", Chapter 14. */
static double ascendant_ecliptic_lon(double lst_deg, double lat_deg)
{
    static const double OBLIQUITY = 23.4393; /* mean obliquity J2000 */

    double lst_rad = lst_deg * PI / 180.0;
    double lat_rad = lat_deg * PI / 180.0;
    double obl_rad = OBLIQUITY * PI / 180.0;

    double cos_lst = cos(lst_rad);
    double sin_lst = sin(lst_rad);
    double cos_obl = cos(obl_rad);
    double sin_obl = sin(obl_rad);
    double tan_lat = tan(lat_rad);

    double denom = -(sin_lst * cos_obl + tan_lat * sin_obl);

    double asc_rad;
    if (fabs(denom) < 1.0e-10) {
        /* Near-zero denominator: ascendant near 90 or 270 deg */
        asc_rad = (cos_lst >= 0.0) ? PI / 2.0 : 3.0 * PI / 2.0;
    } else {
        asc_rad = atan2(cos_lst, denom);
    }

    double asc_deg = asc_rad * 180.0 / PI;
    return normalize_deg(asc_deg);
}

int decan_rising_now(double lst_deg, double lat_deg)
{
    double asc = ascendant_ecliptic_lon(lst_deg, lat_deg);
    return decan_for_ecliptic_longitude(asc);
}

int decan_night_hours(double lst_deg, double lat_deg, decan_hour_t *out, int max_out)
{
    if (max_out <= 0) {
        return 0;
    }

    int count = 12;
    if (count > max_out) {
        count = max_out;
    }

    /* The first night hour is ruled by the decan currently rising.
     * Each subsequent hour, a new decan rises (10 degrees = ~40 min).
     * We advance LST by 10 degrees per hour step (one decan width). */
    int first_decan = decan_rising_now(lst_deg, lat_deg);

    for (int i = 0; i < count; i++) {
        int decan_idx = (first_decan + i) % DECAN_COUNT;
        double hour_lst = normalize_deg(lst_deg + i * DECAN_DEGREES);

        /* Hour angle: difference between LST and star's "RA" (approx
         * from ecliptic midpoint, simplified). */
        double star_lon = decan_star_longitude(decan_idx);
        double ha = normalize_deg(hour_lst - star_lon);

        out[i].decan_index = decan_idx;
        out[i].hour_angle = ha;
        /* Visible if hour angle < 180 (above horizon in simplified model) */
        out[i].is_visible = (ha < 180.0) ? 1 : 0;
    }

    return count;
}

int decan_for_night_hour(double lst_deg, double lat_deg, int hour)
{
    if (hour < 0 || hour > 11) {
        return -1;
    }

    int first_decan = decan_rising_now(lst_deg, lat_deg);
    return (first_decan + hour) % DECAN_COUNT;
}
