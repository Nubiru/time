/* climate_history.c — Major climate events in Earth's history.
 * Pure static const data. No malloc, no globals, no side effects.
 * Data sources: NASA paleoclimate, IPCC reports, Geologic Time Scale 2020. */

#include "climate_history.h"
#include <stddef.h>

/* --- Static event data (chronological, oldest first) --- */

static const climate_event_t CLIMATE_EVENTS[] = {
    { /* 0 */
        .id = 0,
        .name = "Hadean Bombardment Warming",
        .start_ma = 4500.0,
        .end_ma = 3800.0,
        .temp_anomaly_c = 50.0,
        .co2_ppm = 10000.0,
        .type = CLIMATE_TYPE_WARMING,
        .severity = CLIMATE_SEVERITY_MAJOR,
        .description = "Intense late heavy bombardment; molten surface, dense CO2 atmosphere"
    },
    { /* 1 */
        .id = 1,
        .name = "Great Oxidation Event",
        .start_ma = 2400.0,
        .end_ma = 2000.0,
        .temp_anomaly_c = -10.0,
        .co2_ppm = 200.0,
        .type = CLIMATE_TYPE_ATMOSPHERIC,
        .severity = CLIMATE_SEVERITY_CATASTROPHIC,
        .description = "Cyanobacteria produce O2; methane greenhouse collapses, triggering glaciation"
    },
    { /* 2 */
        .id = 2,
        .name = "Huronian Glaciation",
        .start_ma = 2400.0,
        .end_ma = 2100.0,
        .temp_anomaly_c = -30.0,
        .co2_ppm = 200.0,
        .type = CLIMATE_TYPE_GLACIATION,
        .severity = CLIMATE_SEVERITY_MAJOR,
        .description = "Oldest confirmed glaciation; linked to Great Oxidation Event"
    },
    { /* 3 */
        .id = 3,
        .name = "Boring Billion Stability",
        .start_ma = 1800.0,
        .end_ma = 800.0,
        .temp_anomaly_c = 0.0,
        .co2_ppm = 1000.0,
        .type = CLIMATE_TYPE_ATMOSPHERIC,
        .severity = CLIMATE_SEVERITY_MINOR,
        .description = "Long period of relative climate and tectonic stability"
    },
    { /* 4 */
        .id = 4,
        .name = "Sturtian Snowball Earth",
        .start_ma = 717.0,
        .end_ma = 660.0,
        .temp_anomaly_c = -50.0,
        .co2_ppm = 100.0,
        .type = CLIMATE_TYPE_GLACIATION,
        .severity = CLIMATE_SEVERITY_CATASTROPHIC,
        .description = "Global glaciation; ice extends to equator for ~57 million years"
    },
    { /* 5 */
        .id = 5,
        .name = "Marinoan Snowball Earth",
        .start_ma = 650.0,
        .end_ma = 635.0,
        .temp_anomaly_c = -50.0,
        .co2_ppm = 100.0,
        .type = CLIMATE_TYPE_GLACIATION,
        .severity = CLIMATE_SEVERITY_CATASTROPHIC,
        .description = "Second global glaciation; ends with extreme greenhouse rebound"
    },
    { /* 6 */
        .id = 6,
        .name = "Cambrian Explosion Warmth",
        .start_ma = 541.0,
        .end_ma = 485.0,
        .temp_anomaly_c = 10.0,
        .co2_ppm = 4000.0,
        .type = CLIMATE_TYPE_WARMING,
        .severity = CLIMATE_SEVERITY_MAJOR,
        .description = "Warm greenhouse climate coincides with rapid diversification of life"
    },
    { /* 7 */
        .id = 7,
        .name = "Late Ordovician Glaciation",
        .start_ma = 450.0,
        .end_ma = 430.0,
        .temp_anomaly_c = -8.0,
        .co2_ppm = 2000.0,
        .type = CLIMATE_TYPE_GLACIATION,
        .severity = CLIMATE_SEVERITY_MAJOR,
        .description = "Gondwanan ice sheets; second-largest Phanerozoic mass extinction"
    },
    { /* 8 */
        .id = 8,
        .name = "Carboniferous Ice Age",
        .start_ma = 360.0,
        .end_ma = 260.0,
        .temp_anomaly_c = -5.0,
        .co2_ppm = 300.0,
        .type = CLIMATE_TYPE_GLACIATION,
        .severity = CLIMATE_SEVERITY_MAJOR,
        .description = "Major glaciation; high O2 from coal forests, low CO2"
    },
    { /* 9 */
        .id = 9,
        .name = "Permian-Triassic Extinction",
        .start_ma = 252.0,
        .end_ma = 251.0,
        .temp_anomaly_c = 10.0,
        .co2_ppm = 2000.0,
        .type = CLIMATE_TYPE_WARMING,
        .severity = CLIMATE_SEVERITY_CATASTROPHIC,
        .description = "The Great Dying; Siberian Traps volcanism; 96% of marine species lost"
    },
    { /* 10 */
        .id = 10,
        .name = "PETM (Paleocene-Eocene)",
        .start_ma = 56.0,
        .end_ma = 54.0,
        .temp_anomaly_c = 6.0,
        .co2_ppm = 1000.0,
        .type = CLIMATE_TYPE_WARMING,
        .severity = CLIMATE_SEVERITY_MAJOR,
        .description = "Paleocene-Eocene Thermal Maximum; rapid carbon release, ocean acidification"
    },
    { /* 11 */
        .id = 11,
        .name = "Mid-Miocene Optimum",
        .start_ma = 17.0,
        .end_ma = 14.0,
        .temp_anomaly_c = 4.0,
        .co2_ppm = 500.0,
        .type = CLIMATE_TYPE_WARMING,
        .severity = CLIMATE_SEVERITY_MODERATE,
        .description = "Warm peak before late Cenozoic cooling; Antarctica partially ice-free"
    },
    { /* 12 */
        .id = 12,
        .name = "Pleistocene Ice Ages",
        .start_ma = 2.58,
        .end_ma = 0.012,
        .temp_anomaly_c = -6.0,
        .co2_ppm = 180.0,
        .type = CLIMATE_TYPE_OSCILLATION,
        .severity = CLIMATE_SEVERITY_MAJOR,
        .description = "Milankovitch-driven glacial-interglacial cycles; ~100 kyr periodicity"
    },
    { /* 13 */
        .id = 13,
        .name = "Holocene Optimum",
        .start_ma = 0.008,
        .end_ma = 0.005,
        .temp_anomaly_c = 1.0,
        .co2_ppm = 280.0,
        .type = CLIMATE_TYPE_WARMING,
        .severity = CLIMATE_SEVERITY_MINOR,
        .description = "Post-glacial warm period; rise of agriculture and early civilizations"
    },
    { /* 14 */
        .id = 14,
        .name = "Little Ice Age",
        .start_ma = 0.00065,
        .end_ma = 0.00015,
        .temp_anomaly_c = -1.0,
        .co2_ppm = 280.0,
        .type = CLIMATE_TYPE_GLACIATION,
        .severity = CLIMATE_SEVERITY_MINOR,
        .description = "Cool period; reduced solar activity (Maunder Minimum), volcanic aerosols"
    }
};

