/* sky_dome.c — Sky hemisphere mesh for Earth View.
 * Pure functions: hemisphere mesh generation, atmospheric color per vertex,
 * GLSL shader sources. Uses earth_atmosphere.h for physically-based color. */

#include "sky_dome.h"
#include "earth_atmosphere.h"
#include <math.h>
#include <stddef.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

/* =========================================================
 *  Helpers
 * ========================================================= */

static double clamp_d(double v, double lo, double hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static float clamp_f(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* =========================================================
 *  Default configuration
 * ========================================================= */

sky_dome_config_t sky_dome_default_config(void)
{
    sky_dome_config_t cfg;
    cfg.dome_radius      = 10.0f;
    cfg.sun_altitude_deg = 45.0;
    cfg.sun_azimuth_deg  = 180.0;  /* south */
    return cfg;
}

/* =========================================================
 *  Counts
 * ========================================================= */

int sky_dome_vertex_count(void)
{
    return SKY_DOME_MAX_VERTICES;
}

int sky_dome_index_count(void)
{
    return SKY_DOME_MAX_INDICES;
}

/* =========================================================
 *  Sky color at a specific direction
 * ========================================================= */

void sky_dome_color_at(double altitude_deg,
                        double angular_distance_to_sun_deg,
                        double sun_altitude_deg,
                        float rgb[3])
{
    /* Clamp inputs */
    altitude_deg = clamp_d(altitude_deg, 0.0, 90.0);
    angular_distance_to_sun_deg = clamp_d(angular_distance_to_sun_deg, 0.0, 180.0);
    sun_altitude_deg = clamp_d(sun_altitude_deg, -90.0, 90.0);

    /* Base color from earth_atmo_color (sea level observer) */
    atmo_color_t base = earth_atmo_color(0.0, sun_altitude_deg);

    float r = base.r;
    float g = base.g;
    float b = base.b;

    /* Altitude modulation: zenith is deeper blue, horizon is lighter/hazier */
    double alt_factor = altitude_deg / 90.0;

    /* Deepen blue toward zenith during daytime */
    if (sun_altitude_deg > 0.0) {
        float zenith_boost = (float)(alt_factor * 0.3);
        b = clamp_f(b + zenith_boost, 0.0f, 1.0f);
        /* Slightly reduce red/green at zenith for deeper blue */
        r = clamp_f(r - (float)(alt_factor * 0.1), 0.0f, 1.0f);
        g = clamp_f(g - (float)(alt_factor * 0.05), 0.0f, 1.0f);
    }

    /* Sun proximity brightening: closer to sun => brighter and warmer */
    if (angular_distance_to_sun_deg < 60.0 && sun_altitude_deg > -6.0) {
        double proximity = 1.0 - (angular_distance_to_sun_deg / 60.0);
        proximity = proximity * proximity; /* quadratic falloff */

        /* Sun glow: warm brightening */
        double sun_strength = clamp_d(
            (sun_altitude_deg + 6.0) / 96.0, 0.0, 1.0);
        float glow = (float)(proximity * sun_strength * 0.4);

        r = clamp_f(r + glow * 1.0f, 0.0f, 1.0f);
        g = clamp_f(g + glow * 0.7f, 0.0f, 1.0f);
        b = clamp_f(b + glow * 0.3f, 0.0f, 1.0f);
    }

    /* Horizon sunset/sunrise blending when sun is low */
    if (altitude_deg < 15.0 && sun_altitude_deg > -12.0 && sun_altitude_deg < 10.0) {
        double horizon_factor = 1.0 - (altitude_deg / 15.0);
        atmo_color_t sunset = earth_atmo_sunset_color(sun_altitude_deg);

        /* Blend toward sunset colors at horizon */
        double blend = horizon_factor * 0.6;
        r = clamp_f((float)(r * (1.0 - blend) + sunset.r * blend), 0.0f, 1.0f);
        g = clamp_f((float)(g * (1.0 - blend) + sunset.g * blend), 0.0f, 1.0f);
        b = clamp_f((float)(b * (1.0 - blend) + sunset.b * blend), 0.0f, 1.0f);
    }

    /* Night: when sun is well below horizon, everything goes dark */
    if (sun_altitude_deg < -6.0) {
        double night_factor = clamp_d(
            (-sun_altitude_deg - 6.0) / 12.0, 0.0, 1.0);
        double dark = 1.0 - night_factor;
        r = (float)(r * dark);
        g = (float)(g * dark);
        b = (float)(b * dark);

        /* Slight blue tint to night sky */
        b = clamp_f(b + (float)(night_factor * 0.02), 0.0f, 1.0f);
    }

    rgb[0] = clamp_f(r, 0.0f, 1.0f);
    rgb[1] = clamp_f(g, 0.0f, 1.0f);
    rgb[2] = clamp_f(b, 0.0f, 1.0f);
}

/* =========================================================
 *  Hemisphere mesh generation
 * ========================================================= */

/* Compute angular distance between two directions given as (alt, az) in degrees.
 * Uses dot product of unit vectors on the sphere. */
static double angular_distance_deg(double alt1, double az1,
                                     double alt2, double az2)
{
    double a1 = alt1 * PI / 180.0;
    double z1 = az1 * PI / 180.0;
    double a2 = alt2 * PI / 180.0;
    double z2 = az2 * PI / 180.0;

    /* Convert alt/az to unit vectors (Y-up) */
    double x1 = cos(a1) * sin(z1);
    double y1 = sin(a1);
    double z1v = cos(a1) * cos(z1);

    double x2 = cos(a2) * sin(z2);
    double y2 = sin(a2);
    double z2v = cos(a2) * cos(z2);

    double dot = x1 * x2 + y1 * y2 + z1v * z2v;
    dot = clamp_d(dot, -1.0, 1.0);

    return acos(dot) * 180.0 / PI;
}

sky_dome_info_t sky_dome_generate(sky_dome_config_t config,
                                   float *out_vertices,
                                   unsigned int *out_indices)
{
    sky_dome_info_t info;
    info.vertex_count = SKY_DOME_MAX_VERTICES;
    info.index_count  = SKY_DOME_MAX_INDICES;

    if (!out_vertices || !out_indices) {
        return info;
    }

    float radius = config.dome_radius;
    int alt_steps = SKY_DOME_ALT_STEPS;
    int az_steps  = SKY_DOME_AZ_STEPS;
    int vi = 0;

    /* Generate vertices: hemisphere from altitude 0 (horizon) to 90 (zenith) */
    for (int ai = 0; ai <= alt_steps; ai++) {
        double alt_deg = 90.0 * (double)ai / (double)alt_steps;
        double alt_rad = alt_deg * PI / 180.0;

        float cos_alt = (float)cos(alt_rad);
        float sin_alt = (float)sin(alt_rad);

        for (int zi = 0; zi <= az_steps; zi++) {
            double az_deg = 360.0 * (double)zi / (double)az_steps;
            double az_rad = az_deg * PI / 180.0;

            float sin_az = (float)sin(az_rad);
            float cos_az = (float)cos(az_rad);

            /* Position on hemisphere (Y-up, observer at origin) */
            float x = cos_alt * sin_az * radius;
            float y = sin_alt * radius;
            float z = cos_alt * cos_az * radius;

            /* Compute angular distance to sun for coloring */
            double ang_dist = angular_distance_deg(
                alt_deg, az_deg,
                config.sun_altitude_deg, config.sun_azimuth_deg);

            /* Get sky color at this direction */
            float rgb[3];
            sky_dome_color_at(alt_deg, ang_dist, config.sun_altitude_deg, rgb);

            /* UV coordinates */
            float u = (float)(az_deg / 360.0);
            float v = (float)(alt_deg / 90.0);

            /* Pack: position(3) + color(3) + uv(2) */
            int base = vi * SKY_DOME_VERT_FLOATS;
            out_vertices[base + 0] = x;
            out_vertices[base + 1] = y;
            out_vertices[base + 2] = z;
            out_vertices[base + 3] = rgb[0];
            out_vertices[base + 4] = rgb[1];
            out_vertices[base + 5] = rgb[2];
            out_vertices[base + 6] = u;
            out_vertices[base + 7] = v;

            vi++;
        }
    }

    /* Generate triangle indices (inside-out: CW winding for inward-facing) */
    int ii = 0;
    for (int ai = 0; ai < alt_steps; ai++) {
        for (int zi = 0; zi < az_steps; zi++) {
            int row0 = ai * (az_steps + 1);
            int row1 = (ai + 1) * (az_steps + 1);

            /* Triangle 1 (CW from inside) */
            out_indices[ii++] = (unsigned int)(row0 + zi);
            out_indices[ii++] = (unsigned int)(row0 + zi + 1);
            out_indices[ii++] = (unsigned int)(row1 + zi);

            /* Triangle 2 (CW from inside) */
            out_indices[ii++] = (unsigned int)(row0 + zi + 1);
            out_indices[ii++] = (unsigned int)(row1 + zi + 1);
            out_indices[ii++] = (unsigned int)(row1 + zi);
        }
    }

    return info;
}

/* =========================================================
 *  GLSL Shader sources
 * ========================================================= */

const char *sky_dome_vert_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "layout(location = 0) in vec3 a_position;\n"
        "layout(location = 1) in vec3 a_color;\n"
        "layout(location = 2) in vec2 a_uv;\n"
        "\n"
        "uniform mat4 u_mvp;\n"
        "\n"
        "out vec3 v_color;\n"
        "out vec2 v_uv;\n"
        "\n"
        "void main() {\n"
        "    v_color = a_color;\n"
        "    v_uv = a_uv;\n"
        "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
        "}\n";
}

const char *sky_dome_frag_source(void)
{
    return
        "#version 300 es\n"
        "precision highp float;\n"
        "\n"
        "in vec3 v_color;\n"
        "in vec2 v_uv;\n"
        "\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main() {\n"
        "    /* Smooth gradient from vertex colors with slight dithering */\n"
        "    vec3 color = v_color;\n"
        "\n"
        "    /* Horizon fade: blend toward transparent at the very bottom */\n"
        "    float horizon_fade = smoothstep(0.0, 0.05, v_uv.y);\n"
        "    float alpha = horizon_fade;\n"
        "\n"
        "    fragColor = vec4(color, alpha);\n"
        "}\n";
}
