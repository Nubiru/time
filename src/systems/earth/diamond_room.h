/* diamond_room.h — Diamond Room procedural crystal data model.
 *
 * Generates deterministic crystal geometry parameters from a user's
 * birth profile and exploration state. The "ultimate introvert space"
 * — a procedural crystal room unique to each user, growing as they
 * explore Time.
 *
 * Geometry governed by phi (golden ratio). Hues distributed by the
 * golden angle (137.508 deg). Crystal system derived from zodiac element.
 *
 * L3.9 — PERSONA stream.
 *
 * Source: crystal system classification from IUCr (International Union
 * of Crystallography); golden angle from phyllotaxis in botanical math.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_DIAMOND_ROOM_H
#define TIME_DIAMOND_ROOM_H

#define DR_MAX_FACETS      16   /* One per knowledge system */
#define DR_GOLDEN_ANGLE    137.50776405003785  /* degrees: 360 * (1 - 1/phi) */
#define DR_GOLDEN_ANGLE_N  0.38196601125010515 /* normalized: golden_angle / 360 */

/* Six crystal systems (IUCr classification), mapped from zodiac element. */
typedef enum {
    DR_CRYSTAL_CUBIC = 0,        /* Fire signs: Aries, Leo, Sagittarius */
    DR_CRYSTAL_HEXAGONAL,        /* Earth signs: Taurus, Virgo, Capricorn */
    DR_CRYSTAL_TETRAGONAL,       /* Air signs: Gemini, Libra, Aquarius */
    DR_CRYSTAL_ORTHORHOMBIC,     /* Water signs: Cancer, Scorpio, Pisces */
    DR_CRYSTAL_MONOCLINIC,       /* Unknown zodiac (graceful default) */
    DR_CRYSTAL_TRICLINIC,        /* Reserved */
    DR_CRYSTAL_COUNT
} dr_crystal_system_t;

/* Pre-computed input from birth profile + exploration state. */
typedef struct {
    /* Birth data (0 or -1 = unknown) */
    int kin;                /* 1-260 */
    int seal;               /* 0-19 */
    int tone;               /* 1-13 */
    int sun_sign;           /* 0-11 */
    int hexagram;           /* 1-64 */
    int hd_sun_gate;        /* 1-64 */
    int chinese_animal;     /* 0-11 */
    int chinese_element;    /* 0-4 */
    int castle;             /* 0-4 */

    /* Exploration state */
    int systems_explored;                /* 0-16 count */
    int explored[DR_MAX_FACETS];         /* 1=visited per system */
    double engagement[DR_MAX_FACETS];    /* 0.0-1.0 per system */
    int consecutive_days;                /* current streak */
    int total_visits;                    /* lifetime sessions */
    double total_time_sec;               /* total time in app */
    int milestone_count;                 /* milestones earned (0-11) */

    /* Current moment */
    double hour_fraction;   /* 0.0-1.0 (fraction of day: 0=midnight, 0.5=noon) */
} dr_input_t;

/* Crystal seed — deterministic from birth data. */
typedef struct {
    dr_crystal_system_t system;  /* crystal system from zodiac element */
    int symmetry_order;          /* rotational symmetry 2-6 (from castle) */
    double base_scale;           /* overall size 0.5-1.0 (from HD gate, phi-scaled) */
    double hue_base;             /* 0.0-1.0 primary hue (from hexagram, golden angle) */
    double hue_accent;           /* 0.0-1.0 accent hue (from seal color) */
    double rotation_seed;        /* initial rotation radians (from Chinese animal+element) */
    int vertex_count;            /* base shape vertices 12-60 (from tone) */
} dr_seed_t;

/* Per-facet properties — one per knowledge system. */
typedef struct {
    int system_id;       /* 0-15 */
    double hue;          /* 0.0-1.0 (golden-angle-distributed) */
    double saturation;   /* 0.0-1.0 */
    double brightness;   /* 0.0-1.0 */
    double size;         /* relative size 0.0-1.0 */
    double opacity;      /* 0.0-1.0 */
    int active;          /* 1 if system explored */
} dr_facet_t;

/* Growth parameters — from exploration state. */
typedef struct {
    int active_facets;       /* systems explored (0-16) */
    int total_facets;        /* total possible (16) */
    double complexity;       /* 0.0-1.0 subdivision level */
    double luminosity;       /* 0.0-1.0 overall glow */
    double particle_density; /* 0.0-1.0 ambient particles */
    double growth_ratio;     /* 0.0-1.0 how "grown" */
} dr_growth_t;

/* Room ambience — atmospheric parameters. */
typedef struct {
    double bg_hue;           /* 0.0-1.0 background hue (time of day) */
    double bg_brightness;    /* 0.0-1.0 background brightness */
    double ambient_glow;     /* 0.0-1.0 overall glow */
    double rotation_speed;   /* radians/sec (slower = more explored) */
} dr_ambience_t;

/* Complete Diamond Room. */
typedef struct {
    dr_seed_t seed;
    dr_growth_t growth;
    dr_facet_t facets[DR_MAX_FACETS];
    int facet_count;
    dr_ambience_t ambience;
} dr_room_t;

/* Create a default (zeroed) input. All birth fields unknown, no exploration. */
dr_input_t dr_default_input(void);

/* Compute crystal seed from birth data. Unknown fields produce defaults. */
dr_seed_t dr_compute_seed(const dr_input_t *input);

/* Compute growth parameters from exploration state. */
dr_growth_t dr_compute_growth(const dr_input_t *input);

/* Compute a single facet's properties. system_id must be 0..DR_MAX_FACETS-1. */
dr_facet_t dr_compute_facet(const dr_input_t *input, int system_id);

/* Compute room ambience from current moment + growth. */
dr_ambience_t dr_compute_ambience(const dr_input_t *input,
                                   const dr_growth_t *growth);

/* Compute the complete Diamond Room. */
dr_room_t dr_compute(const dr_input_t *input);

/* Human-readable crystal system name. Returns "?" for invalid. */
const char *dr_crystal_name(dr_crystal_system_t system);

/* Compute the golden-angle-distributed hue for a system index (0-15).
 * Returns 0.0-1.0. Returns 0.0 for out-of-range. */
double dr_system_hue(int system_id);

/* Number of crystal systems. */
int dr_crystal_system_count(void);

#endif /* TIME_DIAMOND_ROOM_H */
