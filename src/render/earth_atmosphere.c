/* earth_atmosphere.c — Earth atmosphere rendering data.
 * Pure functions: Rayleigh scattering color, density model, air mass,
 * optical depth, atmosphere shell mesh, cloud bands, limb glow. */

#include "earth_atmosphere.h"
#include <math.h>
#include <stddef.h>

/* =========================================================
 *  Atmospheric density
 * ========================================================= */

double earth_atmo_density(double altitude_km)
{
    if (altitude_km < 0.0) {
        altitude_km = 0.0;
    }
    return exp(-altitude_km / EARTH_ATMO_SCALE_HEIGHT_KM);
}

/* =========================================================
 *  Air mass (Kasten-Young formula)
 * ========================================================= */

double earth_atmo_air_mass(double elevation_deg)
{
    /* Clamp to small positive to avoid division by zero */
    if (elevation_deg < 0.0) {
        elevation_deg = 0.0;
    }

    /* Kasten-Young 1989 formula:
     * AM = 1 / (sin(el) + 0.50572 * (6.07995 + el)^-1.6364) */
    double el_rad = elevation_deg * PI / 180.0;
    double sin_el = sin(el_rad);
    double denominator = sin_el + 0.50572 * pow(6.07995 + elevation_deg, -1.6364);

    if (denominator < 0.001) {
        return 40.0; /* practical maximum near horizon */
    }

    return 1.0 / denominator;
}

/* =========================================================
 *  Optical depth
 * ========================================================= */

double earth_atmo_optical_depth(double zenith_angle_deg)
{
    /* Optical depth relative to zenith.
     * At zenith (0 deg), OD = 1.0.
     * Elevation = 90 - zenith_angle. */
    if (zenith_angle_deg < 0.0) {
        zenith_angle_deg = 0.0;
    }
    if (zenith_angle_deg > 90.0) {
        zenith_angle_deg = 90.0;
    }

    double elevation_deg = 90.0 - zenith_angle_deg;
    double am = earth_atmo_air_mass(elevation_deg);
    return am; /* optical depth = air mass * tau_zenith, normalized so tau_zenith = 1 */
}

/* =========================================================
 *  Rayleigh scattering sky color
 * ========================================================= */

static double clamp01(double v)
{
    if (v < 0.0) return 0.0;
    if (v > 1.0) return 1.0;
    return v;
}

atmo_color_t earth_atmo_color(double altitude_km, double sun_elevation_deg)
{
    atmo_color_t c;

    /* Base sky color (overhead sun, sea level): blue-dominant Rayleigh scatter */
    double base_r = 0.3;
    double base_g = 0.5;
    double base_b = 0.9;

    /* Altitude factor: density decreases with altitude, sky darkens */
    double density = earth_atmo_density(altitude_km);

    /* Sun elevation factor: more light when sun is higher */
    double sun_factor;
    if (sun_elevation_deg >= 0.0) {
        /* Daytime: brightness scales with sin of elevation */
        double el_rad = sun_elevation_deg * PI / 180.0;
        sun_factor = sin(el_rad);
        if (sun_factor < 0.05) sun_factor = 0.05;
    } else {
        /* Night/twilight: exponential fade below horizon */
        sun_factor = 0.05 * exp(sun_elevation_deg / 10.0);
    }

    /* Rayleigh scattering path effect:
     * At low sun angles, air mass is high, blue is preferentially removed.
     * Shift color toward red at low elevations. */
    double am = 1.0;
    if (sun_elevation_deg > 0.0) {
        am = earth_atmo_air_mass(sun_elevation_deg);
    } else {
        am = 40.0; /* maximum path */
    }

    /* Extinction: each color channel attenuated by exp(-tau * AM)
     * Rayleigh: tau proportional to wavelength^-4
     * Approximate: tau_b ~ 0.12, tau_g ~ 0.06, tau_r ~ 0.03 */
    double ext_r = exp(-0.03 * am);
    double ext_g = exp(-0.06 * am);
    double ext_b = exp(-0.12 * am);

    /* Scattered light (what we see as sky color):
     * Scattered intensity proportional to tau * sun_factor * density
     * For scattered light, higher tau means MORE scattering into our line of sight */
    double scat_r = 0.03 * sun_factor * density;
    double scat_g = 0.06 * sun_factor * density;
    double scat_b = 0.12 * sun_factor * density;

    /* Combine: base color * extinction * density + scattered component */
    c.r = (float)clamp01(base_r * ext_r * density * sun_factor + scat_r);
    c.g = (float)clamp01(base_g * ext_g * density * sun_factor + scat_g);
    c.b = (float)clamp01(base_b * ext_b * density * sun_factor + scat_b);

    return c;
}

