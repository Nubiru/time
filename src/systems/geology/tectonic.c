/* tectonic.c — Supercontinent cycle and major tectonic events.
 * Static const data with lookup functions. Pure module, no side effects.
 * Data sources: Wikipedia supercontinent cycle (public domain),
 * Geological Society of America timescale. */

#include "tectonic.h"

#include <string.h>

/* --- Static data: 7 supercontinents (oldest first) --- */

static const supercontinent_t SUPERCONTINENTS[SUPERCONTINENT_COUNT] = {
    { 0, "Vaalbara",         3600, 2800,
      "Earliest proposed supercontinent (Kaapvaal + Pilbara cratons)" },
    { 1, "Kenorland",        2700, 2100,
      "Archean supercontinent (Laurentia, Baltica, Western Australia)" },
    { 2, "Columbia (Nuna)",  1800, 1350,
      "First well-documented supercontinent" },
    { 3, "Rodinia",          1100,  750,
      "Pre-Cambrian supercontinent, breakup led to Snowball Earth" },
    { 4, "Pannotia",          650,  540,
      "Brief supercontinent, fragmented to form Gondwana + Laurentia" },
    { 5, "Pangaea",           335,  175,
      "Most famous supercontinent, all landmasses united" },
    { 6, "Pangaea Proxima",   250,  250,
      "Predicted future supercontinent (~250 Myr from now, placeholder)" }
};

/* --- Static data: 21 major tectonic events (oldest first) --- */

#define EVENT_COUNT 21

static const tectonic_event_t EVENTS[EVENT_COUNT] = {
    {  0, "Isua Greenstone Belt",                3800, 3700, "volcanism",
       "Oldest preserved crustal rocks" },
    {  1, "Barberton Orogeny",                   3500, 3200, "orogeny",
       "Early continental crust formation" },
    {  2, "Kenoran Orogeny",                     2700, 2600, "orogeny",
       "Assembly of Kenorland" },
    {  3, "Great Dyke emplacement",              2575, 2575, "volcanism",
       "Massive mafic intrusion, Zimbabwe craton" },
    {  4, "Trans-Hudson Orogeny",                1900, 1800, "orogeny",
       "Assembly of Columbia" },
    {  5, "Grenville Orogeny",                   1100,  980, "orogeny",
       "Assembly of Rodinia" },
    {  6, "Rodinia Breakup Rifting",              830,  750, "rift",
       "Breakup of Rodinia, may have triggered Snowball Earth" },
    {  7, "Pan-African Orogeny",                  650,  500, "orogeny",
       "Assembly of Gondwana" },
    {  8, "Iapetus Ocean Opening",                600,  500, "rift",
       "Ocean between Laurentia and Baltica" },
    {  9, "Caledonian Orogeny",                   490,  390, "orogeny",
       "Scotland-Norway collision" },
    { 10, "Acadian Orogeny",                      375,  325, "orogeny",
       "Eastern North America terrane accretion" },
    { 11, "Variscan Orogeny",                     370,  280, "orogeny",
       "Assembly of Pangaea (Europe collision)" },
    { 12, "Alleghenian Orogeny",                  325,  260, "orogeny",
       "Final assembly of Pangaea" },
    { 13, "Siberian Traps",                       252,  250, "volcanism",
       "Largest eruption in Earth history, Permian extinction" },
    { 14, "Central Atlantic Magmatic Province",   201,  200, "volcanism",
       "Pangaea breakup volcanism, end-Triassic extinction" },
    { 15, "Opening of the Atlantic",              180,    0, "rift",
       "Americas separate from Africa/Europe" },
    { 16, "Sevier Orogeny",                       140,   50, "orogeny",
       "Western North America mountain building" },
    { 17, "Deccan Traps",                          66,   65, "volcanism",
       "India flood basalts, K-Pg extinction co-factor" },
    { 18, "Alpine Orogeny",                        65,    0, "orogeny",
       "Alps, Himalayas, ongoing collision" },
    { 19, "East African Rift",                     25,    0, "rift",
       "Active continental rifting, future ocean basin" },
    { 20, "Yellowstone Hotspot",                   16,    0, "volcanism",
       "Snake River Plain to present caldera system" }
};

/* --- Supercontinent functions --- */

supercontinent_t tectonic_supercontinent_get(int index) {
    if (index < 0 || index >= SUPERCONTINENT_COUNT) {
        supercontinent_t invalid = { -1, NULL, 0.0, 0.0, NULL };
        return invalid;
    }
    return SUPERCONTINENTS[index];
}

supercontinent_t tectonic_supercontinent_at(double ma) {
    supercontinent_t none = { 0, NULL, 0.0, 0.0, NULL };
    for (int i = 0; i < SUPERCONTINENT_COUNT; i++) {
        /* Skip Pangaea Proxima (future placeholder with start==end) */
        if (SUPERCONTINENTS[i].start_ma == SUPERCONTINENTS[i].end_ma) {
            continue;
        }
        if (ma <= SUPERCONTINENTS[i].start_ma &&
            ma >= SUPERCONTINENTS[i].end_ma) {
            return SUPERCONTINENTS[i];
        }
    }
    return none;
}

int tectonic_supercontinent_count(void) {
    return SUPERCONTINENT_COUNT;
}

const char *tectonic_supercontinent_name_at(double ma) {
    supercontinent_t s = tectonic_supercontinent_at(ma);
    if (s.name != NULL) {
        return s.name;
    }
    return "None";
}

int tectonic_in_supercontinent(double ma) {
    supercontinent_t s = tectonic_supercontinent_at(ma);
    return s.name != NULL ? 1 : 0;
}

double tectonic_next_supercontinent(double ma) {
    /* Find the supercontinent whose start_ma is the largest value
     * that is still less than ma (i.e., forms after the given time
     * in geological convention where lower Ma = more recent). */
    double best = 0.0;
    for (int i = 0; i < SUPERCONTINENT_COUNT; i++) {
        double s = SUPERCONTINENTS[i].start_ma;
        if (s < ma && s > best) {
            best = s;
        }
    }
    return best;
}

/* --- Tectonic event functions --- */

tectonic_event_t tectonic_event_get(int index) {
    if (index < 0 || index >= EVENT_COUNT) {
        tectonic_event_t invalid = { -1, NULL, 0.0, 0.0, NULL, NULL };
        return invalid;
    }
    return EVENTS[index];
}

int tectonic_event_count(void) {
    return EVENT_COUNT;
}

int tectonic_events_between(double start_ma, double end_ma,
                            int *out_indices, int out_max) {
    if (out_indices == NULL || out_max <= 0) {
        return 0;
    }

    /* Ensure start_ma >= end_ma (older first in geological convention) */
    if (start_ma < end_ma) {
        double tmp = start_ma;
        start_ma = end_ma;
        end_ma = tmp;
    }

    int count = 0;
    for (int i = 0; i < EVENT_COUNT && count < out_max; i++) {
        /* Event overlaps range if event.start_ma >= end_ma AND event.end_ma <= start_ma */
        if (EVENTS[i].start_ma >= end_ma && EVENTS[i].end_ma <= start_ma) {
            out_indices[count++] = i;
        }
    }
    return count;
}

int tectonic_events_by_type(const char *type,
                            int *out_indices, int out_max) {
    if (type == NULL || out_indices == NULL || out_max <= 0) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < EVENT_COUNT && count < out_max; i++) {
        if (strcmp(EVENTS[i].type, type) == 0) {
            out_indices[count++] = i;
        }
    }
    return count;
}
