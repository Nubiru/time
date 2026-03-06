#include "galaxy_geometry.h"

#include <math.h>

#define PI 3.14159265358979323846

/* Milky Way pitch angle ~12 degrees */
#define PITCH_DEG 12.0f
/* Galactic year ~225 million Earth years */
#define GALACTIC_YEAR 225000000.0

/* Sun sits on arm 1 (Sagittarius-Carina), offset ~50 degrees from arm start */
#define SUN_BASE_ANGLE_DEG 50.0f

float galaxy_pitch_angle(void)
{
    return PITCH_DEG;
}

double galaxy_year(void)
{
    return GALACTIC_YEAR;
}

float galaxy_sun_angle(float rotation_deg)
{
    float a = SUN_BASE_ANGLE_DEG + rotation_deg;
    a = fmodf(a, 360.0f);
    if (a < 0.0f) a += 360.0f;
    return a;
}

galaxy_mesh_t galaxy_generate(float radius, float sun_distance,
                               float rotation_deg, int points_per_arm)
{
    galaxy_mesh_t mesh;

    if (points_per_arm < 1) points_per_arm = 1;
    if (points_per_arm > GALAXY_MAX_ARM_POINTS)
        points_per_arm = GALAXY_MAX_ARM_POINTS;

    /* Logarithmic spiral: r = r_start * e^(b * theta)
     * b = tan(pitch_angle) */
    float pitch_rad = PITCH_DEG * (float)PI / 180.0f;
    float b = tanf(pitch_rad);

    /* Start radius: 10% of galaxy radius (avoids galactic bulge) */
    float r_start = radius * 0.1f;

    /* Max theta: solve r_start * e^(b * theta_max) = radius
     * theta_max = ln(radius / r_start) / b = ln(10) / b */
    float theta_max = logf(radius / r_start) / b;

    float rot_rad = rotation_deg * (float)PI / 180.0f;

    for (int arm = 0; arm < GALAXY_ARM_COUNT; arm++) {
        galaxy_arm_t *a = &mesh.arms[arm];
        a->count = points_per_arm;

        float arm_offset = (float)arm * (float)PI * 0.5f; /* 90 degrees apart */

        for (int i = 0; i < points_per_arm; i++) {
            float t = (float)i / (float)(points_per_arm - 1);
            float theta = t * theta_max;
            float r = r_start * expf(b * theta);

            float angle = theta + arm_offset + rot_rad;
            float x = r * cosf(angle);
            float z = r * sinf(angle);

            a->positions[i * 3 + 0] = x;
            a->positions[i * 3 + 1] = 0.0f;
            a->positions[i * 3 + 2] = z;

            /* Brightness: peaks at mid-arm, fades at edges */
            float fade = 1.0f - fabsf(2.0f * t - 1.0f);
            a->brightnesses[i] = fade * fade; /* quadratic falloff */
        }
    }

    /* Sun position */
    float sun_angle_deg = galaxy_sun_angle(rotation_deg);
    float sun_rad = sun_angle_deg * (float)PI / 180.0f;
    mesh.sun_x = sun_distance * cosf(sun_rad);
    mesh.sun_y = 0.0f;
    mesh.sun_z = sun_distance * sinf(sun_rad);

    return mesh;
}
