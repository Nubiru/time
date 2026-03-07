/* earth_drama.c — Dramatized Earth history data module.
 * Composition layer over geo_time, climate_history, and fossil_milestones.
 * Pure static const data. No malloc, no globals, no side effects.
 * Data sources: paleontology consensus, NASA paleoclimate,
 * Hazen "Story of Earth", ICS 2024. */

#include "earth_drama.h"

#include <stddef.h>
#include <string.h>

#include "../geology/climate_history.h"
#include "../geology/fossil_milestones.h"
#include "../geology/geo_time.h"

/* --- Atmosphere state names --- */

static const char *const ATMO_NAMES[] = {
    "Reducing",
    "Transitional",
    "Oxidizing",
    "Ozone",
    "Modern"
};

#define ATMO_NAME_COUNT 5

/* --- Atmosphere transition timeline (chronological, oldest first) ---
 * Each entry represents a major atmospheric state.
 * Boundaries: Reducing 4540-2400, Transitional 2400-2000,
 * Oxidizing 2000-600, Ozone 600-400, Modern 400-0. */

#define ATMO_TRANSITION_COUNT 5

static const ed_atmosphere_info_t ATMO_TRANSITIONS[ATMO_TRANSITION_COUNT] = {
    {
        ED_ATMO_REDUCING,
        "Dense CO2/N2/CH4 atmosphere, no free oxygen; volcanic outgassing dominates",
        0.0, 10000.0, 0
    },
    {
        ED_ATMO_TRANSITIONAL,
        "Great Oxidation Event: cyanobacteria produce O2; methane greenhouse collapses",
        1.5, 200.0, 0
    },
    {
        ED_ATMO_OXIDIZING,
        "Significant free oxygen accumulates; banded iron formations cease",
        8.0, 1000.0, 0
    },
    {
        ED_ATMO_OZONE,
        "Ozone layer forms from accumulated O2; UV radiation shielded at surface",
        16.0, 4000.0, 1
    },
    {
        ED_ATMO_MODERN,
        "Modern atmosphere: 78% N2, 21% O2, stable ozone shield",
        21.0, 400.0, 1
    }
};

/* --- Biosphere eras (chronological thresholds) ---
 * Each threshold defines what new life forms appear at that Ma boundary.
 * We accumulate life forms from oldest to the query time. */

typedef struct {
    double threshold_ma;      /* life forms appear at or after this Ma */
    const char *life_form;    /* what appeared */
    const char *dominant;     /* dominant life at this stage */
    int multicellular;        /* cumulative: multicellular exists */
    int land_life;            /* cumulative: life on land */
    int intelligence;         /* cumulative: tool-using intelligence */
} biosphere_stage_t;

#define BIOSPHERE_STAGE_COUNT 11

static const biosphere_stage_t BIOSPHERE_STAGES[BIOSPHERE_STAGE_COUNT] = {
    /* Thresholds are the Ma at which these forms FIRST appear.
     * Listed oldest-first. A query at time T includes all stages
     * where threshold_ma >= T (i.e., the stage existed by time T). */
    { 3800.0, "Prokaryotes",           "Prokaryotes (bacteria, archaea)", 0, 0, 0 },
    { 2700.0, "Cyanobacteria",         "Cyanobacteria (photosynthesizers)", 0, 0, 0 },
    { 2100.0, "Eukaryotes",            "Eukaryotes (complex cells)", 0, 0, 0 },
    { 1000.0, "Multicellular organisms","Multicellular organisms (Ediacaran fauna)", 1, 0, 0 },
    {  541.0, "Animals",               "Marine animals (Cambrian explosion)", 1, 0, 0 },
    {  470.0, "Land plants",           "Land plants and marine animals", 1, 1, 0 },
    {  375.0, "Tetrapods",             "Tetrapods (amphibians, early reptiles)", 1, 1, 0 },
    {  225.0, "Mammals",               "Dinosaurs (dominant), mammals (small)", 1, 1, 0 },
    {   66.0, "Primates",              "Mammals (dominant), primates emerging", 1, 1, 0 },
    {    0.3, "Homo sapiens",          "Homo sapiens (human civilization)", 1, 1, 1 },
    {    0.0, NULL, NULL, 0, 0, 0 }  /* sentinel */
};

