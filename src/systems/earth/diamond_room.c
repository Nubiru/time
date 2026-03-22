/* diamond_room.c — Diamond Room procedural crystal data model.
 *
 * Generates deterministic crystal geometry from birth profile data.
 * All proportions governed by phi. Hues distributed by golden angle.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "diamond_room.h"

#include <math.h>
#include <string.h>

/* --- Constants --- */

#define PI          3.14159265358979323846
#define PHI         1.6180339887498948482
#define PHI_INV     0.6180339887498948482
#define PHI_INV2    0.3819660112501051518
#define PHI_INV3    0.2360679774997896964
#define PHI_INV4    0.1458980337503154554

#define MAX_MILESTONES  11   /* EP_MS_COUNT from exploration_progress */
#define ONE_WEEK_SEC    604800.0

/* --- Crystal system names --- */

static const char *CRYSTAL_NAMES[] = {
    "Cubic",
    "Hexagonal",
    "Tetragonal",
    "Orthorhombic",
    "Monoclinic",
    "Triclinic"
};

/* --- Zodiac element mapping --- */

/* Zodiac signs grouped by element:
 *   Fire  (0=Aries, 4=Leo, 8=Sag)     → Cubic
 *   Earth (1=Taurus, 5=Virgo, 9=Cap)   → Hexagonal
 *   Air   (2=Gemini, 6=Libra, 10=Aqu)  → Tetragonal
 *   Water (3=Cancer, 7=Scorpio, 11=Pis) → Orthorhombic */

static dr_crystal_system_t zodiac_to_crystal(int sun_sign)
{
    if (sun_sign < 0 || sun_sign > 11) {
        return DR_CRYSTAL_MONOCLINIC;
    }
    /* Element = sign % 4: 0=fire, 1=earth, 2=air, 3=water
     * But zodiac ordering is: Aries(fire), Taurus(earth), Gemini(air), Cancer(water)
     * So sign % 4 gives element directly. */
    int element = sun_sign % 4;
    switch (element) {
    case 0: return DR_CRYSTAL_CUBIC;         /* Fire */
    case 1: return DR_CRYSTAL_HEXAGONAL;     /* Earth */
    case 2: return DR_CRYSTAL_TETRAGONAL;    /* Air */
    case 3: return DR_CRYSTAL_ORTHORHOMBIC;  /* Water */
    default: return DR_CRYSTAL_MONOCLINIC;
    }
}

/* --- Clamp helper --- */

static double clamp01(double v)
{
    if (v < 0.0) return 0.0;
    if (v > 1.0) return 1.0;
    return v;
}

/* --- Public API --- */

dr_input_t dr_default_input(void)
{
    dr_input_t in;
    memset(&in, 0, sizeof(in));
    in.seal = -1;
    in.sun_sign = -1;
    in.chinese_animal = -1;
    in.chinese_element = -1;
    in.castle = -1;
    return in;
}

dr_seed_t dr_compute_seed(const dr_input_t *input)
{
    dr_seed_t s;
    memset(&s, 0, sizeof(s));

    /* Defaults for unknown input */
    s.system = DR_CRYSTAL_MONOCLINIC;
    s.symmetry_order = 4;
    s.base_scale = PHI_INV;
    s.vertex_count = 20;

    if (!input) return s;

    /* Crystal system from zodiac */
    s.system = zodiac_to_crystal(input->sun_sign);

    /* Symmetry order from castle (0-4 → 2-6) */
    if (input->castle >= 0 && input->castle <= 4) {
        s.symmetry_order = input->castle + 2;
    }

    /* Hue base from hexagram via golden angle */
    if (input->hexagram >= 1 && input->hexagram <= 64) {
        s.hue_base = fmod((double)input->hexagram * DR_GOLDEN_ANGLE_N, 1.0);
    }

    /* Accent hue from seal color (seal % 4: 0=Red, 1=White, 2=Blue, 3=Yellow)
     * Mapped to hue: Red=0.0, White=0.25, Blue=0.5, Yellow=0.75 */
    if (input->seal >= 0 && input->seal <= 19) {
        int color = input->seal % 4;
        s.hue_accent = color * 0.25;
    }

    /* Base scale from HD sun gate (1-64 → 0.5-1.0, phi-distributed) */
    if (input->hd_sun_gate >= 1 && input->hd_sun_gate <= 64) {
        double ratio = (double)(input->hd_sun_gate - 1) / 63.0;
        s.base_scale = 0.5 + ratio * 0.5;
    }

    /* Rotation seed from Chinese animal + element */
    if (input->chinese_animal >= 0 && input->chinese_element >= 0) {
        double degrees = input->chinese_animal * 30.0 + input->chinese_element * 72.0;
        s.rotation_seed = degrees * PI / 180.0;
    }

    /* Vertex count from tone (1-13 → 12-60, step of 4) */
    if (input->tone >= 1 && input->tone <= 13) {
        s.vertex_count = 8 + input->tone * 4;
    }

    return s;
}

