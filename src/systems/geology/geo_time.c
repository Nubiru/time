/* geo_time.c — International Chronostratigraphic Chart data (ICS 2024).
 * Pure functions over static const data. No malloc, no globals, no GL. */

#include "geo_time.h"

#include <math.h>
#include <stddef.h>

/* --- Constants --- */

static const double J2000_JD = 2451545.0;
static const double DAYS_PER_YEAR = 365.25;

/* --- Eons (4) --- */

#define EON_COUNT 4

static const geo_unit_t EONS[EON_COUNT] = {
    { "Hadean",      4600.0, 4000.0, { 204,   0,  45 } },
    { "Archean",     4000.0, 2500.0, { 240,   4, 127 } },
    { "Proterozoic", 2500.0,  538.8, { 247,  53,  43 } },
    { "Phanerozoic",  538.8,    0.0, { 154, 217, 221 } }
};

/* --- Eras (10) --- */

#define ERA_COUNT 10

static const geo_unit_t ERAS[ERA_COUNT] = {
    { "Eoarchean",        4000.0, 3600.0, { 218,   2, 127 } },
    { "Paleoarchean",     3600.0, 3200.0, { 244,  68, 159 } },
    { "Mesoarchean",      3200.0, 2800.0, { 247, 104, 169 } },
    { "Neoarchean",       2800.0, 2500.0, { 249, 155, 193 } },
    { "Paleoproterozoic", 2500.0, 1600.0, { 247,  67, 112 } },
    { "Mesoproterozoic",  1600.0, 1000.0, { 253, 180,  98 } },
    { "Neoproterozoic",   1000.0,  538.8, { 254, 179,  66 } },
    { "Paleozoic",         538.8,  251.9, { 153, 192, 141 } },
    { "Mesozoic",          251.9,   66.0, { 103, 197, 202 } },
    { "Cenozoic",           66.0,    0.0, { 242, 249,  29 } }
};

/* --- Periods (22) --- */

#define PERIOD_COUNT 22

static const geo_unit_t PERIODS[PERIOD_COUNT] = {
    { "Siderian",       2500.0, 2300.0, { 247,  79, 124 } },
    { "Rhyacian",       2300.0, 2050.0, { 247,  91, 137 } },
    { "Orosirian",      2050.0, 1800.0, { 247, 104, 152 } },
    { "Statherian",     1800.0, 1600.0, { 247, 117, 167 } },
    { "Calymmian",      1600.0, 1400.0, { 253, 192, 122 } },
    { "Ectasian",       1400.0, 1200.0, { 253, 204, 138 } },
    { "Stenian",        1200.0, 1000.0, { 253, 216, 154 } },
    { "Tonian",         1000.0,  720.0, { 254, 191,  78 } },
    { "Cryogenian",      720.0,  635.0, { 254, 204,  92 } },
    { "Ediacaran",       635.0,  538.8, { 254, 217, 106 } },
    { "Cambrian",        538.8,  485.4, { 127, 160,  86 } },
    { "Ordovician",      485.4,  443.8, {   0, 146, 112 } },
    { "Silurian",        443.8,  419.2, { 179, 225, 182 } },
    { "Devonian",        419.2,  358.9, { 203, 140,  55 } },
    { "Carboniferous",   358.9,  298.9, { 103, 165, 153 } },
    { "Permian",         298.9,  251.9, { 240,  64,  40 } },
    { "Triassic",        251.9,  201.4, { 129,  43, 146 } },
    { "Jurassic",        201.4,  145.0, {  52, 178, 201 } },
    { "Cretaceous",      145.0,   66.0, { 127, 198,  78 } },
    { "Paleogene",        66.0,   23.03,{ 253, 154,  82 } },
    { "Neogene",          23.03,   2.58,{ 255, 230,  25 } },
    { "Quaternary",        2.58,   0.0, { 249, 249, 127 } }
};

/* --- Epochs (34) --- */

#define EPOCH_COUNT 34

static const geo_unit_t EPOCHS[EPOCH_COUNT] = {
    { "Lower Cambrian",              538.8,   509.0,   { 140, 176, 108 } },
    { "Middle Cambrian",             509.0,   497.0,   { 166, 207, 134 } },
    { "Upper Cambrian (Furongian)",  497.0,   485.4,   { 179, 224, 149 } },
    { "Lower Ordovician",           485.4,   470.0,   {  26, 171, 135 } },
    { "Middle Ordovician",          470.0,   458.4,   {  77, 195, 153 } },
    { "Upper Ordovician",           458.4,   443.8,   { 127, 202, 147 } },
    { "Llandovery",                 443.8,   433.4,   { 153, 215, 148 } },
    { "Wenlock",                    433.4,   427.4,   { 179, 225, 182 } },
    { "Ludlow",                     427.4,   423.0,   { 191, 230, 192 } },
    { "Pridoli",                    423.0,   419.2,   { 203, 236, 201 } },
    { "Lower Devonian",            419.2,   393.3,   { 229, 172,  77 } },
    { "Middle Devonian",           393.3,   382.7,   { 241, 200, 104 } },
    { "Upper Devonian",            382.7,   358.9,   { 241, 213, 118 } },
    { "Mississippian",             358.9,   323.2,   { 103, 165, 153 } },
    { "Pennsylvanian",             323.2,   298.9,   { 153, 194, 181 } },
    { "Cisuralian",                298.9,   273.0,   { 240, 101,  78 } },
    { "Guadalupian",               273.0,   259.5,   { 240, 138,  64 } },
    { "Lopingian",                 259.5,   251.9,   { 240, 167,  93 } },
    { "Lower Triassic",            251.9,   247.2,   { 152,  57, 153 } },
    { "Middle Triassic",           247.2,   237.0,   { 177, 104, 177 } },
    { "Upper Triassic",            237.0,   201.4,   { 189, 140, 195 } },
    { "Lower Jurassic",            201.4,   174.7,   {  66, 174, 208 } },
    { "Middle Jurassic",           174.7,   161.5,   { 128, 207, 216 } },
    { "Upper Jurassic",            161.5,   145.0,   { 179, 227, 227 } },
    { "Lower Cretaceous",          145.0,   100.5,   { 140, 205,  87 } },
    { "Upper Cretaceous",          100.5,    66.0,   { 166, 216,  74 } },
    { "Paleocene",                  66.0,    56.0,   { 253, 167,  95 } },
    { "Eocene",                     56.0,    33.9,   { 253, 192, 122 } },
    { "Oligocene",                  33.9,    23.03,  { 253, 217, 154 } },
    { "Miocene",                    23.03,    5.333, { 255, 255,   0 } },
    { "Pliocene",                    5.333,   2.58,  { 255, 255, 153 } },
    { "Pleistocene",                 2.58,    0.0117,{ 255, 242, 174 } },
    { "Holocene",                    0.0117,  0.000045, { 254, 235, 210 } },
    /* Anthropocene: informal/disputed in ICS 2024. Start ~1950 CE. */
    { "Anthropocene",               0.000045, 0.0,   { 254, 242, 224 } }
};

