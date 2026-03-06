#include "aspects.h"
#include <math.h>

/* Exact angles for each Ptolemaic aspect */
static const double ASPECT_ANGLES[] = {
    0.0,    /* ASPECT_CONJUNCTION */
    60.0,   /* ASPECT_SEXTILE */
    90.0,   /* ASPECT_SQUARE */
    120.0,  /* ASPECT_TRINE */
    180.0   /* ASPECT_OPPOSITION */
};

static const char *ASPECT_NAMES[] = {
    "Conjunction", "Sextile", "Square", "Trine", "Opposition", "None"
};

static const char *ASPECT_SYMBOLS[] = {
    "\xe2\x98\x8c", /* Conjunction ☌ */
    "\xe2\x9a\xb9", /* Sextile     ⚹ */
    "\xe2\x96\xa1", /* Square      □ */
    "\xe2\x96\xb3", /* Trine       △ */
    "\xe2\x98\x8d", /* Opposition  ☍ */
    "-"              /* None          */
};

#define ASPECT_TYPE_COUNT 5

double angular_separation(double lon_a, double lon_b) {
    double diff = fmod(fabs(lon_a - lon_b), 360.0);
    if (diff > 180.0) diff = 360.0 - diff;
    return diff;
}

aspect_type_t aspect_check(double separation, double max_orb) {
    for (int i = 0; i < ASPECT_TYPE_COUNT; i++) {
        if (fabs(separation - ASPECT_ANGLES[i]) <= max_orb)
            return (aspect_type_t)i;
    }
    return ASPECT_NONE;
}

double aspect_orb(double separation, aspect_type_t type) {
    if (type == ASPECT_NONE) return 0.0;
    return fabs(separation - ASPECT_ANGLES[type]);
}

aspect_list_t aspects_find_all(const double longitudes[8], double max_orb) {
    aspect_list_t list;
    list.count = 0;

    for (int a = 0; a < 7; a++) {
        for (int b = a + 1; b < 8; b++) {
            double sep = angular_separation(longitudes[a], longitudes[b]);
            aspect_type_t type = aspect_check(sep, max_orb);
            if (type != ASPECT_NONE) {
                aspect_t *asp = &list.aspects[list.count];
                asp->planet_a = a;
                asp->planet_b = b;
                asp->type = type;
                asp->orb = aspect_orb(sep, type);
                list.count++;
            }
        }
    }
    return list;
}

const char *aspect_name(aspect_type_t type) {
    if (type >= ASPECT_CONJUNCTION && type <= ASPECT_OPPOSITION)
        return ASPECT_NAMES[type];
    return "None";
}

const char *aspect_symbol(aspect_type_t type) {
    if (type >= ASPECT_CONJUNCTION && type <= ASPECT_OPPOSITION)
        return ASPECT_SYMBOLS[type];
    return "-";
}