dr_growth_t dr_compute_growth(const dr_input_t *input)
{
    dr_growth_t g;
    memset(&g, 0, sizeof(g));
    g.total_facets = DR_MAX_FACETS;

    if (!input) return g;

    int explored = input->systems_explored;
    if (explored < 0) explored = 0;
    if (explored > DR_MAX_FACETS) explored = DR_MAX_FACETS;
    g.active_facets = explored;

    double coverage = (double)explored / (double)DR_MAX_FACETS;

    /* Complexity: sqrt of coverage (gradual ramp) scaled by phi_inv */
    g.complexity = clamp01(sqrt(coverage) * PHI_INV);

    /* Luminosity: time spent (saturating at 1 week) scaled by phi_inv
     * plus streak bonus */
    double time_factor = 0.0;
    if (input->total_time_sec > 0.0) {
        time_factor = input->total_time_sec / ONE_WEEK_SEC;
        if (time_factor > 1.0) time_factor = 1.0;
    }
    double streak_bonus = 0.0;
    if (input->consecutive_days > 0) {
        streak_bonus = clamp01((double)input->consecutive_days / 365.0) * PHI_INV3;
    }
    g.luminosity = clamp01(time_factor * PHI_INV + streak_bonus);

    /* Particle density: milestones earned scaled by phi_inv */
    int milestones = input->milestone_count;
    if (milestones < 0) milestones = 0;
    if (milestones > MAX_MILESTONES) milestones = MAX_MILESTONES;
    g.particle_density = clamp01((double)milestones / (double)MAX_MILESTONES * PHI_INV);

    /* Growth ratio: coverage scaled by phi_inv plus visits bonus */
    double visit_bonus = 0.0;
    if (input->total_visits > 0) {
        visit_bonus = clamp01((double)input->total_visits / 1000.0) * PHI_INV3;
    }
    g.growth_ratio = clamp01(coverage * PHI_INV + visit_bonus);

    return g;
}

dr_facet_t dr_compute_facet(const dr_input_t *input, int system_id)
{
    dr_facet_t f;
    memset(&f, 0, sizeof(f));
    f.system_id = system_id;

    /* Out-of-range or NULL: return inactive facet */
    if (!input || system_id < 0 || system_id >= DR_MAX_FACETS) {
        return f;
    }

    /* Hue: golden-angle-distributed */
    f.hue = dr_system_hue(system_id);

    f.active = input->explored[system_id] ? 1 : 0;
    double eng = input->engagement[system_id];
    if (eng < 0.0) eng = 0.0;
    if (eng > 1.0) eng = 1.0;

    if (f.active) {
        /* Active facet: saturation, brightness, size, opacity scale with engagement */
        f.saturation = PHI_INV + eng * PHI_INV2;
        f.brightness = PHI_INV + eng * PHI_INV2;
        f.size = 0.5 + eng * 0.5;
        f.opacity = PHI_INV + eng * PHI_INV2;
    } else {
        /* Inactive facet: dim, small, nearly transparent */
        f.saturation = PHI_INV3;
        f.brightness = 0.1;
        f.size = PHI_INV3;
        f.opacity = PHI_INV4;
    }

    return f;
}

dr_ambience_t dr_compute_ambience(const dr_input_t *input,
                                   const dr_growth_t *growth)
{
    dr_ambience_t a;
    memset(&a, 0, sizeof(a));

    /* Default rotation speed (when no growth data) */
    a.rotation_speed = PHI_INV * 0.1;

    double hour = 0.0;
    if (input) {
        hour = input->hour_fraction;
        if (hour < 0.0) hour = 0.0;
        if (hour > 1.0) hour = 1.0;
    }

    /* Background hue: direct mapping from time of day */
    a.bg_hue = hour;

    /* Background brightness: sine curve, 0 at midnight, max at noon.
     * brightness = 0.5 * (1 + sin(hour * 2pi - pi/2)) * phi_inv */
    a.bg_brightness = clamp01(0.5 * (1.0 + sin(hour * 2.0 * PI - PI / 2.0)) * PHI_INV);

    if (growth) {
        /* Ambient glow from growth ratio */
        a.ambient_glow = clamp01(growth->growth_ratio * PHI_INV);

        /* Rotation speed: slower as complexity increases */
        a.rotation_speed = (1.0 - growth->complexity) * PHI_INV * 0.1;
        if (a.rotation_speed < 0.0) a.rotation_speed = 0.0;
    }

    return a;
}