/* --- Drama text entries (time-range to one-liner) --- */

typedef struct {
    double start_ma;    /* older boundary (inclusive) */
    double end_ma;      /* younger boundary (exclusive, except 0.0) */
    const char *text;
} drama_entry_t;

#define DRAMA_ENTRY_COUNT 12

static const drama_entry_t DRAMA_ENTRIES[DRAMA_ENTRY_COUNT] = {
    { 4540.0, 3800.0,
      "Molten rock, cosmic bombardment, no atmosphere to speak of — Earth is being forged" },
    { 3800.0, 2700.0,
      "First life stirs in hot springs and ocean vents — invisible, single-celled, tenacious" },
    { 2700.0, 2400.0,
      "Cyanobacteria invent photosynthesis — the atmosphere will never be the same" },
    { 2400.0, 2000.0,
      "The Great Oxidation Event — oxygen poisons most life, triggers a global ice age" },
    { 2000.0, 1000.0,
      "The Boring Billion — stability reigns, eukaryotes quietly evolve complexity" },
    { 1000.0, 541.0,
      "Snowball Earth melts, multicellular life experiments with strange forms" },
    {  541.0, 470.0,
      "The Cambrian Explosion — every animal body plan appears in a geological instant" },
    {  470.0, 375.0,
      "Plants colonize land, forests rise, the atmosphere fills with oxygen" },
    {  375.0, 252.0,
      "Tetrapods walk ashore, coal swamps lock carbon, ice ages grip Gondwana" },
    {  252.0, 66.0,
      "Dinosaurs reign for 186 million years — mammals hide in the shadows" },
    {   66.0, 0.3,
      "An asteroid ends the dinosaurs — mammals inherit a shattered world and thrive" },
    {    0.3, 0.0,
      "Homo sapiens arrives in the final heartbeat of geological time" }
};

/* --- Public API --- */

const char *ed_atmosphere_name(ed_atmosphere_t state)
{
    int s = (int)state;
    if (s < 0 || s >= ATMO_NAME_COUNT) {
        return "?";
    }
    return ATMO_NAMES[s];
}

int ed_atmosphere_transition_count(void)
{
    return ATMO_TRANSITION_COUNT;
}

ed_atmosphere_info_t ed_atmosphere_transition_get(int index)
{
    if (index < 0 || index >= ATMO_TRANSITION_COUNT) {
        ed_atmosphere_info_t invalid;
        memset(&invalid, 0, sizeof(invalid));
        invalid.state = ED_ATMO_REDUCING;
        invalid.description = NULL;
        invalid.o2_percent = 0.0;
        invalid.co2_ppm = 0.0;
        invalid.uv_shielded = 0;
        return invalid;
    }
    return ATMO_TRANSITIONS[index];
}

ed_atmosphere_info_t ed_atmosphere_at(double ma)
{
    /* Boundaries: Reducing >2400, Transitional 2400-2000,
     * Oxidizing 2000-600, Ozone 600-400, Modern <=400 */
    if (ma > 2400.0) {
        return ATMO_TRANSITIONS[0]; /* Reducing */
    }
    if (ma > 2000.0) {
        return ATMO_TRANSITIONS[1]; /* Transitional */
    }
    if (ma > 600.0) {
        return ATMO_TRANSITIONS[2]; /* Oxidizing */
    }
    if (ma > 400.0) {
        return ATMO_TRANSITIONS[3]; /* Ozone */
    }
    return ATMO_TRANSITIONS[4]; /* Modern */
}

