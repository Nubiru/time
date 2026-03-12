/*
 * orbit_trail_pack.c — Orbit trail render pack: GPU-ready vertex packing
 *
 * Generates Keplerian orbit ellipses for all 8 planets as interleaved
 * vertex arrays: position(vec3) + alpha(float) + color(vec3).
 *
 * Each orbit is sampled around the full ellipse starting at the planet's
 * current mean anomaly (brightest) and going backward (fading out).
 * Applies sqrt distance scaling to match planet_pass rendering.
 *
 * Pure module: no GL calls, no malloc, no globals, no side effects.
 */

#include "orbit_trail_pack.h"
#include "../systems/astronomy/orbit.h"
#include "../systems/astronomy/planet_data.h"
#include "../math/kepler.h"

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define DEG_TO_RAD (PI / 180.0)
#define OTP_PLANET_COUNT 8

/* Pre-defined orbit pointers indexed by planet (0-7). */
static const planet_orbit_t *OTP_ORBITS[OTP_PLANET_COUNT] = {
    &ORBIT_MERCURY, &ORBIT_VENUS, &ORBIT_EARTH, &ORBIT_MARS,
    &ORBIT_JUPITER, &ORBIT_SATURN, &ORBIT_URANUS, &ORBIT_NEPTUNE
};

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: Compute ecliptic Cartesian position from orbital elements
 * at a given mean anomaly. Returns distance in AU.
 * Ecliptic plane is XZ, Y is up (matching planet_pack convention).
 * ────────────────────────────────────────────────────────────────────── */

static double orbit_cartesian(const orbital_elements_t *elem,
                              double mean_anomaly_rad,
                              double *out_x, double *out_y, double *out_z)
{
    /* Solve Kepler's equation */
    double E = kepler_solve(mean_anomaly_rad, elem->e);

    /* True anomaly */
    double v = eccentric_to_true_anomaly(E, elem->e);

    /* Heliocentric distance */
    double r = elem->a * (1.0 - elem->e * cos(E));

    /* Position in the orbital plane */
    double x_orb = r * cos(v);
    double y_orb = r * sin(v);

    /* Orbital angles */
    double omega = (elem->omega_p - elem->omega_n) * DEG_TO_RAD;
    double Omega = elem->omega_n * DEG_TO_RAD;
    double inc   = elem->i * DEG_TO_RAD;

    double cos_omega = cos(omega);
    double sin_omega = sin(omega);
    double cos_Omega = cos(Omega);
    double sin_Omega = sin(Omega);
    double cos_i     = cos(inc);
    double sin_i     = sin(inc);

    /* Rotate from orbital plane to ecliptic coordinates.
     * Standard rotation: R_z(-Omega) * R_x(-i) * R_z(-omega)
     * Ecliptic: X-Z plane, Y up. */
    double x_rot = (cos_omega * cos_Omega - sin_omega * sin_Omega * cos_i) * x_orb +
                   (-sin_omega * cos_Omega - cos_omega * sin_Omega * cos_i) * y_orb;

    double y_ecl = (cos_omega * sin_Omega + sin_omega * cos_Omega * cos_i) * x_orb +
                   (-sin_omega * sin_Omega + cos_omega * cos_Omega * cos_i) * y_orb;

    double z_rot = (sin_omega * sin_i) * x_orb +
                   (cos_omega * sin_i) * y_orb;

    /* Map ecliptic to scene: ecliptic X -> scene X, ecliptic Y -> scene Z, ecliptic Z -> scene Y */
    *out_x = x_rot;
    *out_z = y_ecl;
    *out_y = z_rot;

    return r;
}

/* ──────────────────────────────────────────────────────────────────────────
 * Internal: Apply sqrt scaling to a position.
 * r_scene = sqrt(r_AU) * orbit_scale
 * ────────────────────────────────────────────────────────────────────── */

static void apply_sqrt_scale(double x, double y, double z,
                             float orbit_scale,
                             float *out_x, float *out_y, float *out_z)
{
    double r_linear = sqrt(x * x + y * y + z * z);
    if (r_linear < 1e-10 || orbit_scale == 0.0f) {
        *out_x = 0.0f;
        *out_y = 0.0f;
        *out_z = 0.0f;
        return;
    }
    double r_sqrt = sqrt(r_linear) * (double)orbit_scale;
    double scale = r_sqrt / r_linear;
    *out_x = (float)(x * scale);
    *out_y = (float)(y * scale);
    *out_z = (float)(z * scale);
}

/* ──────────────────────────────────────────────────────────────────────────
 * otp_default_config
 * ────────────────────────────────────────────────────────────────────── */

otp_config_t otp_default_config(void)
{
    otp_config_t cfg;
    cfg.orbit_scale      = 3.0f;
    cfg.points_per_orbit = OTP_POINTS_PER_ORBIT;
    cfg.fade_fraction    = 0.85f;
    cfg.line_alpha_max   = 0.6f;
    return cfg;
}

/* ──────────────────────────────────────────────────────────────────────────
 * otp_orbit_point — single orbit point at J2000.0
 * ────────────────────────────────────────────────────────────────────── */

void otp_orbit_point(int planet_index, double mean_anomaly_rad,
                     float orbit_scale, float xyz[3])
{
    if (planet_index < 0 || planet_index >= OTP_PLANET_COUNT) {
        xyz[0] = 0.0f;
        xyz[1] = 0.0f;
        xyz[2] = 0.0f;
        return;
    }

    /* Use J2000.0 elements */
    orbital_elements_t elem = orbit_elements_at(OTP_ORBITS[planet_index],
                                                 2451545.0);

    double ex, ey, ez;
    orbit_cartesian(&elem, mean_anomaly_rad, &ex, &ey, &ez);
    apply_sqrt_scale(ex, ey, ez, orbit_scale, &xyz[0], &xyz[1], &xyz[2]);
}