dr_room_t dr_compute(const dr_input_t *input)
{
    dr_room_t r;
    memset(&r, 0, sizeof(r));

    /* Use default input if NULL */
    dr_input_t def;
    const dr_input_t *in = input;
    if (!in) {
        def = dr_default_input();
        in = &def;
    }

    r.seed = dr_compute_seed(in);
    r.growth = dr_compute_growth(in);
    r.facet_count = DR_MAX_FACETS;
    for (int i = 0; i < DR_MAX_FACETS; i++) {
        r.facets[i] = dr_compute_facet(in, i);
    }
    r.ambience = dr_compute_ambience(in, &r.growth);

    return r;
}

const char *dr_crystal_name(dr_crystal_system_t system)
{
    if (system < 0 || system >= DR_CRYSTAL_COUNT) {
        return "?";
    }
    return CRYSTAL_NAMES[system];
}

double dr_system_hue(int system_id)
{
    if (system_id < 0 || system_id >= DR_MAX_FACETS) {
        return 0.0;
    }
    return fmod((double)system_id * DR_GOLDEN_ANGLE_N, 1.0);
}

int dr_crystal_system_count(void)
{
    return DR_CRYSTAL_COUNT;
}

/* --- Convenience Accessors (E7) --- */

double dr_facet_brightness(const dr_room_t *room, int system_id)
{
    if (!room || system_id < 0 || system_id >= room->facet_count) {
        return 0.0;
    }
    return room->facets[system_id].brightness;
}

double dr_total_luminosity(const dr_room_t *room)
{
    if (!room) return 0.0;
    return room->growth.luminosity;
}

void dr_birth_color(const dr_seed_t *seed, float rgba[4])
{
    if (!seed || !rgba) {
        if (rgba) {
            rgba[0] = rgba[1] = rgba[2] = 0.5f;
            rgba[3] = 1.0f;
        }
        return;
    }

    /* HSL to RGB: hue from seed, full saturation, phi-scaled lightness */
    double h = seed->hue_base * 6.0; /* 0-6 for HSL conversion */
    double s = (float)PHI_INV;       /* saturation = phi^-1 for richness */
    double l = 0.5 + seed->base_scale * 0.3; /* lightness 0.5-0.8 */

    /* HSL → RGB (simplified) */
    double c = (1.0 - fabs(2.0 * l - 1.0)) * s;
    double x = c * (1.0 - fabs(fmod(h, 2.0) - 1.0));
    double m = l - c / 2.0;

    double r = 0.0, g = 0.0, b = 0.0;
    if (h < 1.0)      { r = c; g = x; b = 0.0; }
    else if (h < 2.0) { r = x; g = c; b = 0.0; }
    else if (h < 3.0) { r = 0.0; g = c; b = x; }
    else if (h < 4.0) { r = 0.0; g = x; b = c; }
    else if (h < 5.0) { r = x; g = 0.0; b = c; }
    else               { r = c; g = 0.0; b = x; }

    rgba[0] = (float)clamp01(r + m);
    rgba[1] = (float)clamp01(g + m);
    rgba[2] = (float)clamp01(b + m);
    rgba[3] = 1.0f;
}

/* --- Usage Bridge --- */

void dr_fill_engagement(dr_input_t *input, const double *scores, int count)
{
    if (!input || !scores) return;
    int n = count;
    if (n > DR_MAX_FACETS) n = DR_MAX_FACETS;

    int explored_count = 0;
    for (int i = 0; i < n; i++) {
        double s = scores[i];
        if (s < 0.0) s = 0.0;
        if (s > 1.0) s = 1.0;
        input->engagement[i] = s;
        if (s > 0.0) {
            input->explored[i] = 1;
            explored_count++;
        }
    }
    input->systems_explored = explored_count;
}