/* =========================================================
 *  Sunset / sunrise color
 * ========================================================= */

atmo_color_t earth_atmo_sunset_color(double sun_elevation_deg)
{
    atmo_color_t c;

    if (sun_elevation_deg >= 5.0) {
        /* Above 5 degrees: transition to normal sky */
        double t = clamp01((sun_elevation_deg - 5.0) / 10.0);
        c.r = (float)(0.9 - 0.6 * t);
        c.g = (float)(0.4 - 0.1 * t);
        c.b = (float)(0.1 + 0.6 * t);
    } else if (sun_elevation_deg >= 0.0) {
        /* Golden hour: warm orange/red */
        double t = sun_elevation_deg / 5.0;
        c.r = (float)(0.9 + 0.1 * t);
        c.g = (float)(0.3 + 0.1 * t);
        c.b = (float)(0.05 + 0.05 * t);
    } else if (sun_elevation_deg >= -6.0) {
        /* Civil twilight: orange fading to purple */
        double t = clamp01(-sun_elevation_deg / 6.0);
        c.r = (float)(0.9 - 0.5 * t);
        c.g = (float)(0.3 - 0.2 * t);
        c.b = (float)(0.05 + 0.25 * t);
    } else if (sun_elevation_deg >= -12.0) {
        /* Nautical twilight: deep purple fading to dark */
        double t = clamp01((-sun_elevation_deg - 6.0) / 6.0);
        c.r = (float)(0.4 - 0.35 * t);
        c.g = (float)(0.1 - 0.08 * t);
        c.b = (float)(0.3 - 0.25 * t);
    } else {
        /* Below nautical twilight: near black */
        c.r = 0.05f;
        c.g = 0.02f;
        c.b = 0.05f;
    }

    return c;
}

/* =========================================================
 *  Limb glow
 * ========================================================= */

float earth_atmo_limb_glow(float view_dot_normal)
{
    /* Fresnel-like limb brightening: glow = (1 - dot)^power
     * power ~2.5 gives a nice atmosphere rim effect */
    if (view_dot_normal < 0.0f) view_dot_normal = 0.0f;
    if (view_dot_normal > 1.0f) view_dot_normal = 1.0f;

    float inv = 1.0f - view_dot_normal;
    return inv * inv * inv; /* power = 3 approximation: inv^3 */
}

/* =========================================================
 *  Cloud bands
 * ========================================================= */

/* Static const data table for 7 cloud bands */
static const cloud_band_t cloud_bands[EARTH_CLOUD_BAND_COUNT] = {
    {  5.0f, 10.0f, 0.8f, "ITCZ",                      1 },
    { 30.0f, 15.0f, 0.2f, "N Subtropical High",         0 },
    {-30.0f, 15.0f, 0.2f, "S Subtropical High",         0 },
    { 50.0f, 15.0f, 0.6f, "N Storm Track",              1 },
    {-50.0f, 15.0f, 0.7f, "S Storm Track",              1 },
    { 70.0f, 10.0f, 0.5f, "Arctic Front",               0 },
    {-70.0f, 10.0f, 0.6f, "Antarctic Front",            0 }
};

cloud_band_t earth_cloud_band_get(int index)
{
    if (index < 0 || index >= EARTH_CLOUD_BAND_COUNT) {
        cloud_band_t empty = { 0.0f, 0.0f, 0.0f, NULL, 0 };
        return empty;
    }
    return cloud_bands[index];
}

int earth_cloud_band_count(void)
{
    return EARTH_CLOUD_BAND_COUNT;
}

double earth_cloud_density_at(double lat_deg)
{
    /* Sum Gaussian contributions from each band.
     * Each band: density * exp(-0.5 * ((lat - center) / sigma)^2)
     * sigma = width / 2.0 */
    double total = 0.0;

    for (int i = 0; i < EARTH_CLOUD_BAND_COUNT; i++) {
        double center = (double)cloud_bands[i].lat_center;
        double sigma = (double)cloud_bands[i].lat_width / 2.0;
        double d = (double)cloud_bands[i].density;
        double diff = lat_deg - center;
        double gauss = exp(-0.5 * (diff * diff) / (sigma * sigma));
        total += d * gauss;
    }

    /* Clamp to [0, 1] */
    if (total > 1.0) total = 1.0;
    if (total < 0.0) total = 0.0;

    return total;
}