/* ──────────────────────────────────────────────────────────────────────────
 * otp_pack_planet — pack orbit trail for a single planet
 * ────────────────────────────────────────────────────────────────────── */

int otp_pack_planet(int planet_index, double jd, otp_config_t config,
                    float *out_verts, int max_verts)
{
    if (planet_index < 0 || planet_index >= OTP_PLANET_COUNT) {
        return 0;
    }
    if (max_verts <= 0) {
        return 0;
    }

    /* Clamp points_per_orbit */
    int points = config.points_per_orbit;
    if (points < 1) points = 1;
    if (points > max_verts) points = max_verts;

    /* Get orbital elements at the given JD */
    orbital_elements_t elem = orbit_elements_at(OTP_ORBITS[planet_index], jd);

    /* Planet's current mean anomaly (radians) */
    double M_current = fmod(elem.L - elem.omega_p, 360.0);
    if (M_current < 0.0) M_current += 360.0;
    double M_current_rad = M_current * DEG_TO_RAD;

    /* Planet color */
    planet_color_t color = planet_data_color(planet_index);

    /* Fade parameters */
    float fade_frac = config.fade_fraction;
    if (fade_frac < 0.0f) fade_frac = 0.0f;
    if (fade_frac > 1.0f) fade_frac = 1.0f;

    for (int s = 0; s < points; s++) {
        /* Parameter t goes from 0 (planet position) to 1 (one full orbit behind) */
        double t = (double)s / (double)points;

        /* Mean anomaly for this sample: start at current, go backward */
        double M_sample = M_current_rad - t * 2.0 * PI;

        /* Compute ecliptic Cartesian position */
        double ex, ey, ez;
        orbit_cartesian(&elem, M_sample, &ex, &ey, &ez);

        /* Apply sqrt scaling */
        float px, py, pz;
        apply_sqrt_scale(ex, ey, ez, config.orbit_scale, &px, &py, &pz);

        int vidx = s * OTP_VERTEX_FLOATS;

        /* Position */
        out_verts[vidx + 0] = px;
        out_verts[vidx + 1] = py;
        out_verts[vidx + 2] = pz;

        /* Alpha: linear fade from line_alpha_max at t=0 to 0 at t=fade_fraction.
         * Beyond fade_fraction, alpha is 0. */
        float alpha = 0.0f;
        if (fade_frac > 0.0f && (float)t <= fade_frac) {
            alpha = config.line_alpha_max * (1.0f - (float)t / fade_frac);
        }
        out_verts[vidx + 3] = alpha;

        /* Color */
        out_verts[vidx + 4] = color.r;
        out_verts[vidx + 5] = color.g;
        out_verts[vidx + 6] = color.b;
    }

    return points;
}

/* ──────────────────────────────────────────────────────────────────────────
 * otp_pack — pack orbit trails for all planets
 * ────────────────────────────────────────────────────────────────────── */

otp_info_t otp_pack(double jd, otp_config_t config, float *out_verts)
{
    otp_info_t info;
    info.vertex_count = 0;
    info.orbit_count = 0;

    /* Clamp points_per_orbit to avoid exceeding buffer */
    int points = config.points_per_orbit;
    if (points < 1) points = 1;
    int max_per_planet = OTP_MAX_VERTICES / OTP_PLANET_COUNT;
    if (points > max_per_planet) points = max_per_planet;
    config.points_per_orbit = points;

    for (int i = 0; i < OTP_PLANET_COUNT; i++) {
        int remaining = OTP_MAX_VERTICES - info.vertex_count;
        if (remaining <= 0) break;

        float *dest = out_verts + info.vertex_count * OTP_VERTEX_FLOATS;
        int count = otp_pack_planet(i, jd, config, dest, remaining);
        info.vertex_count += count;
        if (count > 0) {
            info.orbit_count++;
        }
    }

    return info;
}

/* ──────────────────────────────────────────────────────────────────────────
 * GLSL ES 3.00 shader source strings
 * ────────────────────────────────────────────────────────────────────── */

/* Orbit trail vertex shader.
 * Uniforms: u_mvp (mat4).
 * Attributes: a_position (vec3), a_alpha (float), a_color (vec3).
 * Passes alpha and color to fragment shader. */
static const char OTP_VERT[] =
    "#version 300 es\n"
    "precision mediump float;\n"
    "\n"
    "uniform mat4 u_mvp;\n"
    "\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in float a_alpha;\n"
    "layout(location = 2) in vec3 a_color;\n"
    "\n"
    "out float v_alpha;\n"
    "out vec3 v_color;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "    v_alpha = a_alpha;\n"
    "    v_color = a_color;\n"
    "}\n";

/* Orbit trail fragment shader.
 * Simple alpha-blended output from vertex data. */
static const char OTP_FRAG[] =
    "#version 300 es\n"
    "precision mediump float;\n"
    "\n"
    "in float v_alpha;\n"
    "in vec3 v_color;\n"
    "out vec4 frag_color;\n"
    "\n"
    "void main() {\n"
    "    frag_color = vec4(v_color, v_alpha);\n"
    "}\n";

const char *otp_vert_source(void)
{
    return OTP_VERT;
}

const char *otp_frag_source(void)
{
    return OTP_FRAG;
}