#define CLIMATE_EVENT_COUNT \
    ((int)(sizeof(CLIMATE_EVENTS) / sizeof(CLIMATE_EVENTS[0])))

/* --- Severity name lookup --- */

static const char *const SEVERITY_NAMES[] = {
    "Minor",
    "Moderate",
    "Major",
    "Catastrophic"
};

#define SEVERITY_NAME_COUNT \
    ((int)(sizeof(SEVERITY_NAMES) / sizeof(SEVERITY_NAMES[0])))

/* --- Type name lookup --- */

static const char *const TYPE_NAMES[] = {
    "Glaciation",
    "Warming",
    "Atmospheric",
    "Oscillation",
    "Impact"
};

#define TYPE_NAME_COUNT \
    ((int)(sizeof(TYPE_NAMES) / sizeof(TYPE_NAMES[0])))

/* --- Invalid event (returned for bad lookups) --- */

static climate_event_t climate_invalid(void) {
    climate_event_t e;
    e.id = -1;
    e.name = NULL;
    e.start_ma = 0.0;
    e.end_ma = 0.0;
    e.temp_anomaly_c = 0.0;
    e.co2_ppm = 0.0;
    e.type = CLIMATE_TYPE_GLACIATION;
    e.severity = CLIMATE_SEVERITY_MINOR;
    e.description = NULL;
    return e;
}

