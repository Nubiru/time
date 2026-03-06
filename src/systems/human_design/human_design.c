#include "human_design.h"

#include <math.h>

static const double HD_GATE_WIDTH = 5.625; /* 360.0 / 64 */

/* Gate sequence around the ecliptic, starting at 0 Aries.
 * Position 0 = 0.000-5.625, position 1 = 5.625-11.250, etc. */
static const int GATE_SEQUENCE[64] = {
    17, 21, 51, 42,  3, 27, 24,  2, 23,  8, 20, 16,
    35, 45, 12, 15, 52, 39, 53, 62, 56, 31, 33,  7,
     4, 29, 59, 40, 64, 47,  6, 46, 18, 48, 57, 32,
    50, 28, 44,  1, 43, 14, 34,  9,  5, 26, 11, 10,
    58, 38, 54, 61, 60, 41, 19, 13, 49, 30, 55, 37,
    63, 22, 36, 25
};

/* Gate names indexed by gate number - 1 */
static const char *GATE_NAMES[64] = {
    "The Creative",             /* 1  */
    "The Receptive",            /* 2  */
    "Ordering",                 /* 3  */
    "Formulization",            /* 4  */
    "Fixed Rhythms",            /* 5  */
    "Friction",                 /* 6  */
    "The Role of the Self",     /* 7  */
    "Contribution",             /* 8  */
    "Focus",                    /* 9  */
    "Treading",                 /* 10 */
    "Peace",                    /* 11 */
    "Caution",                  /* 12 */
    "The Listener",             /* 13 */
    "Power Skills",             /* 14 */
    "Extremes",                 /* 15 */
    "Skills",                   /* 16 */
    "Opinions",                 /* 17 */
    "Correction",               /* 18 */
    "Wanting",                  /* 19 */
    "The Now",                  /* 20 */
    "The Hunter",               /* 21 */
    "Openness",                 /* 22 */
    "Assimilation",             /* 23 */
    "Rationalization",          /* 24 */
    "The Spirit of the Self",   /* 25 */
    "The Egoist",               /* 26 */
    "Nourishment",              /* 27 */
    "The Game Player",          /* 28 */
    "The Abysmal",              /* 29 */
    "Recognition of Feelings",  /* 30 */
    "Leading",                  /* 31 */
    "Continuity",               /* 32 */
    "Privacy",                  /* 33 */
    "Power",                    /* 34 */
    "Change",                   /* 35 */
    "Crisis",                   /* 36 */
    "Friendship",               /* 37 */
    "The Fighter",              /* 38 */
    "Provocation",              /* 39 */
    "Aloneness",                /* 40 */
    "Contraction",              /* 41 */
    "Increase",                 /* 42 */
    "Insight",                  /* 43 */
    "Alertness",                /* 44 */
    "The Gatherer",             /* 45 */
    "Discovery",                /* 46 */
    "Realization",              /* 47 */
    "Depth",                    /* 48 */
    "Principles",               /* 49 */
    "Values",                   /* 50 */
    "Shock",                    /* 51 */
    "Stillness",                /* 52 */
    "Beginnings",               /* 53 */
    "Ambition",                 /* 54 */
    "Spirit",                   /* 55 */
    "Stimulation",              /* 56 */
    "Intuition",                /* 57 */
    "Vitality",                 /* 58 */
    "Sexuality",                /* 59 */
    "Limitation",               /* 60 */
    "Mystery",                  /* 61 */
    "Detail",                   /* 62 */
    "Doubt",                    /* 63 */
    "Confusion",                /* 64 */
};