/* --- Extinction events (Big Five) --- */

#define EXTINCTION_COUNT 5

static const geo_extinction_t EXTINCTIONS[EXTINCTION_COUNT] = {
    { "Ordovician-Silurian",    443.8, "Glaciation, sea level drop" },
    { "Late Devonian",          372.2, "Ocean anoxia, volcanism" },
    { "Permian-Triassic",       251.9, "Siberian Traps volcanism" },
    { "Triassic-Jurassic",      201.4, "CAMP volcanism" },
    { "Cretaceous-Paleogene",    66.0, "Chicxulub asteroid impact" }
};

/* --- Internal lookup helper --- */

/* Linear scan: return first entry where start_ma >= ma > end_ma.
 * At boundary (ma == start_ma), that unit owns the boundary.
 * Special case: end_ma == 0.0 means "present", so ma == 0 matches. */
static geo_unit_t lookup_unit(const geo_unit_t *table, int count, double ma) {
    for (int i = 0; i < count; i++) {
        int in_range = (ma <= table[i].start_ma) &&
                       ((ma > table[i].end_ma) ||
                        (ma == 0.0 && table[i].end_ma == 0.0));
        if (in_range) {
            return table[i];
        }
    }
    /* Fallback: return empty unit */
    geo_unit_t empty = { NULL, 0.0, 0.0, { 0, 0, 0 } };
    return empty;
}

static geo_unit_t index_unit(const geo_unit_t *table, int count, int index) {
    if (index < 0 || index >= count) {
        geo_unit_t empty = { NULL, 0.0, 0.0, { 0, 0, 0 } };
        return empty;
    }
    return table[index];
}

/* --- Eon API --- */

int geo_eon_count(void) {
    return EON_COUNT;
}

geo_unit_t geo_eon_at(double ma) {
    return lookup_unit(EONS, EON_COUNT, ma);
}

geo_unit_t geo_eon_by_index(int index) {
    return index_unit(EONS, EON_COUNT, index);
}

/* --- Era API --- */

int geo_era_count(void) {
    return ERA_COUNT;
}

geo_unit_t geo_era_at(double ma) {
    return lookup_unit(ERAS, ERA_COUNT, ma);
}

geo_unit_t geo_era_by_index(int index) {
    return index_unit(ERAS, ERA_COUNT, index);
}

/* --- Period API --- */

int geo_period_count(void) {
    return PERIOD_COUNT;
}

geo_unit_t geo_period_at(double ma) {
    return lookup_unit(PERIODS, PERIOD_COUNT, ma);
}

geo_unit_t geo_period_by_index(int index) {
    return index_unit(PERIODS, PERIOD_COUNT, index);
}

/* --- Epoch API --- */

int geo_epoch_count(void) {
    return EPOCH_COUNT;
}

geo_unit_t geo_epoch_at(double ma) {
    return lookup_unit(EPOCHS, EPOCH_COUNT, ma);
}

geo_unit_t geo_epoch_by_index(int index) {
    return index_unit(EPOCHS, EPOCH_COUNT, index);
}

/* --- JD to Ma conversion --- */

double geo_jd_to_ma(double jd) {
    double years = (J2000_JD - jd) / DAYS_PER_YEAR;
    return years / 1.0e6;
}

/* --- Extinction API --- */

int geo_extinction_count(void) {
    return EXTINCTION_COUNT;
}

geo_extinction_t geo_extinction_by_index(int index) {
    if (index < 0 || index >= EXTINCTION_COUNT) {
        geo_extinction_t empty = { NULL, 0.0, NULL };
        return empty;
    }
    return EXTINCTIONS[index];
}

geo_extinction_t geo_extinction_nearest(double ma) {
    int best = 0;
    double best_dist = fabs(EXTINCTIONS[0].ma - ma);
    for (int i = 1; i < EXTINCTION_COUNT; i++) {
        double dist = fabs(EXTINCTIONS[i].ma - ma);
        if (dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    return EXTINCTIONS[best];
}