double earth_cloud_itcz_offset(int month)
{
    /* Clamp month */
    if (month < 1) month = 1;
    if (month > 12) month = 12;

    /* ITCZ migrates: Jan ~5N, Jul ~15N (annual mean ~5N)
     * Model: offset = amplitude * sin((month - 4) * PI / 6)
     * amplitude ~ 5 degrees (range from 0N to 10N offset from mean)
     * At month 1 (Jan): sin((1-4)*PI/6) = sin(-PI/2) = -1 => offset = -5
     * At month 7 (Jul): sin((7-4)*PI/6) = sin(PI/2) = 1 => offset = +5
     * Mean ITCZ at 5N + offset => Jan: 0N-5N, Jul: 10N-15N
     */
    double phase = ((double)month - 4.0) * PI / 6.0;
    return 5.0 * sin(phase);
}

/* =========================================================
 *  Atmosphere shell mesh generation
 * ========================================================= */

int earth_atmo_shell_vertex_count(void)
{
    return EARTH_ATMO_MAX_VERTICES;
}

int earth_atmo_shell_index_count(void)
{
    return EARTH_ATMO_MAX_INDICES;
}

atmo_mesh_info_t earth_atmo_shell_generate(double radius_ratio,
                                            atmo_vertex_t *out_vertices,
                                            unsigned int *out_indices)
{
    atmo_mesh_info_t info;
    info.vertex_count = EARTH_ATMO_MAX_VERTICES;
    info.index_count = EARTH_ATMO_MAX_INDICES;

    if (!out_vertices || !out_indices) {
        return info;
    }

    float r = (float)radius_ratio;
    int lat_seg = EARTH_ATMO_LAT_SEGMENTS;
    int lon_seg = EARTH_ATMO_LON_SEGMENTS;
    int vi = 0;

    /* Generate vertices: UV-sphere at radius_ratio */
    for (int lat = 0; lat <= lat_seg; lat++) {
        double theta = PI * (double)lat / (double)lat_seg; /* 0 to PI */
        float sin_theta = (float)sin(theta);
        float cos_theta = (float)cos(theta);

        /* Latitude in degrees for opacity calculation */
        double lat_deg = 90.0 - (theta * 180.0 / PI);
        double density = earth_atmo_density(0.0); /* shell is at outer edge */
        /* Opacity varies: thicker at limb (lower latitudes in view),
         * use a base density scaled by a simple profile */
        double opacity_val = density * 0.3; /* base transparency for atmosphere shell */
        (void)lat_deg; /* latitude used implicitly via the sphere geometry */

        for (int lon = 0; lon <= lon_seg; lon++) {
            double phi = 2.0 * PI * (double)lon / (double)lon_seg;
            float sin_phi = (float)sin(phi);
            float cos_phi = (float)cos(phi);

            /* Normal = position on unit sphere */
            float nx = sin_theta * cos_phi;
            float ny = cos_theta;
            float nz = sin_theta * sin_phi;

            out_vertices[vi].x = r * nx;
            out_vertices[vi].y = r * ny;
            out_vertices[vi].z = r * nz;
            out_vertices[vi].nx = nx;
            out_vertices[vi].ny = ny;
            out_vertices[vi].nz = nz;
            out_vertices[vi].opacity = (float)opacity_val;
            vi++;
        }
    }

    /* Generate triangle indices */
    int ii = 0;
    for (int lat = 0; lat < lat_seg; lat++) {
        for (int lon = 0; lon < lon_seg; lon++) {
            int row0 = lat * (lon_seg + 1);
            int row1 = (lat + 1) * (lon_seg + 1);

            /* Triangle 1 */
            out_indices[ii++] = (unsigned int)(row0 + lon);
            out_indices[ii++] = (unsigned int)(row1 + lon);
            out_indices[ii++] = (unsigned int)(row0 + lon + 1);

            /* Triangle 2 */
            out_indices[ii++] = (unsigned int)(row0 + lon + 1);
            out_indices[ii++] = (unsigned int)(row1 + lon);
            out_indices[ii++] = (unsigned int)(row1 + lon + 1);
        }
    }

    return info;
}