/* Gate keywords indexed by gate number - 1 */
static const char *GATE_KEYWORDS[64] = {
    "Self-Expression",                /* 1  */
    "Direction",                      /* 2  */
    "Innovation",                     /* 3  */
    "Mental Solutions",               /* 4  */
    "Waiting",                        /* 5  */
    "Conflict Resolution",            /* 6  */
    "Leadership",                     /* 7  */
    "Togetherness",                   /* 8  */
    "Determination",                  /* 9  */
    "Behavior of the Self",           /* 10 */
    "Ideas",                          /* 11 */
    "Standstill",                     /* 12 */
    "Fellowship",                     /* 13 */
    "Possession",                     /* 14 */
    "Modesty",                        /* 15 */
    "Enthusiasm",                     /* 16 */
    "Following",                      /* 17 */
    "Work on What Has Been Spoiled",  /* 18 */
    "Approach",                       /* 19 */
    "Contemplation",                  /* 20 */
    "Control",                        /* 21 */
    "Grace",                          /* 22 */
    "Splitting Apart",                /* 23 */
    "Return",                         /* 24 */
    "Innocence",                      /* 25 */
    "The Taming Power",               /* 26 */
    "Caring",                         /* 27 */
    "Preponderance",                  /* 28 */
    "Saying Yes",                     /* 29 */
    "The Clinging Fire",              /* 30 */
    "Influence",                      /* 31 */
    "Duration",                       /* 32 */
    "Retreat",                        /* 33 */
    "Great Power",                    /* 34 */
    "Progress",                       /* 35 */
    "Darkening of the Light",         /* 36 */
    "The Family",                     /* 37 */
    "Opposition",                     /* 38 */
    "Obstruction",                    /* 39 */
    "Deliverance",                    /* 40 */
    "Decrease",                       /* 41 */
    "Growth",                         /* 42 */
    "Breakthrough",                   /* 43 */
    "Coming to Meet",                 /* 44 */
    "Gathering Together",             /* 45 */
    "Pushing Upward",                 /* 46 */
    "Oppression",                     /* 47 */
    "The Well",                       /* 48 */
    "Revolution",                     /* 49 */
    "The Cauldron",                   /* 50 */
    "The Arousing",                   /* 51 */
    "Keeping Still",                  /* 52 */
    "Development",                    /* 53 */
    "The Marrying Maiden",            /* 54 */
    "Abundance",                      /* 55 */
    "The Wanderer",                   /* 56 */
    "The Gentle",                     /* 57 */
    "The Joyous",                     /* 58 */
    "Dispersion",                     /* 59 */
    "Acceptance",                     /* 60 */
    "Inner Truth",                    /* 61 */
    "Preponderance of the Small",     /* 62 */
    "After Completion",               /* 63 */
    "Before Completion",              /* 64 */
};

static double normalize_longitude(double lon)
{
    lon = fmod(lon, 360.0);
    if (lon < 0.0) lon += 360.0;
    return lon;
}

hd_gate_t hd_gate_from_longitude(double ecliptic_longitude)
{
    hd_gate_t g;
    double lon = normalize_longitude(ecliptic_longitude);

    int pos = (int)(lon / HD_GATE_WIDTH);
    if (pos >= 64) pos = 63;

    double offset = lon - pos * HD_GATE_WIDTH;
    int line = (int)((offset / HD_GATE_WIDTH) * 6.0) + 1;
    if (line > 6) line = 6;

    g.gate = GATE_SEQUENCE[pos];
    g.line = line;
    g.longitude = ecliptic_longitude;
    return g;
}

hd_incarnation_t hd_incarnation(double sun_longitude)
{
    hd_incarnation_t ic;
    ic.sun = hd_gate_from_longitude(sun_longitude);
    ic.earth = hd_gate_from_longitude(sun_longitude + 180.0);
    return ic;
}

const char *hd_gate_name(int gate)
{
    if (gate < 1 || gate > 64)
        return "?";
    return GATE_NAMES[gate - 1];
}

const char *hd_gate_keyword(int gate)
{
    if (gate < 1 || gate > 64)
        return "?";
    return GATE_KEYWORDS[gate - 1];
}

int hd_gate_at_degree(double ecliptic_longitude)
{
    return hd_gate_from_longitude(ecliptic_longitude).gate;
}
