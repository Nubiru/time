/* earth_view_frame.c — Earth View per-frame data aggregator.
 *
 * Collects camera, sun, sky, and horizon data into a single ev_frame_t
 * struct for efficient rendering.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "earth_view_frame.h"
#include "../systems/astronomy/solar_events.h"

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define DEG2RAD (PI / 180.0)
#define RAD2DEG (180.0 / PI)

/* Compute sun azimuth from declination, latitude, hour angle, and altitude.
 * Uses the standard astronomical formula:
 *   cos(A) = (sin(dec) - sin(lat) * sin(alt)) / (cos(lat) * cos(alt))
 * Azimuth measured from south in the astronomical convention, then converted
 * to compass bearing (0=N, 90=E, 180=S, 270=W). */
static float compute_sun_azimuth(double lat_deg, double dec_deg,
                                  double hour_angle_deg, double alt_deg)
{
    double lat_r = lat_deg * DEG2RAD;
    double dec_r = dec_deg * DEG2RAD;
    double alt_r = alt_deg * DEG2RAD;

    double cos_alt = cos(alt_r);
    if (fabs(cos_alt) < 1e-10) {
        /* Sun at zenith or nadir — azimuth undefined, default south */
        return 180.0f;
    }

    double cos_az = (sin(dec_r) - sin(lat_r) * sin(alt_r))
                   / (cos(lat_r) * cos_alt);

    /* Clamp for numerical safety */
    if (cos_az > 1.0)  cos_az = 1.0;
    if (cos_az < -1.0) cos_az = -1.0;

    double az_astro = acos(cos_az) * RAD2DEG; /* 0-180, measured from south */

    /* If hour angle > 0 (afternoon), sun is in the west => azimuth > 180 */
    /* Compass conversion: compass = 180 + astro_az (if HA <= 0) or 180 - astro_az (if HA > 0) */
    /* Actually: astronomical azimuth from south, clockwise. */
    /* Compass from north: az_compass = az_astro + 180 (mod 360) when HA <= 0 */
    /* When HA > 0 (west): az_compass = 360 - az_astro + 180 ... */
    /* Simpler: compass_az = (HA > 0) ? (360 - az_astro) : az_astro */
    /* But this gives azimuth from south. Convert to from-north: add 180, mod 360. */

    /* Standard formula gives azimuth from south. For compass (from north): */
    double compass_az;
    if (hour_angle_deg > 0.0) {
        /* Afternoon: sun in west half */
        compass_az = 360.0 - az_astro;
    } else {
        /* Morning: sun in east half */
        compass_az = az_astro;
    }

    /* The formula above gives azimuth from south. Shift to north-based. */
    compass_az = fmod(compass_az + 180.0, 360.0);

    return (float)compass_az;
}

/* Compute sun hour angle in degrees for given JD and location.
 * Replicates the logic from se_solar_altitude for consistency. */
static double compute_sun_hour_angle(double jd, double lon_deg)
{
    double eot = se_equation_of_time(jd);
    double noon = floor(jd - 0.5) + 0.5;
    double solar_noon = noon - lon_deg / 360.0 - eot / (60.0 * 24.0);
    return (jd - solar_noon) * 360.0;
}

ev_frame_t ev_compute_frame(double jd,
                             double observer_lat, double observer_lon,
                             float look_azimuth, float look_elevation,
                             float fov, float aspect, float globe_radius)
{
    ev_frame_t f;

    /* ---- Camera ---- */
    horizon_camera_t cam = hcam_create(observer_lat, observer_lon, globe_radius);
    cam.look_azimuth = look_azimuth;
    cam.look_elevation = look_elevation;
    cam.fov = fov;
    cam.aspect = aspect;

    f.view_matrix = hcam_view_matrix(&cam);
    f.proj_matrix = hcam_projection_matrix(&cam);
    hcam_position(&cam, f.camera_pos);
    hcam_up_direction(&cam, f.camera_up);
    hcam_look_direction(&cam, f.camera_look);

    /* ---- Sun position ---- */
    f.sun_altitude_deg = (float)se_solar_altitude(jd, observer_lat, observer_lon);

    double sun_dec = se_solar_declination(jd);
    double sun_ha = compute_sun_hour_angle(jd, observer_lon);

    f.sun_azimuth_deg = compute_sun_azimuth(observer_lat, sun_dec,
                                             sun_ha, (double)f.sun_altitude_deg);
    f.is_daytime = f.sun_altitude_deg > 0.0f ? 1 : 0;

    /* ---- Sky dome config ---- */
    f.sky_config.dome_radius = globe_radius * 2.0f;
    f.sky_config.sun_altitude_deg = (double)f.sun_altitude_deg;
    f.sky_config.sun_azimuth_deg = (double)f.sun_azimuth_deg;

    /* ---- Horizon cardinals ---- */
    hzg_config_t hcfg = hzg_default_config();
    hcfg.observer_lat_deg = observer_lat;
    hcfg.observer_lon_deg = observer_lon;
    hcfg.globe_radius = globe_radius;
    f.cardinal_count = hzg_cardinals(hcfg, f.cardinals);

    /* ---- Objects (Sun only for v1) ---- */
    f.object_count = 1;
    f.objects[0].altitude_deg = f.sun_altitude_deg;
    f.objects[0].azimuth_deg = f.sun_azimuth_deg;
    f.objects[0].brightness = 1.0f;
    f.objects[0].is_visible = f.is_daytime;
    f.objects[0].name = "Sun";

    return f;
}

ev_frame_t ev_compute_simple(double jd,
                              double observer_lat, double observer_lon,
                              float look_azimuth, float look_elevation)
{
    float default_fov = (float)(PI / 2.0);    /* 90 degrees */
    float default_aspect = 16.0f / 9.0f;
    float default_radius = 1.0f;

    return ev_compute_frame(jd, observer_lat, observer_lon,
                             look_azimuth, look_elevation,
                             default_fov, default_aspect, default_radius);
}

int ev_is_daytime(const ev_frame_t *frame)
{
    return frame->is_daytime;
}

void ev_sun_position(const ev_frame_t *frame, float *alt_deg, float *az_deg)
{
    *alt_deg = frame->sun_altitude_deg;
    *az_deg = frame->sun_azimuth_deg;
}