/* --- Public API --- */

int climate_event_count(void) {
    return CLIMATE_EVENT_COUNT;
}

climate_event_t climate_event_get(int index) {
    if (index < 0 || index >= CLIMATE_EVENT_COUNT) {
        return climate_invalid();
    }
    return CLIMATE_EVENTS[index];
}

climate_event_t climate_event_at(double ma) {
    climate_event_t best = climate_invalid();

    for (int i = 0; i < CLIMATE_EVENT_COUNT; i++) {
        const climate_event_t *e = &CLIMATE_EVENTS[i];
        /* start_ma >= ma >= end_ma (larger Ma = further back in time) */
        if (ma <= e->start_ma && ma >= e->end_ma) {
            if (best.id == -1 || e->severity > best.severity) {
                best = *e;
            }
        }
    }

    return best;
}

const char *climate_event_name(int index) {
    if (index < 0 || index >= CLIMATE_EVENT_COUNT) {
        return "?";
    }
    return CLIMATE_EVENTS[index].name;
}

const char *climate_severity_name(climate_severity_t severity) {
    int s = (int)severity;
    if (s < 0 || s >= SEVERITY_NAME_COUNT) {
        return "?";
    }
    return SEVERITY_NAMES[s];
}

const char *climate_type_name(climate_type_t type) {
    int t = (int)type;
    if (t < 0 || t >= TYPE_NAME_COUNT) {
        return "?";
    }
    return TYPE_NAMES[t];
}

int climate_events_of_type(climate_type_t type) {
    int count = 0;
    for (int i = 0; i < CLIMATE_EVENT_COUNT; i++) {
        if (CLIMATE_EVENTS[i].type == type) {
            count++;
        }
    }
    return count;
}

int climate_events_of_severity(climate_severity_t severity) {
    int count = 0;
    for (int i = 0; i < CLIMATE_EVENT_COUNT; i++) {
        if (CLIMATE_EVENTS[i].severity == severity) {
            count++;
        }
    }
    return count;
}

int climate_events_in_range(double start_ma, double end_ma,
                            int *out_indices, int max_results) {
    if (out_indices == NULL || max_results <= 0) {
        return 0;
    }

    int found = 0;
    for (int i = 0; i < CLIMATE_EVENT_COUNT && found < max_results; i++) {
        const climate_event_t *e = &CLIMATE_EVENTS[i];
        /* Overlap check: event overlaps [start_ma, end_ma] if
         * event.start_ma >= end_ma AND start_ma >= event.end_ma
         * (remember: larger Ma = further back in time) */
        if (e->start_ma >= end_ma && start_ma >= e->end_ma) {
            out_indices[found++] = i;
        }
    }

    return found;
}

bool climate_is_glaciated(double ma) {
    for (int i = 0; i < CLIMATE_EVENT_COUNT; i++) {
        const climate_event_t *e = &CLIMATE_EVENTS[i];
        if (e->type == CLIMATE_TYPE_GLACIATION &&
            ma <= e->start_ma && ma >= e->end_ma) {
            return true;
        }
    }
    return false;
}
