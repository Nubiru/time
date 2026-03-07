/* cosmic_duality.c — Bundahishn cosmic duality timeline
 * Implementation of the 12,000-year Zoroastrian cosmic cycle. */

#include "cosmic_duality.h"

/* ===== Static data ===== */

static const char *ERA_NAMES[] = {
    "Creation",
    "Mixture",
    "Separation"
};

static const char *ERA_DESCRIPTIONS[] = {
    "Ahura Mazda creates the world in ideal spiritual form, "
        "then gives it material existence",
    "Angra Mainyu invades creation; good and evil are "
        "intertwined in the material world",
    "The Saoshyant appears; evil is finally defeated "
        "and creation is renewed in Frashokereti"
};

static const char *PERIOD_NAMES[] = {
    "Spiritual Creation",
    "Material Creation",
    "Mixture",
    "Renovation"
};

static const char *PERIOD_DESCRIPTIONS[] = {
    "The world exists as thought in the mind of Ahura Mazda",
    "Creation takes physical form — the world in its ideal state",
    "Good and evil intertwined — the world as we know it",
    "The final renovation approaches — evil retreats before the light"
};

static const char *GEO_PARALLELS[] = {
    "Hadean-Archean: Earth forms, oceans condense, first microbial life",
    "Archean-Proterozoic: photosynthesis, oxygen rises, first eukaryotes",
    "Proterozoic: Great Oxidation, Snowball Earth, multicellular life emerges",
    "Phanerozoic: Cambrian explosion, dinosaurs, mammals, humans — transformation"
};

static const cd_amesha_spenta_t AMESHA_SPENTAS[] = {
    { 0, "Ahura Mazda",      "Wise Lord",           "All creation",
      "Fire/Light", "Keter",   1 },
    { 1, "Vohu Manah",       "Good Mind",           "Cattle/Animals",
      "Air",        "Chokmah", 2 },
    { 2, "Asha Vahishta",    "Best Righteousness",  "Fire",
      "Fire",       "Binah",   3 },
    { 3, "Khshathra Vairya", "Desirable Dominion",  "Metals/Sky",
      "Metal",      "Chesed",  4 },
    { 4, "Spenta Armaiti",   "Holy Devotion",       "Earth",
      "Earth",      "Gevurah", 5 },
    { 5, "Haurvatat",        "Wholeness",           "Water",
      "Water",      "Tiferet", 6 },
    { 6, "Ameretat",         "Immortality",         "Plants",
      "Wood",       "Netzach", 7 }
};

static const cd_amesha_spenta_t INVALID_AMESHA = {
    -1, "?", "?", "?", "?", "?", 0
};

/* ===== Helper: clamp cosmic year ===== */

static int clamp_year(int cosmic_year)
{
    if (cosmic_year < 0) return 0;
    if (cosmic_year > CD_CYCLE_YEARS) return CD_CYCLE_YEARS;
    return cosmic_year;
}

/* ===== Public functions ===== */

cd_era_t cd_era(int cosmic_year)
{
    int y = clamp_year(cosmic_year);
    if (y < 6000) return CD_ERA_CREATION;
    if (y < 9000) return CD_ERA_MIXTURE;
    return CD_ERA_SEPARATION;
}

const char *cd_era_name(cd_era_t era)
{
    if (era < 0 || era >= CD_ERA_COUNT) return "?";
    return ERA_NAMES[era];
}

const char *cd_era_description(cd_era_t era)
{
    if (era < 0 || era >= CD_ERA_COUNT) return "?";
    return ERA_DESCRIPTIONS[era];
}

cd_period_t cd_period(int cosmic_year)
{
    int y = clamp_year(cosmic_year);
    if (y < 3000) return CD_PERIOD_SPIRITUAL_CREATION;
    if (y < 6000) return CD_PERIOD_MATERIAL_CREATION;
    if (y < 9000) return CD_PERIOD_MIXTURE;
    return CD_PERIOD_RENOVATION;
}

const char *cd_period_name(cd_period_t period)
{
    if (period < 0 || period >= CD_PERIOD_COUNT) return "?";
    return PERIOD_NAMES[period];
}

cd_position_t cd_position(int cosmic_year)
{
    int y = clamp_year(cosmic_year);
    cd_era_t e = cd_era(y);
    cd_period_t p = cd_period(y);
    double prog = cd_progress(y);

    /* Era progress: how far within the current era */
    double era_prog;
    int era_start;
    int era_len;
    if (e == CD_ERA_CREATION) {
        era_start = 0;
        era_len = 6000;
    } else if (e == CD_ERA_MIXTURE) {
        era_start = 6000;
        era_len = 3000;
    } else {
        era_start = 9000;
        era_len = 3000;
    }
    era_prog = (double)(y - era_start) / (double)era_len;
    if (era_prog < 0.0) era_prog = 0.0;
    if (era_prog > 1.0) era_prog = 1.0;

    cd_position_t pos;
    pos.era = e;
    pos.period = p;
    pos.cycle_year = y;
    pos.progress = prog;
    pos.era_progress = era_prog;
    pos.description = PERIOD_DESCRIPTIONS[p];
    return pos;
}

double cd_progress(int cosmic_year)
{
    int y = clamp_year(cosmic_year);
    return (double)y / (double)CD_CYCLE_YEARS;
}

cd_amesha_spenta_t cd_amesha_spenta(int index)
{
    if (index < 0 || index >= CD_AMESHA_COUNT) return INVALID_AMESHA;
    return AMESHA_SPENTAS[index];
}

cd_amesha_spenta_t cd_amesha_spenta_for_day(int day)
{
    if (day < 1 || day > 7) return INVALID_AMESHA;
    return AMESHA_SPENTAS[day - 1];
}

cd_geo_map_t cd_geological_parallel(int cosmic_year)
{
    int y = clamp_year(cosmic_year);
    cd_geo_map_t g;
    g.cosmic_years = (double)y;
    g.geological_ma = 4540.0 * (1.0 - (double)y / 12000.0);

    /* Determine geological parallel text by period */
    if (y < 3000) {
        g.geological_parallel = GEO_PARALLELS[0];
    } else if (y < 6000) {
        g.geological_parallel = GEO_PARALLELS[1];
    } else if (y < 9000) {
        g.geological_parallel = GEO_PARALLELS[2];
    } else {
        g.geological_parallel = GEO_PARALLELS[3];
    }
    return g;
}

int cd_amesha_spenta_count(void)
{
    return CD_AMESHA_COUNT;
}

int cd_cycle_year_from_yazdgerdi(int yazdgerdi_year)
{
    int cycle_year = 9000 + (yazdgerdi_year - 1);
    if (cycle_year < 0) return 0;
    if (cycle_year > 11999) return 11999;
    return cycle_year;
}
