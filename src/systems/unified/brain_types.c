/* brain_types.c — Name lookup utilities for brain pipeline types.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "brain_types.h"

static const char *const CORRELATION_TYPE_NAMES[] = {
    "Structural",
    "Harmonic",
    "Approximate",
    "Coincidental"
};

static const char *const INSIGHT_TYPE_NAMES[] = {
    "Convergence",
    "Number",
    "Cycle",
    "Festival",
    "Astronomical",
    "Quiet"
};

const char *br_correlation_type_name(int type) {
    if (type < 0 || type >= BR_CORR_COUNT) return "?";
    return CORRELATION_TYPE_NAMES[type];
}

const char *br_insight_type_name(int type) {
    if (type < 0 || type >= BR_INSIGHT_COUNT) return "?";
    return INSIGHT_TYPE_NAMES[type];
}

int br_correlation_type_count(void) {
    return BR_CORR_COUNT;
}

int br_insight_type_count(void) {
    return BR_INSIGHT_COUNT;
}