ed_biosphere_t ed_biosphere_at(double ma)
{
    ed_biosphere_t b;
    memset(&b, 0, sizeof(b));
    b.count = 0;
    b.dominant = NULL;
    b.multicellular = 0;
    b.land_life = 0;
    b.intelligence = 0;

    /* Accumulate life forms from oldest stages to present.
     * A stage is included if its threshold_ma >= ma (it existed by time ma). */
    const char *last_dominant = NULL;

    for (int i = 0; i < BIOSPHERE_STAGE_COUNT - 1; i++) {
        const biosphere_stage_t *stage = &BIOSPHERE_STAGES[i];

        if (ma <= stage->threshold_ma) {
            /* This life form existed by time ma */
            if (b.count < ED_MAX_ALIVE && stage->life_form != NULL) {
                b.life_forms[b.count] = stage->life_form;
                b.count++;
            }
            last_dominant = stage->dominant;

            /* Cumulative flags: once set, stay set */
            if (stage->multicellular) {
                b.multicellular = 1;
            }
            if (stage->land_life) {
                b.land_life = 1;
            }
            if (stage->intelligence) {
                b.intelligence = 1;
            }
        }
    }

    b.dominant = last_dominant;
    return b;
}

ed_human_scale_t ed_human_scale(void)
{
    static const double HUMAN_AGE_MA = 0.3;
    static const double SECONDS_PER_DAY = 86400.0;
    static const double DAYS_PER_YEAR = 365.0;

    ed_human_scale_t s;
    s.earth_age_ma = ED_EARTH_AGE_MA;
    s.human_age_ma = HUMAN_AGE_MA;
    s.ratio = HUMAN_AGE_MA / ED_EARTH_AGE_MA;

    /* If Earth's age were compressed to 24 hours,
     * how many seconds before midnight do humans arrive? */
    s.if_earth_were_24h = s.ratio * SECONDS_PER_DAY;

    /* If Earth's age were compressed to 1 year,
     * what day do humans arrive? (1-365) */
    s.if_earth_were_year = DAYS_PER_YEAR * (1.0 - s.ratio);

    s.analogy = "If Earth's history were a 24-hour day, "
                "humans arrive less than a second before midnight";

    return s;
}

const char *ed_drama_text(double ma)
{
    for (int i = 0; i < DRAMA_ENTRY_COUNT; i++) {
        const drama_entry_t *e = &DRAMA_ENTRIES[i];
        /* Match: start_ma >= ma > end_ma, except end_ma==0 matches ma==0 */
        if (ma <= e->start_ma &&
            (ma > e->end_ma || (ma == 0.0 && e->end_ma == 0.0))) {
            return e->text;
        }
    }
    /* Fallback for times outside range */
    return "Earth endures";
}

ed_snapshot_t ed_snapshot_at(double ma)
{
    ed_snapshot_t s;
    memset(&s, 0, sizeof(s));

    s.ma = ma;

    /* Geological context from geo_time */
    geo_unit_t era = geo_era_at(ma);
    geo_unit_t period = geo_period_at(ma);

    s.era_name = era.name != NULL ? era.name : "Pre-geological";
    s.period_name = period.name != NULL ? period.name : "Pre-geological";

    /* Atmosphere */
    s.atmosphere = ed_atmosphere_at(ma);

    /* Biosphere */
    s.biosphere = ed_biosphere_at(ma);

    /* Climate context */
    climate_event_t clim = climate_event_at(ma);
    if (clim.id >= 0) {
        s.temp_anomaly_c = clim.temp_anomaly_c;
    } else {
        s.temp_anomaly_c = 0.0;
    }

    /* Glaciation */
    s.is_glaciated = climate_is_glaciated(ma) ? 1 : 0;

    /* Extinction (10 Ma tolerance for "near" an extinction) */
    s.is_extinction = fossil_is_extinction(ma, 5.0) ? 1 : 0;

    /* Drama text */
    s.drama_text = ed_drama_text(ma);

    return s;
}
