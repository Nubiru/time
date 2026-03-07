/* megalithic.h — Megalithic site alignment data and celestial azimuth computation.
 *
 * Provides a catalog of ancient megalithic sites with their known astronomical
 * alignments, plus functions to compute sunrise/sunset/moonrise azimuths from
 * latitude and celestial declination. All functions are pure (no GL, no malloc,
 * no globals, no side effects).
 *
 * Azimuth convention: degrees from north, clockwise (east = 90).
 * Flat-horizon assumption: no terrain elevation correction applied. */

#ifndef TIME_MEGALITHIC_H
#define TIME_MEGALITHIC_H

#define PI 3.14159265358979323846

#define MEG_MAX_SITES 8

/* Earth's obliquity of the ecliptic at J2000 (degrees) */
#define MEG_OBLIQUITY 23.4393

/* Moon's orbital inclination to the ecliptic (degrees) */
#define MEG_LUNAR_INCLINATION 5.145

/* Celestial event that a site aligns with */
typedef enum {
    MEG_EVENT_SUMMER_SOLSTICE_SUNRISE = 0,
    MEG_EVENT_SUMMER_SOLSTICE_SUNSET,
    MEG_EVENT_WINTER_SOLSTICE_SUNRISE,
    MEG_EVENT_WINTER_SOLSTICE_SUNSET,
    MEG_EVENT_EQUINOX_SUNRISE,
    MEG_EVENT_EQUINOX_SUNSET,
    MEG_EVENT_LUNAR_STANDSTILL_NORTH,
    MEG_EVENT_LUNAR_STANDSTILL_SOUTH,
    MEG_EVENT_COUNT
} meg_event_t;

/* Megalithic site data */
typedef struct {
    const char *name;
    const char *location;
    double lat;
    double lon;
    double alignment_az;    /* degrees from north */
    meg_event_t event;
    int epoch_bce;
    double tolerance_deg;   /* angular tolerance for "aligned" */
} meg_site_t;

/* Alignment check result */
typedef struct {
    int aligned;
    double event_az;
    double site_az;
    double delta_deg;
} meg_alignment_t;

/* Return number of sites in catalog */
int meg_site_count(void);

/* Get site by index (0-based). Returns sentinel with NULL name if invalid. */
meg_site_t meg_site_get(int index);

/* Human-readable name for a celestial event. Returns "Unknown" for invalid. */
const char *meg_event_name(meg_event_t event);

/* Sunrise azimuth (degrees from north) for a given observer latitude and
 * solar/lunar declination (both in degrees). Uses flat-horizon formula:
 *   Az = acos(sin(dec) / cos(lat))
 * Returns 0.0 for midnight sun (ratio > 1), 180.0 for polar night (ratio < -1). */
double meg_sunrise_azimuth(double lat_deg, double declination_deg);

/* Sunset azimuth (degrees from north). Always 360 - sunrise azimuth. */
double meg_sunset_azimuth(double lat_deg, double declination_deg);

/* Lunar major standstill azimuth. Moon reaches max declination
 * +/-(obliquity + 5.145) = +/-28.5843 degrees.
 * north=1: moonrise azimuth at +28.5843 dec (northeast direction)
 * north=0: moonset azimuth at -28.5843 dec (southwest direction) */
double meg_lunar_standstill_azimuth(double lat_deg, int north);

/* Solar declination at solstices: summer=1 -> +23.4393, summer=0 -> -23.4393 */
double meg_solstice_declination(int summer);

/* Solar declination at equinox: 0.0 */
double meg_equinox_declination(void);

/* Check if a site's alignment matches the current sky at Julian Day jd.
 * Uses se_solar_declination(jd) for sun-based events.
 * Returns alignment result with delta and aligned flag. */
meg_alignment_t meg_check_alignment(int site_index, double jd);

/* Find all sites currently aligned. Writes aligned site indices to out[],
 * up to max_out. Returns count of aligned sites. */
int meg_aligned_now(double jd, int *out, int max_out);

/* Approximate days until next alignment for a solstice/equinox site.
 * Scans forward by day from jd looking for the event.
 * Returns -1 for lunar standstill events or invalid index. */
int meg_days_to_alignment(int site_index, double jd);

#endif /* TIME_MEGALITHIC_H */
